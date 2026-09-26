

#include<bits/stdc++.h>
using namespace std;

using ld = long double;
using ll = long long;
using u32 = unsigned int;
using u64 = unsigned long long;
using i128 = __int128;
using u128 = unsigned __int128;
using f128 = __float128;
 
 
#define pii pair<int,int>
#define pll pair<ll,ll>
 
#define all(x) (x).begin(),(x).end()
#define rall(x) (x).rbegin(),(x).rend()
#define ars(x) (x),(x+n)
 
#define TIME  (1.0 * clock() / CLOCKS_PER_SEC)
 
#define For(i,a,b) for (int i=(a); i<(b); i++)
#define rep(i,a) For(i,0,a)
#define rev(i,a,b) for (int i=(a); i>(b); i--)
#define FOR(i,a,b) for (int i=(a); i<=(b); i++)
#define REP(i,a) FOR(i,1,a)
#define REV(i,a,b) for (int i=(a); i>=(b); i--)
 
#define pb push_back
#define eb emplace_back
#define sz(x) ((int)(x).size())
#define mp make_pair
#define fi first
#define se second
#define FT ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
 
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

using vi=vector<int>;
using vll = vector<ll>;
template <class T>
using vc = vector<T>;
template <class T>
using vvc = vector<vc<T>>;
template <class T>
using vvvc = vector<vvc<T>>;
template <class T>
using vvvvc = vector<vvvc<T>>;
template <class T>
using vvvvvc = vector<vvvvc<T>>;
template <class T>
using pq = priority_queue<T>;
template <class T>
using pqg = priority_queue<T, vector<T>, greater<T>>;
 
#define vv(type, name, h, ...) \
  vector<vector<type>> name(h, vector<type>(__VA_ARGS__))
#define vvv(type, name, h, w, ...)   \
  vector<vector<vector<type>>> name( \
      h, vector<vector<type>>(w, vector<type>(__VA_ARGS__)))
#define vvvv(type, name, a, b, c, ...)       \
  vector<vector<vector<vector<type>>>> name( \
      a, vector<vector<vector<type>>>(       \
             b, vector<vector<type>>(c, vector<type>(__VA_ARGS__))))
 
/*
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<int,null_type,less<int>,rb_tree_tag,tree_order_statistics_node_update> ods;
- insert(x),erase(x)
- find_by_order(k): return iterator to the k-th smallest element
- order_of_key(x): the number of elements that are strictly smaller
*/

#define MIN(v) *min_element(all(v))
#define MAX(v) *max_element(all(v))
#define UNIQUE(x) sort(all(x)), x.erase(unique(all(x)), x.end()), x.shrink_to_fit()

template<typename It, typename T>
auto LB(It first, It last, const T& value) -> typename std::iterator_traits<It>::difference_type{return std::lower_bound(first, last, value) - first;}
template<typename It, typename T>
auto UB(It first, It last, const T& value) -> typename std::iterator_traits<It>::difference_type{return std::upper_bound(first, last, value) - first;}

template <typename T> int popcnt(T x) {return __builtin_popcountll(x);}
template <typename T> int topbit(T x) {return (x == 0 ? -1 : 63 - __builtin_clzll(x));}
template <typename T> int lowbit(T x) {return (x == 0 ? -1 : __builtin_ctzll(x));}

template<class T> T div_floor(T a, T b) { return a / b - ((a ^ b) < 0 && a % b); }
template<class T> T div_ceil(T a, T b) { return a / b + ((a ^ b) > 0 && a % b); }

template <typename T> bool chkmin(T &x,T y){return x>y?x=y,1:0;}
template <typename T> bool chkmax(T &x,T y){return x<y?x=y,1:0;}
 
template<class T> using pq = priority_queue<T>;
template<class T> using pqg = priority_queue<T, vector<T>, greater<T>>;

template<class t>
void myshuffle(vector<t>& a) {
    rep(i,sz(a)){
        uniform_int_distribution<int> dis(0, i);
        swap(a[i], a[dis(rng)]);
    }
}
template<class t>
vector<t> presum(const vector<t>&a){
    vector<t> s(sz(a)+1);
    rep(i,sz(a))s[i+1]=s[i]+a[i];
    return s;
}

void __print(int x) {cerr << x;}
void __print(long x) {cerr << x;}
void __print(long long x) {cerr << x;}
void __print(unsigned x) {cerr << x;}
void __print(unsigned long x) {cerr << x;}
void __print(unsigned long long x) {cerr << x;}
void __print(float x) {cerr << x;}
void __print(double x) {cerr << x;}
void __print(long double x) {cerr << x;}
void __print(char x) {cerr << '\'' << x << '\'';}
void __print(const char *x) {cerr << '\"' << x << '\"';}
void __print(const string &x) {cerr << '\"' << x << '\"';}
void __print(bool x) {cerr << (x ? "true" : "false");}
 
template<typename T, typename V>
void __print(const pair<T, V> &x) {cerr << '{'; __print(x.first); cerr << ", "; __print(x.second); cerr << '}';}
template<typename T>
void __print(const T &x) {int f = 0; cerr << '{'; for (auto it = std::begin(x); it != std::end(x); ++it)  cerr << (f++ ? ", " : ""), __print(*it); cerr << "}";}
void _print() {cerr << "]\n";}
template <typename T, typename... V>
void _print(T t, V... v) {__print(t); if (sizeof...(v)) cerr << ", "; _print(v...);}
 
void dbg_out() { cerr << endl; }
template<typename Head, typename... Tail> void dbg_out(Head H, Tail... T) { __print(H); if (sizeof...(T)) cerr << ", "; dbg_out(T...); }
#define dbg(...) cerr << "[" << #__VA_ARGS__ << "]:"; dbg_out(__VA_ARGS__);

//---------------for faster io-----------------------------------
#pragma once

#include <unistd.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cstring>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include <bit>
#include <cstdint>
#include <cassert>
#include <numeric>
#include <type_traits>

namespace yosupo {

namespace internal {

template <class T>
using is_signed_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value ||
                                  std::is_same<T, __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value ||
                                  std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value,
                              __uint128_t,
                              unsigned __int128>;

template <class T>
using is_integral =
    typename std::conditional<std::is_integral<T>::value ||
                                  internal::is_signed_int128<T>::value ||
                                  internal::is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using is_signed_int = typename std::conditional<(is_integral<T>::value &&
                                                 std::is_signed<T>::value) ||
                                                    is_signed_int128<T>::value,
                                                std::true_type,
                                                std::false_type>::type;

template <class T>
using is_unsigned_int =
    typename std::conditional<(is_integral<T>::value &&
                               std::is_unsigned<T>::value) ||
                                  is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using to_unsigned = typename std::conditional<
    is_signed_int128<T>::value,
    make_unsigned_int128<T>,
    typename std::conditional<std::is_signed<T>::value,
                              std::make_unsigned<T>,
                              std::common_type<T>>::type>::type;

template <class T>
using is_integral_t = std::enable_if_t<is_integral<T>::value>;

template <class T>
using is_signed_int_t = std::enable_if_t<is_signed_int<T>::value>;

template <class T>
using is_unsigned_int_t = std::enable_if_t<is_unsigned_int<T>::value>;

template <class T> using to_unsigned_t = typename to_unsigned<T>::type;

}  // namespace internal

}  // namespace yosupo

namespace yosupo {

struct Scanner {
  public:
    Scanner(const Scanner&) = delete;
    Scanner& operator=(const Scanner&) = delete;

    Scanner(FILE* fp) : fd(fileno(fp)) { line[0] = 127; }

    void read() {}
    template <class H, class... T> void read(H& h, T&... t) {
        bool f = read_single(h);
        assert(f);
        read(t...);
    }

    int read_unsafe() { return 0; }
    template <class H, class... T> int read_unsafe(H& h, T&... t) {
        bool f = read_single(h);
        if (!f) return 0;
        return 1 + read_unsafe(t...);
    }

    int close() { return ::close(fd); }

  private:
    static constexpr int SIZE = 1 << 15;

    int fd = -1;
    std::array<char, SIZE + 1> line;
    int st = 0, ed = 0;
    bool eof = false;

    bool read_single(std::string& ref) {
        if (!skip_space()) return false;
        ref = "";
        while (true) {
            char c = top();
            if (c <= ' ') break;
            ref += c;
            st++;
        }
        return true;
    }
    bool read_single(double& ref) {
        std::string s;
        if (!read_single(s)) return false;
        ref = std::stod(s);
        return true;
    }

    template <class T,
              std::enable_if_t<std::is_same<T, char>::value>* = nullptr>
    bool read_single(T& ref) {
        if (!skip_space<50>()) return false;
        ref = top();
        st++;
        return true;
    }

    template <class T,
              internal::is_signed_int_t<T>* = nullptr,
              std::enable_if_t<!std::is_same<T, char>::value>* = nullptr>
    bool read_single(T& sref) {
        using U = internal::to_unsigned_t<T>;
        if (!skip_space<50>()) return false;
        bool neg = false;
        if (line[st] == '-') {
            neg = true;
            st++;
        }
        U ref = 0;
        do {
            ref = 10 * ref + (line[st++] & 0x0f);
        } while (line[st] >= '0');
        sref = neg ? -ref : ref;
        return true;
    }
    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<!std::is_same<U, char>::value>* = nullptr>
    bool read_single(U& ref) {
        if (!skip_space<50>()) return false;
        ref = 0;
        do {
            ref = 10 * ref + (line[st++] & 0x0f);
        } while (line[st] >= '0');
        return true;
    }

    bool reread() {
        if (ed - st >= 50) return true;
        if (st > SIZE / 2) {
            std::memmove(line.data(), line.data() + st, ed - st);
            ed -= st;
            st = 0;
        }
        if (eof) return false;
        auto u = ::read(fd, line.data() + ed, SIZE - ed);
        if (u == 0) {
            eof = true;
            line[ed] = '\0';
            u = 1;
        }
        ed += int(u);
        line[ed] = char(127);
        return true;
    }

    char top() {
        if (st == ed) {
            bool f = reread();
            assert(f);
        }
        return line[st];
    }

    template <int TOKEN_LEN = 0> bool skip_space() {
        while (true) {
            while (line[st] <= ' ') st++;
            if (ed - st > TOKEN_LEN) return true;
            if (st > ed) st = ed;
            for (auto i = st; i < ed; i++) {
                if (line[i] <= ' ') return true;
            }
            if (!reread()) return false;
        }
    }
};

struct Printer {
  public:
    template <char sep = ' ', bool F = false> void write() {}
    template <char sep = ' ', bool F = false, class H, class... T>
    void write(const H& h, const T&... t) {
        if (F) write_single(sep);
        write_single(h);
        write<true>(t...);
    }
    template <char sep = ' ', class... T> void writeln(const T&... t) {
        write<sep>(t...);
        write_single('\n');
    }

    Printer(FILE* _fp) : fd(fileno(_fp)) {}
    ~Printer() { flush(); }

    int close() {
        flush();
        return ::close(fd);
    }

    void flush() {
        if (pos) {
            auto res = ::write(fd, line.data(), pos);
            assert(res != -1);
            pos = 0;
        }
    }

  private:
    static std::array<std::array<char, 2>, 100> small;
    static std::array<unsigned long long, 20> tens;

    static constexpr size_t SIZE = 1 << 15;
    int fd;
    std::array<char, SIZE> line;
    size_t pos = 0;
    std::stringstream ss;

    template <class T,
              std::enable_if_t<std::is_same<char, T>::value>* = nullptr>
    void write_single(const T& val) {
        if (pos == SIZE) flush();
        line[pos++] = val;
    }

    template <class T,
              internal::is_signed_int_t<T>* = nullptr,
              std::enable_if_t<!std::is_same<char, T>::value>* = nullptr>
    void write_single(const T& val) {
        using U = internal::to_unsigned_t<T>;
        if (val == 0) {
            write_single('0');
            return;
        }
        if (pos > SIZE - 50) flush();
        U uval = val;
        if (val < 0) {
            write_single('-');
            uval = -uval;
        }
        write_unsigned(uval);
    }

    template <class U, internal::is_unsigned_int_t<U>* = nullptr>
    void write_single(U uval) {
        if (uval == 0) {
            write_single('0');
            return;
        }
        if (pos > SIZE - 50) flush();

        write_unsigned(uval);
    }

    static int calc_len(uint64_t x) {
        int i = ((63 - std::countl_zero(x)) * 3 + 3) / 10;
        if (x < tens[i])
            return i;
        else
            return i + 1;
    }

    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<2 >= sizeof(U)>* = nullptr>
    void write_unsigned(U uval) {
        size_t len = calc_len(uval);
        pos += len;

        char* ptr = line.data() + pos;
        while (uval >= 100) {
            ptr -= 2;
            memcpy(ptr, small[uval % 100].data(), 2);
            uval /= 100;
        }
        if (uval >= 10) {
            memcpy(ptr - 2, small[uval].data(), 2);
        } else {
            *(ptr - 1) = char('0' + uval);
        }
    }

    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<4 == sizeof(U)>* = nullptr>
    void write_unsigned(U uval) {
        std::array<char, 8> buf;
        memcpy(buf.data() + 6, small[uval % 100].data(), 2);
        memcpy(buf.data() + 4, small[uval / 100 % 100].data(), 2);
        memcpy(buf.data() + 2, small[uval / 10000 % 100].data(), 2);
        memcpy(buf.data() + 0, small[uval / 1000000 % 100].data(), 2);

        if (uval >= 100000000) {
            if (uval >= 1000000000) {
                memcpy(line.data() + pos, small[uval / 100000000 % 100].data(),
                       2);
                pos += 2;
            } else {
                line[pos] = char('0' + uval / 100000000);
                pos++;
            }
            memcpy(line.data() + pos, buf.data(), 8);
            pos += 8;
        } else {
            size_t len = calc_len(uval);
            memcpy(line.data() + pos, buf.data() + (8 - len), len);
            pos += len;
        }
    }

    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<8 == sizeof(U)>* = nullptr>
    void write_unsigned(U uval) {
        size_t len = calc_len(uval);
        pos += len;

        char* ptr = line.data() + pos;
        while (uval >= 100) {
            ptr -= 2;
            memcpy(ptr, small[uval % 100].data(), 2);
            uval /= 100;
        }
        if (uval >= 10) {
            memcpy(ptr - 2, small[uval].data(), 2);
        } else {
            *(ptr - 1) = char('0' + uval);
        }
    }

    template <
        class U,
        std::enable_if_t<internal::is_unsigned_int128<U>::value>* = nullptr>
    void write_unsigned(U uval) {
        static std::array<char, 50> buf;
        size_t len = 0;
        while (uval > 0) {
            buf[len++] = char((uval % 10) + '0');
            uval /= 10;
        }
        std::reverse(buf.begin(), buf.begin() + len);
        memcpy(line.data() + pos, buf.data(), len);
        pos += len;
    }

    void write_single(const std::string& s) {
        for (char c : s) write_single(c);
    }
    void write_single(const char* s) {
        size_t len = strlen(s);
        for (size_t i = 0; i < len; i++) write_single(s[i]);
    }
    template <class T> void write_single(const std::vector<T>& val) {
        auto n = val.size();
        for (size_t i = 0; i < n; i++) {
            if (i) write_single(' ');
            write_single(val[i]);
        }
    }
};

std::array<std::array<char, 2>, 100> Printer::small = [] {
    std::array<std::array<char, 2>, 100> table;
    for (int i = 0; i <= 99; i++) {
        table[i][1] = char('0' + (i % 10));
        table[i][0] = char('0' + (i / 10 % 10));
    }
    return table;
}();
std::array<unsigned long long, 20> Printer::tens = [] {
    std::array<unsigned long long, 20> table;
    for (int i = 0; i < 20; i++) {
        table[i] = 1;
        for (int j = 0; j < i; j++) {
            table[i] *= 10;
        }
    }
    return table;
}();

}  // namespace yosupo
//sc.read(type) to enter input
//sc.write(type) to print out answer
//sc.writeln() to go break the line
using namespace yosupo;
Scanner sc(stdin);
Printer pr(stdout);
const int INF=1e9;
const ll INFI=1e15;
//----------Author: Nguyen Ho Nam,UIT, Saigon-----------------
template<typename T, bool directed, int MAXN, int MAXM>
struct cycle_detection {
    int n, m,cycle_len=0,ecnt=0;
    array<int, MAXN> head, parent_e, cycle_e; 
    array<int, directed?MAXM:2*MAXM> to, nxt, eid;
    array<uint8_t, MAXN>  vis;        
    array<T, MAXN> parent_v,cycle_v;   
    cycle_detection(int _n, int _m) : n(_n), m(_m) {
        head.fill(-1);
        vis .fill(0);
    }
    void add_edge(int u, int v, int id) {
        to [ecnt] = v;
        eid[ecnt] = id;
        nxt[ecnt] = head[u];
        head[u]   = ecnt++;
        if constexpr (!directed) {
            to [ecnt] = u;
            eid[ecnt] = id;
            nxt[ecnt] = head[v];
            head[v]   = ecnt++;
        }
    }
    bool find_cycle() {
        struct Frame { T v; int ei; };
        array<Frame, MAXN> stk;
        int top = 0;
        for (T start = 0; start < n; ++start) {
            if (vis[start]) continue;
            parent_v[start] = -1;
            parent_e[start] = -1;
            stk[top++] = {start, head[start]};
            while (top) {
                auto &fr = stk[top-1];
                T v = fr.v;
                if (vis[v] == 0) vis[v] = 1;
                int &ei = fr.ei;
                if (ei == -1) {
                    vis[v] = 2;
                    --top;
                    continue;
                }
                int ecur = ei; 
                ei = nxt[ei];
                T w = to[ecur];
                int id = eid[ecur];
                if constexpr (!directed) {
                    if (id == parent_e[v])  continue;  
                }
                if (vis[w] == 0) {
                    parent_v[w] = v;
                    parent_e[w] = id;
                    stk[top++]  = {w, head[w]};
                }
                else if (vis[w] == 1) {
                    int idx = 0;
                    cycle_v[idx] = w;
                    cycle_e[idx] = id;
                    ++idx;
                    T cur = v;
                    while (cur != w) {
                        cycle_v[idx] = cur;
                        cycle_e[idx] = parent_e[cur];
                        cur = parent_v[cur];
                        ++idx;
                    }
                    cycle_len = idx;
                    return true;
                }
            }
        }
        return false;
    }
    int length() const { return cycle_len; }
    const auto& vertices() const { return cycle_v; }
    const auto& edges()    const { return cycle_e; }
};
const int MAXN=500000;
const int MAXM=500000;
void solve()
{
    int n,m; sc.read(n,m);
    cycle_detection<int,false,MAXN,MAXM>cd(n,m);
    rep(i,m){
        int u,v; sc.read(u,v);
        cd.add_edge(u,v,i);
    }
    if(cd.find_cycle()){
        pr.writeln(cd.length());
        rep(i,cd.length()) pr.write(cd.vertices()[i],' ');
        pr.writeln();
        rep(i,cd.length()) pr.write(cd.edges()[i],' ');
        pr.writeln();
    }else{
        pr.writeln(-1);
    }
}

int main(){
FT;
int t=1; 
//sc.read(t); int tc=0;
while(t--)
{
//tc++;
//cout << "Case #" << tc << ": ";
solve();
// cerr << "Time elapsed: " << TIME << " s.\n";
}
}