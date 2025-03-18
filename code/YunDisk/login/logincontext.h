#ifndef LOGINCONTEXT_H
#define LOGINCONTEXT_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QLineEdit>
#include <QCheckBox>
#include "common/loginfo.h"

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
    bool sendLoginMessage(const LoginInfo& info);     // 使用HTTP发送注册消息
private:
    QLabel m_title;      // 用户登录标签
    QLabel m_username;      // 用户账户标签
    QLabel m_password;      // 用户密码标签
    QLineEdit m_usertext;      // 用户账户框
    QLineEdit m_passtext;      // 用户密码框
    QCheckBox m_checkpass;      // 是否记住密码
    QToolButton m_button_register;      // 注册按钮
    QToolButton m_button_login;      // 登录按钮
private slots:
    void on_button_registe_clicked();
    void on_button_login_clicked();

};

#endif // LOGINCONTEXT_H
