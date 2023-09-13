#include <iostream>
#include <cstring>
#include <algorithm>
#include <cmath>

using namespace std;

// ------ 数组维护链表的邻接表，无向无权图 ---------

const int V_SIZE = 5e5 + 1; // 总点数
const int E_SIZE = V_SIZE * 2; // 总边数

int head[V_SIZE]; // head[i] 的值是 ver 下标，相当于链表节点指针
int ver[E_SIZE]; // 边的终点，相当于链表节点的 v 字段
int next_[E_SIZE]; // 相当于链表节点的 next_ 字段

// tot 表示 node 数组(这里是 ver 和 next_)已使用的最右位置，而不是链表长度
int tot;

void init()
{
    tot = 0;
    memset(head, -1, sizeof(head));
    memset(next_, -1, sizeof(head));
}

void add(int x, int y)
{
    ver[++tot] = y;
    next_[tot] = head[x];
    head[x] = tot; // 在表头 x 处插入
}

// --------------------------------------------------

int d[500001], fa[500001][22], parent[500001];

void dfs(int u, int prev)
{
    // 访问从 u 出发的所有边
    for(int i = head[u]; i; i = next_[i])
    {
        int v = ver[i];
        if(v == prev)
            continue;
        d[v] = d[u] + 1;
        parent[v] = u;
        dfs(v, u);
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

void get_fa(int N, int K)
{
    // fa[i][j] := 从 i 爬 2 ^ j 步所到点的 id
    // fa[i][0] := 从 i 爬 1 不所到点的 id
    for(int i = 1; i <= N; ++i)
        fa[i][0] = parent[i];
    for(int j = 1; j < K; ++j)
        fa[0][j] = -1;
    for(int j = 1; j < K; ++j)
        for(int i = 1; i <= N; ++i)
        {
            if(fa[i][j - 1] == -1)
                fa[i][j] = -1;
            else
                fa[i][j] = fa[fa[i][j - 1]][j - 1];
        }
}

int lca(int x, int y)
{
    if(d[x] < d[y])
        swap(x, y);
    // d[x] >= d[y]
    // 将 x 向上调整直到和 y 一个深度
    int delta = d[x] - d[y];
    while(delta > 0)
    {
        x = fa[x][(int)log2(highbit(delta))];
        delta -= highbit(delta);
    }
    if(x == y)
        return x;
    // int n = d.size();
    int m = log2(highbit(d[x]));
    while(true)
    {
        if(fa[x][0] == fa[y][0])
            break;
        int k = 1;
        while(k <= m)
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

int main()
{
    init();

    int N, M, S;
    cin >> N >> M >> S;

    for(int i = 0; i < N - 1; ++i)
    {
        int x, y;
        cin >> x >> y;
        add(x, y);
        add(y, x);
    }

    dfs(S, 0);

    int K = log2(N) + 1;
    get_fa(N, K);

    for(int i = 0; i < M; ++i)
    {
        int a, b;
        cin >> a >> b;
        cout << lca(a, b) << endl;
    }
    return 0;
}
