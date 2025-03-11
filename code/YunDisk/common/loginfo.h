#ifndef LOGIN_INFO_H
#define LOGIN_INFO_H

#include <QObject>
#include <QString>

struct  LoginInfo
{
    QString username;
    QString password;
    QString aeskey;
    bool savepass = false;
};

struct  RegisterInfo
{
    QString username;
    QString nickname;
    QString password;
    QString re_password;
    QString phone;
    QString email;
};

struct  ServerInfo
{
    QString ip;
    int port;
};

class InfoContext : public QObject
{
    Q_OBJECT
public:
    explicit InfoContext(QObject *parent = nullptr);
    explicit InfoContext(const QString& file_name, QObject *parent = nullptr);
    void  setConfPath(const QString& file_name);
    void  readConfContext();
    void  WriteConfContext();
    const QString& getFileName();
    const LoginInfo& getLoginInfo() const;
    const ServerInfo& getServerInfo() const;
    bool setServerInfo(const ServerInfo& info);
    bool setServerInfo(const QString& ip, const int port);
private:

signals:
private:
    QString m_file_name;
    LoginInfo m_login_info;
    RegisterInfo m_register_info;
    ServerInfo m_server_info;
};

#endif // LOGIN_INFO_H
