#include "mainscene.h"
#include "myfilewg.h"
#include "mainwindow.h"
#include "common/global.h"
#include <QPainter>
#include <QStackedWidget>


MainScene::MainScene(QWidget* parent)
    : QWidget{ parent },
    m_btngroup(nullptr),
    m_stacked_widget(nullptr),
    myfiles_page(nullptr)
{
    this->initScene();
    this->InitManagerSignals();
}

MainScene::~MainScene()
{
    if (this->m_btngroup)
        delete this->m_btngroup;
}

void MainScene::initScene()
{
    int iHeight = MAIN_SCENE_WINDOW_H, iWidget = MAIN_SCENE_WINDOW_W;
    int BtnHeight = 115;

    this->setFixedSize(iWidget, iHeight);
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    // 去掉创建的边框
    this->setFont(QFont("微软雅黑", 16, QFont::Bold, false));
    this->m_btngroup = new ButtonGroup(QRect(0, 0, MAIN_SCENE_WINDOW_W, BtnHeight), this);

    this->m_stacked_widget = new QStackedWidget(this);
    this->m_stacked_widget->setGeometry(QRect(0, BtnHeight, MAIN_SCENE_WINDOW_W, MAIN_SCENE_WINDOW_H - BtnHeight));
    this->myfiles_page = new MyFileWg(QRect(0, 0, 755, MAIN_SCENE_WINDOW_H - BtnHeight), this);

    this->m_stacked_widget->addWidget(this->myfiles_page);
    this->m_stacked_widget->setCurrentWidget(myfiles_page);

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
    this->m_stacked_widget->setCurrentWidget(myfiles_page);
    this->myfiles_page->refreshFiles();
    this->show();
}

void MainScene::InitManagerSignals()
{
    MainWindow* window = (MainWindow*)this->parent();
    if (!window)
        return;

    connect(this->m_btngroup, &ButtonGroup::signalCloseWindow, window, &MainWindow::close);         // 关闭
    connect(this->m_btngroup, &ButtonGroup::signalMinWindow, window, &MainWindow::showMinimized);   // 最小化
    connect(this->m_btngroup, &ButtonGroup::signalMaxWindow, window, [window](bool maximized) {
        if (maximized)
            window->showMaximized();        // 最大化
        else
            window->showNormal();           // 还原
    });
    connect(this->m_btngroup, &ButtonGroup::signalMyFile, this, [this]() {slotButtonGroupClick(0); });
    connect(this->m_btngroup, &ButtonGroup::signalShareList, this, [this]() {slotButtonGroupClick(1); });
    connect(this->m_btngroup, &ButtonGroup::signalDownload, this, [this]() {slotButtonGroupClick(2); });
    connect(this->m_btngroup, &ButtonGroup::signalTransfron, this, [this]() {slotButtonGroupClick(3); });
    connect(this->m_btngroup, &ButtonGroup::signalSwitchUser, this, [this]() {slotButtonGroupClick(4); });
}

void MainScene::slotButtonGroupClick(int index)
{
    switch (index)
    {
    case 0:
        this->m_stacked_widget->setCurrentWidget(this->myfiles_page);
        this->myfiles_page->show();
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    default:
        break;
    }
}




