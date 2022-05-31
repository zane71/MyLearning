#pragma once
using namespace std;
#include<iostream>
#include<mysql.h>
#include<chrono>
using namespace chrono;
class Mysqlconn
{
public:
	//初始化数据库连接
	Mysqlconn();
	//释放数据库连接
	~Mysqlconn();
	//连接数据库
	bool connect(string user, string passwd, string dbName, string ip, unsigned short port = 3306);
	//更新数据库 insert, update,dalete
	bool update(string sql);
	//查询数据库
	bool query(string sql);

	//遍历查询得到的结果集
	bool next();//取
	//得到结果集中的字段
	string value(int index);
	//事务操作  默认提交
	bool transaction();
	//提交事务
	bool commit();
	//事务回滚
	bool rollback();
	//刷新起始的空闲时间
	void refreshAliveTime();
	//计算连接存活的总时长
	long long getAliveTime();

private:
	void freeResult();
	MYSQL* m_conn = nullptr;
	MYSQL_RES* m_result = nullptr;
	MYSQL_ROW m_row = nullptr;
	//时钟变量 绝对时间
	steady_clock::time_point m_alivetime;
};

