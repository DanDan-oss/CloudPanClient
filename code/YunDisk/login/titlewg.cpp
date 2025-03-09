#include "titlewg.h"
#include "login.h"
#include "macro.h"
#include <QLabel>
#include <QToolButton>
#include <QMouseEvent>

TitleWg::TitleWg(const QRect& rect, QWidget *parent)
    : QWidget{parent}
{
    this->initScene(rect);

    connect(&this->m_button_close, &QToolButton::clicked, [=]()
    {   // 关闭按钮被点击
        Login* login = (Login*)(this->parent());
        emit login->closeWindow(login);
    });

    connect(&this->m_button_set, &QToolButton::clicked, this, [=]()
    {   // 服务器设置按钮被点击(右上角的小螺丝) 切换到注册界

        Login* login = (Login*)(this->parent());
        emit login->showServerConfPage(login);
        // 发送信号给父窗口切换到服务器设置窗口
    });

    connect(&this->m_button_mix, &QToolButton::clicked, [=]()
    {   // 窗口最小化按钮被点击
        Login* login = (Login*)(this->parent());
        emit login->showMinWindow(login);
    });
}

TitleWg::~TitleWg()
{

}

void TitleWg::initScene(const QRect &rect)
{
    QPixmap pixmap(WINDOW_LOGO_PATH);
    QSize btn_icon = {32, 32};


    this->setGeometry(rect);
    this->setStyleSheet("color:rgb(255, 255, 255)");

    this->m_logolable.setParent(this);
    this->m_logolable.setPixmap(pixmap.scaled(40, 40));
    this->m_logolable.setGeometry(30, 30 ,40, 40);

    this->m_title.setParent(this);
    this->m_title.setText(QString("CloudPan"));
    //this->m_title.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_title.setFont(QFont("新宋体", 14, QFont::Bold, false));
    this->m_title.setStyleSheet("color:#ff6600;");   // 设置颜色
    this->m_title.setGeometry(this->m_logolable.width()+this->m_logolable.x()+20,this->m_logolable.y(), 100, 40);

    this->m_button_close.setParent(this);
    this->m_button_close.setText("...");
    this->m_button_close.setIcon(QIcon(":/images/login_close.png"));
    this->m_button_close.setIconSize(btn_icon);
    this->m_button_close.setFixedSize(39, 38);
    this->m_button_close.move(this->width()- this->m_button_close.width() -20, this->m_logolable.y());
    this->m_button_close.setAutoRaise(true);     // 不会自动填充背景色
    this->m_button_close.show();

    this->m_button_mix.setParent(this);
    this->m_button_mix.setText("...");
    this->m_button_mix.setIcon(QIcon(":/images/login_min.png"));
    this->m_button_mix.setIconSize(btn_icon);
    this->m_button_mix.setFixedSize(39, 38);
    this->m_button_mix.move(this->m_button_close.x() - this->m_button_mix.width() -20, this->m_logolable.y());
    this->m_button_mix.setAutoRaise(true);     // 不会自动填充背景色
    this->m_button_mix.show();

    this->m_button_set.setParent(this);
    this->m_button_set.setText("...");
    this->m_button_set.setIcon(QIcon(":/images/login_setting.png"));
    this->m_button_set.setIconSize(btn_icon);
    this->m_button_set.setFixedSize(39, 38);
    this->m_button_set.move(this->m_button_mix.x() - this->m_button_mix.width() -20,this->m_logolable.y());
    this->m_button_set.setAutoRaise(true);     // 不会自动填充背景色
    this->m_button_set.show();


    //this->setStyleSheet("border: 2px solid #ff0000;");
}


void TitleWg::mouseMoveEvent(QMouseEvent *event)
{
    // 只允许左键拖动
    if(event->buttons() & Qt::LeftButton)
    {
        // 窗口跟随鼠标移动
        // 窗口左上角点 = 鼠标当前位置 - 差值

        this->parentWidget()->parentWidget()->move(event->globalPos() - m_pt);
    }
}

void TitleWg::mousePressEvent(QMouseEvent *ev)
{
    // 如果鼠标左键按下
    if(ev->button() == Qt::LeftButton)
    {
        // 鼠标至当前窗口的相对位置 =  鼠标当前屏幕位置 - 当前主窗口左上角点的屏幕位置
        m_pt = ev->globalPos() - this->parentWidget()->parentWidget()->geometry().topLeft();
    }
}
