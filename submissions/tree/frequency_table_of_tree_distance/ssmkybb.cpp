#ifdef _GLIBCXX_DEBUG
#include <bits/DBGstdc++.h>
#else
#include <bits/stdc++.h>
struct fast_io{fast_io(){std::ios::sync_with_stdio(0); std::cin.tie(0);}} _fast_io_ins;
#endif
using namespace std; using uint = unsigned; using ll = long long; using ull = unsigned long long; using ld = long double;
#define all(x) begin(x), end(x)
#define elif else if

struct FIstream{ static constexpr unsigned SIZ = 1 << 17; char buf[SIZ], *p1 = buf, *p2 = buf; inline char _getchar(){ if(p1 == p2){ p2 = (p1 = buf) + fread(buf, 1, SIZ, stdin); if(p1 == p2) [[unlikely]] assert(0&&"EOF"); } return *p1++; } inline char ignore_space(){ char c; while((c = _getchar()) <= 0x20); return c; } template<typename T> inline void _read(T& res){ T x = 0, f = 1; char c = ignore_space(); if(c == '-'){ f = -1; c = _getchar(); } while('0' <= c && c <= '9'){ x = x*10 + (c-'0'); c = _getchar(); } res = x*f; } template<typename T> inline FIstream& operator>>(T& x){_read(x); return *this;} inline FIstream& operator>>(char& x){x = ignore_space(); return *this;} inline FIstream& operator>>(string& x){ string().swap(x); char c = ignore_space(); while(c > 0x20){ x.push_back(c); c = _getchar(); } return *this; } } _cin; struct FOstream_Pre{ char num[10000][4]; constexpr FOstream_Pre():num(){ for(int i = 0; i < 10000; i++){ int x = i; for(int j = 3; j >= 0; j--){ num[i][j] = x%10 + '0'; x /= 10; } } } } constexpr _FOstream_pre; struct FOstream{ static constexpr unsigned SIZ = 1 << 17; char buf[SIZ], *p1 = buf, *p2 = buf+SIZ; inline void _write(){ fwrite(buf, 1, p1-buf, stdout); p1 = buf; } inline void _putchar(char c){ if(p1 == p2) [[unlikely]] { _write(); } *p1++ = c; } template<typename T> void _write_i(T x){ constexpr int DIGIT_SIZ = 40; static_assert(DIGIT_SIZ <= SIZ); char num[DIGIT_SIZ], *idxp = num+DIGIT_SIZ; if(x < 0){ _putchar('-'); x = -x; } if(p2 - p1 < DIGIT_SIZ) _write(); while(x >= 10000){ idxp -= 4; memcpy(idxp, _FOstream_pre.num[size_t(x%10000)], 4); x /= 10000; } if(x >= 1000){ memcpy(p1, _FOstream_pre.num[size_t(x)], 4); p1 += 4; } else if(x >= 100){ memcpy(p1, _FOstream_pre.num[size_t(x)]+1, 3); p1 += 3; } else if(x >= 10){ memcpy(p1, _FOstream_pre.num[size_t(x)]+2, 2); p1 += 2; } else *p1++ = char(x)+'0'; memcpy(p1, idxp, num+DIGIT_SIZ - idxp); p1 += num+DIGIT_SIZ - idxp; } template<typename T> FOstream &operator<<(const T &x) {_write_i(x); return *this;} FOstream &operator<<(char x) {_putchar(x); return *this;} FOstream &operator<<(const char *x) { while(*x) _putchar(*x++); return *this; } FOstream &operator<<(char *x) {return *this << const_cast<const char*>(x);} FOstream &operator<<(double x) { if(isnan(x)) [[unlikely]] return *this << "nan"; char _b[70]; snprintf(_b, sizeof(_b), "%.*f", 15, x); return *this << const_cast<const char*>(_b); } FOstream &operator<<(long double x) { if(isnan(x)) [[unlikely]] return *this << "nan"; char _b[330]; snprintf(_b, sizeof(_b), "%.*Lf", 15, x); return *this << const_cast<const char*>(_b); } FOstream& operator<<(const string& x){ for(char i : x) _putchar(i); return *this; } ~FOstream(){ if(p1 != buf){ fwrite(buf, 1, p1 - buf, stdout); } } } _cout;
#define cin _cin
#define istream FIstream
#define cout _cout
#define ostream FOstream

constexpr long long LLINF = (1ll<<62)-1; constexpr int INF = (1<<30)-1; constexpr char el = '\n'; template<typename T> istream& operator>>(istream& ist, vector<T>& v) {for(auto& i : v) ist >> i; return ist;} template<typename T, size_t N> istream& operator>>(istream& ist, array<T, N>& v) {for(auto& i : v) ist >> i; return ist;} template<typename T> void read_multi() {} template<typename T, typename... U> void read_multi(int n, vector<T>& v, U&&... args) {if(n < ssize(v)) {cin >> v[n]; read_multi(args..., n+1, v);}} template<typename T, typename... U> void read_multi(vector<T>& v, U&&... args) {read_multi(args..., 0, v);} template<typename T, typename U> ostream& operator<<(ostream& ost, const pair<T, U> p) {ost << '{' << p.first << ' ' << p.second << '}'; return ost;} template<typename T> ostream& operator<<(ostream& ost, const vector<T>& v) {for(int i = 0; i < ssize(v); i++) {ost << (i ? " " : "") << v[i];} return ost;} template<typename T> ostream& operator<<(ostream& ost, const vector<vector<T>>& v) {for(int i = 0; i < ssize(v); i++) {ost << (i ? "\n" : "") << v[i];} return ost;} template<typename T, size_t N> ostream& operator<<(ostream& ost, const array<T, N>& v) {for(int i = 0; i < ssize(v); i++) {ost << (i ? " " : "") << v[i];} return ost;} template<typename T, size_t N, size_t M> ostream& operator<<(ostream& ost, const array<array<T, N>, M>& v) {for(int i = 0; i < ssize(v); i++) {ost << (i ? "\n" : "") << v[i];} return ost;} template<typename T, typename U> inline bool chmin(T& a, U b) {if(a > b){a = b; return true;} return false;} template<typename T, typename U> inline bool chmax(T& a, U b) {if(a < b){a = b; return true;} return false;} long long power(long long val, long long num, long long mod){ assert(mod >= 0); assert(num >= 0); long long res = 1; val %= mod; while(num){ if(num&1) res = (res*val)%mod; val = (val*val)%mod; num >>= 1; } return res; }

#if __has_include(<atcoder/modint>)
#include <atcoder/all>
using namespace atcoder; using mint9 = modint998244353; using mint1 = modint1000000007; ostream& operator<<(ostream& ost, const mint1& x) {ost << x.val(); return ost;} ostream& operator<<(ostream& ost, const mint9& x) {ost << x.val(); return ost;}
#endif

//*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*/
//*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*!?*/


struct FrequencyTableOfTreeDistance {
    struct graph { vector<int> G; vector<int> idx; struct ref_t { vector<int>::iterator begin_, end_; auto begin() const noexcept {return begin_;} auto end() const noexcept {return end_;} auto size() const noexcept {return end_ - begin_;} int operator[](int p) const {return begin_[p];} }; graph() = default; graph(int n, const auto &E){ build(n, E); } void build(int n, const auto &E){ G.resize(E.size()); idx.resize(n+1); for(auto &[u, v] : E){ idx[u+1]++; } for(int i = 1; i <= n; i++) idx[i] += idx[i-1]; auto C = idx; for(auto &[u, v] : E){ G[C[u]++] = v; } } ref_t operator[](int p) {return {G.begin()+idx[p], G.begin()+idx[p+1]};} };
    vector<long long> operator()(int n, vector<array<int, 2>> E){
        assert(n-1 == (int)E.size());
        E.resize((n-1)*2);
        for(int i = 0; i < n-1; i++) E[i+n-1] = {E[i][1], E[i][0]};
        graph G(n, E);
        
        vector<int> siz(n), par(n, -1), memo(n);
        vector<char> alr(n);
        
        auto calc_siz = [&](int root)->void {
            par[root] = -1;
            auto f = [&](auto &self, int pos)->void {
                siz[pos] = 1;
                for(auto nex : G[pos]){
                    if(nex == par[pos] || alr[nex]) continue;
                    par[nex] = pos;
                    self(self, nex);
                    siz[pos] += siz[nex];
                }
            }; f(f, root);
        };
        
        vector<ll> ans(n-1);
        
        pmr::monotonic_buffer_resource pool(malloc((n*6+1) * sizeof(int)), (n*6) * sizeof(int));
        
        pmr::vector<int> s(&pool); s.reserve(n+1);
        pmr::vector<int> Q(&pool); Q.reserve(n);
        pmr::vector<array<int, 2>> li(&pool); li.reserve(n);
        pmr::vector<long long> dt(&pool); dt.reserve(n);
        
        auto g = [&](auto &self, int root)->void {
            calc_siz(root);
            if(siz[root] == 1) return;
            
            int start = root;
            while(true){
                int mx = 0, mxi = 0;
                for(auto nex : G[start]){
                    if(nex == par[start] || alr[nex]) continue;
                    if(chmax(mx, siz[nex])) mxi = nex;
                }
                if(mx <= siz[root]/2) break;
                start = mxi;
            }
            alr[start] = true;
            
            {
                s.clear();
                li.clear();
                dt.clear();
                vector<int> &d = memo;
                int cnt = 0;
                
                for(auto i : G[start]){
                    if(alr[i]) continue;
                    
                    s.push_back(dt.size());
                    Q.clear();
                    int Q_beg = 0;
                    Q.push_back(i);
                    dt.push_back(1);
                    d[i] = 1;
                    par[i] = -1;
                    while((int)Q.size() != Q_beg){
                        int pos = Q[Q_beg++];
                        for(auto nex : G[pos]){
                            if(nex == par[pos] || alr[nex]) continue;
                            d[nex] = d[pos] + 1;
                            par[nex] = pos;
                            if((int)dt.size() - s.back() < d[nex]) dt.push_back(0);
                            dt[s.back() + d[nex]-1]++;
                            Q.push_back(nex);
                        }
                    }
                    li.push_back({(int)dt.size() - s.back(), cnt++});
                }
                s.push_back(dt.size());
                
                sort(all(li));
                vector<int> SU(dt.data() + s[li[0][1]], dt.data() + s[li[0][1] + 1]);
                for(int i = 0; i < cnt-1; i++){
                    vector<int> R(dt.data() + s[li[i+1][1]], dt.data() + s[li[i+1][1] + 1]);
                    auto res = convolution<998244353, int>(SU, R);
                    for(int j = 0; j < (int)res.size(); j++) ans[j+1] += res[j];
                    for(int j = 0; j < (int)SU.size(); j++) R[j] += SU[j];
                    swap(SU, R);
                }
                for(int j = 0; j < (int)SU.size(); j++) ans[j] += SU[j];
            }
            
            for(auto i : G[start]){
                if(alr[i]) continue;
                self(self, i);
            }
        }; g(g, 0);
        
        return ans;
    }
} frequency_table_of_tree_distance;

signed main(){
    int n;
    cin >> n;
    vector<array<int, 2>> E(n-1);
    for(int i = 0; i < n-1; i++) cin >> E[i][0] >> E[i][1];
    
    vector<long long> ans = frequency_table_of_tree_distance(n, E);
    
    for(auto i : ans) cout << i << ' ';
    cout << '\n';
}
