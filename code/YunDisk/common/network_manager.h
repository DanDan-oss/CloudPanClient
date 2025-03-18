#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#include "loginfo.h"
#include <QApplication>
#include <QNetworkAccessManager>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    static QNetworkAccessManager& getNetManager();    // 得到http通信类对象
    static QStringList getLoginStatus(QByteArray json);
    static QString getRegisterStatus(QByteArray json);
    static QByteArray setLoginJson(const LoginInfo& info);   // 设置登录用户信息的json包
    static QByteArray setRegisterJson(const RegisterInfo& info);  // 设置注册用户信息的json包
public:
    static QStringList  m_typeList;

private:
    // 文件类型路径
    static QString      m_typePath;
    // 主要保存文件类型的后缀
    // http类
    static QNetworkAccessManager* m_netManager;
};


#endif // NETWORK_MANAGER_H