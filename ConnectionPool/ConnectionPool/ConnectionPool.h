#pragma once
#include<queue>
#include<mutex>
#include<condition_variable>
#include"Mysqlconn.h"
using namespace std;
class ConnectionPoll
{
public:
	//通过静态方法 得到唯一的单例对象
	static ConnectionPoll* getConnectionPoll();
	ConnectionPoll(const ConnectionPoll& obj) = delete;
	ConnectionPoll& operation(const ConnectionPoll& obj) = delete;
	shared_ptr<Mysqlconn> getConnection();
	~ConnectionPoll();
private:
	//构造函数
	ConnectionPoll();

	//解析json 配置文件
	bool parseJsonFile();
	//线程的任务函数
	void produceConnection();
	void recycleConnection();
	//添加连接
	void addConnection();
	//写入json 配置文件里
	string m_ip;
	string m_user;
	string m_password;
	string m_dbName;
	unsigned short m_port;
	//数据库连接最大最小值
	int m_minSize;
	int m_maxSize;
	//超时时长
	int m_timeout;
	//最大空闲时长
	int m_maxIdleTime;
	//锁
	queue<Mysqlconn*>m_connectionQ;
	mutex m_mutexQ;

	condition_variable m_cond;
};

