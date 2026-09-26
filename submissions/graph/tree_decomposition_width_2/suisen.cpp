#include <cstring>
#include <iostream>
#include <limits>
#include <tuple>

#ifdef _MSC_VER
#  include <intrin.h>
#else
#  include <x86intrin.h>
#endif

// https://judge.yosupo.jp/submission/47903
namespace fastio {
    static constexpr int SZ = 1 << 17;
    char inbuf[SZ], outbuf[SZ];
    int in_left = 0, in_right = 0, out_right = 0;

    struct Pre {
        char num[40000];
        constexpr Pre() : num() {
            for (int i = 0; i < 10000; i++) {
                int n = i;
                for (int j = 3; j >= 0; j--) {
                    num[i * 4 + j] = n % 10 + '0';
                    n /= 10;
                }
            }
        }
    } constexpr pre;

    inline void load() {
        int len = in_right - in_left;
        ::memmove(inbuf, inbuf + in_left, len);
        in_right = len + fread(inbuf + len, 1, SZ - len, stdin);
        in_left = 0;
    }

    inline void flush() {
        ::fwrite(outbuf, 1, out_right, stdout);
        out_right = 0;
    }

    inline void skip_space() {
        if (in_left + 32 > in_right) load();
        while (inbuf[in_left] <= ' ') in_left++;
    }

    inline void rd(char& c) {
        if (in_left + 32 > in_right) load();
        c = inbuf[in_left++];
    }
    template <typename T>
    inline void rd(T& x) {
        if (in_left + 32 > in_right) load();
        char c;
        do c = inbuf[in_left++];
        while (c < '-');
        [[maybe_unused]] bool minus = false;
        if constexpr (std::is_signed<T>::value == true) {
            if (c == '-') minus = true, c = inbuf[in_left++];
        }
        x = 0;
        while (c >= '0') {
            x = x * 10 + (c & 15);
            c = inbuf[in_left++];
        }
        if constexpr (std::is_signed<T>::value == true) {
            if (minus) x = -x;
        }
    }
    inline void rd() {}
    template <typename Head, typename... Tail>
    inline void rd(Head& head, Tail&... tail) {
        rd(head);
        rd(tail...);
    }

    inline void wt(char c) {
        if (out_right > SZ - 32) flush();
        outbuf[out_right++] = c;
    }
    inline void wt(bool b) {
        if (out_right > SZ - 32) flush();
        outbuf[out_right++] = b ? '1' : '0';
    }
    template <typename T>
    inline void wt(T x) {
        if (out_right > SZ - 32) flush();
        if (!x) {
            outbuf[out_right++] = '0';
            return;
        }
        if constexpr (std::is_signed<T>::value == true) {
            if (x < 0) outbuf[out_right++] = '-', x = -x;
        }
        int i = 12;
        char buf[16];
        while (x >= 10000) {
            memcpy(buf + i, pre.num + (x % 10000) * 4, 4);
            x /= 10000;
            i -= 4;
        }
        if (x < 100) {
            if (x < 10) {
                outbuf[out_right] = '0' + x;
                ++out_right;
            } else {
                uint32_t q = (uint32_t(x) * 205) >> 11;
                uint32_t r = uint32_t(x) - q * 10;
                outbuf[out_right] = '0' + q;
                outbuf[out_right + 1] = '0' + r;
                out_right += 2;
            }
        } else {
            if (x < 1000) {
                memcpy(outbuf + out_right, pre.num + (x << 2) + 1, 3);
                out_right += 3;
            } else {
                memcpy(outbuf + out_right, pre.num + (x << 2), 4);
                out_right += 4;
            }
        }
        memcpy(outbuf + out_right, buf + i + 4, 12 - i);
        out_right += 12 - i;
    }
    inline void wt() {}
    template <typename Head, typename... Tail>
    inline void wt(Head&& head, Tail&&... tail) {
        wt(head);
        wt(std::forward<Tail>(tail)...);
    }
    template <typename... Args>
    inline void wtn(Args&&... x) {
        wt(std::forward<Args>(x)...);
        wt('\n');
    }

    struct Dummy {
        Dummy() { atexit(flush); }
    } dummy;

}  // namespace fastio
using fastio::rd;
using fastio::skip_space;
using fastio::wt;
using fastio::wtn;

#include <array>
#include <atcoder/dsu>
#include <cassert>
#include <deque>
#include <set>
#include <optional>
#include <utility>
#include <vector>

namespace suisen {
    struct TreeDecompositionTW2 {
        TreeDecompositionTW2(const int n) : _n(n), _edges{} {}

        void add_edge(int u, int v) {
            _edges.emplace_back(u, v);
        }

        std::optional<std::pair<std::vector<std::vector<int>>, std::vector<std::pair<int, int>>>> build() {
            std::vector<std::vector<std::pair<int, int>>> g(_n);
            for (auto [u, v] : _edges) if (u != v) {
                if (u > v) std::swap(u, v);
                const int du = g[u].size(), dv = g[v].size();
                g[u].emplace_back(v, dv);
                g[v].emplace_back(u, du);
            }

            std::vector<int8_t> seen(_n, false);
            std::deque<int> dq;
            for (int i = 0; i < _n; ++i) if (g[i].size() <= 2) {
                dq.push_back(i);
                seen[i] = true;
            }

            std::vector<int> roots;
            std::vector<std::pair<int, int>> edges;
            edges.reserve(_n - 1);
            std::vector<std::vector<int>> bag(_n);
            std::vector<std::vector<int>> link(_n);

            atcoder::dsu uf(_n);
            for (int id = 0; id < _n; ++id) {
                if (dq.empty()) return std::nullopt;
                int u = dq.front();
                dq.pop_front();
                if (g[u].size() == 0) {
                    bag[id] = { u };
                    roots.push_back(id);
                } else if (g[u].size() == 1) {
                    int v = remove_edge(g, u, 0);
                    if (g[v].size() <= 2 and not std::exchange(seen[v], true)) dq.push_back(v);
                    bag[id] = { u, v };
                    link[v].push_back(id);
                } else {
                    int v = remove_edge(g, u, 0);
                    int w = remove_edge(g, u, 0);
                    if (v > w) std::swap(v, w);
                    bag[id] = { u, v, w };
                    const int dv = g[v].size(), dw = g[w].size();
                    g[v].emplace_back(w, dw);
                    g[w].emplace_back(v, dv);
                    remove_multiedges(g, v, dv);
                    remove_multiedges(g, w, dw);
                    if (g[v].size() <= 2 and not std::exchange(seen[v], true)) dq.push_back(v);
                    if (g[w].size() <= 2 and not std::exchange(seen[w], true)) dq.push_back(w);
                    link[v].push_back(id);
                    link[w].push_back(id);
                }
                std::reverse(link[u].begin(), link[u].end());
                for (int id2 : link[u]) if (not uf.same(id, id2)) {
                    edges.emplace_back(id, id2);
                    uf.merge(id, id2);
                }
                g[u].clear(), g[u].shrink_to_fit(), link[u].clear(), link[u].shrink_to_fit();
            }
            const int root_num = roots.size();
            for (int i = 0; i < root_num - 1; ++i) {
                edges.emplace_back(roots[i], roots[i + 1]);
            }
            return std::pair{ std::move(bag), std::move(edges) };
        }
    private:
        int _n;
        std::vector<std::pair<int, int>> _edges;

        static int remove_edge(std::vector<std::vector<std::pair<int, int>>>& g, int u, int idx_uv) {
            auto [v, idx_vu] = g[u][idx_uv];

            if (idx_vu != int(g[v].size()) - 1) {
                auto [w, idx_wv] = g[v].back();
                std::swap(g[v][idx_vu], g[v].back());
                g[w][idx_wv].second = idx_vu;
            }
            g[v].pop_back();
            if (idx_uv != int(g[u].size()) - 1) {
                auto [z, idx_zu] = g[u].back();
                std::swap(g[u][idx_uv], g[u].back());
                g[z][idx_zu].second = idx_uv;
            }
            g[u].pop_back();

            remove_multiedges(g, v, idx_vu);
            remove_multiedges(g, u, idx_uv);

            return v;
        }
        static void remove_multiedges(std::vector<std::vector<std::pair<int, int>>>& g, int u, int idx_uv) {
            auto is_unnecessary = [&](int idx_uv) {
                const int du = int(g[u].size());
                if (idx_uv >= du) return false;
                if (idx_uv + 1 < du and g[u][idx_uv].first == g[u][idx_uv + 1].first) return true;
                if (idx_uv + 2 < du and g[u][idx_uv].first == g[u][idx_uv + 2].first) return true;
                if (idx_uv - 1 >= 0 and g[u][idx_uv].first == g[u][idx_uv - 1].first) return true;
                if (idx_uv - 2 >= 0 and g[u][idx_uv].first == g[u][idx_uv - 2].first) return true;
                return false;
            };
            while (is_unnecessary(idx_uv)) remove_edge(g, u, idx_uv);
        }
    };
} // namespace suisen

int main() {
    char c;
    rd(c), skip_space(), rd(c), rd(c);

    int n, m;
    rd(n, m);

    suisen::TreeDecompositionTW2 td(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        rd(u, v);
        --u, --v;
        td.add_edge(u, v);
    }

    const auto opt_res = td.build();
    if (not opt_res.has_value()) {
        wtn(-1);
    } else {
        const auto& [bag, edges] = *opt_res;
        const int k = bag.size();
        wtn('s', ' ', 't', 'd', ' ', k, ' ', 2, ' ', n);
        for (int i = 0; i < k; ++i) {
            wt('b', ' ', i + 1);
            for (int v : bag[i]) wt(' ', v + 1);
            wt('\n');
        }
        for (auto [u, v] : edges) {
            wtn(u + 1, ' ', v + 1);
        }
    }

    return 0;
}

