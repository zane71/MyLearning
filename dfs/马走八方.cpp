//马走八方
//#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
using namespace std;
int dir[8][2] = { {2,1},{2,-1},{1,2},{1,-2},{-2,1},{-2,-1},{-1,2} ,{-1,-2} };
char map[1005][1005];
int m, n;

void dfs(int x,int y,int step) {
    
    if (step > 3 || x < 0||x>=m||y<0||y>=n) {
       return;
    }
    
    map[x][y] = '#';
  
    for (int i = 0; i < 8; i++) {
        dfs(x + dir[i][0],y+dir[i][1],step+1);

    }
}

int main(){
    int x = 0;
    int y = 0;
    cin >> m >> n;
    cin >> x>>y;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
        {
            map[i][j] = '.';
        }
    }
 
    dfs(x - 1, y-1, 0);
    for (int i = 1; i<=m; i++)
    {
        cout << map[i] << endl;
    }
    return 0;
}