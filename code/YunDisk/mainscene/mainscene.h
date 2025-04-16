#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "common/loginfo.h"
#include <QWidget>


// 主场景===========================
class MainScene : public QWidget
{
    Q_OBJECT
public:
    explicit MainScene(QWidget* parent = nullptr);
    ~MainScene();

    void showWindow();

protected:
    void initScene();     // 初始化主界面,绘制界面初始化回调函数

private:

    InfoContext m_info;

};

#endif // MAINWINDOW_H