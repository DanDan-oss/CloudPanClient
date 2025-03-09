#include "registercontext.h"


RegisterContext::RegisterContext(const QRect &rect, QWidget *parent)
    : QWidget{parent}

{
    this->initScene(rect);
    this->initShowData();
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
