#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

using ll = long long;
const int MOD = 59393;

struct Point
{
    int x, y;
    Point(){}
    Point(int x, int y):x(x),y(y){}
    bool operator<(const Point& p) const
    {
        if(x == p.x)
            return y < p.y;
        return x < p.x;
    }
};

// (4) 卢卡斯定理 + 预处理
vector<int> fac, invfac;
vector<int> inv;

void preprocess(ll n, ll p)
{
    fac.assign(n, 1);
    invfac.assign(n, 1);
    for(int i = 2; i < n; ++i)
    {
        fac[i] = ((ll)fac[i - 1] * i) % p;
        invfac[i] = (ll)invfac[i-1] * inv[i] % p;
    }
}

void get_inv(ll p)
{
    inv.assign(p, 1);
    for(int i = 2; i < p; ++i)
        inv[i] = p - ((p / i) * inv[p % i]) % p;
}

int Comb(int n, int m, int p)
{
    if(n < 0 || m < 0 || n < m)
        return 0;
    return (ll)fac[n] * invfac[m] % p * invfac[n - m] % p;
}

ll Lucas(ll n, ll m, ll p)
{
    //Lucas定理求C(n,m)%p
    ll ans = 1;

    while(n && m && ans)
    {
        ans = (ans * Comb(n % p, m % p, p)) % p;
        n /= p;
        m /= p;
    }
    return ans;
}

// (1) 组合计数
int S(int x0, int y0, int x1, int y1)
{
    int n = x1 - x0, m = y1 - y0;
    int ans = 0;
    for(int i = 0; i <= min(n, m); ++i)
    {
        int tmp1 = Lucas(n + m - i, i, MOD);
        int tmp2 = Lucas(n + m - 2 * i, n - i, MOD);
        ans = (ans + (ll)tmp1 * tmp2) % MOD;
    }
    return ans;
}

// (2) 容斥计数
bool check(const int subset, const vector<Point>& p, const int K)
{
    int l = -1, r = -1;
    for(int i = 1; i <= K; ++i)
    {
        if((subset >> (i - 1) & 1) == 0)
            continue;
        if(l == -1)
        {
            l = i;
            continue;
        }
        else
        {
            r = i;
            if(p[l].x > p[r].x)
                return false;
            if(p[l].y > p[r].y)
                return false;
            l = r;
        }
    }
    return true;
}

int g(const int subset, const vector<vector<int>>& C, const int K)
{
    int l = 0, r = -1;
    int ans = 1;
    for(int i = 1; i <= K; ++i)
    {
        if(subset >> (i - 1) & 1)
        {
            r = i;
            ans = (ans * (ll)C[l][r]) % MOD;
            l = r;
        }
    }
    ans = (ans * (ll)C[l][K + 1]) % MOD;
    return ans;
}

int inclusion_exclusion(const vector<Point>& p, const vector<vector<int>>& C, const int K)
{
    vector<int> ones(1 << K, 0);
    for(int subset = 1; subset < (1 << K); ++subset)
        ones[subset] = ones[subset >> 1] + (subset & 1);

    int ans = 0;
    for(int subset = 1; subset < (1 << K); ++subset)
    {
        // (3) 枚举组合
        if(!check(subset, p, K))
            continue;
        int sign = 1;
        if(ones[subset] & 1)
            sign = -1;
        ans = (ans + sign * g(subset, C, K) + MOD) % MOD;
    }
    return ans;
}

int main()
{
    int n, m, K;
    cin >> n >> m >> K;

    get_inv(MOD);
    preprocess(MOD, MOD);

    vector<Point> p(K + 2);
    p[0] = Point(1, 1);
    p[K + 1] = Point(n + 1, m + 1);
    for(int i = 1; i <= K; ++i)
    {
        int x, y;
        cin >> x >> y;
        p[i] = Point(x, y);
    }
    // step1
    sort(p.begin(), p.end());

    // step2
    vector<vector<int>> C(K + 2, vector<int>(K + 2, -1));
    // step3
    for(int i = 0; i <= K; ++i)
        for(int j = i + 1; j <= K + 1; ++j)
            C[i][j] = S(p[i].x, p[i].y, p[j].x, p[j].y);

    // step4
    int ans = C[0][K + 1];

    // step5
    ans = ((ll)ans + inclusion_exclusion(p, C, K)) % MOD;

    cout << ans << endl;
}
