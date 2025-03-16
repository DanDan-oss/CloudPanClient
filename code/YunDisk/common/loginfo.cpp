#include "loginfo.h"
#include "cryptutil.h"
#include <QSettings>
#include <QDebug>

QString InfoContext::DEFAULT_DES_KEY = "32141123";//block size 8
QString InfoContext::DEFAULT_AES_KEY = "abcdefg123456732";//block size 16
QString InfoContext::DEFAULT_AES_IV = "abcdefg123456732";//block size 16

InfoContext::InfoContext(QObject *parent)
    : QObject{parent}
{

}

InfoContext::InfoContext(const QString &file_path, QObject *parent)
    : QObject{parent}, m_file_name{file_path}
{
    this->readConfContext();
}

void InfoContext::setConfPath(const QString &file_name)
{
    this->m_file_name = file_name;
}

void InfoContext::readConfContext()
{
    QString tmp;
    QSettings *config = new QSettings(this->m_file_name, QSettings::IniFormat);
    LoginInfo logininfo = { 0 };

    this->m_server_info.ip = config->value("ServerConf/ip_addr", "").toString();
    this->m_server_info.port = config->value("ServerConf/port", "0").toInt();

    logininfo.username= config->value("UserInfo/username", "").toString();
    logininfo.password = config->value("UserInfo/password", "").toString();
    logininfo.aeskey = config->value("UserInfo/aes_key", "").toString();

    if (logininfo.aeskey.size() == 16 || logininfo.aeskey.size() == 32 && logininfo.aeskey.size()== 24)
    {
        logininfo.username = CryptUtil::aesDecryptText(logininfo.username, logininfo.aeskey, InfoContext::DEFAULT_AES_IV);
        logininfo.password = CryptUtil::aesDecryptText(logininfo.password, logininfo.aeskey, InfoContext::DEFAULT_AES_IV);
    }

    this->m_login_info.username = logininfo.username;
    this->m_login_info.password = logininfo.password;
    this->m_login_info.aeskey = logininfo.aeskey;
    delete config;
}

void InfoContext::WriteConfContext()
{
    QSettings *config = new QSettings(this->m_file_name, QSettings::IniFormat);
    LoginInfo logininfo = { 0 };

    logininfo.aeskey = this->m_login_info.aeskey;
    logininfo.username = this->m_login_info.username;
    logininfo.password = this->m_login_info.password;


    if (!(logininfo.aeskey.size() == 16 || logininfo.aeskey.size() == 32 && logininfo.aeskey.size() == 24))
        logininfo.aeskey = InfoContext::DEFAULT_AES_KEY;
    logininfo.username = CryptUtil::aesEncryptText(logininfo.username, logininfo.aeskey, InfoContext::DEFAULT_AES_IV);
    logininfo.password = CryptUtil::aesEncryptText(logininfo.password, logininfo.aeskey, InfoContext::DEFAULT_AES_IV);

    config->setValue("ServerConf/ip_addr", this->m_server_info.ip);
    config->setValue("ServerConf/port", this->m_server_info.port);

    config->setValue("UserInfo/username", logininfo.username);
    config->setValue("UserInfo/password", logininfo.password);
    config->setValue("UserInfo/aes_key", logininfo.aeskey);

    delete config;
}

const QString& InfoContext::getFileName()
{
    return this->m_file_name;
}

const LoginInfo& InfoContext::getLoginInfo() const
{
    return this->m_login_info;
}

const ServerInfo &InfoContext::getServerInfo() const
{
    return this->m_server_info;
}

bool InfoContext::setServerInfo(const ServerInfo & info)
{
    return this->setServerInfo(info.ip, info.port);
}

bool InfoContext::setServerInfo(const QString &ip, const int port)
{
    this->m_server_info.ip=ip;
    this->m_server_info.port = port;
    return true;
}


bool InfoContext::setLoginInfo(const QString& username, const QString& password, const QString& aeskey)
{

    this->m_login_info.username = username;
    this->m_login_info.password = password;
    return true;
}

bool InfoContext::setLoginInfo(const LoginInfo &info)
{
    this->m_login_info.username = info.username;
    this->m_login_info.password = info.password;
    return true;
}

