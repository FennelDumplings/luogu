#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <unordered_set>

using namespace std;
using ll = long long;

struct Point
{
    int id;
    int p;
    list<int>::iterator iter;
    Point(){}
    Point(int id, int p, list<int>::iterator iter):id(id),p(p),iter(iter){}
};

struct HeapCmp
{
    bool operator()(const Point& i1, const Point& i2) const
    {
        return i1.p < i2.p;
    }
};

int main()
{
    // fstream fin("P1484_6.in");
    int n, k;
    cin >> n >> k;
    // fin >> n >> k;
    vector<int> p(n);
    for(int i = 0; i < n; ++i)
        // fin >> p[i];
        cin >> p[i];

    list<int> l(n);
    int id = 0;
    for(auto it = l.begin(); it != l.end(); it++)
    {
        *it = id;
        id++;
    }
    priority_queue<Point, vector<Point>, HeapCmp> pq;

    for(auto it = l.begin(); it != l.end(); it++)
    {
        int id = *it;
        pq.push(Point(id, p[id], it));
    }

    unordered_set<int> setting; // 不可选的 id

    ll ans = 0;
    while(k > 0 && !pq.empty() && pq.top().p > 0)
    {
        if(setting.count(pq.top().id) > 0)
        {
            pq.pop();
            continue;
        }
        k--;
        // 贪心地选 b 点
        int b = pq.top().p;
        ans += b;
        auto it = pq.top().iter;
        pq.pop();

        // 访问到 a, c 两点
        auto next_it = next(it);
        auto prev_it = it;
        if(prev_it == l.begin())
            prev_it = l.end();
        else
            prev_it = prev(prev_it);

        if(prev_it == l.end() && next_it == l.end())
        {
            // 只剩 1 个节点
            continue;
        }

        if(prev_it != l.end() && next_it != l.end())
        {
            int a  = p[*prev_it];
            int c  = p[*next_it];

            // 新增 P 点，链表节点仍为 it，id 不变，值变为 a + c - b
            p[*it] = a + c - b;
            pq.push(Point(*it, a + c - b, it));
        }

        // 标记 a, c 不可选
        if(prev_it != l.end())
        {
            setting.insert(*prev_it);
            l.erase(prev_it);
        }
        if(next_it != l.end())
        {
            setting.insert(*next_it);
            l.erase(next_it);
        }
    }
    cout << ans << endl;
}
