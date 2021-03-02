#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <deque>
#include <iomanip>

using namespace std;

// 二维向量
const double PI = 2.0 * acos(0.0);
const double EPS = 1e-10;

struct Vector2
{
    double x, y;
    // 把构造函数指定 explicit，可以放置发生在 Vector2 的位置中加入实数的错误
    explicit Vector2(double x_=0, double y_=0):x(x_),y(y_){}

    // 比较两个向量
    bool operator==(const Vector2& vec) const
    {
        return fabs(x - vec.x) < EPS && fabs(y - vec.y) < EPS;
    }
    bool operator<(const Vector2& vec) const
    {
        if(fabs(x - vec.x) < EPS)
            return y < vec.y;
        return x < vec.x;
    }

    // 向量的加法和减法
    Vector2 operator+(const Vector2& vec) const
    {
        return Vector2(x + vec.x, y + vec.y);
    }
    Vector2 operator-(const Vector2& vec) const
    {
        return Vector2(x - vec.x, y - vec.y);
    }

    // 向量的数乘
    Vector2 operator*(double a) const
    {
        return Vector2(x * a, y * a);
    }

    // 向量的模
    double norm() const
    {
        return hypot(x, y);
    }

    // 返回方向相同的单位向量
    // 零向量不可调用
    Vector2 normalize() const
    {
        return Vector2(x / norm(), y / norm());
    }

    // 从 x 轴正方向转到当前向量的角度
    double polar() const
    {
        // atan2 返回 (-PI, PI]，修正为 [0，2PI)
        // fmod 求两个实数相除的余数
        return fmod(atan2(y, x) + 2 * PI, 2 * PI);
    }

    // 内积
    double dot(const Vector2& vec) const
    {
        return x * vec.x + y * vec.y;
    }
    double cross(const Vector2& vec) const
    {
        return x * vec.y - y * vec.x;
    }

    // 当前向量映射到另一向量的结果
    // vec的长度需要大于0
    Vector2 project(const Vector2& vec) const
    {
        Vector2 r = vec.normalize();
        return r * r.dot(*this);
    }

    // 两个向量的夹角
    // 两个向量长度均需要大于 0
    double insersection_angle(const Vector2& vec) const
    {
        return acos(dot(vec) / norm() * vec.norm());
    }

    // 判断两个向量是否垂直
    // 两个向量长度均需要大于 0
    bool perpendicular(const Vector2& vec) const
    {
        return fabs(dot(vec)) < EPS;
    }

    // 平行四边形面积
    double parallelogram_area(const Vector2& vec) const
    {
        return fabs(cross(vec));
    }

    // 三角形面积，可以扩找到多边形
    double triangle_area(const Vector2& vec) const
    {
        return fabs(cross(vec)) / 2;
    }
};

// 判定两个向量的方向
// ccw: counter clock wise
double ccw(const Vector2& a, const Vector2& b)
{
    // 正数: b 在 a 的逆时针 180 度内
    // 负数: b 在 a 的顺时针 180 度内
    // 0: 平行
    return a.cross(b);
}
double ccw(const Vector2& p, const Vector2& a, const Vector2& b)
{
    // 正数: 以 p 为基准点时，b 在 a 的逆时针 180 度内
    // 负数: 以 p 为基准点时，b 在 a 的顺时针 180 度内
    return ccw(a - p, b - p);
}

struct Line
{
    // line: a+pb, a 是点, b 是方向向量
    Vector2 a;
    Vector2 b;
    double deg; // 极角
    Line(){}
    Line(Vector2 a, Vector2 b):a(a),b(b)
    {
        deg = b.polar();
    }
    bool operator<(const Line& l) const
    {
        // 方向向量的极角序
        return deg < l.deg;
    }
};

bool on_left(const Line& l, const Vector2& p)
{
    // 点 p 是否在有向直线 l 左侧
    return l.b.cross(p - l.a) > EPS;
}

// 直线与直线相交
bool line_intersection(const Line& l1, const Line& l2, Vector2& x)
{
    double det = l1.b.cross(l2.b); // 两个方向向量的外积
    // 平行或重叠返回false
    if(fabs(det) < EPS)
        return false;
    // 两条有向直线的交点
    Vector2 u = l1.a - l2.a;
    double t = l2.b.cross(u) / det;
    x = l1.a + l1.b * t;
    return true;
}

vector<Vector2> half_plane_intersection(vector<Line>& lines)
{
    // 极角序
    sort(lines.begin(), lines.end());
    // 去除极角相同时右边的边
    vector<Line> tmp;
    for(int i = 0; i < (int)lines.size(); ++i)
    {
        if(tmp.empty() || fabs(tmp.back().deg - lines[i].deg) > EPS)
        {
            tmp.push_back(lines[i]);
        }
        else if(on_left(tmp.back(), lines[i].a))
        {
            tmp.pop_back();
            tmp.push_back(lines[i]);
        }
    }
    lines.swap(tmp);
    int N = lines.size();
    // cout << N << endl;
    // 有向直线集合表示半平面，初始只有半平面 {0}
    vector<Line> deq(N); // 数组形式双端队列
    // deq_p[i] := lines[i] 与 lines[(i + 1) % N] 的交点
    vector<Vector2> deq_p(N);
    int left = 0, right = 0;
    deq[0] = lines[0];
    for(int i = 1; i < N; ++i)
    {
        // cout << lines[i].a.x << " " << lines[i].a.y << " : " << lines[i].deg << endl;
        // 判断该线段加入后对半平面交的影响，对双端队列的头部和尾部进行判断，因为线段加入是有序的
        while(left < right && !on_left(lines[i], deq_p[right - 1]))
            --right;
        while(left < right && !on_left(lines[i], deq_p[left]))
            ++left;
        deq[++right] = lines[i];
        if(left < right)
            line_intersection(deq[right - 1], deq[right], deq_p[right - 1]);
    }
    while(left < right && !on_left(deq[left], deq_p[right - 1]))
        --right;
    if(right - left <= 1)
        return {};
    // 首位两个半平面的交
    line_intersection(deq[right], deq[left], deq_p[right]);
    vector<Vector2> poly(right - left + 1);
    for(int i = 0; i <= right - left; ++i)
        poly[i] = deq_p[left + i];
    return poly;
}

// 简单多边形 p 的有向面积
double area(const vector<Vector2>& p)
{
    double ans = 0.0;
    int n = p.size();
    for(int i = 0; i < n; ++i)
        ans += p[i].cross(p[(i + 1) % n]);
    return ans / 2.0;
}

int main()
{
    int n;
    cin >> n;
    vector<Vector2> A; // 点
    vector<Line> lines; // 边
    A.assign(n, Vector2());
    for(int j = 0; j < n; ++j)
    {
        int x, y;
        cin >> x >> y;
        A[j].x = x;
        A[j].y = y;
    }
    // 如果有向面积为负，则点是顺时针给的，翻转成逆时针
    if(area(A) < -EPS)
        reverse(A.begin(), A.end());
    // 建边，逆时针方向
    for(int j = 0; j < n; ++j)
    {
        int k = (j + 1) % n;
        lines.emplace_back(A[j], (A[k] - A[j]).normalize());
    }
    vector<Vector2> poly = half_plane_intersection(lines);
    double ans = fabs(area(poly));
    cout << std::fixed << std::setprecision(2);
    cout << ans + 1e-9 << endl;
}
