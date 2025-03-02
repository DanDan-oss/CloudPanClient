#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QLabel>
#include <QDialog>

// 窗口主场景类===========================
// 登录窗口
class Login  : public QWidget
{
    Q_OBJECT
public:
    explicit Login (QWidget *parent = nullptr);
    ~Login();
    void initScene();     // 初始化主界面,绘制界面初始化回调函数
    virtual void paintEvent(QPaintEvent* event);    // 绘制事件,绘制背景
private:
    QWidget* m_user_context = nullptr;
    QWidget* m_title = nullptr;
signals:

};

// title
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
    QPoint m_pt;    // 差值: 鼠标当前位置 - 窗口左上角点
    QWidget* m_logolable = nullptr;     // login
    QWidget* m_title = nullptr;     // login
    QWidget* m_button_set = nullptr;
    QWidget* m_button_mix = nullptr;
    QWidget* m_button_close = nullptr;
};

#endif // SCENE_H

class UserContext  : public QWidget
{
    Q_OBJECT
public:
    explicit UserContext (const QRect& rect, QWidget *parent = nullptr);
    void initScene(const QRect &rect);     // 初始化主界面
    virtual void paintEvent(QPaintEvent* event);    // 绘制事件,绘制背景
    ~UserContext();
private:
    QWidget* m_title = nullptr;      // 用户登录标签
    QWidget* m_username = nullptr;      // 用户账户标签
    QWidget* m_password = nullptr;      // 用户密码标签
    QWidget* m_usertext = nullptr;      // 用户账户框
    QWidget* m_passtext = nullptr;      // 用户密码框
    QWidget* m_checkpass = nullptr;      // 是否记住密码
    QWidget* m_button_register = nullptr;      // 注册按钮
    QWidget* m_button_login = nullptr;      // 注册按钮

signals:

};


