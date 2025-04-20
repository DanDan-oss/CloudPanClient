#include "myfilewg.h"
#include "common/global.h"
#include "common/network_manager.h"
#include "common/InfoContext.h"
#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QTimer>
#include <QJsonObject>>
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkReply>
#include <QMessageBox>

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

void MyFileWg::clearFileList()
{

}

void MyFileWg::refreshFileItems()
{

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

// 显示用户的文件列表
// cmd取值，Normal：普通用户列表，PvAsc：按下载量升序， PvDesc：按下载量降序
void MyFileWg::refreshFiles(MyFileWg::Display cmd)
{
    QNetworkAccessManager& manager = NetworkManager::getNetManager();
    //=========================>先获取用户文件数目<=========================
    this->m_userFilesCount = 0;

    // 获取登录信息实例
    QNetworkRequest request;    // 请求对象
    InfoContext* login = InfoContext::getInfoContext();     // 获取登录信息

    // cc.com:80/myfiles&cmd=count          // 获取用户文件个数
    QString url = QString("http://%1:%2/myfiles&cmd=count").arg(login->getIp().arg(login->getPort()));
    request.setUrl(QUrl(url));

    // qt 默认请求头
    //request.setRawHeader("Content-Type", "text/html");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray data = this->setGetCountJson(login->getUser(), login->getToken());   // { "user": "dandan" }

    // 发送post请求
    QNetworkReply* reply = manager.post(request, data);
    if (!reply)
    {
        WinPrintA << "reply == NULL";
        return;
    }

    // 获取请求的数据完成时，就会发送信号SIGNAL(finished())
    connect(reply, &QNetworkReply::finished, this, [=]()
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            WinPrintA << reply->errorString();
            reply->deleteLater(); //释放资源
            return;
        }

        // 服务器返回数据
        QByteArray array = reply->readAll();
        reply->deleteLater();   //释放
        
        // 得到服务器json文件
        QStringList list = this->getCountStatus(array);
        
        // token验证失败
        if ("111" == list.at(0))
        {
        	QMessageBox::warning(this, "账户异常", "请重新登录!");
            emit this->loginAgainSignal();
        	return;
        }
        
        // 转换为long
        this->m_userFilesCount = list.at(1).toLong();
        WinPrintA << "userFilesCount = " << this->m_userFilesCount;
        
        // 清空文件列表信息
        this->clearFileList();
        if (this->m_userFilesCount <= 0)
        {   // 没有文件
        	this->refreshFileItems();   // 更新item
        	return;
        }
        
        // 有文件,获取用户文件列表
        this->m_start = 0;      // 从0开始
        this->m_count = 10;     // 每次请求10个
        this->getUserFilesList(cmd);


    });

}

void MyFileWg::getUserFilesList(Display cmd)
{
    // 遍历数目，结束条件处理
    if (this->m_userFilesCount <= 0)
    {
        WinPrintA << "获取用户文件列表条件结束";
        this->refreshFileItems();   // 更新item
        return;
    }
    // 如果请求文件数量大于用户的文件数量
    if (this->m_count > this->m_userFilesCount)
        this->m_count = this->m_userFilesCount;

    QNetworkRequest request; //请求对象
    QNetworkAccessManager& manager = NetworkManager::getNetManager();
    InfoContext* login = InfoContext::getInfoContext();     // 获取登录信息

    // 获取用户文件信息 cc.com:80/myfiles&cmd=normal
    // 按下载量升序 cc.com:80/myfiles?cmd=pvasc
    // 按下载量降序 cc.com:80/myfiles?cmd=pvdesc

    QString tmp;

    if (MyFileWg::Normal == cmd)
        tmp = "normal";
    else if (MyFileWg::PvAsc == cmd)
        tmp = "pvasc";
    else if(MyFileWg::PvDesc == cmd)
        tmp = "pvdesc";
    QString url = QString("http://%1:%2/myfiles&cmd=%3").arg(login->getIp()).arg(login->getPort()).arg(tmp);
    request.setUrl(QUrl(url));

    // qt默认请求头
    //request.setRawHeader("Content-Type", "text/html");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    /*
    {
        "user": "dandan"
        "token": "xxxx"
        "start": 0
        "count": 10
    }
    */
    QByteArray data = this->setFilesListJson(login->getUser(), login->getToken(), this->m_start, this->m_count);

    // 改变文件起点位置
    this->m_start += this->m_count;
    this->m_userFilesCount -= this->m_count;

    // 发送post请求
    QNetworkReply* reply = manager.post(request, data);
    if (!reply)
    {
        WinPrintA << "reply == NULL";
        return;
    }

    // 获取请求的数据完成时，就会发送信号SIGNAL(finished())
    connect(reply, &QNetworkReply::finished, this, [=]()
    {
        if (QNetworkReply::NoError != reply->error())
        {
            WinPrintA << reply->errorString();
            reply->deleteLater();       //释放资源
            return;
        }

        // 服务器返回用户的数据
        QByteArray array = reply->readAll();
        reply->deleteLater();

        // token验证失败
        QString code = NetworkManager::getCode(array);
        if ("111" == code)
        {
            QMessageBox::warning(this, "账户异常", "请重新登录");
            emit this->loginAgainSignal();
            return;
        }

        // 不是错误码就处理文件列表json信息
        if ("015" != code)
        {
            WinPrintA << array.data();
            this->getFileJsonInfo(array);       // 解析文件列表json信息,存放在文件列表中
            this->getUserFilesList(cmd);
        }
    });
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

QByteArray MyFileWg::setGetCountJson(QString user, QString token)
{
    return QByteArray();
}

QByteArray MyFileWg::setFilesListJson(QString user, QString token, int start, int count)
{
    return QByteArray();
}



void MyFileWg::getFileJsonInfo(QByteArray data)
{
    QJsonParseError error;

    /*
        {
        "user": "dandan",
        "md5": "e8ea6031b779ac26c319ddf949ad9d8d",
        "time": "2017-02-26 21:35:25",
        "filename": "test.mp4",
        "share_status": 0,
        "pv": 0,
        "url": "http://cc:80/group1/M00/00/00/wKgfbViy2Z2AJ-FTAaM3As-g3Z0782.mp4",
        "size": 27473666,
         "type": "mp4"
        }
    */
    //-- user	文件所属用户
    //-- md5 文件md5
    //-- createtime 文件创建时间
    //-- filename 文件名字
    //-- shared_status 共享状态, 0为没有共享， 1为共享
    //-- pv 文件下载量，默认值为0，下载一次加1
    //-- url 文件url
    //-- size 文件大小, 以字节为单位
    //-- type 文件类型： png, zip, mp4……

    // 将来源数据json转化为JsonDocument
    // 由QByteArray对象构造一个QJsonDocument对象，用于我们的读写操作
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError)
    {
        WinPrintA << "err = " << error.errorString();
        return;
    }

    if (doc.isNull() || doc.isEmpty() || !doc.isObject())
    {
        WinPrintA << "doc.isNull() || doc.isEmpty() || doc.isObject() error";
        return;
    }

    // QJsonObject json对象，描述json数据中{}括起来部分
    QJsonObject obj = doc.object();//取得最外层这个大对象

    // 获取games所对应的数组
    // QJsonArray json数组，描述json数据中[]括起来部分
    QJsonArray array = obj.value("files").toArray();

    int size = array.size();        // 数组个数
    WinPrintA << "size = " <<size;

    for (size_t i = 0; i < size; ++i)
    {
        /*
            //文件信息
            struct FileInfo
            {
                QString md5;        //文件md5码
                QString filename;   //文件名字
                QString user;       //用户
                QString time;       //上传时间
                QString url;        //url
                QString type;       //文件类型
                qint64 size;        //文件大小
                int shareStatus;    //是否共享, 1共享， 0不共享
                int pv;             //下载量
                QListWidgetItem *item; //list widget 的item
            };

            {
            "user": "dandan",
            "md5": "e8ea6031b779ac26c319ddf949ad9d8d",
            "time": "2017-02-26 21:35:25",
            "filename": "test.mp4",
            "share_status": 0,
            "pv": 0,
            "url": "http://cc:80/group1/M00/00/00/wKgfbViy2Z2AJ-FTAaM3As-g3Z0782.mp4",
            "size": 27473666,
             "type": "mp4"
            }
        */
        FileInfo* info = new FileInfo;
        QJsonObject tmp = array[i].toObject();                  // 取第i个对象
        info->user = tmp.value("user").toString();              // 用户
        info->md5 = tmp.value("md5").toString();                // 文件md5
        info->time = tmp.value("time").toString();              // 上传时间
        info->filename = tmp.value("filename").toString();      // 文件名字
        info->shareStatus = tmp.value("share_status").toInt();  //共享状态
        info->pv = tmp.value("pv").toInt();                     //下载量
        info->url = tmp.value("url").toString();                //url
        info->size = tmp.value("size").toInt();                 //文件大小，以字节为单位
        info->type = tmp.value("type").toString();              //文件后缀

        QString type = info->type + ".png";
        InfoContext* login = InfoContext::getInfoContext();     // 获取登录信息
        info->item = new QListWidgetItem(QIcon(login->getFileType(type)), info->filename);

        // list 添加节点
        this->m_fileList.append(info);
    }
}