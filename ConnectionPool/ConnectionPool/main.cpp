#define _CRT_SECURE_NO_WARNINGS
#include"Mysqlconn.h"
#include <iostream>
#include<iostream>
#include<memory>
#include"ConnectionPool.h"
using namespace std;
// 1.单线程 使用/不使用连接池
// 2.多线程：使用/不使用连接池

//  不使用连接池
void op1(int begin, int end) {
    for (int i = begin; i < end; i++)
    {
        Mysqlconn conn;
        conn.connect("root", "671520", "demo", "127.0.0.1");
        char sql[1024] = { 0 };
        sprintf(sql, "insert into student values(%d, 11230105, '琪琪', '女', '2022-5-31 21:07:06')", i);
        conn.update(sql);
    }
}
//  使用连接池
void op2(ConnectionPoll* pool,int begin, int end) {
    for (int i = begin; i < end; i++)
    {
        //取可用连接
        shared_ptr<Mysqlconn>conn = pool->getConnection();
        
        char sql[1024] = { 0 };
        sprintf(sql, "insert into student values(%d, 11230105, '琪琪', '女', '2022-5-31 21:07:06')", i);
        conn->update(sql);
    }
}
//测试函数
//单线程
void test1() {
#if 0

    //非连接池，单线程，用时：64336200300纳秒，64336毫秒
    steady_clock::time_point begin = steady_clock::now();
    op1(0, 5000);
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    cout << "非连接池，单线程，用时：" << length.count() << "纳秒，"
        << length.count() / 1000000 << "毫秒" << endl;
#else
    //用连接池，单线程，用时：3312735800纳秒，3312毫秒
    ConnectionPoll* pool = ConnectionPoll::getConnectionPoll();
    steady_clock::time_point begin = steady_clock::now();
    op2(pool,0, 5000);
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    std::cout << "用连接池，单线程，用时：" << length.count() << "纳秒，"
        << length.count() / 1000000 << "毫秒" << endl;
#endif
}

//多线程
void test2() {
#if 0
    //非连接池，多线程，用时：12645346200纳秒，12645毫秒
    //通过额外的连接   解决由于相同的用户名和密码会出现问题  避免  
    // 0x00007FFB4D63355C (libmysql.dll)处
    // (位于 ConnectionPool.exe 中)引发的异常: 0xC0000005: 
    //读取位置 0x00000000000003D8 时发生访问冲突
    Mysqlconn conn;
    conn.connect("root", "671520", "demo", "127.0.0.1");
    steady_clock::time_point begin = steady_clock::now();
    thread t1(op1, 0, 1000);
    thread t2(op1, 1000, 2000);
    thread t3(op1, 2000, 3000);
    thread t4(op1, 3000, 4000);
    thread t5(op1, 4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    std::cout << "非连接池，多线程，用时：" << length.count() << "纳秒，"
        << length.count() / 1000000 << "毫秒" << endl;
#else
    //用连接池，多线程，用时：959476200纳秒，959毫秒
    ConnectionPoll* pool = ConnectionPoll::getConnectionPoll();
    steady_clock::time_point begin = steady_clock::now();
    thread t1(op2, pool,0, 1000);
    thread t2(op2, pool,1000, 2000);
    thread t3(op2, pool,2000, 3000);
    thread t4(op2, pool,3000, 4000);
    thread t5(op2, pool,4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    std::cout << "用连接池，多线程，用时：" << length.count() << "纳秒，"
        << length.count() / 1000000 << "毫秒" << endl;
#endif
}
int query() {
    Mysqlconn conn;
    conn.connect("root", "671520", "demo", "127.0.0.1");

    string sql="insert into student values(6,11230105,'琪琪','女','2022-5-31 21:07:06')";
    bool flag=conn.update(sql);
    std::cout << "flag value:" << flag << endl;
    sql = "select * from student";
    conn.query(sql);//得出结果集
    while (conn.next()) {
        //取字段
        std::cout << conn.value(0) << ","
            << conn.value(1) << ","
            << conn.value(2) << ","
            << conn.value(3) << ","
            << conn.value(4) << endl;
    }
    return 0;
}
int main()
{
    //query();
    //std::cout << "Hello World!\n";
    test2();
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
