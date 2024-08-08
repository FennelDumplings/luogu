#include <iostream>
#include <vector>

using namespace std;

vector<int> get_primes(int n)
{
    if(n < 2)
        return {};
    vector<bool> vec(n, true);
    vec[0] = false;
    vec[1] = false;
    int cnt = 0;
    vector<int> primes;
    for(int i = 2; i <= n; ++i)
    {
        if(vec[i])
        {
            ++cnt;
            primes.push_back(i);
        }
        for(int j = 0; j < cnt && i * primes[j] < n; ++j)
        {
            vec[i * primes[j]] = false;
            if(i % primes[j] == 0)
                break;
        }
    }
    return primes;
}

int main()
{
    // n <= 200
    int n;
    cin >> n;

    // 预处理 200 以内的所有质数
    vector<int> primes = get_primes(n);
    int m = primes.size();
    cout << m << endl;

    vector<int> a(n + 1, 0);
    vector<int> s;
    for(int k = 0; k < n / 2; ++k)
        a[k * 2] = 1;
    for(int j = 2; j < m; ++j)
    {
        s.assign(n, 0);
        // 将多项式展开
        if(primes[j] > n)
            break;
        for(int i = 0; i < n; ++i)
        {
            if(a[i] > 0)
            {
                for(int k = 1; k < (n - i) / primes[j] + 1; ++k)
                {
                    s[i + primes[j] * k] += a[i];
                }
            }
        }
        for(int i = 0; i <= n; ++i)
            a[i] += s[i];
    }
    cout << a[n] << endl;
}
















