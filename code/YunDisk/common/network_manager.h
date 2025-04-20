#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#include <QApplication>
#include <QNetworkAccessManager>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    static QNetworkAccessManager& getNetManager();      // 得到http通信类对象
    static QString getCode(QByteArray json);		    // 得到服务器回复的状态码， 返回值为 "000", 或 "001"

private:
    // http类
    static QNetworkAccessManager* m_netManager;
};


#endif // NETWORK_MANAGER_H