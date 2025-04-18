#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>
#include <QToolButton>

enum Page { MYFILE, SHARE, TRANKING, TRANSFER, SWITCHUSR };

class ButtonGroup : public QWidget
{
	Q_OBJECT
public:
	
	explicit ButtonGroup(const QRect& rect, QWidget* parent = nullptr);
	~ButtonGroup();

	void initScene(const QRect& rect);     // 初始化主界面
protected:
	virtual void paintEvent(QPaintEvent* event);		// 绘制事件,绘制背景
	virtual void mousePressEvent(QMouseEvent* event);	// 鼠标点击事件
	virtual void mouseMoveEvent(QMouseEvent* event);	// 鼠标移动事件

public slots:
	// 按钮处理函数
	void slotButtonClick(Page current);
	void slotButtonClick(QString text);

signals:
	void signalMyFile();			// 我的文件按钮点击消息
	void signalShareList();		// 共享文件按钮点击消息
	void signalDownload();		// 下载榜按钮点击消息
	void signalTransfron();		// 传输列表按钮点击消息
	void signalSwitchUser();		// 切换用户按钮点击消息
	void signalCloseWindow();		// 关闭按钮点击消息
	void signalMinWindow();		// 最小化按钮点击消息
	void signalMaxWindow();		// 最大化按钮点击消息

private:
	QToolButton m_login_user;	// 登录用户按钮
	QToolButton m_myfile;		// 我的文件按钮
	QToolButton m_sharelist;	// 共享列表按钮
	QToolButton m_download;		// 下载榜按钮
	QToolButton m_transform;	// 传输列表按钮
	QToolButton m_switch_user;	// 切换用户列表

	QToolButton m_close;	// 关闭按钮
	QToolButton m_max;      // 最大化按钮
	QToolButton m_mix;      // 最小化按钮

	QPoint m_pos;	// 鼠标至当前窗口的相对位置
	QToolButton* m_currentBtn;		// 当前获取焦点的按钮
	QMap<QString, QToolButton*> m_btns;		// 按钮集合
	QMap<Page, QString> m_pages;		// 
};

#endif // BUTTONGROUP_H
