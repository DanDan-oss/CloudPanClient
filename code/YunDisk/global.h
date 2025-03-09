#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

struct  LoginInfo
{
    QString username;
    QString password;
    bool checkoutpass = false;
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
    QString port;
};

#endif // GLOBAL_H
