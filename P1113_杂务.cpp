
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int main()
{
    int n;
    cin >> n;
    vector<int> dp(n + 1, INT_MIN / 2);
    vector<vector<int>> rg(n + 1);
    vector<int> w(n + 1);
    for(int i = 1; i <= n; ++i)
    {
        int v, t;
        cin >> v >> t;
        int u;
        while((cin >> u) && u != 0)
        {
            rg[v].push_back(u);
        }
        if(rg[v].empty())
            dp[v] = 0;
        w[v] = t;
    }

    int ans = 0;
    for(int v = 1; v <= n; ++v)
    {
        for(int u: rg[v])
            dp[v] = max(dp[v], dp[u] + w[u]);
        ans = max(ans, dp[v] + w[v]);
    }
    cout << ans << endl;
}
