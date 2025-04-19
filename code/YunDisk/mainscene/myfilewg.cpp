#include "myfilewg.h"
#include "common/global.h"
#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>

MyFileWg::MyFileWg(const QRect& rect, QWidget* parent)
    : QWidget{ parent }, m_menu(nullptr), m_menuEmpty(nullptr)
{
    this->initScene(rect);

    connect(&this->m_listWidget, &QListWidget::customContextMenuRequested, this, &MyFileWg::rightMenu);
    connect(&this->m_listWidget, &QListWidget::itemPressed, this, [=](QListWidgetItem* item)
    {
        QString str = item->text();
        if ("上传文件" == str)
            this->addUploadFiles();
    });
}

MyFileWg::~MyFileWg()
{
    if (this->m_menu)
        delete this->m_menu;
    if (this->m_menuEmpty)
        delete this->m_menuEmpty;
}

void MyFileWg::initScene(const QRect& rect)
{
    this->initListWidget();
    this->addActionMenu();
}

void MyFileWg::initListWidget()
{
    this->m_listWidget.setViewMode(QListView::IconMode);          // 设置显示图标模式
    this->m_listWidget.setIconSize(QSize(80, 80));                // 设置图标大小
    this->m_listWidget.setGridSize(QSize(100, 120));              // 设置item大小

    // 设置QLisView大小改变时，图标的调整模式，默认是固定的，可以改成自动调整
    this->m_listWidget.setResizeMode(QListView::Adjust);          // 自动适应布局

    // 设置列表可以拖动，如果想固定不能拖动，使用QListView::Static
    this->m_listWidget.setMovement(QListView::Static);

    // 设置图标之间的间距, 当setGridSize()时，此选项无效
    this->m_listWidget.setSpacing(30);

    // listWidget 右键菜单
    // 发出 customContextMenuRequested 信号
    this->m_listWidget.setContextMenuPolicy(Qt::CustomContextMenu);

}

void MyFileWg::addActionMenu()
{
    QString menuStyleSheet = ("background-color:rgba(202, 245, 238, 80);" "color:rgb(255, 255, 0);" "font: 14pt \"新宋体\";");


    //===================初始化菜单1==================
    this->m_menu = new QMenu(this);
    this->m_menu->setStyleSheet(menuStyleSheet);

    // 初始化菜单选项
    this->m_downloadAction = new QAction("下载", this);
    this->m_shareAction = new QAction("分享", this);
    this->m_delAction = new QAction("删除", this);
    this->m_propertyAction = new QAction("属性", this);

    if (!this->m_downloadAction || !this->m_shareAction || !this->m_delAction || !this->m_propertyAction)
    {
        WinPrintA << "myfile widget 菜单1有子菜单未生成"; 
        return;
    }

    // 动作1添加到菜单1
    this->m_menu->addAction(this->m_downloadAction);
    this->m_menu->addAction(this->m_shareAction);
    this->m_menu->addAction(this->m_delAction);
    this->m_menu->addAction(this->m_propertyAction);


    //===================初始化菜单2==================
    this->m_menuEmpty = new QMenu(this);
    this->m_menuEmpty->setStyleSheet(menuStyleSheet);

    // 初始化菜单选项
    this->m_pvAscendingAction = new QAction("按下载量升序", this);
    this->m_pvDescendingAction = new QAction("按下载量降序", this);
    this->m_refreshAction = new QAction("刷新", this);
    this->m_uploadAction = new QAction("上传", this);
    if (!this->m_pvAscendingAction || !this->m_pvDescendingAction || !this->m_refreshAction || !this->m_uploadAction)
    {
        WinPrintA << "myfile widget菜单2有子菜单未生成";
        return;
    }

    // 动作2添加到菜单2
    this->m_menuEmpty->addAction(this->m_pvAscendingAction);
    this->m_menuEmpty->addAction(this->m_pvDescendingAction);
    this->m_menuEmpty->addAction(this->m_refreshAction);
    this->m_menuEmpty->addAction(this->m_uploadAction);

    this->initMenuSlots();      // 绑定槽函数

}

void MyFileWg::initMenuSlots()
{
    //=====================信号与槽===================

    // 下载
    connect(this->m_downloadAction, &QAction::triggered, [=](){
        WinPrintA << "下载动作";} 
    );

    // 分享
    connect(this->m_shareAction, &QAction::triggered, [=]() { 
        WinPrintA << "分享动作"; 
    });

    // 删除
    connect(this->m_delAction, &QAction::triggered, [=]() {
        WinPrintA << "删除动作";
    });

    // 属性
    connect(this->m_propertyAction, &QAction::triggered, [=]() {
        WinPrintA << "删除动作";
    });

    // 按下载量升序
    connect(this->m_pvAscendingAction, &QAction::triggered, [=]() {
        WinPrintA << "按下载量升序";
    });

    // 按下载量降序
    connect(this->m_pvDescendingAction, &QAction::triggered, [=]() {
        WinPrintA << "按下载量降序";
    });

    // 刷新
    connect(this->m_refreshAction, &QAction::triggered, [=]() {
        WinPrintA << "刷新动作";
    });

    // 上传
    connect(this->m_uploadAction, &QAction::triggered, [=]() {
        WinPrintA << "上传动作";
    });
}

void MyFileWg::rightMenu(const QPoint& pos)
{
    QListWidgetItem* item = this->m_listWidget.itemAt(pos);
    if (!item)
    {   // 没有点图标
        // QPoint QMouseEvent::pos()   这个只是返回相对这个widget(重载了QMouseEvent的widget)的位置
        // QPoint QMouseEvent::globalPos()  窗口坐标，这个是返回鼠标的全局坐标
        // QPoint QCursor::pos() [static] 返回相对显示器的全局坐标
        // QWidget::pos() : QPoint 这个属性获得的是当前目前控件在父窗口中的位置
    }
}

void MyFileWg::addUploadFiles()
{

}