#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

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

void dfs2(vector<vector<int>>& rg, int u, const int scc_id, vector<vector<int>>& scc_list, vector<int>& scc_id_mapping)
{
    for(int v: rg[u])
    {
        if(scc_id_mapping[v] != -1)
            continue;
        scc_id_mapping[v] = scc_id;
        scc_list[scc_id].push_back(v);
        dfs2(rg, v, scc_id, scc_list, scc_id_mapping);
    }
}

vector<vector<int>> get_scc(int n, vector<vector<int>>& g)
{
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
    vector<int> scc_id_mapping(n + 1, -1);
    int scc_id = 0;

    for(int i = n - 1; i >= 0; --i)
    {
        int s = finish_list[i];
        if(scc_id_mapping[s] != -1)
            continue;
        scc_id_mapping[s] = scc_id;
        scc_list.push_back({s});
        dfs2(rg, s, scc_id, scc_list, scc_id_mapping);
        scc_id++;
    }

    return scc_list;
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

    vector<vector<int>> scc_list = get_scc(n, g);

    cout << scc_list.size() << endl;
    vector<int> scc_id_mapping(n + 1, -1);
    for(int i = 0; i < scc_list.size(); ++i)
        for(int u: scc_list[i])
            scc_id_mapping[u] = i;
    vector<int> visited(n + 1, 0);
    for(int s = 1; s <= n; ++s)
    {
        if(visited[s] == 1)
            continue;
        int scc_id = scc_id_mapping[s];
        sort(scc_list[scc_id].begin(), scc_list[scc_id].end());
        for(int u: scc_list[scc_id])
        {
            visited[u] = 1;
            cout << u << " ";
        }
        cout << endl;
    }
}
