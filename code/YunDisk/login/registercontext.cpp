#include "registercontext.h"
#include "login.h"
#include "common/global.h"
#include "common/network_manager.h"
#include <QRegularExpression>
#include <QMessageBox>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QNetworkReply>


RegisterContext::RegisterContext(const QRect &rect, QWidget *parent)
    : QWidget{parent}

{
    this->initScene(rect);
    this->initShowData();
    connect(&this->m_button_register, &QToolButton::clicked, this, &RegisterContext::on_button_registe_clicked);
}

RegisterContext::~RegisterContext()
{

}

void RegisterContext::initScene(const QRect &rect)
{
    int iHeight=rect.height(), iWidget=rect.width();
    this->setGeometry(rect);


    this->setFont(QFont("Microsoft YaHei UI", 9, QFont::Bold, false));
    // 设置用户登录标签
    this->m_title.setParent(this);
    this->m_title.setText("用户注册");
    this->m_title.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_title.setFont(QFont("华文新魏", 26, QFont::Bold, false));
    this->m_title.setGeometry(iWidget/7*2, 0, 200, 50);    // 带了show()

    this->m_username.setParent(this);
    this->m_username.setText("用户名:");
    this->m_username.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_username.setGeometry(iWidget/7*2, iHeight/10*2, 60, 25);
    this->m_usertext.setParent(this);
    this->m_usertext.setGeometry(iWidget/7*3, iHeight/10*2, 133, 25);

    this->m_nickname.setParent(this);
    this->m_nickname.setText("昵称:");
    this->m_nickname.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_nickname.setGeometry(iWidget/7*2, iHeight/10*3, 60, 25);
    this->m_nicktext.setParent(this);
    this->m_nicktext.setGeometry(iWidget/7*3, iHeight/10*3, 133, 25);

    this->m_password.setParent(this);
    this->m_password.setText("密码:");
    this->m_password.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_password.setGeometry(iWidget/7*2,iHeight/10*4, 60, 25);
    this->m_passtext.setParent(this);
    this->m_passtext.setGeometry(iWidget/7*3, iHeight/10*4, 133, 25);

    this->m_confirm_password.setParent(this);
    this->m_confirm_password.setText("确认密码:");
    this->m_confirm_password.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_confirm_password.setGeometry(iWidget/7*2,iHeight/10*5, 60, 25);
    this->m_confirm_text.setParent(this);
    this->m_confirm_text.setGeometry(iWidget/7*3, iHeight/10*5, 133, 25);

    this->m_phone.setParent(this);
    this->m_phone.setText("手机:");
    this->m_phone.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_phone.setGeometry(iWidget/7*2,iHeight/10*6, 60, 25);
    this->m_phone_text.setParent(this);
    this->m_phone_text.setGeometry(iWidget/7*3, iHeight/10*6, 133, 25);

    this->m_email.setParent(this);
    this->m_email.setText("邮箱:");
    this->m_email.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_email.setGeometry(iWidget/7*2,iHeight/10*7, 60, 25);
    this->m_email_text.setParent(this);
    this->m_email_text.setGeometry(iWidget/7*3, iHeight/10*7, 133, 25);

    this->m_button_register.setParent(this);
    this->m_button_register.setText("注册");
    this->m_button_register.setGeometry(iWidget/7*2, iHeight-60 , 200, 50);
    this->m_button_register.setStyleSheet("background-image: url(:/images/balckButton.png);font: 75 18pt \"新宋体\";color: rgb(255, 255, 255);");
}

void RegisterContext::initShowData()
{
    this->m_usertext.setFocus();
}

bool RegisterContext::sendRegisterMessage(const RegisterInfo& info)
{
    Login* login = nullptr;
    QNetworkAccessManager& manager = NetworkManager::getNetManager();
    QByteArray array = NetworkManager::setRegisterJson(info);
    WinPrintA << "register json data" << array;


    login = dynamic_cast<Login*>(this->parent());
    if (!login)
        login = dynamic_cast<Login*>(this->parent()->parent());
    if (!login)
        return false;
    const ServerInfo& server = login->getInfoContext().getServerInfo();

    // 设置连接服务器要发送的url
    QNetworkRequest request;
    request.setUrl(QUrl(QString("http://%1:%2/reg").arg(server.ip).arg(server.port)));
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, array.size());
    // 设置请求体
    //QNetworkReply* reply 
    QNetworkReply* reply = manager.post(request, array);

    // 判断请求是否被成功处理
    connect(reply, &QNetworkReply::readyRead, [=](){
        /*
        注册 - server端返回的json格式数据：
            成功:         {"code":"002"}
            该用户已存在：  {"code":"003"}
            失败:         {"code":"004"}
        */
        WinPrintA << "====================";
        if (reply->error() != QNetworkReply::NoError)
        {
            WinPrintA << reply->errorString();
            //释放资源
            reply->deleteLater();
            return;
        }
        QByteArray jsonData = reply->readAll();
        QString recvCode = NetworkManager::getRegisterStatus(jsonData);
        if ("002" == recvCode)
        {   // 注册成功
            QMessageBox::information(this, "注册成功", "注册成功，请登录");
            WinPrintA << "user " << info.username << " create success";
            login->getInfoContext().setLoginInfo(info.username, info.firstpwd);
            login->getInfoContext().WriteConfContext();
            emit login->closeWindow();
        }
        else if ("003" == recvCode)
        {
            QMessageBox::warning(this, "注册失败", QString("[%1]该用户已经存在!!!").arg(info.username));
            WinPrintA << "user " << info.username << " create faild, user existence";
        }else
        {
            QMessageBox::warning(this, "注册失败", "注册失败！！！");
            WinPrintA << "user " << info.username << " create faild!";
        }
        // delete reply;  // delete reply释放了 reply，但 Qt仍然可能在后续信号中访问 reply导致程序崩溃如 finished
        reply->deleteLater();
        WinPrintA << "user " << info.username << " create faild!";
    });

    /*
    connect(reply, &QNetworkReply::finished,[=]() {
        reply->deleteLater();
    });
    */
    return true;
}


void RegisterContext::on_button_registe_clicked()
{

    RegisterInfo info = { 0 };

    info.username = this->m_usertext.text();
    info.nickname = this->m_nicktext.text();
    info.firstpwd = this->m_passtext.text();
    info.surepwd = this->m_confirm_text.text();
    info.phone = this->m_phone_text.text();
    info.email = this->m_email_text.text();

    // 密码校验
    QRegularExpression regexp(PASSWD_REG);
    if(!regexp.match(info.firstpwd).hasMatch())
    {
        QMessageBox::warning(this, "警告", "密码格式不正确");
        this->m_passtext.clear();
        this->m_confirm_text.clear();
        this->m_passtext.setFocus();
        return;
    }
    if(info.surepwd != info.firstpwd)
    {
        QMessageBox::warning(this, "警告", "两次输入的密码不匹配, 请重新输入");
        this->m_passtext.clear();
        this->m_confirm_text.clear();
        this->m_passtext.setFocus();
        return;
    }
    // 账户校验
    regexp.setPattern(USER_REG);
    if(!regexp.match(info.username).hasMatch())
    {
        QMessageBox::warning(this, "警告", "用户名格式不正确");
        this->m_usertext.clear();
        this->m_usertext.setFocus();
        return;
    }
    if(!regexp.match(info.nickname).hasMatch())
    {
        QMessageBox::warning(this, "警告", "昵称格式不正确");
        this->m_nicktext.clear();
        this->m_nicktext.setFocus();
        return;
    }
    // 手机校验
    regexp.setPattern(PHONE_REG);
    if(!regexp.match(info.phone).hasMatch())
    {
        QMessageBox::warning(this, "警告", "手机号码格式不正确");
        this->m_phone_text.clear();
        this->m_phone_text.setFocus();
        return;
    }
    // 邮箱校验
    regexp.setPattern(EMAIL_REG);
    if(!regexp.match(info.email).hasMatch())
    {
        QMessageBox::warning(this, "警告", "邮箱码格式不正确");
        this->m_email_text.clear();
        this->m_email_text.setFocus();
        return;
    }

    this->sendRegisterMessage(info);
}
