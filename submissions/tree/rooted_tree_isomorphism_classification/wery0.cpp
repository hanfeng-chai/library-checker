#pragma GCC optimize("Ofast")
// #pragma GCC target("avx,avx2,fma")

#include "bits/stdc++.h"

//#define NDEBUG
#define F first
#define S second
#define vec vector
#define pb push_back
#define pll pair<ll, ll>
#define pdd pair<ld, ld>
#define pii pair<int, int>
#define pnn pair<Node*, Node*>
#define all(m) m.begin(), m.end()
#define rall(m) m.rbegin(), m.rend()
#define uid uniform_int_distribution
#define timeStamp() std::chrono::steady_clock::now()
#define unify(m) sort(all(m)); m.erase(unique(all(m)), m.end());
#define duration_micro(a) chrono::duration_cast<chrono::microseconds>(a).count()
#define duration_milli(a) chrono::duration_cast<chrono::milliseconds>(a).count()
#define fast cin.tie(0); cout.tie(0); cin.sync_with_stdio(0); cout.sync_with_stdio(0);
using namespace std;
using str = string;
using ll = long long;
using ld = long double;
using uint = unsigned int;
using ull = unsigned long long;
mt19937 rnd(timeStamp().time_since_epoch().count());
mt19937_64 rndll(timeStamp().time_since_epoch().count());
template<typename T, typename U> bool chmin(T& a, const U& b) {return b < a ? a = b, 1 : 0;}
template<typename T, typename U> bool chmax(T& a, const U& b) {return b > a ? a = b, 1 : 0;}
constexpr inline uint leq_pow2(const uint x) {return 1u << __lg(x);}
constexpr inline ull leq_pow2ll(const ull x) {return 1ull << __lg(x);}
constexpr inline uint geq_pow2(const uint x) {return x & (x - 1) ? 2u << __lg(x) : x;}
constexpr inline ull geq_pow2ll(const ull x) {return x & (x - 1) ? 2ull << __lg(x) : x;}
constexpr inline ll sqd(const pll p1, const pll p2) {return (p1.F - p2.F) * (p1.F - p2.F) + (p1.S - p2.S) * (p1.S - p2.S);}
constexpr inline ll sqd(const ll x1, const ll y1, const ll x2, const ll y2) {return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);}
struct custom_hash {static uint64_t xs(uint64_t x) {x += 0x9e3779b97f4a7c15; x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9; x = (x ^ (x >> 27)) * 0x94d049bb133111eb; return x ^ (x >> 31);} template<typename T> size_t operator()(T x) const {static const uint64_t C = timeStamp().time_since_epoch().count(); return xs(hash<T> {}(x) + C);}};
template<typename K> using uset = unordered_set<K, custom_hash>;
template<typename K> using umset = unordered_multiset<K, custom_hash>;
template<typename K, typename V> using umap = unordered_map<K, V, custom_hash>;
template<typename T1, typename T2> ostream& operator<<(ostream& out, const pair<T1, T2>& x) {return out << x.F << ' ' << x.S;}
template<typename T1, typename T2> istream& operator>>(istream& in, pair<T1, T2>& x) {return in >> x.F >> x.S;}
template<typename T> istream& operator>>(istream& in, vector<T>& a) {for (auto &x : a) in >> x; return in;};
template<typename T> ostream& operator<<(ostream& out, vector<T>& a) {for (int q = 0; q < a.size(); ++q) {out << a[q]; if (q + 1 < a.size()) out << ' ';} return out;};
template<typename T, auto K> istream& operator>>(istream& in, array<T, K>& a) {for (auto &x : a) in >> x; return in;};
template<typename T, auto K> ostream& operator<<(ostream& out, array<T, K>& a) {for (int q = 0; q < a.size(); ++q) {out << a[q]; if (q + 1 < a.size()) out << ' ';} return out;};

template<typename I> auto array_cnt(I f, I l) {umap<typename iterator_traits<I>::value_type, int> mp; while (f != l) ++mp[*f], ++f; return mp;}
template<typename I> auto subset_sum(I f, I l) {int a = l - f; vec<typename iterator_traits<I>::value_type> o(1 << a); for (int q = 1; q < (1 << a); ++q) {const int i = __builtin_ctz(q); o[q] = *(f + i) + o[q ^ (1 << i)];} return o;}
template<typename I> vec<pii> get_segs_of_eq_elems(I first, I last) {using T = typename iterator_traits<I>::value_type; vec<pii> ans; if (first == last) return ans; int l = 0; T prev = *first; int r = 1; for (auto cit = next(first); cit != last; ++cit, ++r) {if (*cit != prev) {ans.pb({l, r - 1}); l = r;} prev = *cit;} ans.pb({l, r - 1}); return ans;}
template<typename I> int LCP(I f1, I l1, I f2, I l2) {for (int o = 0; ; ++f1, ++f2, ++o) if (f1 == l1 || f2 == l2 || *f1 != *f2) return o; return -1;}
template<typename I> int min_period(I f, I l) {int a = l - f; vec<int> m(a); for (int q = 1; q < a; ++q) {for (int w = m[q - 1]; w && !m[q]; w = m[w - 1]) {if (*(f + q) == *(f + w)) m[q] = w + 1;} m[q] += !m[q] && *(f + q) == *f;} int p = a - m.back(); return a % p ? a : p;}
template<typename I> bool is_palindrome(I f, I l) {for (--l; f < l; ++f, --l) if (*f != *l) return 0; return 1;}
str from_base_10_to_base_b(ll x, ll b) {str t; if (x == 0) t = "0"; for (; x; x /= b) t += (char)('0' + x % b); reverse(all(t)); return t;}
#define vi vec<int>
#define vl vec<ll>
#define vvi vec<vec<int>>
#define vvvi vec<vec<vec<int>>>
#define vvl vec<vec<ll>>
#define vpi vec<pii>
#define vpl vec<pll>
#define vs vec<str>
#define vvs vec<vec<str>>
const int dx[] = { -1, 0, 1, 0, -1, 1, 1, -1};
const int dy[] = {0, 1, 0, -1, 1, 1, -1, -1};
template<typename T_arr> int LCP(T_arr m1, T_arr m2) {return LCP(all(m1), all(m2));}
template<typename T_arr> T_arr subset_sum(T_arr m) {return subset_sum(all(m));}
template<typename T> umap<T, int> array_cnt(vec<T> m) {return array_cnt(all(m));}
template<typename T_arr> vec<pii> get_segs_of_eq_elems(T_arr m) {return get_segs_of_eq_elems(all(m));}
template<typename T> int sum_of_digits(T val) {int o = 0; for (; val; val /= 10) o += val % 10; return o;}
template<typename T> struct static_sum_query {vec<T> m; static_sum_query() {} template<typename I> static_sum_query(I f, I l) {m.resize(l - f + 1); for (auto it = m.begin() + 1; f != l; ++f, ++it) *it = *(it - 1) + *f;} template<typename T_arr> static_sum_query(T_arr m) {(*this) = static_sum_query(all(m));} inline T query(const int l, const int r) const {return m[r + 1] - m[l];}};
template<typename T> vec<pair<T, int>> zip_with_positions(vec<T> &m) {int a = m.size(); vec<pair<T, int>> ans(a); for (int q = 0; q < a; ++q) ans[q] = {m[q], q}; return ans;}
vec<str> split(str &s, char c, bool ignore_empty = false) {vec<str> o; str u; for (int q = 0; q < s.size(); q++) {if (s[q] == c) {if (!u.empty() || !ignore_empty) o.pb(u); u.clear();} else u += s[q];} if (!u.empty() || !ignore_empty) o.pb(u); return o;}
str join(vec<str> &m, char c) {str o; for (str &s : m) o += s + c; if (o.size()) o.pop_back(); return o;}
vec<pii> get_reflection_points_in_rect(int a, int b, int x, int y) {assert(0 <= x && x < a); assert(0 <= y && y < b); vec<pii> res = {{x, y}}; if (x != a - x - 1) res.pb({a - x - 1, y}); if (y != a - y - 1) res.pb({x, a - y - 1}); if (x != a - x - 1 && y != a - y - 1) res.pb({a - x - 1, a - y - 1}); return res;}
vec<pii> get_rotation_points_in_square(int a, int x, int y) {assert(0 <= x && x < a); assert(0 <= y && y < a); vec<pii> res = {{x, y}}; if (a % 2 == 1 && x == a / 2 && y == a / 2) return res; res.pb({a - y - 1, x}); res.pb({a - x - 1, a - y - 1}); res.pb({y, a - x - 1}); return res;}
auto integral_binary_search_left = [](auto f, auto l, auto r) {static_assert(is_same<decltype(l), decltype(r)>::value); while (l + 1 < r) {auto md = l + (r - l) / 2; if (f(md)) l = md; else r = md;} return l;};
auto integral_binary_search_right = [](auto f, auto l, auto r) {static_assert(is_same<decltype(l), decltype(r)>::value); while (l + 1 < r) {auto md = l + (r - l) / 2; if (f(md)) l = md; else r = md;} return r;};
bool is_prime(ll c) {if (c < 2) return 0; if (c == 2 || c == 3) return 1; if (c % 2 == 0 || c % 3 == 0) return 0; const ll gr = sqrtl(c) + 1; for (ll q = 6; q <= gr; q += 6) {if (c % (q - 1) == 0) return 0; if (c % (q + 1) == 0) return 0;} return 1;}
vec<vec<int>> get_cycles_of_perm(vec<int> &m, int index) {int a = m.size(); vec<vec<int>> ans; vec<bool> us(a); for (int q = 0; q < a; ++q) {if (us[q]) continue; vec<int> tyt; for (int w = q; !us[w]; w = m[w] - index) {tyt.pb(w); us[w] = 1;} ans.pb(tyt);} return ans;}
int find_closing_bracket(str &s, int i) {char op = s[i], cl = op == '(' ? ')' : op == '{' ? '}' : op == '[' ? ']' : op == '<' ? '>' : '@'; assert(cl != '@'); int dep = 1; for (int q = i + 1; q < s.size(); ++q) {dep += s[q] == op ? 1 : s[q] == cl ? -1 : 0; if (dep == 0) return q;} return -1;}
template<typename T> vec<pair<T, T>> vv_to_vp(vec<vec<T>> &m) {int a = m.size(); vec<pair<T, T>> ans(a); for (int q = 0; q < a; ++q) ans[q] = {m[q][0], m[q][1]}; return ans;}
template<const int k, typename T> vec<array<T, k>> vv_to_varr(vec<vec<T>> &m) {int a = m.size(); vec<array<T, k>> ans(a); for (int q = 0; q < a; ++q) for (int w = 0; w < k; ++w) ans[q][w] = m[q][w]; return ans;}
template<typename I> bool is_B_subseq_A(I A_beg, I A_end, I B_beg, I B_end) {for (; A_beg != A_end && B_beg != B_end; ++A_beg) if (*A_beg == *B_beg) ++B_beg; return B_beg == B_end;}
template<typename T_arr> int min_period(T_arr m) {return min_period(all(m));}
template<typename T_arr> bool is_palindrome(T_arr m) {return is_palindrome(all(m));}
template<typename T_arr> T_arr reverse(T_arr x) {reverse(all(x)); return x;}
int replace(str &s, str from, str to) {str t = from; t.pb(0); t += s; int a = t.size(); vec<int> m(a); for (int q = 1; q < a; ++q) {for (int w = m[q - 1]; w && !m[q]; w = m[w - 1]) {if (t[q] == t[w]) m[q] = w + 1;} m[q] += !m[q] && t[q] == t[0];} int szf = from.size(), lst = szf; for (int q = szf; q < t.size(); ++q) {if (m[q] == szf && q - lst >= szf) {m[q - szf + 1] = -1; lst = q;}} str ans; int o = 0; for (int q = szf + 1; q < t.size(); ++q) {if (m[q] != -1) ans += t[q]; else ans += to, q += szf - 1, ++o;} s = ans; return o;}
str from_base_10_to_base_b(str x, ll b) {return from_base_10_to_base_b(stoll(x), b);}
ll from_base_b_to_base_10(str x, ll b) {ll o = 0, pw = 1; for (int q = (int)x.size() - 1; q >= 0; --q, pw *= b) o += (x[q] - '0') * pw; return o;}
str from_base_a_to_base_b(str x, ll a, ll b) {ll x10 = from_base_b_to_base_10(x, a); return from_base_10_to_base_b(x10, b);}
template<typename T> T binpow(T x, T k) {if (k < 0) return 0; T o = 1; for (; k; k >>= 1) {if (k & 1) o = o * x; x = x * x;} return o;}
template<typename T> T ar_prog_sum_fcl(T first, T cnt, T last) {return (first + last) * cnt / 2;}
template<typename T> T ar_prog_sum_fdc(T first, T diff, T cnt) {return (first * 2 + diff * (cnt - 1)) * cnt / 2;}
template<typename T> T ar_prog_sum_fdl(T first, T diff, T last) {return (first + last) * ((last - first) / diff + 1) / 2;}
template<typename T> T geom_prog_sum_fdl(T first, T diff, T last) {return (last * diff - first) / (diff - 1);}
template<typename T> T geom_prog_sum_fdc(T first, T diff, T cnt) {return (first * binpow(diff, cnt) - first) / (diff - 1);}
ll lcm(ll x, ll y) {return x / __gcd(x, y) * y;}

template<typename Data>
struct edge {
    int to;
    Data data;

    edge() {}

    edge(int _to) : to(_to) {}
    edge(int _to, Data _data) : to(_to), data(_data) {}

    bool operator<(const edge &other) const {
        return to < other.to;
    }

    bool operator==(const edge &other) const {
        return to == other.to && data == other.data;
    }
};

template<>
struct edge<void> {
    int to;

    edge() {}

    edge(int _to) : to(_to) {}

    bool operator<(const edge &other) const {
        return to < other.to;
    }

    bool operator==(const edge &other) const {
        return to == other.to;
    }
};

template<typename Edge>
struct graph {

    int V = -1, E;
    vec<Edge> store;
    vec<int> fir;
    vec<pair<int, Edge>> acc_edge;
    bool is_prepared = 0;

    graph() = default;
    graph(int V) : V(V) {}
    graph(int V, int E) : V(V) { if (E != -1) acc_edge.reserve(E); }

    void prepare() {
        if (V == -1) {
            int mxn = -1;
            for (const auto &[v, e] : acc_edge) {
                chmax(mxn, v);
                chmax(mxn, e.to);
            }
            V = mxn + 1;
        }
        E = acc_edge.size();
        store.resize(E);
        vec<int> deg_cnt(V + 1);
        for (const auto &p : acc_edge) ++deg_cnt[p.F + 1];
        partial_sum(all(deg_cnt), deg_cnt.begin());
        fir = deg_cnt;
        for (const auto &p : acc_edge) store[deg_cnt[p.F]++] = p.S;
        acc_edge.clear();
        acc_edge.shrink_to_fit();
        is_prepared = 1;
    }

    inline void add_edge(int x, Edge e, const bool is_dir) {
        assert(0 <= x && x < V); assert(0 <= e.to && e.to < V);
        acc_edge.pb({x, e});
        if (!is_dir) {
            swap(x, e.to);
            acc_edge.pb({x, e});
        }
    }

    struct edge_range {
        __gnu_cxx::__normal_iterator<Edge *, vector<Edge>> first, last;
        auto begin() const { return first; }
        auto end() const { return last; }

        int size() const {return last - first;}
        Edge& operator[](const int v) {return *(first + v);}
    };

    edge_range operator[](const int v) {assert(is_prepared); return {store.begin() + fir[v], store.begin() + fir[v + 1]};}
    int size() const {return V;}
    inline int deg(const int v) const {assert(is_prepared); return fir[v + 1] - fir[v];}

    vec<int> get_nums_of_inv_edges() {assert(is_prepared); vec<int> ans(E, -1); vec<bool> us(V); umap<ll, int> mp(E); for (int v = 0; v < V; ++v) {for (int i = fir[v]; i < fir[v + 1]; ++i) {const auto& e = store[i]; ll hs = 1ll * v * V + e.to; ll ihs = 1ll * e.to * V + v; if (mp.count(ihs)) {int ips = mp[ihs]; mp.erase(ihs); ans[i] = ips; ans[ips] = i;} else mp[hs] = i;}} assert(mp.empty()); assert(count(all(ans), -1) == 0); return ans;}
    int count_connected_components() {assert(is_prepared); vec<bool> us(V); auto dfs = [&](auto && dfs, int v) -> void {us[v] = 1; for (const auto &h : (*this)[v]) if (!us[h.to]) dfs(dfs, h.to);}; int ans = 0; for (int q = 0; q < V; ++q) if (!us[q]) dfs(dfs, q), ++ans; return ans;}

    bool is_tree() {return E == V - 1 && is_connected();}
    bool is_connected() {return count_connected_components() == 1;}
    bool is_bipartite() {assert(is_prepared); vec<int> col(V, -1); bool fl = 1; auto dfs = [&](auto && dfs, int v, int c) -> void {col[v] = c; for (const auto &h : (*this)[v]) {if (col[h.to] == -1) dfs(dfs, h.to, c ^ 1); else fl &= col[h.to] != c;}}; for (int q = 0; q < V; ++q) if (col[q] == -1) dfs(dfs, q, 0); return fl;}
    bool has_self_edges() {assert(is_prepared); for (int v = 0; v < V; ++v) {for (const auto &h : (*this)[v]) {if (v == h.to) return true;}} return false;}
    bool has_multiple_edges() {assert(is_prepared); for (int v = 0; v < V; ++v) {uset<int> s(deg(v)); for (const auto &h : (*this)[v]) {if (s.count(h.to)) return true; s.insert(h.to);}} return false;}
    bool is_simple() {return !has_self_edges() && !has_multiple_edges();}
    bool is_functional() {assert(is_prepared); for (int q = 0; q < V; ++q) if (deg(q) > 1) return false; return true;}

    bool is_acyclic_dir() {assert(is_prepared); vec<int> state(V); auto dfs = [&](auto && dfs, int v) -> bool{state[v] = 1; for (const auto &h : (*this)[v]) {if (state[h.to] == 1) return false; if (state[h.to] == 0 && !dfs(dfs, h.to)) return false;} state[v] = 2; return true;}; for (int q = 0; q < V; ++q) {if (state[q] == 0 && !dfs(dfs, q)) return false;} return true;}
    bool is_acyclic_undir() {assert(is_prepared); vec<int> state(V); auto dfs = [&](auto && dfs, int v, int p = -1) -> bool{state[v] = 1; int cnt_par = 0; for (const auto &h : (*this)[v]) {if (h.to == p) {if (++cnt_par == 1) continue;} if (state[h.to] == 1) return false; if (state[h.to] == 0 && !dfs(dfs, h.to, v)) return false;} state[v] = 2; return true;}; for (int q = 0; q < V; ++q) if (state[q] == 0 && !dfs(dfs, q)) return false; return true;}
    vec<int> get_any_dir_cycle_vertex_nums() {assert(is_prepared); assert(!is_acyclic_dir()); vec<int> state(V), ans; auto dfs = [&](auto && dfs, int v) -> int{state[v] = 1; int ret = -1; for (const auto &h : (*this)[v]) {if (state[h.to] == 1) return h.to; int res = dfs(dfs, h.to); if (res == -1) continue; if (res == -2) {ret = -2; break;} ans.pb(h.to); if (v == res) ans.pb(v); return v != res ? res : -2;} state[v] = 2; return ret;}; for (int q = 0; q < V && ans.empty(); ++q) if (state[q] == 0) dfs(dfs, q); assert(!ans.empty()); reverse(all(ans)); return ans;}
    vec<int> get_any_dir_cycle_edge_nums() {assert(is_prepared); assert(!is_acyclic_dir()); vec<int> state(V), ans; auto dfs = [&](auto && dfs, int v) -> int{state[v] = 1; int ret = -1; for (const auto &h : (*this)[v]) {if (state[h.to] == 1) {ans.pb(h.data); return h.to;} int res = dfs(dfs, h.to); if (res == -1) continue; if (res == -2) {ret = -2; break;} ans.pb(h.data); return v != res ? res : -2;} state[v] = 2; return ret;}; for (int q = 0; q < V && ans.empty(); ++q) if (state[q] == 0) dfs(dfs, q); assert(!ans.empty()); reverse(all(ans)); return ans;}
    vec<int> topsort() {assert(is_acyclic_dir()); vec<bool> us(V); vec<int> ans; ans.reserve(V); auto dfs = [&](auto && dfs, int v) -> void {us[v] = 1; for (const auto& e : (*this)[v]) {if (!us[e.to]) dfs(dfs, e.to);} ans.pb(v);}; for (int q = 0; q < V; ++q) {if (!us[q]) dfs(dfs, q);} reverse(all(ans)); return ans;}

    vec<vec<int>> get_connected_components_undir() {assert(is_prepared); vec<vec<int>> ans; vec<bool> us(V); auto dfs = [&](auto && dfs, int v) -> void {us[v] = 1; ans.back().pb(v); for (const auto& e : (*this)[v]) {if (us[e.to]) continue; dfs(dfs, e.to);}}; for (int q = 0; q < V; ++q) {if (us[q]) continue; ans.pb({}); dfs(dfs, q);} return ans;}
    vec<vec<int>> get_vertex_biconnected_components() {assert(is_prepared); vec<bool> us(V); vec<int> tin(V), fup(V), stq; vec<vec<int>> ans; ans.reserve(V); stq.reserve(V); auto dfs = [&](auto && dfs, int v, int p = -1) -> void {static int T = 0; stq.pb(v); us[v] = 1; tin[v] = fup[v] = T++; int chd = 0; for (const auto &h : (*this)[v]) {if (h.to == p) continue; if (!us[h.to]) {++chd; int stop_sz = stq.size(); dfs(dfs, h.to, v); chmin(fup[v], fup[h.to]); if ((p == -1 && chd > 1) || (p != -1 && fup[h.to] >= tin[v])) {ans.pb({v}); ans.back().reserve(stq.size() - stop_sz + 1); for (; stq.size() > stop_sz;) {ans.back().pb(stq.back()); stq.pop_back();}}} else chmin(fup[v], tin[h.to]);}}; for (int q = 0; q < V; ++q) {if (!us[q]) {dfs(dfs, q); ans.pb(stq); stq.clear();}} return ans;}
    vec<int> get_cutpoints() {assert(is_prepared); vec<bool> us(V); vec<int> is_cp(V), tin(V), fup(V); auto dfs = [&](auto && dfs, int v, int p = -1) -> void {static int T = 0; us[v] = 1; tin[v] = fup[v] = T++; int chd = 0; for (const auto &h : (*this)[v]) {if (h.to == p) continue; if (!us[h.to]) {++chd; dfs(dfs, h.to, v); chmin(fup[v], fup[h.to]); is_cp[v] |= p == -1 ? chd > 1 : fup[h.to] >= tin[v];} else chmin(fup[v], tin[h.to]);}}; for (int q = 0; q < V; ++q) if (!us[q]) dfs(dfs, q); vec<int> ans; for (int q = 0; q < V; ++q) if (is_cp[q]) ans.pb(q); return ans;}
    vec<vec<int>> get_edge_biconnected_components() {assert(is_prepared); vec<bool> is_bridge(E), us(V); vec<int> tin(V), fup(V); auto dfs = [&](auto dfs, int v, int p = -1) -> int {static int T = 0; us[v] = 1; tin[v] = fup[v] = T++; int cnt_par = 0, ie = -1; for (int i = fir[v]; i < fir[v + 1]; ++i) {const auto& h = store[i]; if (h.to == p) {if (++cnt_par > 1) {chmin(fup[v], tin[h.to]);} else ie = i;} else if (!us[h.to]) {int inv_edge = dfs(dfs, h.to, v); chmin(fup[v], fup[h.to]); is_bridge[i] = is_bridge[inv_edge] = fup[h.to] > tin[v];} else chmin(fup[v], tin[h.to]);} return ie;}; int cnt_cmp = 0; for (int q = 0; q < V; ++q) if (!us[q]) dfs(dfs, q), ++cnt_cmp; int cnt_bridges = count(all(is_bridge), true) / 2; int cnt_tec_cmp = cnt_cmp + cnt_bridges; vec<vec<int>> ans(cnt_tec_cmp); fill(all(us), false); auto go = [&](auto && go, int v, int nc) -> void {us[v] = 1; assert(nc < cnt_tec_cmp); ans[nc].pb(v); for (int i = fir[v]; i < fir[v + 1]; ++i) {const auto& h = store[i]; if (!us[h.to] && !is_bridge[i]) go(go, h.to, nc);}}; for (int q = 0, nc = 0; q < V; ++q) if (!us[q]) go(go, q, nc++); return ans;}
    vec<int> get_bridges_nums() {assert(is_prepared); vec<bool> is_bridge(E), us(V); vec<int> tin(V), fup(V); auto dfs = [&](auto dfs, int v, int p = -1) -> int {static int T = 0; us[v] = 1; tin[v] = fup[v] = T++; int cnt_par = 0, ie = -1; for (int i = fir[v]; i < fir[v + 1]; ++i) {const auto& h = store[i]; if (h.to == p) {if (++cnt_par > 1) {chmin(fup[v], tin[h.to]);} else ie = i;} else if (!us[h.to]) {int inv_edge = dfs(dfs, h.to, v); chmin(fup[v], fup[h.to]); is_bridge[i] = is_bridge[inv_edge] = fup[h.to] > tin[v];} else chmin(fup[v], tin[h.to]);} return ie;}; for (int q = 0; q < V; ++q) if (!us[q]) dfs(dfs, q); int cnt_bridges = count(all(is_bridge), true) / 2; vec<int> ans(cnt_bridges); for (int q = 0, j = 0; q < V; ++q) {for (int i = fir[q]; i < fir[q + 1]; ++i) {const auto &h = store[i]; if (is_bridge[i] && q < h.to) ans[j++] = h.data;}} return ans;}

    //dst[v] = -1, if no path from cluster
    vec<int> BFS_from_cluster(vec<int> cluster) {assert(is_prepared); vec<int> dst(V, -1); for (int i : cluster) dst[i] = 0; deque<int> dq(all(cluster)); while (dq.size()) {int v = dq.front(); dq.pop_front(); for (const auto &e : (*this)[v]) {if (dst[e.to] == -1) {dst[e.to] = dst[v] + 1; dq.pb(e.to);}}} return dst;}
    vec<int> BFS_from_cluster_on_inverse_graph(vec<int> cluster) {assert(is_prepared); vec<int> dst(V, -1); for (int i : cluster) dst[i] = 0; vec<int> s; for (int q = 0; q < V; ++q) if (dst[q] != 0) s.pb(q); deque<int> dq(all(cluster)); vec<bool> tyt(V); while (dq.size()) {int v = dq.front(); dq.pop_front(); for (const auto &e : (*this)[v]) tyt[e.to] = 1; for (int q = 0; q < s.size(); ++q) {const int h = s[q]; if (!tyt[h]) {dst[h] = dst[v] + 1; dq.pb(h); if (q + 1 < s.size()) swap(s[q], s.back()); s.pop_back(); --q;}} for (const auto &e : (*this)[v]) tyt[e.to] = 0;} return dst;}
};

static const auto IOSetup = [] {
    std::cin.tie(nullptr)->sync_with_stdio(false);
    // std::cout << std::setprecision(6) << std::fixed;
    return nullptr;
}();
struct IOPre {static constexpr int TEN = 10, SZ = TEN * TEN * TEN * TEN; std::array<char, 4 * SZ> num; constexpr IOPre() : num{} {for (int i = 0; i < SZ; i++) {int n = i; for (int j = 3; j >= 0; j--) {num[i * 4 + j] = static_cast<char>(n % TEN + '0'); n /= TEN;}}}};
struct IO {
#if !HAVE_DECL_FREAD_UNLOCKED
#define fread_unlocked fread
#endif
#if !HAVE_DECL_FWRITE_UNLOCKED
#define fwrite_unlocked fwrite
#endif
    static constexpr int SZ = 1 << 17, LEN = 32, TEN = 10, HUNDRED = TEN * TEN, THOUSAND = HUNDRED * TEN, TENTHOUSAND = THOUSAND * TEN, MAGIC_MULTIPLY = 205, MAGIC_SHIFT = 11, MASK = 15, TWELVE = 12, SIXTEEN = 16; static constexpr IOPre io_pre = {}; std::array<char, SZ> input_buffer, output_buffer; int input_ptr_left, input_ptr_right, output_ptr_right;
    IO(): input_buffer{}, output_buffer{}, input_ptr_left{}, input_ptr_right{}, output_ptr_right{} {}
    IO(const IO&) = delete; IO(IO&&) = delete; IO& operator=(const IO&) = delete; IO& operator=(IO&&) = delete;
    ~IO() { flush(); }
    template <class T>struct is_char {static constexpr bool value = std::is_same_v<T, char>;};
    template <class T>struct is_bool {static constexpr bool value = std::is_same_v<T, bool>;};
    template <class T>struct is_string {static constexpr bool value = std::is_same_v<T, std::string> || std::is_same_v<T, const char*> || std::is_same_v<T, char*> || std::is_same_v<std::decay_t<T>, char*>;;};
    template <class T, class D = void>struct is_custom {static constexpr bool value = false;};
    template <class T>struct is_custom<T, std::void_t<typename T::internal_value_type>> {static constexpr bool value = true;};
    template <class T>struct is_default {static constexpr bool value = is_char<T>::value || is_bool<T>::value || is_string<T>::value || std::is_integral_v<T>;};
    template <class T, class D = void>struct is_iterable {static constexpr bool value = false;};
    template <class T>struct is_iterable <T, typename std::void_t<decltype(std::begin(std::declval<T>())) >> {static constexpr bool value = true;};
    template <class T, class D = void, class E = void>struct is_applyable {static constexpr bool value = false;};
    template <class T>struct is_applyable<T, std::void_t<typename std::tuple_size<T>::type>, std::void_t<decltype(std::get<0>(std::declval<T>()))>> {static constexpr bool value = true;};
    template <class T>static constexpr bool needs_newline = (is_iterable<T>::value || is_applyable<T>::value) && (!is_default<T>::value);
    template <typename T, typename U> struct any_needs_newline {static constexpr bool value = false;}; template <typename T>
    struct any_needs_newline<T, std::index_sequence<>> {static constexpr bool value = false;};
    template <typename T, std::size_t I, std::size_t... Is>struct any_needs_newline<T, std::index_sequence<I, Is...>> {static constexpr bool value = needs_newline<decltype(std::get<I>(std::declval<T>()))> || any_needs_newline<T, std::index_sequence<Is...>>::value;};
    inline void load() {memmove(std::begin(input_buffer), std::begin(input_buffer) + input_ptr_left, input_ptr_right - input_ptr_left); input_ptr_right = input_ptr_right - input_ptr_left + static_cast<int>(fread_unlocked(std::begin(input_buffer) + input_ptr_right - input_ptr_left, 1, SZ - input_ptr_right + input_ptr_left, stdin)); input_ptr_left = 0;}
    inline void read_char(char& c) {if (input_ptr_left + LEN > input_ptr_right) load(); c = input_buffer[input_ptr_left++];}
    inline void read_string(std::string& x) {char c; while (read_char(c), c < '!') continue; x = c; while (read_char(c), c >= '!') x += c;}
    template <class T> inline std::enable_if_t<std::is_integral_v<T>, void> read_int(T& x) {if (input_ptr_left + LEN > input_ptr_right) load(); char c = 0; do c = input_buffer[input_ptr_left++]; while (c < '-'); [[maybe_unused]] bool minus = false; if constexpr (std::is_signed<T>::value == true)if (c == '-') minus = true, c = input_buffer[input_ptr_left++]; x = 0; while (c >= '0')x = x * TEN + (c & MASK), c = input_buffer[input_ptr_left++]; if constexpr (std::is_signed<T>::value == true)if (minus) x = -x;}
    inline void skip_space() {if (input_ptr_left + LEN > input_ptr_right) load(); while (input_buffer[input_ptr_left] <= ' ') input_ptr_left++;}
    inline void flush() {fwrite_unlocked(std::begin(output_buffer), 1, output_ptr_right, stdout); output_ptr_right = 0;}
    inline void write_char(char c) {if (output_ptr_right > SZ - LEN) flush(); output_buffer[output_ptr_right++] = c;}
    inline void write_bool(bool b) {if (output_ptr_right > SZ - LEN) flush(); output_buffer[output_ptr_right++] = b ? '1' : '0';}
    inline void write_string(const std::string& s) {for (auto x : s) write_char(x);}
    inline void write_string(const char* s) {while (*s) write_char(*s++);}
    inline void write_string(char* s) {while (*s) write_char(*s++);}
    template <typename T>inline std::enable_if_t<std::is_integral_v<T>, void> write_int(T x) {if (output_ptr_right > SZ - LEN) flush(); if (!x) {output_buffer[output_ptr_right++] = '0'; return;} if constexpr (std::is_signed<T>::value == true)if (x < 0) output_buffer[output_ptr_right++] = '-', x = -x; int i = TWELVE; std::array<char, SIXTEEN> buf{}; while (x >= TENTHOUSAND) {memcpy(std::begin(buf) + i, std::begin(io_pre.num) + (x % TENTHOUSAND) * 4, 4); x /= TENTHOUSAND; i -= 4;} if (x < HUNDRED) {if (x < TEN) {output_buffer[output_ptr_right++] = static_cast<char>('0' + x);} else {std::uint32_t q = (static_cast<std::uint32_t>(x) * MAGIC_MULTIPLY) >> MAGIC_SHIFT; std::uint32_t r = static_cast<std::uint32_t>(x) - q * TEN; output_buffer[output_ptr_right] = static_cast<char>('0' + q); output_buffer[output_ptr_right + 1] = static_cast<char>('0' + r); output_ptr_right += 2;}} else {if (x < THOUSAND) {memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(io_pre.num) + (x << 2) + 1, 3), output_ptr_right += 3;} else {memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(io_pre.num) + (x << 2), 4), output_ptr_right += 4;}} memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(buf) + i + 4, TWELVE - i); output_ptr_right += TWELVE - i;}
    template <typename T_>IO& operator<<(T_&& x) {using T = typename std::remove_cv < typename std::remove_reference<T_>::type >::type; static_assert(is_custom<T>::value or is_default<T>::value or is_iterable<T>::value or is_applyable<T>::value); if constexpr (is_custom<T>::value) {write_int(x.get());} else if constexpr (is_default<T>::value) {if constexpr (is_bool<T>::value) {write_bool(x);} else if constexpr (is_string<T>::value) {write_string(x);} else if constexpr (is_char<T>::value) {write_char(x);} else if constexpr (std::is_integral_v<T>) {write_int(x);}} else if constexpr (is_iterable<T>::value) {using E = decltype(*std::begin(x)); constexpr char sep = needs_newline<E> ? '\n' : ' '; int i = 0; for (const auto& y : x) {if (i++) write_char(sep); operator<<(y);}} else if constexpr (is_applyable<T>::value) {constexpr char sep = (any_needs_newline < T, std::make_index_sequence<std::tuple_size_v<T> >>::value) ? '\n' : ' '; int i = 0; std::apply([this, &sep, &i](auto const & ... y) {(((i++ ? write_char(sep) : void()), this->operator<<(y)), ...);}, x);} return *this;}
    template <typename T>IO& operator>>(T& x) {static_assert(is_custom<T>::value or is_default<T>::value or is_iterable<T>::value or is_applyable<T>::value); static_assert(!is_bool<T>::value); if constexpr (is_custom<T>::value) {typename T::internal_value_type y; read_int(y); x = y;} else if constexpr (is_default<T>::value) {if constexpr (is_string<T>::value) {read_string(x);} else if constexpr (is_char<T>::value) {read_char(x);} else if constexpr (std::is_integral_v<T>) {read_int(x);}} else if constexpr (is_iterable<T>::value) {for (auto& y : x) operator>>(y);} else if constexpr (is_applyable<T>::value) {std::apply([this](auto & ... y) { ((this->operator>>(y)), ...); }, x);} return *this;}
    IO* tie(std::nullptr_t) { return this; }
    void sync_with_stdio(bool) {}
} io;
#define cin io
#define cout io

template<typename K, typename V, const int LG>
struct hash_map_open_addressing {

    const int N = 1 << LG;
    vec<pair<K, V>> store = vec<pair<K, V>>(N);
    vec<bool> is_occupied = vec<bool>(N);

    constexpr inline void next_pos(int &pos) const {
        pos = pos < N - 1 ? pos + 1 : 0;
    }

    static constexpr ull kek = 11995408973635179863ull;
    constexpr inline int hsh(const K& key) const {
        if constexpr(std::is_integral<K>::value) {
            return key & (N - 1);
            //return (key * kek) >> (64 - LG);
        } else {
            return hash<K> {}(key) % N;
        }
    }

    inline void clear() {
        fill(all(is_occupied), false);
    }

    inline V operator[](const K& key) const {
        int pos = hsh(key);
        for (; is_occupied[pos]; next_pos(pos)) {
            if (store[pos].F == key) return store[pos].S;
        }
        return 0;
    }

    inline V cringe(const K& key, V &cnt) {
        int pos = hsh(key);
        for (; is_occupied[pos]; next_pos(pos)) {
            if (store[pos].F == key) return store[pos].S;
        }
        is_occupied[pos] = 1;
        store[pos] = {key, cnt};
        return cnt++;
    }

    inline bool contains(const K& key) const {
        int pos = hsh(key);
        for (; is_occupied[pos]; next_pos(pos)) {
            if (store[pos].F == key) return true;
        }
        return false;
    }

    inline void add(const K& key, const V& val) {
        int pos = hsh(key);
        for (; is_occupied[pos]; next_pos(pos)) {
            if (store[pos].F == key) {
                store[pos].S = val;
                return;
            }
        }
        store[pos] = {key, val};
        is_occupied[pos] = 1;
    }
};

const ull kek = uid<ull>(0, ULLONG_MAX)(rndll);
template<typename I>
ull hash_sequence_2_64(I f, I l, const ll p = 1000000009) {
    ull o = 0, pw = 1;
    for (; f != l; ++f, pw = pw * p) {
        o ^= (*f ^ kek) * pw;
    }
    return o;
}

hash_map_open_addressing<ull, int, 19> ht;

int main() {
    int a; cin >> a;
    graph<edge<void>> t(a);
    for (int q = 1; q < a; ++q) {
        int p; cin >> p;
        t.add_edge(p, q, 1);
    }
    t.prepare();
    vi ans(a);
    int cnt = 1;
    vec<pii> stq = {{0, 0}};
    while (stq.size()) {
        pii p = stq.back();
        int v = p.F;
        if (p.S < t[v].size()) {
            ++stq.back().S;
            stq.pb({t[v][p.S].to, 0});
        } else {
            vi tyt; tyt.reserve(t[v].size());
            for (const auto &e : t[v]) tyt.pb(ans[e.to]);
            sort(all(tyt));
            ull hs = hash_sequence_2_64(all(tyt));
            ans[v] = ht.cringe(hs, cnt) - 1;
            stq.pop_back();
        }
    }
    // auto dfs = [&](auto && dfs, int v) -> void {
    //     for (const auto &e : t[v]) {
    //         dfs(dfs, e.to);
    //     }
    //     vi tyt;
    //     for (const auto &e : t[v]) {
    //         tyt.pb(ans[e.to]);
    //     }
    //     sort(all(tyt));
    //     ull hs = hash_sequence_2_64(all(tyt));
    //     int i = ht[hs];
    //     if (!i) i = cnt++, ht.add(hs, i);
    //     ans[v] = i - 1;
    // };
    // dfs(dfs, 0);
    cout << cnt - 1 << '\n';
    cout << ans;
}