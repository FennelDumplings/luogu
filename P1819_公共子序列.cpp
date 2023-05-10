#include <iostream>
#include <string>
#include <vector>

using namespace std;

using ll = long long;
const int MOD = 1e8;

class SeqAM
{
public:
    SeqAM(){}
    SeqAM(const string& t)
    {
        build(t);
    }

    void build(const string& t)
    {
        // 由字符串 t 建立序列自动机
        int n = t.size();
        int m = 26; // 字母表中的字符数
        nxt = vector<vector<int> >(n + 1, vector<int>(m, -1));

        for(int i = t.size() - 1; i >= 0; --i)
        {
            for(int j = 0; j < 26; ++j)
                nxt[i][j] = nxt[i + 1][j];
            nxt[i][t[i] - 'a'] = i; // 在 nxt[i] 这一行只更新了第 t[i] - 'a' 这一列。
        }
    }

    bool find(const string& s)
    {
        int l = s.size();
        int p = -1;
        for(int i = 0; i < l; ++i)
        {
            p = nxt[p + 1][s[i] - 'a'];
            if(p == -1)
                return false;
        }
        return true;
    }

    int check_nxt(int i, char ch)
    {
        return nxt[i][ch - 'a'];
    }

private:
    vector<vector<int> > nxt;
};

vector<vector<vector<int> > > dp;
SeqAM seq_am1;
SeqAM seq_am2;
SeqAM seq_am3;
int n;

int dfs(int x, int y, int z)
{
    // 动态规划
    if(dp[x][y][z] != -1)
        return dp[x][y][z];

    dp[x][y][z] = 0;
    for(int i = 0; i < 26; ++i)
    {
        int nxt_x = seq_am1.check_nxt(x, 'a' + i);
        int nxt_y = seq_am2.check_nxt(y, 'a' + i);
        int nxt_z = seq_am3.check_nxt(z, 'a' + i);
        if(nxt_x != -1 && nxt_y != -1 && nxt_z != -1)
        {
            // i 字符同时在三个后缀串中
            dp[x][y][z] = ((ll)dp[x][y][z] + 1) % MOD;
            if(nxt_x + 1 < n && nxt_y + 1 < n && nxt_z + 1 < n)
                dp[x][y][z] = ((ll)dp[x][y][z] + dfs(nxt_x + 1, nxt_y + 1, nxt_z + 1)) % MOD;
        }
    }
    return dp[x][y][z];
}

int main()
{
    cin >> n;
    string s1, s2, s3;
    cin >> s1 >> s2 >> s3;
    seq_am1.build(s1);
    seq_am2.build(s2);
    seq_am3.build(s3);
    dp.assign(n, vector<vector<int> >(n, vector<int>(n, -1)));
    cout << dfs(0, 0, 0) << endl;
}
