#include <iostream>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

// ------ 数组维护链表的邻接表，无向无权图 ---------

const int V_SIZE = 5e5 + 5; // 总点数
const int E_SIZE = V_SIZE * 2; // 总边数

int head[V_SIZE]; // head[i] 的值是 ver 下标，相当于链表节点指针
int ver[E_SIZE]; // 边的终点，相当于链表节点的 v 字段
int next_[E_SIZE]; // 相当于链表节点的 next_ 字段

// tot 表示 node 数组(这里是 ver 和 next_)已使用的最右位置，而不是链表长度
int tot;

void init_graph()
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

// ---- 离线处理所有查询，vector 数组实现邻接表，维护所有的查询及其 id -----

vector<int> query[V_SIZE], query_id[V_SIZE];

void add_query(int x, int y, int i)
{
    query[x].push_back(y);
    query[y].push_back(x);
    query_id[x].push_back(i);
    query_id[y].push_back(i);
}

// ------------- 数组维护并查集 ----------

int father[V_SIZE];

int find(int x)
{
    if(father[x] == x)
        return x;
    else
        return father[x] = find(father[x]); // 路径压缩
}

void init_unionfindset(int N)
{
    for(int i = 0; i <= N; ++i)
        father[i] = i;
}

// -------------- Tarjan 算法 -----------

int visited[V_SIZE];
int ans[V_SIZE];

void init_tarjan()
{
    memset(visited, 0, sizeof(visited));
    memset(ans, -1, sizeof(ans));
}

void tarjan(int u)
{
    visited[u] = 1;
    for(int i = head[u]; i != -1; i = next_[i])
    {
        int v = ver[i];
        if(visited[v] != 0)
            continue;
        tarjan(v);
        // 在并查集中将 v 与 u 合并
        father[v] = u;
    }
    // u 节点准备回溯，标记 2，此时 u 的父节点 fa 一定是标记 1 的
    // 此时在并查集中将 u 代表的集合与 fa 代表的集合合并
    // 此时扫描所有与 u 有关的查询，若另一个节点 v 的标记为 2，则 lca(x, y) = y 的代表元
    int x = u;
    for(int i = 0; i < query[u].size(); ++i)
    {
        int y = query[x][i];
        int id = query_id[x][i];
        if(x == y)
            ans[id] = x;
        if(visited[y] == 2)
        {
            int lca = find(y);
            ans[id] = lca;
        }
    }
    visited[u] = 2;
}

int main()
{
    int N, M, S;
    cin >> N >> M >> S;

    init_graph();
    init_unionfindset(N);
    init_tarjan();

    for(int i = 0; i < N - 1; ++i)
    {
        int x, y;
        cin >> x >> y;
        add(x, y);
        add(y, x);
    }

    for(int i = 0; i < M; ++i)
    {
        int a, b;
        cin >> a >> b;
        add_query(a, b, i);
    }

    tarjan(S);

    for(int i = 0; i < M; ++i)
    {
        cout << ans[i] << endl;
    }
}
