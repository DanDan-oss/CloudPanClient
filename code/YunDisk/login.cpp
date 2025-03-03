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

Login::Login(QWidget *parent)
    : QWidget{parent}
{
    this->initScene();
}

Login::~Login()
{
    if(this->m_user_context)
        delete this->m_user_context;
    if(this->m_title)
        delete this->m_title;
    if(this->m_stacked_widget)
        delete this->m_stacked_widget;
}

void Login::initScene()
{
    int iHeight= MAIN_SCENE_WINDOW_H, iWidget=MAIN_SCENE_WINDOW_W;
    QStackedWidget* stacked_widget = nullptr;

    this->setFixedSize(iWidget, iHeight);
    this->parentWidget()->setFixedSize(iWidget, iHeight);
    this->setWindowTitle(WINDOW_TITLE_TEXT);
    this->setWindowIcon(QIcon(WINDOW_ICON_PATH));

    // 去掉创建的边框
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    // 设置当前窗口所有的字体
    this->setFont(QFont("微软雅黑", 16, QFont::Bold, false));
    this->m_title = new TitleWg(QRect(0,0, MAIN_SCENE_WINDOW_W, iHeight/5), this); // title，占据上1/5的地方
    this->m_user_context = new UserContext(QRect(iWidget/8,iHeight/5, iWidget/4*3, iHeight/3*2), this); // 账户密码输入框

    //this->m_title->show();
    //this->m_user_context->show();

    this->m_stacked_widget = stacked_widget = new QStackedWidget(this);
    stacked_widget->setGeometry(QRect(iWidget/8,iHeight/5, iWidget/4*3, iHeight/3*2));
    stacked_widget->addWidget(this->m_user_context);
    stacked_widget->setCurrentWidget(this->m_user_context);


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


TitleWg::TitleWg(const QRect& rect, QWidget *parent)
    : QWidget{parent}
{
    this->initScene(rect);
}

TitleWg::~TitleWg()
{
    if(this->m_logolable)
        delete this->m_logolable;
    if(this->m_title)
        delete this->m_title;
    if(this->m_button_set)
        delete this->m_button_set;
    if(this->m_button_mix)
        delete this->m_button_mix;
    if(this->m_button_close)
        delete this->m_button_close;
}

void TitleWg::initScene(const QRect &rect)
{
    QPixmap pixmap(WINDOW_LOGO_PATH);
    QLabel* logo = nullptr;
    QLabel* title = nullptr;
    QToolButton* button_set = nullptr;
    QToolButton* button_mix = nullptr;
    QToolButton* button_close = nullptr;
    QSize btn_icon = {32, 32};


    this->setGeometry(rect);
    this->setStyleSheet("color:rgb(255, 255, 255)");

    logo = new  QLabel(this);
    logo->setPixmap(pixmap.scaled(40, 40));
    logo->setGeometry(30, 30 ,40, 40);

    title = new  QLabel(QString("CloudPan"),this);
    //title->setAlignment(Qt::AlignCenter);   // 设置文本居中
    title->setFont(QFont("新宋体", 14, QFont::Bold, false));
    title->setStyleSheet("color:#ff6600;");   // 设置颜色
    title->setGeometry(logo->width()+logo->x()+20,logo->y(), 100, 40);

    button_close = new QToolButton(this);
    button_close->setText("...");
    button_close->setIcon(QIcon(":/images/login_close.png"));
    button_close->setIconSize(btn_icon);
    button_close->setFixedSize(39, 38);
    button_close->move(this->width()- button_close->width() -20, logo->y());
    button_close->setAutoRaise(true);     // 不会自动填充背景色
    button_close->show();

    button_mix = new QToolButton(this);
    button_mix->setText("...");
    button_mix->setIcon(QIcon(":/images/login_min.png"));
    button_mix->setIconSize(btn_icon);
    button_mix->setFixedSize(39, 38);
    button_mix->move(button_close->x() - button_mix->width() -20, logo->y());
    button_mix->setAutoRaise(true);     // 不会自动填充背景色
    button_mix->show();

    button_set = new QToolButton(this);
    button_set->setText("...");
    button_set->setIcon(QIcon(":/images/login_setting.png"));
    button_set->setIconSize(btn_icon);
    button_set->setFixedSize(39, 38);
    button_set->move(button_mix->x() - button_mix->width() -20, logo->y());
    button_set->setAutoRaise(true);     // 不会自动填充背景色
    button_set->show();

    this->m_logolable = logo;
    this->m_title = title;
    this->m_button_set = button_set;
    this->m_button_mix = button_mix;
    this->m_button_close = button_close;

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

UserContext::UserContext(const QRect &rect, QWidget *parent)
    : QWidget{parent}
{
    this->initScene(rect);
}

UserContext::~UserContext()
{
    if(this->m_title)
        delete this->m_title;
    if(this->m_username)
        delete this->m_username;
    if(this->m_password)
        delete this->m_password;
    if(this->m_usertext)
        delete this->m_usertext;
    if(this->m_passtext)
        delete this->m_passtext;
    if(this->m_checkpass)
        delete this->m_checkpass;
    if(this->m_button_register)
        delete this->m_button_register;
    if(this->m_button_login)
        delete this->m_button_login;
}

void UserContext::initScene(const QRect &rect)
{
    QLabel* title  = nullptr;
    QLabel* username = nullptr;
    QLabel* password = nullptr;
    QLineEdit* usertext = nullptr;
    QLineEdit* passtext = nullptr;
    QCheckBox* checkpass = nullptr;
    QToolButton* button_register = nullptr;
    QToolButton* button_login = nullptr;
    int iHeight=rect.height(), iWidget=rect.width();

    this->setGeometry(rect);
    // 设置用户登录标签
    this->m_title = title = new  QLabel("用户登录", this);
    title->setAlignment(Qt::AlignCenter);   // 设置文本居中
    title->setFont(QFont("新宋体", 24, QFont::Bold, false));
    title->setGeometry(iWidget/7*2, 0, 200, 50);    // 带了show()

    this->m_username = username = new  QLabel("用户名:", this);
    username->setAlignment(Qt::AlignCenter);   // 设置文本居中
    username->setGeometry(iWidget/8, iHeight/5*1, 100, 30);

    this->m_password = password = new QLabel("密码:", this);
    password->setAlignment(Qt::AlignCenter);   // 设置文本居中
    password->setGeometry(iWidget/8,iHeight/5*2, 100, 30);

    this->m_usertext = usertext = new QLineEdit(this);
    this->m_passtext = passtext = new QLineEdit(this);
    usertext->setGeometry(iWidget/8*3, iHeight/5*1, 200, 30);
    passtext->setGeometry(iWidget/8*3, iHeight/5*2, 200, 30);

    this->m_checkpass = checkpass =new QCheckBox("记住密码" , this);
    checkpass->setGeometry(iWidget/8, iHeight/5*3, 183, 25);

    QFont font("Microsoft YaHei UI", 9, QFont::Bold, false);
    font.setUnderline(true);
    this->m_button_register = button_register = new QToolButton(this);
    button_register->setFont(font);
    button_register->setText("还没有账户,马上注册");
    button_register->setStyleSheet("color: rgb(255, 170, 0);");
    button_register->setAutoRaise(true);
    button_register->setGeometry(checkpass->x() + checkpass->width()+20, checkpass->y()+5, 127, 18);

    this->m_button_login = button_login = new QToolButton(this);
    button_login->setText("登录");
    button_login->setGeometry(iWidget/4, iHeight/5*4 - checkpass->height() , 200, 50);
    button_login->setStyleSheet("background-image: url(:/images/balckButton.png);");


    //this->setStyleSheet("border: 2px solid #ff0000;");
    //this->m_title->setGeometry(1, 1, 334, 33);
    //this->m_username->setGeometry(iWidget-10,iHeight-10, 39, 21);
}

void UserContext::paintEvent(QPaintEvent* event)
{


    return QWidget::paintEvent(event);
}
