#include "buttongroup.h"
#include "mainwindow.h"
#include <QPainter>
#include <QMovie>
#include <QMouseEvent>

ButtonGroup::ButtonGroup(const QRect& rect, QWidget* parent)
    : QWidget{ parent }
{
    this->initScene(rect);
}

ButtonGroup::~ButtonGroup()
{

}

void ButtonGroup::initScene(const QRect& rect)
{
    int iHeigiht = rect.height(), iWidget = rect.width();
    this->setGeometry(rect);
    //this->setStyleSheet("color:rgb(255, 255, 255)");

    this->setFont(QFont("华文琥珀", 12, QFont::Bold, false));

    this->m_login_user.setText("unknow");
    this->m_login_user.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->m_login_user.setParent(this);
    this->m_login_user.setIcon(QIcon(":/images/title_user.png"));
    this->m_login_user.setIconSize(QSize(70, 70));
    this->m_login_user.setGeometry(0, 0, 77, 97);
    this->m_login_user.setAutoRaise(true);     // 不会自动填充背景色
    this->m_login_user.setPopupMode(QToolButton::DelayedPopup);
    this->m_login_user.show();

    this->m_myfile.setText("我的文件");
    this->m_myfile.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->m_myfile.setParent(this);
    this->m_myfile.setIcon(QIcon(":/images/tile_file.png"));
    this->m_myfile.setIconSize(QSize(50, 50));
    this->m_myfile.setGeometry(83, 10, 79, 77);
    this->m_myfile.setAutoRaise(true);     // 不会自动填充背景色
    this->m_myfile.setPopupMode(QToolButton::DelayedPopup);
    this->m_myfile.show();

    this->m_sharelist.setText("共享列表");
    this->m_sharelist.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->m_sharelist.setParent(this);
    this->m_sharelist.setIcon(QIcon(":/images/title_share.png"));
    this->m_sharelist.setIconSize(QSize(50, 50));
    this->m_sharelist.setGeometry(168, 10, 79, 77);
    this->m_sharelist.setAutoRaise(true);     // 不会自动填充背景色
    this->m_sharelist.setPopupMode(QToolButton::DelayedPopup);
    this->m_sharelist.show();

    this->m_download.setText("下载榜");
    this->m_download.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->m_download.setParent(this);
    this->m_download.setIcon(QIcon(":/images/tile_hot.png"));
    this->m_download.setIconSize(QSize(50, 50));
    this->m_download.setGeometry(253, 10, 63, 77);
    this->m_download.setAutoRaise(true);     // 不会自动填充背景色
    this->m_download.setPopupMode(QToolButton::DelayedPopup);
    this->m_download.show();

    this->m_transform.setText("传输列表");
    this->m_transform.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->m_transform.setParent(this);
    this->m_transform.setIcon(QIcon(":/images/title_data.png"));
    this->m_transform.setIconSize(QSize(50, 50));
    this->m_transform.setGeometry(322, 10, 79, 77);
    this->m_transform.setAutoRaise(true);     // 不会自动填充背景色
    this->m_transform.setPopupMode(QToolButton::DelayedPopup);
    this->m_transform.show();

    this->m_switch_user.setText("切换用户");
    this->m_switch_user.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->m_switch_user.setParent(this);
    this->m_switch_user.setIcon(QIcon(":/images/title_change.png"));
    this->m_switch_user.setIconSize(QSize(50, 50));
    this->m_switch_user.setGeometry(407, 10, 79, 77);
    this->m_switch_user.setAutoRaise(true);     // 不会自动填充背景色
    this->m_switch_user.setPopupMode(QToolButton::DelayedPopup);
    this->m_switch_user.show();

    this->m_close.setParent(this);
    this->m_close.setText("...");
    this->m_close.setIcon(QIcon(":/images/title_close.png"));
    this->m_close.setIconSize(QSize(30, 30));
    this->m_close.setFixedSize(37, 36);
    this->m_close.move(this->width()- this->m_close.width() -20, 20);
    this->m_close.setAutoRaise(true);     // 不会自动填充背景色
    this->m_close.show();

    this->m_max.setParent(this);
    this->m_max.setText("...");
    this->m_max.setIcon(QIcon(":/images/title_max.png"));
    this->m_max.setIconSize(QSize(30, 30));
    this->m_max.setFixedSize(37, 36);
    this->m_max.move(this->m_close.x() - this->m_max.width() -20, 20);
    this->m_max.setAutoRaise(true);     // 不会自动填充背景色
    this->m_max.show();

    this->m_mix.setParent(this);
    this->m_mix.setText("...");
    this->m_mix.setIcon(QIcon(":/images/title_min.png"));
    this->m_mix.setIconSize(QSize(30, 30));
    this->m_mix.setFixedSize(37, 36);
    this->m_mix.move(this->m_max.x() - this->m_max.width() -20,20);
    this->m_mix.setAutoRaise(true);     // 不会自动填充背景色
    this->m_mix.show();

}

void ButtonGroup::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QPixmap bk(":/images/title_bk.jpg");
    painter.drawPixmap(0, 0, width(), height(), bk);
}

void ButtonGroup::mousePressEvent(QMouseEvent* event)
{
    // 如果鼠标左键按下
    if (event->button() == Qt::LeftButton)
    {
        // 鼠标至当前窗口的相对位置 =  鼠标当前屏幕位置 - 当前主窗口左上角点的屏幕位置
        this->m_pos = event->globalPos() - this->parentWidget()->parentWidget()->geometry().topLeft();
    }
}

void ButtonGroup::mouseMoveEvent(QMouseEvent* event)
{
    // 只允许左键拖动
    if (event->buttons() & Qt::LeftButton)
    {
        // 窗口跟随鼠标移动
        // 窗口左上角点 = 鼠标当前位置 - 差值
        this->parentWidget()->parentWidget()->move(event->globalPos() - this->m_pos);
    }
}
