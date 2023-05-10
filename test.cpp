#include <iostream>
#include <cstdio>
#include <string>
#include <memory.h>
#include <vector>

using namespace std;

const int MOD = 1e8;

const int N = 160, NPOS = -1;  // NPOS为数组默认值，查找子串时有用

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
        nxt.assign(n + 1, vector<int>(26, -1));
        cout << "build" << endl;
        // for (int i = 0; i < 26; i++) this -> nxt[s.size()][i] = NPOS;  // 如果要查找子串，需要进行初始化
        for (int i = n - 1; i >= 0; i--)
        {
            for (int j = 0; j < 26; j++)
                nxt[i][j] = nxt[i + 1][j];
            nxt[i][t[i] - 'a'] = i + 1;
        }
    }

    bool find(string t) {  // 查找函数，此程序中不会使用
        int p = -1, length = t.size();  // p 指针
        for (int i = 0; i < length; i++) {  // 循环每个字符
            p = this -> nxt[p + 1][t[i] - 'a'];  // 指针跳转
            if (p == NPOS) return 0;  // 如果当前值为空（或不存在），返回未找到
        }
        return 1;  // 子串t存在
    }

    void show()
    {
        for(int i = 0; i < nxt.size(); ++i)
        {
            for(int j = 0; j < 26; ++j)
                cout << nxt[i][j] << " ";
            cout << endl;
        }
    }

    int check_nxt(int i, char ch)
    {
        return nxt[i][ch - 'a'];
    }

    // int nxt[160][30];  // 核心数组
    vector<vector<int> > nxt;
};

SeqAM seq_am1, seq_am2, seq_am3;  // 三个串
long long dp[N][N][N];  // 记忆化数组

long long dfs(int x, int y, int z)
{
    cout << x << " " << y << " " << z << endl;
    if (dp[x][y][z])
        return dp[x][y][z];  // 记忆化
    for (int i = 0; i < 26; i++) {  // 枚举每个字符
        int nxt_x = seq_am1.nxt[x][i];
        int nxt_y = seq_am2.nxt[y][i];
        int nxt_z = seq_am3.nxt[z][i];
        if (nxt_x != -1 && nxt_y != -1 && nxt_z != -1)
        {
            dp[x][y][z] = (dp[x][y][z] + dfs(nxt_x, nxt_y, nxt_z)) % MOD;  // 继续dfs，并加上它的返回值，记得取余
        }
    }
    if (x || y || z) dp[x][y][z]++;  // 不都为0则加一
    return dp[x][y][z] % MOD;
}

int main()
{
    int n;
    cin >> n;
    string s1, s2, s3;
    cin >> s1 >> s2 >> s3;
    seq_am1.build(s1);
    seq_am2.build(s2);
    seq_am3.build(s3);
    cout << dfs(0, 0, 0) << endl;
    return 0;
}
