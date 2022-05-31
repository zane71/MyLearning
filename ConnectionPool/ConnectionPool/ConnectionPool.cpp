#include "ConnectionPool.h"
#include<json/json.h>
#include<fstream>
#include<thread>
using namespace Json;
ConnectionPoll* ConnectionPoll::getConnectionPoll()
{
	static ConnectionPoll pool;
	return &pool;
}
//智能指针管理
shared_ptr<Mysqlconn> ConnectionPoll::getConnection()
{
	//看连接池是否为空的 为空阻塞一会
	unique_lock<mutex>locker(m_mutexQ);
	while (m_connectionQ.empty()) {
		if (cv_status::timeout == m_cond.wait_for(locker, chrono::milliseconds(m_timeout))){
			if(m_connectionQ.empty()) 
			{
				//return nullptr;
				continue;
			}
		}
	}
	//共享指针 回收 析构 重新放回到队列
	shared_ptr<Mysqlconn> connptr(m_connectionQ.front(), [this] (Mysqlconn* conn){
		lock_guard<mutex>locker(m_mutexQ);
		m_connectionQ.push(conn);
		conn->refreshAliveTime();
		});
	m_connectionQ.pop();
	m_cond.notify_all();
	return connptr;
}

ConnectionPoll::~ConnectionPoll()
{
	while (!m_connectionQ.empty()) {
		Mysqlconn* conn = m_connectionQ.front();
		m_connectionQ.pop();
		delete conn;
	}
}

bool ConnectionPoll::parseJsonFile()
{
	ifstream ifs("dbconf.json");
	Reader rd;
	Value root;
	rd.parse(ifs, root);
	//是否未json对象
	//解析json对象
	if (root.isObject()) {
		m_ip = root["ip"].asString();
		m_port = root["port"].asInt();
		m_user = root["userName"].asString();
		m_password = root["password"].asString();
		m_dbName = root["dbName"].asString();
		m_minSize = root["minSize"].asInt();
		m_maxSize = root["maxSize"].asInt();
		m_maxIdleTime = root["maxIdleTime"].asInt();
		m_timeout = root["timeout"].asInt();
		return true;

	}
	return false;
}

void ConnectionPoll::produceConnection()
{
	while (true) {
		//判断连接池的连接是否够用
		//当连接队列里连接个数小于最小连接数的时候
		//涉及到多线程访问 加锁
		unique_lock<mutex>locker(m_mutexQ);
		while (m_connectionQ.size()>=m_minSize) {
			//条件变量阻塞
			m_cond.wait(locker);
		}
		//创建新的连接
		addConnection();
		//调用条件变量唤醒函数
		m_cond.notify_all();

	}
}
void ConnectionPoll::recycleConnection()
{
	//周期性检测
	while (true) {
		//每隔半秒检测一次
		this_thread::sleep_for(chrono::milliseconds(500));
		//涉及到多线程访问 加锁
		lock_guard<mutex>locker(m_mutexQ);
		while (m_connectionQ.size() > m_minSize) {
			Mysqlconn* conn = m_connectionQ.front();
			//如果大于最大空闲 删掉队头
			if (conn->getAliveTime()> m_maxIdleTime) {
				m_connectionQ.pop();
				delete conn;
			}
			else {
				break;
			}
		}
	}
}

void ConnectionPoll::addConnection()
{
	Mysqlconn* conn = new Mysqlconn;
	conn->connect(m_user, m_password, m_dbName, m_ip, m_port);
	//成功连接后 保存到 连接池队列中
	//并保存时间戳
	conn->refreshAliveTime();
	m_connectionQ.push(conn);
}


//连接池的构造函数
ConnectionPoll:: ConnectionPoll() {
	//加载配置文件
	if (!parseJsonFile()) {
		return;
	}
	for (int i = 0; i < m_minSize; i++) {
		//实例化 数据库连接对象
		//先实例化最小值  100个
		addConnection();
	}
	//创建线程进行 生成 或销毁
	thread producer(&ConnectionPoll::produceConnection,this);
	thread recycler(&ConnectionPoll::recycleConnection,this);
	producer.detach();
	recycler.detach();
}