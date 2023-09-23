#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

void get_parent(const vector<vector<int>>& g, int u, int prev, vector<int>& d, vector<int>& parent)
{
    for(int v: g[u])
    {
        if(v == prev)
            continue;
        d[v] = d[u] + 1;
        parent[v] = u;
        get_parent(g, v, u, d, parent);
    }
}

void get_fa(const vector<int>& parent, vector<vector<int>>& fa, const int N, const int M)
{
    // fa[i][j] := 从 i 爬 2 ^ j 步所到点的 id
    // fa[i][0] := 从 i 爬 1 不所到点的 id
    for(int i = 1; i <= N; ++i)
        fa[i][0] = parent[i];
    for(int j = 1; j < M; ++j)
        fa[0][j] = -1;
    for(int j = 1; j < M; ++j)
        for(int i = 1; i <= N; ++i)
        {
            if(fa[i][j - 1] == -1)
                fa[i][j] = -1;
            else
                fa[i][j] = fa[fa[i][j - 1]][j - 1];
        }
}

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

int lca(int x, int y, const vector<int>& d, const vector<vector<int>>& fa)
{
    // d[x] >= d[y]
    if(d[x] < d[y])
        return lca(y, x, d, fa);
    // 将 y 向上调整直到和 x 一个深度
    int delta = d[x] - d[y];
    while(delta > 0)
    {
        x = fa[x][log2(highbit(delta))];
        delta -= highbit(delta);
    }
    if(x == y)
        return x;
    int M = fa[0].size();
    while(true)
    {
        if(fa[x][0] == fa[y][0])
            break;
        int k = 0;
        while(k <= M)
        {
            if(fa[x][k] == -1 || fa[y][k] == -1)
                break;
            if(fa[x][k] == fa[y][k])
                break;
            ++k;
        }
        x = fa[x][k - 1];
        y = fa[y][k - 1];
    }
    return fa[x][0];
}

void dfs(const vector<vector<int>>& g, int u, int prev, const vector<int>& diff, vector<int>& sums)
{
    for(int v: g[u])
    {
        if(v == prev)
            continue;
        dfs(g, v, u, diff, sums);
        sums[u] += sums[v];
    }
    sums[u] += diff[u];
}

int main()
{
    // fstream fin("P3128_1.in");
    int N, K;
    cin >> N >> K;
    // fin >> N >> K;

    vector<vector<int>> g(N + 1);
    for(int i = 0; i < N - 1; ++i)
    {
        int x, y;
        cin >> x >> y;
        // fin >> x >> y;
        g[x].push_back(y);
        g[y].push_back(x);
    }

    vector<int> d(N + 1);
    int M = log2(N) + 1;
    vector<int> parent(N + 1);
    get_parent(g, 1, -1, d, parent); // 视 1 为根

    vector<vector<int>> fa(N + 1, vector<int>(M));
    get_fa(parent, fa, N, M);

    vector<int> diff(N + 1);
    for(int i = 0; i < K; ++i)
    {
        int s, t;
        cin >> s >> t;
        // fin >> s >> t;
        diff[s] += 1;
        diff[t] += 1;
        diff[lca(s, t, d, fa)] -= 1;
        diff[parent[lca(s, t, d, fa)]] -= 1;
    }

    vector<int> sums(N + 1);
    dfs(g, 1, -1, diff, sums);

    int ans = 0;
    for(int s: sums)
    {
        // cout << s << endl;
        ans = max(ans, s);
    }

    cout << ans << endl;
}
