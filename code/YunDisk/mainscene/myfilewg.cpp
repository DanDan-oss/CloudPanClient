#include "myfilewg.h"
#include "common/global.h"
#include "common/network_manager.h"
#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QTimer>
#include <QJsonObject>>
#include <QJsonDocument>


QNetworkAccessManager& manager = NetworkManager::getNetManager();

MyFileWg::MyFileWg(const QRect& rect, QWidget* parent)
    : QWidget{ parent }, m_listWidget(this),
    m_menu(nullptr), m_downloadAction(nullptr), m_shareAction(nullptr), 
    m_delAction(nullptr), m_propertyAction(nullptr),
    m_menuEmpty(nullptr), m_pvAscendingAction(nullptr), 
    m_pvDescendingAction(nullptr), m_refreshAction(nullptr), m_uploadAction(nullptr)
{
    this->initScene(rect);
    this->checkTaskList();
}

MyFileWg::~MyFileWg()
{
    if (this->m_menu)
        delete this->m_menu;
    if (this->m_downloadAction)
        delete this->m_downloadAction;
    if (this->m_shareAction)
        delete this->m_shareAction;
    if (this->m_delAction)
        delete this->m_delAction;
    if (this->m_propertyAction)
        delete this->m_propertyAction;

    if (this->m_menuEmpty)
        delete this->m_menuEmpty;
    if (this->m_pvAscendingAction)
        delete this->m_pvAscendingAction;
    if (this->m_pvDescendingAction)
        delete this->m_pvDescendingAction;
    if (this->m_refreshAction)
        delete this->m_refreshAction;
    if (this->m_uploadAction)
        delete this->m_uploadAction;
}

void MyFileWg::initScene(const QRect& rect)
{
    this->initListWidget(rect);
    this->addActionMenu();
}

void MyFileWg::initListWidget(const QRect& rect)
{
    this->m_listWidget.setGeometry(rect);
    this->m_listWidget.setViewMode(QListView::IconMode);          // 设置显示图标模式
    this->m_listWidget.setIconSize(QSize(80, 80));                // 设置图标大小
    this->m_listWidget.setGridSize(QSize(100, 120));              // 设置item大小

    // 设置QLisView大小改变时，图标的调整模式，默认是固定的，可以改成自动调整
    this->m_listWidget.setResizeMode(QListView::Adjust);          // 自动适应布局

    // 设置列表可以拖动，如果想固定不能拖动，使用QListView::Static
    this->m_listWidget.setMovement(QListView::Static);

    // 设置图标之间的间距, 当setGridSize()时，此选项无效
    this->m_listWidget.setSpacing(30);

    this->m_listWidget.showDropIndicator();
    this->m_listWidget.setCurrentRow(-1);

    // listWidget 右键菜单
    // 发出 customContextMenuRequested 信号
    this->m_listWidget.setContextMenuPolicy(Qt::CustomContextMenu);

    bool isConnected = connect(&this->m_listWidget, &QListWidget::customContextMenuRequested, this, &MyFileWg::rightMenu);
    WinPrintA << "右键菜单连接状态: " << isConnected;
    connect(&this->m_listWidget, &QListWidget::itemPressed, this, [=](QListWidgetItem* item)
    {
        QString str = item->text();
        if ("上传文件" == str)
            this->addUploadFiles();
    });
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

// 显示右键菜单
void MyFileWg::rightMenu(const QPoint& pos)
{
    QListWidgetItem* item = this->m_listWidget.itemAt(pos);
    // 没有点图标
    if (!item)
    {   
        // QPoint QMouseEvent::pos()   这个只是返回相对这个widget(重载了QMouseEvent的widget)的位置
        // QPoint QMouseEvent::globalPos()  窗口坐标，这个是返回鼠标的全局坐标
        // QPoint QCursor::pos() [static] 返回相对显示器的全局坐标
        // QWidget::pos() : QPoint 这个属性获得的是当前目前控件在父窗口中的位置
        this->m_menuEmpty->exec(QCursor::pos());    //在鼠标点击的地方弹出菜单
        return;
    }

    // 点击图标
    this->m_listWidget.setCurrentItem(item);
    if (item->text() == "上传文件")     // 如果是上传文件,没有右击菜单
        return;
    this->m_menu->exec(QCursor::pos());
}

void MyFileWg::clearAllTask()
{

}

void MyFileWg::checkTaskList()
{
    //定时检查上传队列是否有任务需要上传
    connect(&this->m_uploadFileTimer, &QTimer::timeout, this, &MyFileWg::uploadFilesAction);

    // 定时检查下载队列是否有任务需要下载
    connect(&this->m_downloadTimer, &QTimer::timeout, this, &MyFileWg::downloadFilesAction);

    // 启动定时器，500毫秒间隔
    // 每个500毫秒，检测上传任务，每一次只能上传一个文件
    this->m_uploadFileTimer.start(500);

    // 每个500毫秒，检测下载任务，每一次只能下载一个文件
    this->m_downloadTimer.start(500);

}

void MyFileWg::addUploadFiles()
{

}

void MyFileWg::uploadFilesAction()
{

}

void MyFileWg::addDownloadFiles()
{
    
}

void MyFileWg::downloadFilesAction()
{

}

/******************* The -文件item展示- End *******************/
/******************* 显示用户的文件列表 *******************/
// 得到服务器json文件
QStringList MyFileWg::getCountStatus(QByteArray json)
{
    QJsonParseError error;
    QStringList list;

    WinPrintA << "json = " << json.data();

    //将来源数据json转化为JsonDocument
    //由QByteArray对象构造一个QJsonDocument对象，用于我们的读写操作
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if (error.error != QJsonParseError::NoError)
    {
        WinPrintA << "err = " << error.errorString();
        return list;
    }

    if (doc.isNull() || doc.isEmpty() || !doc.isObject())
    {
        WinPrintA << "doc.isNull() || doc.isEmpty() || doc.isObject() error";
        return list;
    }
    
    QJsonObject obj = doc.object();     // 取得最外层这个大对象
    list.append(obj.value("token").toString());     // 登陆token
    list.append(obj.value("num").toString());       // 文件个数
}