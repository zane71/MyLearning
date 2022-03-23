//dijkstra最短路径
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
const int M = 1001;
using namespace std;
#include <cstring>
struct edge {
	int v, w, next;  //v：顶点编号 w：边权重  next:下一个链表节点
	edge() {}
	edge(int _v, int _w, int _next) {
		v = _v;
		w = _w;
		next = _next;
	}
}e[M*2]; //用链表存放边 权重 连接
int head[1001];//连接的边  用来存放 边号
int sizes;
void init() {
	memset(head, -1, sizeof(head));
	sizes = 0;  //边数初始化为0；
}
//插入边
void insert(int u,int v,int w) {
	e[sizes] = edge(v, w, head[u]);  //边  e[0]->next =head[u]=0; 
	head[u] = sizes++;  //边号
}
void insert2(int u, int v, int w) {
	insert(u, v, w); //无向图
	insert(v, u, w);//
}

int n, m;
int dis[1001];//从源点到 每个点 他的最小距离
bool vis[1001];//每个点有没有被标记
void dijkstar(int u) {
	memset(vis, false, sizeof(vis));//初始化
	memset(dis, 0x3f, sizeof(dis));//字节填充
	dis[u] = 0;
	for (int i = 0; i < n; i++)
	{
		int mind = 1000000000;int minj = -1;
		for (int j = 1; j <=n; j++)
		{
			if (!vis[j] && dis[j] < mind) {
				minj = j;  //边
				mind = dis[j];  //距离  j=1
			}
		}
		if (minj == -1) {
			return;
		}
		vis[minj] = true;
		for (int j = head[minj]; j!=-1; j=e[j].next)
		{
			int v = e[j].v;
			//cout << "v:" << v<<endl;
			int w = e[j].w;
			//cout << "w:" << w << endl;
			if (!vis[v] && dis[v] > dis[minj] + w) {
				dis[v] = dis[minj] + w;
				//cout << "测试：" << dis[v] << endl;
			}
		}
	}
}
int main()
{
	init();
	int u, v, w;
	cin >>n>>m;
	while (m--) {
		cin >> u >> v >> w;
		insert2(u, v, w);
	}
	dijkstar(1);
	cout << dis[3] << endl;
	return 0;
}