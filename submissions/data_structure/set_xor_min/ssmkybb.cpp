#ifdef LOCAL
#include <bits/stdc++.h>
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
#define initv2(t,a,...) (a), vector<t>(__VA_ARGS__)
#define initv3(t,a,b,...) (a), vector<vector<t>>((b), vector<t>(__VA_ARGS__))
#define vec vector
#define elif else if

struct FIstream{
    #ifndef LOCAL
    #define fread fread_unlocked
    #endif
    
    static constexpr unsigned SIZ = 1 << 17;
    char buf[SIZ], *p1 = buf, *p2 = buf;
    
    inline char _getchar(){
        if(p1 == p2){
            p2 = (p1 = buf) + fread(buf, 1, SIZ, stdin);
            if(p1 == p2) return EOF;
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
    inline FIstream& operator>>(std::string& x){
        std::string().swap(x);
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
    
    static constexpr unsigned SIZ = 1 << 17;
    char buf[SIZ], *p1 = buf, *p2 = buf+SIZ;
    
    inline void _write(){
        fwrite(buf, 1, p1-buf, stdout);
        p1 = buf;
    }
    
    inline void _putchar(char c){
        if(p1 == p2){
            fwrite(buf, 1, p1 - buf, stdout);
            p1 = buf;
        }
        *p1++ = c;
    }
    
    template<typename T>
    void _write_i(T x){
        char num[100], *idxp = num+100;
        if(x < 0){
            _putchar('-');
            x = -x;
        }
        if(p1 - buf < 100) _write();
        while(x >= 10000){
            idxp -= 4;
            memcpy(idxp, _FOstream_pre.num[x%10000], 4);
            x /= 10000;
        }
        if(x >= 1000){
            memcpy(p1, _FOstream_pre.num[x], 4);
            p1 += 4;
        } else if(x >= 100){
            memcpy(p1, _FOstream_pre.num[x]+1, 3);
            p1 += 3;
        } else if(x >= 10){
            memcpy(p1, _FOstream_pre.num[x]+2, 2);
            p1 += 2;
        } else *p1++ = x+'0';
        memcpy(p1, idxp, num+100 - idxp);
        p1 += num+100 - idxp;
    }
    
    template<typename T>
    FOstream& operator<<(T x){
        if constexpr(is_same_v<T, char>){
            _putchar(x);
        } else if constexpr(is_same_v<decay_t<T>, const char*> || is_same_v<decay_t<T>, char*>){
            while(*x) _putchar(*x++);
        } else if constexpr(is_integral_v<T> || is_same_v<T, __int128>){
            _write_i(x);
        } else if constexpr(is_same_v<T, float> || is_same_v<T, double>){
            char _b[100];
            snprintf(_b, sizeof(_b), "%.*f", 15, x);
            *this << _b;
        } else if constexpr(is_same_v<T, long double>){
            char _b[100];
            snprintf(_b, sizeof(_b), "%.*Lf", 15, x);
            *this << _b;
        } else assert(0);
        return *this;
    }
    #ifdef _GLIBCXX_STRING
    FOstream& operator<<(const std::string& x){
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


template <typename T = int>
struct binary_trie{
    private:
    
    struct node_t{
        T value;
        int count = 0;
        array<int, 4> child = {};
        int width = 0;
    };
    
    struct ref_node_t{
        T val;
        bool exist;
        ref_node_t(T x, bool e) : val{x}, exist{e} {}
    };
    
    static constexpr T one = 1;
    static constexpr int bit_width = sizeof(T) * 8;
    vector<node_t> node;
    T xor_val = 0;
    int root = -1;
    int siz = 0;
    
    //__builtin_clz
    template<typename _Tp>
    inline int clz(_Tp x) const {
        if constexpr(sizeof(_Tp) == 8ull) return __builtin_clzll(x);
        else return __builtin_clz(x);
    }
    
    //[l, r)のマスクを返す
    inline T mask(int l, int r) const {
        if(r >= bit_width) return -(one<<l);
        return (one<<r) - (one<<l);
    }
    
    //[l, r)bitを取り出す
    inline T masked(T v, int l, int r) const {
        return mask(l, r) & v;
    }
    
    //上位から見て初めて異なるbit以降の個数を返す(例えば、一致していたら0)
    inline int diff_bit(T x, T y) const {
        return ((bit_width-1 - clz(x^y))|1) + 1;
    }
    
    //ノードを返す
    inline int make_node(T v){
        node.emplace_back();
        node.back().value = v;
        return ssize(node)-1;
    }
    
    public:
    
    binary_trie(){
        make_node(0);
        root = make_node(0);
        node[root].width = bit_width;
    }
    
    binary_trie<T>& operator=(binary_trie<T>&& o) noexcept = default;
    
    binary_trie(binary_trie&& o) noexcept = default;
    
    void insert(T v) {
        int pos = root;
        int bit = bit_width;
        siz++;
        v ^= xor_val;
        while(pos != 0){
            T mv = masked(v, bit-node[pos].width, bit);
            T mnv = masked(node[pos].value, bit-node[pos].width, bit);
            if(mv != mnv){
                int diff = diff_bit(mv, mnv);
                int b = (mv>>(diff-2))&3;
                int nb = (mnv>>(diff-2))&3;
                int inter = make_node(node[pos].value);
                int leaf = make_node(v);
                node[inter] = node[pos];
                node[inter].width -= bit - diff;
                memset(node[pos].child.data(), 0, sizeof(int)*4);
                node[pos].child[b] = leaf;
                node[pos].child[nb] = inter;
                node[pos].count++;
                node[pos].width = bit - diff;
                bit = diff;
                node[leaf].width = bit;
                node[leaf].count = 1;
                pos = leaf;
                return;
            } else {
                node[pos].count++;
                bit -= node[pos].width;
                if(bit == 0) return;
                int nex = node[pos].child[(v>>(bit-2))&3];
                if(nex == 0){
                    nex = node[pos].child[(v>>(bit-2))&3] = make_node(v);
                    node[nex].count = 1;
                    node[nex].width = bit;
                    return;
                }
                pos = nex;
            }
        }
    }
    
    int count(T v) const {
        int pos = root;
        int bit = bit_width;
        v ^= xor_val;
        while(pos != 0){
            T mv = masked(v, bit-node[pos].width, bit);
            T mnv = masked(node[pos].value, bit-node[pos].width, bit);
            if(mv != mnv) return 0;
            bit -= node[pos].width;
            if(bit == 0) return node[pos].count;
            pos = node[pos].child[(v>>(bit-2))&3];
        }
        return 0;
    }
    
    void erase(T v, int n = -1) {
        if(n == -1) n = count(v);
        if(n == 0) return;
        int pos = root;
        int bit = bit_width;
        siz -= n;
        v ^= xor_val;
        while(true){
            node[pos].count -= n;
            bit -= node[pos].width;
            if(bit == 0) return;
            pos = node[pos].child[(v>>(bit-2))&3];
        }
    }
    
    const ref_node_t operator[](int k) const {
        if(k >= 0) k = siz-k-1;
        else k += siz;
        if(k < 0 || siz <= k) return ref_node_t{0, false};
        
        k++;
        int pos = root;
        int bit = bit_width;
        while(true){
            bit -= node[pos].width;
            if(bit == 0) return ref_node_t{node[pos].value^xor_val, true};
            int b = (xor_val>>(bit-2))&3;
            auto &child = node[pos].child;
            if(k <= node[child[b^3]].count){
                pos = child[b^3];
            } else {
                k -= node[child[b^3]].count;
                if(k <= node[child[b^2]].count){
                    pos = child[b^2];
                } else {
                    k -= node[child[b^2]].count;
                    if(k <= node[child[b^1]].count){
                        pos = child[b^1];
                    } else {
                        k -= node[child[b^1]].count;
                        pos = child[b];
                    }
                }
            }
        }
    }
    
    //未満の要素の個数
    int order(T v) const {
        if(v == 0) return 0;
        v--;
        int res = 0;
        int pos = root;
        int bit = bit_width;
        while(pos != 0){
            T mv = masked(v, bit-node[pos].width, bit);
            T mnv = masked(node[pos].value^xor_val, bit-node[pos].width, bit);
            if(mv < mnv){
                return res;
            } else {
                if(mv > mnv){
                    res += node[pos].count;
                    return res;
                } else {
                    bit -= node[pos].width;
                    if(bit == 0) return res + node[pos].count;
                    int b = (v>>(bit-2))&3;
                    auto &child = node[pos].child;
                    T mxv = (xor_val>>(bit-2))&3;
                    
                    if(b >= 1){
                        res += node[child[mxv]].count;
                        if(b >= 2){
                            res += node[child[mxv^1]].count;
                            if(b >= 3){
                                res += node[child[mxv^2]].count;
                            }
                        }
                    }
                    pos = node[pos].child[b^((xor_val>>(bit-2))&3)];
                }
            }
        }
        return res;
    }
    
    const ref_node_t lower_bound(T v) const {
        int ord = order(v);
        if(siz == ord) return ref_node_t{0, false};
        else return ref_node_t{(*this)[ord].val, true};
    }
    
    const ref_node_t less_bound(T v) const {
        int ord = v!=numeric_limits<T>::max() ? order(v+1) : siz;
        if(ord == 0) return ref_node_t{0, false};
        else return ref_node_t{(*this)[ord-1].val, true};
    }
    
    void reserve(int n) {
        node.reserve(2*n+2);
    }
    
    int size() const {
        return siz;
    }
    
    void apply_xor(T x) {
        xor_val ^= x;
    }
    
    const ref_node_t xor_min(T v) {
        if(siz == 0) return ref_node_t{0, false};
        apply_xor(v);
        ref_node_t res{(*this)[0].val^v, true};
        apply_xor(v);
        return res;
    }
    
    const ref_node_t xor_max(T v) {
        if(siz == 0) return ref_node_t{0, false};
        apply_xor(v);
        ref_node_t res{(*this)[-1].val^v, true};
        apply_xor(v);
        return res;
    }
};


int main(){
    int q, t, x;
    cin >> q;
    
    binary_trie<int> S;
    S.reserve(q);
    
    while(q--){
        cin >> t >> x;
        switch(t){
            case 0:
                S.insert(x);
                break;
            case 1:
                S.erase(x);
                break;
            case 2:
                cout << (x^S.xor_min(x).val) << '\n';
                break;
        }
    }
    
}
