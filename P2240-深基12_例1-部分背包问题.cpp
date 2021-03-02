// P2240 【深基12.例1】部分背包问题

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct Item
{
    int v, w; // v: 体积, w: 价值
    Item(int v, int w):v(v),w(w){}
};

struct Cmp
{
    bool operator()(const Item& i1, const Item& i2) const
    {
        return (double)i1.w / i1.v > (double)i2.w / i2.v;
    }
};

int main()
{
    int N, T;
    cin >> N >> T;
    vector<Item> items(N, Item(-1, -1));
    for(int i = 0; i < N; ++i)
    {
        cin >> items[i].v;
        cin >> items[i].w;
    }
    sort(items.begin(), items.end(), Cmp());
    double ans = 0.0;
    for(const Item &item: items)
    {
        if(item.v >= T)
        {
            ans += (double)item.w / item.v * T;
            break;
        }
        else
        {
            ans += (double)item.w;
            T -= item.v;
        }
    }
    cout.precision(2);
    cout.setf(ios::fixed);
    cout << ans << endl;
}
