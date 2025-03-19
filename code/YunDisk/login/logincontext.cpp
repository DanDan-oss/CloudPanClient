#include "logincontext.h"
#include "login.h"
#include "common/loginfo.h"
#include "common/global.h"
#include "common/cryptutil.h"
#include "common/network_manager.h"
#include <QRegularExpression>
#include <QJsonObject>>
#include <QJsonDocument>
#include <QMessageBox>
#include <QNetworkReply>

LoginContext::LoginContext(const QRect &rect, QWidget *parent)
    : QWidget{parent}
{
    this->initScene(rect);
    this->initShowData();
    connect(&this->m_button_register, &QToolButton::clicked, this, &LoginContext::on_button_registe_clicked);
    connect(&this->m_button_login,  &QToolButton::clicked, this, &LoginContext::on_button_login_clicked);
}

LoginContext::~LoginContext()
{

}


void LoginContext::paintEvent(QPaintEvent* event)
{
    return QWidget::paintEvent(event);
}


void LoginContext::initScene(const QRect &rect)
{
    int iHeight=rect.height(), iWidget=rect.width();

    this->setGeometry(rect);
    // 设置用户登录标签
    this->m_title.setParent(this);
    this->m_title.setText("用户登录");
    this->m_title.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_title.setFont(QFont("新宋体", 24, QFont::Bold, false));
    this->m_title.setGeometry(iWidget/7*2, 0, 200, 50);    // 带了show()

    this->m_username.setParent(this);
    this->m_username.setText("用户名:");
    this->m_username.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_username.setGeometry(iWidget/8, iHeight/5*1, 100, 30);

    this->m_password.setParent(this);
    this->m_password.setText("密码:");
    this->m_password.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_password.setGeometry(iWidget/8,iHeight/5*2, 100, 30);

    this->m_usertext.setParent(this);
    this->m_usertext.setGeometry(iWidget/8*3, iHeight/5*1, 200, 30);
    this->m_usertext.setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 3~16");

    this->m_passtext.setParent(this);
    this->m_passtext.setGeometry(iWidget/8*3, iHeight/5*2, 200, 30);
    this->m_passtext.setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 6~18");
    this->m_passtext.setEchoMode(QLineEdit::Password);  // 设置为密码格式

    this->m_checkpass.setText("记住密码");
    this->m_checkpass.setParent(this);
    this->m_checkpass.setGeometry(iWidget/8, iHeight/5*3, 183, 25);

    QFont font("Microsoft YaHei UI", 9, QFont::Bold, false);
    font.setUnderline(true);
    this->m_button_register.setParent(this);
    this->m_button_register.setFont(font);
    this->m_button_register.setText("还没有账户,马上注册");
    this->m_button_register.setStyleSheet("color: rgb(255, 170, 0);");
    this->m_button_register.setAutoRaise(true);
    this->m_button_register.setGeometry(this->m_checkpass.x() + this->m_checkpass.width()+20, this->m_checkpass.y()+5, 127, 18);

    this->m_button_login.setParent(this);
    this->m_button_login.setText("登录");
    this->m_button_login.setGeometry(iWidget/4, iHeight/5*4 - this->m_checkpass.height() , 200, 50);
    this->m_button_login.setStyleSheet("background-image: url(:/images/balckButton.png);font: 75 18pt \"新宋体\";color: rgb(255, 255, 255);");

    //this->setStyleSheet("border: 2px solid #ff0000;");
    //this->m_title->setGeometry(1, 1, 334, 33);
    //this->m_username->setGeometry(iWidget-10,iHeight-10, 39, 21);
}

void LoginContext::initShowData()
{
    this->m_usertext.setFocus();            // 设置获取焦点
    Login* login = dynamic_cast<Login*>(this->parent());
    if(!login)
        login = dynamic_cast<Login*>(this->parent()->parent());
    if(!login)
        return;
    const LoginInfo& login_info = login->getInfoContext().getLoginInfo();
    if(!login_info.username.length() ||  !login_info.password.length() )
        return;

    this->m_usertext.setText(login_info.username);
    this->m_passtext.setText(login_info.password);
    this->m_checkpass.setChecked(true);
}


void LoginContext::on_button_registe_clicked()
{
    Login* login = (Login*)(this->parent()->parent());
    emit login->showRegisterPage();
}

void LoginContext::on_button_login_clicked()
{
    Login* login = nullptr;
    QString user = this->m_usertext.text();
    QString pass = this->m_passtext.text();
    LoginInfo info = {0};

    QRegularExpression regexp(USER_REG);
    if(!regexp.match(user).hasMatch())
    {
        QMessageBox::warning(this, "警告", "用户名格式不正确");
        this->m_usertext.clear();
        this->m_usertext.setFocus();
        return;
    }
    regexp.setPattern(PASSWD_REG);
    if(!regexp.match(pass).hasMatch())
    {
        QMessageBox::warning(this, "警告", "密码格式不正确");
        this->m_passtext.clear();
        this->m_passtext.setFocus();
        return;
    }
    if(!this->m_checkpass.isChecked())
        return;

    login = dynamic_cast<Login*>(this->parent());
    if(!login)
        login = dynamic_cast<Login*>(this->parent()->parent());
    if(!login)
        return;
    info.username = this->m_usertext.text();
    info.password = this->m_passtext.text();

    // 登录信息写入配置文件cfg.json
    login->getInfoContext().setLoginInfo(info);
    login->getInfoContext().WriteConfContext();

    this->sendLoginMessage(info);

}

bool  LoginContext::sendLoginMessage(const LoginInfo& info)
{
    Login* login = nullptr;
    QNetworkAccessManager& manager = NetworkManager::getNetManager();
    QByteArray array = this->setLoginJson(info);
    login = dynamic_cast<Login*>(this->parent());
    if (!login)
        login = dynamic_cast<Login*>(this->parent()->parent());
    if (!login)
        return false;
    const ServerInfo& server = login->getInfoContext().getServerInfo();

    // 设置连接服务器要发送的url
    QNetworkRequest request;
    request.setUrl(QUrl(QString("http://%1:%2/login").arg(server.ip).arg(server.port)));
    // 请求头信息
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(array.size()));
    // 向服务器发送post请求
    QNetworkReply* reply = manager.post(request, array);
    WinPrintA << "post url:" << request.url().toString() << "  post data: " << array;

    // 接收服务器发回的http响应消息
    // 判断请求是否被成功处理
    connect(reply, &QNetworkReply::readyRead, [=]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            WinPrintA << reply->errorString();
            //释放资源
            reply->deleteLater();
            return;
        }
        /*
            登陆 - 服务器回写的json数据包格式：
                成功：{"code":"000"}
                失败：{"code":"001"}
        */
        // 将server回写的数据读出
        QByteArray json = reply->readAll();
        WinPrintA << "server return value: " << json;
        QStringList tmpList = this->getLoginStatus(json);
        if (tmpList.at(0) != "000" || 0 == tmpList.size())
        {
            QMessageBox::warning(this, "登录失败", tmpList.at(0));
            reply->deleteLater(); //释放资源
            return;
        }
        WinPrintA << "登陆成功";
        reply->deleteLater(); //释放资源
    });


}

// 得到服务器回复的登陆状态， 状态码返回值为 "000", 或 "001"，还有登陆section
QStringList LoginContext::getLoginStatus(QByteArray json)
{
    QJsonParseError error;
    QStringList list;

    // 将来源数据json转化为JsonDocument
    // 由QByteArray对象构造一个QJsonDocument对象，用于我们的读写操作
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);

    if (error.error != QJsonParseError::NoError)
    {
        WinPrintA << "err = " << error.errorString();
        WinPrintA << "err = " << QString(json.data()).indexOf("404 Not Found");
        if( QString(json.data()).indexOf("404 Not Found"))
            list.append("服务连接失败");
        else
            list.append(error.errorString());
        return list;
    }

    if (doc.isNull() || doc.isEmpty() || false == doc.isObject())
    {
        WinPrintA << "doc.isNull() || doc.isEmpty() || doc.isObject() == null";
        list.append("解析服务器返回数据失败");
        return list;
    }

    //取得最外层这个大对象
    QJsonObject obj = doc.object();
    WinPrintA << "recv server code  " << obj.value("code").toString();
    //状态码
    list.append(obj.value("code").toString());
    //登陆token
    list.append(obj.value("token").toString());
    return list;
}

QByteArray LoginContext::setLoginJson(const LoginInfo& info)
{
    QMap<QString, QVariant> login;
    login.insert("user", info.username);
    // 密码使用MD5加密
    login.insert("pwd", CryptUtil::md5Text(info.password));

    /*json数据如下
        {
            user:xxxx,
            pwd:xxx
        }
    */

    QJsonDocument jsonDocument = QJsonDocument::fromVariant(login);
    if (jsonDocument.isNull())
    {
        WinPrintA << " jsonDocument.isNull() ";
        return "";
    }
    return jsonDocument.toJson();
}