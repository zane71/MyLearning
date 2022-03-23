//这道题是解求最小面积的题 给出体积 n和 蛋糕层数  求其最小表面积
//递归 搜索  加剪枝
// 范围  r0=sqrt(n)  n=r*r*h;    m-u<r<r0-1  r0为前一层！

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string>
#include<iostream>
using namespace std;
#include<string>
#include<vector>
int n, m; //体积为 n  m层；
int va[20];
int ans;
const int INF = 10000000;

void dfs(int u,int v,int s,int r0,int h0) { //第几层 ，体积 ，面积 ，半径 高度
    if (u == m && v == n) {
        ans = min(ans, s);
        return;
    }
    if (va[m - u] + v > n) return;
    if (2.0 * (n - v) / r0+s > ans)return;
    for (int r = r0; r >=m-u; r--)
    {
        for (int h = h0; h>=m-u; h--)
        {
            int tv = v + h * r * r;
            if (tv > n)continue;
            int ts = s + h * 2 * r;
            if (u == 0)ts += r * r;
            dfs(u + 1, tv, ts, r - 1, h - 1);
        }
    }

}
int main() {
    int count = 0;
    cin >> n >> m;

    for (int i = 1; i <= m; i++) {
        va[i] = va[i - 1] + i * i * i;  //剩下每层的每层最小体积   剪枝用
    }
    int r0 = sqrt(n) + 0.5;
    ans = INF;
    dfs(0, 0, 0, r0, n);

    if (ans == INF)return 0;
   
    cout << ans<< endl;
    return 0;
}