#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>

class QToolButton;
enum Page { MYFILE, SHARE, TRANKING, TRANSFER, SWITCHUSR };

class ButtonGroup : public QWidget
{
	Q_OBJECT
public:
	
	explicit ButtonGroup(const QRect& rect, QWidget* parent = nullptr);
	~ButtonGroup();

	void initScene(const QRect& rect);     // 初始化主界面

private:

};

#endif // BUTTONGROUP_H