#include "bits/stdc++.h"
// #pragma GCC optimize("Ofast,unroll-loops")
// #pragma GCC target("avx2,tune=native")
using namespace std;

#define int long long
#define FOR(i, a, b) for (int i = (a), _##i = (b); i <= _##i; ++i)
#define FORD(i, a, b) for (int i = (a), _##i = (b); i >= _##i; --i)
#define REP(i, a) for (int i = 0, _##i = (a); i < _##i; ++i)
 
#define DEBUG(X) { auto _X = (X); cerr << "L" << __LINE__ << ": " << #X << " = " << (_X) << endl; }
#define PR(A, n) { cerr << "L" << __LINE__ << ": " << #A << " = "; FOR(_, 1, n) cerr << A[_] << ' '; cerr << endl; }
#define PR0(A, n) { cerr << "L" << __LINE__ << ": " << #A << " = "; REP(_, n) cerr << A[_] << ' '; cerr << endl; }
 
#define __builtin_popcount __builtin_popcountll
#define SZ(x) ((int)(x).size())
#define ALL(a) (a).begin(), (a).end()
#define TWO(x) (1LL<<(x))
inline int gcd(int a, int b) {int r; while (b) {r = a % b; a = b; b = r;} return a;}

// For printing pair, container, etc.
// Copied from https://quangloc99.github.io/2021/07/30/my-CP-debugging-template.html
template<class U, class V> ostream& operator << (ostream& out, const pair<U, V>& p) {
    return out << '(' << p.first << ", " << p.second << ')';
}

template<class Con, class = decltype(begin(declval<Con>()))>
typename enable_if<!is_same<Con, string>::value, ostream&>::type
operator << (ostream& out, const Con& con) {
    out << '{';
    for (auto beg = con.begin(), it = beg; it != con.end(); it++) {
        out << (it == beg ? "" : ", ") << *it;
    }
    return out << '}';
}
template<size_t i, class T> ostream& print_tuple_utils(ostream& out, const T& tup) {
    if constexpr(i == tuple_size<T>::value) return out << ")"; 
    else return print_tuple_utils<i + 1, T>(out << (i ? ", " : "(") << get<i>(tup), tup); 
}
template<class ...U> ostream& operator << (ostream& out, const tuple<U...>& t) {
    return print_tuple_utils<0, tuple<U...>>(out, t);
}

// for 64-bit, use mt19937_64
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int get_rand(int r) {
    return uniform_int_distribution<int> (0, r-1)(rng);
}

// use shuffle instead of random_shuffle
#define random_shuffle askcjaljc

// buffered_reader
namespace IO {
    const int BUFSIZE = 1<<14;
    char buf[BUFSIZE + 1], *inp = buf;

    bool reacheof;
    char get_char() {
        if (!*inp && !reacheof) {
            memset(buf, 0, sizeof buf);
            int tmp = fread(buf, 1, BUFSIZE, stdin);
            if (tmp != BUFSIZE) reacheof = true;
            inp = buf;
        }
        return *inp++;
    }
    template<typename T>
    T get_int() {
        int neg = 0;
        T res = 0;
        char c = get_char();
        while (!std::isdigit(c) && c != '-' && c != '+') c = get_char();
        if (c == '+') { neg = 0; }
        else if (c == '-') { neg = 1; }
        else res = c - '0';

        c = get_char();
        while (std::isdigit(c)) {
            res = res * 10 + (c - '0');
            c = get_char();
        }
        return neg ? -res : res;
    }
};

// Copied from https://judge.yosupo.jp/submission/16114
//
// Min-cost matching
// 
// Returns {total weight, matches (from left)}
const int N = 511;
template<typename T>
pair<T, vector<int>> Hungarian (int n, int m, T c[N][N]) {
    vector<T> v(m), dist(m);
    vector<int> L(n, -1), R(m, -1);
    vector<int> index(m), prev(m);
    auto getc = [&] (int i, int j) {return c[i][j] - v[j];};

    iota(index.begin(), index.end(), 0);
    for (int f = 0; f < n; ++f) {
        for (int j = 0; j < m; ++j) {
            dist[j] = getc(f, j), prev[j] = f;
        }
        T w = 0; int j, l = 0, s = 0, t = 0;
        while (true) {
            if (s == t) {
                l = s, w = dist[index[t++]];
                for (int k = t; k < m; ++k) {
                    j = index[k]; T h = dist[j];
                    if (h <= w) {
                        if (h < w) t = s, w = h;
                        index[k] = index[t], index[t++] = j;
                    }
                }
                for (int k = s; k < t; ++k) {
                    j = index[k];
                    if (R[j] < 0) goto augment;
                }
            }
            int q = index[s++], i = R[q];
            for (int k = t; k < m; ++k) {
                j = index[k];
                T h = getc(i, j) - getc(i, q) + w;
                if (h < dist[j]) {
                    dist[j] = h, prev[j] = i;
                    if (h == w) {
                        if (R[j] < 0) goto augment;
                        index[k] = index[t], index[t++] = j;
                    }
                }
            }
        }
    augment:
        for (int k = 0; k < l; ++k) v[index[k]] += dist[index[k]] - w;
        int i;
        do {
            i = R[j] = prev[j];
            swap(j, L[i]);
        } while (i != f);
    }
    T ret = 0;
    for (int i = 0; i < n; ++i) ret += c[i][L[i]];
    return {ret, L};
}

int c[N][N];

int32_t main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n = IO::get_int<int>();
    REP(i,n) REP(j,n) c[i][j] = IO::get_int<int>();

    auto [cost, matchL] = Hungarian(n, n, c);
    cout << cost << endl;
    for (int m : matchL) cout << m << ' ';
    cout << endl;
    return 0;
}
