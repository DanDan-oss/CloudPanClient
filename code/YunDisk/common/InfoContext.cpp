#include "InfoContext.h"
#include "global.h"
#include "cryptutil.h"
#include <QSettings>
#include <QDebug>
#include <QDir>

QString InfoContext::DEFAULT_DES_KEY = CryptUtil::DEFAULT_DES_KEY;  //block size 8
QString InfoContext::DEFAULT_AES_KEY = CryptUtil::DEFAULT_AES_KEY   ;//block size 16
QString InfoContext::DEFAULT_AES_IV = CryptUtil::DEFAULT_AES_IV;   //block size 16

QString InfoContext::m_typePath = FILETYPEDIR;
QStringList InfoContext::m_typeList = QStringList();

InfoContext::Garbo InfoContext::tmp;
InfoContext* InfoContext::instance = new InfoContext();

InfoContext* InfoContext::getInfoContext()
{
    return InfoContext::instance;
}

void InfoContext::destroy()
{
    if (InfoContext::instance)
    {
        delete InfoContext::instance;
        InfoContext::instance = NULL;
        qDebug() << "instance is detele";
    }
}

InfoContext::InfoContext(QObject* parent)
    : QObject{ parent }

{
    this->setConfPath(QString("./conf/Config.ini"));
    this->readConfContext();
}

InfoContext::InfoContext(const QString &file_path, QObject *parent)
    : QObject{parent}, m_file_name{file_path}
{
    this->readConfContext();
}

InfoContext::~InfoContext()
{

}

InfoContext::InfoContext(const InfoContext&)
{
    
}

InfoContext& InfoContext::operator=(const InfoContext&)
{
    // TODO: 在此处插入 return 语句
    return *this;
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

QString InfoContext::getUser() const
{
    return QString(this->m_login_info.username);
}

QString InfoContext::getIp() const
{
    return QString(this->m_server_info.ip);
}

int InfoContext::getPort() const
{
    return this->m_server_info.port;
}

QString InfoContext::getToken() const
{
    return QString(this->m_login_info.token);
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


bool InfoContext::setLoginInfo(const QString& username, const QString& password, const QString& token)
{
    if(username.size() >0 )
        this->m_login_info.username = username;
    if (password.size() > 0)
        this->m_login_info.password = password;
    if (token.size() > 0)
        this->m_login_info.token = token;
    return true;
}

void InfoContext::setLoginToken(const QString& token)
{
    if (token.size() > 0)
        this->m_login_info.token = token;
}

QString InfoContext::getFileType(QString type)
{
    if (true == this->m_typeList.contains(type))
        return this->m_typePath + "/" + type;
    return this->m_typePath + "/other.png";
}

void InfoContext::getFileTypeList()
{
    /*
        QDir::Dirs      列出目录；
        QDir::AllDirs   列出所有目录，不对目录名进行过滤；
        QDir::Files     列出文件；
        QDir::Drives    列出逻辑驱动器名称，该枚举变量在Linux/Unix中将被忽略；
        QDir::NoSymLinks        不列出符号链接；
        QDir::NoDotAndDotDot    不列出文件系统中的特殊文件.及..；
        QDir::NoDot             不列出.文件，即指向当前目录的软链接
        QDir::NoDotDot          不列出..文件；
        QDir::AllEntries        其值为Dirs | Files | Drives，列出目录、文件、驱动器及软链接等所有文件；
        QDir::Readable      列出当前应用有读权限的文件或目录；
        QDir::Writable      列出当前应用有写权限的文件或目录；
        QDir::Executable    列出当前应用有执行权限的文件或目录；
        Readable、Writable及Executable均需要和Dirs或Files枚举值联合使用；
        QDir::Modified      列出已被修改的文件，该值在Linux/Unix系统中将被忽略；
        QDir::Hidden        列出隐藏文件；
        QDir::System        列出系统文件；
        QDir::CaseSensitive 设定过滤器为大小写敏感。
    */

    // QDir类使用相对或绝对文件路径来指向一个文件/目录。
    QDir dir(this->m_typePath);
    if (!dir.exists())
    {
        dir.mkdir(this->m_typePath);
        WinPrintA << this->m_typePath << " 创建成功!!!";
    }
    dir.setFilter(QDir::Files | QDir::NoDot | QDir::NoDotDot | QDir::NoSymLinks);       // 过滤文件
    dir.setSorting(QDir::Size | QDir::Reversed);        // 排序

    QFileInfoList list = dir.entryInfoList();
    for (size_t i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        this->m_typeList.append(fileInfo.fileName());
    }

}




