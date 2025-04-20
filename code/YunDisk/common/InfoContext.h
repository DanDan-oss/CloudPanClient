#ifndef LOGIN_INFO_H
#define LOGIN_INFO_H

#include <QObject>
#include <QString>
#include <QListWidgetItem>

struct  LoginInfo
{
    QString username;
    QString password;
    QString token;
    QString aeskey;
};

struct  RegisterInfo
{
    QString username;
    QString nickname;
    QString firstpwd;
    QString surepwd;
    QString phone;
    QString email;
};

struct FileInfo
{
    QString md5;            // 文件md5码
    QString filename;       // 文件名字
    QString user;           // 用户
    QString time;           // 上传时间
    QString url;            // url
    QString type;           // 文件类型
    qint64 size;            // 文件大小
    int shareStatus;        // 是否共享, 1共享， 0不共享
    int pv;                 // 下载量
    QListWidgetItem* item;  // list widget 的item
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
    static InfoContext* getInfoContext();		// 获取实例
    static void destroy();	// 释放堆空间
public:
    void  setConfPath(const QString& file_name);
    void  readConfContext();
    void  WriteConfContext();
    const QString& getFileName();
    const LoginInfo& getLoginInfo() const;
    const ServerInfo& getServerInfo() const;
    bool setServerInfo(const ServerInfo& info);
    bool setServerInfo(const QString& ip, const int port);
    // 设置登录信息
    bool setLoginInfo(const QString& username, const QString& password, const QString& token="");
    void setLoginToken(const QString& token = "");

    QString getUser() const;		// 获取登录用户
    QString getIp() const;			// 获取服务器IP
    int getPort() const;		// 获取服务器端口
    QString getToken() const;		// 获取登录token

    void getFileTypeList();             // 通过读取文件, 得到文件类型, 存放在typeList
    // 得到文件后缀，参数为文件类型，函数内部判断是否有此类型，如果有，使用此类型，没有，使用other.png
    QString getFileType(QString type);
signals:

private:
    // 将构造析构函数设为私有
    explicit InfoContext(QObject* parent = nullptr);
    explicit InfoContext(const QString& file_name, QObject* parent = nullptr);
    ~InfoContext();
    // 将复制构造函数和=操作符也设为私有,防止被复制
    InfoContext(const InfoContext&);
    InfoContext& operator=(const InfoContext&);

private:
    //定义一个静态成员变量，程序结束时，系统会自动调用它的析构函数
    // static类的析构函数在main()退出后调用
    class Garbo
    {
    public:
        ~Garbo()
        {
            //释放堆区空间
            InfoContext::destroy();
        }
    };
    static Garbo tmp; //静态数据成员，类中声明，类外定义;

private:
    QString m_file_name;
    LoginInfo m_login_info;
    RegisterInfo m_register_info;
    ServerInfo m_server_info;

    static QString DEFAULT_DES_KEY;
    static QString DEFAULT_AES_KEY;
    static QString DEFAULT_AES_IV;

    //静态数据成员，类中声明，类外必须定义
    static InfoContext* instance;

public:
    static QStringList  m_typeList;

private:
    // 文件类型路径
    static QString      m_typePath;
    // 主要保存文件类型的后缀

};

#endif // LOGIN_INFO_H
