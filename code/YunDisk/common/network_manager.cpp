#include "network_manager.h"
#include "global.h"
#include "common/cryptutil.h"
#include <QString>
#include <QNetworkAccessManager>
#include <QJsonObject>>
#include <QJsonDocument>
#include <QStringList>

QString NetworkManager::m_typePath = FILETYPEDIR;
QStringList NetworkManager::m_typeList = QStringList();
QNetworkAccessManager* NetworkManager::m_netManager = new QNetworkAccessManager;

QNetworkAccessManager& NetworkManager::getNetManager()
{
	return *m_netManager;
}

// 得到服务器回复的登陆状态， 状态码返回值为 "000", 或 "001"，还有登陆section
QStringList NetworkManager::getLoginStatus(QByteArray json)
{
	QJsonParseError error;
	QStringList list;

	// 将来源数据json转化为JsonDocument
	// 由QByteArray对象构造一个QJsonDocument对象，用于我们的读写操作
	QJsonDocument doc = QJsonDocument::fromJson(json, &error);

	if (error.error != QJsonParseError::NoError)
	{
		WinPrintA << "err = " << error.errorString();
		return list;
	}

	if (doc.isNull() || doc.isEmpty() || false == doc.isObject() )
	{
		WinPrintA << "doc.isNull() || doc.isEmpty() || doc.isObject() == null";
		return list;
	}
	
	//取得最外层这个大对象
	QJsonObject obj = doc.object();
	WinPrintA << "recv server code  " << obj.value("code").toString();
	//状态码
	list.append(obj.value("code").toString());
	//登陆token
	list.append(obj.value("token").toString());
	return list;
}

QString NetworkManager::getRegisterStatus(QByteArray json)
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

	if (doc.isNull() || doc.isEmpty() || false == doc.isObject( ))
	{
		WinPrintA << "doc.isNull() || doc.isEmpty() || doc.isObject() == null";
		return "";
	
	}
	// 取得最外层这个大对象
	QJsonObject obj = doc.object();
	return obj.value("code").toString();
}

QByteArray NetworkManager::setLoginJson(const LoginInfo& info)
{
	QMap<QString, QVariant> login;
	login.insert("user", info.username);
	// 密码使用MD5加密
	login.insert("pwd", CryptUtil::md5Text(info.password));

	/*json数据如下
		{
			user:xxxx,
			pwd:xxx
		}
	*/

	QJsonDocument jsonDocument = QJsonDocument::fromVariant(login);
	if (jsonDocument.isNull())
	{
		WinPrintA << " jsonDocument.isNull() ";
		return "";
	}
	return QByteArray();
}

QByteArray NetworkManager::setRegisterJson(const RegisterInfo& info)
{
	QMap<QString, QVariant> reg;
	reg.insert("userName", info.username);
	reg.insert("nickName", info.nickname);
	reg.insert("firstPwd", CryptUtil::md5Text(info.firstpwd));
	reg.insert("phone", info.phone);
	reg.insert("email", info.email);

	/*json数据如下
		{
			userName:xxxx,
			nickName:xxx,
			firstPwd:xxx,
			phone:xxx,
			email:xxx
		}
	*/

	QJsonDocument jsonDocument = QJsonDocument::fromVariant(reg);
	if (jsonDocument.isNull())
	{
		WinPrintA << " jsonDocument.isNull() ";
		return "";
	}
	//WinPrintA << jsonDocument.toJson().data();

	return jsonDocument.toJson();
}
