#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

void dfs1(vector<vector<int>>& g, int u, vector<int>& finish_list, vector<int>& visited)
{
    if(visited[u] == 1)
        return;
    visited[u] = 1;
    for(int v: g[u])
    {
        dfs1(g, v, finish_list, visited);
    }
    finish_list.push_back(u);
}

void dfs2(vector<vector<int>>& rg, int u, const int scc_id, vector<vector<int>>& scc_list, vector<int>& visited)
{
    scc_list[scc_id].push_back(u);
    for(int v: rg[u])
    {
        if(visited[v] == 1)
            continue;
        visited[v] = 1;
        dfs2(rg, v, scc_id, scc_list, visited);
    }
}

int main()
{
    int n, m;
    cin >> n >> m;
    vector<vector<int>> g(n + 1);
    unordered_set<int> setting;
    for(int i = 0; i < m; ++i)
    {
        int u, v;
        cin >> u >> v;
        if(u == v)
            continue;
        int id = (u - 1) * 1e4 + (v - 1);
        if(setting.count(id) > 0)
            continue;
        setting.insert(id);
        g[u].push_back(v);
    }

    vector<int> finish_list;
    vector<int> visited(n + 1);
    for(int s = 1; s <= n; ++s)
    {
        dfs1(g, s, finish_list, visited);
    }

    vector<vector<int>> rg(n + 1);
    for(int u = 1; u <= n; ++u)
    {
        for(int v: g[u])
            rg[v].push_back(u);
    }

    vector<vector<int>> scc_list;
    visited.assign(n + 1, 0);
    int scc_id = 0;

    for(int i = n - 1; i >= 0; --i)
    {
        int s = finish_list[i];
        if(visited[s] == 1)
            continue;
        visited[s] = 1;
        scc_list.push_back({});
        dfs2(rg, s, scc_id, scc_list, visited);
        scc_id++;
    }

    cout << scc_list.size() << endl;
    for(int i = 0; i < scc_list.size(); ++i)
    {
        for(int u: scc_list[i])
            cout << u << " ";
        cout << endl;
    }
}
