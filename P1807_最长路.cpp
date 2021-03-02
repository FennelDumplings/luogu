#include <vector>
#include <iostream>
#include <climits>

using namespace std;

// DFS 拓扑序 DP
struct EdgeTo
{
    int v;
    int w;
    EdgeTo(int v, int w):v(v),w(w){}
};

void dfs(const vector<vector<EdgeTo>>& g, int u, vector<int>& dp)
{
    if(dp[u] != INT_MIN / 2)
        return;
    for(const EdgeTo &son: g[u])
    {
        int v = son.v;
        int w = son.w;
        dfs(g, v, dp);
        if(dp[v] != INT_MIN / 2 - 1 && dp[v] + w > dp[u])
            dp[u] = dp[v] + w;
    }
    if(dp[u] == INT_MIN / 2)
        dp[u] = INT_MIN / 2 - 1;
}

int main()
{
    int n, m;
    cin >> n >> m;
    vector<vector<EdgeTo>> g(n + 1);
    for(int i = 0; i < m; ++i)
    {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].emplace_back(v, w);
    }
    int s = 1, t = n;
    vector<int> dp(n + 1, INT_MIN / 2);
    dp[t] = 0;
    dfs(g, s, dp);
    if(dp[s] == INT_MIN / 2 - 1)
        cout << -1 << endl;
    else
        cout << dp[s] << endl;
}
