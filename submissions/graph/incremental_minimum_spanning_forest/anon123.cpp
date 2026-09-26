#include "bits/stdc++.h"
using namespace std;

using int64 = long long;

namespace FastIO {
    class Scanner {
        static constexpr int buf_size = (1 << 19);
        static constexpr int integer_size = 20;
        static constexpr int string_size = (int)5e5 + 5; // default
        char buf[buf_size] = {};
        char *cur = buf, *ed = buf;

    public:
        Scanner() {}

        template <class T>
        inline Scanner& operator>>(T& val) {
            read(val);
            return *this;
        }

    private:
        inline void reload() {
            size_t len = ed - cur;
            memmove(buf, cur, len);
            char* tmp = buf + len;
            ed = tmp + fread(tmp, 1, buf_size - len, stdin);
            *ed = 0;
            cur = buf;
        }

        inline void skip_space() {
            while (true) {
                if (cur == ed) reload();
                while (*cur == ' ' || *cur == '\n') ++cur;
                if (__builtin_expect(cur != ed, 1)) return;
            }
        }

        template <class T, std::enable_if_t<std::is_same<T, int>::value, int> = 0>
        inline void read(T& num) {
            skip_space();
            if (cur + integer_size >= ed) reload();
            bool neg = false;
            num = 0;
            if (*cur == '-') neg = true, ++cur;
            while (*cur >= '0') num = num * 10 + (*cur ^ 48), ++cur;
            if (neg) num = -num;
        }

        template <class T, std::enable_if_t<std::is_same<T, int64>::value, int> = 0>
        inline void read(T& num) {
            skip_space();
            if (cur + integer_size >= ed) reload();
            bool neg = false;
            num = 0;
            if (*cur == '-') neg = true, ++cur;
            while (*cur >= '0') num = num * 10 + (*cur ^ 48), ++cur;
            if (neg) num = -num;
        }

        template <class T,
                std::enable_if_t<std::is_same<T, std::string>::value, int> = 0>
        inline void read(T& str) {
            skip_space();
            if (cur + str.size() >= ed) reload();
            auto it = cur;
            while (!(*cur == ' ' || *cur == '\n')) ++cur;
            str = std::string(it, cur);
        }

        template <class T, std::enable_if_t<std::is_same<T, char>::value, int> = 0>
        inline void read(T& c) {
            skip_space();
            if (cur + 1 >= ed) reload();
            c = *cur, ++cur;
        }

        template <class T, std::enable_if_t<std::is_same<T, double>::value, int> = 0>
        inline void read(T& num) {
            skip_space();
            if (cur + integer_size >= ed) reload();
            bool neg = false;
            num = 0;
            if (*cur == '-') neg = true, ++cur;
            while (*cur >= '0' && *cur <= '9') num = num * 10 + (*cur ^ 48), ++cur;
            if (*cur != '.') return;
            ++cur;
            T base = 0.1;
            while (*cur >= '0' && *cur <= '9') {
                num += base * (*cur ^ 48);
                ++cur;
                base *= 0.1;
            }
            if (neg) num = -num;
        }

        template <class T,
                std::enable_if_t<std::is_same<T, long double>::value, int> = 0>
        inline void read(T& num) {
            skip_space();
            if (cur + integer_size >= ed) reload();
            bool neg = false;
            num = 0;
            if (*cur == '-') neg = true, ++cur;
            while (*cur >= '0' && *cur <= '9') num = num * 10 + (*cur ^ 48), ++cur;
            if (*cur != '.') return;
            ++cur;
            T base = 0.1;
            while (*cur >= '0' && *cur <= '9') {
                num += base * (*cur ^ 48);
                ++cur;
                base *= 0.1;
            }
            if (neg) num = -num;
        }

        template <class T>
        inline void read(std::vector<T>& vec) {
            for (T& e : vec) read(e);
        }

        template <class T, class U>
        inline void read(std::pair<T, U>& p) {
            read(p.first, p.second);
        }

        template <class Tuple, std::size_t... Is>
        inline void tuple_scan(Tuple& tp, std::index_sequence<Is...>) {
            (read(std::get<Is>(tp)), ...);
        }

        template <class... Args>
        inline void read(std::tuple<Args...>& tp) {
            tuple_scan(tp, std::index_sequence_for<Args...>{});
        }

        inline void read() {}

        template <class Head, class... Tail>
        inline void read(Head&& head, Tail&&... tail) {
            read(head);
            read(std::forward<Tail>(tail)...);
        }
    };

    class Printer {
        static constexpr int buf_size = (1 << 18);
        static constexpr int integer_size = 20;
        static constexpr int string_size = (1 << 6);
        static constexpr int margin = 1;
        static constexpr int n = 10000;
        char buf[buf_size + margin] = {};
        char table[n * 4] = {};
        char* cur = buf;

    public:
        constexpr Printer() { build(); }

        ~Printer() { flush(); }

        template <class T>
        inline Printer& operator<<(T val) {
            write(val);
            return *this;
        }

        template<class T>
        inline void println(T val) {
            write(val);
            write('\n');
        }

    private:
        constexpr void build() {
            for (int i = 0; i < 10000; ++i) {
                int tmp = i;
                for (int j = 3; j >= 0; --j) {
                    table[i * 4 + j] = tmp % 10 + '0';
                    tmp /= 10;
                }
            }
        }

        inline void flush() {
            fwrite(buf, 1, cur - buf, stdout);
            cur = buf;
        }

        template <class T, std::enable_if_t<std::is_same<T, int>::value, int> = 0>
        inline int get_digit(T n) {
            if (n >= (int)1e5) {
                if (n >= (int)1e8) return 9;
                if (n >= (int)1e7) return 8;
                if (n >= (int)1e6) return 7;
                return 6;
            } else {
                if (n >= (int)1e4) return 5;
                if (n >= (int)1e3) return 4;
                if (n >= (int)1e2) return 3;
                if (n >= (int)1e1) return 2;
                return 1;
            }
        }

        template <class T, std::enable_if_t<std::is_same<T, int64>::value, int> = 0>
        inline int get_digit(T n) {
            if (n >= (int64)1e10) {
                if (n >= (int64)1e14) {
                    if (n >= (int64)1e18) return 19;
                    if (n >= (int64)1e17) return 18;
                    if (n >= (int64)1e16) return 17;
                    if (n >= (int64)1e15) return 16;
                    return 15;
                } else {
                    if (n >= (int64)1e14) return 15;
                    if (n >= (int64)1e13) return 14;
                    if (n >= (int64)1e12) return 13;
                    if (n >= (int64)1e11) return 12;
                    return 11;
                }
            } else {
                if (n >= (int64)1e5) {
                    if (n >= (int64)1e9) return 10;
                    if (n >= (int64)1e8) return 9;
                    if (n >= (int64)1e7) return 8;
                    if (n >= (int64)1e6) return 7;
                    return 6;
                } else {
                    if (n >= (int64)1e4) return 5;
                    if (n >= (int64)1e3) return 4;
                    if (n >= (int64)1e2) return 3;
                    if (n >= (int64)1e1) return 2;
                    return 1;
                }
            }
        }

        template <class T, std::enable_if_t<std::is_same<T, int>::value, int> = 0>
        inline void write(T num) {
            if (__builtin_expect(cur + integer_size >= buf + buf_size, 0)) flush();
            if (num == 0) {
                write('0');
                return;
            }
            if (num < 0) {
                write('-');
                num = -num;
            }
            int len = get_digit(num);
            int digits = len;
            while (num >= 10000) {
                memcpy(cur + len - 4, table + (num % 10000) * 4, 4);
                num /= 10000;
                len -= 4;
            }
            memcpy(cur, table + num * 4 + (4 - len), len);
            cur += digits;
        }

        template <class T, std::enable_if_t<std::is_same<T, int64>::value, int> = 0>
        inline void write(T num) {
            if (__builtin_expect(cur + integer_size >= buf + buf_size, 0)) flush();
            if (num == 0) {
                write('0');
                return;
            }
            if (num < 0) {
                write('-');
                num = -num;
            }
            int len = get_digit(num);
            int digits = len;
            while (num >= 10000) {
                memcpy(cur + len - 4, table + (num % 10000) * 4, 4);
                num /= 10000;
                len -= 4;
            }
            memcpy(cur, table + num * 4 + (4 - len), len);
            cur += digits;
        }

        template <class T, std::enable_if_t<std::is_same<T, char>::value, int> = 0>
        inline void write(T c) {
            if (__builtin_expect(cur + 1 >= buf + buf_size, 0)) flush();
            *cur = c;
            ++cur;
        }

        template <class T,
                std::enable_if_t<std::is_same<T, std::string>::value, int> = 0>
        inline void write(T str) {
            if (__builtin_expect(cur + str.size() >= buf + buf_size, 0)) flush();
            for (char c : str) write(c);
        }

        template <class T,
                std::enable_if_t<std::is_same<T, const char*>::value, int> = 0>
        inline void write(T str) {
            if (__builtin_expect(cur + string_size >= buf + buf_size, 0)) flush();
            for (int i = 0; str[i]; ++i) write(str[i]);
        }
    };
}  // namespace FastIO

FastIO::Scanner fin;
FastIO::Printer fout;

const int inf = 2e9;
const int N = 5e5 + 1;
const float alpha = 1.5;
int p[N], s[N];
pair<int, int> t[N];

inline void Rotate(int x, int y) {
    if (t[x] >= t[y]) {
        s[y] -= s[x];
        p[x] = p[y];
    } else {
        s[y] -= s[x];
        s[x] += s[y];
        std::swap(t[x], t[y]);
        p[x] = p[y];
        p[y] = x;
    }
}

inline void UpwardMaintain(int x) {
    while (p[x] != -1) {
        int y = p[x];
        if (s[x] * alpha <= s[y]) {
            x = y;
        } else {
            Rotate(x, y);
        }
    }
}

pair<int, int> Add(pair<int, int> ts, int u, int v) {
    if (u == v) return ts;
    // UpwardMaintain(u);
    // UpwardMaintain(v);
    int x = u, y = v;
    pair<int, int> max_t = {-inf, -1};
    int max_x = -1;
    for (;;) {
        if (x == y) break;
        if (s[x] > s[y]) std::swap(x, y);
        if (t[x].first == inf) break;
        if (t[x].first > max_t.first) {
            max_t = t[x];
            max_x = x;
        }
        x = p[x];
    }
    pair<int, int> res = {-1, -1};
    if (x == y) {
        if (ts >= max_t) return ts;
        res = max_t;
        x = max_x;
        for (y = p[x]; y != -1; y = p[y]) {
            s[y] -= s[x];
        }
        p[x] = -1;
        t[x] = {inf, -1};
    }
    x = u, y = v;
    pair<int, int> w = ts;
    int dx = 0, dy = 0;
    while (x != -1 && y != -1) {
        if (w >= t[x]) {
            s[p[x]] += dx;
            x = p[x];
        } else if (w >= t[y]) {
            s[p[y]] += dy;
            y = p[y];
        } else {
            if (s[x] > s[y]) {
                std::swap(x, y);
                std::swap(dx, dy);
            }
            dx -= s[x];
            dy += s[x];
            s[y] += s[x];
            int px_old = p[x];
            auto t_old = t[x];
            p[x] = y;
            t[x] = w;
            w = t_old;
            if (px_old != -1) s[px_old] += dx;
            x = px_old;
        }
    }
    if (y != -1) {
        y = p[y];
        while (y != -1) {
            s[y] += dy;
            y = p[y];
        }
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);

    int n, m;
    fin >> n >> m;
    for (int i = 0; i < n; i++) p[i] = -1;
    for (int i = 0; i < n; i++) t[i] = {inf, -1};
    for (int i = 0; i < n; i++) s[i] = 1;

    for (int i = 0; i < m; i++) {
        int u, v, w;
        fin >> u >> v >> w;
        pair<int, int> res = Add(make_pair(w, i), u, v);
        if (res.first == -1) {
            fout << "-1 ";
        } else {
            fout << res.second << " ";
        }
    }
}