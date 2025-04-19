#ifndef MYFILEWG_H
#define MYFILEWG_H

#include <QWidget>
#include <QListWidget>

class MyFileWg : public QWidget
{
	Q_OBJECT
public:
	explicit MyFileWg(const QRect& rect, QWidget* parent = nullptr);
	~MyFileWg();

	void initScene(const QRect& rect);     // 初始化主界面
	void initListWidget();		// 初始化listWidget文件列表
	void addActionMenu();		// 添加右键菜单
	void initMenuSlots();		// 添加右键菜单槽函数

public:
	//==========>上传文件处理<==============
	// 添加需要上传的文件到上传任务列表
	void addUploadFiles();

private:
	// 右键菜单信号的槽函数
	void rightMenu(const QPoint& pos);

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
};


#endif // MYFILEWG_H