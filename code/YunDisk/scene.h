#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QLabel>
#include <QPainter>

// 窗口主场景类===========================
// 登录窗口
class MainScene  : public QWidget
{
    Q_OBJECT
public:
    explicit MainScene (QWidget *parent = nullptr);
    void initScene();     // 初始化主场景

    virtual void paintEvent(QPaintEvent* event);    // 绘制事件,绘制背景
signals:

};

#endif // SCENE_H
