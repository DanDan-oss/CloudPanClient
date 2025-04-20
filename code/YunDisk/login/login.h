#ifndef LOGIN_H
#define LOGIN_H

#include "titlewg.h"
#include "logincontext.h"
#include "registercontext.h"
#include "serverconfig.h"
#include <QWidget>
#include <QStackedWidget>


// 窗口主场景类===========================
// 登录窗口
class Login  : public QWidget
{
    Q_OBJECT
public:
    explicit Login (QWidget *parent = nullptr);
    ~Login();
    void showWindow();

protected:
    void initScene();     // 初始化主界面,绘制界面初始化回调函数
    virtual void paintEvent(QPaintEvent* event);    // 绘制事件,绘制背景

public slots:
    void showRegisterPage();      // 显示注册窗口
    void showServerConfPage();    // 显示配置服务器窗口
    void closeWindow();            //
    void showMinWindow();            // 关闭窗口按钮被点击

private slots:
private:
    QStackedWidget m_stacked_widget;
    TitleWg* m_title_page;
    LoginContext* m_login_page = nullptr;
    RegisterContext* m_register_page = nullptr;
    ServerConfig* m_serverconf_page = nullptr;

signals:
};



#endif // LOGIN_H
