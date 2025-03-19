#include "network_manager.h"
#include "global.h"
#include <QNetworkAccessManager>

QString NetworkManager::m_typePath = FILETYPEDIR;
QStringList NetworkManager::m_typeList = QStringList();
QNetworkAccessManager* NetworkManager::m_netManager = new QNetworkAccessManager;

QNetworkAccessManager& NetworkManager::getNetManager()
{
	return *m_netManager;
}
