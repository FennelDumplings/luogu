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
        nxt.assign(n + 1, vector<int>(m, -1)); // 状态节点 0 ~ n，字母表 0 ~ m-1

        for(int i = n - 1; i >= 0; --i)
        {
            for(int ch = 0; ch < 26; ++ch)
                nxt[i][ch] = nxt[i + 1][ch];
            nxt[i][t[i] - 'a'] = i + 1;
        }
    }

    bool find(const string& s)
    {
        int l = s.size();
        int p = 0; // 初始状态
        for(int i = 0; i < l; ++i)
        {
            p = nxt[p][s[i] - 'a'];
            if(p == -1)
                return false;
        }
        return true;
    }

    int check_nxt(int i, int ch)
    {
        return nxt[i][ch];
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
        int nxt_x = seq_am1.check_nxt(x, i);
        int nxt_y = seq_am2.check_nxt(y, i);
        int nxt_z = seq_am3.check_nxt(z, i);
        if(nxt_x != -1 && nxt_y != -1 && nxt_z != -1)
        {
            // i 字符同时在三个后缀串中
            dp[x][y][z] = ((ll)dp[x][y][z] + 1 + dfs(nxt_x, nxt_y, nxt_z)) % MOD;
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
    dp.assign(n + 1, vector<vector<int> >(n + 1, vector<int>(n + 1, -1)));
    cout << dfs(0, 0, 0) << endl;
}
