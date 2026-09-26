#pragma GCC optimize("O3")
// #pragma GCC target("avx,avx2,fma")
#include "bits/stdc++.h"
#define vec vector
#define pb push_back
#define pll pair<ll, ll>
#define pii pair<int, int>
#define all(m) m.begin(), m.end()
#define rall(m) m.rbegin(), m.rend()
#define uid uniform_int_distribution
#define timeStamp() std::chrono::steady_clock::now()
#define unify(m) sort(all(m)), m.erase(unique(all(m)), m.end());
#define duration_milli(a) chrono::duration_cast<chrono::milliseconds>(a).count()
#define fast cin.tie(0), cout.tie(0), cin.sync_with_stdio(0), cout.sync_with_stdio(0);
using namespace std;
using str = string;
using ll = long long;
using ld = long double;
mt19937 rnd(timeStamp().time_since_epoch().count());
template<typename T, typename U> bool chmin(T& a, const U& b) {return (T)b < a ? a = b, 1 : 0;}
template<typename T, typename U> bool chmax(T& a, const U& b) {return (T)b > a ? a = b, 1 : 0;}
struct custom_hash {static uint64_t xs(uint64_t x) {x += 0x9e3779b97f4a7c15; x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9; x = (x ^ (x >> 27)) * 0x94d049bb133111eb; return x ^ (x >> 31);} template<typename T> size_t operator()(T x) const {static const uint64_t C = timeStamp().time_since_epoch().count(); return xs(hash<T> {}(x) + C);}};
template<typename K> using uset = unordered_set<K, custom_hash>;
template<typename K, typename V> using umap = unordered_map<K, V, custom_hash>;
template<typename T1, typename T2> ostream& operator<<(ostream& out, const pair<T1, T2>& x) {return out << x.first << ' ' << x.second;}
template<typename T1, typename T2> istream& operator>>(istream& in, pair<T1, T2>& x) {return in >> x.first >> x.second;}
template<typename T, size_t N> istream& operator>>(istream& in, array<T, N>& a) {for (auto &x : a) in >> x; return in;}
template<typename T, size_t N> ostream& operator<<(ostream& out, const array<T, N>& a) {for (size_t i = 0; i < a.size(); ++i) {out << a[i];if (i + 1 < a.size()) out << ' ';}return out;}
template<typename T> istream& operator>>(istream& in, vector<T>& a) {for (auto& x : a) in >> x; return in;}
template<typename T> ostream& operator<<(ostream& out, const vector<T>& a) {for (size_t i = 0; i < a.size(); ++i) {out << a[i]; if (i + 1 < a.size()) out << ' ';} return out;}

template<typename K, typename V, const int LG>
class hashmap_open_addressing {

    const size_t N = 1 << LG;
    size_t size_ = 0;
    vector<pair<K, V>> store = vector<pair<K, V>>(N);
    vector<char> is_occupied = vector<char>(N);

    constexpr inline void next_pos(int& pos) const {
        pos = pos < N - 1 ? pos + 1 : 0;
    }

    static constexpr uint64_t kek = 11995408973635179863ull;
    constexpr inline int hsh(const K& key) const {
        if constexpr(is_integral_v<K>) {
            //return key & (N - 1);
            return (key * kek) >> (64 - LG);
        } else {
            return hash<K>{}(key) % N;
        }
    }

public:
    hashmap_open_addressing() = default;

    bool empty() const {return size_ == 0;}
    size_t size() const {return size_;}
    size_t capacity() const {return N;}
    size_t count(const K& key) const {return contains(key);}

    void clear() {
        size_ = 0;
        fill(is_occupied.begin(), is_occupied.end(), false);
    }

    V& operator[](const K& key) {
        int pos = hsh(key);
        while (is_occupied[pos]) {
            if (store[pos].first == key) return store[pos].second;
            next_pos(pos);
        }
        ++size_;
        store[pos] = {key, V()};
        is_occupied[pos] = true;
        return store[pos].second;
    }

    bool contains(const K& key) const {
        int pos = hsh(key);
        while (is_occupied[pos]) {
            if (store[pos].first == key) return true;
            next_pos(pos);
        }
        return false;
    }

    void insert(const K& key, const V& val) {
        int pos = hsh(key);
        while (is_occupied[pos]) {
            if (store[pos].first == key) {
                store[pos].second = val;
                return;
            }
            next_pos(pos);
        }
        ++size_;
        store[pos] = {key, val};
        is_occupied[pos] = true;
    }
};

hashmap_open_addressing<int32_t, int32_t, 18> mp;
//Implementation of Shanks'es baby-step giant-step algorithm for solving equation a ^ x = b (mod m)
//Returns minimal x if it exists, -1 otherwise
//Complexity: <O(sqrt(m)), O(sqrt(m))>
template<typename T, typename T2>
T discrete_log_bsgs(T a, T b, T m) {
    static_assert(is_integral_v<T> && is_signed_v<T>);
    static_assert(is_integral_v<T2> && is_signed_v<T2>);
    static_assert(sizeof(T) * 2 == sizeof(T2));
    auto mulmod = [&](T x, T y) {return T2(x) * y % m;};
    assert(0 <= a && a < m);
    assert(0 <= b && b < m);
    T k = 1, add = 0, g;
    while ((g = gcd(a, m)) > 1) {
        if (b == k) return add;
        if (b % g) return -1;
        b /= g, m /= g, ++add;
        k = mulmod(k, a / g);
    }
    T n = sqrtl(m) + 1, an = 1;
    for (T k = n, aa = a; k; k >>= 1, aa = mulmod(aa, aa)) if (k & 1) an = mulmod(an, aa);

    // unordered_map<T, T> mp(n * 2);
    mp.clear();
    for (T q = 0, cur = b; q <= n; ++q, cur = mulmod(cur, a)) mp[cur] = q;
    for (T p = 1, cur = k; p <= n; ++p) {
        cur = mulmod(cur, an);
        if (mp.count(cur)) return n * p - mp[cur] + add;
    }
    return -1;
}

int main() {
    fast;
    int z;
    cin >> z;
    for (; z--;) {
        int x, y, m;
        cin >> x >> y >> m;
        auto v = discrete_log_bsgs<int32_t, int64_t>(x, y, m);
        cout << v << '\n';
    }
}
