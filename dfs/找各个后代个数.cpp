//找 后代 个数  搜索算法

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
using namespace std;
#include<string>
#include<vector>
int ans[1005]; int n;
vector<int>son[1005];
bool isson[1005];
int u;
int dfs(int u) {
    int cnt = 0;
    for (int i = 0; i < son[u].size(); i++)
    {
        cnt += dfs(son[u][i]);
    }
    ans[u] = cnt;
    return cnt + 1;
}



int main(){
    cin >> n;
    int x, y;
    for (int i = 0; i < n-1; i++) {
        cin >> x>>y;
        son[x].push_back(y);
        isson[y] = true;
    }
    for (int i = 1; i<=n; i++)
    {
        if (!isson[i]) {
            u = i;
            break;
        }
    }
    dfs(u); //找到祖宗 u;
    for (int i = 1; i <= n; i++)
    {
        cout << ans[i] << endl;//输出 有多少直系后代

    }
    cout << ans<< endl;
    return 0;
}