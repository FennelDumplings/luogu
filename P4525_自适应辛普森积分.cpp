#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

const double EPS = 1e-9;

struct Function
{
    double a, b, c, d;
    Function(double a, double b, double c, double d):a(a),b(b),c(c),d(d){}
    double operator()(double x) const
    {
        return (c * x + d) / (a * x + b);
    }
};

double simpson(Function &f, double a, double b)
{
    // f(x) 在 [a, b] 上的积分
    double mid = (a + b) / 2;
    return (f(a) + 4 * f(mid) + f(b)) * (b - a) / 6;
}

double adaptive_simpson(Function& f, double a, double b)
{
    double mid = (a + b) / 2;
    double l_ans = simpson(f, a, mid);
    double r_ans = simpson(f, mid, b);
    double ans = simpson(f, a, b);
    if(fabs(l_ans + r_ans - ans) < EPS)
        return ans;
    return adaptive_simpson(f, a, mid) + adaptive_simpson(f, mid, b);
}

int main()
{
    double a, b, c, d, L, R;
    cin >> a >> b >> c >> d >> L >> R;
    Function f(a, b, c, d);
    double ans = adaptive_simpson(f, L, R);
    cout << std::fixed << std::setprecision(6);
    cout << ans + 1e-9 << endl;
}
