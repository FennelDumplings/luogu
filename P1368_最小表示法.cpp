#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int N;
    cin >> N;
    vector<int> s(N);
    for(int i = 0; i < N; ++i)
        cin >> s[i];

    int i = 0, j = 1;
    while(i < N && j < N)
    {
        int k = 0;
        while(k < N && s[(i + k) % N] == s[(j + k) % N])
            k++;
        if(k == N)
            break;
        if(s[(i + k) % N] < s[(j + k) % N])
        {
            j += k + 1;
            if(i == j)
                j++;
        }
        else
        {
            i += k + 1;
            if(i == j)
                i++;
        }
    }
    int ans = min(i, j);
    for(int k = 0; k < N; ++k)
        cout << s[(ans + k) % N] << " ";
    cout << endl;
}
