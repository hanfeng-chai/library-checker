// COMPUTE CHROMATIC POLYNOMIAL OF A GRAPH G

// -- IMPLEMENTS DELETION/CONTRACTION ALGORITHM
// -- LIMITED TO GRAPHS WITH AT MOST 64 NODES
// -- OUTPUTS COEFFICIENTS OF  x^0  x^1 ... x^n  OF CHROMATIC POLYNOMIAL
// -- ALLOWS COEFFICIENTS TO BE EXPRESSED AS MODULAR INT, INT64, INT128, OR
//      OTHER USER-DEFINED INT SUCH AS BIG INTEGER
// -- PASSES TESTS AT  https://judge.yosupo.jp/problem/chromatic_polynomial

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <stack>
#include <unordered_map>
#include <vector>
using namespace std;

// MODULAR INTEGER CLASS, STORED IN MONTGOMERY FORM
template <uint32_t mod>
class Mint {
   private:
    using i32 = int32_t;
    using i64 = int64_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    static constexpr u32 get_r() {
        u32 ret = mod;
        for (i32 i = 0; i < 4; ++i) ret *= 2 - mod * ret;
        return ret;
    }

    static constexpr u32 reduce(const u64 &b) {
        return (b + u64(u32(b) * u32(-r)) * mod) >> 32;
    }

    // CLASS MEMBER DATA
    static constexpr u32 r = get_r();
    static constexpr u32 n2 = -u64(mod) % mod;
    u32 a;

    static_assert(r * mod == 1, "invalid, r * mod != 1");
    static_assert(mod < (1 << 30), "invalid, mod >= 2 ^ 30");
    static_assert((mod & 1) == 1, "invalid, mod % 2 == 0");

   public:
    constexpr Mint() : a(0) {}
    constexpr Mint(i64 b) : a(reduce(u64(b % mod + mod) * n2)){};

    constexpr Mint &operator+=(const Mint &b) {
        if (i32(a += b.a - 2 * mod) < 0) a += 2 * mod;
        return *this;
    }
    constexpr Mint &operator-=(const Mint &b) {
        if (i32(a -= b.a) < 0) a += 2 * mod;
        return *this;
    }
    constexpr Mint &operator*=(const Mint &b) {
        a = reduce(u64(a) * b.a);
        return *this;
    }
    constexpr Mint &operator/=(const Mint &b) {
        *this *= b.inverse();
        return *this;
    }

    constexpr Mint operator+(const Mint &b) const { return Mint(*this) += b; }
    constexpr Mint operator-(const Mint &b) const { return Mint(*this) -= b; }
    constexpr Mint operator*(const Mint &b) const { return Mint(*this) *= b; }
    constexpr Mint operator/(const Mint &b) const { return Mint(*this) /= b; }
    constexpr Mint operator-() const { return Mint() - Mint(*this); }
    Mint &operator++() { return *this += Mint(1); }
    Mint &operator--() { return *this -= Mint(1); }

    constexpr Mint pow(u64 n) const {
        Mint ret(1), mul(*this);
        while (n > 0) {
            if (n & 1) ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }
    constexpr Mint inverse() const { return pow(mod - 2); }
    constexpr u32 get() const {
        u32 ret = reduce(a);
        return ret >= mod ? ret - mod : ret;
    }
    static constexpr u32 get_mod() { return mod; }

    constexpr bool operator==(const Mint &b) const {
        return (a >= mod ? a - mod : a) == (b.a >= mod ? b.a - mod : b.a);
    }
    constexpr bool operator!=(const Mint &b) const {
        return (a >= mod ? a - mod : a) != (b.a >= mod ? b.a - mod : b.a);
    }

    friend ostream &operator<<(ostream &os, const Mint &b) {
        return os << b.get();
    }
    friend istream &operator>>(istream &is, Mint &b) {
        int64_t t;
        is >> t;
        b = Mint(t);
        return is;
    }
};

// POLYNOMIAL CLASS
template <typename T>
class Polynomial {
   public:
    using i64 = int64_t;
    using P = Polynomial<T>;

    vector<T> a;

   public:
    constexpr Polynomial(size_t sz = 0) { a.resize(sz, 0); }
    constexpr Polynomial(const initializer_list<T> v) { a = v; }
    constexpr Polynomial(const vector<T> &v) { a = v; }
    constexpr Polynomial(size_t sz, T val) { a.resize(sz, val); }
    constexpr size_t size() const { return a.size(); }
    void resize(size_t sz, T m = 0) { (*this).a.resize(sz, m); }

    constexpr P operator-(const P &a) const { return P(*this) -= a; }
    constexpr P operator+(const P &a) const { return P(*this) += a; }
    constexpr P operator*(const i64 a) const { return P(*this) *= a; }
    constexpr P operator*(const P &a) const { return P(*this) *= a; }

    P &operator+=(const P &r) {
        this->resize(max(this->size(), r.size()));
        for (size_t i = 0; i < r.size(); ++i) this->a[i] += r.a[i];
        return *this;
    }
    P &operator-=(const P &r) {
        this->resize(max(this->size(), r.size()));
        for (size_t i = 0; i < r.size(); ++i) this->a[i] -= r.a[i];
        return *this;
    }
    P &operator*=(const i64 v) {
        for (size_t i = 0; i < this->size(); ++i) this->a[i] *= v;
        return *this;
    }
    P &operator*=(const P &r) {
        this->convolution_inplace(r);
        return *this;
    }
    P pow(int b) {
        P res = {1};
        P a = *this;
        while (b > 0) {
            if (b & 1) res = a * res;
            b >>= 1;
            if (b > 0) a = a * a;
        }
        return res;
    }
    T evaluate(T x) {
        T ans = 0, powx = 1;
        for (auto &v : a) {
            ans += v * powx;
            powx *= x;
        }
        return ans;
    }
    friend std::ostream &operator<<(std::ostream &os, const P &p) {
        for (auto &v : p.a) os << v << " ";
        return os;
    }
    friend istream &operator>>(istream &is, P &p) {
        for (auto &v : p.a) {
            i64 w;
            is >> w;
            v = w;
        }
        return is;
    }

   private:
    T &operator[](size_t x) {
        assert(x < this->a.size());
        return a[x];
    }
    constexpr T &operator[](size_t x) const {
        assert(x < this->a.size());
        return a[x];
    }
    P convolution_brute(P &a, P &b) const {
        size_t n = a.size(), m = b.size();
        P ans(n + m - 1);
        if (n < m)
            for (size_t j = 0; j < m; ++j)
                for (size_t i = 0; i < n; ++i) ans[i + j] += a[i] * b[j];
        else
            for (size_t i = 0; i < n; ++i)
                for (size_t j = 0; j < m; ++j) ans[i + j] += a[i] * b[j];
        return ans;
    }

    P &convolution_inplace(P b) {
        if (this->size() == 0 || b.size() == 0) {
            this->a.clear();
            return *this;
        }
        return *this = convolution_brute(*this, b);
    }
};

// 128 BIT INTEGER I/O METHODS
using i128 = __int128_t;
string toString(i128 value) {
    string output;
    while (output.empty() || value > 0) {
        output = (char)(value % 10 + '0') + output;
        value /= 10;
    }
    return output;
}
istream &operator>>(istream &stream, i128 &v) {
    string s;
    stream >> s;
    bool neg = false;
    if (s[0] == '-') {
        s.erase(0, 1);
        neg = true;
    }
    int len = s.length();
    v = 0;
    for (int32_t i = 0; i < len; ++i) v = v * 10 + s[i] - '0';
    if (neg) v = -v;
    return stream;
}
ostream &operator<<(ostream &stream, i128 v) {
    if (v == 0)
        stream << "0";
    else {
        if (v < 0) {
            stream << "-";
            v = -v;
        }
        stream << toString(v);
    }
    return stream;
}

// INTEGER TYPE USED IN POLYNOMIAL COEFFICIENTS SET HERE:
using Tp = Mint<998244353>;
// using  Tp = __int128_t;
// using  Tp = int64_t;
// using  Tp = Biggy;
using Poly = Polynomial<Tp>;

using i32 = int32_t;
using i64 = int64_t;
using u32 = uint32_t;
using u64 = uint64_t;
using pr = pair<u32, u32>;

// REQUIRE A HASH FOR AN UNORDERED MAP TO MEMOIZE GRAPH/POLYNOMIAL PAIRS
struct VectorHasher {
    size_t operator()(const vector<u32>& vec) const {
        size_t seed = vec.size();
        for (uint32_t i : vec)
            seed ^= hash<u32>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};
unordered_map<vector<u32>, Poly, VectorHasher> memo;

vector<Poly> clique;

class Graph {
   public:
    vector<u32> g;

    // CONSTRUCTORS
    Graph(){};
    Graph(const vector<u32> &v) { g = v; }

    Graph contractEdge(const pr &edge) {
        Graph G(*this);
        const auto [u, v] = edge;
        G.g[u] |= G.g[v];
        for (auto &e : G.g) e |= (e >> v & 1) << u;
        G.g[u] &= ~(u32(1) << u);
        for (auto &e : G.g)
            e = (e & ~(~u32(0) << v)) | (e & (~u32(0) << (v + 1))) >> 1;
        G.g.erase(G.g.begin() + v);
        return G;
    }

    bool findBridge(pr &edge) {
        const u32 n = g.size();
        u32 visited = u32(0);
        edge = {0, 0};
        vector<u32> tin(n, 0), low(n, 0);
        u32 best = 0, timer = 0;

        for (u32 i = 0; i < n; ++i)
            if (!(visited >> i & 1))
                bridgeDFS(tin, low, visited, timer, best, edge, timer, i);
        return (edge != pr({0, 0})) ? true : false;
    }

    bool isComplete() {  // ASSUMES CONNECTED
        const int n = (int)g.size() - 1;
        for (auto e : g)
            if (__builtin_popcount(e) != n) return false;
        return true;
    }

    bool isLoop() {  // ASSUMES G IS CONNECTED AND NOT CHORDAL
        if (g.size() < 4) return false;
        for (const auto &e : g)
            if (__builtin_popcount(e) != 2) return false;
        return true;
    }

    Graph deleteEdge(const pr &edge) {
        const auto [u, v] = edge;
        Graph K(*this);
        K.g[u] &= ~(u32(1) << v);
        K.g[v] &= ~(u32(1) << u);
        return K;
    }

    Graph addEdge(const pr &edge) {
        const auto [u, v] = edge;
        Graph K(*this);
        K.g[u] |= (u32(1) << v);
        K.g[v] |= (u32(1) << u);
        return K;
    }

    pr chooseEdge() {
        pr edge = {0, 0};
        const u32 n = g.size();
        u32 best = 0;

        for (u32 u = 0; u < n; ++u) {
            auto e = g[u];
            const u32 p = __builtin_popcount(e) / 2;
            // e REPS NBRS OF u WITH INDEX > u
            e &= ~u32(0) << u;

            while (e) {
                const u32 w = __builtin_ctz(e);
                e &= ~(u32(1) << w);

                if (best == 0 || p + __builtin_popcount(g[w]) <= best) {
                    best = p + __builtin_popcount(g[w]);
                    edge = {u, w};
                }
            }
        }
        return edge;
    }

    pr newEdge() {
        const u32 n = g.size();
        pr edge = {0, 0};
        u32 best = 0;

        for (u32 u = 0; u < n; ++u) {
            auto e = g[u];
            const u32 p = __builtin_popcount(e);
            // e REPS NON NBRS OF u WITH INDEX > u
            e = ~(e ^ (~u32(0) << n)) & (~u32(0) << (u + 1));

            while (e) {
                const u32 w = __builtin_ctz(e);
                e &= ~(u32(1) << w);

                if (best == 0 || p + __builtin_popcount(g[w]) > best) {
                    best = p + __builtin_popcount(g[w]);
                    edge = {u, w};
                }
            }
        }
        return edge;
    }

    int countEdges2x() {
        int ret = 0;
        for (const auto &e : g) ret += __builtin_popcount(e);
        return ret;
    }

    double density() {
        return double(countEdges2x()) / double(g.size() * (g.size() - 1));
    }

    Graph extractNodes(const vector<u32> &nodes) {
        const size_t n = nodes.size();
        Graph G(*this);
        for (size_t i = 0; i < n; ++i) G.g[i] = G.g[nodes[i]];
        G.g.resize(n);

        for (auto &e : G.g) {
            for (size_t j = 0; j < n; ++j)
                e = (e & ~(u32(1) << j)) | (e >> nodes[j] & 1) << j;
            e &= (u32(1) << n) - 1;
        }
        return G;
    }

    // FIND ORDERING USED TO TEST FOR CHORDAL GRAPH
    vector<u32> maximumCardinalitySearch() {
        const i32 n = g.size();
        vector<u32> ordering(n);
        vector<i32> lp(2 * n + 1), idx(n, 0);

        iota(lp.begin(), lp.end(), 0);
        vector<i32> rp(lp);

        auto Insert = [&](i32 i, i32 j) {
            rp[lp[j]] = i;
            lp[i] = lp[j];
            lp[j] = i;
            rp[i] = j;
        };
        auto Erase = [&](i32 i) {
            rp[lp[i]] = rp[i];
            lp[rp[i]] = lp[i];
        };

        for (i32 i = 0; i < n; ++i) Insert(i, n);

        for (i32 i = 0, d = n + 1; i < n; ++i, ++d) {
            while (lp[d] == d) --d;
            const i32 v = lp[d];
            auto e = g[v];
            idx[v] = -1;
            Erase(v);

            while (e) {
                const u32 w = __builtin_ctz(e);
                e &= ~(u32(1) << w);
                if (idx[w] >= 0) {
                    Erase(w);
                    Insert(w, n + (++idx[w]));
                }
            }
            ordering[i] = v;
        }
        return ordering;
    }

    Poly checkIfChordal() {
        const vector<u32> ordering = maximumCardinalitySearch();
        u32 visited = 0;
        vector<u32> tally(g.size(), 0);

        for (const u32 u : ordering) {
            u32 neighbors = g[u] & visited;
            u32 counter = 0;

            while (neighbors) {
                const u32 v = __builtin_ctz(neighbors);
                neighbors &= ~(u32(1) << v);
                if ((g[v] & neighbors) != neighbors) return Poly();
                ++counter;
            }
            visited |= u32(1) << u;
            ++tally[counter];
        }

        // G IS CHORDAL
        Poly chromPoly = {1};
        for (i32 i = 0; i < (i32)g.size(); ++i)
            if (tally[i]) chromPoly *= Poly({-i, 1}).pow(tally[i]);
        return chromPoly;
    }

    // FIND CONNECTED COMPONENTS OF G
    vector<vector<u32>> components() {
        const u32 n = g.size();
        u32 visited = u32(0);
        vector<vector<u32>> comps;

        for (u32 i = 0; i < n; ++i) {
            if (visited >> i & 1) continue;
            stack<u32> stk;
            stk.push(i);
            visited |= u32(1) << i;
            vector<u32> component;

            while (stk.size()) {
                const u32 u = stk.top();
                u32 e = g[u];
                stk.pop();
                component.push_back(u);

                while (e) {
                    const u32 w = __builtin_ctz(e);
                    e &= ~(u32(1) << w);
                    if (!(visited >> w & 1)) {
                        visited |= u32(1) << w;
                        stk.push(w);
                    }
                }
            }
            sort(component.begin(), component.end());
            comps.push_back(component);
        }
        return comps;
    }

    Poly chromaticPolynomial() {
        // CHECK FOR SELF LOOPS
        for (size_t i = 0; i < g.size(); ++i)
            if (g[i] & (u32(1) << i)) return Poly(g.size() + 1, 0);

        // FIND CONNECTED COMPONENTS OF G
        vector<vector<u32>> comps = components();

        // PROCESS EACH CONNECTED COMPONENT SEPARATELY
        if (comps.size() > 1) {
            Poly cp = {1};
            for (auto &cmp : comps) {
                if (cmp.size() == 0)
                    cp *= {1};
                else if (cmp.size() == 1)
                    cp *= {0, 1};
                else if (cmp.size() == 2)
                    cp *= {0, -1, 1};
                else
                    cp *= extractNodes(cmp).chromaticPolyConnected();
            }
            return cp;
        }
        return chromaticPolyConnected();
    }

   private:
    void bridgeDFS(vector<u32> &tin, vector<u32> &low, u32 &visited, u32 &timer,
                   u32 &best, pr &edge, u32 u, i32 p = -1) {
        u32 e = g[u];
        const u32 pcu = __builtin_popcount(e);
        bool parentSkipped = false;
        tin[u] = low[u] = timer++;
        visited |= (u32(1) << u);

        while (e) {
            const i32 w = __builtin_ctz(e);
            e &= ~(u32(1) << w);

            if (w == p && !parentSkipped) {
                parentSkipped = true;
                continue;
            }
            if (visited >> w & 1) {
                low[u] = min(low[u], tin[w]);
            } else {
                bridgeDFS(tin, low, visited, timer, best, edge, w, u);
                low[u] = min(low[u], low[w]);
                if (low[w] > tin[u] &&
                    pcu + __builtin_popcount(g[w]) > best) {
                    best = pcu + __builtin_popcount(g[w]);
                    edge = {u, w};
                }
            }
        }
    }

    Poly chromaticPolyConnected(bool checkBridges = true) {
        // G IS ASSUMED CONNECTED.  CONNECTIVITY IS MAINTAINED.
        const i32 n = g.size();
        if (n == 0) return {1};
        if (n == 1) return {0, 1};
        if (n == 2) return {0, -1, 1};
        if (n == 3) {
            if (isComplete())
                return {0, 2, -3, 1};
            else
                return {0, 1, -2, 1};
        }

        if (memo.find(g) != memo.end()) return memo[g];

        // CHECK FOR COMPLETE GRAPH
        if (isComplete()) return memo[g] = clique[n - 1];

        // CHECK FOR BRIDGES
        if (checkBridges) {
            pr edge;
            if (findBridge(edge))
                return memo[g] = contractEdge(edge).chromaticPolyConnected() *
                                 Poly({-1, 1});
        }

        // CHECK IF G IS CHORDAL
        Poly chordalPoly = checkIfChordal();
        if (chordalPoly.size()) return memo[g] = chordalPoly;

        // CHECK IF LOOP
        if (isLoop()) {
            Poly d({-1, 1});
            Poly chromPoly = d.pow(n);
            if (g.size() & 1)
                chromPoly -= d;
            else
                chromPoly += d;
            return memo[g] = chromPoly;
        }

        // NO JOY.  PERFORM DELETION/CONTRACTION.
        if (density() < .59) {
            // LOW DENSITY
            pr edge = chooseEdge();
            return memo[g] = deleteEdge(edge).chromaticPolyConnected() -
                             contractEdge(edge).chromaticPolyConnected();
        }

        // IF HIGH DENSITY
        pr edge = newEdge();
        Graph K = addEdge(edge);
        return memo[g] = K.chromaticPolyConnected(false) +
                         K.contractEdge(edge).chromaticPolyConnected();
    }
};

vector<u32> readData() {
    // REQUIRES NODES TO BE LABELED  0 through ( n - 1 )
    u32 n, m;
    cin >> n >> m;

    vector<u32> G(n, 0);
    vector<pr> edges;

    for (u32 i = 0, u, v; i < m; ++i) {
        cin >> u >> v;
        edges.emplace_back(u, v);
        G[u] |= u32(1) << v;
        G[v] |= u32(1) << u;
    }

    // RELABEL NODES, SORTING BY DEGREE
    vector<pr> counts(n);
    vector<u32> revIndex(n);
    for (u32 i = 0; i < n; ++i)
        counts[i] = make_pair(__builtin_popcount(G[i]), i);
    sort(counts.begin(), counts.end(),
         [](pr &a, pr &b) { return a.first > b.first; });
    for (u32 i = 0; i < n; ++i) revIndex[counts[i].second] = i;

    vector<u32> H(n, u32(0));

    for (auto &x : edges) {
        H[revIndex[x.first]] |= u32(1) << revIndex[x.second];
        H[revIndex[x.second]] |= u32(1) << revIndex[x.first];
    }

    return H;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    memo.reserve(1000);
    memo.max_load_factor(0.7);

    auto dat = readData();
    Graph G(dat);

    // PRECOMPUTE VALUES FOR CLIQUES
    int sz = dat.size();
    clique.resize(sz);
    Poly poly = {1};
    for (i32 i = 0; i < sz; ++i) {
        poly *= Poly({-i, 1});
        clique[i] = poly;
    }

    cout << G.chromaticPolynomial() << '\n';

    return 0;
}
