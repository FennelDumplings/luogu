#include <vector>
#include <climits>
#include <iostream>

using namespace std;
using ll = long long;

int main()
{
    int N, M;
    cin >> N >> M;
    vector<vector<int> > adj(N + 1, vector<int>(N + 1, INT_MAX / 2));
    for(int i = 1; i <= M; ++i)
    {
        int u, v, w;
        cin >> u >> v >> w;
        if(u < 1 || u > N || v < 1 || v > N) continue;
        adj[u][v] = min(adj[u][v], w);
        adj[v][u] = min(adj[v][u], w);
    }
    for(int i = 1; i <= N; ++i)
        adj[i][i] = 0;
    vector<vector<int> > d = adj;
    int ans = INT_MAX / 2;
    for(int k = 1; k <= N; ++k)
    {
        for(int i = 1; i < k; ++i)
            for(int j = i + 1; j < k; ++j)
                ans = min((ll)ans, (ll)d[i][j] + adj[j][k] + adj[k][i]);
        for(int i = 1; i <= N; ++i)
            for(int j = 1; j <= N; ++j)
                d[i][j] = min((ll)d[i][j], (ll)d[i][k] + d[k][j]);
    }
    if(ans == INT_MAX / 2)
        cout << "No solution." << endl;
    else
        cout << ans << endl;
}
