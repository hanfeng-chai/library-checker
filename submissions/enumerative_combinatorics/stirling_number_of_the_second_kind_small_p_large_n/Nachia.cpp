#define PROBLEM "https://judge.yosupo.jp/problem/stirling_number_of_the_second_kind_small_p_large_n"
#include <vector>

namespace nachia {

template<class Modint>
struct StirlingNumber2SqmodP {
    int MOD;
    std::vector<std::vector<Modint>> s;
    std::vector<Modint> f;
    std::vector<Modint> finv;
    StirlingNumber2SqmodP(){}
    StirlingNumber2SqmodP(int mod)
        : MOD(mod), s(mod), f(mod), finv(mod)
    {
        f[0] = Modint(1);
        for(int i=1; i<mod; i++) f[i] = f[i-1] * Modint::raw(i);
        finv[mod-1] = -f[0];
        for(int i=mod-1; i>=1; i--) finv[i-1] = finv[i] * Modint::raw(i);
        for(int i=0; i<mod; i++) s[i].resize(i+1, Modint::raw(0));
        s[0][0] = f[0];
        for(int n=1; n<mod; n++){
            for(int k=0; k<n; k++) s[n][k+1] = s[n-1][k];
            for(int k=0; k<n; k++) s[n][k] += s[n-1][k] * Modint::raw(k);
        }
    }
    Modint comb(long long n, long long k) const {
        if(n < 0 || n < k || k < 0) return Modint::raw(0);
        Modint ans = f[0];
        while(n != 0 && k != 0){
            auto nl = n % MOD; n /= MOD;
            auto kl = k % MOD; k /= MOD;
            if(nl < kl) return Modint::raw(0);
            ans *= f[nl] * finv[kl] * finv[nl-kl];
        }
        return ans;
    }
    Modint operator()(long long n, long long k) const {
        if(n < 0 || n < k || k < 0) return Modint::raw(0);
        if(n < MOD) return s[n][k];
        if(k == 0) return Modint::raw(0);
        auto ak = k / MOD;
        auto bk = k % MOD;
        auto n2 = n - ak;
        auto an = (n2-1) / (MOD - 1);
        auto bn = (n2-1) % (MOD - 1) + 1;
        if(bk == 0){
            if(bn == MOD-1) return comb(an, ak-1);
            return Modint::raw(0);
        }
        if(bn < bk) return Modint::raw(0);
        return comb(an, ak) * s[bn][bk];
    }
};

}
#include <cassert>

#include <utility>
#include <algorithm>
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
public:
    DynamicModSupplier(unsigned int MOD = 998244353) : mod(MOD) {
        assert(2 <= MOD);
        assert(MOD < (1u << 31));
        imod = (u64)(-1) / mod + 1;
    }
    Int reduce(u64 z) const noexcept {
        Int v = reduce2(z);
        if(mod <= v) v += mod;
        return v;
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

//#include "nachia/counting/stirling-number-2-sqmodp.hpp"
//#include "nachia/modulo/dynamic-modint.hpp"
#include <iostream>

int main(){
    std::ios::sync_with_stdio(false); std::cin.tie(nullptr);
    using Modint = nachia::DynamicModint<0>;
    int T, p; std::cin >> T >> p;
    Modint::setMod(p);
    auto ds = nachia::StirlingNumber2SqmodP<Modint>(Modint::mod());
    for(int t=0; t<T; t++){
        long long n, k; std::cin >> n >> k;
        auto ans = ds(n,k);
        std::cout << ans.val() << '\n';
    }
    return 0;
}
