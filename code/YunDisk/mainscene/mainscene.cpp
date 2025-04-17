#include "mainscene.h"
#include "common/global.h"
#include "common/cryptutil.h"
#include "common/network_manager.h"
#include "common/logininfoinstance.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QPainter>


QNetworkAccessManager& manager = NetworkManager::getNetManager();

MainScene::MainScene(QWidget* parent)
    : QWidget{ parent }
{
    this->initScene();
}

MainScene::~MainScene()
{

}

void MainScene::initScene()
{
    int iHeight = MAIN_SCENE_WINDOW_H, iWidget = MAIN_SCENE_WINDOW_W;

    this->setFixedSize(iWidget, iHeight);
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    // 去掉创建的边框
    this->setFont(QFont("微软雅黑", 16, QFont::Bold, false));
    this->m_btngroup = new ButtonGroup(QRect(0, 0, MAIN_SCENE_WINDOW_W, 115), this);

}

void MainScene::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap bk(":/images/title_bk3.jpg");
    painter.drawPixmap(0, 0, this->width(), this->height(), bk);
}

void MainScene::showWindow()
{
    this->parentWidget()->setFixedSize(this->width(), this->height());
    this->show();
}



