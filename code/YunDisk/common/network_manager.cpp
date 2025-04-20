#include "network_manager.h"
#include "global.h"
#include <QNetworkAccessManager>
#include <QJsonParseError>
#include <QJsonObject>

QNetworkAccessManager* NetworkManager::m_netManager = new QNetworkAccessManager;

QNetworkAccessManager& NetworkManager::getNetManager()
{
	return *m_netManager;
}

QString NetworkManager::getCode(QByteArray json)
{
    QJsonParseError error;

    // 将来源数据json转化为JsonDocument
    // 由QByteArray对象构造一个QJsonDocument对象，用于我们的读写操作

    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if (error.error != QJsonParseError::NoError)
    {
        WinPrintA << "err = " << error.errorString();
        return "";
    }

    if (doc.isNull() || doc.isEmpty() || !doc.isObject())
    {
        WinPrintA << "doc.isNull() || doc.isEmpty() || doc.isObject() error";
        return "";
    }

    // 取得最外层这个大对象
    QJsonObject obj = doc.object();
    return obj.value("code").toString();
}