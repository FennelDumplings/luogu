#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <unordered_set>

using namespace std;

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
    int n, k;
    cin >> n >> k;
    vector<int> p(n);
    for(int i = 0; i < n; ++i)
        cin >> p[i];

    if(k > n / 2)
    {
        cout << "Error!" << endl;
        return 0;
    }

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

    int ans = 0;
    while(k > 0 && !pq.empty())
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
        if(next_it == l.end())
            next_it = l.begin();
        auto prev_it = it;
        if(prev_it == l.begin())
            prev_it = l.end();
        prev_it = prev(prev_it);

        if(prev_it == it || prev_it == next_it)
            continue;

        int a  = p[*prev_it];
        int c  = p[*next_it];

        // 标记 a, c 不可选
        setting.insert(*next_it);
        setting.insert(*prev_it);
        l.erase(prev_it);
        l.erase(next_it);

        // 新增 P 点，链表节点仍为 it，id 不变，值变为 a + c - b
        p[*it] = a + c - b;
        pq.push(Point(*it, a + c - b, it));
    }
    cout << ans << endl;
}
