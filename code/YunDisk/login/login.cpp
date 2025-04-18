#include "common/global.h"
#include "login.h"
#include "mainwindow.h"
#include <QLabel>
#include <QMovie>
#include <QPainter>
#include <QLineEdit>
#include <QToolButton>
#include <QCheckBox>
#include <QStackedWidget>
#include <QMainWindow>
#include <QDebug>

Login::Login(QWidget *parent)
    : QWidget{parent}, m_login_page(nullptr), m_title_page(nullptr), 
    m_register_page(nullptr), m_serverconf_page(nullptr)
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
    int iHeight= LOGIN_SCENE_WINDOW_H, iWidget= LOGIN_SCENE_WINDOW_W;
    
    this->setFixedSize(iWidget, iHeight);
    this->setWindowTitle(WINDOW_TITLE_TEXT);
    this->setWindowIcon(QIcon(WINDOW_ICON_PATH));

    // 去掉创建的边框
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    // 设置当前窗口所有的字体
    this->setFont(QFont("微软雅黑", 16, QFont::Bold, false));
    this->m_title_page = new TitleWg(QRect(0,0, LOGIN_SCENE_WINDOW_W, iHeight/5), this); // title，占据上1/5的地方
    this->m_login_page = new LoginContext(QRect(iWidget/8,iHeight/5, iWidget/4*3, iHeight/3*2), this); // 用户登录
    this->m_register_page = new RegisterContext(QRect(iWidget/8,iHeight/5, iWidget/4*3, iHeight/3*2), this); // 注册
    this->m_serverconf_page = new ServerConfig(QRect(iWidget/8,iHeight/5, iWidget/4*3, iHeight/3*2), this);  // 服务器配置

    //this->m_title->show();
    //this->m_user_context->show();


    this->m_stacked_widget.setParent(this);
    this->m_stacked_widget.setGeometry(QRect(iWidget/8,iHeight/5, iWidget/4*3, iHeight/3*2));
    this->m_stacked_widget.addWidget(this->m_login_page);       // 会将this->m_login_page的Parent重置为this->m_stacked_widget
    this->m_stacked_widget.addWidget(this->m_register_page);       // 会将this->m_register_page的Parent重置为this->m_stacked_widget
    this->m_stacked_widget.addWidget(this->m_serverconf_page);       // 会将this->m_serverconf_page的Parent重置为this->m_stacked_widget
    this->m_stacked_widget.setCurrentWidget(this->m_login_page);
    this->m_login_page->initShowData();
}

void Login::paintEvent(QPaintEvent* event)
{
    int iHeight= LOGIN_SCENE_WINDOW_H, iWidget= LOGIN_SCENE_WINDOW_W;

    // 初始化主场景背景图
    QPainter painter(this);
    QPixmap pixmap(WINDOW_BACKUP_PATH);

    painter.drawPixmap(0, 0, iWidget,iHeight, pixmap);
    return QWidget::paintEvent(event);
}

void Login::showWindow()
{
    this->parentWidget()->setFixedSize(this->width(), this->height());
    this->show();
}

void Login::showRegisterPage()
{
    this->m_stacked_widget.setCurrentWidget(this->m_register_page);
    this->m_register_page->initShowData();
}

void Login::showServerConfPage()
{
    this->m_stacked_widget.setCurrentWidget(this->m_serverconf_page);
    this->m_serverconf_page->initShowData();
}

void Login::closeWindow()
{
    QWidget* pqwCurrentWidget= this->m_stacked_widget.currentWidget();
    QMainWindow* window=(QMainWindow*)this->parent();

    if(pqwCurrentWidget == this->m_register_page || pqwCurrentWidget ==this->m_serverconf_page)
    {   // 当前在 注册窗口 或者 服务器配置窗口 点击关闭按钮,返回登录主界面
        this->m_stacked_widget.setCurrentWidget(this->m_login_page);
        ((LoginContext*)this->m_login_page)->initShowData();
        return;
    }
    if(pqwCurrentWidget == this->m_login_page)
    {   // 当前在登录窗口,直接关闭
        window->close();
        return;
    }
}

void Login::showMinWindow()
{
    QMainWindow* window=(QMainWindow*)this->parent();
    window->showMinimized();
}

InfoContext& Login::getInfoContext()
{
    InfoContext m_info;
    MainWindow* window = dynamic_cast<MainWindow*>(this->parent());
    if (!window)
        return m_info;
    return window->getInfoContext();
}
