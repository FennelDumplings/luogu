
/*
 * https://www.luogu.com.cn/problem/P5180
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class UnionFindSet
{
public:
    UnionFindSet(int N)
    {
        _father = vector<int>(N + 1);
        weight = vector<int>(N + 1);
        for(int i = 1; i <= N; ++i)
        {
            _father[i] = i;
            weight[i] = i;
        }
    }

    int query(int u, const vector<int>& sdom)
    {
        // 返回 u -> 根的路径上的节点 v 中 sdom 最小值对应的节点
        _find(u, sdom);
        return weight[u];
    }

    void add(int u, int v)
    {
        // 加边 u -> v(fa[u])
        _father[u] = v;
    }

private:
    vector<int> _father, weight;

    int _find(int u, const vector<int>& sdom)
    {
        // 返回根，用于路径压缩
        if(_father[u] == u)
            return u;
        int root = _find(_father[u], sdom);
        // 路径压缩 _father[u] = root 之前做以下比较和更新
        if(sdom[weight[_father[u]]] < sdom[weight[u]])
            weight[u] = weight[_father[u]];
        return _father[u] = root;
    }
};


void dfs(const vector<vector<int>>& D, int u, int& dfnid, vector<int>& id_dfn, vector<int>& dfn_id, vector<int>& fa, vector<int>& sdom)
{
    id_dfn[u] = ++dfnid;
    dfn_id[dfnid] = u;
    sdom[u] = id_dfn[u];
    for(int v: D[u])
        if(!id_dfn[v]) // v 已经访问过
        {
            dfs(D, v, dfnid, id_dfn, dfn_id, fa, sdom);
            fa[v] = u;
        }
}

int dfs2(const vector<vector<int>>& DT, int u, vector<int>& cnts)
{
    int ans = 1;
    for(int v: DT[u])
    {
        if(u != v)
            ans += dfs2(DT, v, cnts);
    }
    return cnts[u] = ans;
}

vector<vector<int>> dominated_tree(const vector<vector<int>>& D, const vector<vector<int>>& RD, const int S, const int N)
{

    int dfnid = 0;
    // dfn_id[dfnid] := dfn 序为 dfnid 的节点
    // id_dfn[u] = 节点 u 的 dfn 序;
    // fa[u] := dfs 树中 u 的父节点
    vector<int> id_dfn(N + 1), dfn_id(N + 1), fa(N + 1);
    // sdom[u] := u 的 sdom 的 dfn 序
    // idom[u] := u 的 idom 的节点编号
    vector<int> idom(N + 1), sdom(N + 1);

    // 预处理 dfn_id, id_dfn, fa, sdom
    dfs(D, S, dfnid, id_dfn, dfn_id, fa, sdom);

    UnionFindSet forest(N);
    vector<vector<int>> sdom_set(N + 1);
    // sdom_set[u] := sdom 为 id_dfn[u] 的点集
    // 求 sdom[u], 顺便求 sdom_set[fa[u]] 中的 v 的 idom[v]
    for(int dfn = dfnid; dfn >= 2; --dfn)
    {
        int u = dfn_id[dfn];
        // 求 u 的 sdom 只需要对它的所有直接前驱 query 一次
        // 求得前驱中的 sdom 最小值即可
        for(int v : RD[u])
        {
            // id_dfn[u] > id_dfn[v] 时
            //   v 尚未访问到，所以它自己就是根, query 的结果就是它自己
            // id_dfn[u] < id_dfn[v] 时
            //   sdom[u] = min(sdom[k]) 其中 dfn[k] > dfn[u] 且 k 能到 u
            //     即 u -> v -> ... 的祖先链中 sdom 的最小值
            //   query(v) 求的正是 u -> v -> ... dfs 树上已经连边的 v 的根节点
            //     这条链上的 sdom 最小值对应的节点
            if(id_dfn[v])
            {
                sdom[u] = min(sdom[u], sdom[forest.query(v, sdom)]);
            }
        }
        sdom_set[dfn_id[sdom[u]]].push_back(u);
        int fp = fa[u];
        // 给森林加一条 dfs 树上的边
        forest.add(u, fa[u]);
        // 现在 fa[u] -> u 的这棵子树中已经处理完了
        // sdom 为 id_dfn[fa[u]] 的节点 v 可以处理了
        for(int v : sdom_set[fp])
        {
            int k = forest.query(v, sdom);
            // 此时 dfs 树中 fa[u] -> k -> v
            if(sdom[k] == sdom[v])
            {
                // sdom[v] 也就是 fa[u] 的 dfn 序
                idom[v] = fp;
            }
            else
            {
                // 按公式是 idom[v] = idom[k]
                // 先记下 idom[v] = k，之后集中处理
                idom[v] = k;
            }
        }
        sdom_set[fp].clear();
    }
    // 之后集中处理 idom[v] = idom[k] 的这些点
    for(int dfn = 2; dfn <= dfnid; ++dfn)
    {
        int u = dfn_id[dfn];
        if(idom[u] == dfn_id[sdom[u]])
            idom[u] = idom[u];
        else
            idom[u] = idom[idom[u]];
    }
    // 由 idom[u] 建 DT
    vector<vector<int>> DT(N + 1);
    for(int u = 1; u <= N; ++u)
        DT[idom[u]].push_back(u);
    return DT;
}

int main()
{
    int n, m;
    cin >> n >> m;
    // 建图
    vector<vector<int>> D(n + 1), RD(n + 1);
    for(int i = 1; i <= m; ++i)
    {
        int u, v;
        cin >> u >> v;
        D[u].push_back(v);
        RD[v].push_back(u);
    }
    // 源点，这里给了源点，不用设超级源
    int S = 1;

    // 给定图 D，反图 RD，源点 S，求支配树
    vector<vector<int>> DT = dominated_tree(D, RD, S, n);

    vector<int> cnts(n + 1);
    dfs2(DT, S, cnts);

    for(int u = 1; u <= n; ++u)
        cout << cnts[u] << " ";
    cout << endl;
}
