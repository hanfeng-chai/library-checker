#include <atcoder/modint>
#include <bits/stdc++.h>

auto rerooting(const auto &g,   // Graph
               auto op,         // (PathState, PathState) -> PathState
               auto to_path,    // (TreeState, EdgeWeight) -> PathState
               auto to_subtree, // (PathState, NodeWeight) -> TreeState
               auto e           // PathState (Identity)
) {
    int n = g.size();
    using PathState = decltype(e);
    using TreeState = decltype(to_subtree(e, g.node_weight(0)));
    std::vector<TreeState> dp(n), dp_parent(n);
    std::vector<int> q, parent(n, -1);
    std::vector<PathState> pref(n + 1);
    q.reserve(n);
    for (int root = 0; root < n; ++root) {
        if (~parent[root]) {
            continue;
        }
        parent[root] = root;
        q.clear();
        q.push_back(root);
        auto it = q.cbegin();
        while (it != q.cend()) {
            int u = *it++;
            for (auto [v, w] : g[u]) {
                if (v != parent[u]) {
                    parent[v] = u;
                    q.push_back(v);
                }
            }
        }
        for (auto u : q | std::views::reverse) {
            PathState merged = e;
            for (auto [v, w] : g[u]) {
                if (v != parent[u]) {
                    merged = op(merged, to_path(dp[v], w));
                }
            }
            dp[u] = to_subtree(merged, g.node_weight(u));
        }
        for (auto u : q) {
            int i = 0;
            pref[0] = e;
            for (auto [v, w] : g[u]) {
                auto state = v == parent[u] ? dp_parent[u] : dp[v];
                pref[i + 1] = op(pref[i], to_path(state, w));
                ++i;
            }
            auto suff = e;
            for (auto [v, w] : g[u] | std::views::reverse) {
                if (v != parent[u]) {
                    PathState except_child = op(pref[i - 1], suff);
                    dp_parent[v] = to_subtree(except_child, g.node_weight(u));
                }
                auto state = v == parent[u] ? dp_parent[u] : dp[v];
                suff = op(to_path(state, w), suff);
                --i;
            }
            dp[u] = to_subtree(suff, g.node_weight(u));
        }
    }
    return dp;
}

template <typename NodeWeight = std::monostate, typename EdgeWeight = std::monostate> struct CSRGraph {
    static constexpr bool HasNodeWeight = !std::is_same_v<NodeWeight, std::monostate>;
    CSRGraph(int n) : n_(n), start_(n + 1) {
        if constexpr (HasNodeWeight) {
            nodes_.resize(n_);
        }
    }
    void set_node(int u, NodeWeight w) {
        assert(0 <= u && u < n_);
        if constexpr (HasNodeWeight) {
            nodes_[u] = w;
        }
    }
    NodeWeight node_weight(int u) const {
        assert(0 <= u && u < n_);
        if constexpr (HasNodeWeight) {
            return nodes_[u];
        } else {
            return {};
        }
    }
    void add_edge(int u, int v, EdgeWeight w = {}) {
        assert(0 <= u && u < n_ && 0 <= v && v < n_);
        raw_edges_.push_back({u, v, w});
    }
    void build_undirected() {
        assert(!built_);
        edges_.resize(2 * raw_edges_.size());
        for (const auto &e : raw_edges_) {
            ++start_[e.u + 1];
            ++start_[e.v + 1];
        }
        for (int i = 0; i < n_; ++i) {
            start_[i + 1] += start_[i];
        }
        auto counter = start_;
        for (const auto &e : raw_edges_) {
            edges_[counter[e.u]++] = {e.v, e.w};
            edges_[counter[e.v]++] = {e.u, e.w};
        }
        std::vector<RawEdge>().swap(raw_edges_);
        built_ = true;
    }
    void build_directed() {
        assert(!built_);
        edges_.resize(raw_edges_.size());
        for (const auto &e : raw_edges_) {
            ++start_[e.u + 1];
        }
        for (int i = 0; i < n_; ++i) {
            start_[i + 1] += start_[i];
        }
        auto counter = start_;
        for (const auto &e : raw_edges_) {
            edges_[counter[e.u]++] = {e.v, e.w};
        }
        std::vector<RawEdge>().swap(raw_edges_);
        built_ = true;
    }
    auto operator[](int u) const {
        assert(built_);
        assert(0 <= u && u < n_);
        constexpr auto f = [](Edge e) { return std::pair(e.to, e.w); };
        return std::ranges::subrange(edges_.begin() + start_[u], edges_.begin() + start_[u + 1]) | std::views::transform(f);
    }
    int size() const { return n_; }
    struct Edge {
        int to;
        [[no_unique_address]] EdgeWeight w;
    };
    struct RawEdge {
        int u, v;
        [[no_unique_address]] EdgeWeight w;
    };
    int n_;
    bool built_ = false;
    std::vector<Edge> edges_;
    std::vector<int> start_;
    std::vector<RawEdge> raw_edges_;
    std::vector<NodeWeight> nodes_;
};

#include <sys/mman.h>
#include <sys/stat.h>

struct scanner {
    scanner() {
        struct stat st;
        fstat(0, &st);
        data_ = p_ = static_cast<char *>(mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0));
        size_ = st.st_size;
    }
    template <typename... Args> void operator()(Args &...args) { (read(args), ...); }
    std::string_view getline() {
        skip();
        auto first = p_;
        while (*p_ >= ' ') {
            ++p_;
        }
        return std::string_view(first, p_ - first);
    }

private:
    template <typename T> std::enable_if_t<std::is_integral_v<T>, void> read(T &x) {
        skip();
        x = 0;
        auto is_negative = false;
        if (*p_ == '-') {
            ++p_;
            is_negative = true;
        }
        for (; *p_ > ' '; ++p_) {
            x = (x << 1) + (x << 3) + (*p_ & 15);
        }
        if (is_negative) {
            x = -x;
        }
    }
    template <typename T> std::enable_if_t<std::is_floating_point_v<T>, void> read(T &x) {
        skip();
        auto first = p_;
        while (*p_ > ' ') {
            ++p_;
        }
        std::from_chars(first, p_, x);
    }
    void read(char &x) {
        skip();
        x = *p_++;
    }
    void skip() {
        while (*p_ <= ' ') {
            ++p_;
        }
    }
    char *data_;
    char *p_;
    size_t size_;
};

using namespace std;
using mint = atcoder::modint998244353;

scanner scan;

int main() {
    int N;
    scan(N);
    CSRGraph<int, pair<int, int>> g(N);
    for (auto i = 0; i < N; ++i) {
        int a;
        scan(a);
        g.set_node(i, a);
    }
    for (auto i = 0; i < N - 1; ++i) {
        int u, v, b, c;
        scan(u, v, b, c);
        g.add_edge(u, v, {b, c});
    }
    g.build_undirected();
    using State = pair<mint, int>;
    auto op = [](State a, State b) -> State { return {a.first + b.first, a.second + b.second}; };
    auto to_path = [](State x, auto e) -> State { return {mint::raw(e.first) * x.first + mint::raw(e.second) * mint::raw(x.second), x.second}; };
    auto to_subtree = [](State x, int v) -> State { return {x.first + mint::raw(v), x.second + 1}; };
    State e{0, 0};
    auto dp = rerooting(g, op, to_path, to_subtree, e);
    for (auto [sum, cnt] : dp) {
        cout << sum.val() << ' ';
    }
}
