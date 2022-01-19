#include <iostream>
#include <vector>

using namespace std;

using ll = long long;

void init(vector<vector<ll>>& dp, int x0, int y0, int n, int m)
{
    dp[n][m] = 1;
    dp[x0][y0] = 0;
    int dx[8] = {1, 1, 2, 2, -1, -1, -2, -2};
    int dy[8] = {2, -2, 1, -1, 2, -2, 1, -1};
    for(int d = 0; d < 8; ++d)
    {
        int x = x0 + dx[d];
        int y = y0 + dy[d];
        if(x >= 0 && y >= 0 && x <= n && y <= m)
            dp[x][y] = 0;
    }
}

ll solve(int i, int j, const int n, const int m, vector<vector<ll>>& dp)
{
    if(dp[i][j] != -1)
        return dp[i][j];
    ll ans = 0;
    if(i + 1 <= n)
        ans += solve(i + 1, j, n, m, dp);
    if(j + 1 <= m)
        ans += solve(i, j + 1, n, m, dp);
    return dp[i][j] = ans;
}

int main()
{
    int n, m, x0, y0;
    cin >> n >> m >> x0 >> y0;
    vector<vector<ll>> dp(n + 1, vector<ll>(m + 1, -1));
    init(dp, x0, y0, n, m);
    ll ans =  solve(0, 0, n, m, dp);
    cout << ans << endl;
}
