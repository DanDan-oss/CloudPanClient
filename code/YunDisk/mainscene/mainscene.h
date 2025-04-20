#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "common/loginfo.h"
#include "buttongroup.h"
#include "myfilewg.h"
#include <QWidget>
#include <QStackedWidget>


// 主场景===========================
class MainScene : public QWidget
{
    Q_OBJECT
public:
    explicit MainScene(QWidget* parent = nullptr);
    ~MainScene();

    void showWindow();
    void InitManagerSignals();      // 初始化处理信息


protected:
    void initScene();           // 初始化主界面,绘制界面初始化回调函数
    virtual void paintEvent(QPaintEvent* event);    // 绘制事件,绘制背景

public slots:
    // 按钮处理函数
    void slotButtonGroupClick(int index);
private:
    InfoContext m_info;
    ButtonGroup* m_btngroup;
    MyFileWg* myfiles_page;
    QStackedWidget* m_stacked_widget;
};

#endif // MAINWINDOW_H