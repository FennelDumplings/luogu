
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;
using ll = long long;

struct Item
{
    int p;
    int d;
    Item(int p, int d):p(p),d(d){}
};

struct Cmp
{
    bool operator()(const Item& i1, const Item& i2) const
    {
        return i1.d < i2.d;
    }
};

struct HeapCmp
{
    bool operator()(const Item& i1, const Item& i2) const
    {
        return i1.p > i2.p;
    }
};

int main()
{
    // ifstream fin("P2949_2.in");
    int N;
    cin >> N;
    // fin >> N;
    vector<Item> items;
    for(int i = 0; i < N; ++i)
    {
        int p, d;
        cin >> d >> p;
        // fin >> d >> p;
        items.emplace_back(p, d);
    }
    sort(items.begin(), items.end(), Cmp());
    priority_queue<Item, vector<Item>, HeapCmp> pq;
    int left = 0;
    ll ans = 0;
    for(const Item& i: items)
    {
        pq.push(i);
        ++left;
        ans += i.p;
        if(left > i.d)
        {
            ans -= pq.top().p;
            pq.pop();
            --left;
        }
    }
    cout << ans << endl;
}
