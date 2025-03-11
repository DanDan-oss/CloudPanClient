#include "loginfo.h"
#include <QSettings>
#include <QCryptographicHash>
#include <QDebug>

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

    this->m_server_info.ip = config->value("ServerConf/ip_addr", "").toString();
    this->m_server_info.port = config->value("ServerConf/port", "0").toInt();

    this->m_login_info.username = config->value("UserInfo/username", "").toString();
    this->m_login_info.password = config->value("UserInfo/password", "").toString();
    this->m_login_info.aeskey = config->value("UserInfo/aes_key", "").toString();
    tmp=config->value("UserInfo/save_pass", "false").toString();
    this->m_login_info.savepass = tmp.compare("true", Qt::CaseInsensitive) == 0 ? true : false;

    delete config;
}

void InfoContext::WriteConfContext()
{
    QSettings *config = new QSettings(this->m_file_name, QSettings::IniFormat);
    config->setValue("ServerConf/ip_addr", this->m_server_info.ip);
    config->setValue("ServerConf/port", this->m_server_info.port);

    config->setValue("UserInfo/username", this->m_login_info.username);
    config->setValue("UserInfo/password", this->m_login_info.password);
    config->setValue("UserInfo/aes_key", this->m_login_info.aeskey);
    config->setValue("UserInfo/save_pass", this->m_login_info.savepass == true ? "true": "false");

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

