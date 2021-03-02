#include <vector>
#include <climits>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

using ll = long long;

class SegmentTree
{
public:
    SegmentTree(vector<ll> *ys)
    {
        this -> n = ys -> size();
        this -> ys = ys;
        tree.assign(n * 8, 0);
        len.assign(n * 8, 0);
    }

    void range_update_add(int i, int j, int delta)
    {
        _range_update(0, 0, n - 1, i, j, delta);
    }

    ll query()
    {
        // 查询整个区间
        return len[0];
    }

private:
    vector<int> tree; // 节点被覆盖的次数 cnt
    vector<ll> len; // 节点代表的区间被覆盖的长度 len
    vector<ll> *ys;
    int n;

    void _range_update(int node, int l, int r, int i, int j, int delta)
    {
        // 线段树节点 [l, r]
        if(l > r || l > j || i > r)
            return;
        if(i <= l && j >= r)
        {
            tree[node] += delta;
            if(tree[node] > 0)
                len[node] = (*ys)[r + 1] - (*ys)[l];
            else
                len[node] = len[node * 2 + 1] + len[node * 2 + 2];
            return;
        }
        int mid = (l + r) / 2;
        // if(i <= mid)
            _range_update(node * 2 + 1, l, mid, i, j, delta);
        // if(mid < j)
            _range_update(node * 2 + 2, mid + 1, r, i, j, delta);
        if(tree[node] > 0)
            len[node] = (*ys)[r + 1] - (*ys)[l];
        else
            len[node] = len[node * 2 + 1] + len[node * 2 + 2];
    }
};


struct Rectangle
{
    ll x1, y1, x2, y2;
    Rectangle(ll x1, ll y1, ll x2, ll y2):x1(x1),y1(y1),x2(x2),y2(y2){}
};

struct Event
{
    vector<Rectangle> *recs;
    int id; // 所属正方形 id
    int left; // 1 表示左边，-1 表示右边
    Event(vector<Rectangle> *recs, int id, int left):recs(recs),id(id),left(left){}
    ll getx() const
    {
        if(left == 1)
            return (*recs)[id].x1;
        else
            return (*recs)[id].x2;
    }
    bool operator<(const Event& e) const
    {
        if(getx() == e.getx())
            return left > e.left;
        return getx() < e.getx();
    }
};

int find(ll v, const vector<ll>& aa) // 从 nums 的值找到对应的离散化之后的值
{
    return lower_bound(aa.begin(), aa.end(), v) - aa.begin();
}

ll union_area(vector<Rectangle>& recs)
{
    int n = recs.size();
    vector<Event> events;
    // 将 y 离散化，每经过一个 x，会有一条边变化，左边则加，右边则减，该变化会对应离散化后的某些区间产生影响。
    vector<ll> ys;
    for(int i = 0; i < n; ++i)
    {
        ys.push_back(recs[i].y1);
        ys.push_back(recs[i].y2);
        events.emplace_back(&recs, i, 1);
        events.emplace_back(&recs, i, -1);
    }
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    sort(events.begin(), events.end());

    SegmentTree sttree(&ys);
    // 遍历事件
    // 统计事件对应的左边或右边对有几个离散化后的区间产生了影响，并记录。
    // cnt[i] := [ys[i], ys[i+1]) 上的矩形个数
    ll ans = 0;
    for(int i = 0; i < (int)events.size(); ++i)
    {
        ll x = events[i].getx();
        int delta = events[i].left;
        ll y1 = recs[events[i].id].y1;
        ll y2 = recs[events[i].id].y2;
        int l = find(y1, ys);
        int r = find(y2, ys);
        // 影响 ys 上的 [l, r) 区间
        sttree.range_update_add(l, r - 1, delta);
        ll len_y = sttree.query();
        if(i + 1 < (int)events.size())
        {
            ans += (events[i + 1].getx() - x) * len_y;
        }
    }
    return ans;
}

int main()
{
    int n;
    cin >> n;
    vector<Rectangle> recs;
    for(int i = 0; i < n; ++i)
    {
        ll x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        recs.emplace_back(x1, y1, x2, y2);
    }
    ll ans = union_area(recs);
    cout << ans << endl;
}
