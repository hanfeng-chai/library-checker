#pragma GCC optimize("O3")

#include <bits/stdc++.h>

using namespace std;

#define IOS ios::sync_with_stdio(false); cin.tie(0)
#define ll long long
#define ld long double
#define mp(a, b) make_pair(a, b)
#define pii pair<int, int>
#define pll pair<ll, ll>
#define pil pair<int, ll>
#define pli pair<ll, int>
#define fi first
#define se second


namespace FastIO {
	char buf[1 << 21], *p1 = buf, *p2 = buf;
#define getchar() (p1 == p2 && (p1 = buf, p2 = (p1 + fread(buf, 1, 1 << 21, stdin))) == p1 ? EOF : *p1++)
	template <typename T> inline T read() { T x = 0, w = 0; char ch = getchar(); while (ch < '0' || ch > '9') w |= (ch == '-'), ch = getchar(); while ('0' <= ch && ch <= '9') x = x * 10 + (ch ^ '0'), ch = getchar(); return w ? -x : x; }
	template <typename T> inline void write(T x) { if (!x) return; write<T>(x / 10), putchar((x % 10) ^ '0'); }
	template <typename T> inline void print(T x) { if (x > 0) write<T>(x); else if (x < 0) putchar('-'), write<T>(-x); else putchar('0'); }
	template <typename T> inline void print(T x, char en) { print<T>(x), putchar(en); }
}; using namespace FastIO;
#undef getchar()


struct P {
    long long x, y;
    int ind;

    void read1() {
        x = read<ll>();
        y = read<ll>();
    }

    P operator-(const P& b) const {
        return P{x - b.x, y - b.y};
    }
    void operator-=(const P& b) {
        x -= b.x;
        y -= b.y;
    }

    long long operator* (const P& b) const {
        return x * b.y - y * b.x;
    }
    long long triangle(const P& b, const P& c) const{
        return (b - *this) * (c - *this);
    }
    bool operator <(const P& b) const {
        return make_pair(y, x) < make_pair(b.y, b.x);
    }
    /*bool operator ==(P b) const {
      return (x == b.x) && (y == b.y);
      }*/
    long long dist(const P& b) const {
        return (x - b.x) * (x - b.x) + (y - b.y) * (y - b.y);
    }
};

bool cmp(const P& a, const P& b) {
    return a.x < b.x;
}


const int N = 2e5 + 10;
const ll LINF = 1e18;
const int INF = 1e9;
const ld PI = 4*atan((ld)1);
const int dx[4] = {1,0,-1,0}, dy[4] = {0,1,0,-1};

int main() {
    int t = read<int>();
    while(t--) {

        int n = read<int>();
        ll d = LLONG_MAX;
        ll sq = LINF;

        vector<P> p(n);
        for (int i = 0; i < n; ++i) {
            p[i].read1();
            p[i].ind = i;
        }

        sort(p.begin(), p.end(), cmp);
        set<P> s;
        pii res;

        for (int i = 0, j = 0; i < n; ++i) {
            while(j < i && p[i].x - p[j].x >= sq) {
                s.erase(p[j]);
                ++j;
            }

            if (!s.empty()) {
                auto it = s.lower_bound(P{ -INF, p[i].y - sq, 0});
                while(it != s.end() && it->y < p[i].y + sq) {
                    auto tmp = p[i].dist(*it);
                    if (tmp < d) {
                        d = tmp;
                        res = {it->ind, p[i].ind};
                        sq = sqrtl(d) + 1;
                    }
                    ++it;
                }
            }
            s.insert(p[i]);
        }
        print<ll>(res.fi, ' ');
        print<ll>(res.se, '\n');
    }
}