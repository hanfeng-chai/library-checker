#include <cassert>
#line 1 "verify/linalg/system.test.cpp"
// @brief System of Linear Equations
#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations"
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2")
#include <bits/stdc++.h>
#line 1 "blazingio/blazingio.min.hpp"
// NOLINTBEGIN
// clang-format off
// DO NOT REMOVE THIS MESSAGE. The mess that follows is a minified build of
// https://github.com/purplesyringa/blazingio. Refer to the repository for
// a human-readable version and documentation.
// Options: cbfoiedrhWLMXaIaAn
#define M$(x,...)_mm256_##x##_epi8(__VA_ARGS__)
#define $u(...)__VA_ARGS__
#if __APPLE__
#define $m(A,B)A
#else
#define $m(A,B)B
#endif
#if _WIN32
#define $w(A,B)A
#else
#define $w(A,B)B
#endif
#if __i386__|_M_IX86
#define $H(A,B)A
#else
#define $H(A,B)B
#endif
#if __aarch64__
#define $a(A,B)A
#else
#define $a(A,B)B
#endif
#define $P(x)void F(x K){
#define $T template<$c T
#define $c class
#define $C constexpr
#define $R return
#define $O operator
#define u$ uint64_t
#define $r $R*this;
#line 41 "blazingio/blazingio.min.hpp"
#include $a(<arm_neon.h>,<immintrin.h>)
#line 43 "blazingio/blazingio.min.hpp"
#include $w(<windows.h>,<sys/mman.h>)
#include<sys/stat.h>
#include $w(<io.h>,<unistd.h>)
#include $w(<ios>,<sys/resource.h>)
#if _MSC_VER
#define __builtin_add_overflow(a,b,c)_addcarry_u64(0,a,b,c)
#define $s
#else
$H(,u$ _umul128(u$ a,u$ b,u$*D){auto x=(__uint128_t)a*b;*D=u$(x>>64);$R(u$)x;})
#define $s $a(,__attribute__((target("avx2"))))
#endif
#define $z $a(16,32)
#define $t $a(uint8x16_t,__m256i)
#define $I $w(__forceinline,__attribute__((always_inline)))
#define $F M(),
#define E$(x)if(!(x))abort();
$w(LONG WINAPI $x(_EXCEPTION_POINTERS*);,)namespace $f{using namespace std;struct B{enum $c A:char{}c;B&$O=(char x){c=A{x};$r}$O char(){$R(char)c;}};$C u$ C=~0ULL/255;struct D{string&K;};static B E[65568];template<int F>struct G{B*H,*S;void K(off_t C){$w(char*D=(char*)VirtualAlloc(0,(C+8191)&-4096,8192,1);E$(D)E$(VirtualFree(D,0,32768))DWORD A=C&-65536;E$(!A||MapViewOfFileEx(CreateFileMapping(GetStdHandle(-10),0,2,0,A,0),4,0,0,0,D)==D)E$(VirtualAlloc(D+A,65536,12288,4)==D+A)E$(~_lseek(0,A,0))DWORD E=0;ReadFile(GetStdHandle(-10),D+A,65536,&E,0);,int A=getpagesize();char*D=(char*)mmap(0,C+A,3,2,0,0);E$(D!=(void*)-1)E$(mmap(D+((C+A-1)&-A),A,3,$m(4114,50),-1,0)!=(void*)-1))H=(B*)D+C;*H=10;H[1]=48;H[2]=0;S=(B*)D;}void L(){H=S=E;}$I void M(){if(F&&S==H){$w(DWORD A=0;ReadFile(GetStdHandle(-10),S=E,65536,&A,0);,$a($u(register long A asm("x0")=0,D asm("x1")=(long)E,G asm("x2")=65536,C asm($m("x16","x8"))=$m(3,63);asm volatile("svc 0" $m("x80",):"+r"(A),"+r"(D):"r"(C),"r"(G));S=launder(E);),off_t A=$H(3,$m(33554435,0));B*D=E;asm volatile($H("int $128","syscall"):"+a"(A),$H("+c"(D):"b","+S"(D):"D")(0),"d"(65536)$H(,$u(:"rcx","r11")));S=D;))H=S+A;*H=10;if(!A)E[1]=48,E[2]=0;}}$T>$I void N(T&x){while($F(*S&240)==48)x=T(x*10+(*S++-48));}$T>$I decltype((void)~T{1})O(T&x){M();int A=is_signed_v<T>&&*S==45;S+=A;N(x=0);x=A?1+~x:x;}$T>$I decltype((void)T{1.})O(T&x){M();int A=*S==45;S+=A;$F S+=*S==43;u$ n=0;int i=0;for(;i<18&&($F*S&240)==48;i++)n=n*10+*S++-48;int B=20;int C=*S==46;S+=C;for(;i<18&&($F*S&240)==48;i++)n=n*10+*S++-48,B-=C;x=(T)n;while(($F*S&240)==48)x=x*10+*S++-48,B-=C;if(*S==46)S++,C=1;while(($F*S&240)==48)x=x*10+*S++-48,B-=C;int D;if((*S|32)==101)S++,$F S+=*S==43,O(D),B+=D;static $C auto E=[](){array<T,41>E{};T x=1;for(int i=21;i--;)E[40-i]=x,E[i]=1/x,x*=10;$R E;}();while(B>40)x*=(T)1e10,B-=10;while(B<0)x*=(T)1e-10,B+=10;x*=E[B];x=A?-x:x;}$I void O(bool&x){$F x=*S++==49;}$I void O(char&x){$F x=*S++;}$I void O(uint8_t&x){$F x=*S++;}$I void O(int8_t&x){$F x=*S++;}$T>$s void P(string&K,T C){M();B*G=S;C();K.assign((char*)G,S-G);while(F&&S==H&&($F H!=E)){C();K.append(E,S);}}$s void O(string&K){P(K,[&]()$s{B*p=S;$w(ULONG R;,)$t x;$a(uint64x2_t A;while(memcpy(&x,p,16),A=uint64x2_t(x<33),!(A[0]|A[1]))p+=16;S=p+(A[0]?0:8)+$w((_BitScanForward64(&R,A[0]?A[0]:A[1]),R),__builtin_ctzll(A[0]?A[0]:A[1]))/8;,int J;$t C=M$(set1,32);while(memcpy(&x,p,32),!(J=M$(movemask,M$(cmpeq,C,_mm256_max_epu8(C,x)))))p+=32;S=p+$w((_BitScanForward(&R,J),R),__builtin_ctz(J));)});}$s void O(D&A){P(A.K,[&](){S=(B*)memchr(S,10,H-S+1);});if(A.K.size()&&A.K.back()==13)A.K.pop_back();if(A.K.empty()||S<H)S+=*S==10;}$T>$I void O(complex<T>&K){T A,B{};if($F*S==40){S++;O(A);if($F*S++==44)Q(B),S++;}else O(A);K={A,B};}template<size_t N>$s void O(bitset<N>&K){if(N>4095&&!*this)$R;ptrdiff_t i=N;while(i)if($F i%$z||H-S<$z)K[--i]=*S++==49;else{B*p=S;for(int64_t j=0;j<min(i,H-S)/$z;j++){i-=$z;$t x;memcpy(&x,p,$z);$a(auto B=(uint8x16_t)vdupq_n_u64(~2ULL/254)&(48-x);auto C=vzip_u8(vget_high_u8(B),vget_low_u8(B));auto y=vaddvq_u16((uint16x8_t)vcombine_u8(C.val[0],C.val[1]));,u$ a=~0ULL/65025;auto y=$w(_byteswap_ulong,__builtin_bswap32)(M$(movemask,M$(shuffle,_mm256_slli_epi32(x,7),_mm256_set_epi64x(a+C*24,a+C*16,a+C*8,a))));)p+=$z;memcpy((char*)&K+i/8,&y,$z/8);}S=p;}}$T>$I void Q(T&K){if(!is_same_v<T,D>)while($F(uint8_t)*S<33)S++;O(K);}$O bool(){$R!!*this;}bool $O!(){$R S>H;}};struct U{G<0>A;G<1>B;U(){struct stat D;E$(~fstat(0,&D))(D.st_mode>>12)==8?A.K(D.st_size):B.L();}U*tie(nullptr_t){$R this;}void sync_with_stdio(bool){}$T>$I U&$O>>(T&K){A.S?A.Q(K):B.Q(K);$r}$O bool(){$R!!*this;}bool $O!(){$R A.S?!A:!B;}};short A[100];char L[64]{1};struct
V{char*D;B*S;int J;V(){$w(E$(D=(char*)VirtualAlloc(0,536870912,8192,4))E$(VirtualAlloc(D,4096,4096,260))AddVectoredExceptionHandler(1,$x);,size_t C=536870912;$m(,rlimit E;getrlimit(RLIMIT_AS,&E);if(~E.rlim_cur)C=25165824;)D=(char*)mmap(0,C,3,$m(4162,16418),-1,0);E$(D!=(void*)-1))S=(B*)D;for(int i=0;i<100;i++)A[i]=short((48+i/10)|((48+i%10)<<8));for(int i=1;i<64;i++)L[i]=L[i-1]+(0x8922489224892249>>i&1);}~V(){flush($w(!J,));}void flush($w(int F=0,)){$w(J=1;auto E=GetStdHandle(-11);auto C=F?ReOpenFile(E,1073741824,7,2684354560):(void*)-1;DWORD A;E$(C==(void*)-1?WriteFile(E,D,DWORD((char*)S-D),&A,0):(WriteFile(C,D,DWORD(((char*)S-D+4095)&-4096),&A,0)&&~_chsize(1,int((char*)S-D)))),auto G=D;ssize_t A;while((A=write(1,G,(char*)S-G))>0)G+=A;E$(~A))S=(B*)D;}$P(char)*S++=K;}$P(uint8_t)*S++=K;}$P(int8_t)*S++=K;}$P(bool)*S++=48+K;}$T>decltype((void)~T{1})F(T K){using D=make_unsigned_t<T>;D C=K;if(K<0)F('-'),C=1+~C;static $C auto N=[](){array<D,5*sizeof(T)/2>N{};D n=1;for(size_t i=1;i<N.size();i++)n*=10,N[i]=n;$R N;}();$w(ULONG M;,)int G=L[$w(($H(_BitScanReverse(&M,ULONG((int64_t)C>>32))?M+=32:_BitScanReverse(&M,(ULONG)C|1),_BitScanReverse64(&M,C|1)),M),63^__builtin_clzll(C|1))];G-=C<N[G-1];short H[20];if $C(sizeof(T)==2){auto n=33555U*C-C/2;u$ H=A[n>>25];n=(n&33554431)*25;H|=A[n>>23]<<16;H|=u$(48+((n&8388607)*5>>22))<<32;H>>=40-G*8;memcpy(S,&H,8);}else if $C(sizeof(T)==4){auto n=1441151881ULL*C;$H(n>>=25;n++;for(int i=0;i<5;i++){H[i]=A[n>>32];n=(n&~0U)*100;},int K=57;auto J=~0ULL>>7;for(int i=0;i<5;i++){H[i]=A[n>>K];n=(n&J)*25;K-=2;J/=4;})memcpy(S,(B*)H+10-G,16);}else{$H($u(if(C<(1ULL<<32)){$R F((uint32_t)C);}auto J=(u$)1e10;auto x=C/J,y=C%J;int K=100000,b[]{int(x/K),int(x%K),int(y/K),int(y%K)};B H[40];for(int i=0;i<4;i++){int n=int((429497ULL*b[i]>>7)+1);B*p=H+i*5;*p=48+char(n>>25);n=(n&~0U>>7)*25;memcpy(p+1,A+(n>>23),2);memcpy(p+3,A+((n&~0U>>9)*25>>21),2);}),$u(u$ D,E=_umul128(18,C,&D),F;_umul128(0x725dd1d243aba0e8,C,&F);D+=__builtin_add_overflow(E,F+1,&E);for(int i=0;i<10;i++)H[i]=A[D],E=_umul128(100,E,&D);))memcpy(S,(B*)H+20-G,20);}S+=G;}$T>decltype((void)T{1.})F(T K){if(K<0)F('-'),K=-K;auto G=[&](){auto x=u$(K*1e12);$H($u(x-=x>999999999999;uint32_t n[]{uint32_t(x/1000000*429497>>7)+1,uint32_t(x%1000000*429497>>7)+1};int K=25,J=~0U>>7;for(int i=0;i<3;i++){for(int j=0;j<2;j++)memcpy(S+i*2+j*6,A+(n[j]>>K),2),n[j]=(n[j]&J)*25;K-=2;J/=4;}S+=12;),$u(u$ D,E=_umul128(472236648287,x,&D)>>8;E|=D<<56;D>>=8;E++;for(int i=0;i<6;i++)memcpy(S,A+D,2),S+=2,E=_umul128(100,E,&D);))};if(K==0)$R F('0');if(K>=1e16){K*=(T)1e-16;int B=16;while(K>=1)K*=(T).1,B++;F("0.");G();F('e');F(B);}else if(K>=1){auto B=(u$)K;F(B);if((K-=(T)B)>0)F('.'),G();}else F("0."),G();}$P(const char*)$w(size_t A=strlen(K);memcpy((char*)S,K,A);S+=A;,S=(B*)stpcpy((char*)S,K);)}$P(const uint8_t*)F((char*)K);}$P(const int8_t*)F((char*)K);}$P(string_view)memcpy(S,K.data(),K.size());S+=K.size();}$T>$P(complex<T>)*this<<'('<<K.real()<<','<<K.imag()<<')';}template<size_t N>$s $P(const bitset<N>&)auto i=N;while(i%$z)*S++=48+K[--i];B*p=S;while(i){i-=$z;$a(short,int)x;memcpy(&x,(char*)&K+i/8,$z/8);$a(auto A=(uint8x8_t)vdup_n_u16(x);vst1q_u8((uint8_t*)p,48-vtstq_u8(vcombine_u8(vuzp2_u8(A,A),vuzp1_u8(A,A)),(uint8x16_t)vdupq_n_u64(~2ULL/254)));,auto b=_mm256_set1_epi64x(~2ULL/254);_mm256_storeu_si256(($t*)p,M$(sub,M$(set1,48),M$(cmpeq,_mm256_and_si256(M$(shuffle,_mm256_set1_epi32(x),_mm256_set_epi64x(0,C,C*2,C*3)),b),b)));)p+=$z;}S=p;}$T>V&$O<<(const T&K){F(K);$r}V&$O<<(V&(*A)(V&)){$R A(*this);}};struct W{$T>W&$O<<(const T&K){$r}W&$O<<(W&(*A)(W&)){$R A(*this);}};}namespace std{$f::U i$;$f::V o$;$f::W e$;$f::U&getline($f::U&B,string&K){$f::D A{K};$R B>>A;}$f::V&flush($f::V&B){if(!i$.A.S)B.flush();$R B;}$f::V&endl($f::V&B){$R B<<'\n'<<flush;}$f::W&endl($f::W&B){$R B;}$f::W&flush($f::W&B){$R B;}}$w(LONG WINAPI $x(_EXCEPTION_POINTERS*A){auto C=A->ExceptionRecord;auto B=C->ExceptionInformation[1];if(C->ExceptionCode==2147483649&&B-(ULONG_PTR)std::o$.D<0x40000000){E$(VirtualAlloc((char*)B,16777216,4096,4)&&VirtualAlloc((char*)(B+16777216),4096,4096,260))$R-1;}$R 0;},)
#define freopen(...)if(freopen(__VA_ARGS__)==stdin)std::i$=$f::U{}
#define cin i$
#define cout o$
#ifdef ONLINE_JUDGE
#define cerr e$
#define clog e$
#endif
// End of blazingio
// NOLINTEND
// clang-format on
#line 1 "cp-algo/linalg/matrix.hpp"


#line 1 "cp-algo/random/rng.hpp"


#include <chrono>
#include <random>
namespace cp_algo::random {
    std::mt19937_64 gen(
        std::chrono::steady_clock::now().time_since_epoch().count()
    );
    uint64_t rng() {
        return gen();
    }
}

#line 1 "cp-algo/math/common.hpp"


#include <functional>
#include <cstdint>
#include <cassert>
#include <bit>
#include <vector>
#include <algorithm>
namespace cp_algo::math {
#ifdef CP_ALGO_MAXN
    const int maxn = CP_ALGO_MAXN;
#else
    const int maxn = 1 << 19;
#endif
    const int magic = 64; // threshold for sizes to run the naive algo

    // Nonnegative 64-bit exponents, with an associative operation and its identity.
    // Windows >1 precompute odd powers only when that saves operations.
    template<int window = 1>
    auto bpow(auto const& x, auto n, auto const& one, auto op) {
        static_assert(window >= 1 && window <= 6);
        if constexpr(window > 1) {
            if(n == 0) {return one;}
            int bits = std::bit_width(uint64_t(n));
            auto low_bit = [&](int high) {
                int low = std::max(0, high - window + 1);
                while(!((n >> low) & 1)) {low++;}
                return low;
            };
            int first = low_bit(bits - 1);
            int cost = (1 << (window - 1)) + first;
            for(int j = first - 1; j >= 0;) {
                if(!((n >> j) & 1)) {j--;}
                else {cost++; j = low_bit(j) - 1;}
            }
            // Do not pay for the table when binary powering uses fewer operations.
            if(cost >= bits + std::popcount(uint64_t(n)) - 2) {return bpow<1>(x, n, one, op);}
            using T = std::decay_t<decltype(x)>;
            std::vector<T> odd;
            odd.reserve(1 << (window - 1));
            odd.push_back(x);
            auto square = op(x, x);
            while(odd.size() < size_t(1 << (window - 1))) {odd.push_back(op(odd.back(), square));}
            auto ans = odd[(n >> first) / 2];
            for(int j = first - 1; j >= 0;) {
                if(!((n >> j) & 1)) {ans = op(ans, ans); j--;}
                else {
                    int low = low_bit(j), length = j - low + 1;
                    auto digit = (n >> low) & ((1u << length) - 1);
                    for(int i = 0; i < length; i++) {ans = op(ans, ans);}
                    ans = op(ans, odd[digit / 2]);
                    j = low - 1;
                }
            }
            return ans;
        } else {
            if (n == 0) {
                return one;
            }
            auto ans = x;
            for(int j = std::bit_width<uint64_t>(n) - 2; ~j; j--) {
                ans = op(ans, ans);
                if((n >> j) & 1) {
                    ans = op(ans, x);
                }
            }
            return ans;
        }
    }
    template<int window = 1>
    auto bpow(auto x, auto n, auto ans) {
        return bpow<window>(x, n, ans, std::multiplies{});
    }
    template<typename T>
    T bpow(T const& x, auto n) {
        return bpow(x, n, T(1));
    }
    inline constexpr auto inv2(auto x) {
        assert(x % 2);
        std::make_unsigned_t<decltype(x)> y = 1;
        while(y * x != 1) {
            y *= 2 - x * y;
        }
        return y;
    }
}

#line 1 "cp-algo/linalg/vector.hpp"


#line 1 "cp-algo/number_theory/modint.hpp"


#line 4 "cp-algo/number_theory/modint.hpp"
#include <iostream>
#include <cassert>
namespace cp_algo::math {

    template<typename modint, typename _Int>
    struct modint_base {
        using Int = _Int;
        using UInt = std::make_unsigned_t<Int>;
        static constexpr size_t bits = sizeof(Int) * 8;
        using Int2 = std::conditional_t<bits <= 32, int64_t, __int128_t>;
        using UInt2 = std::conditional_t<bits <= 32, uint64_t, __uint128_t>;
        constexpr static Int mod() {
            return modint::mod();
        }
        constexpr static UInt remod() {
            return modint::remod();
        }
        constexpr static UInt2 modmod() {
            return UInt2(mod()) * mod();
        }
        constexpr modint_base() = default;
        constexpr modint_base(Int2 rr) {
            to_modint().setr(UInt((rr + modmod()) % mod()));
        }
        constexpr modint inv() const {
            return bpow(to_modint(), mod() - 2);
        }
        modint operator - () const {
            modint neg;
            neg.r = std::min(-r, remod() - r);
            return neg;
        }
        modint& operator /= (const modint &t) {
            return to_modint() *= t.inv();
        }
        modint& operator *= (const modint &t) {
            r = UInt(UInt2(r) * t.r % mod());
            return to_modint();
        }
        modint& operator += (const modint &t) {
            r += t.r; r = std::min(r, r - remod());
            return to_modint();
        }
        modint& operator -= (const modint &t) {
            r -= t.r; r = std::min(r, r + remod());
            return to_modint();
        }
        modint operator + (const modint &t) const {return modint(to_modint()) += t;}
        modint operator - (const modint &t) const {return modint(to_modint()) -= t;}
        modint operator * (const modint &t) const {return modint(to_modint()) *= t;}
        modint operator / (const modint &t) const {return modint(to_modint()) /= t;}
        // Why <=> doesn't work?..
        auto operator == (const modint &t) const {return to_modint().getr() == t.getr();}
        auto operator != (const modint &t) const {return to_modint().getr() != t.getr();}
        auto operator <= (const modint &t) const {return to_modint().getr() <= t.getr();}
        auto operator >= (const modint &t) const {return to_modint().getr() >= t.getr();}
        auto operator < (const modint &t) const {return to_modint().getr() < t.getr();}
        auto operator > (const modint &t) const {return to_modint().getr() > t.getr();}
        Int rem() const {
            UInt R = to_modint().getr();
            return R - (R > (UInt)mod() / 2) * mod();
        }
        constexpr void setr(UInt rr) {
            r = rr;
        }
        constexpr UInt getr() const {
            return r;
        }

        // Only use these if you really know what you're doing!
        static uint64_t modmod8() {return uint64_t(8 * modmod());}
        void add_unsafe(UInt t) {r += t;}
        void pseudonormalize() {r = std::min(r, r - modmod8());}
        modint const& normalize() {
            if(r >= (UInt)mod()) {
                r %= mod();
            }
            return to_modint();
        }
        void setr_direct(UInt rr) {r = rr;}
        UInt getr_direct() const {return r;}
    protected:
        UInt r;
    private:
        constexpr modint& to_modint() {return static_cast<modint&>(*this);}
        constexpr modint const& to_modint() const {return static_cast<modint const&>(*this);}
    };
    template<typename modint>
    concept modint_type = std::is_base_of_v<modint_base<modint, typename modint::Int>, modint>;
    template<modint_type modint>
    decltype(std::cin)& operator >> (decltype(std::cin) &in, modint &x) {
        typename modint::UInt r;
        auto &res = in >> r;
        x.setr(r);
        return res;
    }
    template<modint_type modint>
    decltype(std::cout)& operator << (decltype(std::cout) &out, modint const& x) {
        return out << x.getr();
    }

    template<auto m>
    struct modint: modint_base<modint<m>, decltype(m)> {
        using Base = modint_base<modint<m>, decltype(m)>;
        using Base::Base;
        static constexpr Base::Int mod() {return m;}
        static constexpr Base::UInt remod() {return m;}
        auto getr() const {return Base::r;}
    };

    // Odd moduli up to a quarter of the unsigned word keep Montgomery residues lazily in
    // [0, 2 mod): remod() = 2 mod, and both 4 mod and ab + q * mod fit. Any other modulus keeps
    // fully reduced residues, remod() = mod, which is what the sums of modint_base need then:
    // an even one without the Montgomery form, a wide odd one with a reduction that subtracts
    // high words instead of adding double words that would overflow.
    template<typename Int = int>
    struct dynamic_modint: modint_base<dynamic_modint<Int>, Int> {
        using Base = modint_base<dynamic_modint<Int>, Int>;
        using Base::Base;

        // Out of line, so that the hot path stays as small as it was.
        [[gnu::noinline, gnu::cold]] static Base::UInt m_reduce_reduced(Base::UInt2 ab) {
            if(mod() % 2 == 0) {return typename Base::UInt(ab % mod());}
            // q * mod has the low word of ab, so the difference of the high words is exact.
            typename Base::UInt q = -(typename Base::UInt(ab) * inverse);
            auto high = typename Base::UInt(ab >> Base::bits);
            auto low = typename Base::UInt(typename Base::UInt2(q) * typename Base::UInt(mod()) >> Base::bits);
            return high >= low ? high - low : high - low + mod();
        }
        static Base::UInt m_reduce(Base::UInt2 ab) {
            if(imod() == 0) [[unlikely]] {return m_reduce_reduced(ab);}
            typename Base::UInt2 m = typename Base::UInt(ab) * imod();
            return typename Base::UInt((ab + m * mod()) >> Base::bits);
        }
        static Base::UInt m_transform(Base::UInt a) {
            if(mod() % 2 == 0) [[unlikely]] {
                return a;
            } else {
                return m_reduce(a * pw128());
            }
        }
        dynamic_modint& operator *= (const dynamic_modint &t) {
            Base::r = m_reduce(typename Base::UInt2(Base::r) * t.r);
            return *this;
        }
        void setr(Base::UInt rr) {
            Base::r = m_transform(rr);
        }
        Base::UInt getr() const {
            typename Base::UInt res = m_reduce(Base::r);
            return std::min(res, res - mod());
        }
        static Int mod() {return m;}
        static Base::UInt remod() {return rm;}
        static Base::UInt imod() {return im;}
        static Base::UInt2 pw128() {return r2;}
        static void switch_mod(Int nm) {
            m = nm;
            bool lazy = m % 2 && typename Base::UInt(m) <= typename Base::UInt(-1) / 4;
            rm = typename Base::UInt(m) * (lazy ? 2 : 1);
            inverse = m % 2 ? inv2(-m) : 0;
            im = lazy ? inverse : 0;
            r2 = static_cast<Base::UInt>(static_cast<Base::UInt2>(-1) % m + 1);
        }

        // Wrapper for temp switching
        auto static with_mod(Int tmp, auto callback) {
            struct scoped {
                Int prev = mod();
                ~scoped() {switch_mod(prev);}
            } _;
            switch_mod(tmp);
            return callback();
        }
    private:
        static thread_local Int m;
        // im: -1 / mod modulo 2^bits for lazy residues and 0 for reduced ones; inverse: the same
        // for every odd mod; rm: the value of remod().
        static thread_local Base::UInt im, r2, inverse, rm;
    };
    template<typename Int>
    Int thread_local dynamic_modint<Int>::m = 1;
    template<typename Int>
    dynamic_modint<Int>::Base::UInt thread_local dynamic_modint<Int>::im = -1;
    template<typename Int>
    dynamic_modint<Int>::Base::UInt thread_local dynamic_modint<Int>::r2 = 0;
    template<typename Int>
    dynamic_modint<Int>::Base::UInt thread_local dynamic_modint<Int>::inverse = -1;
    template<typename Int>
    dynamic_modint<Int>::Base::UInt thread_local dynamic_modint<Int>::rm = 2;
}

#line 1 "cp-algo/util/big_alloc.hpp"



#include <set>
#include <map>
#include <deque>
#include <stack>
#include <queue>
#line 10 "cp-algo/util/big_alloc.hpp"
#include <string>
#include <cstddef>
#line 13 "cp-algo/util/big_alloc.hpp"
#include <forward_list>

// Single macro to detect POSIX platforms (Linux, Unix, macOS)
#if defined(__linux__) || defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#  define CP_ALGO_USE_MMAP 1
#  include <sys/mman.h>
#else
#  define CP_ALGO_USE_MMAP 0
#endif

namespace cp_algo {
    template <typename T, size_t Align = 32>
    class big_alloc {
        static_assert( Align >= alignof(void*), "Align must be at least pointer-size");
        static_assert(std::popcount(Align) == 1, "Align must be a power of two");
    public:
        using value_type = T;
        template <class U> struct rebind { using other = big_alloc<U, Align>; };
        constexpr bool operator==(const big_alloc&) const = default;
        constexpr bool operator!=(const big_alloc&) const = default;

        big_alloc() noexcept = default;
        template <typename U, std::size_t A>
        big_alloc(const big_alloc<U, A>&) noexcept {}

        [[nodiscard]] T* allocate(std::size_t n) {
            std::size_t padded = round_up(n * sizeof(T));
            std::size_t align = std::max<std::size_t>(alignof(T),  Align);
#if CP_ALGO_USE_MMAP
            if (padded >= MEGABYTE) {
                void* raw = mmap(nullptr, padded,
                                PROT_READ | PROT_WRITE,
                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                madvise(raw, padded, MADV_HUGEPAGE);
                return static_cast<T*>(raw);
            }
#endif
            return static_cast<T*>(::operator new(padded, std::align_val_t(align)));
        }

        void deallocate(T* p, std::size_t n) noexcept {
            if (!p) return;
            std::size_t padded = round_up(n * sizeof(T));
            std::size_t align  = std::max<std::size_t>(alignof(T),  Align);
    #if CP_ALGO_USE_MMAP
            if (padded >= MEGABYTE) { munmap(p, padded); return; }
    #endif
            ::operator delete(p, padded, std::align_val_t(align));
        }

    private:
        static constexpr std::size_t MEGABYTE = 1 << 20;
        static constexpr std::size_t round_up(std::size_t x) noexcept {
            return (x + Align - 1) / Align * Align;
        }
    };

    template<typename T> using big_vector = std::vector<T, big_alloc<T>>;
    template<typename T> using big_basic_string = std::basic_string<T, std::char_traits<T>, big_alloc<T>>;
    template<typename T> using big_deque = std::deque<T, big_alloc<T>>;
    template<typename T> using big_stack = std::stack<T, big_deque<T>>;
    template<typename T> using big_queue = std::queue<T, big_deque<T>>;
    template<typename T> using big_priority_queue = std::priority_queue<T, big_vector<T>>;
    template<typename T> using big_forward_list = std::forward_list<T, big_alloc<T>>;
    using big_string = big_basic_string<char>;

    template<typename Key, typename Value, typename Compare = std::less<Key>>
    using big_map = std::map<Key, Value, Compare, big_alloc<std::pair<const Key, Value>>>;
    template<typename T, typename Compare = std::less<T>>
    using big_multiset = std::multiset<T, Compare, big_alloc<T>>;
    template<typename T, typename Compare = std::less<T>>
    using big_set = std::set<T, Compare, big_alloc<T>>;
}


#line 1 "cp-algo/util/simd.hpp"


#include <experimental/simd>
#line 6 "cp-algo/util/simd.hpp"
#include <memory>

#if defined(__x86_64__) && !defined(CP_ALGO_DISABLE_AVX2)
#define CP_ALGO_SIMD_AVX2_TARGET _Pragma("GCC target(\"avx2,fma\")")
#else
#define CP_ALGO_SIMD_AVX2_TARGET
#endif

#define CP_ALGO_SIMD_PRAGMA_PUSH \
    _Pragma("GCC push_options") \
    CP_ALGO_SIMD_AVX2_TARGET

CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo {
    template<typename T, size_t len>
    using simd [[gnu::vector_size(len * sizeof(T))]] = T;
    using u64x8 = simd<uint64_t, 8>;
    using u32x16 = simd<uint32_t, 16>;
    using i64x4 = simd<int64_t, 4>;
    using u64x4 = simd<uint64_t, 4>;
    using u32x8 = simd<uint32_t, 8>;
    using u16x16 = simd<uint16_t, 16>;
    using i32x4 = simd<int32_t, 4>;
    using u32x4 = simd<uint32_t, 4>;
    using u16x8 = simd<uint16_t, 8>;
    using u16x4 = simd<uint16_t, 4>;
    using i16x4 = simd<int16_t, 4>;
    using u8x32 = simd<uint8_t, 32>;
    using u8x16 = simd<uint8_t, 16>;
    using u8x8 = simd<uint8_t, 8>;
    using u8x4 = simd<uint8_t, 4>;
    using dx4 = simd<double, 4>;

    inline dx4 abs(dx4 a) {
        return dx4{
            std::abs(a[0]),
            std::abs(a[1]),
            std::abs(a[2]),
            std::abs(a[3])
        };
    }

    // https://stackoverflow.com/a/77376595
    // works for ints in (-2^51, 2^51)
    static constexpr dx4 magic = dx4() + (3ULL << 51);
    inline i64x4 lround(dx4 x) {
        return i64x4(x + magic) - i64x4(magic);
    }
    inline dx4 to_double(i64x4 x) {
        return dx4(x + i64x4(magic)) - magic;
    }

    inline dx4 round(dx4 a) {
        return dx4{
            std::nearbyint(a[0]),
            std::nearbyint(a[1]),
            std::nearbyint(a[2]),
            std::nearbyint(a[3])
        };
    }

    inline u64x4 low32(u64x4 x) {
        return x & uint32_t(-1);
    }
    inline auto swap_bytes(auto x) {
        return decltype(x)(__builtin_shufflevector(u32x8(x), u32x8(x), 1, 0, 3, 2, 5, 4, 7, 6));
    }
    inline u64x4 montgomery_reduce(u64x4 x, uint32_t mod, uint32_t imod) {
#ifdef __AVX2__
        auto x_ninv = u64x4(_mm256_mul_epu32(__m256i(x), __m256i() + imod));
        x += u64x4(_mm256_mul_epu32(__m256i(x_ninv), __m256i() + mod));
#else
        auto x_ninv = u64x4(u32x8(low32(x)) * imod);
        x += x_ninv * uint64_t(mod);
#endif
        return swap_bytes(x);
    }

    // x - mod where x >= mod, for x in [0, 2 mod). Unsigned, so it holds up to mod < 2^31;
    // zero upper halves of 64-bit lanes stay zero.
    inline u32x8 reduce_once(u32x8 x, uint32_t mod) {
        auto y = x - mod;
        return x < y ? x : y;
    }
    inline u64x4 reduce_once(u64x4 x, uint32_t mod) {
        return u64x4(reduce_once(u32x8(x), mod));
    }

    inline u64x4 montgomery_mul(u64x4 x, u64x4 y, uint32_t mod, uint32_t imod) {
#ifdef __AVX2__
        return montgomery_reduce(u64x4(_mm256_mul_epu32(__m256i(x), __m256i(y))), mod, imod);
#else
        return montgomery_reduce(x * y, mod, imod);
#endif
    }
    inline u32x8 montgomery_mul(u32x8 x, u32x8 y, uint32_t mod, uint32_t imod) {
        return u32x8(montgomery_mul(u64x4(x), u64x4(y), mod, imod)) |
               u32x8(swap_bytes(montgomery_mul(u64x4(swap_bytes(x)), u64x4(swap_bytes(y)), mod, imod)));
    }
    inline dx4 rotate_right(dx4 x) {
        static constexpr u64x4 shuffler = {3, 0, 1, 2};
        return __builtin_shuffle(x, shuffler);
    }

    template<std::size_t Align = 32>
    inline bool is_aligned(const auto* p) noexcept {
        return (reinterpret_cast<std::uintptr_t>(p) % Align) == 0;
    }

    template<class Target>
    inline Target& vector_cast(auto &&p) {
        return *reinterpret_cast<Target*>(std::assume_aligned<alignof(Target)>(&p));
    }
}
#pragma GCC pop_options

#line 1 "cp-algo/util/checkpoint.hpp"


#line 8 "cp-algo/util/checkpoint.hpp"
namespace cp_algo {
#ifdef CP_ALGO_CHECKPOINT
    big_map<big_string, double> checkpoints;
    double last;
#endif
    template<bool final = false>
    void checkpoint([[maybe_unused]] auto const& _msg) {
#ifdef CP_ALGO_CHECKPOINT
        big_string msg = _msg;
        double now = (double)clock() / CLOCKS_PER_SEC;
        double delta = now - last;
        last = now;
        if(msg.size() && !final) {
            checkpoints[msg] += delta;
        }
        if(final) {
            for(auto const& [key, value] : checkpoints) {
                std::cerr << key << ": " << value * 1000 << " ms\n";
            }
            std::cerr << "Total: " << now * 1000 << " ms\n";
        }
#endif
    }
    template<bool final = false>
    void checkpoint() {
        checkpoint<final>("");
    }
}

#line 10 "cp-algo/linalg/vector.hpp"
#include <valarray>
#line 12 "cp-algo/linalg/vector.hpp"
#include <iterator>
#include <cassert>
#include <ranges>
#include <array>
#include <cstring>
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::linalg {
    template<typename base, class Alloc = big_alloc<base>>
    struct vec: std::basic_string<base, std::char_traits<base>, Alloc> {
        using Base = std::basic_string<base, std::char_traits<base>, Alloc>;
        using Base::Base;

        vec(Base const& t): Base(t) {}
        vec(Base &&t): Base(std::move(t)) {}
        vec(size_t n): Base(n, base()) {}
        vec(auto &&r): Base(r.begin(), r.end()) {}

        static vec ei(size_t n, size_t i) {
            vec res(n);
            res[i] = 1;
            return res;
        }

        auto operator-() const {
            return *this | std::views::transform([](auto x) {return -x;});
        }
        auto operator *(base t) const {
            return *this | std::views::transform([t](auto x) {return x * t;});
        }
        vec& operator *=(base t) {
            for(auto &it: *this) {
                it *= t;
            }
            return *this;
        }

        virtual void add_scaled(vec const& b, base scale, size_t i = 0) {
            if(scale != base(0)) {
                for(; i < size(*this); i++) {
                    (*this)[i] += scale * b[i];
                }
            }
        }
        virtual vec const& normalize() {
            return static_cast<vec&>(*this);
        }
        virtual base normalize(size_t i) {
            return (*this)[i];
        }
        void read() {
            for(auto &it: *this) {
                std::cin >> it;
            }
        }
        void print() const {
            for(auto &it: *this) {
                std::cout << it << ' ';
            }
            std::cout << '\n';
        }
        static vec random(size_t n) {
            vec res(n);
            std::ranges::generate(res, random::rng);
            return res;
        }
        // Concatenate vectors
        vec operator |(vec const& t) const {
            return std::views::join(std::array{
                std::views::all(*this),
                std::views::all(t)
            });
        }

        // Generally, vec shouldn't be modified
        // after its pivot index is set
        std::pair<size_t, base> find_pivot() {
            if(pivot == size_t(-1)) {
                pivot = 0;
                while(pivot < size(*this) && normalize(pivot) == base(0)) {
                    pivot++;
                }
                if(pivot < size(*this)) {
                    pivot_inv = base(1) / (*this)[pivot];
                }
            }
            return {pivot, pivot_inv};
        }
        void reduce_by(vec &t) {
            auto [pivot, pinv] = t.find_pivot();
            if(pivot < size(*this)) {
                add_scaled(t, -normalize(pivot) * pinv, pivot);
            }
        }
    private:
        size_t pivot = -1;
        base pivot_inv;
    };

    template<math::modint_type base, class Alloc = big_alloc<base>>
    struct modint_vec: vec<base, Alloc> {
        using Base = vec<base, Alloc>;
        using Base::Base;

        modint_vec(Base const& t): Base(t) {}
        modint_vec(Base &&t): Base(std::move(t)) {}

        void add_scaled(Base const& b, base scale, size_t i = 0) override {
            static_assert(base::bits >= 64, "Only wide modint types for linalg");
            if(scale != base(0)) {
                assert(Base::size() == b.size());
                size_t n = size(*this);
                u64x4 scaler = u64x4() + scale.getr();
                bool aligned = is_aligned(&(*this)[0]) && is_aligned(&b[0]);
                if(aligned) i -= i % 4;
                bool reduce = ++counter == accumulation_period();
                if(reduce) {
                    counter = 0;
                    for(size_t j = 0; j < i; j++) (*this)[j].pseudonormalize();
                }
                if(aligned) for(; i + 3 < n; i += 4) {
                    auto &ai = vector_cast<u64x4>((*this)[i]);
                    auto bi = vector_cast<u64x4 const>(b[i]);
#ifdef __AVX2__
                    ai += u64x4(_mm256_mul_epu32(__m256i(scaler), __m256i(bi)));
#else
                    ai += scaler * bi;
#endif
                    if(reduce) ai = shrink(ai);
                }
                for(; i < n; i++) {
                    (*this)[i].add_unsafe(b[i].getr_direct() * scale.getr());
                    if(reduce) (*this)[i].pseudonormalize();
                }
            }
        }
        Base const& normalize() override {
            for(auto &it: *this) {
                it.normalize();
            }
            return *this;
        }
        base normalize(size_t i) override {
            return (*this)[i].normalize();
        }
    private:
        template<typename, typename> friend struct matrix;
        static size_t accumulation_period() {
            // Eight canonical products keep the accumulator below 16 * mod^2.
            // Montgomery residues can be wider, so retain four updates there.
            return base::remod() == typename base::UInt(base::mod()) && base::mod() < (1LL << 30) ? 8 : 4;
        }
        static u64x4 mul(u64x4 a, u64x4 b) {
#ifdef __AVX2__
            return u64x4(_mm256_mul_epu32(__m256i(a), __m256i(b)));
#else
            return a * b;
#endif
        }
        static u64x4 shrink(u64x4 a) {
            auto b = a - (u64x4() + base::modmod8());
            return a < b ? a : b;
        }
        // Two source contributions to two distinct rows; sources must be normalized.
        static void add_scaled_pair(modint_vec &x, modint_vec &y, Base const& p, Base const& q,
                                    std::array<base, 4> c, size_t first = 0) {
            if(std::ranges::find(c, base(0)) != c.end()) {
                x.add_scaled(p, c[0], first); x.add_scaled(q, c[1], first);
                y.add_scaled(p, c[2], first); y.add_scaled(q, c[3], first);
                return;
            }
            size_t n = x.size();
            assert(y.size() == n && p.size() == n && q.size() == n && first <= n);
            // Pivot sources vanish before first; include that zero prefix for aligned updates.
            if(is_aligned(x.data()) && is_aligned(y.data()) &&
               is_aligned(p.data()) && is_aligned(q.data())) first -= first % 4;
            size_t period = accumulation_period();
            auto prepare = [&](modint_vec &a) {
                // A pair must not cross the accumulator's reduction boundary.
                if(a.counter + 2 > period) {
                    for(auto &v: a) v.pseudonormalize();
                    a.counter = 0;
                }
                a.counter += 2;
                if(a.counter != period) return false;
                a.counter = 0;
                for(size_t i = 0; i < first; i++) a[i].pseudonormalize();
                return true;
            };
            bool nx = prepare(x), ny = prepare(y);
            auto * __restrict__ dx = x.data();
            auto * __restrict__ dy = y.data();
            auto const * __restrict__ sp = p.data();
            auto const * __restrict__ sq = q.data();
            uint64_t xp = c[0].getr(), xq = c[1].getr(), yp = c[2].getr(), yq = c[3].getr();
            u64x4 xp4 = u64x4() + xp, xq4 = u64x4() + xq;
            u64x4 yp4 = u64x4() + yp, yq4 = u64x4() + yq;
            size_t i = first;
            for(; i + 4 <= n; i += 4) {
                u64x4 vx, vy, vp, vq;
                std::memcpy(&vx, dx + i, sizeof vx); std::memcpy(&vy, dy + i, sizeof vy);
                std::memcpy(&vp, sp + i, sizeof vp); std::memcpy(&vq, sq + i, sizeof vq);
                vx += mul(xp4, vp) + mul(xq4, vq);
                vy += mul(yp4, vp) + mul(yq4, vq);
                if(nx) vx = shrink(vx);
                if(ny) vy = shrink(vy);
                std::memcpy(dx + i, &vx, sizeof vx); std::memcpy(dy + i, &vy, sizeof vy);
            }
            for(; i < n; i++) {
                dx[i].add_unsafe(xp * sp[i].getr_direct() + xq * sq[i].getr_direct());
                dy[i].add_unsafe(yp * sp[i].getr_direct() + yq * sq[i].getr_direct());
                if(nx) dx[i].pseudonormalize();
                if(ny) dy[i].pseudonormalize();
            }
        }
        size_t counter = 0;
    };

    // Narrow rows keep canonical residues; small batches accumulate in wide registers.
    template<typename base> requires (base::bits <= 32)
    struct modint_vec<base>: vec<base> {
        using Base = vec<base>;
        using Base::Base;
        modint_vec(Base const& t): Base(t) {}
        modint_vec(Base &&t): Base(std::move(t)) {}

        void add_scaled(Base const& b, base scale, size_t first = 0) override {
            if(scale == base(0)) return;
            if(&b == this) Base::add_scaled(b, scale, first);
            else add_scaled_batch<1, 1>({this}, {&b}, {scale}, first);
        }
    private:
        template<typename, typename> friend struct matrix;
        static constexpr size_t batch_size = 8;
        static constexpr bool use_simd = [] {
            if constexpr(requires {
                std::integral_constant<uint32_t, base::mod()>{};
                std::integral_constant<uint32_t, base::remod()>{};
            }) {
                return sizeof(base) == sizeof(uint32_t) && base::mod() > 1 &&
                    base::mod() % 2 && base::mod() < (1U << 30) && base::remod() == typename base::UInt(base::mod());
            } else return false;
        }();
        static u64x4 mul(u64x4 a, u64x4 b) {
#ifdef __AVX2__
            return u64x4(_mm256_mul_epu32(__m256i(a), __m256i(b)));
#else
            return low32(a) * low32(b);
#endif
        }
        // Sources are normalized and do not alias the distinct destination rows.
        template<size_t count, size_t rows>
        static void add_scaled_batch(std::array<modint_vec*, rows> const& dst,
                                     std::array<Base const*, count> const& src,
                                     std::array<base, rows * count> const& c, size_t first = 0) {
            static_assert(count <= batch_size && (rows == 1 || rows == 2));
            if(std::ranges::all_of(c, [](base value) { return value == base(0); })) return;
            if constexpr(use_simd) {
                constexpr uint32_t mod = base::mod(), inv = math::inv2(uint32_t(-mod));
                std::array<uint32_t, rows * count> scale;
                for(size_t t = 0; t < rows * count; t++) {
                    scale[t] = uint32_t((uint64_t(c[t].getr()) << 32) % mod);
                }
                auto * __restrict__ dx = dst[0]->data();
                auto * __restrict__ dy = rows == 2 ? dst[1]->data() : nullptr;
                size_t n = dst[0]->size();
                for(; first + 8 <= n; first += 8) {
                    u64x4 acc[rows][2]{};
#pragma GCC unroll 1
                    for(size_t t = 0; t < count; t++) {
                        u64x4 p;
                        std::memcpy(&p, src[t]->data() + first, sizeof p);
                        auto q = p >> 32;
                        for(size_t row = 0; row < rows; row++) {
                            auto v = u64x4(u32x8() + scale[row * count + t]);
                            acc[row][0] += mul(p, v); acc[row][1] += mul(q, v);
                        }
                    }
                    for(size_t row = 0; row < rows; row++) {
                        // At most eight products: reduction gives <3*mod, then add the old residue.
                        for(auto &v: acc[row]) v = montgomery_reduce(v, mod, inv);
                        auto *out = (row ? dy : dx) + first;
                        u32x8 old;
                        std::memcpy(&old, out, sizeof old);
                        auto z = old + u32x8(acc[row][0] | (acc[row][1] << 32));
                        z = z < z - 2 * mod ? z : z - 2 * mod;
                        z = z < z - mod ? z : z - mod;
                        std::memcpy(out, &z, sizeof z);
                    }
                }
            }
            for(size_t t = 0; t < count; t++) for(size_t row = 0; row < rows; row++) {
                dst[row]->Base::add_scaled(*src[t], c[row * count + t], first);
            }
        }
        static void add_scaled_pair(modint_vec &x, modint_vec &y, Base const& p, Base const& q,
                                    std::array<base, 4> c, size_t first = 0) {
            add_scaled_batch<2, 2>({&x, &y}, {&p, &q}, c, first);
        }
    };
}
#pragma GCC pop_options

#line 1 "cp-algo/linalg/strassen.hpp"


#line 4 "cp-algo/linalg/strassen.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::linalg::impl {
    template<class row> constexpr bool use_strassen = false;
    template<auto mod> constexpr bool use_strassen<modint_vec<math::modint<mod>>> =
        mod > 1 && mod % 2 && mod < (1LL << 30);

    // Internal packed storage; the public matrix keeps its usual row representation.
    template<uint32_t mod>
    struct strassen_product {
        struct view {
            uint32_t *data;
            size_t stride;
            uint32_t* operator[](size_t i) const {return data + i * stride;}
        };
        static u64x4 mul(u64x4 a, u64x4 b) {
#ifdef __AVX2__
            return u64x4(_mm256_mul_epu32(__m256i(a), __m256i(b)));
#else
            return low32(a) * low32(b);
#endif
        }
        static u64x4 shrink(u64x4 x) {
            // x < 4*mod*2^32; reduce the high word modulo 2*mod.
            auto words = u32x8(x);
            auto bound = u32x8(u64x4() + (uint64_t(2) * mod << 32));
#ifdef __AVX2__
            return u64x4(_mm256_min_epu32(__m256i(words), __m256i(words - bound)));
#else
            return u64x4(words < words - bound ? words : words - bound);
#endif
        }
        template<size_t dim = 0>
        [[gnu::noinline]] static void leaf(view a, view b, view c, size_t n, size_t m, size_t k) {
            if constexpr(dim) {
                n = m = k = dim;
                a.stride = b.stride = c.stride = dim;
            }
            for(size_t i = 0; i < n; i += 4) {
                for(size_t j = 0; j < k; j += 8) {
                    u64x4 acc[4][2]{};
                    for(size_t first = 0; first < m; first += 8) {
#pragma GCC unroll 1
                        for(size_t z = first; z < first + 8; z++) {
                            u64x4 x;
                            std::memcpy(&x, b[z] + j, sizeof x);
                            u64x4 scales[4];
                            for(size_t t = 0; t < 4; t++) scales[t] = u64x4(u32x8() + a[i + t][z]);
                            for(size_t t = 0; t < 4; t++) acc[t][0] += mul(scales[t], x);
                            x >>= 32;
                            for(size_t t = 0; t < 4; t++) acc[t][1] += mul(scales[t], x);
                        }
                        for(auto &row: acc) for(auto &x: row) x = shrink(x);
                    }
                    for(size_t t = 0; t < 4; t++) {
                        constexpr uint32_t inv = math::inv2(uint32_t(-mod));
                        // x < 2*mod*2^32, so Montgomery reduction yields a value below 3*mod.
                        for(auto &x: acc[t]) x = montgomery_reduce(x, mod, inv);
                        u32x8 out = u32x8(acc[t][0] | (acc[t][1] << 32));
                        out = out < out - mod ? out : out - mod;
                        out = out < out - mod ? out : out - mod;
                        std::memcpy(c[i + t] + j, &out, sizeof out);
                    }
                }
            }
        }
        // c = a +/- b; c may alias a.
        template<bool subtract = false>
        static void combine(uint32_t *a, uint32_t *b, uint32_t *c, size_t n, size_t m) {
            for(size_t j = 0; j < n * m; j += 8) {
                u32x8 x, y;
                std::memcpy(&x, a + j, sizeof x);
                std::memcpy(&y, b + j, sizeof y);
                u32x8 z = subtract ? x + mod - y : x + y;
                z = z < z - mod ? z : z - mod;
                std::memcpy(c + j, &z, sizeof z);
            }
        }
        static bool can_split(size_t n, size_t m, size_t k) {
            return std::min({n, m, k}) > 64 && n % 16 == 0 && m % 16 == 0 && k % 16 == 0;
        }
        [[gnu::noinline]] static void multiply(uint32_t *a, uint32_t *b, uint32_t *c, size_t n, size_t m, size_t k,
                                              uint32_t *work) {
            // Every leaf dimension must remain a multiple of eight.
            if(!can_split(n, m, k)) {
                if(n == 64 && m == 64 && k == 64) leaf<64>({a, m}, {b, k}, {c, k}, n, m, k);
                else leaf({a, m}, {b, k}, {c, k}, n, m, k);
                return;
            }
            n /= 2; m /= 2; k /= 2;
            auto s = work, t = s + n * m, p = t + m * k;
            work += n * m + m * k + n * k;
            auto a00 = a, a01 = a + n * m, a10 = a + 2 * n * m, a11 = a + 3 * n * m;
            auto b00 = b, b01 = b + m * k, b10 = b + 2 * m * k, b11 = b + 3 * m * k;
            auto c00 = c, c01 = c + n * k, c10 = c + 2 * n * k, c11 = c + 3 * n * k;

            // Winograd's schedule uses seven products and fifteen additions.
            multiply(a00, b00, c11, n, m, k, work); // P1
            multiply(a01, b10, c00, n, m, k, work); // P2
            combine(c00, c11, c00, n, k); // C00 = P1 + P2

            combine(a10, a11, s, n, m); combine<true>(b01, b00, t, m, k); // S1, T1
            multiply(s, t, c01, n, m, k, work); // P5
            combine<true>(s, a00, s, n, m); combine<true>(b11, t, t, m, k); // S2, T2
            multiply(s, t, c10, n, m, k, work); // P6
            combine(c11, c10, c10, n, k); // U2 = P1 + P6

            combine<true>(a01, s, s, n, m); // S4
            multiply(s, b11, p, n, m, k, work); // P3
            combine(c10, c01, c11, n, k); // U4 = U2 + P5
            combine(c11, p, c01, n, k); // C01 = U4 + P3

            combine<true>(t, b10, t, m, k); // T4
            multiply(a11, t, p, n, m, k, work); // P4
            combine<true>(c10, p, c10, n, k);
            combine<true>(a00, a10, s, n, m); combine<true>(b11, b01, t, m, k); // S3, T3
            multiply(s, t, p, n, m, k, work); // P7
            combine(c10, p, c10, n, k); combine(c11, p, c11, n, k); // C10, C11
        }
        static u32x8 encode(u32x8 x) {
            // Scale by 2^32 with a fixed reciprocal; its quotient is off by at most one.
            constexpr uint32_t scale = (uint64_t(1) << 32) % mod;
            constexpr uint32_t quotient = (uint64_t(scale) << 32) / mod;
            auto packed = u64x4(x), q = u64x4() + quotient;
            auto lo = mul(packed, q) >> 32, hi = mul(packed >> 32, q);
            auto approx = u32x8(lo | (hi & (~uint64_t(0) << 32)));
            auto out = x * scale - approx * mod;
            return out < out - mod ? out : out - mod;
        }
        // Copy between matrix rows and contiguous recursive quadrants.
        template<bool unpack = false, class matrix>
        static void copy(matrix &a, uint32_t *ptr, size_t n, size_t m, size_t depth,
                         size_t row = 0, size_t col = 0) {
            if(depth) {
                n /= 2; m /= 2;
                for(size_t q = 0; q < 4; q++) {
                    copy<unpack>(a, ptr + q * n * m, n, m, depth - 1,
                                 row + q / 2 * n, col + q % 2 * m);
                }
                return;
            }
            if(col >= a.m()) return;
            size_t width = std::min(m, a.m() - col);
            for(size_t i = row; i < std::min(row + n, a.n()); i++) {
                auto data = a[i].data() + col;
                auto packed = ptr + (i - row) * m;
                for(size_t j = 0; j < width; j++) {
                    if constexpr(unpack) data[j].setr(packed[j]);
                    else packed[j] = uint32_t(data[j].getr());
                }
            }
        }
        template<class matrix>
        static matrix product(matrix const& a, matrix const& b) {
            auto pad = [](size_t x) {return (x + 31) / 32 * 32;};
            size_t n = pad(a.n()), m = pad(a.m()), k = pad(b.m());
            // Each recursion level needs a quarter as much scratch; children reuse it.
            size_t entries = n * m + m * k + n * k;
            // Small products avoid repeated mmap/madvise setup for temporary storage.
            std::vector<uint32_t> small;
            big_vector<uint32_t> large;
            size_t count = entries + entries / 3;
            auto ap = count < (1 << 21) ? (small.resize(count), small.data())
                                       : (large.resize(count), large.data());
            auto bp = ap + n * m, cp = bp + m * k;
            auto scratch = cp + n * k;
            // A, B, and C must use the same depth, including rectangular products.
            size_t depth = 0;
            for(size_t x = n, y = m, z = k; can_split(x, y, z); x /= 2, y /= 2, z /= 2) depth++;
            copy(a, ap, n, m, depth); copy(b, bp, m, k, depth);
            // Only A is scaled; each leaf's Montgomery reduction removes the factor.
            for(size_t i = 0; i < n * m; i += 8) {
                u32x8 x;
                std::memcpy(&x, ap + i, sizeof x);
                x = encode(x);
                std::memcpy(ap + i, &x, sizeof x);
            }
            multiply(ap, bp, cp, n, m, k, scratch);
            matrix res(a.n(), b.m());
            copy<true>(res, cp, n, k, depth);
            return res;
        }
    };
}
#pragma GCC pop_options

#line 8 "cp-algo/linalg/matrix.hpp"
#include <optional>
#include <cassert>
#line 12 "cp-algo/linalg/matrix.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::linalg {
    enum gauss_mode {normal, reverse};

    template<typename base_t, class _vec_t = std::conditional_t<
        math::modint_type<base_t>,
        modint_vec<base_t>,
        vec<base_t>>>
    struct matrix: big_vector<_vec_t> {
        using vec_t = _vec_t;
        using base = base_t;
        using Base = big_vector<vec_t>;
        using Base::Base;

        matrix(size_t n): matrix(n, n) {}
        matrix(size_t n, size_t m) {
            Base::reserve(n);
            for(size_t i = 0; i < n; i++) Base::emplace_back(m);
        }

        matrix(Base const& t): Base(t) {}
        matrix(Base &&t): Base(std::move(t)) {}
        
        template<std::ranges::input_range R>
        matrix(R &&r): Base(r.begin(), r.end()) {}

        size_t n() const {return size(*this);}
        size_t m() const {return n() ? size(row(0)) : 0;}
        
        void resize(size_t n, size_t m) {
            Base::resize(n);
            for(auto &it: *this) {
                it.resize(m);
            }
        }

        auto& row(size_t i) {return (*this)[i];}
        auto const& row(size_t i) const {return (*this)[i];}

        auto elements() {return *this | std::views::join;}
        auto elements() const {return *this | std::views::join;}

        matrix operator-() const {
            return *this | std::views::transform([](auto const& x) {return vec_t(-x);});
        }
        matrix& operator+=(matrix const& t) {
            for(auto [a, b]: std::views::zip(*this, t)) {
                for(auto [x, y]: std::views::zip(a, b)) x += y;
            }
            return *this;
        }
        matrix& operator -=(matrix const& t) {
            for(auto [a, b]: std::views::zip(*this, t)) {
                for(auto [x, y]: std::views::zip(a, b)) x -= y;
            }
            return *this;
        }
        matrix operator+(matrix const& t) const {return matrix(*this) += t;}
        matrix operator-(matrix const& t) const {return matrix(*this) -= t;}
        
        matrix& operator *=(base t) {for(auto &it: *this) it *= t; return *this;}
        matrix operator *(base t) const {return matrix(*this) *= t;}
        matrix& operator /=(base t) {return *this *= base(1) / t;}
        matrix operator /(base t) const {return matrix(*this) /= t;}

        // Make sure the result is matrix, not Base
        matrix& operator *=(matrix const& t) {return *this = *this * t;}

        void read_transposed() {
            for(size_t j = 0; j < m(); j++) {
                for(size_t i = 0; i < n(); i++) {
                    std::cin >> (*this)[i][j];
                }
            }
        }
        void read() {
            for(auto &it: *this) {
                it.read();
            }
        }
        void print() const {
            for(auto const& it: *this) {
                it.print();
            }
        }

        static matrix block_diagonal(big_vector<matrix> const& blocks) {
            size_t n = 0;
            for(auto &it: blocks) {
                assert(it.n() == it.m());
                n += it.n();
            }
            matrix res(n);
            n = 0;
            for(auto &it: blocks) {
                for(size_t i = 0; i < it.n(); i++) {
                    std::ranges::copy(it[i], begin(res[n + i]) + n);
                }
                n += it.n();
            }
            return res;
        }
        static matrix random(size_t n, size_t m) {
            matrix res(n, m);
            std::ranges::generate(res, std::bind(vec_t::random, m));
            return res;
        }
        static matrix random(size_t n) {
            return random(n, n);
        }
        static matrix eye(size_t n) {
            matrix res(n);
            for(size_t i = 0; i < n; i++) {
                res[i][i] = 1;
            }
            return res;
        }

        // Concatenate matrices
        matrix operator |(matrix const& b) const {
            assert(n() == b.n());
            matrix res(n(), 0);
            for(size_t i = 0; i < n(); i++) {
                res[i] = row(i) | b[i];
            }
            return res;
        }
        void assign_submatrix(auto viewx, auto viewy, matrix const& t) {
            for(auto [a, b]: std::views::zip(*this | viewx, t)) {
                std::ranges::copy(b, begin(a | viewy));
            }
        }
        auto submatrix(auto viewx, auto viewy) const {
            return *this | viewx | std::views::transform([viewy](auto const& y) {
                return y | viewy;
            });
        }

        matrix T() const {
            matrix res(m(), n());
            constexpr size_t block = 128;
            for(size_t first = 0; first < m(); first += block) {
                size_t last = std::min(first + block, m());
                for(size_t i = 0; i < n(); i++) {
                    auto const& src = row(i);
                    for(size_t j = first; j < last; j++) res[j][i] = src[j];
                }
            }
            return res;
        }

        matrix operator *(matrix const& b) const {
            assert(m() == b.n());
            if constexpr(impl::use_strassen<vec_t>) {
                if(std::min({n(), m(), b.m()}) >= (base::bits <= 32 ? 64 : 512)) {
                    return impl::strassen_product<base::mod()>::product(*this, b);
                }
            }
            matrix res(n(), b.m());
            if constexpr(requires { requires vec_t::use_simd; }) {
                if(n() == 1) {
                    res[0] = b.apply(row(0));
                    return res;
                }
            }
            constexpr size_t block = 32;
            for(size_t first = 0; first < m(); first += block) {
                size_t last = std::min(first + block, m());
                size_t i = 0;
                for(; i + 1 < n(); i += 2) {
                    size_t j = first;
                    if constexpr(requires { requires vec_t::use_simd; }) {
                        constexpr size_t batch = vec_t::batch_size;
                        if(b.m() >= 8) for(; j + batch <= last; j += batch) {
                            std::array<typename vec_t::Base const*, batch> src;
                            std::array<base, 2 * batch> c;
                            for(size_t t = 0; t < batch; t++) {
                                src[t] = &b[j + t];
                                c[t] = row(i)[j + t]; c[batch + t] = row(i + 1)[j + t];
                            }
                            vec_t::template add_scaled_batch<batch, 2>({&res[i], &res[i + 1]}, src, c);
                        }
                    }
                    for(; j + 1 < last; j += 2) {
                        add_scaled_pair(res[i], res[i + 1], b[j], b[j + 1],
                            {row(i)[j], row(i)[j + 1], row(i + 1)[j], row(i + 1)[j + 1]});
                    }
                    if(j < last) {
                        res[i].add_scaled(b[j], row(i)[j]);
                        res[i + 1].add_scaled(b[j], row(i + 1)[j]);
                    }
                }
                for(; i < n(); i++) {
                    for(size_t j = first; j < last; j++) {
                        res[i].add_scaled(b[j], row(i)[j]);
                    }
                }
            }
            res.normalize();
            return res;
        }

        vec_t apply(vec_t const& x) const {
            assert(x.size() == n());
            vec_t res(m());
            size_t i = 0;
            if constexpr(requires { requires vec_t::use_simd; }) {
                constexpr size_t batch = vec_t::batch_size;
                for(; i + batch <= n(); i += batch) {
                    std::array<typename vec_t::Base const*, batch> src;
                    std::array<base, batch> c;
                    for(size_t t = 0; t < batch; t++) {src[t] = &row(i + t); c[t] = x[i + t];}
                    vec_t::template add_scaled_batch<batch, 1>({&res}, src, c);
                }
            }
            for(; i < n(); i++) res.add_scaled(row(i), x[i]);
            res.normalize();
            return res;
        }

        matrix pow(uint64_t k) const {
            assert(n() == m());
            return bpow<3>(*this, k, eye(n()));
        }

        matrix& normalize() {
            for(auto &it: *this) {
                it.normalize();
            }
            return *this;
        }
        template<gauss_mode mode = normal>
        void eliminate(size_t i, size_t k) {
            auto kinv = base(1) / row(i).normalize()[k];
            for(size_t j = (mode == normal) * i; j < n(); j++) {
                if(j != i) {
                    row(j).add_scaled(row(i), -row(j).normalize(k) * kinv);
                }
            }
        }
        template<gauss_mode mode = normal>
        void eliminate(size_t i) {
            row(i).normalize();
            for(size_t j = (mode == normal) * i; j < n(); j++) {
                if(j != i) {
                    row(j).reduce_by(row(i));
                }
            }
        }
        template<gauss_mode mode = normal>
        matrix& gauss() {
            return gauss_to<mode>(m());
        }
    private:
        // Restrict pivot columns, while updating every column of each row.
        template<gauss_mode mode>
        matrix& gauss_to(size_t lim) {
            constexpr size_t block = 32;
            for(size_t first = 0; first < n(); first += block) {
                size_t last = std::min(first + block, n());
                // Reduce the pivot block before applying it to the other rows.
                for(size_t i = first; i < last; i++) {
                    row(i).normalize();
                    if(row(i).find_pivot().first >= lim) continue;
                    for(size_t j = mode == normal ? i + 1 : first; j < last; j++) {
                        if(j != i) row(j).reduce_by(row(i));
                    }
                }
                if constexpr(mode == reverse) {
                    // Later pivots may have changed earlier rows in this block.
                    for(size_t i = first; i < last; i++) row(i).normalize();
                }
                for(size_t j = mode == normal ? last : 0; j < n(); j++) {
                    if(j >= first && j < last) continue;
                    bool pair = j + 1 < n() && j + 1 != first;
                    size_t i = first;
                    if constexpr(requires { requires vec_t::use_simd; }) {
                        constexpr size_t batch = vec_t::batch_size;
                        if(pair) for(; i + batch <= last; i += batch) reduce_batch<mode, batch>(j, i, lim);
                    }
                    if(pair) for(; i + 1 < last; i += 2) reduce_pair<mode>(j, i, lim);
                    for(; i < last; i++) {
                        if(row(i).find_pivot().first >= lim) continue;
                        row(j).reduce_by(row(i));
                        if(pair) row(j + 1).reduce_by(row(i));
                    }
                    j += pair;
                }
            }
            // Forward pivots are normalized before use and never modified afterward.
            return mode == reverse ? normalize() : *this;
        }
    public:
        template<gauss_mode mode = normal>
        auto echelonize(size_t lim) {
            return gauss<mode>().sort_classify(lim);
        }
        template<gauss_mode mode = normal>
        auto echelonize() {
            return echelonize<mode>(m());
        }

        size_t rank() const {
            auto A = n() > m() ? T() : *this;
            A.gauss();
            return std::ranges::count_if(A, [&](auto &row) {
                return row.find_pivot().first < A.m();
            });
        }

        base det() const {
            assert(n() == m());
            matrix b = *this;
            b.gauss();
            base res = 1;
            for(size_t i = 0; i < n(); i++) {
                for(size_t p = b[i].find_pivot().first; p != i; p = b[i].find_pivot().first) {
                    if(p == n()) return 0;
                    std::swap(b[i], b[p]);
                    res = -res;
                }
                res *= b[i][i];
            }
            return res;
        }

        // Pfaffian of an alternating matrix over a field.
        base pfaffian() const {
            assert(n() == m() && n() % 2 == 0);
            matrix b = *this;
            base res = 1;
            for(size_t i = 1; i < n(); i++) {
                for(size_t j = i + 1; j < n() && b[i].normalize(i - 1) == base(0); j++) {
                    if(b[j].normalize(i - 1) != base(0)) {
                        std::swap(b[i], b[j]);
                        for(size_t k = i; k < n(); k++) {
                            std::swap(b[k][i], b[k][j]);
                        }
                        res = -res;
                    }
                }
                b[i].normalize();
                if(i % 2) {
                    res *= -b[i][i - 1];
                    if(res == base(0)) return res;
                }
                if(b[i][i - 1] == base(0)) continue;
                base inv = base(1) / b[i][i - 1];
                for(size_t j = i + 1; j < n(); j++) {
                    b[j].add_scaled(b[i], -b[j].normalize(i - 1) * inv, i);
                }
            }
            return res;
        }

        std::pair<base, matrix> inv() const {
            assert(n() == m());
            matrix b(n(), 2 * n());
            for(size_t i = 0; i < n(); i++) {
                std::ranges::copy(row(i), begin(b[i]));
                b[i][n() + i] = 1;
            }
            // Pivots in the appended identity cannot make A invertible.
            auto [pivots, free] = b.template gauss_to<normal>(n()).sort_classify(n());
            if(size(pivots) < n()) {
                return {0, {}};
            }
            base det = 1;
            for(size_t i = 0; i < n(); i++) {
                det *= b[i][i];
            }
            matrix res = b.submatrix(std::views::all, std::views::drop(n()));
            back_substitute(b, pivots, res);
            return {det, std::move(res)};
        }

        auto kernel() const {
            return kernel_of(*this);
        }

        // [solution, basis], transposed
        std::optional<std::array<matrix, 2>> solve(matrix const& t) const {
            matrix sols = kernel_of(*this | t);
            if(sols.n() < t.m() || matrix(sols.submatrix(
                std::views::drop(sols.n() - t.m()),
                std::views::drop(m())
            )) != -eye(t.m())) {
                return std::nullopt;
            } else {
                for(auto &row: sols) row.resize(m());
                auto first = begin(sols) + (sols.n() - t.m());
                matrix sol(std::make_move_iterator(first), std::make_move_iterator(end(sols)));
                sols.erase(first, end(sols));
                return std::array{std::move(sol), std::move(sols)};
            }
        }

        // To be called after a gaussian elimination run
        // Sorts rows by pivots and classifies
        // variables into pivots and free
        auto sort_classify(size_t lim) {
            size_t rk = 0;
            big_vector<size_t> free, pivots;
            for(size_t j = 0; j < lim; j++) {
                for(size_t i = rk + 1; i < n() && row(rk)[j] == base(0); i++) {
                    if(row(i)[j] != base(0)) {
                        std::swap(row(i), row(rk));
                        row(rk) = -row(rk);
                    }
                }
                if(rk < n() && row(rk)[j] != base(0)) {
                    pivots.push_back(j);
                    rk++;
                } else {
                    free.push_back(j);
                }
            }
            return std::array{std::move(pivots), std::move(free)};
        }
    private:
        // Own the working matrix so augmented temporaries do not need a copy.
        static matrix kernel_of(matrix A) {
            auto [pivots, free] = A.echelonize();
            matrix rhs(size(pivots), size(free));
            for(size_t i = 0; i < size(pivots); i++) {
                for(size_t j = 0; j < size(free); j++) {
                    rhs[i][j] = A[i][free[j]];
                }
            }
            back_substitute(A, pivots, rhs);
            matrix sols(size(free), A.m());
            for(size_t j = 0; j < size(pivots); j++) {
                for(size_t i = 0; i < size(free); i++) {
                    sols[i][pivots[j]] = rhs[j][i];
                }
            }
            for(size_t i = 0; i < size(free); i++) {
                sols[i][free[i]] = -1;
            }
            return sols;
        }

        // Solve an upper-echelon system for the supplied right-hand sides.
        static void back_substitute(matrix const& A, auto const& pivots, matrix &b) {
            for(size_t i = size(pivots); i-- > 0;) {
                b[i].normalize();
                b[i] *= base(1) / A[i][pivots[i]];
                for(size_t j = 0; j < i; j++) {
                    b[j].add_scaled(b[i], -A[j][pivots[i]]);
                }
            }
        }

        static void add_scaled_pair(vec_t &x, vec_t &y, vec_t const& p, vec_t const& q,
                                    std::array<base, 4> c, size_t first = 0) {
            if constexpr(requires { vec_t::add_scaled_pair(x, y, p, q, c, first); }) {
                vec_t::add_scaled_pair(x, y, p, q, c, first);
            } else {
                x.add_scaled(p, c[0], first); x.add_scaled(q, c[1], first);
                y.add_scaled(p, c[2], first); y.add_scaled(q, c[3], first);
            }
        }
        // Determine the sequential pivot coefficients before updating the full rows.
        template<gauss_mode mode, size_t count>
        void reduce_batch(size_t dst, size_t src, size_t lim) {
            static_assert(count <= 8);
            std::array<typename vec_t::Base const*, count> sources;
            std::array<size_t, count> pivots;
            std::array<base, count> inverses;
            size_t first = lim;
            for(size_t t = 0; t < count; t++) {
                sources[t] = &row(src + t);
                auto [p, inv] = row(src + t).find_pivot();
                pivots[t] = p; inverses[t] = p < lim ? inv : base(0);
                first = std::min(first, p);
            }
            if(first == lim) return;
            std::array<base, 2 * count> c{};
            for(size_t r = 0; r < 2; r++) for(size_t t = 0; t < count; t++) {
                if(pivots[t] >= lim) continue;
                base value = row(dst + r).normalize(pivots[t]);
                if constexpr(mode == normal) {
                    // Fewer than eight canonical products fit in a 64-bit accumulator.
                    uint64_t sum = value.getr();
                    for(size_t h = 0; h < t; h++) {
                        sum += uint64_t(c[r * count + h].getr()) * (*sources[h])[pivots[t]].getr();
                    }
                    value.setr(sum % base::mod());
                }
                c[r * count + t] = -value * inverses[t];
            }
            vec_t::template add_scaled_batch<count, 2>({&row(dst), &row(dst + 1)}, sources, c, first);
        }
        // Fuse two sequential reductions, accounting for the first one's effect
        // on the second pivot before updating either destination row.
        template<gauss_mode mode>
        void reduce_pair(size_t dst, size_t src, size_t lim) {
            auto &p = row(src), &q = row(src + 1);
            auto [u, pu] = p.find_pivot();
            auto [v, qv] = q.find_pivot();
            if(u >= lim || v >= lim) {
                for(size_t j = dst; j < dst + 2; j++) {
                    if(u < lim) row(j).reduce_by(p);
                    if(v < lim) row(j).reduce_by(q);
                }
                return;
            }
            // Canonical reduction coefficients fit in a 64-bit product.
            auto mul = [](base x, base y) {
                if constexpr(impl::use_strassen<vec_t>) {
                    x.setr(uint64_t(x.getr()) * y.getr() % base::mod());
                    return x;
                } else return x * y;
            };
            auto scales = [&](vec_t &a) {
                base s = mul(-a.normalize(u), pu);
                base t = -a.normalize(v);
                // Reverse elimination has already cleared p[v] within the pivot block.
                if constexpr(mode == normal) t -= mul(s, p[v]);
                t = mul(t, qv);
                return std::array{s, t};
            };
            auto a = scales(row(dst)), b = scales(row(dst + 1));
            add_scaled_pair(row(dst), row(dst + 1), p, q,
                            {a[0], a[1], b[0], b[1]}, std::min(u, v));
        }
    };
    template<typename base_t>
    auto operator *(base_t t, matrix<base_t> const& A) {return A * t;}
}
#pragma GCC pop_options

#line 2 ".verify-helper/cache/dense-align-20260922/bundle-system.cpp"

#line 8 "verify/linalg/system.test.cpp"

using namespace std;
using namespace cp_algo::linalg;
using namespace cp_algo::math;

const int64_t mod = 998244353;

void solve() {
    int n, m;
    cin >> n >> m;
    matrix<modint<mod>> A(n, m), b(n, 1);
    A.read();
    b.read();
    auto x = A.solve(b);
    if(!x) {
        cout << -1 << "\n";
    } else {
        auto [sol, basis] = *x;
        cout << basis.n() << "\n";
        sol.print();
        basis.print();
    }
}

signed main() {
    //freopen("input.txt", "r", stdin);
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    while(t--) {
        solve();
    }
}
