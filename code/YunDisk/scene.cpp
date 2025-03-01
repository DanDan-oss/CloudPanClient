#include "scene.h"
#include "macro.h"
#include <QLabel>
#include <QMovie>
#include <QPainter>
#include <QMouseEvent>
#include <QTextEdit>

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
    this->setFont(QFont("华文彩云", 16, QFont::Bold, false));
    this->m_title = new TitleWg(QRect(0,0, MAIN_SCENE_WINDOW_W, iHeight/5), this); // title，占据上1/5的地方
    this->m_user_context = new UserContext(QRect(iWidget/8,iHeight/5, iWidget/4*3, iHeight/5*2), this); // 账户密码输入框

    this->m_title->show();
    this->m_user_context->show();
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
    this->setGeometry(rect);

    this->setStyleSheet("color:rgb(255, 255, 255)");
}

TitleWg::~TitleWg()
{

}

void TitleWg::paintEvent(QPaintEvent* event)
{
    QPixmap pixmap(WINDOW_LOGO_PATH);
    QPainter painter(this);

    //pixmap = pixmap.scaled(40, 40);
    pixmap = pixmap.scaled(pixmap.width()*0.1, pixmap.height()*0.1);
    painter.drawPixmap(30, 30, pixmap.width(), pixmap.height(), pixmap);
    return QWidget::paintEvent(event);
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
        // 求差值 = 鼠标当前位置 - 窗口左上角点
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
}

void UserContext::initScene(const QRect &rect)
{
    int iHeight=rect.height(), iWidget=rect.width();

    this->setGeometry(rect);
    QLabel* title = new  QLabel("用户登录", this);
    QLabel* username = new  QLabel("用户名:", this);
    QLabel* password = new QLabel("密码:", this);
    QTextEdit* usertext = new QTextEdit(this);
    QTextEdit* passtext = new QTextEdit(this);


    // 设置用户登录标签
    title->setAlignment(Qt::AlignCenter);   // 设置文本居中
    title->setFont(QFont("新宋体", 24, QFont::Bold, false));
    title->setGeometry(iWidget/7*2, 0, 200, 50);
    title->show();

    username->setAlignment(Qt::AlignCenter);   // 设置文本居中
    username->setGeometry(iWidget/8, iHeight/5*2, 100, 30);
    username->show();

    password->setAlignment(Qt::AlignCenter);   // 设置文本居中
    password->setGeometry(iWidget/8,iHeight/5*4, 100, 30);
    password->show();


    usertext->setGeometry(iWidget/8*3, iHeight/5*2, 200, 32);
    passtext->setGeometry(iWidget/8*3, iHeight/5*4, 200, 32);

    this->m_title=title;
    this->m_username=username;
    this->m_password=password;
    this->m_usertext=usertext;
    this->m_passtext=passtext;


    //this->setStyleSheet("border: 2px solid #ff0000;");
    //this->m_title->setGeometry(1, 1, 334, 33);
    //this->m_username->setGeometry(iWidget-10,iHeight-10, 39, 21);
}

void UserContext::paintEvent(QPaintEvent* event)
{


    return QWidget::paintEvent(event);
}

