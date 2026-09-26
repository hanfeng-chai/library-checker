#include <bits/stdc++.h>
// #include <bits/extc++.h>
using namespace std;

// #define FILERR

using u32 = unsigned;
using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128_t;
using u128 = __uint128_t;

#define ll long long
#define db double
#define DB long db
#define pii pair<int, int>
#define fi first
#define se second
#define mkpr make_pair
#define vi vector<int>
#define v64 vector<i64>
#define vii vector<pii>
#define rsz resize
#define ep emplace
#define pb pop_back
#define eb emplace_back
#define all(c) (c).begin(), (c).end()
#define disc(c) (sort(all(c)), (c).rsz(unique(all(c)) - (c).begin()))
#define ers(S, x) ((S).erase((S).find(x)))
#define bit(k) (1u << (k))
#define Bit(k) (1ull << (k))
#define BIT(k) ((u128)1 << (k))
#define lowbit(x) ((x) & -(x))
#define bin(s, k) ((s) >> (k) & 1)
#define lg2(x) (31 ^ __builtin_clz(x))
#define LG2(x) (63 ^ __builtin_clzll(x))
#define highbit(x) bit(lg2(x))
#define highbitll(x) Bit(LG2(x))
#define popcnt(x) __builtin_popcount(x)
#define popcntll(x) __builtin_popcountll(x)
#define mem(a, x) memset(a, x, sizeof(a))
#define req(i, l, r) for (int i(l), i##End(r); i < i##End; i = -~i)
#define qer(i, r, l) for (int i(r), i##End(l); i > i##End; i = ~-i)
#define rep(i, l, r) for (int i(l), i##End(r); i <= i##End; i = -~i)
#define per(i, r, l) for (int i(r), i##End(l); i >= i##End; i = ~-i)

#ifdef JYR
#include "debug.h"
#define errm(x, ...) fprintf(stderr, x, ##__VA_ARGS__)
#define errs(x, ...) errm(x "\n", ##__VA_ARGS__)
#else
#define dbg(...) (__VA_ARGS__)
#define dbgArr(...) (__VA_ARGS__)
#define errm(x, ...) (x, ##__VA_ARGS__)
#define errs(x, ...) (x, ##__VA_ARGS__)
#endif

#define __

template<typename T, typename U> void chkmx(T &_a, U _b) { if (_a < _b) _a = _b; }
template<typename T, typename U> void chkmn(T &_a, U _b) { if (_b < _a) _a = _b; }

bool Mbe;

struct FastIO {
    char buf[1 << 20], *p1, *p2;
    char puf[1 << 20], *pf;
    
    FastIO() : p1(buf), p2(buf), pf(puf) {}
    ~FastIO() { fwrite(puf, 1, pf - puf, stdout); }
    
    char gc() {
        if (p1 == p2) p2 = (p1 = buf) + fread(buf, 1, 1 << 20, stdin);
        return p1 == p2 ? EOF : *p1++;
    }
    
    bool blank(char c) { return c == ' ' || c == '\r' || c == '\n' || c == '\t'; }
    
    char rd() {
        char c = gc(); while (blank(c)) c = gc();
        return c;
    }
    
    template<typename T> T rd() {
        T x = 0; int f = 0; char c = gc();
        while (!isdigit(c)) f = (c == '-'), c = gc();
        while (isdigit(c)) x = (x << 1) + (x << 3) + (c - '0'), c = gc();
        return f ? -x : x;
    }
    
    int rds(char *s) {
        char c = gc(), *S = s;
        while (blank(c)) c = gc();
        while (!blank(c) && c != EOF) *s++ = c, c = gc();
        return *s = 0, abs(s - S);
    }
    
    int rdl(char *s) {
        char c = gc(), *S = s;
        while (c == '\r' || c == '\n') c = gc();
        while (c != '\r' && c != '\n' && c != EOF) *s++ = c, c = gc();
        return *s = 0, abs(s - S);
    }
    
    void rd(char &c) { c = rd(); }
    
    void rd(char *s) {
        char c = gc();
        while (blank(c)) c = gc();
        if (c == EOF) { *s = 0; return; }
        while (!blank(c) && c != EOF) *s++ = c, c = gc();
        *s = 0;
    }
    
    void rd(string &s) {
        char c = gc(); s = "";
        while (blank(c)) c = gc();
        if (c == EOF) return;
        while (!blank(c) && c != EOF) s += c, c = gc();
    }
    
    template<typename T> void rd(T &x) {
        x = 0; int f = 0; char c = gc();
        while (!isdigit(c)) f = (c == '-'), c = gc();
        while (isdigit(c)) x = (x << 1) + (x << 3) + (c - '0'), c = gc();
        if (f) x = -x;
    }
    
    template<typename T, typename... Ts>
    void rd(T& x, Ts&... xs) { rd(x), rd(xs...); }
    
    template<typename T>
    void rda(T* x, int l, int r) { rep(i, l, r) rd(x[i]); }
    
    template<typename T>
    void rda(T* x, int n) { rep(i, 1, n) rd(x[i]); }
    
    template<typename T>
    void rda(T* l, T* r) { while (l != r) rd(*l++); }
    
    template<typename T>
    void rda(vector<T> &c) { for (auto &x : c) rd(x); }
    
    void pc(const char &c) {
        if (pf - puf == 1 << 20) fwrite(pf = puf, 1, 1 << 20, stdout);
        *pf++ = c;
    }
    
    void prt(char c) { pc(c); }
    void prt(char* s) { while (*s) pc(*s++); }
    void prt(const char* s) { while (*s) pc(*s++); }
    void prt(bool b) { pc(b ? '1' : '0'); }
    void prt(string s) { for (auto &&c : s) pc(c); }
    
    template<typename T> void prt(T x) {
        static int st[41], tp = 0;
        if (x == 0) { pc('0'); return; }
        if (x < 0) x = -x, pc('-');
        while (x) st[++tp] = x % 10, x /= 10;
        while (tp) pc(st[tp--] + '0');
    }
    
    template<typename T> void prt(T *x) { while (*x) pc(*x++); }
    
    template<typename T, typename... Ts>
    void prt(T x, Ts... xs) { prt(x), prt(xs...); }
    
    template<typename T> void prb(T x) { prt(x, ' '); }
    
    template<typename T> void prt(vector<T> vt) { for (auto x : vt) prb(x); }
    
    template<typename T> void prt(T* l, T* r) { while (l != r) prb(*l++); pc('\n'); }
    
    template<typename T, typename... Ts>
    void prb(T x, Ts... xs) { prt(x, ' '), prb(xs...); }
    
    template<typename T> void prd(T x) { prt(x, '\n'); }
    
    template<typename T, typename... Ts>
    void prd(T x, Ts... xs) { prt(x, ' '), prd(xs...); }
} IO;

#define rd IO.rd
#define rda IO.rda
#define rds IO.rds
#define rdl IO.rdl
#define r32 rd<int>()
#define r64 rd<i64>()
#define prt IO.prt
#define prs(...) prt(__VA_ARGS__, '\n')
#define prb IO.prb
#define prd IO.prd
#define edl IO.pc('\n')

#define MC

#define N 500005
#define mod 998244353
#define inf 0x3f3f3f3f
#define INF 0x3f3f3f3f3f3f3f3f

int n, m;
int st[N], tp;
struct P { i64 x, y; int i; } a[N];
P operator-(const P &u, const P &v) { return {u.x - v.x, u.y - v.y, 0}; }
i64 operator*(const P &u, const P &v) { return u.x * v.y - u.y * v.x; }
i64 D(const P &u) { return u.x * u.x + u.y * u.y; }
i64 S(const P &u, const P &v, const P &w) { return abs((v - u) * (w - u)); }

void mslv() {
    rd(n);
    rep(i, 1, n) { auto &[x, y, t] = a[i]; rd(x, y), t = i - 1; }
    sort(a + 1, a + n + 1, [](P u, P v) { return u.x != v.x ? u.x < v.x : u.y < v.y; });
    tp = 0;
    rep(i, 1, n) {
        while (tp > 1 && (a[st[tp]] - a[st[tp - 1]]) * (a[i] - a[st[tp - 1]]) >= 0) tp--;
        st[++tp] = i;
    } vi vt(st + 1, st + tp + 1); tp = 0;
    rep(i, 1, n) {
        while (tp > 1 && (a[st[tp]] - a[st[tp - 1]]) * (a[i] - a[st[tp - 1]]) <= 0) tp--;
        st[++tp] = i;
    } tp--; while (tp) vt.eb(st[tp--]);
    m = vt.size();
    if (m == 2) return prt("0 1\n");
    if (m == 3) return prd(a[vt[0]].i, a[vt[1]].i);
    req(i, 1, m) vt.eb(vt[i]);
    i64 ans = -1; int I = 0, J = 0;
    for (int i = 0, j = 1; i < m - 1; i++) {
        while (S(a[vt[i]], a[vt[i + 1]], a[vt[j]]) <= S(a[vt[i]], a[vt[i + 1]], a[vt[j + 1]])) j++;
        i64 x = D(a[vt[i]] - a[vt[j]]);
        if (ans < x) ans = x, I = a[vt[i]].i, J = a[vt[j]].i;
        x = D(a[vt[i + 1]] - a[vt[j]]);
        if (ans < x) ans = x, I = a[vt[i + 1]].i, J = a[vt[j]].i;
    } prd(I, J);
}

void mprw() {}

bool Med;

int main() {
    #ifdef JYR
    errs("\033[1;34mRunning!\033[0;m");
    freopen("Test.in", "r", stdin);
    freopen("Test.out", "w", stdout);
    #ifdef FILERR
    freopen("Test.err", "w", stderr);
    #endif
    #endif
    mprw();
    #ifdef MC
    int _ = r32;
    while (_--) errs("------------------------------"), mslv();
    errs("------------------------------");
    #else
    mslv();
    #endif
    #ifdef JYR
    errm("%.3lfMB ", abs(&Med - &Mbe) / 1048576.);
    errm("%.0lfms\n", clock() * 1000. / CLOCKS_PER_SEC);
    #endif
    return 0;
}