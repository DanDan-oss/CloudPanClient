#include "logininfoinstance.h"
#include <QDebug>

LoginInfoInstance::Garbo LoginInfoInstance::tmp;
LoginInfoInstance* LoginInfoInstance::instance = new LoginInfoInstance();

LoginInfoInstance* LoginInfoInstance::getInstance()
{
    return LoginInfoInstance::instance;
}

void LoginInfoInstance::destroy()
{
    if (LoginInfoInstance::instance)
    {
        delete LoginInfoInstance::instance;
        LoginInfoInstance::instance = NULL;
        qDebug() << "instance is detele";
    }
}

LoginInfoInstance::LoginInfoInstance()
{

}

LoginInfoInstance::~LoginInfoInstance()
{

}

LoginInfoInstance::LoginInfoInstance(const LoginInfoInstance&)
{

}

LoginInfoInstance& LoginInfoInstance::operator=(const LoginInfoInstance&)
{
    // TODO: 在此处插入 return 语句
    return *this;
}


void LoginInfoInstance::setLoginInfo(QString tmpUser, QString tmpIp, QString tmpPort, QString token)
{
    this->user = tmpUser;
    this->ip = tmpIp;
    this->port = tmpPort;
    this->token = token;
}

QString LoginInfoInstance::getUser() const
{
    return QString(this->user);
}

QString LoginInfoInstance::getIp() const
{
    return QString(this->ip);
}

QString LoginInfoInstance::getPort() const
{
    return QString(this->port);
}

QString LoginInfoInstance::getToken() const
{
    return QString(this->token);
}
