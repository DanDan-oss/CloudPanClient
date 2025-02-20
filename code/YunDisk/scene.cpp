#include "scene.h"
#include "macro.h"

MainScene::MainScene(QWidget *parent)
    : QWidget{parent}
{
    this->initScene();
}


void MainScene::initScene()
{

}

void MainScene::paintEvent(QPaintEvent* event)
{
    // 初始化主场景背景图
    QPainter painter(this);
    QPixmap pixmap;
    QLabel lab(this);
    pixmap.load(WINDOW_BACKUP_PATH);


    int iHeight= pixmap.height(), iWidget=pixmap.width();
    painter.drawPixmap(0, 0, iWidget,iHeight, pixmap);

    pixmap.load(WINDOW_LOGO_PATH);

    pixmap = pixmap.scaled(pixmap.width()*0.1, pixmap.height()*0.1);
    painter.drawPixmap(30, 30, pixmap.width(), pixmap.height(), pixmap);

    this->setFixedSize(iWidget, iHeight);
    this->parentWidget()->setFixedSize(iWidget,iHeight);
    return QWidget::paintEvent(event);
}
