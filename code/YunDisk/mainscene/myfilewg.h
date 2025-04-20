#ifndef MYFILEWG_H
#define MYFILEWG_H

#include <QWidget>
#include <QListWidget>
#include <QTimer>
#include "common/InfoContext.h"

class MyFileWg : public QWidget
{
	Q_OBJECT
public:
	explicit MyFileWg(const QRect& rect, QWidget* parent = nullptr);
	~MyFileWg();

	void initScene(const QRect& rect);			// 初始化主界面
	void initListWidget(const QRect& rect);		// 初始化listWidget文件列表
	void addActionMenu();						// 添加右键菜单
	void initMenuSlots();						// 添加右键菜单槽函数

private:
	// 右键菜单信号的槽函数
	void rightMenu(const QPoint& pos);

public:
	
	void checkTaskList();		// 定时检查处理任务队列中的任务
	void clearAllTask();		// 清除上传下载任务

public:
	// ==========>文件item展示<==============
	void clearFileList();									// 清空文件列表
	void refreshFileItems();								// 文件item展示

public:
	//==========>上传文件处理<==============
	void addUploadFiles();				// 添加需要上传的文件到上传任务列表
	void uploadFilesAction();			// 上传文件处理，取出上传任务列表的队首任务，上传完后，再取下一个任务

	//==========>下载文件处理<==============
	void addDownloadFiles();			// 添加需要下载的文件到下载任务列表
	void downloadFilesAction();			//下载文件处理，取出下载任务列表的队首任务，下载完后，再取下一个任务

public:
	//==========>显示用户的文件列表<==============
	// desc是descend 降序意思
	// asc 是ascend 升序意思
	// Normal：普通用户列表，PvAsc：按下载量升序， PvDesc：按下载量降序
	enum Display { Normal, PvAsc, PvDesc };
	void refreshFiles(MyFileWg::Display cmd = Normal);		// 显示用户的文件列表
	void getUserFilesList(Display cmd = Normal);			// 获取用户文件列表

public:
	QStringList getCountStatus(QByteArray json);					// 得到服务器json文件
	void getFileJsonInfo(QByteArray data);							// 解析文件列表json信息，存放在文件列表中

	QByteArray setGetCountJson(QString user, QString token);		// 设置json包
	QByteArray setFilesListJson(QString user, QString token, int start, int count);		// 设置json包

signals:
	void loginAgainSignal();
	//void gotoTransfer(TransferStatus status);

private:
	QListWidget m_listWidget;

	QMenu* m_menu;			// 菜单1
	QAction* m_downloadAction; // 下载
	QAction* m_shareAction;    // 分享
	QAction* m_delAction;      // 删除
	QAction* m_propertyAction; // 属性

	QMenu* m_menuEmpty;		// 菜单2
	QAction* m_pvAscendingAction;  // 按下载量升序
	QAction* m_pvDescendingAction; // 按下载量降序
	QAction* m_refreshAction;      // 刷新
	QAction* m_uploadAction;       // 上传

	long m_userFilesCount;        //用户文件数目
	int m_start;                  //文件位置起点
	int m_count;                  //每次请求文件个数
	QList<FileInfo*> m_fileList;    //文件列表

	//定时器
	QTimer m_uploadFileTimer;       //定时检查上传队列是否有任务需要上传
	QTimer m_downloadTimer;         //定时检查下载队列是否有任务需要下载
};


#endif // MYFILEWG_H