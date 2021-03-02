

#include <vector>
#include <climits>
#include <iostream>
#include <fstream>

using namespace std;

using ll = long long;

class LazySegmentTree
{
public:
    LazySegmentTree(int n=0, int p=1e9+7)
    {
        this -> n = n;
        this -> MOD = p;
        tree.assign(n * 4, 0);
        lazy_add.assign(n * 4 , 0);
        lazy_mul.assign(n * 4, 1);
    }

    void range_update_add(int i, int j, int delta)
    {
        _range_update(0, 0, n, i, j, delta, 1);
    }

    void range_update_mul(int i, int j, int factor)
    {
        _range_update(0, 0, n, i, j, 0, factor);
    }

    int range_query(int i, int j)
    {
        return _range_query(0, 0, n, i, j);
    }

    int point_query(int i)
    {
        return _range_query(0, 0, n, i, i);
    }

    void point_update_add(int idx, int delta)
    {
        _range_update(0, 0, n, idx, idx, delta, 1);
    }

    void point_update_mul(int idx, int factor)
    {
        _range_update(0, 0, n, idx, idx, 0, factor);
    }

private:
    vector<int> tree;
    vector<int> lazy_add;
    vector<int> lazy_mul;
    int n;
    int MOD;

    void _range_update(int node, int l, int r, int i, int j, int delta, int factor)
    {
        if(lazy_add[node] != 0 || lazy_mul[node] != 1)
        {
            tree[node] = (tree[node] * (ll)lazy_mul[node] + (r - l + 1) * (ll)lazy_add[node]) % MOD;
            if(l != r)
            {
                lazy_mul[node * 2 + 1] = (lazy_mul[node * 2 + 1] * (ll)lazy_mul[node]) % MOD;
                lazy_add[node * 2 + 1] = (lazy_add[node * 2 + 1] * (ll)lazy_mul[node]) % MOD;
                lazy_add[node * 2 + 1] = (lazy_add[node * 2 + 1] + (ll)lazy_add[node]) % MOD;
                lazy_mul[node * 2 + 2] = (lazy_mul[node * 2 + 2] * (ll)lazy_mul[node]) % MOD;
                lazy_add[node * 2 + 2] = (lazy_add[node * 2 + 2] * (ll)lazy_mul[node]) % MOD;
                lazy_add[node * 2 + 2] = (lazy_add[node * 2 + 2] + (ll)lazy_add[node]) % MOD;
            }
            lazy_mul[node] = 1;
            lazy_add[node] = 0;
        }
        if(l > r || l > j || i > r)
            return;
        if(i <= l && j >= r)
        {
            tree[node] = (tree[node] * (ll)factor + (r - l + 1) * (ll)delta) % MOD;
            if(l != r)
            {
                lazy_mul[node * 2 + 1] = (lazy_mul[node * 2 + 1] * (ll)factor) % MOD;
                lazy_add[node * 2 + 1] = (lazy_add[node * 2 + 1] * (ll)factor) % MOD;
                lazy_add[node * 2 + 1] = (lazy_add[node * 2 + 1] + (ll)delta) % MOD;
                lazy_mul[node * 2 + 2] = (lazy_mul[node * 2 + 2] * (ll)factor) % MOD;
                lazy_add[node * 2 + 2] = (lazy_add[node * 2 + 2] * (ll)factor) % MOD;
                lazy_add[node * 2 + 2] = (lazy_add[node * 2 + 2] + (ll)delta) % MOD;
            }
            return;
        }
        int mid = (l + r) /2;
        _range_update(node * 2 + 1, l, mid, i, j, delta, factor);
        _range_update(node * 2 + 2, mid + 1, r, i, j, delta, factor);
        tree[node] = (tree[node * 2 + 1] + (ll)tree[node * 2 + 2]) % MOD;
    }

    int _range_query(int node, int l, int r, int i, int j)
    {
        if(l > r || l > j || r < i)
        {
            cout <<"---" << endl;
            return INT_MIN / 3;
        }
        // 需要保证 lazy_add 和 lazy_mul 只有一个有内容
        if(lazy_add[node] != 0 || lazy_mul[node] != 1)
        {
            tree[node] = (tree[node] * (ll)lazy_mul[node] + (r - l + 1) * (ll)lazy_add[node]) % MOD;
            if(l != r)
            {
                lazy_mul[node * 2 + 1] = (lazy_mul[node * 2 + 1] * (ll)lazy_mul[node]) % MOD;
                lazy_add[node * 2 + 1] = (lazy_add[node * 2 + 1] * (ll)lazy_mul[node]) % MOD;
                lazy_add[node * 2 + 1] = (lazy_add[node * 2 + 1] + (ll)lazy_add[node]) % MOD;
                lazy_mul[node * 2 + 2] = (lazy_mul[node * 2 + 2] * (ll)lazy_mul[node]) % MOD;
                lazy_add[node * 2 + 2] = (lazy_add[node * 2 + 2] * (ll)lazy_mul[node]) % MOD;
                lazy_add[node * 2 + 2] = (lazy_add[node * 2 + 2] + (ll)lazy_add[node]) % MOD;
            }
            lazy_mul[node] = 1;
            lazy_add[node] = 0;
        }
        if(i <= l && r <= j)
        {
            return tree[node];
        }
        int mid = (l + r) / 2;
        if(i > mid)
            return _range_query(node * 2 + 2, mid + 1, r, i, j);
        if(j <= mid)
            return _range_query(node * 2 + 1, l, mid, i, j);
        int ans_left = _range_query(node * 2 + 1, l, mid, i, j);
        int ans_right = _range_query(node * 2 + 2, mid + 1, r, i, j);
        int ans = 0;
        if(ans_left != INT_MIN / 3) ans = (ans + (ll)ans_left) % MOD;
        if(ans_right != INT_MIN / 3) ans = (ans + (ll)ans_right) % MOD;
        return ans;
    }
};

int main()
{
    // fstream fin("test/P3373_1.in");
    int n, m, p;
    cin >> m >> n >> p;
    // fin >> n >> m >> p;
    LazySegmentTree st(n + 1, p);
    for(int i = 1; i <= n; ++i)
    {
        int x;
        cin >> x;
        // fin >> x;
        st.point_update_add(i, x);
    }
    for(int i = 0; i < m; ++i)
    {
        int op, x, y, k;
        cin >> op >> x >> y;
        // fin >> op >> x >> y;
        if(op != 3)
            // fin >> k;
            cin >> k;
        if(op == 1)
            st.range_update_mul(x, y, k);
        else if(op == 2)
            st.range_update_add(x, y, k);
        else
            cout << st.range_query(x, y) << endl;
    }
}
