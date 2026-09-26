#pragma GCC optimize("Ofast")


#include "bits/stdc++.h"

#define F first
#define S second
#define vec vector
#define pb push_back
#define pll pair<ll, ll>
#define all(m) m.begin(), m.end()
#define rall(m) m.rbegin(), m.rend()
#define uid uniform_int_distribution
#define timeStamp() std::chrono::steady_clock::now()
#define unify(m) sort(all(m)), m.erase(unique(all(m)), m.end());
#define duration_micro(a) chrono::duration_cast<chrono::microseconds>(a).count()
#define duration_milli(a) chrono::duration_cast<chrono::milliseconds>(a).count()
#define fast cin.tie(0), cout.tie(0), cin.sync_with_stdio(0), cout.sync_with_stdio(0);
using namespace std;
using str = string;
using ll = long long;
using ld = long double;
mt19937 rnd(timeStamp().time_since_epoch().count());
mt19937_64 rndll(timeStamp().time_since_epoch().count());
template<typename T, typename U> bool chmin(T& a, const U& b) {return (T)b < a ? a = b, 1 : 0;}
template<typename T, typename U> bool chmax(T& a, const U& b) {return (T)b > a ? a = b, 1 : 0;}

static const auto IOSetup = [] {
    std::cin.tie(nullptr)->sync_with_stdio(false);
    // std::cout << std::setprecision(6) << std::fixed;
    return nullptr;}();
struct IOPre {static constexpr int TEN = 10, SZ = TEN * TEN * TEN * TEN;std::array<char, 4 * SZ> num;constexpr IOPre() : num{} {for (int i = 0; i < SZ; i++) {int n = i;for (int j = 3; j >= 0; j--) {num[i * 4 + j] = static_cast<char>(n % TEN + '0');n /= TEN;}}}};
struct IO {
#if !HAVE_DECL_FREAD_UNLOCKED
#define fread_unlocked fread
#endif
#if !HAVE_DECL_FWRITE_UNLOCKED
#define fwrite_unlocked fwrite
#endif
    static constexpr int SZ = 1 << 17, LEN = 32, TEN = 10, HUNDRED = TEN * TEN,THOUSAND = HUNDRED * TEN, TENTHOUSAND = THOUSAND * TEN,MAGIC_MULTIPLY = 205, MAGIC_SHIFT = 11, MASK = 15,TWELVE = 12, SIXTEEN = 16;static constexpr IOPre io_pre = {};std::array<char, SZ> input_buffer, output_buffer;int input_ptr_left, input_ptr_right, output_ptr_right;
    IO(): input_buffer{},output_buffer{},input_ptr_left{},input_ptr_right{},output_ptr_right{} {}
    IO(const IO&) = delete;IO(IO&&) = delete;IO& operator=(const IO&) = delete;IO& operator=(IO&&) = delete;
    ~IO() { flush(); }
    template <class T>struct is_char {static constexpr bool value = std::is_same_v<T, char>;};
    template <class T>struct is_bool {static constexpr bool value = std::is_same_v<T, bool>;};
    template <class T>struct is_string {static constexpr bool value =std::is_same_v<T, std::string> || std::is_same_v<T, const char*> ||std::is_same_v<T, char*> || std::is_same_v<std::decay_t<T>, char*>;;};
    template <class T, class D = void>struct is_custom {static constexpr bool value = false;};
    template <class T>struct is_custom<T, std::void_t<typename T::internal_value_type>> {static constexpr bool value = true;};
    template <class T>struct is_default {static constexpr bool value = is_char<T>::value || is_bool<T>::value ||is_string<T>::value ||std::is_integral_v<T>;};
    template <class T, class D = void>struct is_iterable {static constexpr bool value = false;};
    template <class T>struct is_iterable <T, typename std::void_t<decltype(std::begin(std::declval<T>())) >> {static constexpr bool value = true;};
    template <class T, class D = void, class E = void>struct is_applyable {static constexpr bool value = false;};
    template <class T>struct is_applyable<T, std::void_t<typename std::tuple_size<T>::type>,std::void_t<decltype(std::get<0>(std::declval<T>()))>> {static constexpr bool value = true;};
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

template<class T>
class directed_mst {
    struct dsu {
        vector<int> p;
        dsu(int n): p(n, -1) {}
        int find(int i) {return p[i] < 0 ? i : p[i] = find(p[i]);}
        bool same(int i, int j) {return find(i) == find(j);}
        int size(int i) { return -p[find(i)];}
        bool join(int i, int j) {
            i = find(i), j = find(j);
            if (i == j) return false;
            if (p[i] > p[j]) swap(i, j);
            p[i] += p[j], p[j] = i;
            return true;
        }
    };

    struct rollback_dsu {
        vector<int> p;
        vector<pair<int, int>> joins;

        rollback_dsu(int n): p(n, -1) {}
        int find(int i) const {return p[i] < 0 ? i : find(p[i]);}
        bool same(int i, int j) const {return find(i) == find(j);}
        int size(int i) const {return -p[find(i)];}
        int time() const {return joins.size();}

        bool join(int i, int j) {
            i = find(i), j = find(j);
            if (i == j) return false;
            if (p[i] > p[j]) swap(i, j);
            joins.emplace_back(j, p[j]);
            p[i] += p[j], p[j] = i;
            return true;
        }

        void rollback(int t) {
            while (time() > t) {
                auto [i, pi] = joins.back(); joins.pop_back();
                assert(p[p[i]] < 0);
                p[p[i]] -= pi;
                p[i] = pi;
            }
        }
    };

    struct Node {int l, r, from, to; T weight, lz;};
    size_t n;
    vector<Node> nodes;
    vector<int> heap;

    void apply(int i, T upd) {nodes[i].weight -= upd; nodes[i].lz += upd;}
    void push(int i) {
        if (nodes[i].l != -1) apply(nodes[i].l, nodes[i].lz);
        if (nodes[i].r != -1) apply(nodes[i].r, nodes[i].lz);
        nodes[i].lz = 0;
    }
    int merge(int u, int v) {
        if (u == -1 || v == -1) return (u == -1 ? v : u);
        if (nodes[v].weight < nodes[u].weight) swap(u, v);
        push(u);
        nodes[u].r = merge(nodes[u].r, v);
        swap(nodes[u].l, nodes[u].r);
        return u;
    }
    void pop(int v) {
        push(heap[v]);
        heap[v] = merge(nodes[heap[v]].l, nodes[heap[v]].r);
    }

public:
    directed_mst(size_t n, size_t k = 0): n(n), heap(n, -1) {nodes.reserve(k);}

    void add_edge(size_t from, size_t to, T weight) {
        assert(from < n && to < n);
        nodes.push_back({ -1, -1, int(from), int(to), weight, T(0)});
        heap[to] = merge(heap[to], nodes.size() - 1);
    }

    //Returns {min ordered mst weight, vector of parents for nodes in mst}
    //If not all vertexes are reachable from root, returns {0, {}}
    template<typename C = T>
    pair<C, vector<int>> solve(size_t root) {
        assert(root < n);
        C ans = 0;
        vector edge(heap.size(), -1);
        vector<pair<int, int>> cycles;
        dsu dsu_cyc(heap.size());
        rollback_dsu dsu_contract(heap.size());
        for (size_t i = 0; i < n; ++i) {
            if (i == root) continue;
            int v = i;
            while (true) {
                if (heap[v] == -1) return {T(0), {}};
                edge[v] = heap[v];
                ans += nodes[edge[v]].weight;
                apply(edge[v], nodes[edge[v]].weight);
                if (dsu_cyc.join(v, dsu_contract.find(nodes[edge[v]].from))) {
                    break;
                }

                int vnext = dsu_contract.find(nodes[edge[v]].from), t = dsu_contract.time();
                while (dsu_contract.join(v, vnext)) {
                    heap[dsu_contract.find(v)] = merge(heap[v], heap[vnext]);
                    v = dsu_contract.find(v);
                    vnext = dsu_contract.find(nodes[edge[vnext]].from);
                }
                cycles.emplace_back(edge[v], t);

                while (heap[v] != -1 && dsu_contract.same(nodes[heap[v]].from, v)) {
                    pop(v);
                }
            }
        }

        for (auto it = rbegin(cycles); it != rend(cycles); ++it) {
            int vrepr = dsu_contract.find(nodes[it->first].to);
            dsu_contract.rollback(it->second);
            int vinc = dsu_contract.find(nodes[edge[vrepr]].to);
            edge[vinc] = exchange(edge[vrepr], it->first);
        }

        for (size_t i = 0; i < n; ++i) {
            edge[i] = i == root ? -1 : nodes[edge[i]].from;
        }
        return {ans, edge};
    }
};

int main() {
    fast;
    int a, k, st;
    cin >> a >> k >> st;
    directed_mst<int> g(a, k);
    for (int i = 0; i < k; i++) {
        int u, v, w; cin >> u >> v >> w;
        g.add_edge(u, v, w);
    }
    auto [cost, edges] = g.solve<ll>(st);
    cout << cost << '\n';
    for (int q = 0; q < a; ++q) {
        cout << (q == st ? q : edges[q]) << ' ';
    }
}
