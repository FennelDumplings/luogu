#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main()
{
    int N;
    cin >> N;
    vector<int> p(N);
    for(int i = 0; i < N; ++i)
        cin >> p[i];
    priority_queue<int, vector<int>, greater<int>> pq;
    int ans = 0;
    for(int i = 0; i < N; ++i)
    {
        pq.push(p[i]);
        if(!pq.empty() and p[i] > pq.top())
        {
            ans += p[i] - pq.top();
            pq.pop();
            pq.push(p[i]);
        }
    }
    cout << ans << endl;
}
