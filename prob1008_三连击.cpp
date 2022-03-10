#include <iostream>
#include <vector>

using namespace std;

struct Group
{
    int a, b, c;
    Group(){}
    Group(int a, int b, int c):a(a),b(b),c(c){}
};

void dfs(const vector<int>& nums, vector<Group>& groups, int i, int used, int a)
{
    if(i == 3)
    {
        if(a > (1000 / 3))
            return;
        int b = a * 2;
        int c = a * 3;
        int used_tmp = used;
        while(b > 0)
        {
            int digit = b % 10;
            if(used_tmp >> digit & 1)
                return;
            if(digit == 0)
                return;
            used_tmp |= (1 << digit);
            b /= 10;
        }
        while(c > 0)
        {
            int digit = c % 10;
            if(used_tmp >> digit & 1)
                return;
            if(digit == 0)
                return;
            used_tmp |= (1 << digit);
            c /= 10;
        }
        groups.emplace_back(a, a * 2, a * 3);
        return;
    }
    for(int j = 1; j <= 9; ++j)
    {
        if(used >> j & 1)
            continue;
        used |= (1 << j);
        a *= 10;
        a += j;
        dfs(nums, groups, i + 1, used, a);
        a -= j;
        a /= 10;
        used &= ~(1 << j);
    }
}

int main()
{
    vector<Group> groups;
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int used = 0;
    dfs(nums, groups, 0, used, 0);
    for(const Group& group: groups)
    {
        cout << group.a << " " << group.b << " " << group.c << endl;;
    }
}
