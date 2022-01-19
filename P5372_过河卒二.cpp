#include <vector>
#include <iostream>

using namespace std;

using ll = long long;
const int MOD = 59393;

int main()
{
    int n, m, k;
    cin >> n >> m >> k;
    vector<vector<int>>dp(n + 2, vector<int>(m + 2, -1));

    for(int i = 0; i < k; ++i)
    {
        int x, y;
        cin >> x >> y;
        dp[x][y] = 0;
    }
    dp[1][1] = 1;

    for(int i = 1; i <= n + 1; ++i)
        for(int j = 1; j <= m + 1; ++j)
        {
            if(dp[i][j] != -1)
                continue;
            dp[i][j] = 0;
            if(i - 1 >= 1)
                dp[i][j] = ((ll)dp[i][j] + dp[i - 1][j]) % MOD;
            if(j - 1 >= 1)
                dp[i][j] = ((ll)dp[i][j] + dp[i][j - 1]) % MOD;
            if(i - 1 >= 1 and j - 1 >= 1)
                dp[i][j] = ((ll)dp[i][j] + dp[i - 1][j - 1]) % MOD;
        }
    cout << dp[n + 1][m + 1] << endl;
}

