#ifndef REGISTERCONTEXT_H
#define REGISTERCONTEXT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include "common/loginfo.h"

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
    bool sendRegisterMessage(const RegisterInfo& info);     // 使用HTTP发送注册消息
    QByteArray setRegisterJson(const RegisterInfo& info);   // 设置注册用户信息的json包
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
    QLineEdit m_phone_text;      // 手机信息框
    QLineEdit m_email_text;      // 邮箱信息框
    QToolButton m_button_register;      // 注册按钮


signals:
private slots:
    void on_button_registe_clicked();
};

#endif // REGISTERCONTEXT_H
