#include<bits/stdc++.h>
#define all(x) begin(x), end(x)
#define ff first
#define ss second
#define O_O
using namespace std;
template <typename T>
using bstring = basic_string<T>;
template <typename T>
using matrix = vector<vector<T>>;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long long ll;
typedef pair<int,int> pii;
typedef pair<ll,ll> pll;
typedef double dbl;
typedef long double dbll; 
const ll INFL = 4e18+25;
const int INF = 1e9+42;
const double EPS = 1e-7;
const int MOD = (1<<23)*17*7 + 1; // 998244353
const int RANDOM = chrono::high_resolution_clock::now().time_since_epoch().count();
const int MAXN = 1e6+1;
mt19937 rng(RANDOM);

using int64 = ll;
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

struct IncrementalMST{
    vector<int> par, sz, prio;
    vector<pii> parw;

    IncrementalMST(int n) : par(n), sz(n,1), prio(n), parw(n,{INF,INF}){
        iota(all(prio),0);
        par = prio;
        shuffle(all(prio),rng);
    }
    private:

    int find(int v, pii w = {INF-1,INF}){
        while(parw[v] <= w){
            while(parw[v] > parw[par[v]]){
                // sz[par[v]]-=sz[v];
                par[v] = par[par[v]];
            }
            v = par[v];
        }
        return v;
    }

    void disconnect(int v){
        // if(par[v] == v)
        //     return;
        // disconnect(par[v]);
        // sz[par[v]]-=sz[v];
    }

    int connect(int v, pii w = {INF-1, INF}){
        while(parw[v] <= w){
            // sz[par[v]]+=sz[v];
            v = par[v];
        }
        return v;
    }

    void consider_edge(int a, int b, pii w){
        disconnect(a), disconnect(b);

        while(a != b){
            a = connect(a,w);
            b = connect(b,w);
            if(prio[a] > prio[b])
                swap(a,b);
            swap(par[a],b);
            swap(parw[a],w);
        }
        
        // connect(a);
    }

    public:

    /**
     * Finds maximum edge in the path from a to b
     * @return weight of maximum edge from a to b (or {INF,-1} if
     * they are disconnected)
     */
    pii max_edge(int a, int b){
        int ra = find(a), rb = find(b);
        if(ra != rb)
            return {INF,-1};

        if(parw[a] > parw[b])
            swap(a,b);
        while(par[a] != b){
            a = par[a];
            if(parw[a] > parw[b])
                swap(a,b);
        }
        return parw[a];
    }

    /**
     * Deletes maximum edge of the path from a to b
     * from the MST
     * @return weight of the edge removed from the MST (or {INF,-1} if
     * they are disconnected)
     */
    pii delete_maximum(int a, int b){
        int ra = find(a), rb = find(b);
        if(ra != rb)
            return {INF,-1};

        if(parw[a] > parw[b])
            swap(a,b);
        while(par[a] != b){
            a = par[a];
            if(parw[a] > parw[b])
                swap(a,b);
        }

        b = a;
        while(par[b] != b){
            // sz[par[b]]-=sz[a];
            b = par[b];
        }
        par[a] = a;
        pii ret = {INF,INF};
        swap(parw[a],ret);
        return ret;
    }

    /**
     * Adds edge between a and b with weight w to the graph.
     * @return weight of the edge removed from the MST (or {INF,-1} if
     * there was none)
     */
    pii add_edge(int a, int b, pii w){
        if(a == b)
            return {INF,-1};
        
        pii ret = delete_maximum(a,b);
        if(ret <= w)
            swap(w,ret);
        
        consider_edge(a,b,w);

        return ret;
    }
};


int main(){
    
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    fin >> n >> m;

    IncrementalMST uf(n);

    for(int i = 0; i < m; i++){
        int a, b, w;
        fin >> a >> b >> w;
        if(a == b){
            fout << i << ' ';
        } else fout << uf.add_edge(a,b,{w,i}).ss << ' ';
    }
    fout << '\n';
    
    return 0;

}