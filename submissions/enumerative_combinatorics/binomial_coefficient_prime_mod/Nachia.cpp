#line 2 "D:\\Programming\\VSCode\\competitive-cpp\\nachia\\math-modulo\\dynamic-mod-supply.hpp"
#include <cassert>
#line 2 "D:\\Programming\\VSCode\\competitive-cpp\\nachia\\math\\ext-gcd.hpp"

#include <utility>
#include <algorithm>
#line 6 "D:\\Programming\\VSCode\\competitive-cpp\\nachia\\math\\ext-gcd.hpp"
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
#line 4 "D:\\Programming\\VSCode\\competitive-cpp\\nachia\\math-modulo\\dynamic-mod-supply.hpp"

namespace nachia{

class DynamicModSupplier{
    using u64 = unsigned long long;
    using u128 = unsigned __int128;
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
#line 3 "D:\\Programming\\VSCode\\competitive-cpp\\nachia\\math\\combination-dyn-prime.hpp"
#include <vector>

namespace nachia{

class CombDynPrime{
private:
    using Int = unsigned int;
    DynamicModSupplier dynmod;
    std::vector<unsigned int> F;
    std::vector<unsigned int> iF;
public:
    void extend(unsigned int newN){
        unsigned int prevN = (int)F.size() - 1;
        if(newN >= dynmod.getMod()){
            newN = dynmod.getMod() - 1;
        }
        if(prevN >= newN) return;
        F.resize(newN+1);
        iF.resize(newN+1);
        for(unsigned int i=prevN+1; i<=newN; i++){
            F[i] = dynmod.mul(F[i-1], i);
        }
        iF[newN] = dynmod.inv(F[newN]);
        for(unsigned int i=newN; i>prevN; i--){
            iF[i-1] = dynmod.mul(iF[i], i);
        }
    }
    CombDynPrime(unsigned int mod = 2, unsigned int n = 1)
        : dynmod(mod)
    {
        F.assign(2, 1 % mod);
        iF.assign(2, 1 % mod);
        extend(n);
    }
    unsigned int factorial(int n) const { return F[n]; }
    unsigned int invFactorial(int n) const { return iF[n]; }
    unsigned int invOf(int n) const {
        return dynmod.mul(iF[n], F[n-1]);
    }
    unsigned int comb(int n, int r) const {
        if(n < 0 || n < r || r < 0) return 0;
        return dynmod.mul(dynmod.mul(F[n], iF[r]), iF[n-r]);
    }
    unsigned int invComb(int n, int r) const {
        if(n < 0 || n < r || r < 0) return 0;
        return dynmod.mul(dynmod.mul(iF[n], F[r]), F[n-r]);
    }
    unsigned int perm(int n, int r) const {
        if(n < 0 || n < r || r < 0) return 0;
        return dynmod.mul(F[n], iF[n-r]);
    }
    unsigned int invPerm(int n, int r) const {
        if(n < 0 || n < r || r < 0) return 0;
        return dynmod.mul(iF[n], F[n-r]);
    }
    unsigned int operator()(int n, int r) const {
        return comb(n,r);
    }
};

} // namespace nachia
#line 2 "D:\\Programming\\VSCode\\competitive-cpp\\nachia\\misc\\nyaanio.hpp"
#include <cstring>
#include <type_traits>
#line 5 "D:\\Programming\\VSCode\\competitive-cpp\\nachia\\misc\\nyaanio.hpp"
#include <cstdio>
#include <string>
#include <cstdint>

namespace fastio {
static constexpr int SZ = 1 << 17;
char inbuf[SZ], outbuf[SZ];
int in_left = 0, in_right = 0, out_right = 0;

struct Pre {
    char num[40000];
    constexpr Pre() : num() {
        for (int i = 0; i < 10000; i++) {
        int n = i;
            for (int j = 3; j >= 0; j--) {
                num[i * 4 + j] = n % 10 + '0';
                n /= 10;
            }
        }
    }
} constexpr pre;

inline void load() {
    int len = in_right - in_left;
    memmove(inbuf, inbuf + in_left, len);
    in_right = len + fread(inbuf + len, 1, SZ - len, stdin);
    in_left = 0;
}

inline void flush() {
    fwrite(outbuf, 1, out_right, stdout);
    out_right = 0;
}

inline void skip_space() {
    if (in_left + 32 > in_right) load();
    while (inbuf[in_left] <= ' ') in_left++;
}

inline void rd(char& c) {
    if (in_left + 32 > in_right) load();
    c = inbuf[in_left++];
}
template <typename T>
inline void rd(T& x) {
    if (in_left + 32 > in_right) load();
    char c;
    do c = inbuf[in_left++];
    while (c < '-');
    [[maybe_unused]] bool minus = false;
    if constexpr (std::is_signed<T>::value == true) {
        if (c == '-') minus = true, c = inbuf[in_left++];
    }
    x = 0;
    while (c >= '0') {
        x = x * 10 + (c & 15);
        c = inbuf[in_left++];
    }
    if constexpr (std::is_signed<T>::value == true) {
        if (minus) x = -x;
    }
}

inline void wt(char c) {
    if (out_right > SZ - 32) flush();
    outbuf[out_right++] = c;
}
inline void wt(bool b) {
    if (out_right > SZ - 32) flush();
    outbuf[out_right++] = b ? '1' : '0';
}
inline void wt(const std::string &s) {
    if (out_right + s.size() > SZ - 32) flush();
    if (s.size() > SZ - 32){ fwrite(s.c_str(), 1, s.size(), stdout); return; }
    memcpy(outbuf + out_right, s.data(), sizeof(char) * s.size());
    out_right += s.size();
}
inline void wt(const char* s) {
    wt(std::string(s));
}
template <typename T>
inline void wt(T x) {
    if (out_right > SZ - 32) flush();
    if (!x) {
        outbuf[out_right++] = '0';
        return;
    }
    if constexpr (std::is_signed<T>::value == true) {
        if (x < 0) outbuf[out_right++] = '-', x = -x;
    }
    int i = 12;
    char buf[16];
    while (x >= 10000) {
        memcpy(buf + i, pre.num + (x % 10000) * 4, 4);
        x /= 10000;
        i -= 4;
    }
    if (x < 100) {
        if (x < 10) {
            outbuf[out_right] = '0' + x;
            ++out_right;
        } else {
            uint32_t q = (uint32_t(x) * 205) >> 11;
            uint32_t r = uint32_t(x) - q * 10;
            outbuf[out_right] = '0' + q;
            outbuf[out_right + 1] = '0' + r;
            out_right += 2;
        }
    } else {
        if (x < 1000) {
            memcpy(outbuf + out_right, pre.num + (x << 2) + 1, 3);
            out_right += 3;
        } else {
            memcpy(outbuf + out_right, pre.num + (x << 2), 4);
            out_right += 4;
        }
    }
    memcpy(outbuf + out_right, buf + i + 4, 12 - i);
    out_right += 12 - i;
}

} // namespace fastio

namespace nachia{

struct CInStream{} cin;
template <typename T>
inline CInStream& operator>>(CInStream& c, T& dest){ fastio::rd(dest); return c; }
struct COutStream{
	~COutStream(){ atexit(fastio::flush); }
} cout;
template <typename T>
inline COutStream& operator<<(COutStream& c, const T& src){ fastio::wt(src); return c; }

} // namespace nachia
#line 3 "..\\Main.cpp"

int main(){
    using nachia::cin;
    using nachia::cout;

    int T; cin >> T;
    int m; cin >> m;

    auto comb = nachia::CombDynPrime(m, 10'000'000);

    for(int i=0; i<T; i++){
        int n, k; cin >> n >> k;
        auto ans = comb.comb(n, k);
        cout << ans << '\n';
    }
    return 0;
}
