#include "network_manager.h"
#include "global.h"
#include <QString>
#include <QNetworkAccessManager>
#include <QJsonObject>>
#include <QJsonDocument>

QString NetworkManager::m_typePath = FILETYPEDIR;
QStringList NetworkManager::m_typeList = QStringList();
QNetworkAccessManager* NetworkManager::m_netManager = new QNetworkAccessManager;

QNetworkAccessManager& NetworkManager::getNetManager()
{

	return *m_netManager;
}

QString NetworkManager::getServerRecvCode(QByteArray json)
{
	QJsonParseError error;

	// 将来源数据json转化为JsonDocument
	// 由QByteArray对象构造一个QJsonDocument对象，用于我们的读写操作
	QJsonDocument doc = QJsonDocument::fromJson(json, &error);
	if (error.error == QJsonParseError::NoError)
	{
		if (doc.isNull() || doc.isEmpty())
		{
			WinPrintA << "doc.isNull() || doc.isEmpty()";
			return "";
		}
		if (doc.isObject())
		{
			// 取得最外层这个大对象
			QJsonObject obj = doc.object();
			return obj.value("code").toString();
		}
	}
	WinPrintA << "err = " << error.errorString();
	return "";
}
