
/*
 * https://www.luogu.com.cn/problem/P2597
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

int lowbit(int n)
{
    return n & (-n);
}

int highbit(int n)
{
    int p = lowbit(n);
    while(p != n)
    {
        n -= p;
        p = lowbit(n);
    }
    return p;
}

int lca(int x, int y, const vector<int>& d, const vector<vector<int>>& f)
{
    // d[x] >= d[y]
    if(d[x] < d[y])
        return lca(y, x, d, f);
    // 将 y 向上调整直到和 x 一个深度
    int delta = d[x] - d[y];
    while(delta > 0)
    {
        x = f[x][log2(highbit(delta))];
        delta -= highbit(delta);
    }
    if(x == y)
        return x;
    int n = d.size();
    int m = log2(n);
    while(true)
    {
        if(f[x][0] == f[y][0])
            break;
        int k = 1;
        while(k <= m)
        {
            if(f[x][k] == -1 || f[y][k] == -1)
                break;
            if(f[x][k] == f[y][k])
                break;
            ++k;
        }
        x = f[x][k - 1];
        y = f[y][k - 1];
    }
    return f[x][0];
}

int dfs(const vector<vector<int>>& DT, int u, vector<int>& result)
{
    int size = 1;
    for(int v: DT[u])
        size += dfs(DT, v, result);
    result[u] = size - 1;
    return size;
}

int main()
{
    int n;
    cin >> n;
    vector<vector<int>> D(n + 1);
    vector<vector<int>> RD(n + 1);
    // 建图
    for(int i = 1; i <= n; ++i)
    {
        int v;
        while(cin >> v && v > 0)
        {
            D[v].push_back(i);
            RD[i].push_back(v);
        }
    }
    // 超级源 : 0
    vector<int> indegree(n + 1);
    for(int i = 1; i <= n; ++i)
    {
        if(RD[i].empty())
        {
            RD[i].push_back(0);
            D[0].push_back(i);
        }
        indegree[i] = RD[i].size();
    }
    // 拓扑排序
    vector<int> topo_order;
    queue<int> q;
    q.push(0); // 超级源
    while(!q.empty())
    {
        int u = q.front();
        q.pop();
        topo_order.push_back(u);
        for(int v: D[u])
        {
            --indegree[v];
            if(indegree[v] == 0)
                q.push(v);
        }
    }
    vector<vector<int>> DT(n + 1);
    // 2 ^ m <= n
    // log2(2^m) <= log2(n)
    // m <= log2(n)
    int m = log2(n);
    vector<vector<int>> f(n + 1, vector<int>(m + 1, -1));
    // f[u][k] := u 的第 2^k 个祖先
    vector<int> d(n + 1, -1);
    // d[u] := u 在 DT 中的深度，超级源为 0
    d[0] = 0;
    for(int i = 0; i <= n; ++i)
    {
        int u = topo_order[i];
        if(RD[u].empty()) // DT 的树根是超级源
            continue;
        int k = RD[u].size();
        int idom = RD[u][0];
        for(int j = 1; j < k; ++j)
        {
            int fa = RD[u][j];
            idom = lca(idom, fa, d, f);
        }
        DT[idom].push_back(u);
        // DT 中连接一条边 idom -> u
        // u 的祖先链在 f 中要更新
        f[u][0] = idom;
        d[u] = d[idom] + 1;
        int p = 1; // step = 2 ^ p
        while(p <= m && f[f[u][p - 1]][p - 1] != -1)
        {
            f[u][p] = f[f[u][p - 1]][p - 1];
            ++p;
        }
    }
    vector<int> result(n + 1);
    // result[i] := DT 中 i 的 size - 1
    dfs(DT, 0, result);
    for(int u = 1; u <= n; ++u)
        cout << result[u] << endl;
}
