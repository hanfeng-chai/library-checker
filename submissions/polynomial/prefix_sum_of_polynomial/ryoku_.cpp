#define CP_BUNDLED_SOURCE
#ifndef CP_BUNDLE_HEADER_3840A09F237A779E
#define CP_BUNDLE_HEADER_3840A09F237A779E
#ifdef TEMPLATE
#else
#define TEMPLATE
# pragma GCC optimize("O3")
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <climits>
#include <cassert>
#include <functional>
#include <iterator>
#include <utility>
#include <complex>
#include <bitset>
#include <chrono>
#include <random>
#include <limits>
#include <optional>
#include <variant>
#include <any>

#include <array>
#include <bit>
#include <compare>
#include <concepts>
#include <numbers>
#include <ranges>
#include <span>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <version>
using namespace std;
using uint=unsigned;
using ll=long long;
using ull=unsigned long long;
using ld=long double;
using pii=pair<int,int>;
using pll=pair<ll,ll>;
using i128=__int128;
using u128=unsigned __int128;
template<class T>using vc=vector<T>;
template<class T>using vvc=vc<vc<T>>;
template<class T>using vvvc=vvc<vc<T>>;
template<class T>using smpq=priority_queue<T,vector<T>,greater<T>>;
template<class T>using bipq=priority_queue<T>;
#define rep(i,n) for(ll i=0;i<(ll)(n);i++)
#define REP(i,j,n) for(ll i=(j);i<(ll)(n);i++)
#define DREP(i,n,m) for(ll i=(n);i>=(m);i--)
#define drep(i,n) for(ll i=((n)-1);i>=0;i--)
#define rall(x) x.rbegin(),x.rend()
#define mp(...) make_pair(__VA_ARGS__)
#define pb push_back
#define fi first
#define se second
#define is insert
#define bg begin()
#define ed end()
#define all(x) x.begin(),x.end()
void scan(int&a) { cin >> a; }
void scan(ll&a) { cin >> a; }
void scan(string&a) { cin >> a; }
void scan(char&a) { cin >> a; }
void scan(uint&a) { cin >> a; }
void scan(ull&a) { cin >> a; }
void scan(bool&a) { cin >> a; }
void scan(ld&a){ cin>> a;}
template<class T> void scan(vector<T>&a) { for(auto&x:a) scan(x); }
void read() {}
template<class Head, class... Tail> void read(Head&head, Tail&... tail) { scan(head); read(tail...); }
#define INT(...) int __VA_ARGS__; read(__VA_ARGS__);
#define LL(...) ll __VA_ARGS__; read(__VA_ARGS__);
#define ULL(...) ull __VA_ARGS__; read(__VA_ARGS__);
#define STR(...) string __VA_ARGS__; read(__VA_ARGS__);
#define VC(type, name, ...) vector<type> name(__VA_ARGS__); read(name);
#define VVC(type, name, size, ...) vector<vector<type>> name(size, vector<type>(__VA_ARGS__)); read(name);
template<class T>void print(T a) { cout << a; }
template<class T> void print(vector<T>a) { for(int i=0;i<(int)a.size();i++){if(i)cout<<" ";print(a[i]);}}
void PRT() { cout <<endl; return ; }
template<class T> void PRT(T a) { print(a); cout <<endl; return; }
template<class Head, class... Tail> void PRT(Head head, Tail ... tail) { print(head); cout << " "; PRT(tail...); return; }
template<class T,class F>
bool chmin(T &x, F y){
    if(x>y){
        x=y;
        return true;
    }
    return false;
}
template<class T, class F>
bool chmax(T &x, F y){
    if(x<y){
        x=y;
        return true;
    }
    return false;
}
template<typename T>
T floor(T a,T b){
    return a/b-(a%b&&((a<0)!=(b<0)));
}
template<typename T>
T ceil(T a,T b){
    return a/b+(a%b&&((a<0)==(b<0)));
}
template<typename T>
T bmod(T x,T y){
    return x-y*floor(x,y);
}
template<typename T>
pair<T,T>divmod(T x,T y){
    T q=floor(x,y);
    return{q,x-q*y};
}
void YesNo(bool b){
    cout<<(b?"Yes":"No")<<endl;
}
void YESNO(bool b){
    cout<<(b?"YES":"NO")<<endl;
}
void AliceBob(bool b){
    cout<<(b?"Alice":"Bob")<<endl;
}
void TakahashiAoki(bool b){
    cout<<(b?"Takahashi":"Aoki")<<endl;
}
void Yes(){
    cout<<"Yes"<<endl;
}
void No(){
    cout<<"No"<<endl;
}
vc<int>stovi(const string&s,const string&S){
    vc<int>v(s.size());
    rep(i,s.size()){
        auto t=S.find(s[i]);
        assert(t!=string::npos);
        v[i]=t;
    }
    return v;
}
template<class T=ll>
T isqrt(T x){
    T F=sqrtl(x);
    while((F+1)*(F+1)<=x)F++;
    while(F*F>x)F--;
    return F;
}
template<class T>
T tri(T x){return x*(x-1)/2;}

//[l,r)
template<class T>T tri(T l,T r){return (r-l)*(l+r-1)/2;}
//n を先頭に持ってくる
template<class T>
vc<T>rot(vc<T>v,int n){
    rotate(v.begin(),v.begin()+n,v.end());
    return v;
}
template<class T>
vc<T>iota(int n){
    vc<T>v(n);rep(i,n)v[i]=i;
    return v;
}
template<class T,int minfirst>
vc<int>argsort(const vc<T>&a){
    auto idx=iota<int>(a.size());
    sort(all(idx),[&](int i,int j){
        return (minfirst?make_pair(a[i],i)<make_pair(a[j],j):make_pair(a[i],i)>make_pair(a[j],j));
    });
    return idx;
}
template<class T>
vvc<T>trans(const vvc<T>&a){
    assert(a.size()&&a[0].size());
    rep(i,a.size())assert(a[i].size()==a[0].size());
    vvc<T>b(a[0].size(),vc<T>(a.size()));
    rep(i,a.size())rep(j,a[0].size())b[j][i]=a[i][j];
    return b;
}
vc<string>trans(const vc<string>&a){
    assert(a.size()&&a[0].size());
    rep(i,a.size())assert(a[i].size()==a[0].size());
    vc<string>b(a[0].size(),string(a.size(),0));
    rep(i,a.size())rep(j,a[0].size())b[j][i]=a[i][j];
    return b;
}
template<class T>
int popcount(T n){
    return __builtin_popcountll(n);
}
template<class T,class L=ll>
L sum(const vc<T>&a){
    return accumulate(all(a),L(0));
}
template<class T>
struct subset_view{
    T s;
    struct iterator{
        T s,x;
        bool done;
        T operator*()const{return x;}
        iterator&operator++(){
            if(x==0)done=true;
            else x=(x-1)&s;
            return*this;
        }
        bool operator!=(const iterator&r)const{return done!=r.done;}
    };
    iterator begin()const{return{s,s,false};}
    iterator end()const{return{s,0,true};}
};
template<class T>
subset_view<T>subset(T s){
    return{s};
}
template<class T>
T max(vc<T>&a){
    return *max_element(all(a));
}
template<class T>
T min(vc<T>&a){
    return *min_element(all(a));
}
template<class T,class L=ll>
vc<L> presum(vc<T> &a){
    vc<L> ret(a.size()+1);
    rep(i,a.size())ret[i+1]=ret[i]+a[i];
    return ret;
}
template<class T, class F>
vc<T> &operator+=(vc<T> &a,F b){
    for (auto&v:a)v += b;
    return a;
}
template<class T, class F>
vc<T> &operator-=(vc<T>&a,F b){
    for (auto&v:a)v-=b;
    return a;
}
template<class T, class F>
vc<T> &operator*=(vc<T>&a,F b){
    for (auto&v:a)v*=b;
    return a;
}
template<class T=ll>
constexpr T pow(T a,T b){
    T res=1;
    while(b){
        if(b&1)res*=a;
        a*=a;
        b/=2;
    }
    return res;
}
constexpr ll ten(ll a){
    return pow<ll>(10,a);
}
template<typename T>constexpr T inf=numeric_limits<T>::max()/2-1;
template<class T>
int tbit(T x){
    using U=make_unsigned_t<T>;
    U y=(U)x;
    return y?(int)bit_width(y)-1:-1;
}
template<class T>
int lbit(T x){
    using U=make_unsigned_t<T>;
    U y=(U)x;
    return y?(int)countr_zero(y):-1;
}
template<class T>
int tbit(T x,int p){
    using U=make_unsigned_t<T>;
    constexpr int W=numeric_limits<U>::digits;
    U y=(U)x;
    if(p<0)return -1;
    if(p>=W-1)return tbit(y);
    return tbit(y&((U(1)<<(p+1))-1));
}
template<class T>
int lbit(T x,int p){
    using U=make_unsigned_t<T>;
    constexpr int W=numeric_limits<U>::digits;
    U y=(U)x;
    if(p<0)return lbit(y);
    if(p>=W)return -1;
    return lbit(y&(~U(0)<<p));
}
istream& operator>>(istream&is,i128&x){
    string s;is>>s;
    x=0;
    int i=0,neg=0;
    if(s[0]=='-')neg=1,i=1;
    for(;i<(int)s.size();i++)x=x*10+s[i]-'0';
    if(neg)x=-x;
    return is;
}
ostream& operator<<(ostream&os,i128 x){
    if(x==0)return os<<0;
    if(x<0)os<<"-";
    u128 y=x<0?-(u128)x:(u128)x;
    string s;
    while(y)s.pb('0'+y%10),y/=10;
    reverse(all(s));
    return os<<s;
}
#ifdef LOCAL
#include "debug/debug.hpp"
inline int dbging=1;
#else
#define dbg(...) ((void)0)
#endif

struct template_setup{
    template_setup(){
        #ifdef LOCAL
        freopen("input.txt","r",stdin);
        freopen("output.txt","w",stdout);
        
        #endif
        cin.tie(0)->sync_with_stdio(0);
        #ifdef LOCAL
        cout<<fixed<<setprecision(6);
        #else
        cout<<fixed<<setprecision(20);
        #endif
    }
};
inline template_setup template_setup;

#if defined(LOCAL)&&!defined(CP_NO_AUTO_ALL)&&!defined(CP_BUNDLED_SOURCE)
#include <all.hpp>
#endif

#endif
#endif
#ifndef CP_BUNDLE_HEADER_8DD6832EF023BDC7
#define CP_BUNDLE_HEADER_8DD6832EF023BDC7
template<uint32_t mod>
struct static_modint{
    static_assert(0<mod&&mod<=(1u<<31)-1);
    using u32=uint32_t;
    using u64=uint64_t;
    u32 val;
    static_modint():val(0){}
    static_modint(ll x){
        ll v=x%mod;
        if(v<0)v+=mod;
        val=v;
    }
    constexpr static uint32_t get_mod(){
        return mod;
    }
    static static_modint raw(int v){
        assert(v>=0);
        static_modint mi;
        mi.val=v;
        return mi;
    }
    static_modint &operator+=(const static_modint&m){
        u32 x=val+m.val-mod;
        val=x+(mod&-(x>>31));
        return *this;
    }
    static_modint &operator-=(const static_modint&m){
        u32 x=val-m.val;
        val=x+(mod&-(x>>31));
        return *this;
    }
    static_modint &operator*=(const static_modint&m){
        val=u64(val)*m.val%mod;
        return *this;
    }
    static_modint &operator/=(const static_modint&m){
        val=u64(val)*m.inv().val%mod;
        return *this;
    }
    static_modint operator-() const{
        return static_modint(mod-val);
    }
    static_modint operator+() const {
        return *this;
    }
    friend static_modint operator+(static_modint lhs, const static_modint& rhs){
        return lhs+=rhs;
    }
    friend static_modint operator-(static_modint lhs, const static_modint& rhs){
        return lhs-=rhs;
    }
    friend static_modint operator*(static_modint lhs, const static_modint& rhs){
        return lhs*=rhs;
    }
    friend static_modint operator/(static_modint lhs,const static_modint&rhs){
        return lhs/=rhs;
    }
    bool operator==(const static_modint&p) const{
        return p.val==val;
    }
    bool operator!=(const static_modint&p) const{
        return p.val!=val;
    }
    static_modint pow(int64_t n) const{
        static_modint res(1),mul(val);
        while(n){
            if(n%2)res*=mul;
            mul*=mul;
            n/=2;
        }
        return res;
    }

    friend ostream&operator<<(ostream&os,const static_modint&p){
        os<<p.val;
        return os;
    }
    friend istream&operator>>(istream&is,static_modint&p){
        int64_t x;
        is>>x;
        p=static_modint(x);
        return is;
    }
    static_modint inv()const{
        int64_t a=val,b=mod,u=1,v=0,t;
        #ifdef LOCAL
        assert(gcd(a,b)==1);
        #endif
        while(b>0){
            t=a/b;
            swap(a-=t*b,b);
            swap(u-=t*v,v);
        }
        return static_modint(u);
    }
};
#endif
#ifndef CP_BUNDLE_HEADER_412FFE9058386D60
#define CP_BUNDLE_HEADER_412FFE9058386D60
template<class,class=void>
struct binom_has_get_mod:false_type{};
template<class mint>
struct binom_has_get_mod<mint,void_t<decltype(mint::get_mod())>>:true_type{};
template<class mint>
struct binom{
private:
    static vector<mint>&fact_table(){static vector<mint>v={1};return v;}
    static vector<mint>&invfact_table(){static vector<mint>v={1};return v;}
    static vector<mint>&invs_table(){static vector<mint>v={0};return v;}
    static int&built_mod(){static int mod=-1;return mod;}
public:
    static void build(int n){
        auto&_fact=fact_table();
        auto&_invfact=invfact_table();
        auto&_invs=invs_table();
        if constexpr(binom_has_get_mod<mint>::value){
            auto mod=mint::get_mod();
            if(built_mod()!=mod){
                _fact={1};
                _invfact={1};
                _invs={0};
                built_mod()=mod;
            }
        }
        if(n<(int)_fact.size())return;
        int old=_fact.size();
        _fact.resize(n+1);
        _invfact.resize(n+1);
        _invs.resize(n+1);
        if constexpr(binom_has_get_mod<mint>::value){
            auto mod=mint::get_mod();
            for(int i=old;i<=n;i++){
                _fact[i]=_fact[i-1]*i;
                if(i==1)_invs[i]=1;
                else _invs[i]=-_invs[mod%i]*(mod/i);
                _invfact[i]=_invfact[i-1]*_invs[i];
            }
        }else{
            for(int i=old;i<=n;i++){
                _fact[i]=_fact[i-1]*i;
                _invs[i]=mint(1)/i;
                _invfact[i]=_invfact[i-1]*_invs[i];
            }
        }
    }
    static mint fact(int i){
        assert(i>=0);
        build(i);
        return fact_table()[i];
    }
    static mint invfact(int i){
        assert(i>=0);
        build(i);
        return invfact_table()[i];
    }
    static mint inv(int i){
        assert(i>0);
        build(i);
        return invs_table()[i];
    }
    static mint C(int a,int b){//aCb
        if(b==0)return 1;
        if(a<0||b<0||a-b<0)return mint(0);
        build(a);
        auto&_fact=fact_table();
        auto&_invfact=invfact_table();
        return _fact[a]*_invfact[b]*_invfact[a-b];
    }
    static mint iC(int a,int b){//1/aCb
        if(b==0)return 1;
        if(a<0||b<0||a-b<0)return mint(0);
        build(a);
        auto&_fact=fact_table();
        auto&_invfact=invfact_table();
        return _fact[b]*_fact[a-b]*_invfact[a];
    }
    static mint P(int a,int b){
        if(a<b||b<0)return 0;
        build(a);
        auto&_fact=fact_table();
        auto&_invfact=invfact_table();
        return _fact[a]*_invfact[a-b];
    }
    static mint H(int a,int b){
        return C(a+b-1,b);
    }

};
#endif
#ifndef CP_BUNDLE_HEADER_752CC73BC612CC72
#define CP_BUNDLE_HEADER_752CC73BC612CC72
#ifndef CP_BUNDLE_HEADER_302E2F308B295D16
#define CP_BUNDLE_HEADER_302E2F308B295D16
#include <utility>
#ifdef _MSC_VER
#include <intrin.h>
#endif
namespace atcoder {
namespace internal {
constexpr long long safe_mod(long long x, long long m) {
    x %= m;
    if (x < 0) x += m;
    return x;
}
struct barrett_reduction {
    unsigned int _m;
    unsigned long long im;
    explicit barrett_reduction(unsigned int m) : _m(m), im((unsigned long long)(-1) / m + 1) {}
    unsigned int umod() const { return _m; }
    unsigned int mul(unsigned int a, unsigned int b) const {
        unsigned long long z = a;
        z *= b;
#ifdef _MSC_VER
        unsigned long long x;
        _umul128(z, im, &x);
#else
        unsigned long long x =
            (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
#endif
        unsigned long long y = x * _m;
        return (unsigned int)(z - y + (z < y ? _m : 0));
    }
};
constexpr long long pow_mod_constexpr(long long x, long long n, int m) {
    if (m == 1) return 0;
    unsigned int _m = (unsigned int)(m);
    unsigned long long r = 1;
    unsigned long long y = safe_mod(x, m);
    while (n) {
        if (n & 1) r = (r * y) % _m;
        y = (y * y) % _m;
        n >>= 1;
    }
    return r;
}
constexpr bool is_prime_constexpr(int n) {
    if (n <= 1) return false;
    if (n == 2 || n == 7 || n == 61) return true;
    if (n % 2 == 0) return false;
    long long d = n - 1;
    while (d % 2 == 0) d /= 2;
    constexpr long long bases[3] = {2, 7, 61};
    for (long long a : bases) {
        long long t = d;
        long long y = pow_mod_constexpr(a, t, n);
        while (t != n - 1 && y != 1 && y != n - 1) {
            y = y * y % n;
            t <<= 1;
        }
        if (y != n - 1 && t % 2 == 0) {
            return false;
        }
    }
    return true;
}
template <int n> constexpr bool is_prime = is_prime_constexpr(n);
constexpr std::pair<long long, long long> inv_gcd(long long a, long long b) {
    a = safe_mod(a, b);
    if (a == 0) return {b, 0};
    long long s = b, t = a;
    long long m0 = 0, m1 = 1;
    while (t) {
        long long u = s / t;
        s -= t * u;
        m0 -= m1 * u;
        auto tmp = s;
        s = t;
        t = tmp;
        tmp = m0;
        m0 = m1;
        m1 = tmp;
    }
    if (m0 < 0) m0 += b / s;
    return {s, m0};
}
constexpr int primitive_root_constexpr(int m) {
    if (m == 2) return 1;
    if (m == 167772161) return 3;
    if (m == 469762049) return 3;
    if (m == 754974721) return 11;
    if (m == 998244353) return 3;
    int divs[20] = {};
    divs[0] = 2;
    int cnt = 1;
    int x = (m - 1) / 2;
    while (x % 2 == 0) x /= 2;
    for (int i = 3; (long long)(i)*i <= x; i += 2) {
        if (x % i == 0) {
            divs[cnt++] = i;
            while (x % i == 0) {
                x /= i;
            }
        }
    }
    if (x > 1) {
        divs[cnt++] = x;
    }
    for (int g = 2;; g++) {
        bool ok = true;
        for (int i = 0; i < cnt; i++) {
            if (pow_mod_constexpr(g, (m - 1) / divs[i], m) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}
template <int m> constexpr int primitive_root = primitive_root_constexpr(m);
unsigned long long floor_sum_unsigned(unsigned long long n,
                                      unsigned long long m,
                                      unsigned long long a,
                                      unsigned long long b) {
    unsigned long long ans = 0;
    while (true) {
        if (a >= m) {
            ans += n * (n - 1) / 2 * (a / m);
            a %= m;
        }
        if (b >= m) {
            ans += n * (b / m);
            b %= m;
        }
        unsigned long long y_max = a * n + b;
        if (y_max < m) break;
        n = (unsigned long long)(y_max / m);
        b = (unsigned long long)(y_max % m);
        std::swap(m, a);
    }
    return ans;
}
}
}
namespace atcoder {
long long pow_mod(long long x, long long n, int m) {
    assert(0 <= n && 1 <= m);
    if (m == 1) return 0;
    internal::barrett_reduction bt((unsigned int)(m));
    unsigned int r = 1, y = (unsigned int)(internal::safe_mod(x, m));
    while (n) {
        if (n & 1) r = bt.mul(r, y);
        y = bt.mul(y, y);
        n >>= 1;
    }
    return r;
}
long long inv_mod(long long x, long long m) {
    assert(1 <= m);
    auto z = internal::inv_gcd(x, m);
    assert(z.first == 1);
    return z.second;
}
std::pair<long long, long long> crt(const std::vector<long long>& r,
                                    const std::vector<long long>& m) {
    assert(r.size() == m.size());
    int n = int(r.size());
    long long r0 = 0, m0 = 1;
    for (int i = 0; i < n; i++) {
        assert(1 <= m[i]);
        long long r1 = internal::safe_mod(r[i], m[i]), m1 = m[i];
        if (m0 < m1) {
            std::swap(r0, r1);
            std::swap(m0, m1);
        }
        if (m0 % m1 == 0) {
            if (r0 % m1 != r1) return {0, 0};
            continue;
        }
        long long g, im;
        std::tie(g, im) = internal::inv_gcd(m0, m1);
        long long u1 = (m1 / g);
        if ((r1 - r0) % g) return {0, 0};
        long long x = (r1 - r0) / g % u1 * im % u1;
        r0 += x * m0;
        m0 *= u1;
        if (r0 < 0) r0 += m0;
    }
    return {r0, m0};
}
long long floor_sum(long long n, long long m, long long a, long long b) {
    assert(0 <= n && n < (1LL << 32));
    assert(1 <= m && m < (1LL << 32));
    unsigned long long ans = 0;
    if (a < 0) {
        unsigned long long a2 = internal::safe_mod(a, m);
        ans -= 1ULL * n * (n - 1) / 2 * ((a2 - a) / m);
        a = a2;
    }
    if (b < 0) {
        unsigned long long b2 = internal::safe_mod(b, m);
        ans -= 1ULL * n * ((b2 - b) / m);
        b = b2;
    }
    return ans + internal::floor_sum_unsigned(n, m, a, b);
}
}
#include <algorithm>
#include <array>
#include <cassert>
#include <type_traits>
#include <vector>
#if __cplusplus >= 202002L
#include <bit>
#endif
namespace atcoder {
namespace internal {
#if __cplusplus >= 202002L
using std::bit_ceil;
#else
unsigned int bit_ceil(unsigned int n) {
    unsigned int x = 1;
    while (x < (unsigned int)(n)) x *= 2;
    return x;
}
#endif
int countr_zero(unsigned int n) {
#ifdef _MSC_VER
    unsigned long index;
    _BitScanForward(&index, n);
    return index;
#else
    return __builtin_ctz(n);
#endif
}
constexpr int countr_zero_constexpr(unsigned int n) {
    int x = 0;
    while (!(n & (1 << x))) x++;
    return x;
}
}
}
namespace atcoder {
namespace internal {
template <class mint,
          int g = internal::primitive_root<mint::get_mod()>>
struct fast_fourier_transform_info {
    static constexpr int rank2 = countr_zero_constexpr(mint::get_mod() - 1);
    std::array<mint, rank2 + 1> root;
    std::array<mint, rank2 + 1> iroot;
    std::array<mint, std::max(0, rank2 - 2 + 1)> rate2;
    std::array<mint, std::max(0, rank2 - 2 + 1)> irate2;
    std::array<mint, std::max(0, rank2 - 3 + 1)> rate3;
    std::array<mint, std::max(0, rank2 - 3 + 1)> irate3;
    fast_fourier_transform_info() {
        root[rank2] = mint(g).pow((mint::get_mod() - 1) >> rank2);
        iroot[rank2] = root[rank2].inv();
        for (int i = rank2 - 1; i >= 0; i--) {
            root[i] = root[i + 1] * root[i + 1];
            iroot[i] = iroot[i + 1] * iroot[i + 1];
        }
        {
            mint prod = 1, iprod = 1;
            for (int i = 0; i <= rank2 - 2; i++) {
                rate2[i] = root[i + 2] * prod;
                irate2[i] = iroot[i + 2] * iprod;
                prod *= iroot[i + 2];
                iprod *= root[i + 2];
            }
        }
        {
            mint prod = 1, iprod = 1;
            for (int i = 0; i <= rank2 - 3; i++) {
                rate3[i] = root[i + 3] * prod;
                irate3[i] = iroot[i + 3] * iprod;
                prod *= iroot[i + 3];
                iprod *= root[i + 3];
            }
        }
    }
};
template <class mint>
void butterfly(std::vector<mint>& a) {
    int n = int(a.size());
    int h = internal::countr_zero((unsigned int)n);
    static const fast_fourier_transform_info<mint> info;
    int len = 0;
    while (len < h) {
        if (h - len == 1) {
            int p = 1 << (h - len - 1);
            mint rot = 1;
            for (int s = 0; s < (1 << len); s++) {
                int offset = s << (h - len);
                for (int i = 0; i < p; i++) {
                    auto l = a[i + offset];
                    auto r = a[i + offset + p] * rot;
                    a[i + offset] = l + r;
                    a[i + offset + p] = l - r;
                }
                if (s + 1 != (1 << len))
                    rot *= info.rate2[countr_zero(~(unsigned int)(s))];
            }
            len++;
        } else {
            int p = 1 << (h - len - 2);
            mint rot = 1, imag = info.root[2];
            for (int s = 0; s < (1 << len); s++) {
                mint rot2 = rot * rot;
                mint rot3 = rot2 * rot;
                int offset = s << (h - len);
                for (int i = 0; i < p; i++) {
                    auto mod2 = 1ULL * mint::get_mod() * mint::get_mod();
                    auto a0 = 1ULL * a[i + offset].val;
                    auto a1 = 1ULL * a[i + offset + p].val * rot.val;
                    auto a2 = 1ULL * a[i + offset + 2 * p].val * rot2.val;
                    auto a3 = 1ULL * a[i + offset + 3 * p].val * rot3.val;
                    auto a1na3imag =
                        1ULL * mint(a1 + mod2 - a3).val * imag.val;
                    auto na2 = mod2 - a2;
                    a[i + offset] = a0 + a2 + a1 + a3;
                    a[i + offset + 1 * p] = a0 + a2 + (2 * mod2 - (a1 + a3));
                    a[i + offset + 2 * p] = a0 + na2 + a1na3imag;
                    a[i + offset + 3 * p] = a0 + na2 + (mod2 - a1na3imag);
                }
                if (s + 1 != (1 << len))
                    rot *= info.rate3[countr_zero(~(unsigned int)(s))];
            }
            len += 2;
        }
    }
}
template <class mint>
void butterfly_inv(std::vector<mint>& a) {
    int n = int(a.size());
    int h = internal::countr_zero((unsigned int)n);
    static const fast_fourier_transform_info<mint> info;
    int len = h;
    while (len) {
        if (len == 1) {
            int p = 1 << (h - len);
            mint irot = 1;
            for (int s = 0; s < (1 << (len - 1)); s++) {
                int offset = s << (h - len + 1);
                for (int i = 0; i < p; i++) {
                    auto l = a[i + offset];
                    auto r = a[i + offset + p];
                    a[i + offset] = l + r;
                    a[i + offset + p] =
                        (unsigned long long)((unsigned int)(l.val - r.val) + mint::get_mod()) *
                        irot.val;
                    ;
                }
                if (s + 1 != (1 << (len - 1)))
                    irot *= info.irate2[countr_zero(~(unsigned int)(s))];
            }
            len--;
        } else {
            int p = 1 << (h - len);
            mint irot = 1, iimag = info.iroot[2];
            for (int s = 0; s < (1 << (len - 2)); s++) {
                mint irot2 = irot * irot;
                mint irot3 = irot2 * irot;
                int offset = s << (h - len + 2);
                for (int i = 0; i < p; i++) {
                    auto a0 = 1ULL * a[i + offset + 0 * p].val;
                    auto a1 = 1ULL * a[i + offset + 1 * p].val;
                    auto a2 = 1ULL * a[i + offset + 2 * p].val;
                    auto a3 = 1ULL * a[i + offset + 3 * p].val;
                    auto a2na3iimag =
                        1ULL *
                        mint((mint::get_mod() + a2 - a3) * iimag.val).val;
                    a[i + offset] = a0 + a1 + a2 + a3;
                    a[i + offset + 1 * p] =
                        (a0 + (mint::get_mod() - a1) + a2na3iimag) * irot.val;
                    a[i + offset + 2 * p] =
                        (a0 + a1 + (mint::get_mod() - a2) + (mint::get_mod() - a3)) *
                        irot2.val;
                    a[i + offset + 3 * p] =
                        (a0 + (mint::get_mod() - a1) + (mint::get_mod() - a2na3iimag)) *
                        irot3.val;
                }
                if (s + 1 != (1 << (len - 2)))
                    irot *= info.irate3[countr_zero(~(unsigned int)(s))];
            }
            len -= 2;
        }
    }
}
template <class mint>
std::vector<mint> convolution_naive(const std::vector<mint>& a,
                                    const std::vector<mint>& b) {
    int n = int(a.size()), m = int(b.size());
    std::vector<mint> ans(n + m - 1);
    if (n < m) {
        for (int j = 0; j < m; j++) {
            for (int i = 0; i < n; i++) {
                ans[i + j] += a[i] * b[j];
            }
        }
    } else {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                ans[i + j] += a[i] * b[j];
            }
        }
    }
    return ans;
}
template <class mint>
std::vector<mint> convolution_fft(std::vector<mint> a, std::vector<mint> b) {
    int n = int(a.size()), m = int(b.size());
    int z = (int)internal::bit_ceil((unsigned int)(n + m - 1));
    a.resize(z);
    internal::butterfly(a);
    b.resize(z);
    internal::butterfly(b);
    for (int i = 0; i < z; i++) {
        a[i] *= b[i];
    }
    internal::butterfly_inv(a);
    a.resize(n + m - 1);
    mint iz = mint(z).inv();
    for (int i = 0; i < n + m - 1; i++) a[i] *= iz;
    return a;
}
}
template <class mint>
requires requires{mint::get_mod();}
std::vector<mint> convolution(std::vector<mint>&& a, std::vector<mint>&& b) {
    int n = int(a.size()), m = int(b.size());
    if (!n || !m) return {};
    int z = (int)internal::bit_ceil((unsigned int)(n + m - 1));
    assert((mint::get_mod() - 1) % z == 0);
    if (std::min(n, m) <= 60) return internal::convolution_naive(std::move(a), std::move(b));
    return internal::convolution_fft(std::move(a), std::move(b));
}
template <class mint>
requires requires{mint::get_mod();}
std::vector<mint> convolution(const std::vector<mint>& a,
                              const std::vector<mint>& b) {
    int n = int(a.size()), m = int(b.size());
    if (!n || !m) return {};
    int z = (int)internal::bit_ceil((unsigned int)(n + m - 1));
    assert((mint::get_mod() - 1) % z == 0);
    if (std::min(n, m) <= 60) return internal::convolution_naive(a, b);
    return internal::convolution_fft(a, b);
}
template <unsigned int mod = 998244353,
          class T>
requires std::is_integral_v<T>
std::vector<T> convolution(const std::vector<T>& a, const std::vector<T>& b) {
    int n = int(a.size()), m = int(b.size());
    if (!n || !m) return {};
    using mint = static_modint<mod>;
    int z = (int)internal::bit_ceil((unsigned int)(n + m - 1));
    assert((mint::get_mod() - 1) % z == 0);
    std::vector<mint> a2(n), b2(m);
    for (int i = 0; i < n; i++) {
        a2[i] = mint(a[i]);
    }
    for (int i = 0; i < m; i++) {
        b2[i] = mint(b[i]);
    }
    auto c2 = convolution(std::move(a2), std::move(b2));
    std::vector<T> c(n + m - 1);
    for (int i = 0; i < n + m - 1; i++) {
        c[i] = c2[i].val;
    }
    return c;
}
std::vector<long long> convolution_ll(const std::vector<long long>& a,
                                      const std::vector<long long>& b) {
    int n = int(a.size()), m = int(b.size());
    if (!n || !m) return {};
    static constexpr unsigned long long MOD1 = 754974721;
    static constexpr unsigned long long MOD2 = 167772161;
    static constexpr unsigned long long MOD3 = 469762049;
    static constexpr unsigned long long M2M3 = MOD2 * MOD3;
    static constexpr unsigned long long M1M3 = MOD1 * MOD3;
    static constexpr unsigned long long M1M2 = MOD1 * MOD2;
    static constexpr unsigned long long M1M2M3 = MOD1 * MOD2 * MOD3;
    static constexpr unsigned long long i1 =
        internal::inv_gcd(MOD2 * MOD3, MOD1).second;
    static constexpr unsigned long long i2 =
        internal::inv_gcd(MOD1 * MOD3, MOD2).second;
    static constexpr unsigned long long i3 =
        internal::inv_gcd(MOD1 * MOD2, MOD3).second;
    static constexpr int MAX_AB_BIT = 24;
    static_assert(MOD1 % (1ull << MAX_AB_BIT) == 1, "MOD1 isn't enough to support an array length of 2^24.");
    static_assert(MOD2 % (1ull << MAX_AB_BIT) == 1, "MOD2 isn't enough to support an array length of 2^24.");
    static_assert(MOD3 % (1ull << MAX_AB_BIT) == 1, "MOD3 isn't enough to support an array length of 2^24.");
    assert(n + m - 1 <= (1 << MAX_AB_BIT));
    auto c1 = convolution<MOD1>(a, b);
    auto c2 = convolution<MOD2>(a, b);
    auto c3 = convolution<MOD3>(a, b);
    std::vector<long long> c(n + m - 1);
    for (int i = 0; i < n + m - 1; i++) {
        unsigned long long x = 0;
        x += (c1[i] * i1) % MOD1 * M2M3;
        x += (c2[i] * i2) % MOD2 * M1M3;
        x += (c3[i] * i3) % MOD3 * M1M2;
        long long diff =
            c1[i] - internal::safe_mod((long long)(x), (long long)(MOD1));
        if (diff < 0) diff += MOD1;
        static constexpr unsigned long long offset[5] = {
            0, 0, M1M2M3, 2 * M1M2M3, 3 * M1M2M3};
        x -= offset[diff % 5];
        c[i] = x;
    }
    return c;
}
}
#endif

#ifndef FPS_STRUCT
#define FPS_STRUCT
template<class mint>
struct fps:vc<mint>{
    #define BF atcoder::internal::butterfly
    #define IBF atcoder::internal::butterfly_inv
    using vc<mint>::vc;
    using vc<mint>::operator[];
    fps(const vc<mint>&v):vc<mint>(v){}
    fps&operator=(const vc<mint>&v){vc<mint>::operator=(v);return *this;}
    fps&operator+=(const fps&g){
        int n=g.size();
        if(n>(int)this->size())this->resize(n);
        for(int i=0;i<n;i++)(*this)[i]+=g[i];
        return *this;
    }
    fps&operator-=(const fps&g){
        int n=g.size();
        if(n>(int)this->size())this->resize(n);
        for(int i=0;i<n;i++)(*this)[i]-=g[i];
        return *this;
    }
    fps&operator*=(const fps&g){
        if(this->empty()||g.empty()){this->clear();return *this;}
        auto res=atcoder::convolution<mint>(*this,g);
        *this=fps(res.begin(),res.end());
        return *this;
    }
    fps& operator+=(const mint&v){if(this->empty())this->resize(1);(*this)[0]+=v;return *this;}
    fps& operator-=(const mint&v){if(this->empty())this->resize(1);(*this)[0]-=v;return *this;}
    fps& operator*=(const mint&v){for(auto& x:*this)x*=v;return *this;}
    fps& operator/=(const mint&v){mint inv=v.inv();for(auto& x:*this)x*=inv;return *this;}
    fps& operator>>=(const ll&v){*this=fps(this->begin()+min<int>(this->size(),v),this->end());return *this;} 
    fps& operator<<=(const ll&v){fps np(v+this->size());rep(i,this->size())np[i+v]=(*this)[i];*this=np;return *this;}
    fps operator+(const fps&g)const{return fps(*this)+=g;}
    fps operator-(const fps&g)const{return fps(*this)-=g;}
    fps operator*(const fps&g)const{return fps(*this)*=g;}
    fps operator+(const mint&v)const{return fps(*this)+=v;}
    friend fps operator+(const mint&v,const fps&g){return fps(g)+=v;}
    fps operator-(const mint&v)const{return fps(*this)-=v;}
    friend fps operator-(const mint&v,const fps&g){return -g+=v;}
    fps operator*(const mint&v)const{return fps(*this)*=v;}
    friend fps operator*(const mint&v,const fps&g){return fps(g)*=v;}
    fps operator/(const mint&v)const{return fps(*this)/=v;}
    friend fps operator/(const mint&v,const fps&g){return fps(g)/=v;}
    fps operator>>(const ll&v)const{return fps(*this)>>=v;}
    fps operator<<(const ll&v)const{return fps(*this)<<=v;}
    fps operator-()const{
        fps res=*this;
        for(auto&x:res)x=-x;
        return res;
    }
    fps pre(int sz)const{
        return fps(this->begin(),this->begin()+min((int)this->size(),sz));
    }
    //[0,n)
    fps inv(int deg=-1)const{
        assert((*this).size()&&(*this)[0].val!=0);
        if(deg==-1)deg=this->size();
        fps res{(*this)[0].inv()};
        for(int len=1;len<deg;len*=2){
            int nxt=len*2;
            auto F=(*this).pre(len*2);
            auto res2=res;
            F.resize(nxt);
            res2.resize(nxt);
            BF(F);
            BF(res2);
            rep(i,res2.size())F[i]*=res2[i];
            IBF(F);
            mint inv=1/mint(nxt);F*=inv;
            rep(i,len)F[i]=0;REP(i,len,len*2)F[i]=-F[i];
            BF(F);
            rep(i,res2.size())F[i]*=res2[i];
            IBF(F);
            F*=inv;
            res.resize(len*2);
            REP(i,len,len*2)res[i]=F[i];
        }
        res.resize(deg);
        return res;
    }
    fps diff()const{
        auto res=*this;
        rep(i,res.size())res[i]*=i;
        if(res.size())res.erase(res.begin());
        return res;
    }
    fps integral()const{
        fps res(this->size()+1);
        rep(i,this->size())res[i+1]=(*this)[i]*binom<mint>::inv(i+1);
        return res;
    }
    fps log(int deg=-1)const{
        if(deg==-1)deg=this->size();
        assert(this->size()&&(*this)[0]==1);
        return (this->diff().pre(deg)*this->inv(deg)).integral().pre(deg);
    }
    fps exp(int deg=-1)const{
        assert((*this).size()&&(*this)[0]==0); 
        if(deg==-1)deg=this->size();
        fps res{1},h{1};
        for(int len=1;len<deg;len*=2){
            int nxt=len*2;
            mint inv=1/mint(nxt);
            fps G=res;G.resize(nxt);BF(G);
            fps H=h;H.resize(nxt);BF(H);
            fps V=G;
            rep(i,nxt)V[i]*=H[i];
            IBF(V);rep(i,nxt)V[i]*=inv;
            rep(i,len)V[i]=0;REP(i,len,nxt)V[i]=-V[i];
            BF(V);
            fps g_prime(nxt);
            rep(i,len-1)g_prime[i]=res[i+1]*(i+1);
            BF(g_prime);
            fps f_prime(nxt);
            rep(i,len)if(i+1<(int)this->size())f_prime[i]=(*this)[i+1]*(i+1);
            BF(f_prime);
            fps w_prime(nxt);
            rep(i,nxt)w_prime[i]=g_prime[i]*H[i]+f_prime[i]*V[i];
            IBF(w_prime);w_prime*=inv;
            DREP(i,nxt-1,len){
                mint fi=(i<(int)this->size())?(*this)[i]:0;
                w_prime[i]=fi-w_prime[i-1]*binom<mint>::inv(i);
            }
            rep(i,len)w_prime[i]=0;
            BF(w_prime);
            rep(i,nxt)G[i]*=w_prime[i];
            IBF(G);G*=inv;
            res.resize(nxt);
            REP(i,len,nxt)res[i]=G[i];
            if(nxt<deg){
                rep(i,nxt)H[i]*=(V[i]-w_prime[i]);
                IBF(H);rep(i,nxt)H[i]*=inv;
                h.resize(nxt);
                REP(i,len,nxt)h[i]=H[i];
            }
        }
        res.resize(deg);
        return res;
    }
    fps pow(ll m,int deg=-1)const{
        if(deg==-1)deg=this->size();
        if(m==0){fps res(deg);res[0]=1;return res;}
        rep(i,this->size()){
            if(m*i>=deg)return fps(deg);
            if((*this)[i].val){
                auto target=(*this)>>i;
                mint coef=mint((*this)[i]);
                mint inv=1/coef;
                target*=inv; 
                return ((m*target.log(deg-i*m)).exp(deg-i*m)*mint(coef).pow(m))<<(i*m);
            }
        }
        return fps(deg);
    }
    void extend(int n){
        if(this->size()<n)this->resize(n);
    }
    fps shrink(){
        auto res=(*this);
        while(res.size()&&res.back()==0)res.pop_back();
        return res;
    }
    pair<fps,fps>div(fps f)const{
        if(this->size()<f.size()){ 
            return {{},(*this)}; 
        }
        int qsize=this->size()-f.size()+1;
        auto rthis=*this;reverse(all(rthis));
        auto rf=f;reverse(all(rf));
        auto rq=(rthis.pre(qsize)*rf.inv(qsize)).pre(qsize);
        reverse(all(rq));
        return {rq,(*this-rq*f).shrink()};
    }
    fps div_only(fps f)const{
        if(this->size()<f.size()){ 
            return {{}}; 
        }
        int qsize=this->size()-f.size()+1;
        auto rthis=*this;reverse(all(rthis));
        auto rf=f;reverse(all(rf));
        auto rq=(rthis.pre(qsize)*rf.inv(qsize)).pre(qsize);
        reverse(all(rq));
        return rq;
    }
    
    optional<fps> sqrt(int deg=-1)const{
        if(deg==-1)deg=this->size();
        if((this)->size()==0)return fps(deg);
        if((*this)[0]==0){
            REP(i,1,this->size()){
                if((*this)[i].val){
                    if(i%2)return nullopt; 
                    if(deg-i/2<=0)break;
                    auto res=((*this)>>i).sqrt(deg-i/2);
                    if(!res.has_value())return nullopt;
                    auto ret=res.value();
                    ret<<=(i/2);
                    ret.resize(deg);
                    return ret;
                }
            }
            return fps(deg);
        }
        ll s=mod_sqrt((*this)[0].val,mint::get_mod());
        if(s==-1)return nullopt;
        fps res{s};
        mint inv=1/mint(2);
        for(int i=1;i<deg;i*=2){
            res=(res+(*this).pre(i<<1)*res.inv(i<<1))*inv;
        }
        return res.pre(deg);
    }
    //f(x) -> f(vx)
    fps substitute(mint v){
        mint coef=1;
        fps nxt(*this);rep(i,nxt.size()){
            nxt[i]*=coef;
            coef*=v;
        }
        return nxt;
    }
};
#endif
#endif
template<class mint>
vc<mint>enumerate_pow(int N,ll k){
    vc<mint>f(N+1);
    vc<uint16_t>lp(N+1);
    vc<int>ps;
    if(N>=1)f[1]=1;
    for(int i=2;i<=N;i++){
        if(!lp[i]){
            ps.pb(i);
            f[i]=mint(i).pow(k);
        }
        for(auto p:ps){
            ll x=1LL*i*p;
            if(x>N)break;
            lp[x]=p;
            f[x]=f[i]*f[p];
            if(lp[i]&&p==lp[i])break;
        }
    }
    return f;
}
//B_n^{-}
//[1,1/2,1/6,0,-1/30,0,5/66,0,...]
template<class mint>
vc<mint>bernoulli(int n){
    fps<mint>g(n);rep(i,n)g[i]=binom<mint>::invfact(i+1);
    g=g.inv(n);
    rep(i,n)g[i]*=binom<mint>::fact(i);
    return g;
}
template<class mint>
fps<mint>prefixsum_poly(fps<mint>f){
    int n=f.size();
    auto B=bernoulli<mint>(n+1);
    rep(i,B.size())B[i]*=binom<mint>::invfact(i);reverse(all(B));
    rep(i,n)f[i]*=binom<mint>::fact(i);

    auto fb=f*B;
    fps<mint>ans(n+1);
    REP(i,1,n+1){
        ans[i]=fb[i+n-1]*binom<mint>::invfact(i);
    }
    return ans;
}
using mint=static_modint<998244353>;
using B=binom<mint>;
void solve(){
    INT(n);
    fps<mint>f(n);rep(i,n)cin>>f[i];
    auto ans=prefixsum_poly(f);rep(i,ans.size())cout<<ans[i]<<" \n"[i+1==ans.size()];
}
signed main(){
    int t=1;
    // cin >> t;
    while(t--)solve();
}