#ifdef LOCAL
#include <bits/Lstdc++.h>
#else
#include <bits/stdc++.h>
struct fast_io{fast_io(){std::ios::sync_with_stdio(0); std::cin.tie(0);}} _fast_io_ins;
#endif
using namespace std;

using uint = unsigned;
using ull = unsigned long long;
using ll = long long;
using ld = long double;
#define el '\n'
#define all(x) begin(x), end(x)
#define elif else if

struct FIstream{
    #ifndef LOCAL
    #define fread fread_unlocked
    #endif
    
    static constexpr unsigned SIZ = 1 << 20;
    char buf[SIZ], *p1 = buf, *p2 = buf;
    
    inline char _getchar(){
        if(p1 == p2){
            p2 = (p1 = buf) + fread(buf, 1, SIZ, stdin);
            if(p1 == p2) [[unlikely]] assert(0&&"EOF");
        }
        return *p1++;
    }
    
    inline char ignore_space(){
        char c;
        while((c = _getchar()) <= 0x20);
        return c;
    }
    
    template<typename T>
    inline void _read(T& res){
        T x = 0, f = 1;
        char c = ignore_space();
        if(c == '-'){
            f = -1;
            c = _getchar();
        }
        while('0' <= c && c <= '9'){
            x = x*10 + (c-'0');
            c = _getchar();
        }
        res = x*f;
    }
    
    template<typename T>
    inline FIstream& operator>>(T& x){_read(x); return *this;}
    inline FIstream& operator>>(char& x){x = ignore_space(); return *this;}
    #ifdef _GLIBCXX_STRING
    inline FIstream& operator>>(string& x){
        string().swap(x);
        char c = ignore_space();
        while(c > 0x20){
            x.push_back(c);
            c = _getchar();
        }
        return *this;
    }
    #endif
    #ifndef LOCAL
    #undef fread
    #endif
} _cin;
#define cin _cin
#define istream FIstream

struct FOstream_Pre{
    char num[10000][4];
    constexpr FOstream_Pre():num(){
        for(int i = 0; i < 10000; i++){
            int x = i;
            for(int j = 3; j >= 0; j--){
                num[i][j] = x%10 + '0';
                x /= 10;
            }
        }
    }
} constexpr _FOstream_pre;

struct FOstream{
    #ifndef LOCAL
    #define fwrite fwrite_unlocked
    #endif
    
    static constexpr unsigned SIZ = 1 << 21;
    char buf[SIZ], *p1 = buf, *p2 = buf+SIZ;
    
    inline void _write(){
        fwrite(buf, 1, p1-buf, stdout);
        p1 = buf;
    }
    
    inline void _putchar(char c){
        if(p1 == p2) [[unlikely]] {
            _write();
        }
        *p1++ = c;
    }
    
    template<typename T>
    void _write_i(T x){
        constexpr int DIGIT_SIZ = 40;
        static_assert(DIGIT_SIZ <= SIZ);
        char num[DIGIT_SIZ], *idxp = num+DIGIT_SIZ;
        if(x < 0){
            _putchar('-');
            x = -x;
        }
        if(p2 - p1 < DIGIT_SIZ) _write();
        while(x >= 10000){
            idxp -= 4;
            memcpy(idxp, _FOstream_pre.num[size_t(x%10000)], 4);
            x /= 10000;
        }
        if(x >= 1000){
            memcpy(p1, _FOstream_pre.num[size_t(x)], 4);
            p1 += 4;
        } else if(x >= 100){
            memcpy(p1, _FOstream_pre.num[size_t(x)]+1, 3);
            p1 += 3;
        } else if(x >= 10){
            memcpy(p1, _FOstream_pre.num[size_t(x)]+2, 2);
            p1 += 2;
        } else *p1++ = char(x)+'0';
        memcpy(p1, idxp, num+DIGIT_SIZ - idxp);
        p1 += num+DIGIT_SIZ - idxp;
    }
    
    template<typename T>
    FOstream &operator<<(const T &x) {_write_i(x); return *this;}
    FOstream &operator<<(char x) {_putchar(x); return *this;}
    FOstream &operator<<(const char *x) {
        while(*x) _putchar(*x++);
        return *this;
    }
    FOstream &operator<<(char *x) {return *this << const_cast<const char*>(x);}
    FOstream &operator<<(double x) {
        if(isnan(x)) [[unlikely]] return *this << "nan";
        char _b[70];
        snprintf(_b, sizeof(_b), "%.*f", 15, x);
        return *this << const_cast<const char*>(_b);
    }
    FOstream &operator<<(long double x) {
        if(isnan(x)) [[unlikely]] return *this << "nan";
        char _b[330];
        snprintf(_b, sizeof(_b), "%.*Lf", 15, x);
        return *this << const_cast<const char*>(_b);
    }
    #ifdef _GLIBCXX_STRING
    FOstream& operator<<(const string& x){
        for(char i : x) _putchar(i);
        return *this;
    }
    #endif
    
    ~FOstream(){
        if(p1 != buf){
            fwrite(buf, 1, p1 - buf, stdout);
        }
    }
    
    #ifndef LOCAL
    #undef fwrite
    #endif
} _cout;
#define cout _cout
#define ostream FOstream

constexpr long long LLINF = (1ll<<62)-1; constexpr int INF = (1<<30)-1;
template<typename T, typename U> istream& operator>>(istream& ist, pair<T, U>& p) {ist >> p.first >> p.second; return ist;} template<typename T> istream& operator>>(istream& ist, vector<T>& v) {for(T& i : v) ist >> i; return ist;} void read_d_graph(vector<vector<pair<long long, int>>>& v, int m, int num = -1); void read_d_graph(vector<vector<int>>& v, int m, int num = -1); void read_ud_graph(vector<vector<pair<long long, int>>>& v, int m, int num = -1); void read_ud_graph(vector<vector<int>>& v, int m, int num = -1); template<typename T> void read_multi() {} template<typename T, typename... U> void read_multi(int n, vector<T>& v, U&&... args) {if(n < ssize(v)) {cin >> v[n]; read_multi(args..., n+1, v);}} template<typename T, typename... U> void read_multi(vector<T>& v, U&&... args) {read_multi(args..., 0, v);} template<typename T = string> T input() {T res; cin >> res; return res;} template<typename T, typename U> ostream& operator<<(ostream& ost, const pair<T, U> p) {ost << '{' << p.first << ' ' << p.second << '}'; return ost;} template<typename T> ostream& operator<<(ostream& ost, const vector<T>& v) {for(int i = 0; i < ssize(v); i++) {ost << (i ? " " : "") << v[i];} return ost;} template<typename T> ostream& operator<<(ostream& ost, const vector<vector<T>>& v) {for(int i = 0; i < ssize(v); i++) {ost << (i ? "\n" : "") << v[i];} return ost;}
void add_each(...) {} template<typename T, typename... U> void add_each(long long n, vector<T>& v, U&... args); template<typename T, typename U> inline bool chmin(T& a, U b) {if(a > b){a = b; return true;} return false;} template<typename T, typename U> inline bool chmax(T& a, U b) {if(a < b){a = b; return true;} return false;} template<typename T> inline T minv(const vector<T>& v) {return *min_element(v.begin(), v.end());} template<typename T> inline T maxv(const vector<T>& v) {return *max_element(v.begin(), v.end());} long long power(long long val, long long num, long long mod = LLONG_MAX);

#if __has_include(<atcoder/modint>)
#include <atcoder/modint>
using namespace atcoder;
using mint9 = modint998244353;
using mint1 = modint1000000007;
ostream& operator<<(ostream& ost, const mint1& x) {ost << x.val(); return ost;} ostream& operator<<(ostream& ost, const mint9& x) {ost << x.val(); return ost;}
#endif

//*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*/
//*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*/

template<typename T>
struct graph {
    vector<T> G;
    vector<int> idx;
    struct ref_t {
        vector<T>::iterator begin_, end_;
        auto begin() const noexcept {return begin_;}
        auto end() const noexcept {return end_;}
        auto size() const noexcept {return end_ - begin_;}
        T operator[](int p) const {return begin_[p];}
    };
    
    void build(int n, const auto &E){
        G.resize(E.size());
        idx.resize(n+1);
        for(auto &[u, v] : E){
            idx[u+1]++;
        }
        for(int i = 1; i <= n; i++) idx[i] += idx[i-1];
        auto C = idx;
        for(auto &[u, v] : E){
            G[C[u]++] = v;
        }
    }
    
    ref_t operator[](int p) {return {G.begin()+idx[p], G.begin()+idx[p+1]};}
};

auto cycle_detection = [](int n, const auto &e)->pair<int, vector<int>> {
    int m = e.size()*2;
    vector<pair<int, array<int, 2>>> E;
    E.reserve(m);
    for(auto &[u, v] : e){
        const int siz = E.size()/2;
        E.push_back({u, {v, siz}});
        E.push_back({v, {u, siz}});
    }
    graph<array<int, 2>> G;
    G.build(n, E);
    
    vector<int> st(1, -1); st.reserve(n+1);
    vector<int> ans; ans.reserve(n);
    vector<int> state(n), idx(n), par(n, -1), done(n);
    int pos, ret = -1, beg = -1;
    for(int start = 0; start < n; start++){
        if(state[start] != 0) continue;
        pos = start;
        while(pos != -1){
            auto g = G[pos];
            if(done[pos]){
                done[pos] = false;
                if(ret >= 0){
                    ans.push_back(st.back());
                    st.pop_back();
                    if(ret == pos) ret = -2;
                    pos = par[pos];
                    continue;
                } else if(ret == -2){
                    st.pop_back();
                    pos = par[pos];
                    continue;
                } else st.pop_back();
            }
            if(idx[pos] == (int)g.size()){
                state[pos] = 2;
                pos = par[pos];
                ret = -1;
            } else {
                state[pos] = 1;
                auto [nex, i] = g[idx[pos]];
                idx[pos]++;
                if(i == st.back()) continue;
                if(state[nex] == 2) continue;
                if(state[nex] == 1) {
                    ans.push_back(i);
                    beg = nex;
                    ret = nex;
                    pos = par[pos];
                    continue;
                }
                done[pos] = true;
                st.push_back(i);
                par[nex] = pos;
                pos = nex;
            }
        }
        
        if(ret != -1){
            reverse(all(ans));
            return {beg, ans};
        }
    }
    
    return {-1, {}};
};

signed main(){
    
    int n, m;
    cin >> n >> m;
    vector<array<int, 2>> E(m);
    for(int i = 0; i < m; i++) cin >> E[i][0] >> E[i][1];
    
    auto [beg, ans] = cycle_detection(n, E);
    
    if(beg == -1){
        cout << -1 << el;
    } else {
        cout << ans.size() << el;
        for(auto i : ans){
            cout << beg << ' ';
            beg = E[i][0]+E[i][1]-beg;
        }
        cout << el;
        cout << ans << el;
    }
}
