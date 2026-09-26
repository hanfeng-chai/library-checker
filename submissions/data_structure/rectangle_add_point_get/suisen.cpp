
#include <algorithm>
#include <array>
#include <numeric>
#include <vector>

#include <atcoder/fenwicktree>

namespace suisen {
    template <typename T, typename Index = int>
    struct OfflineRectangleAddPointGet {
    private:
        using Rectangle = std::array<Index, 4>;
        using Point = std::array<Index, 2>;
    public:
        void add(Index l, Index r, Index d, Index u, T w) {
            if (l == r or d == u) return;
            assert(l <= r and d <= u);
            _rects.push_back({ l, r, d, u });
            _weights.push_back(w);
        }
        void get(Index x, Index y) {
            _points.push_back({ x, y });
            _times.push_back(_rects.size());
        }

        std::vector<T> solve() const {
            std::vector<T> ans(_points.size());

            const int rect_num = _rects.size();

            //            (l,d,+w), (l,u,-w), (r,d,-w), (r,u,+w)
            // index x:      2i        2i       2i+1      2i+1
            // index y:      2i       2i+1       2i       2i+1

            using Key = std::pair<Index, int>;
            const auto compare = [](const Key &k1, const Key& k2) { return k1.first < k2.first; };

            std::vector<Key> es_asc_x(2 * rect_num), es_asc_y(2 * rect_num);
            for (int i = 0; i < 2 * rect_num; ++i) {
                es_asc_x[i] = Key{ rect_x(i), i };
                es_asc_y[i] = Key{ rect_y(i), i };
            }
            std::vector<std::vector<Key>> ps_asc_x(rect_num), ps_asc_y(rect_num);
            for (int pid = 0; pid < int(_points.size()); ++pid) if (_times[pid]) {
                const int r = floor_pow2(_times[pid]);
                ps_asc_x[r - 1].emplace_back(point_x(pid), pid);
                ps_asc_y[r - 1].emplace_back(point_y(pid), pid);
            }

            std::vector<int> ps_comp_y(_points.size()), es_comp_y(2 * rect_num);

            for (int r = 1; r <= rect_num; ++r) {
                const int w = -r & r, l = r - w;
                for (int t = 1; t < w; t <<= 1) {
                    auto it_r_x = es_asc_x.begin() + 2 * r, it_m_x = it_r_x - 2 * t, it_l_x = it_m_x - 2 * t;
                    std::inplace_merge(it_l_x, it_m_x, it_r_x, compare);
                    auto it_r_y = es_asc_y.begin() + 2 * r, it_m_y = it_r_y - 2 * t, it_l_y = it_m_y - 2 * t;
                    std::inplace_merge(it_l_y, it_m_y, it_r_y, compare);
                }
                const int point_num = ps_asc_x[r - 1].size();
                if (point_num == 0) continue;
                if (r == (-r & r)) {
                    std::sort(ps_asc_x[r - 1].begin(), ps_asc_x[r - 1].end(), compare);
                    std::sort(ps_asc_y[r - 1].begin(), ps_asc_y[r - 1].end(), compare);
                }
                int ynum = 0;
                {
                    std::vector<Key> asc_y(es_asc_y.begin() + 2 * l, es_asc_y.begin() + 2 * r);
                    const int mid_siz = asc_y.size(), siz = mid_siz + point_num;
                    asc_y.reserve(siz);
                    for (const auto& [y, pid] : ps_asc_y[r - 1]) asc_y.emplace_back(y, ~pid);
                    std::inplace_merge(asc_y.begin(), asc_y.begin() + mid_siz, asc_y.end(), compare);
                    for (int t = 0; t < siz; ++t) {
                        const Key pkey = t ? asc_y[t - 1] : Key{ 0, 0 }, key = asc_y[t];
                        ynum += t and compare(pkey, key) and (pkey.second < 0) and (key.second >= 0);
                        const int i = key.second;
                        (i >= 0 ? es_comp_y[i] : ps_comp_y[~i]) = ynum;
                    }
                    ++ynum;
                }

                atcoder::fenwick_tree<T> ft(ynum);
                for (int j = 0, i = 2 * l; j < point_num; ++j) {
                    const auto &[px, pid] = ps_asc_x[r - 1][j];
                    for (; i < 2 * r; ++i) {
                        const auto &[ex, eid] = es_asc_x[i];
                        if (px < ex) break;
                        const T& w = _weights[eid >> 1];
                        ft.add(es_comp_y[eid & ~1], (eid & 1) ? -w : +w);
                        ft.add(es_comp_y[eid | 1], (eid & 1) ? +w : -w);
                    }
                    ans[pid] += ft.sum(0, ps_comp_y[pid] + 1);
                }
                for (Key &key : ps_asc_x[r - 1]) if (int d = _times[key.second] - r) ps_asc_x[r + floor_pow2(d) - 1].push_back(std::move(key));
                ps_asc_x[r - 1].clear(), ps_asc_x[r - 1].shrink_to_fit();
                for (Key &key : ps_asc_y[r - 1]) if (int d = _times[key.second] - r) ps_asc_y[r + floor_pow2(d) - 1].push_back(std::move(key));
                ps_asc_y[r - 1].clear(), ps_asc_y[r - 1].shrink_to_fit();
            }
            return ans;
        }
    private:
        std::vector<Rectangle> _rects{};
        std::vector<T> _weights{};
        std::vector<Point> _points{};
        std::vector<int> _times{};

        static int floor_pow2(int x) { return 1 << (31 - __builtin_clz(x)); }

        Index rect_x(int x_event_id) const { return _rects[x_event_id >> 1][0 + (x_event_id & 1)]; }
        Index rect_y(int y_event_id) const { return _rects[y_event_id >> 1][2 + (y_event_id & 1)]; }
        Index point_x(int point_id) const { return _points[point_id][0]; }
        Index point_y(int point_id) const { return _points[point_id][1]; }
    };
} // namespace suisen

#include <string>
#include <utility>

#include <unistd.h>

namespace io {
    namespace internal {
        template <typename T>
        struct is_container {
            template <typename T2>
            static auto test(T2 t) -> decltype(++t.begin() != t.end(), *t.begin(), std::true_type{});
            static std::false_type test(...);
        public:
            static constexpr bool value = decltype(test(std::declval<T>()))::value;
        };
        template <typename T>
        constexpr bool is_container_v = is_container<T>::value;

        template <typename T>
        using is_integral = std::disjunction<std::is_integral<T>, std::is_same<T, __int128_t>, std::is_same<T, __uint128_t>>;
        template <typename T>
        constexpr bool is_integral_v = is_integral<T>::value;

        struct IO {
            static constexpr int IBUF_SIZE = 1 << 25;
            static constexpr int OBUF_SIZE = 1 << 25;
            char ibuf[IBUF_SIZE], obuf[OBUF_SIZE];
            char* ibuf_ptr = ibuf, *obuf_ptr = obuf;
            char* ibuf_ptr_r = ibuf;

            ~IO() { flush(); }

            void load() {
                if (ibuf_ptr == ibuf + IBUF_SIZE) ibuf_ptr = ibuf;
                ibuf_ptr_r = ibuf_ptr + ::read(STDIN_FILENO, ibuf_ptr, IBUF_SIZE - (ibuf_ptr - ibuf));
            }
            char nextchar() {
                if (ibuf_ptr == ibuf_ptr_r) load();
                return *ibuf_ptr++;
            }

            void read(char& c) { do c = nextchar(); while (not isgraph(c)); }
            template <typename T, std::enable_if_t<is_integral_v<T>, std::nullptr_t> = nullptr>
            void read(T& x) {
                char c;
                read(c);
                if (c == '-') {
                    read<T>(x), x = -x;
                    return;
                }
                if (not isdigit(c)) throw - 1;
                x = 0;
                do x = x * 10 + (std::exchange(c, nextchar()) - '0'); while (isdigit(c));
            }
            template <typename T, std::enable_if_t<is_container_v<T>, std::nullptr_t> = nullptr>
            void read(T& x) { for (auto& e : x) read(e); }
            void read(std::string& x) {
                x.clear();
                char c;
                read(c);
                do x += std::exchange(c, nextchar()); while (isgraph(c));
            }

            void flush() {
                ssize_t wt = ::write(STDOUT_FILENO, obuf, obuf_ptr - obuf);
                if (wt != obuf_ptr - obuf) throw -1;
                obuf_ptr = obuf;
            }
            void write(char c) {
                if (obuf_ptr == obuf + OBUF_SIZE) {
                    ssize_t wt = ::write(STDOUT_FILENO, obuf, OBUF_SIZE);
                    if (wt != OBUF_SIZE) throw -1;
                    obuf_ptr = obuf;
                }
                *obuf_ptr++ = c;
            }
            template <typename T, std::enable_if_t<is_integral_v<T>, std::nullptr_t> = nullptr>
            void write(T x) {
                static char buf[50];
                if constexpr (std::is_signed_v<T>) if (x < 0) write('-'), x = -x;
                int i = 0;
                do buf[i++] = '0' + (x % 10), x /= 10; while (x);
                while (i--) write(buf[i]);
            }
            template <typename T, std::enable_if_t<is_container_v<T>, std::nullptr_t> = nullptr>
            void write(const T& x) {
                bool insert_delim = false;
                for (auto it = x.begin(); it != x.end(); ++it) {
                    if (std::exchange(insert_delim, true)) write(' ');
                    write(*it);
                }
            }
            void write(const std::string& x) { for (char c : x) write(c); }
        } io{};
    }

    template <typename ...Args>
    void read(Args &...args) { (internal::io.read(args), ...); }
    template <typename Head, typename ...Tails>
    void print(Head&& head, Tails &&...tails) { internal::io.write(head), ((internal::io.write(' '), internal::io.write(tails)), ...), internal::io.write('\n'); }
}

int main() {
    int n, q;
    io::read(n, q);

    suisen::OfflineRectangleAddPointGet<long long> processor;

    for (int i = 0; i < n; ++i) {
        int l, r, d, u, w;
        io::read(l, d, r, u, w);
        processor.add(l, r, d, u, w);
    }
    for (int i = 0; i < q; ++i) {
        int query_type;
        io::read(query_type);
        if (query_type == 0) {
            int l, r, d, u, w;
            io::read(l, d, r, u, w);
            processor.add(l, r, d, u, w);
        } else {
            int x, y;
            io::read(x, y);
            processor.get(x, y);
        }
    }
    for (long long ans : processor.solve()) {
        io::print(ans);
    }
}

