#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common/InfoContext.h"
#include "login/login.h"
#include "mainscene/mainscene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // 显示主窗口
    void showMainWindow(int index=0);

    // 窗口在屏幕中央显示
    void moveToCenter(QWidget* tmp);

private:
    Login* login = nullptr;
    MainScene* m_scene = nullptr;

};
#endif // MAINWINDOW_H
