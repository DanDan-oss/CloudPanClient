#include "mainscene.h"
#include "common/cryptutil.h"
#include "common/network_manager.h"
#include "common/logininfoinstance.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QScreen>

#define MAIN_SCENE_WINDOW_H 480     // 主窗口界面高
#define MAIN_SCENE_WINDOW_W 600     // 主窗口界面宽

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
    // 去掉创建的边框
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());

}

void MainScene::showWindow()
{
    this->parentWidget()->setFixedSize(this->width(), this->height());
    this->show();
}

