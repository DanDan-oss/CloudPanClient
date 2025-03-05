#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLabel>
#include <QDialog>
#include <QLabel>
#include <QToolButton>
#include <QCheckBox>
#include <QStackedWidget>
#include <QLineEdit>

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
    QWidget* m_stacked_widget = nullptr;
    QWidget* m_title = nullptr;
    QWidget* m_user_context = nullptr;
    QWidget* m_register_context = nullptr;

signals:

};

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
};

// 登录界面
class UserContext  : public QWidget
{
    Q_OBJECT
public:
    explicit UserContext (const QRect& rect, QWidget *parent = nullptr);
    void initScene(const QRect &rect);     // 初始化主界面
    virtual void paintEvent(QPaintEvent* event);    // 绘制事件,绘制背景
    ~UserContext();
private:
    QLabel m_title;      // 用户登录标签
    QLabel m_username;      // 用户账户标签
    QLabel m_password;      // 用户密码标签
    QLineEdit m_usertext;      // 用户账户框
    QLineEdit m_passtext;      // 用户密码框
    QCheckBox m_checkpass;      // 是否记住密码
    QToolButton m_button_register;      // 注册按钮
    QToolButton m_button_login;      // 登录按钮

signals:

};

// 注册界面
class RegisterContext  : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterContext (const QRect& rect, QWidget *parent = nullptr);
    ~RegisterContext();
    void initScene(const QRect &rect);     // 初始化主界面
private:
    QLabel m_title;      // 用户注册标签
    QLabel m_username;      // 用户账户标签
    QLabel m_nickname;      // 用户昵称标签
    QLabel m_password;      // 用户密码标签
    QLabel m_confirm_password;      // 确定密码标签
    QLabel m_phone;         // 手机标签
    QLabel m_email;         // 邮箱标签

    QLineEdit m_usertext;      // 用户账户框
    QLineEdit m_nicktext;      // 用户昵称框
    QLineEdit m_passtext;      // 用户密码框
    QLineEdit m_confirm_text;      // 确定密码框
    QLineEdit m_phone_text;      // 手机密码框
    QLineEdit m_email_text;      // 邮箱框
    QToolButton m_button_register;      // 注册按钮

signals:

};

#endif // LOGIN_H
