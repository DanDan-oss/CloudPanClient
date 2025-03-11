#include "logincontext.h"
#include "login.h"
#include "common/loginfo.h"

LoginContext::LoginContext(const QRect &rect, QWidget *parent)
    : QWidget{parent}
{
    this->initScene(rect);
    this->initShowData();
    connect(&this->m_button_register, &QToolButton::clicked, this, &LoginContext::on_button_registe_clicked);
}

LoginContext::~LoginContext()
{

}

void LoginContext::on_button_registe_clicked()
{
    Login* login = (Login*)(this->parent()->parent());
    emit login->showRegisterPage();
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
    this->m_usertext.setText(login_info.username);
    this->m_passtext.setText(login_info.password);
    this->m_checkpass.setCheckState(login_info.savepass? Qt::Checked :Qt::Unchecked);
}

void LoginContext::paintEvent(QPaintEvent* event)
{


    return QWidget::paintEvent(event);
}
