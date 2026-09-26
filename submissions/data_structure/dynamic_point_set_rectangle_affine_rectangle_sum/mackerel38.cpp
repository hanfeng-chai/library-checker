#line 1 "verify/yosupo_dynamic_point_set_rectangle_affine_rectangle_sum.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_point_set_rectangle_affine_rectangle_sum"
#line 2 "math/modint.hpp"

#include <cassert>
#include <iostream>
#include <limits>
#include <type_traits>
#include <vector>

namespace poe {

namespace internal {

constexpr long long safe_mod(long long x, long long m) {
    x %= m;
    if (x < 0) x += m;
    return x;
}

constexpr std::pair<long long, long long> inv_gcd(long long a, long long b) {
    a = safe_mod(a, b);
    if (a == 0) return {b, 0};
    long long s = b, t = a;
    long long m0 = 0, m1 = 1;
    while (t) {
        long long u = s / t;
        s -= t * u;
        m0 -= m1 * u;
        auto tmp = s;
        s = t;
        t = tmp;
        tmp = m0;
        m0 = m1;
        m1 = tmp;
    }
    if (m0 < 0) m0 += b / s;
    return {s, m0};
}

}  // namespace internal

template <int MOD>
class static_modint {
    static_assert(MOD > 0);

public:
    using mint = static_modint;

    static constexpr int mod() { return MOD; }

    constexpr static_modint() : v_(0) {}

    template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
    constexpr static_modint(T v) : v_(static_cast<unsigned int>(internal::safe_mod(static_cast<long long>(v), MOD))) {}

    static constexpr mint raw(int v) {
        mint x;
        x.v_ = static_cast<unsigned int>(v);
        return x;
    }

    constexpr int val() const { return static_cast<int>(v_); }

    constexpr mint operator+() const { return *this; }
    constexpr mint operator-() const { return v_ == 0 ? mint() : raw(MOD - static_cast<int>(v_)); }

    constexpr mint& operator+=(const mint& rhs) {
        v_ += rhs.v_;
        if (v_ >= MOD) v_ -= MOD;
        return *this;
    }

    constexpr mint& operator-=(const mint& rhs) {
        if (v_ < rhs.v_) v_ += MOD;
        v_ -= rhs.v_;
        return *this;
    }

    constexpr mint& operator*=(const mint& rhs) {
        v_ = static_cast<unsigned int>((static_cast<unsigned long long>(v_) * rhs.v_) % MOD);
        return *this;
    }

    constexpr mint& operator/=(const mint& rhs) { return *this *= rhs.inv(); }

    constexpr mint pow(long long n) const {
        assert(n >= 0);
        mint x = *this, r = 1;
        while (n) {
            if (n & 1) r *= x;
            x *= x;
            n >>= 1;
        }
        return r;
    }

    constexpr mint inv() const {
        auto [g, x] = internal::inv_gcd(v_, MOD);
        assert(g == 1);
        return x;
    }

    friend constexpr mint operator+(mint lhs, const mint& rhs) { return lhs += rhs; }
    friend constexpr mint operator-(mint lhs, const mint& rhs) { return lhs -= rhs; }
    friend constexpr mint operator*(mint lhs, const mint& rhs) { return lhs *= rhs; }
    friend constexpr mint operator/(mint lhs, const mint& rhs) { return lhs /= rhs; }
    friend constexpr bool operator==(const mint& lhs, const mint& rhs) { return lhs.v_ == rhs.v_; }
    friend constexpr bool operator!=(const mint& lhs, const mint& rhs) { return lhs.v_ != rhs.v_; }
    friend std::istream& operator>>(std::istream& is, mint& x) {
        long long v;
        is >> v;
        x = mint(v);
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const mint& x) { return os << x.val(); }

private:
    unsigned int v_;
};

template <int ID>
class dynamic_modint {
public:
    using mint = dynamic_modint;

    static int mod() { return mod_ref(); }
    static void set_mod(int m) {
        assert(m > 0);
        mod_ref() = m;
    }

    dynamic_modint() : v_(0) {}

    template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
    dynamic_modint(T v) : v_(static_cast<unsigned int>(internal::safe_mod(static_cast<long long>(v), mod()))) {}

    static mint raw(int v) {
        mint x;
        x.v_ = static_cast<unsigned int>(v);
        return x;
    }

    int val() const { return static_cast<int>(v_); }

    mint operator+() const { return *this; }
    mint operator-() const { return v_ == 0 ? mint() : raw(mod() - static_cast<int>(v_)); }

    mint& operator+=(const mint& rhs) {
        v_ += rhs.v_;
        if (v_ >= static_cast<unsigned int>(mod())) v_ -= mod();
        return *this;
    }

    mint& operator-=(const mint& rhs) {
        if (v_ < rhs.v_) v_ += mod();
        v_ -= rhs.v_;
        return *this;
    }

    mint& operator*=(const mint& rhs) {
        v_ = static_cast<unsigned int>((static_cast<unsigned long long>(v_) * rhs.v_) % mod());
        return *this;
    }

    mint& operator/=(const mint& rhs) { return *this *= rhs.inv(); }

    mint pow(long long n) const {
        assert(n >= 0);
        mint x = *this, r = 1;
        while (n) {
            if (n & 1) r *= x;
            x *= x;
            n >>= 1;
        }
        return r;
    }

    mint inv() const {
        auto [g, x] = internal::inv_gcd(v_, mod());
        assert(g == 1);
        return x;
    }

    friend mint operator+(mint lhs, const mint& rhs) { return lhs += rhs; }
    friend mint operator-(mint lhs, const mint& rhs) { return lhs -= rhs; }
    friend mint operator*(mint lhs, const mint& rhs) { return lhs *= rhs; }
    friend mint operator/(mint lhs, const mint& rhs) { return lhs /= rhs; }
    friend bool operator==(const mint& lhs, const mint& rhs) { return lhs.v_ == rhs.v_; }
    friend bool operator!=(const mint& lhs, const mint& rhs) { return lhs.v_ != rhs.v_; }
    friend std::istream& operator>>(std::istream& is, mint& x) {
        long long v;
        is >> v;
        x = mint(v);
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const mint& x) { return os << x.val(); }

private:
    static int& mod_ref() {
        static int m = 998244353;
        return m;
    }

    unsigned int v_;
};

using modint998244353 = static_modint<998244353>;
using modint1000000007 = static_modint<1000000007>;
using modint = dynamic_modint<0>;

template <class Mint>
class mod_combination {
public:
    mod_combination() : fact_(1, Mint(1)), inv_fact_(1, Mint(1)), inv_(1, Mint(0)) {}
    explicit mod_combination(int n) : mod_combination() { reserve(n); }

    void reserve(int n) {
        if (n < static_cast<int>(fact_.size())) return;
        int old = static_cast<int>(fact_.size());
        fact_.resize(n + 1);
        inv_fact_.resize(n + 1);
        inv_.resize(n + 1);
        for (int i = old; i <= n; ++i) fact_[i] = fact_[i - 1] * i;
        inv_fact_[n] = fact_[n].inv();
        for (int i = n; i > old; --i) inv_fact_[i - 1] = inv_fact_[i] * i;
        for (int i = old; i <= n; ++i) inv_[i] = fact_[i - 1] * inv_fact_[i];
    }

    Mint fact(int n) {
        assert(n >= 0);
        reserve(n);
        return fact_[n];
    }

    Mint inv_fact(int n) {
        assert(n >= 0);
        reserve(n);
        return inv_fact_[n];
    }

    Mint inv(int n) {
        assert(n > 0);
        reserve(n);
        return inv_[n];
    }

    Mint C(long long n, long long k) {
        if (k < 0 || k > n) return 0;
        assert(n <= std::numeric_limits<int>::max());
        reserve(static_cast<int>(n));
        return fact_[n] * inv_fact_[k] * inv_fact_[n - k];
    }

    Mint P(long long n, long long k) {
        if (k < 0 || k > n) return 0;
        assert(n <= std::numeric_limits<int>::max());
        reserve(static_cast<int>(n));
        return fact_[n] * inv_fact_[n - k];
    }

    Mint H(long long n, long long k) {
        if (n == 0 && k == 0) return 1;
        if (n <= 0 || k < 0) return 0;
        return C(n + k - 1, k);
    }

private:
    std::vector<Mint> fact_;
    std::vector<Mint> inv_fact_;
    std::vector<Mint> inv_;
};

}  // namespace poe
#line 2 "structure/kd_tree_rectangle_affine_sum.hpp"

#include <algorithm>
#line 6 "structure/kd_tree_rectangle_affine_sum.hpp"
#include <map>
#line 8 "structure/kd_tree_rectangle_affine_sum.hpp"

namespace poe {

template <class T>
class kd_tree_rectangle_affine_sum {
public:
    struct point {
        long long x, y;
        T w;
        bool active = true;
    };
    struct affine {
        T a;
        T b;
    };

    kd_tree_rectangle_affine_sum() = default;
    explicit kd_tree_rectangle_affine_sum(const std::vector<point>& points) : points_(points) {
        std::vector<long long> xs, ys;
        xs.reserve(points_.size());
        ys.reserve(points_.size());
        for (const auto& p : points_) {
            xs.push_back(p.x);
            ys.push_back(p.y);
        }
        std::sort(xs.begin(), xs.end());
        xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
        std::sort(ys.begin(), ys.end());
        ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
        constexpr int small_limit = 700;
        if (!points_.empty() && static_cast<int>(ys.size()) <= small_limit && ys.size() <= xs.size()) {
            mode_ = mode::rows;
            build_groups(false, ys);
            return;
        }
        if (!points_.empty() && static_cast<int>(xs.size()) <= small_limit) {
            mode_ = mode::cols;
            build_groups(true, xs);
            return;
        }
        mode_ = mode::kd;
        std::vector<int> ids(points_.size());
        for (int i = 0; i < static_cast<int>(ids.size()); ++i) ids[i] = i;
        nodes_.reserve(points_.size());
        parent_.reserve(points_.size());
        leaf_.assign(points_.size(), -1);
        root_ = build(ids, 0, static_cast<int>(ids.size()), 0);
    }

    int size() const { return static_cast<int>(points_.size()); }

    void activate(int id, T w) {
        assert(0 <= id && id < size());
        points_[id].active = true;
        if (mode_ != mode::kd) {
            groups_[group_id_[id]].set(point_pos_[id], w);
            return;
        }
        set_weight(id, w);
    }

    void set(int id, T w) {
        assert(0 <= id && id < size());
        points_[id].active = true;
        if (mode_ != mode::kd) {
            groups_[group_id_[id]].set(point_pos_[id], w);
            return;
        }
        set_weight(id, w);
    }

    T sum(long long l, long long d, long long r, long long u) {
        if (mode_ == mode::rows) return groups_sum(d, u, l, r);
        if (mode_ == mode::cols) return groups_sum(l, r, d, u);
        return sum(root_, l, d, r, u);
    }

    void apply(long long l, long long d, long long r, long long u, affine f) {
        if (mode_ == mode::rows) {
            groups_apply(d, u, l, r, f);
            return;
        }
        if (mode_ == mode::cols) {
            groups_apply(l, r, d, u, f);
            return;
        }
        apply(root_, l, d, r, u, f);
    }

private:
    enum class mode { kd, rows, cols };

    struct group_entry {
        long long coord;
        int id;
        point p;
    };

    struct group {
        int n = 0;
        std::vector<long long> coord;
        std::vector<int> active_count;
        std::vector<T> sum;
        std::vector<affine> lazy;

        explicit group(const std::vector<group_entry>& entries) {
            n = static_cast<int>(entries.size());
            coord.reserve(n);
            active_count.assign(n * 4, 0);
            sum.assign(n * 4, T{});
            lazy.assign(n * 4, {T(1), T(0)});
            for (const auto& e : entries) coord.push_back(e.coord);
            build(entries, 1, 0, n);
        }

        void set(int pos, T value) { set(1, 0, n, pos, value); }

        T range_sum(long long l, long long r) {
            int ql = static_cast<int>(std::lower_bound(coord.begin(), coord.end(), l) - coord.begin());
            int qr = static_cast<int>(std::lower_bound(coord.begin(), coord.end(), r) - coord.begin());
            return range_sum(1, 0, n, ql, qr);
        }

        void range_apply(long long l, long long r, affine f) {
            int ql = static_cast<int>(std::lower_bound(coord.begin(), coord.end(), l) - coord.begin());
            int qr = static_cast<int>(std::lower_bound(coord.begin(), coord.end(), r) - coord.begin());
            range_apply(1, 0, n, ql, qr, f);
        }

    private:
        static affine compose(affine f, affine g) {
            return {f.a * g.a, f.a * g.b + f.b};
        }

        void build(const std::vector<group_entry>& entries, int k, int l, int r) {
            if (r - l == 1) {
                if (entries[l].p.active) {
                    active_count[k] = 1;
                    sum[k] = entries[l].p.w;
                }
                return;
            }
            int m = (l + r) >> 1;
            build(entries, k << 1, l, m);
            build(entries, k << 1 | 1, m, r);
            pull(k);
        }

        void all_apply(int k, affine f) {
            if (active_count[k] == 0) return;
            sum[k] = f.a * sum[k] + f.b * T(active_count[k]);
            lazy[k] = compose(f, lazy[k]);
        }

        void push(int k) {
            affine f = lazy[k];
            if (f.a == T(1) && f.b == T(0)) return;
            all_apply(k << 1, f);
            all_apply(k << 1 | 1, f);
            lazy[k] = {T(1), T(0)};
        }

        void pull(int k) {
            active_count[k] = active_count[k << 1] + active_count[k << 1 | 1];
            sum[k] = sum[k << 1] + sum[k << 1 | 1];
        }

        void set(int k, int l, int r, int pos, T value) {
            if (r - l == 1) {
                active_count[k] = 1;
                sum[k] = value;
                lazy[k] = {T(1), T(0)};
                return;
            }
            push(k);
            int m = (l + r) >> 1;
            if (pos < m) set(k << 1, l, m, pos, value);
            else set(k << 1 | 1, m, r, pos, value);
            pull(k);
        }

        T range_sum(int k, int l, int r, int ql, int qr) {
            if (qr <= l || r <= ql || active_count[k] == 0) return T{};
            if (ql <= l && r <= qr) return sum[k];
            push(k);
            int m = (l + r) >> 1;
            return range_sum(k << 1, l, m, ql, qr) + range_sum(k << 1 | 1, m, r, ql, qr);
        }

        void range_apply(int k, int l, int r, int ql, int qr, affine f) {
            if (qr <= l || r <= ql || active_count[k] == 0) return;
            if (ql <= l && r <= qr) {
                all_apply(k, f);
                return;
            }
            push(k);
            int m = (l + r) >> 1;
            range_apply(k << 1, l, m, ql, qr, f);
            range_apply(k << 1 | 1, m, r, ql, qr, f);
            pull(k);
        }
    };

    struct node {
        int left = -1;
        int right = -1;
        int point_id = -1;
        long long min_x, max_x, min_y, max_y;
        int active_count = 0;
        T sum{};
        affine lazy{T(1), T(0)};
    };

    static affine compose(affine f, affine g) {
        return {f.a * g.a, f.a * g.b + f.b};
    }

    void build_groups(bool use_x_as_group, const std::vector<long long>& keys) {
        group_keys_ = keys;
        std::map<long long, std::vector<group_entry>> buckets;
        for (int i = 0; i < size(); ++i) {
            long long key = use_x_as_group ? points_[i].x : points_[i].y;
            long long value = use_x_as_group ? points_[i].y : points_[i].x;
            buckets[key].push_back({value, i, points_[i]});
        }
        group_id_.assign(size(), -1);
        point_pos_.assign(size(), -1);
        groups_.reserve(group_keys_.size());
        for (int gid = 0; gid < static_cast<int>(group_keys_.size()); ++gid) {
            auto& entries = buckets[group_keys_[gid]];
            std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
                return a.coord != b.coord ? a.coord < b.coord : a.id < b.id;
            });
            for (int pos = 0; pos < static_cast<int>(entries.size()); ++pos) {
                group_id_[entries[pos].id] = gid;
                point_pos_[entries[pos].id] = pos;
            }
            groups_.push_back(group(entries));
        }
    }

    T groups_sum(long long kl, long long kr, long long vl, long long vr) {
        T res{};
        int gl = static_cast<int>(std::lower_bound(group_keys_.begin(), group_keys_.end(), kl) - group_keys_.begin());
        int gr = static_cast<int>(std::lower_bound(group_keys_.begin(), group_keys_.end(), kr) - group_keys_.begin());
        for (int i = gl; i < gr; ++i) res += groups_[i].range_sum(vl, vr);
        return res;
    }

    void groups_apply(long long kl, long long kr, long long vl, long long vr, affine f) {
        int gl = static_cast<int>(std::lower_bound(group_keys_.begin(), group_keys_.end(), kl) - group_keys_.begin());
        int gr = static_cast<int>(std::lower_bound(group_keys_.begin(), group_keys_.end(), kr) - group_keys_.begin());
        for (int i = gl; i < gr; ++i) groups_[i].range_apply(vl, vr, f);
    }

    int build(std::vector<int>& ids, int l, int r, int depth) {
        if (l == r) return -1;
        int k = static_cast<int>(nodes_.size());
        nodes_.push_back({});
        parent_.push_back(-1);
        nodes_[k].min_x = nodes_[k].min_y = std::numeric_limits<long long>::max();
        nodes_[k].max_x = nodes_[k].max_y = std::numeric_limits<long long>::min();
        for (int i = l; i < r; ++i) {
            auto p = points_[ids[i]];
            nodes_[k].min_x = std::min(nodes_[k].min_x, p.x);
            nodes_[k].max_x = std::max(nodes_[k].max_x, p.x);
            nodes_[k].min_y = std::min(nodes_[k].min_y, p.y);
            nodes_[k].max_y = std::max(nodes_[k].max_y, p.y);
        }
        if (r - l == 1) {
            nodes_[k].point_id = ids[l];
            leaf_[ids[l]] = k;
            if (points_[ids[l]].active) {
                nodes_[k].active_count = 1;
                nodes_[k].sum = points_[ids[l]].w;
            }
            return k;
        }
        bool split_x = (depth % 2 == 0);
        if (nodes_[k].min_x == nodes_[k].max_x) split_x = false;
        if (nodes_[k].min_y == nodes_[k].max_y) split_x = true;
        int m = (l + r) >> 1;
        std::nth_element(ids.begin() + l, ids.begin() + m, ids.begin() + r, [&](int a, int b) {
            if (split_x) return points_[a].x != points_[b].x ? points_[a].x < points_[b].x : points_[a].y < points_[b].y;
            return points_[a].y != points_[b].y ? points_[a].y < points_[b].y : points_[a].x < points_[b].x;
        });
        nodes_[k].left = build(ids, l, m, depth + 1);
        nodes_[k].right = build(ids, m, r, depth + 1);
        if (nodes_[k].left != -1) parent_[nodes_[k].left] = k;
        if (nodes_[k].right != -1) parent_[nodes_[k].right] = k;
        pull(k);
        return k;
    }

    bool disjoint(int k, long long l, long long d, long long r, long long u) const {
        const auto& nd = nodes_[k];
        return nd.max_x < l || r <= nd.min_x || nd.max_y < d || u <= nd.min_y;
    }

    bool covered(int k, long long l, long long d, long long r, long long u) const {
        const auto& nd = nodes_[k];
        return l <= nd.min_x && nd.max_x < r && d <= nd.min_y && nd.max_y < u;
    }

    void all_apply(int k, affine f) {
        if (k == -1 || nodes_[k].active_count == 0) return;
        nodes_[k].sum = f.a * nodes_[k].sum + f.b * T(nodes_[k].active_count);
        nodes_[k].lazy = compose(f, nodes_[k].lazy);
    }

    void push(int k) {
        affine f = nodes_[k].lazy;
        if (f.a == T(1) && f.b == T(0)) return;
        all_apply(nodes_[k].left, f);
        all_apply(nodes_[k].right, f);
        nodes_[k].lazy = {T(1), T(0)};
    }

    void pull(int k) {
        auto& nd = nodes_[k];
        nd.active_count = 0;
        nd.sum = T{};
        if (nd.point_id != -1) {
            if (points_[nd.point_id].active) {
                nd.active_count = 1;
                nd.sum = points_[nd.point_id].w;
            }
            return;
        }
        if (nd.left != -1) {
            nd.active_count += nodes_[nd.left].active_count;
            nd.sum += nodes_[nd.left].sum;
        }
        if (nd.right != -1) {
            nd.active_count += nodes_[nd.right].active_count;
            nd.sum += nodes_[nd.right].sum;
        }
    }

    void set_weight(int id, T w) {
        int k = leaf_[id];
        std::vector<int> path;
        for (int v = k; v != -1; v = parent_[v]) path.push_back(v);
        for (int i = static_cast<int>(path.size()) - 1; i >= 0; --i) push(path[i]);
        points_[id].w = w;
        nodes_[k].active_count = 1;
        nodes_[k].sum = w;
        for (int i = 1; i < static_cast<int>(path.size()); ++i) pull(path[i]);
    }

    T sum(int k, long long l, long long d, long long r, long long u) {
        if (k == -1 || disjoint(k, l, d, r, u) || nodes_[k].active_count == 0) return T{};
        if (covered(k, l, d, r, u)) return nodes_[k].sum;
        push(k);
        if (nodes_[k].point_id != -1) {
            const auto& p = points_[nodes_[k].point_id];
            return p.active && l <= p.x && p.x < r && d <= p.y && p.y < u ? nodes_[k].sum : T{};
        }
        return sum(nodes_[k].left, l, d, r, u) + sum(nodes_[k].right, l, d, r, u);
    }

    void apply(int k, long long l, long long d, long long r, long long u, affine f) {
        if (k == -1 || disjoint(k, l, d, r, u) || nodes_[k].active_count == 0) return;
        if (covered(k, l, d, r, u)) {
            all_apply(k, f);
            return;
        }
        push(k);
        if (nodes_[k].point_id != -1) {
            const auto& p = points_[nodes_[k].point_id];
            if (p.active && l <= p.x && p.x < r && d <= p.y && p.y < u) all_apply(k, f);
            return;
        }
        apply(nodes_[k].left, l, d, r, u, f);
        apply(nodes_[k].right, l, d, r, u, f);
        pull(k);
    }

    std::vector<point> points_;
    mode mode_ = mode::kd;
    std::vector<long long> group_keys_;
    std::vector<group> groups_;
    std::vector<int> group_id_;
    std::vector<int> point_pos_;
    std::vector<node> nodes_;
    std::vector<int> parent_;
    std::vector<int> leaf_;
    int root_ = -1;
};

}  // namespace poe
#line 4 "verify/yosupo_dynamic_point_set_rectangle_affine_rectangle_sum.test.cpp"

#include <bits/stdc++.h>
using namespace std;

struct query {
    int type;
    long long x, y, l, d, r, u;
    poe::modint998244353 w, a, b;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using mint = poe::modint998244353;
    int n, q;
    cin >> n >> q;
    vector<poe::kd_tree_rectangle_affine_sum<mint>::point> points;
    points.reserve(n + q);
    for (int i = 0; i < n; ++i) {
        long long x, y;
        mint w;
        cin >> x >> y >> w;
        points.push_back({x, y, w, true});
    }
    vector<query> queries(q);
    for (auto& e : queries) {
        cin >> e.type;
        if (e.type == 0) {
            cin >> e.x >> e.y >> e.w;
            points.push_back({e.x, e.y, mint(0), false});
        } else if (e.type == 1) {
            cin >> e.x >> e.w;
        } else if (e.type == 2) {
            cin >> e.l >> e.d >> e.r >> e.u;
        } else {
            cin >> e.l >> e.d >> e.r >> e.u >> e.a >> e.b;
        }
    }
    poe::kd_tree_rectangle_affine_sum<mint> ds(points);
    int next_id = n;
    for (auto e : queries) {
        if (e.type == 0) {
            ds.activate(next_id++, e.w);
        } else if (e.type == 1) {
            ds.set(static_cast<int>(e.x), e.w);
        } else if (e.type == 2) {
            cout << ds.sum(e.l, e.d, e.r, e.u) << '\n';
        } else {
            ds.apply(e.l, e.d, e.r, e.u, {e.a, e.b});
        }
    }
}