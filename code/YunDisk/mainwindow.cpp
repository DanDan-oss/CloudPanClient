#include "mainwindow.h"
#include <QString>
#include "macro.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(WINDOW_TITLE_TEXT);
    this->setWindowIcon(QIcon(WINDOW_ICON_PATH));
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->login = new Login(this);
    this->login->show();
}

MainWindow::~MainWindow()
{
    delete this->login;
    this->login = nullptr;
}

