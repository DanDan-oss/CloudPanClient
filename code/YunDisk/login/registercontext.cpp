#include "registercontext.h"
#include "common/global.h"
#include "login.h"
#include <QRegularExpression>
#include <QMessageBox>

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

void RegisterContext::on_button_registe_clicked()
{
    QString userName = this->m_usertext.text();
    QString nickName = this->m_nicktext.text();
    QString firstPwd = this->m_passtext.text();
    QString surePwd = this->m_confirm_text.text();
    QString phone = this->m_phone_text.text();
    QString email = this->m_email_text.text();
    Login* login = nullptr;
    LoginInfo info = {0};

    // 密码校验
    QRegularExpression regexp(PASSWD_REG);
    if(!regexp.match(firstPwd).hasMatch())
    {
        QMessageBox::warning(this, "警告", "密码格式不正确");
        this->m_passtext.clear();
        this->m_confirm_text.clear();
        this->m_passtext.setFocus();
        return;
    }
    if(firstPwd != surePwd)
    {
        QMessageBox::warning(this, "警告", "两次输入的密码不匹配, 请重新输入");
        this->m_passtext.clear();
        this->m_confirm_text.clear();
        this->m_passtext.setFocus();
        return;
    }
    // 账户校验
    regexp.setPattern(USER_REG);
    if(!regexp.match(userName).hasMatch())
    {
        QMessageBox::warning(this, "警告", "用户名格式不正确");
        this->m_usertext.clear();
        this->m_usertext.setFocus();
        return;
    }
    if(!regexp.match(nickName).hasMatch())
    {
        QMessageBox::warning(this, "警告", "昵称格式不正确");
        this->m_nicktext.clear();
        this->m_nicktext.setFocus();
        return;
    }
    // 手机校验
    regexp.setPattern(PHONE_REG);
    if(!regexp.match(phone).hasMatch())
    {
        QMessageBox::warning(this, "警告", "手机号码格式不正确");
        this->m_phone_text.clear();
        this->m_phone_text.setFocus();
        return;
    }
    // 邮箱校验
    regexp.setPattern(EMAIL_REG);
    if(!regexp.match(email).hasMatch())
    {
        QMessageBox::warning(this, "警告", "邮箱码格式不正确");
        this->m_email_text.clear();
        this->m_email_text.setFocus();
        return;
    }
    login = dynamic_cast<Login*>(this->parent());
    if(!login)
        login = dynamic_cast<Login*>(this->parent()->parent());
    if(!login)
        return;
    info.username = this->m_usertext.text();
    info.password = this->m_passtext.text();
    login->getInfoContext().setLoginInfo(info);
    login->getInfoContext().WriteConfContext();
}
