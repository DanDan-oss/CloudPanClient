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
#include <QStackedWidget>
#include "global.h"

class Login;
class TitleWg;
class LoginContext;
class RegisterContext;
class ServerConfig;

// 窗口主场景类===========================
// 登录窗口
class Login  : public QWidget
{
    Q_OBJECT
public:
    explicit Login (QWidget *parent = nullptr);
    ~Login();
protected:
    void initScene();     // 初始化主界面,绘制界面初始化回调函数
    virtual void paintEvent(QPaintEvent* event);    // 绘制事件,绘制背景

public slots:
    void showRegisterPage(Login* login);      // 显示注册窗口
    void showServerConfPage(Login* login);    // 显示配置服务器窗口
    void closeWindow(Login* login);            //
    void showMinWindow(Login* login);            // 关闭窗口按钮被点击


private slots:
    void onRegisterButtonClicked();      // 注册按钮被点击
    void onLoginButtonClicked();         // 登录按钮被点击
    void onServerConfButtonClicked();    // 服务器设置按钮被点击

private:
    QStackedWidget m_stacked_widget;
    TitleWg* m_title_page;
    LoginContext* m_login_page = nullptr;
    RegisterContext* m_register_page = nullptr;
    ServerConfig* m_serverconf_page = nullptr;

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
signals:
    void CloseWindow();
};

// 登录界面
class LoginContext  : public QWidget
{
    Q_OBJECT
public:
    explicit LoginContext (const QRect& rect, QWidget *parent = nullptr);
    void initScene(const QRect &rect);     // 初始化主界面
    void initShowData();
    virtual void paintEvent(QPaintEvent* event);    // 绘制事件,绘制背景
    ~LoginContext();
private:
    QLabel m_title;      // 用户登录标签
    QLabel m_username;      // 用户账户标签
    QLabel m_password;      // 用户密码标签
    QLineEdit m_usertext;      // 用户账户框
    QLineEdit m_passtext;      // 用户密码框
    QCheckBox m_checkpass;      // 是否记住密码
    QToolButton m_button_register;      // 注册按钮
    QToolButton m_button_login;      // 登录按钮

    LoginInfo m_login_info;
};

// 注册界面
class RegisterContext  : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterContext (const QRect& rect, QWidget *parent = nullptr);
    ~RegisterContext();
    void initScene(const QRect &rect);     // 初始化主界面
    void initShowData();                    // 界面显示数据
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

    RegisterInfo m_register_info;

signals:

};

// 注册界面
class ServerConfig  : public QWidget
{
    Q_OBJECT
public:
    explicit ServerConfig (const QRect& rect, QWidget *parent = nullptr);
    ~ServerConfig();
    void initScene(const QRect &rect);     // 初始化主界面
    void initShowData();                    // 界面显示数据
private:
    QLabel m_title;             // 服务器设置标签
    QLabel m_server;             // 服务器设置标签
    QLabel m_port;             // 服务器设置标签

    QLineEdit m_server_address;    // 服务器地址
    QLineEdit m_server_port;       // 服务器端口
    QToolButton m_button_ok;      // 注册按钮

    ServerInfo m_server_info;

signals:

};

#endif // LOGIN_H
