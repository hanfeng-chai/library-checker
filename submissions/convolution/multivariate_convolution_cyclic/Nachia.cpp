#include <vector>

namespace nachia{

template<unsigned int MOD>
struct PrimitiveRoot{
    using u64 = unsigned long long;
    static constexpr u64 powm(u64 a, u64 i) {
        u64 res = 1, aa = a;
        for( ; i; i /= 2){
            if(i & 1) res = res * aa % MOD;
            aa = aa * aa % MOD;
        }
        return res;
    }
    static constexpr bool ExamineVal(unsigned int g){
        u64 t = MOD - 1;
        for(u64 d=2; d*d<=t; d+=1+(d&1)) if(t % d == 0){
            if(powm(g, (MOD - 1) / d) == 1) return false;
            while(t % d == 0) t /= d;
        }
        if(t != 1) if(powm(g, (MOD - 1) / t) == 1) return false;
        return true;
    }
    static constexpr unsigned int GetVal(){
        for(u64 x=2; x<MOD; x++) if(ExamineVal(x)) return x;
        return 0;
    }
    static const unsigned int val = GetVal();
};

} // namespace nachia

namespace nachia{

int Popcount(unsigned long long c) noexcept {
#ifdef __GNUC__
    return __builtin_popcountll(c);
#else
    c = (c & (~0ull/3)) + ((c >> 1) & (~0ull/3));
    c = (c & (~0ull/5)) + ((c >> 2) & (~0ull/5));
    c = (c & (~0ull/17)) + ((c >> 4) & (~0ull/17));
    c = (c * (~0ull/257)) >> 56;
    return c;
#endif
}

// please ensure x != 0
int MsbIndex(unsigned long long x) noexcept {
#ifdef __GNUC__
    return 63 - __builtin_clzll(x);
#else
    using u64 = unsigned long long;
    int q = (x >> 32) ? 32 : 0;
    auto m = x >> q;
    constexpr u64 hi = 0x8888'8888;
    constexpr u64 mi = 0x1111'1111;
    m = (((m | ~(hi - (m & ~hi))) & hi) * mi) >> 35;
    m = (((m | ~(hi - (x & ~hi))) & hi) * mi) >> 31;
    q += (m & 0xf) << 2;
    q += 0x3333'3333'2222'1100 >> (((x >> q) & 0xf) << 2) & 0xf;
    return q;
#endif
}

// please ensure x != 0
int LsbIndex(unsigned long long x) noexcept {
#ifdef __GNUC__
    return __builtin_ctzll(x);
#else
    return MsbIndex(x & -x);
#endif
}

}

#include <algorithm>

namespace nachia {

template<class mint>
struct NttInterface{

template<class Iter>
void Butterfly(Iter, int) const {}

template<class Iter>
void IButterfly(Iter, int) const {}

template<class Iter>
void BitReversal(Iter a, int N) const {
    for(int i=0, j=0; j<N; j++){
        if(i < j) std::swap(a[i], a[j]);
        for(int k = N>>1; k > (i^=k); k>>=1);
    }
}

};

} // namespace nachia
#include <iterator>
#include <cassert>
#include <array>

namespace nachia{

template <class mint>
struct Ntt : NttInterface<mint> {

using u32 = unsigned int;
using u64 = unsigned long long;
    
static int ceil_pow2(int n) {
    int x = 0;
    while ((1U << x) < (u32)(n)) x++;
    return x;
}
    
static constexpr int bsf_constexpr(unsigned int n) {
    int x = 0;
    while (!(n & (1 << x))) x++;
    return x;
}

struct fft_info {
    static constexpr u32 g = nachia::PrimitiveRoot<mint::mod()>::val;
    static constexpr int rank2 = bsf_constexpr(mint::mod()-1);
    using RootTable = std::array<mint, rank2+1>;
    RootTable root, iroot, rate3, irate3;

    fft_info(){
        root[rank2] = mint(g).pow((mint::mod() - 1) >> rank2);
        iroot[rank2] = root[rank2].inv();
        for(int i=rank2-1; i>=0; i--){
            root[i] = root[i+1] * root[i+1];
            iroot[i] = iroot[i+1] * iroot[i+1];
        }
        mint prod = 1, iprod = 1;
        for(int i=0; i<=rank2-3; i++){
            rate3[i] = root[i+3] * prod;
            irate3[i] = iroot[i+3] * iprod;
            prod *= iroot[i+3];
            iprod *= root[i+3];
        }
    }
};

template<class RandomAccessIterator>
void ButterflyLayered(RandomAccessIterator a, int n, int stride, int repeat) const {
    static const fft_info info;
    int h = n * stride;
    
    while(repeat--){

    int len = 1;
    int p = h;
    if(ceil_pow2(n)%2 == 1){
        p >>= 1;
        for(int i=0; i<p; i++){
            mint l = a[i], r = a[i+p];
            a[i] = l+r; a[i+p] = l-r;
        }
        len <<= 1;
    }
    for( ; p > stride; ){
        p >>= 2;
        mint rot = 1, imag = info.root[2];
        u64 mod2 = u64(mint::mod()) * mint::mod();
        int offset = p;
        for(int s=0; s<len; s++){
            if(s) rot *= info.rate3[LsbIndex(~(u32)(s-1))];
            mint rot2 = rot * rot;
            mint rot3 = rot2 * rot;
            for(int i=offset-p; i<offset; i++){
                u64 a0 = u64(a[i].val());
                u64 a1 = u64(a[i+p].val()) * rot.val();
                u64 a2 = u64(a[i+2*p].val()) * rot2.val();
                u64 a3 = u64(a[i+3*p].val()) * rot3.val();
                u64 a1na3imag = u64(mint(a1 + mod2 - a3).val()) * imag.val();
                u64 na2 = mod2 - a2;
                a[i] = a0 + a2 + a1 + a3;
                a[i+1*p] = a0 + a2 + (2 * mod2 - (a1 + a3));
                a[i+2*p] = a0 + na2 + a1na3imag;
                a[i+3*p] = a0 + na2 + (mod2 - a1na3imag);
            }
            offset += p << 2;
        }
        len <<= 2;
    }
    
    a += h;
    }
}

template<class RandomAccessIterator>
void Butterfly(RandomAccessIterator a, int n) const {
    ButterflyLayered(a, n, 1, 1);
}

template<class RandomAccessIterator>
void IButterflyLayered(RandomAccessIterator a, int n, int stride, int repeat) const {

    static const fft_info info;
    constexpr int MOD = mint::mod();
    
    while(repeat--){
    
    int len = n;
    int p = stride;

    for( ; 2 < len; ){
        len >>= 2;
        mint irot = 1, iimag = info.iroot[2];
        int offset = p;
        for(int s=0; s<len; s++){
            if(s) irot *= info.irate3[LsbIndex(~(u32)(s-1))];
            mint irot2 = irot * irot;
            mint irot3 = irot2 * irot;
            for(int i=offset-p; i<offset; i++){
                u64 a0 = a[i].val();
                u64 a1 = a[i+p].val();
                u64 a2 = a[i+2*p].val();
                u64 a3 = a[i+3*p].val();
                u64 a2na3iimag = mint((a2 + MOD - a3) * iimag.val()).val();
                a[i] = a0 + a1 + a2 + a3;
                a[i+p] = (a0 + (MOD - a1) + a2na3iimag) * irot.val();
                a[i+2*p] = (a0 + a1 + (MOD - a2) + (MOD - a3)) * irot2.val();
                a[i+3*p] = (a0 + (MOD - a1) + (MOD - a2na3iimag)) * irot3.val();
            }
            offset += p << 2;
        }
        p <<= 2;
    }
    if(len == 2){
        for(int i=0; i<p; i++){
            mint l = a[i], r = a[i+p];
            a[i] = l+r; a[i+p] = l-r;
        }
        p <<= 1;
    }
    
    a += p;
    }
}

template<class RandomAccessIterator>
void IButterfly(RandomAccessIterator a, int n) const {
    IButterflyLayered(a, n, 1, 1);
}

};

} // namespace nachia

#include <utility>
namespace nachia{

// ax + by = gcd(a,b)
// return ( x, - )
std::pair<long long, long long> ExtGcd(long long a, long long b){
    long long x = 1, y = 0;
    while(b){
        long long u = a / b;
        std::swap(a-=b*u, b);
        std::swap(x-=y*u, y);
    }
    return std::make_pair(x, a);
}

} // namespace nachia

namespace nachia{

class DynamicModSupplier{
    using u64 = unsigned long long;
    using Int = unsigned int;
private:
    u64 imod;
    Int mod;
    // atcoder library
    u64 reduce2(u64 z) const noexcept {
        // atcoder library
#ifdef _MSC_VER
        u64 x; _umul128(z, im, &x);
#else
        using u128 = unsigned __int128;
        u64 x = (u64)(((u128)(z)*imod) >> 64);
#endif
        return z - x * mod;
    }
    Int reduce(u64 z) const noexcept {
        Int v = reduce2(z);
        if(mod <= v) v += mod;
        return v;
    }
public:
    DynamicModSupplier(unsigned int MOD = 998244353) : mod(MOD) {
        assert(2 <= MOD);
        assert(MOD < (1u << 31));
        imod = (u64)(-1) / mod + 1;
    }
    Int add(Int a, Int b) const { a += b; if(a >= mod){ a -= mod; } return a; }
    Int sub(Int a, Int b) const { a -= b; if(a >= mod){ a += mod; } return a; }
    Int mul(Int a, Int b) const { return reduce((u64)a * b); }
    Int muladd(Int a, Int b, Int c) const { return reduce((u64)a * b + c); }
    Int inv(Int a) const {
        Int v = ExtGcd(a, mod).first;
        return (v < mod) ? v : (v + mod);
    }
    Int pow(Int a, u64 i) const {
        Int r = a, ans = 1;
        while(i){
            if(i & 1) ans = mul(ans, r);
            i /= 2;
            r = mul(r, r);
        }
        return ans;
    }
    Int getMod() const { return mod; }
};

} // namespace nachia

namespace nachia{

template<class FinishType>
struct GarnerMod{
    using Int = unsigned int;
    using IntLong = unsigned long long;
    std::vector<Int> mods;
    std::vector<DynamicModSupplier> dynmods;
    std::vector<std::vector<Int>> table_coeff;
    std::vector<Int> table_coeffinv;

    void precalc(std::vector<Int> new_mods){
        mods = std::move(new_mods);
        dynmods.resize(mods.size());
        for(size_t i=0; i<mods.size(); i++) dynmods[i] = DynamicModSupplier(mods[i]);
        int nmods = mods.size();
        table_coeff.assign(nmods+1, std::vector<Int>(nmods, 1));
        for(int j=0; j<nmods; j++){
            for(int k=0; k<nmods; k++) table_coeff[j+1][k] = table_coeff[j][k];
            for(int k=j+1; k<nmods; k++) table_coeff[j+1][k] = dynmods[k].mul(table_coeff[j+1][k], mods[j] % mods[k]);
        }
        table_coeffinv.resize(nmods);
        for(int i=0; i<nmods; i++) table_coeffinv[i] = dynmods[i].inv(table_coeff[i][i]);
    }

    FinishType calc(const std::vector<Int>& x){
        int nmods = mods.size();
        std::vector<Int> table_const(nmods);
        FinishType res = 0;
        FinishType res_coeff = 1;
        for(int j=0; j<nmods; j++){
            Int t = dynmods[j].mul(dynmods[j].sub(x[j], table_const[j]), table_coeffinv[j]);
            for(int k=j+1; k<nmods; k++){
                table_const[k] = dynmods[k].muladd(t, table_coeff[j][k], table_const[k]);
            }
            res += res_coeff * FinishType(t);
            res_coeff *= mods[j];
        }
        return res;
    }

    std::vector<FinishType> calc(std::vector<std::vector<Int>> x){
        int n = x[0].size(), m = x.size();
        std::vector<FinishType> res(n);
        std::vector<Int> buf(m);
        for(int i=0; i<n; i++){
            for(int j=0; j<m; j++) buf[j] = x[j][i];
            res[i] = calc(buf);
        }
        return res;
    }
};

} // namespace nachia

namespace nachia{

template<unsigned int MOD>
struct StaticModint{
private:
    using u64 = unsigned long long;
    unsigned int x;
public:

    using my_type = StaticModint;
    template< class Elem >
    static Elem safe_mod(Elem x){
        if(x < 0){
            if(0 <= x+MOD) return x + MOD;
            return MOD - ((-(x+MOD)-1) % MOD + 1);
        }
        return x % MOD;
    }

    StaticModint() : x(0){}
    StaticModint(const my_type& a) : x(a.x){}
    StaticModint& operator=(const my_type&) = default;
    template< class Elem >
    StaticModint(Elem v) : x(safe_mod(v)){}
    unsigned int operator*() const noexcept { return x; }
    my_type& operator+=(const my_type& r) noexcept { auto t = x + r.x; if(t >= MOD) t -= MOD; x = t; return *this; }
    my_type operator+(const my_type& r) const noexcept { my_type res = *this; return res += r; }
    my_type& operator-=(const my_type& r) noexcept { auto t = x + MOD - r.x; if(t >= MOD) t -= MOD; x = t; return *this; }
    my_type operator-(const my_type& r) const noexcept { my_type res = *this; return res -= r; }
    my_type operator-() const noexcept { my_type res = *this; res.x = ((res.x == 0) ? 0 : (MOD - res.x)); return res; }
    my_type& operator*=(const my_type& r)noexcept { x = (u64)x * r.x % MOD; return *this; }
    my_type operator*(const my_type& r) const noexcept { my_type res = *this; return res *= r; }
    my_type pow(unsigned long long i) const noexcept {
        my_type a = *this, res = 1;
        while(i){ if(i & 1){ res *= a; } a *= a; i >>= 1; }
        return res;
    }
    my_type inv() const { return my_type(ExtGcd(x, MOD).first); }
    unsigned int val() const noexcept { return x; }
    static constexpr unsigned int mod() { return MOD; }
    static my_type raw(unsigned int val) noexcept { auto res = my_type(); res.x = val; return res; }
    my_type& operator/=(const my_type& r){ return operator*=(r.inv()); }
    my_type operator/(const my_type& r) const { return operator*(r.inv()); }
};

} // namespace nachia
#include <initializer_list>

namespace nachia{

bool IsPrime(unsigned long long x) noexcept {
    if(x <= 1) return false;
    if(x % 2 == 0) return x == 2;
    using u64 = unsigned long long;
    using u128 = __uint128_t;
    u64 d = x-1;
    int s = 0;
    int q = 63;
    while(!(d&1)){ d >>= 1; s++; }
    while(!(d >> q)) q--;
    u64 r = x; for(int t=0; t<6; t++) r*=2-r*x;
    u128 n2 = -(u128)x % x;
    auto red = [=](u128 t) noexcept -> u64 {
        t = (t + (u128)((u64)t*-r)*x) >> 64;
        return (t >= x) ? t-x : t;
    };
    u64 one = red(n2);
    for(u64 base : { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 }){
        if(base%x==0) continue;
        u64 a = base = red(base%x*n2);
        for(int e=q-1; e>=0; e--){ a = red((u128)a*a); if((d>>e)&1) a = red((u128)a*base); }
        if(a == one) continue;
        for(int t=1; t<s&&a!=x-one; t++) a = red((u128)a*a);
        if(a != x-one) return false;
    }
    return true;
}

} // namespace nachia

namespace nachia{

std::vector<std::pair<unsigned long long, int>> Factorize(unsigned long long x){
    if(x == 1) return {};
    if(IsPrime(x)) return {{x,1}};
    using u64 = unsigned long long;
    using u128 = __uint128_t;
    u64 X = x;
    std::vector<u64> p;
    for(u64 i=2; i<100; i+=1+i%2) if(x%i==0){ p.push_back(i); while(x%i==0) x/=i; }
    u64 r=1; u128 n2=1;
    auto updX = [&](){
        r = x; for(int t=0; t<6; t++) r*=2-r*x;
        n2 = -(u128)x % x;
    };
    auto red = [&](u128 t) noexcept -> u64 {
        u64 s = ((u128)x*((u64)t*r)) >> 64;
        u64 t2 = t >> 64;
        return t2-s + (t2 < s ? x : 0);
    };
    auto mult = [&](u64 a, u64 b) noexcept { return red((u128)red((u128)a*n2)*b); };
    auto gcd = [](u64 a, u64 b) noexcept {
        if(!a || !b) return a|b;
        int q = LsbIndex(a|b);
        b >>= LsbIndex(b);
        a >>= LsbIndex(a);
        while(a!=b){
            if(a<b){ b-=a; b>>=LsbIndex(b); }
            else{ a-=b; a>>=LsbIndex(a); }
        }
        return a<<q;
    };
    static u64 v = 7001;
    p.push_back(x);
    for(int pi=p.size()-1; pi<(int)p.size(); pi++) while(p[pi] != 1 && !IsPrime(p[pi])){
        x = p[pi]; updX();
        while(p[pi] == x){
            v^=v<<13; v^=v>>7; v^=v<<17; // Xorshift https://www.jstatsoft.org/article/download/v008i14/916
            u64 c = red(v); if(c == 0) continue;
            auto f = [=](u64 a) noexcept -> u64 { return red((u128)a*a+c); };
            u64 a=0, b=f(a);
            u64 buf = 1, sz = 1, nx = 10;
            while(true){
                while(nx != sz && a != b){
                    buf = mult(buf, a<=b?b-a:a-b); sz++;
                    a = f(a); b = f(f(b));
                }
                u64 g = gcd(buf, x);
                if(g != 1){
                    while(p[pi] % g == 0) p[pi] /= g;
                    p.push_back(g);
                    break;
                }
                if(a == b) break;
                nx = sz * 3 / 2;
            }
        }
    }
    std::vector<std::pair<u64, int>> res;
    for(u64 q : p) if(q != 1){
        int e=0; while(X%q == 0){ e++; X/=q; }
        if(e) res.push_back({ q, e });
    }
    return res;
}

unsigned long long Totient(unsigned long long x){
    auto F = Factorize(x);
    for(auto f : F) x -= x / f.first;
    return x;
}

} // namespace nachia
#include <cstdint>

namespace nachia{

unsigned long long PrimitiveRootDyn(unsigned long long p){
    using u64 = unsigned long long;
    using u128 = __uint128_t;
    assert(IsPrime(p));
    if(p == 2) return 1;
    auto F = Factorize(p - 1);
    u64 r = p; for(int t=0; t<6; t++) r*=2-r*p;
    u64 n2 = -(u128)p % p;
    auto red = [&](u128 t) noexcept -> u64 {
        t = (t + (u128)((u64)t*-r)*p) >> 64;
        return (t >= p) ? t-p : t;
    };
    auto mult = [&](u64 a, u64 b) noexcept { return red((u128)red((u128)a*b)*n2); };
    auto powm = [&](u64 a, u64 i) noexcept {
        u64 b = 1;
        while(i){ if(i&1){ b=mult(a,b); } a=mult(a,a); i/=2; }
        return b;
    };
    //static u64 v = 7001;
    u64 v = 1;
    while(true){
        //v^=v<<13; v^=v>>7; v^=v<<17; // Xorshift https://www.jstatsoft.org/article/download/v008i14/916
        v++;
        u64 vv = v%p;
        if(vv == 0) continue;
        bool ok = true;
        for(auto f : F){
            u64 i = (p-1)/f.first;
            if(powm(vv,i) == 1){ ok = false; break; }
        }
        if(ok) break;
    }
    return v%p;
}

} // namespace nachia

namespace nachia{

template<class Modint, unsigned int NewMod>
std::vector<unsigned int> MultidimCyclicConvolutionBasicTransform(
    const std::vector<unsigned int>& A,
    const std::vector<Modint>& B,
    int s, int z, int r
){
    using Modint2 = StaticModint<NewMod>;
    static Ntt<Modint2> ntt;
    int lgz2 = 0;
    while((1 << lgz2) < z*2) lgz2++;
    std::vector<Modint2> A2((s * r) << lgz2);
    for(int i=0; i<r; i++) for(int j=0; j<s*z; j++){
        A2[(i<<lgz2) * s + j] = Modint2(A[i*z*s + j]);
    }
    std::vector<Modint2> B2(1<<lgz2);
    for(int i=0; i<z*2-1; i++) B2[i] = Modint2(B[i].val());
    ntt.ButterflyLayered(A2.begin(), 1<<lgz2, s, r);
    ntt.Butterfly(B2.begin(), 1<<lgz2);
    for(int i=0; i<r; i++) for(int j=0; j<(1<<lgz2); j++){
        for(int k=0; k<s; k++) A2[((i<<lgz2)+j)*s+k] *= B2[j];
    }
    ntt.IButterflyLayered(A2.begin(), 1<<lgz2, s, r);
    Modint2 invlen = Modint2(1 << lgz2).inv();
    std::vector<unsigned int> res(A.size());
    for(int i=0; i<r; i++) for(int j=0; j<s*z; j++){
        res[i*z*s + j] = (A2[((i<<lgz2)+z-1) * s + j] * invlen).val();
    }
    return res;
}

template<class Modint>
void MultidimCyclicConvolutionBasicTransform2(
    std::vector<Modint>& A,
    int s, int z, int r,
    Modint g,
    GarnerMod<Modint>& garner
){
    Modint f = g.pow((Modint::mod()-1) / z);
    if(z <= 100){
        std::vector<Modint> ff(z); ff[0] = 1;
        for(int i=1; i<z; i++) ff[i] = ff[i-1] * f;
        for(int i=0; i<r; i++){
            std::vector<Modint> buf(z*s);
            for(int j=z-1; j>=0; j--) for(int k=0; k<z; k++) for(int l=0; l<s; l++){
                buf[k*s+l] = buf[k*s+l] * ff[k] + A[i*z*s+j*s+l];
            }
            for(int j=0; j<z*s; j++) A[i*z*s+j] = buf[j];
        }
        return;
    }
    std::vector<Modint> fp(z+1);
    fp[0] = 1;
    for(int i=1; i<=z; i++) fp[i] = fp[i-1] * f;
    std::vector<Modint> iwt(z); iwt[0] = 1;
    for(int i=1; i<z; i++) iwt[i] = iwt[i-1] * fp[z-i];
    std::vector<Modint> wt(z*2); wt[0] = 1;
    for(int i=1; i<z; i++) wt[i] = wt[i-1] * fp[i];
    for(int i=0; i<z; i++) wt[z+i] = wt[z+i-1] * fp[i];
    std::vector<unsigned int> abuf(A.size());
    for(int i=0; i<r; i++){
        for(int j=0; j<z; j++) for(int k=0; k<s; k++){
            abuf[(i*z+z-1-j)*s+k] = (A[(i*z+j)*s+k] * iwt[j]).val();
        }
    }
    std::vector<std::vector<unsigned int>> tbuf;
    tbuf.push_back(MultidimCyclicConvolutionBasicTransform<Modint,1107296257>(abuf, wt, s, z, r));
    tbuf.push_back(MultidimCyclicConvolutionBasicTransform<Modint,1711276033>(abuf, wt, s, z, r));
    tbuf.push_back(MultidimCyclicConvolutionBasicTransform<Modint,1811939329>(abuf, wt, s, z, r));
    A = garner.calc(std::move(tbuf));
    for(int i=0; i<r; i++) for(int j=0; j<z; j++){
        for(int k=0; k<s; k++) A[(i*z+j)*s+k] *= iwt[j];
    }
}

template<class Modint>
void MultidimCyclicConvolutionTransform(
    std::vector<Modint>& A,
    const std::vector<int>& base,
    Modint g
){
    if (base.size() == 0) return;
    GarnerMod<Modint> garner;
    garner.precalc(std::vector<unsigned int>{ 1107296257, 1711276033, 1811939329 });
    int n = 1;
    for(auto b : base) n *= b;
    int stride = 1;
    int repeat = n;
    for(auto b : base){
        repeat /= b;
        MultidimCyclicConvolutionBasicTransform2(A, stride, b, repeat, g, garner);
        stride *= b;
    }
}

template<class Modint>
std::vector<Modint> MultidimCyclicConvolution(
    std::vector<Modint> A,
    std::vector<Modint> B,
    const std::vector<int>& base
){
    int n = 1;
    for(auto b : base) n *= b;
    auto promitiveRoot = Modint(PrimitiveRootDyn(Modint::mod()));
    MultidimCyclicConvolutionTransform(A, base, promitiveRoot);
    MultidimCyclicConvolutionTransform(B, base, promitiveRoot);
    for(int i=0; i<n; i++) A[i] *= B[i];
    MultidimCyclicConvolutionTransform(A, base, promitiveRoot.inv());
    auto invlen = Modint(n).inv();
    for(auto& a : A) a *= invlen;
    return A;
}

} // namespace nachia
#include <cstdio>
#include <cctype>
#include <string>

namespace nachia{

struct CInStream{
private:
	static const unsigned int INPUT_BUF_SIZE = 1 << 17;
	unsigned int p = INPUT_BUF_SIZE;
	static char Q[INPUT_BUF_SIZE];
public:
	using MyType = CInStream;
	char seekChar(){
		if(p == INPUT_BUF_SIZE){
			size_t len = fread(Q, 1, INPUT_BUF_SIZE, stdin);
			if(len != INPUT_BUF_SIZE) Q[len] = '\0';
			p = 0;
		}
		return Q[p];
	}
	void skipSpace(){ while(isspace(seekChar())) p++; }
private:
	template<class T, int sp = 1>
	T nextUInt(){
		if constexpr (sp) skipSpace();
		T buf = 0;
		while(true){
			char tmp = seekChar();
			if('9' < tmp || tmp < '0') break;
			buf = buf * 10 + (tmp - '0');
			p++;
		}
		return buf;
	}
public:
	uint32_t nextU32(){ return nextUInt<uint32_t>(); }
	int32_t nextI32(){
		skipSpace();
		if(seekChar() == '-'){
			p++; return (int32_t)(-nextUInt<uint32_t, 0>());
		}
		return (int32_t)nextUInt<uint32_t, 0>();
	}
	uint64_t nextU64(){ return nextUInt<uint64_t>();}
	int64_t nextI64(){
		skipSpace();
		if(seekChar() == '-'){
			p++; return (int64_t)(-nextUInt<int64_t, 0>());
		}
		return (int64_t)nextUInt<int64_t, 0>();
	}
	template<class T>
	T nextInt(){
		skipSpace();
		if(seekChar() == '-'){
			p++;
			return - nextUInt<T, 0>();
		}
		return nextUInt<T, 0>();
	}
	char nextChar(){ skipSpace(); char buf = seekChar(); p++; return buf; }
	std::string nextToken(){
		skipSpace();
		std::string buf;
		while(true){
			char ch = seekChar();
			if(isspace(ch) || ch == '\0') break;
			buf.push_back(ch);
			p++;
		}
		return buf;
	}
	MyType& operator>>(unsigned int& dest){ dest = nextU32(); return *this; }
	MyType& operator>>(int& dest){ dest = nextI32(); return *this; }
	MyType& operator>>(unsigned long& dest){ dest = nextU64(); return *this; }
	MyType& operator>>(long& dest){ dest = nextI64(); return *this; }
	MyType& operator>>(unsigned long long& dest){ dest = nextU64(); return *this; }
	MyType& operator>>(long long& dest){ dest = nextI64(); return *this; }
	MyType& operator>>(std::string& dest){ dest = nextToken(); return *this; }
	MyType& operator>>(char& dest){ dest = nextChar(); return *this; }
} cin;

struct FastOutputTable{
	char LZ[1000][4] = {};
	char NLZ[1000][4] = {};
	constexpr FastOutputTable(){
		using u32 = uint_fast32_t;
		for(u32 d=0; d<1000; d++){
			LZ[d][0] = ('0' + d / 100 % 10);
			LZ[d][1] = ('0' + d /  10 % 10);
			LZ[d][2] = ('0' + d /   1 % 10);
			LZ[d][3] = '\0';
		}
		for(u32 d=0; d<1000; d++){
			u32 i = 0;
			if(d >= 100) NLZ[d][i++] = ('0' + d / 100 % 10);
			if(d >=  10) NLZ[d][i++] = ('0' + d /  10 % 10);
			if(d >=   1) NLZ[d][i++] = ('0' + d /   1 % 10);
			NLZ[d][i++] = '\0';
		}
	}
};

struct COutStream{
private:
	using u32 = uint32_t;
	using u64 = uint64_t;
	using MyType = COutStream;
	static const u32 OUTPUT_BUF_SIZE = 1 << 17;
	static char Q[OUTPUT_BUF_SIZE];
	static constexpr FastOutputTable TB = FastOutputTable();
	u32 p = 0;
	static constexpr u32 P10(u32 d){ return d ? P10(d-1)*10 : 1; }
	static constexpr u64 P10L(u32 d){ return d ? P10L(d-1)*10 : 1; }
	template<class T, class U> static void Fil(T& m, U& l, U x){ m = l/x; l -= m*x; }
public:
	void next_dig9(u32 x){
		u32 y;
		Fil(y, x, P10(6));
		nextCstr(TB.LZ[y]);
		Fil(y, x, P10(3));
		nextCstr(TB.LZ[y]); nextCstr(TB.LZ[x]);
	}
	void nextChar(char c){
		Q[p++] = c;
		if(p == OUTPUT_BUF_SIZE){ fwrite(Q, p, 1, stdout); p = 0; }
	}
	void nextEoln(){ nextChar('\n'); }
	void nextCstr(const char* s){ while(*s) nextChar(*(s++)); }
	void nextU32(uint32_t x){
		u32 y = 0;
		if(x >= P10(9)){
			Fil(y, x, P10(9));
			nextCstr(TB.NLZ[y]); next_dig9(x);
		}
		else if(x >= P10(6)){
			Fil(y, x, P10(6));
			nextCstr(TB.NLZ[y]);
			Fil(y, x, P10(3));
			nextCstr(TB.LZ[y]); nextCstr(TB.LZ[x]);
		}
		else if(x >= P10(3)){
			Fil(y, x, P10(3));
			nextCstr(TB.NLZ[y]); nextCstr(TB.LZ[x]);
		}
		else if(x >= 1) nextCstr(TB.NLZ[x]);
		else nextChar('0');
	}
	void nextI32(int32_t x){
		if(x >= 0) nextU32(x);
		else{ nextChar('-'); nextU32((u32)-x); }
	}
	void nextU64(uint64_t x){
		u32 y = 0;
		if(x >= P10L(18)){
			Fil(y, x, P10L(18));
			nextU32(y);
			Fil(y, x, P10L(9));
			next_dig9(y); next_dig9(x);
		}
		else if(x >= P10L(9)){
			Fil(y, x, P10L(9));
			nextU32(y); next_dig9(x);
		}
		else nextU32(x);
	}
	void nextI64(int64_t x){
		if(x >= 0) nextU64(x);
		else{ nextChar('-'); nextU64((u64)-x); }
	}
	template<class T>
	void nextInt(T x){
		if(x < 0){ nextChar('-'); x = -x; }
		if(!(0 < x)){ nextChar('0'); return; }
		std::string buf;
		while(0 < x){
			buf.push_back('0' + (int)(x % 10));
			x /= 10;
		}
		for(int i=(int)buf.size()-1; i>=0; i--){
			nextChar(buf[i]);
		}
	}
	void writeToFile(bool flush = false){
		fwrite(Q, p, 1, stdout);
		if(flush) fflush(stdout);
		p = 0;
	}
	COutStream(){ Q[0] = 0; }
	~COutStream(){ writeToFile(); }
	MyType& operator<<(unsigned int tg){ nextU32(tg); return *this; }
	MyType& operator<<(unsigned long tg){ nextU64(tg); return *this; }
	MyType& operator<<(unsigned long long tg){ nextU64(tg); return *this; }
	MyType& operator<<(int tg){ nextI32(tg); return *this; }
	MyType& operator<<(long tg){ nextI64(tg); return *this; }
	MyType& operator<<(long long tg){ nextI64(tg); return *this; }
	MyType& operator<<(const std::string& tg){ nextCstr(tg.c_str()); return *this; }
	MyType& operator<<(const char* tg){ nextCstr(tg); return *this; }
	MyType& operator<<(char tg){ nextChar(tg); return *this; }
} cout;

char CInStream::Q[INPUT_BUF_SIZE];
char COutStream::Q[OUTPUT_BUF_SIZE];

} // namespace nachia

namespace nachia{

template<unsigned int IDENTIFER, class IDENTIFER2 = void>
class DynamicModint{
    using Int = unsigned int;
    using MyType = DynamicModint;
private:
    static DynamicModSupplier _c;
    Int v;
    template< class Elem >
    static Elem SafeMod(Elem x, Int mod){
        if(x < 0){
            if(0 <= x+mod) return x + mod;
            return mod - ((-(x+mod)-1) % mod + 1);
        }
        return x % mod;
    }
public:
    DynamicModint() : v(0) {}
    DynamicModint(const MyType& r) : v(r.v) {}
    MyType& operator=(const MyType&) = default;
    DynamicModint(long long x){ v = SafeMod(x, _c.getMod()); }
    static MyType raw(Int _v){ MyType res; res.v = _v; return res; }
    static void setMod(DynamicModSupplier sup){ _c = std::move(sup); }
    MyType operator+=(MyType r){ return v = _c.add(v, r.v); }
    MyType operator+(MyType r) const { return raw(_c.add(v, r.v)); }
    MyType operator-=(MyType r){ return v = _c.sub(v, r.v); }
    MyType operator-(MyType r) const { return raw(_c.sub(v, r.v)); }
    MyType operator-() const { return raw(v ? _c.getMod()-v : 0); }
    MyType operator*=(MyType r){ return v = _c.mul(v, r.v); }
    MyType operator*(MyType r) const { return raw(_c.mul(v, r.v)); }
    MyType operator/=(MyType r){ return v = _c.mul(v, _c.inv(r.v)); }
    MyType operator/(MyType r) const { return raw(_c.mul(v, _c.inv(r.v))); }
    MyType inv() const { return raw(_c.inv(v)); }
    MyType pow(unsigned long long r) const { return raw(_c.pow(v, r)); }
    MyType mulAdd(MyType mul, MyType add) const { return raw(_c.muladd(v, mul.v, add.v)); }
    Int val() const { return v; }
    Int operator*() const { return v; }
    static Int mod(){ return _c.getMod(); }
};

template<unsigned int IDENTIFER, class IDENTIFER2>
DynamicModSupplier DynamicModint<IDENTIFER, IDENTIFER2>::_c;

} // namespace nachia

int main(){
    using Modint = nachia::DynamicModint<0>;
    using nachia::cin;
    using nachia::cout;
    int P, K; cin >> P >> K;
    Modint::setMod(P);
    int N = 1;
    std::vector<int> base(K);
    for(int i=0; i<K; i++){
        int b; cin >> b;
        base[i] = b;
        N *= b;
    }
    std::vector<Modint> F(N), G(N);
    for(int i=0; i<N; i++){
        int a; cin >> a; F[i] = Modint::raw(a);
    }
    for(int i=0; i<N; i++){
        int a; cin >> a; G[i] = Modint::raw(a);
    }
    auto ans = nachia::MultidimCyclicConvolution(F, G, base);
    for(int i=0; i<N; i++){
        if(i) cout << ' ';
        cout << ans[i].val();
    } cout << '\n';
    return 0;
}
