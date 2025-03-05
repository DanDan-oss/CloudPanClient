#include "login.h"
#include "macro.h"
#include <QLabel>
#include <QMovie>
#include <QPainter>
#include <QMouseEvent>
#include <QLineEdit>
#include <QToolButton>
#include <QCheckBox>
#include <QStackedWidget>
#include <QMainWindow>
#include <QDebug>

Login::Login(QWidget *parent)
    : QWidget{parent}
{
    this->initScene();
    //connect(this->m_title)
}

Login::~Login()
{
    if(this->m_login_page)
        delete this->m_login_page;
    if(this->m_title_page)
        delete this->m_title_page;
    if(this->m_register_page)
        delete this->m_register_page;
    if(this->m_serverconf_page)
        delete this->m_serverconf_page;
}

void Login::initScene()
{
    int iHeight= MAIN_SCENE_WINDOW_H, iWidget=MAIN_SCENE_WINDOW_W;

    this->setFixedSize(iWidget, iHeight);
    this->parentWidget()->setFixedSize(iWidget, iHeight);
    this->setWindowTitle(WINDOW_TITLE_TEXT);
    this->setWindowIcon(QIcon(WINDOW_ICON_PATH));

    // 去掉创建的边框
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    // 设置当前窗口所有的字体
    this->setFont(QFont("微软雅黑", 16, QFont::Bold, false));
    this->m_title_page = new TitleWg(QRect(0,0, MAIN_SCENE_WINDOW_W, iHeight/5), this); // title，占据上1/5的地方
    this->m_login_page = new LoginContext(QRect(iWidget/8,iHeight/5, iWidget/4*3, iHeight/3*2), this); // 用户登录
    this->m_register_page = new RegisterContext(QRect(iWidget/8,iHeight/5, iWidget/4*3, iHeight/3*2), this); // 注册
    this->m_serverconf_page = new ServerConfig(QRect(iWidget/8,iHeight/5, iWidget/4*3, iHeight/3*2), this);  // 服务器配置

    //this->m_title->show();
    //this->m_user_context->show();


    this->m_stacked_widget.setParent(this);
    this->m_stacked_widget.setGeometry(QRect(iWidget/8,iHeight/5, iWidget/4*3, iHeight/3*2));
    this->m_stacked_widget.addWidget(this->m_login_page);
    this->m_stacked_widget.addWidget(this->m_register_page);
    this->m_stacked_widget.addWidget(this->m_serverconf_page);
    this->m_stacked_widget.setCurrentWidget(this->m_login_page);

}

void Login::paintEvent(QPaintEvent* event)
{
    int iHeight= MAIN_SCENE_WINDOW_H, iWidget=MAIN_SCENE_WINDOW_W;

    // 初始化主场景背景图
    QPainter painter(this);
    QPixmap pixmap(WINDOW_BACKUP_PATH);

    painter.drawPixmap(0, 0, iWidget,iHeight, pixmap);
    return QWidget::paintEvent(event);
}

void Login::switch_register_page()
{
    //Login* login = (Login*)me;

    //login->m_stacked_widget.setCurrentWidget(this->m_register_page);
    // qDebug() << this->m_register_page;
    // qDebug() << this->m_login_page;
    // qDebug() << this->m_serverconf_page;
    // qDebug() << &(this->m_stacked_widget);
    qDebug() <<" &(this->m_stacked_widget)";
}

void Login::on_register_button_clicked()
{

}

void Login::on_login_button_clicked()
{

}

void Login::on_serverconf_button_clicked()
{

}


TitleWg::TitleWg(const QRect& rect, QWidget *parent)
    : QWidget{parent}
{
    this->initScene(rect);

    // 关闭按钮被点击
    connect(&this->m_button_close, &QToolButton::clicked, [=]()
    {
        emit CloseWindow();
    });
    // 服务器设置按钮被点击(右上角的小螺丝)
    connect(&this->m_button_set, &QToolButton::clicked, [=]()
    {
        // 发送信号给父窗口切换到服务器设置窗口
        emit showSetServerConfig();
    });
    // 窗口最小化按钮被点击
    connect(&this->m_button_mix, &QToolButton::clicked, [=]()
    {
        ((QMainWindow*)(this->parent()->parent()))->showMinimized();
    });
}

TitleWg::~TitleWg()
{

}

void TitleWg::initScene(const QRect &rect)
{
    QPixmap pixmap(WINDOW_LOGO_PATH);
    QSize btn_icon = {32, 32};


    this->setGeometry(rect);
    this->setStyleSheet("color:rgb(255, 255, 255)");

    this->m_logolable.setParent(this);
    this->m_logolable.setPixmap(pixmap.scaled(40, 40));
    this->m_logolable.setGeometry(30, 30 ,40, 40);

    this->m_title.setParent(this);
    this->m_title.setText(QString("CloudPan"));
    //this->m_title.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_title.setFont(QFont("新宋体", 14, QFont::Bold, false));
    this->m_title.setStyleSheet("color:#ff6600;");   // 设置颜色
    this->m_title.setGeometry(this->m_logolable.width()+this->m_logolable.x()+20,this->m_logolable.y(), 100, 40);

    this->m_button_close.setParent(this);
    this->m_button_close.setText("...");
    this->m_button_close.setIcon(QIcon(":/images/login_close.png"));
    this->m_button_close.setIconSize(btn_icon);
    this->m_button_close.setFixedSize(39, 38);
    this->m_button_close.move(this->width()- this->m_button_close.width() -20, this->m_logolable.y());
    this->m_button_close.setAutoRaise(true);     // 不会自动填充背景色
    this->m_button_close.show();

    this->m_button_mix.setParent(this);
    this->m_button_mix.setText("...");
    this->m_button_mix.setIcon(QIcon(":/images/login_min.png"));
    this->m_button_mix.setIconSize(btn_icon);
    this->m_button_mix.setFixedSize(39, 38);
    this->m_button_mix.move(this->m_button_close.x() - this->m_button_mix.width() -20, this->m_logolable.y());
    this->m_button_mix.setAutoRaise(true);     // 不会自动填充背景色
    this->m_button_mix.show();

    this->m_button_set.setParent(this);
    this->m_button_set.setText("...");
    this->m_button_set.setIcon(QIcon(":/images/login_setting.png"));
    this->m_button_set.setIconSize(btn_icon);
    this->m_button_set.setFixedSize(39, 38);
    this->m_button_set.move(this->m_button_mix.x() - this->m_button_mix.width() -20,this->m_logolable.y());
    this->m_button_set.setAutoRaise(true);     // 不会自动填充背景色
    this->m_button_set.show();


    //this->setStyleSheet("border: 2px solid #ff0000;");
}


void TitleWg::mouseMoveEvent(QMouseEvent *event)
{
    // 只允许左键拖动
    if(event->buttons() & Qt::LeftButton)
    {
        // 窗口跟随鼠标移动
        // 窗口左上角点 = 鼠标当前位置 - 差值

        this->parentWidget()->parentWidget()->move(event->globalPos() - m_pt);
    }
}

void TitleWg::mousePressEvent(QMouseEvent *ev)
{
    // 如果鼠标左键按下
    if(ev->button() == Qt::LeftButton)
    {
        // 鼠标至当前窗口的相对位置 =  鼠标当前屏幕位置 - 当前主窗口左上角点的屏幕位置
        m_pt = ev->globalPos() - this->parentWidget()->parentWidget()->geometry().topLeft();
    }
}

LoginContext::LoginContext(const QRect &rect, QWidget *parent)
    : QWidget{parent}
{
    this->initScene(rect);
    connect(&this->m_button_register, &QToolButton::clicked, this, [=]()
    {
        // 切换到注册界
        emit ((Login*)(this->parent()))->switch_register_page();
    });
}

LoginContext::~LoginContext()
{

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
    this->m_usertext.setFocus();        // 设置获取焦点

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

void LoginContext::paintEvent(QPaintEvent* event)
{


    return QWidget::paintEvent(event);
}


RegisterContext::RegisterContext(const QRect &rect, QWidget *parent)
    : QWidget{parent}

{
    this->initScene(rect);
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

ServerConfig::ServerConfig(const QRect &rect, QWidget *parent)
{
    this->initScene(rect);
}

ServerConfig::~ServerConfig()
{

}

void ServerConfig::initScene(const QRect &rect)
{
    int iHeight=rect.height(), iWidget=rect.width();
    this->setGeometry(rect);

    this->setFont(QFont("Microsoft YaHei UI", 9, QFont::Bold, false));

    // 设置用户登录标签
    this->m_title.setParent(this);
    this->m_title.setText("服务器设置");
    this->m_title.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_title.setFont(QFont("华文新魏", 26, QFont::Bold, false));
    this->m_title.setGeometry(iWidget/7*2, 0, 200, 50);    // 带了show()

    this->m_server.setParent(this);
    this->m_server.setText("服务器地址:");
    this->m_server.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_server.setGeometry(iWidget/8*2, iHeight/5*1, 63, 25);
    this->m_server_address.setParent(this);
    this->m_server_address.setGeometry(iWidget/8*4, iHeight/5*1, 133, 25);

    this->m_port.setParent(this);
    this->m_port.setText("服务器端口:");
    this->m_port.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_port.setGeometry(iWidget/8*2, iHeight/5*2, 63, 25);
    this->m_server_port.setParent(this);
    this->m_server_port.setGeometry(iWidget/8*4, iHeight/5*2, 133, 25);

    this->m_button_ok.setParent(this);
    this->m_button_ok.setText("确定");
    this->m_button_ok.setGeometry(iWidget/7*2, iHeight/5*3 , 200, 50);
    this->m_button_ok.setStyleSheet("background-image: url(:/images/balckButton.png);font: 75 18pt \"新宋体\";color: rgb(255, 255, 255);");
    this->m_button_ok.setAutoRaise(true);
}
