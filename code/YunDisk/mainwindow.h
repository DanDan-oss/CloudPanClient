#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common/loginfo.h"
#include "login/login.h"
#include "mainscene/mainscene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    InfoContext& getInfoContext();
    // 显示主窗口
    void showMainWindow(int index=0);

    // 窗口在屏幕中央显示
    void moveToCenter(QWidget* tmp);

private:
    Login* login = nullptr;
    MainScene* m_scene = nullptr;
    InfoContext m_info;

};
#endif // MAINWINDOW_H
