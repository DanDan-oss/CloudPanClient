#include "common/global.h"
#include "mainwindow.h"
#include <QApplication>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(WINDOW_TITLE_TEXT);
    this->setWindowIcon(QIcon(WINDOW_ICON_PATH));
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->m_info.setConfPath(QString("./conf/Config.ini"));
    this->m_info.readConfContext();

    this->login = new Login(this);
    this->m_scene = new MainScene(this);

    this->showMainWindow(0);
}

MainWindow::~MainWindow()
{
    delete this->login;
    this->login = nullptr;
}

void MainWindow::showMainWindow(int index)
{
    //this->moveToCenter(this->m_scene);
    switch (index)
    {
    case  0:    // 显示登录界面
        this->m_scene->hide();
        this->login->showWindow();
        break;
    case  1:    // 显示功能主界面
        this->login->hide();
        this->m_scene->showWindow();
        break;
    default:
        break;
    }

    
    //Login* login = nullptr;
    //MainScene* m_scene = nullptr;

    // 切换至我的文件页面
}

void MainWindow::moveToCenter(QWidget* tmp)
{
    // 显示窗口
    tmp->show();

    //QScreen* desktop = QApplication::QScreen();
    //tmp->move((desktop->width() - tmp->width())/2, (desktop->height() - tmp->height()) / 2)
}

InfoContext& MainWindow::getInfoContext()
{
    return this->m_info;
}

