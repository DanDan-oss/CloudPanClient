#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include "infocontext.h"

// 服务器配置界面
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

#endif // SERVERCONFIG_H
