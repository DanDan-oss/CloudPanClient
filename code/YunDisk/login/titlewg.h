#ifndef TITLEWG_H
#define TITLEWG_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>

// title 标题栏
class TitleWg :  public QWidget
{
    Q_OBJECT
public:
    explicit TitleWg (const QRect& rect, QWidget *parent = nullptr);
    ~TitleWg();
    void initScene(const QRect &rect);     // 初始化主界面
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *ev);

private:
    QPoint m_pt;    // 鼠标至当前窗口的相对位置
    QLabel m_logolable;     // login
    QLabel m_title;     // login
    QToolButton m_button_set;
    QToolButton m_button_mix;
    QToolButton m_button_close;
signals:
private slots:
    void on_button_close_clicked();
    void on_button_set_clicked();
    void on_button_mix_clicked();
};


#endif // TITLEWG_H
