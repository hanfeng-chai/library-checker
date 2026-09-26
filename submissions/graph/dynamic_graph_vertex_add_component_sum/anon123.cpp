#include <bits/stdc++.h>
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

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

const int N = 3e5;

struct dsu {
    vector<int> parent, sz;
    vector<long long> val;
    vector<pair<int, int>> weight;

    dsu(int n): parent(n), weight(n), sz(n, 1), val(n) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
            weight[i] = make_pair(1, 0);
        }
    }

    int& zz(int u) {
        if (parent[u] == u) return parent[u];
        while (weight[parent[u]].first <= weight[u].first) {
            sz[parent[u]] -= sz[u];
            val[parent[u]] -= val[u];
            parent[u] = parent[parent[u]];
        }
        return parent[u];
    }

    int find(int u, int w = 0) {
        while (weight[u].first <= w) u = zz(u);
        return u;
    }

    void disconnect(int v) {
        if (parent[v] == v) return;
        disconnect(parent[v]);
        sz[parent[v]] -= sz[v];
        val[parent[v]] -= val[v];
    }

    int connect(int v, int w = 0) {
        while (weight[v].first <= w) {
            sz[parent[v]] += sz[v];
            val[parent[v]] += val[v];
            v = parent[v];
        }
        return v;
    }

    void add_edge(int u, int v, pair<int, int> w) {
        disconnect(u);
        disconnect(v);
        while (u != v) {
            u = connect(u, w.first);
            v = connect(v, w.first);
            if (sz[u] < sz[v]) swap(u, v);
            swap(parent[v], u);
            swap(weight[v], w);
        }
        connect(u);
    }

    int max_edge(int u, int v) {
        if (find(u) != find(v)) return -1;
        while (true) {
            if (weight[u].first > weight[v].first) swap(u, v);
            if (parent[u] == v) break;
            u = parent[u];
        }
        return u;
    }

    void delete_edge(int v, int w) {
        while (parent[v] != v) {
            if (weight[v].first == w) {
                int u = v;
                while (parent[u] != u) {
                    u = parent[u];
                    sz[u] -= sz[v];
                    val[u] -= val[v];
                }
                parent[v] = v;
                weight[v] = {1, 0};
                return;
            }
            while (weight[parent[v]].first <= weight[v].first) {
                sz[parent[v]] -= sz[v];
                val[parent[v]] -= val[v];
                parent[v] = parent[parent[v]];
            }
            v = parent[v];
        }
    }

    void del(int u, int v, int w) {
        delete_edge(u, w);
        delete_edge(v, w);
    }

    int merge(int u, int v, pair<int, int> w) {
        if (u == v) return w.second;
        int p = max_edge(u, v);
        if (p == -1) {
            add_edge(u, v, w);
            return -1;
        } else if (weight[p].first > w.first) {
            int res = weight[p].second;
            delete_edge(p, weight[p].first);
            add_edge(u, v, w);
            return res;
        }
        return w.second;
    }

    void modify(int u, int v) {
        while (true) {
            val[u] += v;
            if (parent[u] == u) break;
            u = parent[u];
        }
    }

    long long get_val(int u) {
        return val[find(u)];
    }
};

int t[N], u[N], v[N];
pair<int, int> w[N];
unordered_map<long long, int, custom_hash> mp;

int main() {
    int n, q;
    fin >> n >> q;
    dsu D(n);
    for (int i = 0; i < n; ++i) {
        fin >> D.val[i];
    }
    for (int i = 0; i < q; ++i) w[i] = {-q, i};
    for (int i = 0; i < q; ++i) {
        fin >> t[i];
        switch (t[i]) {
            case 0: {
                fin >> u[i] >> v[i];
                if (u[i] > v[i]) swap(u[i], v[i]);
                mp[(long long)u[i] * n + v[i]] = i;
                break;
            }
            case 1: {
                fin >> u[i] >> v[i];
                if (u[i] > v[i]) swap(u[i], v[i]);
                w[mp[(long long)u[i] * n + v[i]]].first = w[i].first = -i;
                break;
            }
            case 2: {
                fin >> u[i] >> v[i];
                break;
            }
            case 3: {
                fin >> u[i];
                break;
            }
        }
    }
    for (int i = 0; i < q; ++i) {
        switch (t[i]) {
            case 0: {
                D.merge(u[i], v[i], w[i]);
                break;
            }
            case 1: {
                D.del(u[i], v[i], w[i].first);
                break;
            }
            case 2: {
                D.modify(u[i], v[i]);
                break;
            }
            case 3: {
                fout << D.get_val(u[i]) << '\n';
                break;
            }
        }
    }
}