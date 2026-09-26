#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <tr2/dynamic_bitset>

using namespace std;
using namespace __gnu_pbds;
using namespace tr2;

#define ar array
#define vt vector
#define pq priority_queue
#define pu push
#define pub push_back
#define em emplace
#define emb emplace_back
#define mt make_tuple

#define all(x) x.begin(), x.end()
#define allr(x) x.rbegin(), x.rend()
#define allp(x, l, r) x.begin() + l, x.begin() + r
#define len(x) (int)x.size()
#define uniq(x) unique(all(x)), x.end()

using ll = long long;
using ld = long double;
using ull = unsigned long long;

template<class Fun> class y_combinator_result {
    Fun fun_;
public:
    template<class T> explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}
    template<class ...Args> decltype(auto) operator()(Args &&...args) { return fun_(std::ref(*this), std::forward<Args>(args)...); }
};
template<class Fun> decltype(auto) y_combinator(Fun &&fun) { return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun)); }

template <class T, size_t N>
void re(array <T, N>& x);
template <class T> 
void re(vt <T>& x);

template <class T> 
void re(T& x) {
    cin >> x;
}

template <class T, class... M> 
void re(T& x, M&... args) {
    re(x), re(args...);
}

template <class T> 
void re(vt <T>& x) {
    for(auto& it : x) re(it);
}

template <class T, size_t N>
void re(array <T, N>& x) {
    for(auto& it : x) re(it);
}

template <class T, size_t N>
void wr(const array <T, N>& x);
template <class T> 
void wr(const vt <T>& x);

template <class T> 
void wr(const T& x) {
    cout << x;
}

template <class T, class ...M>  
void wr(const T& x, const M&... args) {
    wr(x), wr(args...);
}

template <class T> 
void wr(const vt <T>& x) {
    for(auto it : x) wr(it, ' ');
}

template <class T, size_t N>
void wr(const array <T, N>& x) {
    for(auto it : x) wr(it, ' ');
}

template<class T, class... M>
auto mvt(size_t n, M&&... args) {
    if constexpr(sizeof...(args) == 1)
        return vector<T>(n, args...);
    else
        return vector(n, mvt<T>(args...));
}

void set_fixed(int p = 0) {
    cout << fixed << setprecision(p);
}

void set_scientific() {
    cout << scientific;
}

void Open(const string& name) {
#ifndef ONLINE_JUDGE
    (void)!freopen((name + ".in").c_str(), "r", stdin);
    (void)!freopen((name + ".out").c_str(), "w", stdout);
#endif
}

struct Stirling_Number_Query {
    const int p;
    vt <vt <int>> C, S1, S2;
    
    Stirling_Number_Query(int p, bool first_kind = true, bool second_kind = true): p(p) {
        build_C();
        if (first_kind)  build_S1();
        if (second_kind) build_S2();
    }

    int nCr(ll n, ll k) {
        if (k < 0 || k > n) return 0;
        int res = 1;
        while (n) {
            int i = n % p, j = k % p;
            if (j > i) return 0;
            res = (res * C[i][j]) % p;
            n /= p;
            k /= p;
        }
        return res;
    }

    int s(ll n, ll k) {
        if (k < 0 || k > n) return 0;
        ll i = n / p, j = n % p;
        if (i > k) return 0;

        ll a = (k - i) / (p - 1);
        ll b = (k - i) % (p - 1);

        if (b == 0 && j > 0) {
            b += (p - 1);
            --a;
        }

        if (a < 0 || i < a || b > j) return 0;

        int x = nCr(i, a);
        int y = S1[j][b];
        int res = x * y % p;
        if ((i + a) % 2 == 1 && res) res = p - res;
        return res;
    }

    int S(ll n, ll k) {
        if (k < 0 || k > n) return 0;
        if(n < p) return S2[n][k];

        ll i = k / p, j = k % p;
        if (n < i) return 0;

        ll a = (n - i) / (p - 1);
        ll b = (n - i) - (p - 1) * a;  

        if (b == 0) {
            b += p - 1;
            --a;
        }

        if (a < 0 || j > b) return 0;
        if (b < p - 1) return nCr(a, i) * S2[b][j] % p;
        if (j == 0) return nCr(a, i - 1);

        return nCr(a, i) * S2[p - 1][j] % p;
    }

    void build_C() {
        auto& A = C;
        A.resize(p);
        A[0] = {1};
        for (int i = 1; i < p; ++i) {
            A[i] = A[i - 1];
            A[i].emb(0);
            for (int j = 1; j <= i; ++j) {
                A[i][j] += A[i - 1][j - 1];
                if (A[i][j] >= p) A[i][j] -= p;
            }
        } 
    }

    void build_S1() {
        auto& A = S1;
        A.resize(p);

        A[0] = {1};
        for (int i = 1; i < p; ++i) {
            A[i].assign(i + 1, 0);
            for (int j = 0; j <= i; ++j) {
                if (j) A[i][j] += A[i - 1][j - 1];
                if (j < i) A[i][j] += A[i - 1][j] * (p - i + 1);
                A[i][j] %= p;
            }
        }
    }

    void build_S2() {
        auto& A = S2;
        A.resize(p);

        A[0] = {1};
        for (int i = 1; i < p; ++i) {
            A[i].assign(i + 1, 0);
            for (int j = 0; j <= i; ++j) {
                if (j) A[i][j] += A[i - 1][j - 1];
                if (j < i) A[i][j] += A[i - 1][j] * j;
                A[i][j] %= p;
            }
        }
    }
};

void solve(Stirling_Number_Query& Q) {
    ll n, k; re(n, k);
    wr(Q.S(n, k), '\n');
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    //Open("");

    int t, p; re(t, p);
    Stirling_Number_Query Q(p, false, true);
    for(;t;t--) {
        solve(Q);
    }
    
    return 0;
}
