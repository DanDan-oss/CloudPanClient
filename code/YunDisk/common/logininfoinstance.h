#ifndef LOGIN_INFO_INSTANCE_H
#define LOGIN_INFO_INSTANCE_H

#include <QString>

//单例类
class LoginInfoInstance 
{
public:
	static LoginInfoInstance* getInstance();		// 获取实例
	static void destroy();	// 释放堆空间

	void setLoginInfo(QString tmpUser, QString tmpIp, QString tmpPort, QString token = "");	// 设置登录信息
	QString getUser() const;		// 获取登录用户
	QString getIp() const;			// 获取服务器IP
	QString getPort() const;		// 获取服务器端口
	QString getToken() const;		// 获取登录token

private:
	// 将构造析构函数设为私有
	LoginInfoInstance();
	~LoginInfoInstance();
	// 将复制构造函数和=操作符也设为私有,防止被复制
	LoginInfoInstance(const LoginInfoInstance&);
	LoginInfoInstance& operator=(const LoginInfoInstance&);

private:
	//定义一个静态成员变量，程序结束时，系统会自动调用它的析构函数
	// static类的析构函数在main()退出后调用
	class Garbo
	{
	public:
		~Garbo()
		{
			//释放堆区空间
			LoginInfoInstance::destroy();
		}
	};
	static Garbo tmp; //静态数据成员，类中声明，类外定义;

private:
	QString user;   //当前登陆用户
	QString token;  //登陆token
	QString ip;     //web服务器ip
	QString port;   //web服务器端口

	//静态数据成员，类中声明，类外必须定义
	static LoginInfoInstance* instance;
};


#endif // LOGIN_INFO_INSTANCE_H