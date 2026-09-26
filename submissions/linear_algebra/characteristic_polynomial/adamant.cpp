#include <cassert>
#line 1 "verify/linalg/characteristic.test.cpp"
// @brief Characteristic Polynomial
#define PROBLEM "https://judge.yosupo.jp/problem/characteristic_polynomial"
#include <bits/stdc++.h>
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2")
#define CP_ALGO_MAXN 1 << 10
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
#line 1 "cp-algo/linalg/frobenius.hpp"


#line 1 "cp-algo/math/poly/powmod.hpp"


#line 1 "cp-algo/math/poly/div.hpp"


#line 1 "cp-algo/math/poly/impl/div.hpp"


#line 1 "cp-algo/math/poly/series/inv.hpp"


#line 1 "cp-algo/math/poly/base.hpp"


#line 1 "cp-algo/math/fft.hpp"


#line 1 "cp-algo/math/ring.hpp"


#line 1 "cp-algo/number_theory/discrete_sqrt.hpp"


#line 1 "cp-algo/number_theory/modint.hpp"


#line 1 "cp-algo/math/common.hpp"


#line 5 "cp-algo/math/common.hpp"
#include <cassert>
#include <bit>
#line 9 "cp-algo/math/common.hpp"
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

#line 5 "cp-algo/number_theory/modint.hpp"
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

#line 1 "cp-algo/random/rng.hpp"


#line 5 "cp-algo/random/rng.hpp"
namespace cp_algo::random {
    std::mt19937_64 gen(
        std::chrono::steady_clock::now().time_since_epoch().count()
    );
    uint64_t rng() {
        return gen();
    }
}

#line 1 "cp-algo/math/affine.hpp"


#include <optional>
#line 5 "cp-algo/math/affine.hpp"
#include <cassert>
#line 7 "cp-algo/math/affine.hpp"
namespace cp_algo::math {
    // a * x + b
    template<typename base>
    struct lin {
        base a = 1, b = 0;
        std::optional<base> c;
        lin() {}
        lin(base b): a(0), b(b) {}
        lin(base a, base b): a(a), b(b) {}
        lin(base a, base b, base _c): a(a), b(b), c(_c) {}

        // polynomial product modulo x^2 - c
        lin operator * (const lin& t) {
            assert(c && t.c && *c == *t.c);
            return {a * t.b + b * t.a, b * t.b + a * t.a * (*c), *c};
        }

        // a * (t.a * x + t.b) + b
        lin apply(lin const& t) const {
            return {a * t.a, a * t.b + b};
        }

        void prepend(lin const& t) {
            *this = t.apply(*this);
        }

        base eval(base x) const {
            return a * x + b;
        }
    };

    // (ax+b) / (cx+d)
    template<typename base>
    struct linfrac {
        base a, b, c, d;
        linfrac(): a(1), b(0), c(0), d(1) {} // x, identity for composition
        linfrac(base a): a(a), b(1), c(1), d(0) {} // a + 1/x, for continued fractions
        linfrac(base a, base b, base c, base d): a(a), b(b), c(c), d(d) {}

        // composition of two linfracs
        linfrac operator * (linfrac t) const {
            return t.prepend(linfrac(*this));
        }

        linfrac operator-() const {
            return {-a, -b, -c, -d};
        }

        linfrac adj() const {
            return {d, -b, -c, a};
        }
        
        linfrac& prepend(linfrac const& t) {
            t.apply(a, c);
            t.apply(b, d);
            return *this;
        }

        // apply linfrac to A/B
        void apply(base &A, base &B) const {
            std::tie(A, B) = std::pair{a * A + b * B, c * A + d * B};
        }
    };
}

#line 6 "cp-algo/number_theory/discrete_sqrt.hpp"
namespace cp_algo::math {
    // https://en.wikipedia.org/wiki/Berlekamp-Rabin_algorithm
    template<modint_type base>
    std::optional<base> sqrt(base b) {
        if(b == base(0)) {
            return base(0);
        } else if(bpow(b, (b.mod() - 1) / 2) != base(1)) {
            return std::nullopt;
        } else {
            while(true) {
                base z = random::rng();
                if(z * z == b) {
                    return z;
                }
                lin<base> x(1, z, b); // x + z (mod x^2 - b)
                x = bpow(x, (b.mod() - 1) / 2, lin<base>(0, 1, b));
                if(x.a != base(0)) {
                    return x.a.inv();
                }
            }
        }
    }
}

#line 1 "cp-algo/number_theory/primality.hpp"


#line 6 "cp-algo/number_theory/primality.hpp"
namespace cp_algo::math {
    // https://en.wikipedia.org/wiki/Miller–Rabin_primality_test
    template<typename _Int>
    bool is_prime(_Int m) {
        using Int = std::make_signed_t<_Int>;
        using UInt = std::make_unsigned_t<Int>;
        if(m == 1 || m % 2 == 0) {
            return m == 2;
        }
        // m - 1 = 2^s * d
        int s = std::countr_zero(UInt(m - 1));
        auto d = (m - 1) >> s;
        using base = dynamic_modint<Int>;
        auto test = [&](base x) {
            x = bpow(x, d);
            if(std::abs(x.rem()) <= 1) {
                return true;
            }
            for(int i = 1; i < s && x != -1; i++) {
                x *= x;
            }
            return x == -1;
        };
        return base::with_mod(m, [&]() {
#ifdef CP_ALGO_NUMBER_THEORY_PRIMALITY_BASES_HPP
            uint16_t base2 = 7, base3 = 61;
            if (m != uint32_t(m)) {
                base2 = base_table1[uint32_t(m * 0xAD625B89) >> 18];
                base3 = base_table2[base2 >> 13];
            }
            return test(2) && test(base2) && test(base3);
#else
            return std::ranges::all_of(std::array{2, 325, 9375, 28178, 450775, 9780504, 1795265022}, test);
#endif
        });
    }
}

#line 1 "cp-algo/util/checkpoint.hpp"


#line 1 "cp-algo/util/big_alloc.hpp"



#line 14 "cp-algo/util/big_alloc.hpp"

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

#line 1 "cp-algo/math/cvector.hpp"


#line 1 "cp-algo/util/simd.hpp"


#include <experimental/simd>
#line 7 "cp-algo/util/simd.hpp"

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

#line 1 "cp-algo/util/complex.hpp"


#line 5 "cp-algo/util/complex.hpp"
#include <type_traits>
#line 7 "cp-algo/util/complex.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo {
    // Custom implementation, since std::complex is UB on non-floating types
    template<typename T>
    struct complex {
        using value_type = T;
        T x, y;
        inline constexpr complex(): x(), y() {}
        inline constexpr complex(T const& x): x(x), y() {}
        inline constexpr complex(T const& x, T const& y): x(x), y(y) {}
        inline complex& operator *= (T const& t) {x *= t; y *= t; return *this;}
        inline complex& operator /= (T const& t) {x /= t; y /= t; return *this;}
        inline complex operator * (T const& t) const {return complex(*this) *= t;}
        inline complex operator / (T const& t) const {return complex(*this) /= t;}
        inline complex& operator += (complex const& t) {x += t.x; y += t.y; return *this;}
        inline complex& operator -= (complex const& t) {x -= t.x; y -= t.y; return *this;}
        inline complex operator * (complex const& t) const {return {x * t.x - y * t.y, x * t.y + y * t.x};}
        inline complex operator / (complex const& t) const {return *this * t.conj() / t.norm();}
        inline complex operator + (complex const& t) const {return complex(*this) += t;}
        inline complex operator - (complex const& t) const {return complex(*this) -= t;}
        inline complex& operator *= (complex const& t) {return *this = *this * t;}
        inline complex& operator /= (complex const& t) {return *this = *this / t;}
        inline complex operator - () const {return {-x, -y};}
        inline complex conj() const {return {x, -y};}
        inline T norm() const {return x * x + y * y;}
        inline T abs() const {return std::sqrt(norm());}
        inline T const real() const {return x;}
        inline T const imag() const {return y;}
        inline T& real() {return x;}
        inline T& imag() {return y;}
        inline static constexpr complex polar(T r, T theta) {return {T(r * cos(theta)), T(r * sin(theta))};}
        inline auto operator <=> (complex const& t) const = default;
    };
    template<typename T> inline complex<T> conj(complex<T> const& x) {return x.conj();}
    template<typename T> inline T norm(complex<T> const& x) {return x.norm();}
    template<typename T> inline T abs(complex<T> const& x) {return x.abs();}
    template<typename T> inline T& real(complex<T> &x) {return x.real();}
    template<typename T> inline T& imag(complex<T> &x) {return x.imag();}
    template<typename T> inline T const real(complex<T> const& x) {return x.real();}
    template<typename T> inline T const imag(complex<T> const& x) {return x.imag();}
    template<typename T>
    inline constexpr complex<T> polar(T r, T theta) {
        return complex<T>::polar(r, theta);
    }
    template<typename T>
    inline std::ostream& operator << (std::ostream &out, complex<T> const& x) {
        return out << x.real() << ' ' << x.imag();
    }
}
#pragma GCC pop_options

#line 7 "cp-algo/math/cvector.hpp"
#include <immintrin.h>
#include <numbers>
#line 10 "cp-algo/math/cvector.hpp"
#include <ranges>
#line 13 "cp-algo/math/cvector.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace stdx = std::experimental;
namespace cp_algo::math::fft {
    static constexpr size_t flen = 4;
    using ftype = double;
    using vftype = dx4;
    using point = complex<ftype>;
    using vpoint = complex<vftype>;
    static constexpr vftype vz = {};
    vpoint vi(vpoint const& r) {
        return {-imag(r), real(r)};
    }

    // Spectrum storage skips zero-filling on resize; every user writes before reading.
    template<class T>
    struct spectrum_alloc: big_alloc<T> {
        using big_alloc<T>::big_alloc;
        template<class U> struct rebind { using other = spectrum_alloc<U>; };
        template<class U> requires (std::is_same_v<U, vpoint>)
        void construct(U*) noexcept {}
    };
    using spectrum_vector = std::vector<vpoint, spectrum_alloc<vpoint>>;

    struct cvector {
        spectrum_vector r;
        cvector(size_t n) {
            n = std::max(flen, std::bit_ceil(n));
            r.assign(n / flen, vpoint{});
            prepare_roots(n / 16);
            checkpoint("cvector create");
        }

        vpoint& at(size_t k) {return r[k / flen];}
        vpoint at(size_t k) const {return r[k / flen];}
        template<class pt = point>
        inline void set(size_t k, pt const& t) {
            if constexpr(std::is_same_v<pt, point>) {
                real(r[k / flen])[k % flen] = real(t);
                imag(r[k / flen])[k % flen] = imag(t);
            } else {
                at(k) = t;
            }
        }
        template<class pt = point>
        inline pt get(size_t k) const {
            if constexpr(std::is_same_v<pt, point>) {
                return {real(r[k / flen])[k % flen], imag(r[k / flen])[k % flen]};
            } else {
                return at(k);
            }
        }

        size_t size() const {
            return flen * r.size();
        }
        static constexpr size_t eval_arg(size_t n) {
            if(n < pre_evals) {
                return eval_args[n];
            } else {
                return eval_arg(n / 2) | (n & 1) << (std::bit_width(n) - 1);
            }
        }
        static constexpr point eval_point(size_t n) {
            if(n % 2) {
                return -eval_point(n - 1);
            } else if(n % 4) {
                return eval_point(n - 2) * point(0, 1);
            } else if(n / 4 < pre_evals) {
                return evalp[n / 4];
            } else if(n / 4 - pre_evals < extra.size()) {
                return extra[n / 4 - pre_evals];
            } else {
                return polar<ftype>(1., std::numbers::pi / (ftype)std::bit_floor(n) * (ftype)eval_arg(n));
            }
        }
        static constexpr std::array<point, 32> roots = []() {
            std::array<point, 32> res;
            for(size_t i = 2; i < 32; i++) {
                res[i] = polar<ftype>(1., std::numbers::pi / (1ull << (i - 2)));
            }
            return res;
        }();
        static constexpr point root(size_t n) {
            return roots[std::bit_width(n)];
        }
        template<int step>
        static void exec_on_eval(size_t n, size_t k, auto &&callback) {
            callback(k, root(4 * step * n) * eval_point(step * k));
        }
        template<int step>
        static void exec_on_evals(size_t n, auto &&callback) {
            point factor=root(4*step*n);
            if constexpr(step==1 || step==2 || step==4){
                prepare_roots((step*n+3)/4);
                size_t i=0;
                if constexpr(step==1){
                    for(;i+4<=n;i+=4){
                        size_t k=i/4;
                        point e=k<pre_evals?evalp[k]:extra[k-pre_evals];
                        point v=factor*e;
                        callback(i,v);callback(i+1,-v);
                        point iv(-imag(v),real(v));
                        callback(i+2,iv);callback(i+3,-iv);
                    }
                }else if constexpr(step==2){
                    for(;i+2<=n;i+=2){
                        size_t k=i/2;
                        point e=k<pre_evals?evalp[k]:extra[k-pre_evals];
                        point v=factor*e;
                        callback(i,v);callback(i+1,point(-imag(v),real(v)));
                    }
                }
                for(;i<n;i++){
                    size_t index=step*i,k=index/4;
                    point e=k<pre_evals?evalp[k]:extra[k-pre_evals];
                    if(index&2)e=e*point(0,1);
                    if(index&1)e=-e;
                    callback(i,factor*e);
                }
            }else{
                for(size_t i=0;i<n;i++)callback(i,factor*eval_point(step*i));
            }
        }

        static void do_dot_iter(point rt, vpoint& Bv, vpoint const& Av, vpoint& res) {
            res += Av * Bv;
            real(Bv) = rotate_right(real(Bv));
            imag(Bv) = rotate_right(imag(Bv));
            auto x = real(Bv)[0], y = imag(Bv)[0];
            real(Bv)[0] = x * real(rt) - y * imag(rt);
            imag(Bv)[0] = x * imag(rt) + y * real(rt);
        }

        template<size_t fixed = 0>
        void dot(cvector const& t) {
            size_t n = fixed?fixed:this->size();
            exec_on_evals<1>(n / flen, [&](size_t k, point rt) __attribute__((always_inline)) {
                k *= flen;
                auto [Ax, Ay] = at(k);
                auto Cv = t.at(k);
                vpoint vrt = {vz + real(rt), vz + imag(rt)};
                auto Cr = Cv * vrt;
                vpoint res = vz;
                auto iter = [&]<int i>() __attribute__((always_inline)) {
                    auto wrap = [&](vftype original, vftype rotated) {
                        if constexpr(i == 0) {return original;}
                        else {return __builtin_shufflevector(rotated, original, 4-i,5-i,6-i,7-i);}
                    };
                    vpoint Cw = {wrap(real(Cv),real(Cr)),wrap(imag(Cv),imag(Cr))};
                    vpoint Av = {vz+Ax[i],vz+Ay[i]};
                    return Av*Cw;
                };
                auto p0=iter.template operator()<0>(), p1=iter.template operator()<1>();
                auto p2=iter.template operator()<2>(), p3=iter.template operator()<3>();
                res=(p0+p1)+(p2+p3);
                set(k, res);
            });
            checkpoint("dot");
        }
        // normalize=false leaves the inverse-transform scale for the caller.
        template<bool partial = true, bool normalize = true, size_t fixed = 0>
        void ifft() {
            size_t n = fixed?fixed:size();
            if constexpr (!partial) {
                prepare_roots(n / 4);
                point pi(0, 1);
                exec_on_evals<4>(n / 4, [&](size_t k, point rt) __attribute__((always_inline)) {
                    k *= 4;
                    point v1 = conj(rt);
                    point v2 = v1 * v1;
                    point v3 = v1 * v2;
                    auto A = get(k);
                    auto B = get(k + 1);
                    auto C = get(k + 2);
                    auto D = get(k + 3);
                    set(k, (A + B) + (C + D));
                    set(k + 2, ((A + B) - (C + D)) * v2);
                    set(k + 1, ((A - B) - pi * (C - D)) * v1);
                    set(k + 3, ((A - B) + pi * (C - D)) * v3);
                });
            }
            bool parity = std::countr_zero(n) % 2;
            if(parity) {
                exec_on_evals<2>(n / (2 * flen), [&](size_t k, point rt) __attribute__((always_inline)) {
                    k *= 2 * flen;
                    vpoint cvrt = {vz + real(rt), vz - imag(rt)};
                    auto B = at(k) - at(k + flen);
                    at(k) += at(k + flen);
                    at(k + flen) = B * cvrt;
                });
            }

            transform<true,fixed>(n, parity);
            checkpoint("ifft");
            if constexpr(normalize) {
                auto scale = vz + ftype(partial ? flen : 1) / ftype(n);
                for(size_t k = 0; k < n; k += flen) {
                    set(k, get<vpoint>(k) * scale);
                }
            }
        }
        template<bool partial = true, size_t fixed = 0>
        void fft() {
            size_t n = fixed?fixed:size();
            prepare_roots(n / (partial ? 16 : 4));
            bool parity = std::countr_zero(n) % 2;
            transform<false,fixed>(n, parity);
            if(parity) {
                exec_on_evals<2>(n / (2 * flen), [&](size_t k, point rt) __attribute__((always_inline)) {
                    k *= 2 * flen;
                    vpoint vrt = {vz + real(rt), vz + imag(rt)};
                    auto t = at(k + flen) * vrt;
                    at(k + flen) = at(k) - t;
                    at(k) += t;
                });
            }
            if constexpr (!partial) {
                prepare_roots(n / 4);
                point pi(0, 1);
                exec_on_evals<4>(n / 4, [&](size_t k, point rt) __attribute__((always_inline)) {
                    k *= 4;
                    point v1 = rt;
                    point v2 = v1 * v1;
                    point v3 = v1 * v2;
                    auto A = get(k);
                    auto B = get(k + 1) * v1;
                    auto C = get(k + 2) * v2;
                    auto D = get(k + 3) * v3;
                    set(k, (A + C) + (B + D));
                    set(k + 1, (A + C) - (B + D));
                    set(k + 2, (A - C) + pi * (B - D));
                    set(k + 3, (A - C) - pi * (B - D));
                });
            }
            checkpoint("fft");
        }
        static std::array<vpoint,4> transpose(std::array<vpoint,4> const&a){
            auto half=[](auto part){
                auto a0=__m256d(part(0)),a1=__m256d(part(1)),a2=__m256d(part(2)),a3=__m256d(part(3));
                auto t0=_mm256_unpacklo_pd(a0,a1),t1=_mm256_unpackhi_pd(a0,a1),t2=_mm256_unpacklo_pd(a2,a3),t3=_mm256_unpackhi_pd(a2,a3);
                return std::array<vftype,4>{vftype(_mm256_permute2f128_pd(t0,t2,0x20)),vftype(_mm256_permute2f128_pd(t1,t3,0x20)),vftype(_mm256_permute2f128_pd(t0,t2,0x31)),vftype(_mm256_permute2f128_pd(t1,t3,0x31))};
            };
            auto re=half([&](int k){return real(a[k]);}),im=half([&](int k){return imag(a[k]);});
            return {vpoint{re[0],im[0]},vpoint{re[1],im[1]},vpoint{re[2],im[2]},vpoint{re[3],im[3]}};
        }
        // Multiply groups of 16 points as polynomials modulo x^16 - rt: one radix-4 level on
        // each operand, a lane-parallel 4-coefficient Karatsuba product, one inverse level.
        // The weights rt, rt^2, rt^3 of a tile of groups are computed four groups per SIMD
        // operation ahead of the tile, so the product loop only broadcasts them from memory.
        static constexpr size_t dot_tile = 64;
        template<size_t fixed, bool square = false>void dot_fused16(cvector const& t,size_t offset,size_t length){
            constexpr size_t T=dot_tile;
            const size_t n=fixed?fixed:size();
            point factor=root(n);
            auto fr=_mm256_set1_pd(real(factor)),fi=_mm256_set1_pd(imag(factor));
            alignas(32) double tw[6][T];
            const bool ahead=offset+length<n;
            auto cmadd=[](vpoint x,vpoint y,vpoint c) __attribute__((always_inline)) {
                auto re=_mm256_fmadd_pd(__m256d(real(x)),__m256d(real(y)),_mm256_fnmadd_pd(__m256d(imag(x)),__m256d(imag(y)),__m256d(real(c))));
                auto im=_mm256_fmadd_pd(__m256d(real(x)),__m256d(imag(y)),_mm256_fmadd_pd(__m256d(imag(x)),__m256d(real(y)),__m256d(imag(c))));
                return vpoint{vftype(re),vftype(im)};
            };
            auto cmcross=[](vpoint x,vpoint y,vpoint p,vpoint q) __attribute__((always_inline)) {
                auto re=_mm256_sub_pd(_mm256_fmsub_pd(__m256d(real(x)),__m256d(real(y)),__m256d(real(p))),_mm256_fmadd_pd(__m256d(imag(x)),__m256d(imag(y)),__m256d(real(q))));
                auto im=_mm256_add_pd(_mm256_fmsub_pd(__m256d(real(x)),__m256d(imag(y)),__m256d(imag(p))),_mm256_fmsub_pd(__m256d(imag(x)),__m256d(real(y)),__m256d(imag(q))));
                return vpoint{vftype(re),vftype(im)};
            };
            // z * conj(v)
            auto mulconj=[](vpoint z,vpoint v) __attribute__((always_inline)) {
                auto re=_mm256_fmadd_pd(__m256d(real(z)),__m256d(real(v)),_mm256_mul_pd(__m256d(imag(z)),__m256d(imag(v))));
                auto im=_mm256_fmsub_pd(__m256d(imag(z)),__m256d(real(v)),_mm256_mul_pd(__m256d(real(z)),__m256d(imag(v))));
                return vpoint{vftype(re),vftype(im)};
            };
            for(size_t tile=offset;tile<offset+length;tile+=16*T){
                size_t k0=tile/16;
                auto const* e=reinterpret_cast<double const*>(k0<pre_evals?evalp.data()+k0:extra.data()+(k0-pre_evals));
                for(size_t g=0;g<T;g+=4){
                    auto x=_mm256_loadu_pd(e+2*g),y=_mm256_loadu_pd(e+2*g+4);
                    auto er=_mm256_permute4x64_pd(_mm256_unpacklo_pd(x,y),0xD8),ei=_mm256_permute4x64_pd(_mm256_unpackhi_pd(x,y),0xD8);
                    auto r1=_mm256_fmsub_pd(fr,er,_mm256_mul_pd(fi,ei)),i1=_mm256_fmadd_pd(fr,ei,_mm256_mul_pd(fi,er));
                    auto r2=_mm256_fmsub_pd(r1,r1,_mm256_mul_pd(i1,i1)),i2=_mm256_fmadd_pd(r1,i1,_mm256_mul_pd(i1,r1));
                    auto r3=_mm256_fmsub_pd(r1,r2,_mm256_mul_pd(i1,i2)),i3=_mm256_fmadd_pd(r1,i2,_mm256_mul_pd(i1,r2));
                    _mm256_store_pd(tw[0]+g,r1);_mm256_store_pd(tw[1]+g,i1);_mm256_store_pd(tw[2]+g,r2);
                    _mm256_store_pd(tw[3]+g,i2);_mm256_store_pd(tw[4]+g,r3);_mm256_store_pd(tw[5]+g,i3);
                }
                for(size_t g=0;g<T;g++){
                    size_t pos=tile+16*g;
                    if(ahead){
                        // Pull the next block of both operands towards the cache while this one is multiplied.
                        auto const* pa=reinterpret_cast<char const*>(r.data()+(pos+length)/flen);
                        auto const* pb=reinterpret_cast<char const*>(t.r.data()+(pos+length)/flen);
                        for(size_t c=0;c<4;c++){_mm_prefetch(pa+64*c,_MM_HINT_T2);_mm_prefetch(pb+64*c,_MM_HINT_T2);}
                    }
                    auto bc=[&](size_t c) __attribute__((always_inline)) {return vftype(_mm256_broadcast_sd(tw[c]+g));};
                    vpoint v1={bc(0),bc(1)},v2={bc(2),bc(3)},v3={bc(4),bc(5)};
                    auto forward=[&](cvector const& a) __attribute__((always_inline)) {
                        auto A=a.at(pos),B=a.at(pos+4)*v1,C=a.at(pos+8)*v2,D=a.at(pos+12)*v3;
                        return std::array<vpoint,4>{(A+C)+(B+D),(A+C)-(B+D),(A-C)+vi(B-D),(A-C)-vi(B-D)};
                    };
                    auto a=transpose(forward(*this));
                    auto b=[&] {if constexpr(square) return a; else return transpose(forward(t));}();
                    // The four residues are taken modulo x^4 - rt * {1, -1, i, -i}.
                    const auto flip_re=_mm256_set_pd(0.,-0.,-0.,0.),flip_im=_mm256_set_pd(-0.,0.,-0.,0.);
                    vpoint w={vftype(_mm256_xor_pd(_mm256_blend_pd(__m256d(real(v1)),__m256d(imag(v1)),0b1100),flip_re)),
                              vftype(_mm256_xor_pd(_mm256_blend_pd(__m256d(imag(v1)),__m256d(real(v1)),0b1100),flip_im))};
                    auto mul=[&](vpoint a0,vpoint a1,vpoint b0,vpoint b1) __attribute__((always_inline)) {
                        auto p=a0*b0,q=a1*b1;
                        return std::array<vpoint,2>{cmadd(w,q,p),cmcross(a0+a1,b0+b1,p,q)};
                    };
                    auto p=mul(a[0],a[2],b[0],b[2]),q=mul(a[1],a[3],b[1],b[3]);
                    auto m=mul(a[0]+a[1],a[2]+a[3],b[0]+b[1],b[2]+b[3]);
                    auto c=transpose({cmadd(w,q[1],p[0]),(m[0]-p[0])-q[0],p[1]+q[0],(m[1]-p[1])-q[1]});
                    auto A=c[0],B=c[1],C=c[2],D=c[3];
                    at(pos)=(A+B)+(C+D);at(pos+8)=mulconj((A+B)-(C+D),v2);
                    at(pos+4)=mulconj((A-B)-vi(C-D),v1);at(pos+12)=mulconj((A-B)+vi(C-D),v3);
                }
            }
        }
        // A product in two steps, this <- this * t modulo x^n - i up to the factor n / flen:
        // forward() on both operands, then multiply(t), which may be *this for a square.
        // The forward transform stops at groups of 16 points and dot_fused16 multiplies those,
        // which saves a pass over each operand and one over the result. That needs powers of
        // four, so a length 2 * 4^k takes its radix-two level at the top, modulo
        // x^(n/2) -+ sqrt(i); short lengths take the complete transform and the 4-point product.
        void forward() {
            if(!fused_leaves()) {return fft();}
            size_t n = size(), part = fused_part();
            if(part < n) {
                vpoint vrt = {vz + real(roots[4]), vz + imag(roots[4])};
                for(size_t k = 0; k < part; k += flen) {
                    auto t = at(k + part) * vrt;
                    at(k + part) = at(k) - t;
                    at(k) += t;
                }
            }
            for(size_t offset = 0; offset < n; offset += part) {
                transform<false, 0, 0, -1, true>(n, false, offset, part);
            }
            checkpoint("fft");
        }
        void multiply(cvector const& t) {
            if(!fused_leaves()) {dot(t); return ifft<true, false>();}
            size_t n = size(), part = fused_part();
            dot_fused16<0>(t, 0, n);
            checkpoint("dot");
            for(size_t offset = 0; offset < n; offset += part) {
                transform<true, 0, 0, -1, true>(n, false, offset, part);
            }
            if(part < n) {
                vpoint cvrt = {vz + real(roots[4]), vz - imag(roots[4])};
                for(size_t k = 0; k < part; k += flen) {
                    auto t = at(k) - at(k + part);
                    at(k) += at(k + part);
                    at(k + part) = t * cvrt;
                }
            }
            checkpoint("ifft");
        }
        // Radix-64 out-of-cache pass for n = 2^24, run as two tiled radix-8 stages.
        // With Fuse=1 (forward only) the first stage lifts u32 residues to Gaussian
        // coordinates on load, so the spectrum buffer is written once and never re-read
        // before the in-cache block phase. The index-keyed noise makes both branches see
        // the same representatives.
        struct fuse_args {
            const uint32_t* src = nullptr;
            size_t count = 0;
            uint64_t seed = 0;
            double a = 0, b = 0, a_over_p = 0, b_over_p = 0;
        };
        static constexpr size_t sweep_tile = 256;
        template<bool inverse, int Mode, size_t Tile, int Fuse = 0, bool Neg = false>
        void sweep8(fuse_args const& fa = {}) {
            constexpr size_t n = 1 << 24;
            static const std::array<std::array<point, 7>, 9> weights = []() {
                std::array<std::array<point, 7>, 9> table;
                for(size_t count: {size_t(1), size_t(8)}) for(size_t k = 0; k < count; k++) {
                    size_t rev = 0, x = k;
                    for(size_t c = count; c > 1; c >>= 1) {rev = (rev << 1) | (x & 1); x >>= 1;}
                    long double angle = std::numbers::pi_v<long double> * (1 + 4 * rev) / (16 * count);
                    if(k & 1) {angle -= std::numbers::pi_v<long double> / 4;}
                    for(size_t j = 1; j < 8; j++) {
                        long double a = j * angle;
                        if constexpr(Mode == 0) {table[(count - 1) / 7 + k][j - 1] = {double(cosl(a)), double(sinl(a))};}
                        else {table[(count - 1) / 7 + k][j - 1] = {double(sinl(a)), double(tanl(a / 2))};}
                    }
                }
                return table;
            }();
            auto lift = [&](size_t idx) __attribute__((always_inline)) -> vpoint {
                i32x4 bits{};
                if(idx + 4 <= fa.count) {std::memcpy(&bits, fa.src + idx, sizeof(bits));}
                else if(idx < fa.count) {for(size_t j = 0; j < fa.count - idx; j++) {bits[j] = int32_t(fa.src[idx + j]);}}
                else {return vpoint{vz, vz};}
                auto x = __builtin_convertvector(bits, vftype);
                u32x4 h = u32x4{uint32_t(idx), uint32_t(idx + 1), uint32_t(idx + 2), uint32_t(idx + 3)} ^ uint32_t(fa.seed);
                h *= 0x9E3779B1u; h ^= h >> 15; h *= 0x85EBCA77u; h ^= h >> 13; h *= 0xC2B2AE3Du; h ^= h >> 16;
                auto noise = __builtin_convertvector(i32x4(h), vftype) * 0x1p-32;
                auto q = round(x * fa.a_over_p + noise), t = round(x * fa.b_over_p + noise);
                auto re = x - q * fa.a - t * fa.b, im = t * fa.a - q * fa.b;
                return vpoint{re, Neg ? -im : im};
            };
            auto stage = [&]<bool top>(size_t offset, size_t length, size_t begin, size_t end) __attribute__((always_inline)) {
                size_t step = length / 8, k = offset / length;
                auto const& w = weights[(n / length - 1) / 7 + k];
                auto rot = [&]<size_t J>(vpoint z) __attribute__((always_inline)) {
                    auto c = w[J - 1];
                    if constexpr(Mode == 0) {return z * vpoint{vz + real(c), inverse ? vz - imag(c) : vz + imag(c)};}
                    else {
                        auto s = inverse ? vz - real(c) : vz + real(c), t = inverse ? vz - imag(c) : vz + imag(c);
                        auto x = vftype(_mm256_fnmadd_pd(__m256d(t), __m256d(imag(z)), __m256d(real(z))));
                        auto y = vftype(_mm256_fmadd_pd(__m256d(s), __m256d(x), __m256d(imag(z))));
                        return vpoint{vftype(_mm256_fnmadd_pd(__m256d(t), __m256d(y), __m256d(x))), y};
                    }
                };
                auto add = [](vpoint a, vpoint b) __attribute__((always_inline)) {
                    if constexpr(Mode != 2) {return a + b;}
                    else {return vpoint{vftype(_mm256_fmadd_pd(__m256d(real(a)), _mm256_set1_pd(1), __m256d(real(b)))), vftype(_mm256_fmadd_pd(__m256d(imag(a)), _mm256_set1_pd(1), __m256d(imag(b))))};}
                };
                auto sub = [](vpoint a, vpoint b) __attribute__((always_inline)) {
                    if constexpr(Mode != 2) {return a - b;}
                    else {return vpoint{vftype(_mm256_fmsub_pd(__m256d(real(a)), _mm256_set1_pd(1), __m256d(real(b)))), vftype(_mm256_fmsub_pd(__m256d(imag(a)), _mm256_set1_pd(1), __m256d(imag(b))))};}
                };
                auto d4 = [](vpoint a, vpoint b, vpoint c, vpoint d) __attribute__((always_inline)) {
                    auto s = a + c, t = a - c, u = b + d, v = vi(b - d);
                    if constexpr(inverse) {return std::array<vpoint, 4>{s + u, t - v, s - u, t + v};}
                    else {return std::array<vpoint, 4>{s + u, t + v, s - u, t - v};}
                };
                constexpr double q = 0.707106781186547524400844362104849039;
                auto r1 = [](vpoint z) __attribute__((always_inline)) {
                    if constexpr(inverse) {return vpoint{(real(z) + imag(z)) * q, (imag(z) - real(z)) * q};}
                    else {return vpoint{(real(z) - imag(z)) * q, (real(z) + imag(z)) * q};}
                };
                auto r3 = [](vpoint z) __attribute__((always_inline)) {
                    if constexpr(inverse) {return vpoint{(imag(z) - real(z)) * q, (-imag(z) - real(z)) * q};}
                    else {return vpoint{(-real(z) - imag(z)) * q, (real(z) - imag(z)) * q};}
                };
                std::array<vpoint*, 8> input, output;
                for(size_t j = 0; j < 8; j++) {input[j] = output[j] = r.data() + (offset + begin + j * step) / flen;}
                if(k & 1) {
                    constexpr std::array<size_t, 8> perm = {7, 6, 4, 5, 0, 1, 2, 3};
                    for(size_t j = 0; j < 8; j++) {
                        if constexpr(inverse) {input[j] = output[perm[j]];}
                        else {output[j] = input[perm[j]];}
                    }
                }
                constexpr bool fused_in = top && Fuse == 1 && !inverse;
                for(size_t j = 0; j < (end - begin) / flen; j++) {
                    size_t base = offset + begin + j * flen;
                    auto in = [&]<size_t S>() __attribute__((always_inline)) {
                        if constexpr(fused_in) {return lift(base + S * step);}
                        else {return input[S][j];}
                    };
                    if constexpr(!inverse) {
                        auto E = d4(in.template operator()<0>(), rot.template operator()<2>(in.template operator()<2>()), rot.template operator()<4>(in.template operator()<4>()), rot.template operator()<6>(in.template operator()<6>()));
                        auto O = d4(rot.template operator()<1>(in.template operator()<1>()), rot.template operator()<3>(in.template operator()<3>()), rot.template operator()<5>(in.template operator()<5>()), rot.template operator()<7>(in.template operator()<7>()));
                        auto a = O[0], b = r1(O[1]), c = vi(O[2]), d = r3(O[3]);
                        output[0][j] = add(E[0], a); output[1][j] = sub(E[0], a);
                        output[2][j] = add(E[2], c); output[3][j] = sub(E[2], c);
                        output[4][j] = add(E[1], b); output[5][j] = sub(E[1], b);
                        output[6][j] = add(E[3], d); output[7][j] = sub(E[3], d);
                    } else {
                        auto E = d4(add(input[0][j], input[1][j]), add(input[4][j], input[5][j]), add(input[2][j], input[3][j]), add(input[6][j], input[7][j]));
                        auto O = d4(sub(input[0][j], input[1][j]), r1(sub(input[4][j], input[5][j])), -vi(sub(input[2][j], input[3][j])), r3(sub(input[6][j], input[7][j])));
                        output[0][j] = E[0]; output[1][j] = rot.template operator()<1>(O[0]);
                        output[2][j] = rot.template operator()<2>(E[1]); output[3][j] = rot.template operator()<3>(O[1]);
                        output[4][j] = rot.template operator()<4>(E[2]); output[5][j] = rot.template operator()<5>(O[2]);
                        output[6][j] = rot.template operator()<6>(E[3]); output[7][j] = rot.template operator()<7>(O[3]);
                    }
                }
            };
            constexpr size_t h = n / 64;
            for(size_t j = 0; j < h; j += Tile) {
                size_t end = std::min(h, j + Tile);
                auto first = [&]() {for(size_t t = 0; t < 8; t++) {stage.template operator()<true>(0, n, j + t * h, end + t * h);}};
                auto second = [&]() {for(size_t k = 0; k < 8; k++) {stage.template operator()<false>(k * n / 8, n / 8, j, end);}};
                if constexpr(inverse) {second(); first();} else {first(); second();}
            }
        }
        // Product for n = 2^24: the top three stages split each input into 64 independent
        // blocks. Both forward transforms read the u32 inputs directly; each block then
        // completes its two forward transforms, the product, and the inverse before the
        // final three inverse stages combine the results.
        // Fusing the Gaussian lift into the first pass saves a write and a read of each
        // spectrum, but makes that pass compute-bound on the judge (measured slower there),
        // so it stays opt-in; by default both spectra are filled first and swept in place.
        static constexpr bool fuse_forward = false;
        template<bool Neg, bool Fused = fuse_forward, bool Square = false>
        void cache_product(cvector& b, fuse_args const& fa = {}, fuse_args const& fb = {}) {
            constexpr size_t n = 1 << 24, block = 1 << 18;
            prepare_roots(n / 16); prepare_shear_roots();
            if constexpr(Fused) {
                sweep8<false, 2, sweep_tile, 1, Neg>(fa);
                if constexpr(!Square) {b.sweep8<false, 2, sweep_tile, 1, Neg>(fb);}
            } else {
                sweep8<false, 2, sweep_tile>();
                if constexpr(!Square) {b.sweep8<false, 2, sweep_tile>();}
            }
            checkpoint("sweep forward");
            for(size_t offset = 0; offset < n; offset += block) {
                transform<false, n, block, 0, true>(n, false, offset, block);
                if constexpr(!Square) {b.transform<false, n, block, 0, true>(n, false, offset, block);}
                dot_fused16<n, Square>(b, offset, block);
                transform<true, n, block, 0, true>(n, false, offset, block);
            }
            checkpoint("blocks");
            sweep8<true, 2, sweep_tile>();
            checkpoint("sweep inverse");
        }
        static constexpr size_t pre_evals = 1 << 16;
        static const std::array<size_t, pre_evals> eval_args;
        static const std::array<point, pre_evals> evalp;
    private:
        // The power of four that the fused product works on: all of n = 4^k, half of n = 2 * 4^k.
        size_t fused_part() const {
            return size() >> (std::countr_zero(size()) % 2);
        }
        bool fused_leaves() const {
            return fused_part() >= 16 * dot_tile;
        }
        // Tile two radix-four stages together before descending into each child.
        template<bool inverse, size_t fixed = 0, size_t range_fixed=0, int top_fixed=-1,bool omit16=false>
        void transform(size_t input_n, bool parity, size_t range_offset=0, size_t range_length=0, int top_only=0) {
            if constexpr(range_fixed)range_length=range_fixed;
            if constexpr(top_fixed>=0)top_only=top_fixed;
            const size_t n=fixed?fixed:input_n;
            if constexpr(!range_fixed){prepare_roots(n/16);
            if constexpr(fixed==(1<<24))prepare_shear_roots();}
            size_t log_n=std::countr_zero(n);
            auto butterfly = [&](size_t offset,size_t length,size_t begin,size_t end) __attribute__((always_inline)) {
                if constexpr(omit16)if(length==16)return;
                size_t step=length/4,log_length=std::countr_zero(length),k=offset>>log_length;
                auto *p0=r.data()+(offset+begin)/flen,*p1=r.data()+(offset+begin+step)/flen,*p2=r.data()+(offset+begin+2*step)/flen,*p3=r.data()+(offset+begin+3*step)/flen;
                auto run=[&]<bool shear>() __attribute__((always_inline)) {
                    vpoint v1,v2,v3;vftype t1{},t2{},t3{};
                    if constexpr(shear && fixed==(1<<24)) {
                        auto const& c=shear_roots[((n>>log_length)-1)/3+k];
                        v1={vz,inverse?vz-real(c[0]):vz+real(c[0])};
                        v2={vz,inverse?vz-real(c[1]):vz+real(c[1])};
                        v3={vz,inverse?vz-real(c[2]):vz+real(c[2])};
                        t1=inverse?vz-imag(c[0]):vz+imag(c[0]);
                        t2=inverse?vz-imag(c[1]):vz+imag(c[1]);
                        t3=inverse?vz-imag(c[2]):vz+imag(c[2]);
                    }else {
                        point e=k<pre_evals?evalp[k]:extra[k-pre_evals];point rt=roots[log_n+5-log_length]*e;
                        v1={vz+real(rt),inverse?vz-imag(rt):vz+imag(rt)};
                        if constexpr(shear)if(k&1){if constexpr(inverse)v1=vi(v1);else v1=-vi(v1);}
                        v2=v1*v1;v3=v1*v2;
                        if constexpr(shear){t1=imag(v1)/(vz+1.0+real(v1));t2=imag(v2)/(vz+1.0+real(v2));t3=imag(v3)/(vz+1.0+real(v3));}
                    }
                    auto rotate=[](vpoint z,vpoint v,vftype t) __attribute__((always_inline)) {
                        if constexpr(!shear)return z*v;
                        else {
                            auto x=vftype(_mm256_fnmadd_pd(__m256d(t),__m256d(imag(z)),__m256d(real(z))));
                            auto y=vftype(_mm256_fmadd_pd(__m256d(imag(v)),__m256d(x),__m256d(imag(z))));
                            return vpoint{vftype(_mm256_fnmadd_pd(__m256d(t),__m256d(y),__m256d(x))),y};
                        }
                    };
                    auto *i0=p0,*i1=p1,*i2=p2,*i3=p3,*o0=p0,*o1=p1,*o2=p2,*o3=p3;
                    if constexpr(shear)if(k&1) {
                        if constexpr(inverse){i0=p3;i1=p2;i2=p0;i3=p1;}
                        else{o0=p3;o1=p2;o2=p0;o3=p1;}
                    }
                    for(size_t j=0;j<(end-begin)/flen;j++) {
                        auto A=i0[j],B=i1[j],C=i2[j],D=i3[j];
                        if constexpr(inverse) {
                            o0[j]=(A+B)+(C+D);
                            o2[j]=rotate((A+B)-(C+D),v2,t2);
                            o1[j]=rotate((A-B)-vi(C-D),v1,t1);
                            o3[j]=rotate((A-B)+vi(C-D),v3,t3);
                        }else{
                            B=rotate(B,v1,t1);C=rotate(C,v2,t2);D=rotate(D,v3,t3);
                            o0[j]=(A+C)+(B+D);o1[j]=(A+C)-(B+D);
                            o2[j]=(A-C)+vi(B-D);o3[j]=(A-C)-vi(B-D);
                        }
                    }
                };
                if(length>=shear_min<fixed>)run.template operator()<true>();else run.template operator()<false>();
            };
            if(top_only){
                size_t offset=range_offset,length=range_length;
                if(top_only==1){butterfly(offset,length,0,length/4);return;}
                if(top_only==3){
                    size_t h=length/64;
                    for(size_t j=0;j<h;j+=512){
                        size_t end=std::min(h,j+512);
                        auto stage0=[&](){for(size_t t=0;t<16;t++)butterfly(offset,length,j+t*h,end+t*h);};
                        auto stage1=[&](){for(size_t q=0;q<4;q++)for(size_t t=0;t<4;t++)butterfly(offset+q*length/4,length/4,j+t*h,end+t*h);};
                        auto stage2=[&](){for(size_t q=0;q<16;q++)butterfly(offset+q*length/16,length/16,j,end);};
                        if constexpr(inverse){stage2();stage1();stage0();}else{stage0();stage1();stage2();}
                    }
                    return;
                }
                size_t step=length/16;
                for(size_t j=0;j<step;j+=256){
                    size_t end=std::min(step,j+256);
                    if constexpr(inverse){
                        for(size_t t=0;t<4;t++)butterfly(offset+t*length/4,length/4,j,end);
                        for(size_t t=0;t<4;t++)butterfly(offset,length,j+t*step,end+t*step);
                    }else{
                        for(size_t t=0;t<4;t++)butterfly(offset,length,j+t*step,end+t*step);
                        for(size_t t=0;t<4;t++)butterfly(offset+t*length/4,length/4,j,end);
                    }
                }
                return;
            }
            auto recurse = [&](auto &&self, size_t offset, size_t length) -> void {
                if(length < 4 * flen) {return;}
                if(length >= (1 << 15)) {
                    size_t step = length / 16;
                    if constexpr(inverse) {
                        for(size_t t = 0; t < 16; t++) {self(self, offset + t*step, step);}
                    }
                    for(size_t j = 0; j < step; j += 256) {
                        size_t end = std::min(step, j+256);
                        if constexpr(inverse) {
                            for(size_t t=0;t<4;t++) {butterfly(offset+t*length/4, length/4, j,end);}
                            for(size_t t=0;t<4;t++) {butterfly(offset,length,j+t*step,end+t*step);}
                        } else {
                            for(size_t t=0;t<4;t++) {butterfly(offset,length,j+t*step,end+t*step);}
                            for(size_t t=0;t<4;t++) {butterfly(offset+t*length/4,length/4,j,end);}
                        }
                    }
                    if constexpr(!inverse) {
                        for(size_t t = 0; t < 16; t++) {self(self, offset + t*step, step);}
                    }
                } else if(length >= (size_t(1) << (6 + parity))) {
                    auto finish=[&]<bool par>() {
                        constexpr size_t chunk=size_t(1)<<(6+par);
                        constexpr size_t bottom=size_t(1)<<(4+par);
                        auto small=[&]<size_t L>(auto&& self,size_t pos) __attribute__((always_inline)) -> void {
                            if constexpr(inverse && L>bottom) {
                                for(size_t q=0;q<4;q++)self.template operator()<L/4>(self,pos+q*(L/4));
                            }
                            butterfly(pos,L,0,L/4);
                            if constexpr(!inverse && L>bottom) {
                                for(size_t q=0;q<4;q++)self.template operator()<L/4>(self,pos+q*(L/4));
                            }
                        };
                        for(size_t leaf=offset;leaf<offset+length;leaf+=chunk){
                            if constexpr(inverse){
                                small.template operator()<chunk>(small,leaf);
                                size_t level=std::min<size_t>(std::countr_one(leaf+chunk-1),std::countr_zero(length));
                                for(size_t lvl=6+2+par;lvl<=level;lvl+=2){size_t len=size_t(1)<<lvl;butterfly(leaf & ~(len-1),len,0,len/4);}
                            }else{
                                size_t level=std::min<size_t>(std::countr_zero(n+leaf),std::countr_zero(length));
                                level-=level%2!=par;
                                for(size_t lvl=level;lvl>=6+2+par;lvl-=2){size_t len=size_t(1)<<lvl;butterfly(leaf & ~(len-1),len,0,len/4);}
                                small.template operator()<chunk>(small,leaf);
                            }
                        }
                    };
                    if(parity)finish.template operator()<true>();else finish.template operator()<false>();
                } else {
                    if constexpr(inverse) {
                        for(size_t leaf = offset + 3 * flen; leaf < offset + length; leaf += 4 * flen) {
                            size_t level = std::min<size_t>(std::countr_one(leaf + 3), std::countr_zero(length));
                            for(size_t lvl = 4 + parity; lvl <= level; lvl += 2) {
                                size_t len = size_t(1) << lvl;
                                butterfly(leaf & ~(len-1), len, 0, len / 4);
                            }
                        }
                    } else {
                        for(size_t leaf = offset; leaf < offset + length; leaf += 4 * flen) {
                            size_t level = std::min<size_t>(std::countr_zero(n + leaf), std::countr_zero(length));
                            level -= level % 2 != parity;
                            for(size_t lvl = level; lvl >= 4; lvl -= 2) {
                                size_t len = size_t(1) << lvl;
                                butterfly(leaf & ~(len-1), len, 0, len / 4);
                            }
                        }
                    }
                }
            };
            // Radix two is performed separately at the leaves.
            recurse(recurse, range_offset, range_length?range_length:n);
        }
        // Shortest butterfly that uses the shear rotation; the fixed 2^24 transform reads its
        // shear roots from a table, so they are worth using down to the last in-block level.
        template<size_t fixed> static constexpr size_t shear_min = fixed == (1 << 24) ? 64 : 256;
        static big_vector<std::array<point,3>> shear_roots;
        static void prepare_shear_roots() {
            constexpr size_t n=1<<24;
            if(!shear_roots.empty())return;
            shear_roots.resize((n/16-1)/3,std::array<point,3>{});
            for(size_t len=n;len>=shear_min<n>;len/=4) {
                size_t count=n/len,base=(count-1)/3;
                point factor=roots[29-std::countr_zero(len)];
                for(size_t k=0;k<count;k++) {
                    point rt=factor*(k<pre_evals?evalp[k]:extra[k-pre_evals]);
                    vpoint v1={vz+real(rt),vz+imag(rt)};
                    if(k&1)v1=-vi(v1);
                    vpoint v2=v1*v1,v3=v1*v2;
                    vftype t1=imag(v1)/(vz+1.0+real(v1)),t2=imag(v2)/(vz+1.0+real(v2)),t3=imag(v3)/(vz+1.0+real(v3));
                    shear_roots[base+k]={point(imag(v1)[0],t1[0]),point(imag(v2)[0],t2[0]),point(imag(v3)[0],t3[0])};
                }
            }
        }
        static big_vector<point> extra;
        // Keep the usual table small; cache additional roots for large transforms.
        static void prepare_roots(size_t n) {
            if(n <= pre_evals + extra.size()) {return;}
            size_t old = extra.size();
            extra.resize(std::bit_ceil(n) - pre_evals);
            static const std::array<point,256> coarse=[](){
                std::array<point,256> out;
                for(size_t i=0;i<256;i++)out[i]=polar<ftype>(1.,std::numbers::pi*double((eval_args[256+i]-1)/2)/512.0);
                return out;
            }();
            for(size_t h=pre_evals+old;h<pre_evals+extra.size();h*=2){
                for(size_t i=h;i<2*h;i+=256){
                    point fine=polar<ftype>(1.,std::numbers::pi/double(4*h)*double(eval_arg(4*i)));
                    for(size_t j=0;j<256;j++)extra[i+j-pre_evals]=coarse[j]*fine;
                }
            }
        }
    };

    big_vector<std::array<point,3>> cvector::shear_roots;
    big_vector<point> cvector::extra;

    const std::array<size_t, cvector::pre_evals> cvector::eval_args = []() {
        std::array<size_t, pre_evals> res = {};
        for(size_t i = 1; i < pre_evals; i++) {
            res[i] = res[i >> 1] | (i & 1) << (std::bit_width(i) - 1);
        }
        return res;
    }();
    const std::array<point, cvector::pre_evals> cvector::evalp = []() {
        std::array<point, pre_evals> res = {};
        res[0] = 1;
        for(size_t n = 1; n < pre_evals; n++) {
            res[n] = polar<ftype>(1., std::numbers::pi * ftype(eval_args[n]) / ftype(4 * std::bit_floor(n)));
        }
        return res;
    }();
}
#pragma GCC pop_options

#line 12 "cp-algo/math/ring.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math::fft {
    size_t com_size(size_t as, size_t bs) {
        if(!as || !bs) {
            return 0;
        }
        return std::max(flen, std::bit_ceil(as + bs - 1) / 2);
    }
    // Whether a product of these sizes exceeds half of its padded length by a tail short enough
    // to be corrected naively, which lets mul_truncate halve its transform.
    constexpr size_t short_tail = 32;
    bool has_short_tail(size_t as, size_t bs) {
        size_t n = com_size(as, bs);
        return as + bs - 1 - n <= short_tail && as <= n && bs <= n;
    }
    // Convolution through an imaginary quadratic ring, for a prime modulus p < 2^31.
    // Let d be the smallest positive integer such that -d is a quadratic residue (d = 1 exactly
    // when p = 1 mod 4) and root^2 = -d. A residue x is written as re + im*sqrt(-d) with
    // re + im*root = x, (re, im) reduced by the lattice of representations of zero under the norm
    // re^2 + d*im^2, so that |re| and sqrt(d)*|im| are of order sqrt(p), and is embedded as the
    // complex number re + i*sqrt(d)*im. The product is then one complex convolution instead of
    // three real ones. For d = 1 (Gaussian integers) it is computed modulo x^n - i and x^n + i
    // (the latter through conjugation) and recombined modulo p, which needs i to lie in the ring;
    // for any other d it is a single transform of the full product length.
    constexpr uint64_t pow_mod(uint64_t x, uint64_t e, uint64_t p) {
        uint64_t res = 1;
        for(x %= p; e; e >>= 1, x = x * x % p) {if(e & 1) {res = res * x % p;}}
        return res;
    }
    // Smallest d > 0 with -d a quadratic residue modulo the odd prime p < 2^31, or 0.
    constexpr uint32_t least_negated_residue(uint64_t p) {
        for(uint32_t d = 1; d < 256 && d < p; d++) {
            if(pow_mod(p - d, (p - 1) / 2, p) == 1) {return d;}
        }
        return 0;
    }
    template<modint_type base>
    struct quadratic {
        static inline bool ready = false, available = false;
        static inline uint32_t d = 0, root = 0, prime = 0;
        // (a, b) and (a2, b2): reduced basis of the pairs (re, im) that represent zero.
        static inline int32_t a = 0, b = 0, a2 = 0, b2 = 0;
        // A fixed modulus settles d at compile time, so only the path in use is instantiated.
        static constexpr bool fixed_mod = requires {typename std::bool_constant<(base::mod(), true)>;};
        static constexpr uint32_t fixed_d = [] {
            if constexpr(fixed_mod) {return base::mod() > 2 && base::mod() % 2 ? least_negated_residue(base::mod()) : 0u;}
            else {return 0u;}
        }();
        static void init() {
            if(ready && prime == uint32_t(base::mod())) {return;}
            ready = true; available = false;
            uint64_t p = base::mod();
            prime = uint32_t(p);
            if(p < 3 || p >= (uint64_t(1) << 31) || !is_prime(p)) {return;}
            d = least_negated_residue(p);
            auto s = d ? cp_algo::math::sqrt(base(-int64_t(d))) : std::nullopt;
            if(!s) {return;}
            root = s->getr();
            // Lagrange reduction of (p, 0), (-root, 1) under the norm x^2 + d*y^2.
            using wide = __int128;
            wide x1 = p, y1 = 0, x2 = -wide(root), y2 = 1;
            auto norm = [&](wide x, wide y) {return x * x + d * y * y;};
            while(true) {
                if(norm(x1, y1) > norm(x2, y2)) {std::swap(x1, x2); std::swap(y1, y2);}
                wide dot = x1 * x2 + d * y1 * y2, len = norm(x1, y1);
                wide m = (2 * dot + (dot >= 0 ? len : -len)) / (2 * len);
                if(m == 0 || norm(x2 - m * x1, y2 - m * y1) >= norm(x2, y2)) {break;}
                x2 -= m * x1; y2 -= m * y1;
            }
            a = int32_t(x1); b = int32_t(y1);
            // In the Gaussian integers i*(a, b) = (-b, a) lies in the lattice as well.
            if(d == 1) {a2 = b; b2 = -a;}
            else {a2 = int32_t(x2); b2 = int32_t(y2);}
            assert(base(a) + base(b) * base(root) == base(0) && base(a2) + base(b2) * base(root) == base(0));
            assert(std::abs(int64_t(a) * b2 - int64_t(a2) * b) == int64_t(p));
            available = true;
        }
        // Points per transform: two branches of half the padded product length for d = 1, one
        // transform of the whole length otherwise. A short tail beyond that is computed naively
        // and taken back out, as in mul_truncate, which halves the transform: over the residues
        // for d = 1, where the branches together work modulo x^(2n) + 1, and in ring coordinates
        // for the single transform, which works modulo x^n - i.
        static size_t length(size_t as, size_t bs) {
            return (d == 1 ? com_size(as, bs) : 2 * com_size(as, bs)) >> has_short_tail(as, bs);
        }
        // Whether this path is used for operands of these sizes. With n = com_size(as, bs) it
        // takes six transforms of n points (three of 2n for d > 1), four for a square, where the
        // split representation takes seven and five, so it is preferred whenever it is exact,
        // unless a transform exceeds 2^24 points, the only length with a kernel tiled for data
        // outside the caches (which in turn does not let an operand wrap around).
        // Empirical rounding budget. Reusing a spectrum for a tiled square correlates its
        // errors, so that optimization uses a stricter budget below and otherwise keeps
        // independent lifts of the two operands.
        static bool usable(size_t as, size_t bs) {
            init();
            if(!available || std::min(as, bs) < size_t(magic)) {return false;}
            size_t need = as + bs - 1, n = length(as, bs);
            if(n > (1 << 24) || (n == (1 << 24) && d == 1 && std::max(as, bs) > n)) {return false;}
            using wide = unsigned __int128;
            return wide(need) * d * base::mod() * base::mod() <= wide(1) << 90;
        }
        // Lattice constants as doubles, hoisted out of the hot loops (the statics are 32-bit
        // integers and could alias the 32-bit output stream).
        struct lattice {
            // (c, e): the basis vector with the larger second coordinate, used to shrink im.
            double a, b, a2, b2, to_q, to_t, c, e, inv_e, root, scale, inv_scale, p;
            lattice(): a(quadratic::a), b(quadratic::b), a2(quadratic::a2), b2(quadratic::b2), p(base::mod()) {
                double det = a * b2 - a2 * b;
                to_q = b2 / det; to_t = -b / det;
                bool first = std::abs(b) >= std::abs(b2);
                c = first ? a : a2; e = first ? b : b2; inv_e = 1.0 / e;
                root = quadratic::root;
                scale = std::sqrt(double(d)); inv_scale = 1.0 / scale;
            }
        };
        // Map a rounded coordinate pair back to the residue re + root*im.
        template<bool unit>
        static u32x4 project(vpoint value, bool negative, lattice const& L) {
            const double p = L.p;
            auto R = round(real(value)), I = negative ? -imag(value) : imag(value);
            if constexpr(unit) {I = round(I);}
            else {I = round(I * L.inv_scale);}
            auto q = round(I * L.inv_e);
            auto U = vftype(_mm256_fnmadd_pd(__m256d(q), _mm256_set1_pd(L.c), __m256d(R)));
            auto V = vftype(_mm256_fnmadd_pd(__m256d(q), _mm256_set1_pd(L.e), __m256d(I)));
            auto h = vftype(_mm256_fmadd_pd(__m256d(V), _mm256_set1_pd(L.root), __m256d(U)));
            q = round(h * (1.0 / p));
            auto out = vftype(_mm256_fnmadd_pd(__m256d(q), _mm256_set1_pd(p), __m256d(h)));
            out = out < 0 ? out + p : out;
            return u32x4(_mm256_cvttpd_epi32(__m256d(out)));
        }
        // Lift residues to ring coordinates with stochastic rounding.
        // The xorshift stream is advanced once per 8 residues and restarted from the same
        // seed for both branches, so both see the same representatives.
        // With stream set the spectrum is written with non-temporal stores and left for
        // cache_product to transform: for n = 2^24 it is far larger than the caches and is next
        // read by a separate pass, so this saves the read-for-ownership of every destination line.
        // Coefficients from n on (d = 1 only) wrap around: x^n = i in the branch modulo x^n - i,
        // and the conjugated branch modulo x^n + i sees conj(-i * z) = i * conj(z).
        template<bool stream, bool unit>
        static void fill(cvector& c, auto const& x, auto const& upper, size_t n, bool negative, u64x4 state, bool transform = true) {
            const lattice L;
            auto const* src = reinterpret_cast<const uint32_t*>(std::data(x));
            size_t count = std::size(x);
            assert(count <= n && (std::empty(upper) || (unit && !stream && count == n)));
            c.r.resize(n / flen);
            auto* dst = reinterpret_cast<double*>(c.r.data());
            u32x8 words{};
            auto emit = [&]<bool wrap = false>(size_t i, i32x4 bits) __attribute__((always_inline)) {
                auto v = __builtin_convertvector(bits, vftype);
                if(i % 8 == 0) {state ^= state << 13; state ^= state >> 7; state ^= state << 17; words = u32x8(state);}
                i32x4 small = i % 8 ? i32x4(__builtin_shufflevector(words, words, 1, 3, 5, 7)) : i32x4(__builtin_shufflevector(words, words, 0, 2, 4, 6));
                auto noise = __builtin_convertvector(small, vftype) * 0x1p-32;
                auto q = round(v * L.to_q + noise), t = round(v * L.to_t + noise);
                auto re = v - q * L.a - t * L.a2, im = -(q * L.b) - t * L.b2;
                if constexpr(!unit) {im = im * L.scale;}
                if constexpr(stream) {
                    _mm256_stream_pd(dst + 2 * i, __m256d(re));
                    _mm256_stream_pd(dst + 2 * i + flen, __m256d(negative ? -im : im));
                } else if constexpr(wrap) {
                    c.r[(i - n) / flen] += vpoint{negative ? im : -im, re};
                } else {
                    c.r[i / flen] = vpoint{re, negative ? -im : im};
                }
            };
            size_t full = count / flen * flen;
            for(size_t i = 0; i < full; i += flen) {
                i32x4 bits;
                std::memcpy(&bits, src + i, sizeof(bits));
                emit(i, bits);
            }
            if(full < count) {
                i32x4 bits{};
                for(size_t j = full; j < count; j++) {bits[j - full] = int32_t(src[j]);}
                emit(full, bits);
            }
            if constexpr(stream) {_mm_sfence();}
            std::fill(c.r.begin() + (count + flen - 1) / flen, c.r.end(), vpoint{});
            auto const* wrapped = reinterpret_cast<const uint32_t*>(std::data(upper));
            for(size_t i = 0; i < std::size(upper); i += flen) {
                i32x4 bits{};
                for(size_t j = i; j < std::min(i + flen, std::size(upper)); j++) {bits[j - i] = int32_t(wrapped[j]);}
                emit.template operator()<true>(n + i, bits);
            }
            checkpoint("quadratic init");
            if constexpr(!stream) {if(transform) {c.forward();}}
        }
        // Lift a range of residues to ring coordinates, one point per coefficient, for the
        // reusable transforms below. Unlike fill it reads through the modint interface, so it
        // serves views and Montgomery storage alike, and it neither wraps nor streams.
        template<bool unit>
        static void lift(cvector& c, auto const& x, size_t n, bool negative, u64x4 state) {
            const lattice L;
            size_t total = std::size(x), count = std::min(n, total);
            assert(total <= 2 * n && (d == 1 || total <= n));
            c.r.resize(n / flen);
            u32x8 words{};
            size_t step = 0;
            auto emit = [&]<bool wrap>(size_t i, i32x4 bits) __attribute__((always_inline)) {
                auto v = __builtin_convertvector(bits, vftype);
                if(step % 2 == 0) {state ^= state << 13; state ^= state >> 7; state ^= state << 17; words = u32x8(state);}
                i32x4 small = step++ % 2 ? i32x4(__builtin_shufflevector(words, words, 1, 3, 5, 7))
                                         : i32x4(__builtin_shufflevector(words, words, 0, 2, 4, 6));
                auto noise = __builtin_convertvector(small, vftype) * 0x1p-32;
                auto q = round(v * L.to_q + noise), t = round(v * L.to_t + noise);
                auto re = v - q * L.a - t * L.a2, im = -(q * L.b) - t * L.b2;
                if constexpr(!unit) {im = im * L.scale;}
                // Coefficients from n on wrap around: x^n is i in this branch, -i in the other.
                if constexpr(wrap) {c.r[(i - n) / flen] += vpoint{negative ? im : -im, re};}
                else {c.r[i / flen] = vpoint{re, negative ? -im : im};}
            };
            // Residues are raw words exactly when the modulus is a compile-time constant; wider
            // storage than the residue needs is narrowed on the way in.
            constexpr bool plain = fixed_mod && std::ranges::contiguous_range<std::decay_t<decltype(x)>>;
            constexpr bool raw = plain && sizeof(base) == 4;
            constexpr bool wide = plain && sizeof(base) == 8;
            auto load = [&](size_t i, size_t upto) {
                i32x4 bits{};
                if constexpr(raw || wide) {
                    if(i + flen <= upto) {
                        if constexpr(raw) {
                            std::memcpy(&bits, reinterpret_cast<const uint32_t*>(std::data(x)) + i, sizeof(bits));
                        } else {
                            u64x4 words;
                            std::memcpy(&words, reinterpret_cast<const uint64_t*>(std::data(x)) + i, sizeof(words));
                            bits = __builtin_convertvector(words, i32x4);
                        }
                        return bits;
                    }
                }
                for(size_t j = i; j < std::min(i + flen, upto); j++) {bits[j - i] = int32_t(x[j].getr());}
                return bits;
            };
            for(size_t i = 0; i < count; i += flen) {emit.template operator()<false>(i, load(i, count));}
            std::fill(c.r.begin() + (count + flen - 1) / flen, c.r.end(), vpoint{});
            for(size_t i = n; i < total; i += flen) {emit.template operator()<true>(i, load(i, total));}
            checkpoint("quadratic init");
        }
        // Read a product out of its transformed branches, applying the inverse-transform scale.
        // For d = 1 the branches hold the product modulo x^n - i and modulo x^n + i, so the low
        // half of the result is their half-sum and the high half their half-difference over i.
        // The half of the result that the branches are recombined into is parked in the output
        // itself, which is always long enough: the high half is only wanted where the low half
        // has already been read back.
        static void recover(std::array<cvector, 2> const& parts, size_t n, double factor, auto& out, size_t k) {
            // Residues are raw 32-bit words exactly when the modulus is a compile-time constant,
            // which is what lets the recombination stay vectorized.
            constexpr bool plain = fixed_mod && std::ranges::contiguous_range<std::decay_t<decltype(out)>>;
            constexpr bool raw = plain && sizeof(base) == 4;
            constexpr bool wide = plain && sizeof(base) == 8;
            const lattice L;
            auto scale = vz + factor;
            size_t low = std::min(k, n);
            auto project_at = [&](cvector const& part, size_t i, bool negative) {
                if(d == 1) {return project<true>(part.at(i) * scale, negative, L);}
                else {return project<false>(part.at(i) * scale, negative, L);}
            };
            // Eight coefficients at a time, the width the recombination works in.
            auto project8 = [&](cvector const& part, size_t i, bool negative, size_t count) {
                auto lo8 = project_at(part, i, negative);
                auto hi8 = count > flen ? project_at(part, i + flen, negative) : u32x4{};
                return __builtin_shufflevector(lo8, hi8, 0, 1, 2, 3, 4, 5, 6, 7);
            };
            auto store8 = [&](size_t idx, u32x8 v, size_t count) {
                if constexpr(raw) {
                    if(count == 8) {std::memcpy(reinterpret_cast<uint32_t*>(std::data(out)) + idx, &v, sizeof(v)); return;}
                } else if constexpr(wide) {
                    if(count == 8) {
                        auto words = reinterpret_cast<uint64_t*>(std::data(out)) + idx;
                        auto lo4 = __builtin_convertvector(u32x4{v[0], v[1], v[2], v[3]}, u64x4);
                        auto hi4 = __builtin_convertvector(u32x4{v[4], v[5], v[6], v[7]}, u64x4);
                        std::memcpy(words, &lo4, sizeof(lo4));
                        std::memcpy(words + flen, &hi4, sizeof(hi4));
                        return;
                    }
                }
                for(size_t l = 0; l < count; l++) {out[idx + l].setr(typename base::UInt(v[l]));}
            };
            auto load8 = [&](size_t idx, size_t count) {
                u32x8 v{};
                if constexpr(raw) {
                    if(count == 8) {std::memcpy(&v, reinterpret_cast<uint32_t const*>(std::data(out)) + idx, sizeof(v)); return v;}
                } else if constexpr(wide) {
                    if(count == 8) {
                        auto words = reinterpret_cast<uint64_t const*>(std::data(out)) + idx;
                        u64x4 lo4, hi4;
                        std::memcpy(&lo4, words, sizeof(lo4));
                        std::memcpy(&hi4, words + flen, sizeof(hi4));
                        auto lo = __builtin_convertvector(lo4, u32x4), hi = __builtin_convertvector(hi4, u32x4);
                        return u32x8(__builtin_shufflevector(lo, hi, 0, 1, 2, 3, 4, 5, 6, 7));
                    }
                }
                for(size_t l = 0; l < count; l++) {v[l] = uint32_t(out[idx + l].getr());}
                return v;
            };
            for(size_t i = 0; i < low; i += 8) {
                store8(i, project8(parts[0], i, false, std::min<size_t>(8, low - i)), std::min<size_t>(8, low - i));
            }
            if(d > 1) {checkpoint("quadratic recover"); return;}
            const uint32_t mod32 = uint32_t(base::mod()), imod32 = -inv2<uint32_t>(base::mod());
            auto highmul = u32x8{} + uint32_t(((base(2) * base(root)).inv() * bpow(base(2), 32)).getr());
            for(size_t i = 0; i < low; i += 8) {
                size_t count = std::min<size_t>(8, low - i);
                auto minus = project8(parts[1], i, true, count);
                auto plus = load8(i, count);
                auto lo8 = reduce_once(plus + minus, mod32);
                lo8 = (lo8 + (lo8 & 1) * mod32) >> 1;
                auto hi8 = reduce_once(montgomery_mul(plus + mod32 - minus, highmul, mod32, imod32), mod32);
                store8(i, lo8, count);
                if(n + i < k) {store8(n + i, hi8, std::min<size_t>(count, k - n - i));}
            }
            checkpoint("quadratic recover");
        }
        // Cyclic product modulo x^k - 1, in place over a, for operands of exactly k coefficients.
        // The transform evaluates at the k-th roots of i, so twisting coefficient j by w^j with
        // w^k = -i turns the product it computes, modulo x^k - i, into the cyclic one; the
        // inverse twist is folded into the readback. A fold of the linear product would need
        // twice the transform.
        static void cyclic(auto& a, auto const& b, size_t k) {
            init();
            assert(available && std::popcount(k) == 1 && std::size(a) == k && std::size(b) == k);
            bool square = (void const*)std::data(a) == (void const*)std::data(b);
            // w^j from a table of every fourth power, built the way the root tables of the
            // transform are, and four consecutive powers per vector by one broadcast multiply.
            size_t groups = k / flen;
            size_t fine_bits = std::min<size_t>(8, std::countr_zero(std::max<size_t>(groups, 1)));
            size_t fine = size_t(1) << fine_bits;
            big_vector<point> low(fine), high((groups + fine - 1) >> fine_bits), step(groups);
            auto w = [&](size_t j) {return polar<ftype>(1., -std::numbers::pi * ftype(j) / ftype(2 * k));};
            for(size_t t = 0; t < low.size(); t++) {low[t] = w(flen * t);}
            for(size_t c = 0; c < high.size(); c++) {high[c] = w(flen * (c << fine_bits));}
            for(size_t c = 0; c < groups; c++) {step[c] = high[c >> fine_bits] * low[c & (fine - 1)];}
            vpoint quarter, quarter_conj;
            for(size_t l = 0; l < flen; l++) {
                point v = w(l);
                real(quarter)[l] = real(v); imag(quarter)[l] = imag(v);
                real(quarter_conj)[l] = real(v); imag(quarter_conj)[l] = -imag(v);
            }
            auto twiddle = [&](size_t j, bool inverse, ftype scale) {
                point t = step[j / flen];
                vpoint head = {vz + real(t) * scale, vz + (inverse ? -imag(t) : imag(t)) * scale};
                return head * (inverse ? quarter_conj : quarter);
            };
            auto run = [&]<bool unit>() {
                const lattice L;
                cvector A(k), B(0);
                lift<unit>(A, a, k, false, seed());
                if(!square) {lift<unit>(B, b, k, false, seed());}
                for(size_t j = 0; j < k; j += flen) {
                    auto w = twiddle(j, false, 1);
                    A.at(j) *= w;
                    if(!square) {B.at(j) *= w;}
                }
                A.forward();
                if(!square) {B.forward();}
                A.multiply(square ? A : B);
                for(size_t j = 0; j < k; j += flen) {
                    auto v = project<unit>(A.at(j) * twiddle(j, true, ftype(flen) / ftype(k)), false, L);
                    for(size_t l = 0; l < flen; l++) {a[j + l].setr(typename base::UInt(v[l]));}
                }
            };
            if(d == 1) {run.template operator()<true>();} else {run.template operator()<false>();}
            checkpoint("quadratic recover");
        }
        static u64x4 seed() {
            return u64x4{random::rng() | 1, random::rng() | 1, random::rng() | 1, random::rng() | 1};
        }
        // a <- a * b for d = 1, or a <- a * a with square set (b is not read then).
        // The first branch is stored in the upper half of a, so the part of a that wraps around
        // is set aside first.
        static void mul_branches(auto& a, auto const& b, bool square) {
            size_t as = std::size(a), bs = square ? as : std::size(b), need = as + bs - 1;
            size_t n = length(as, bs);
            assert(available && d == 1);
            // Coefficients from 2n on come back negated at the bottom; there are few of them.
            std::array<uint32_t, short_tail> high{};
            size_t tail = need > 2 * n ? need - 2 * n : 0;
            for(size_t i = 0; i < tail; i++) {
                auto const* x = reinterpret_cast<const uint32_t*>(std::data(a));
                auto const* y = square ? x : reinterpret_cast<const uint32_t*>(std::data(b));
                uint64_t sum = 0;
                for(size_t j = 2 * n + i - bs + 1; j < as; j++) {
                    sum = (sum + uint64_t(x[j]) * y[2 * n + i - j]) % prime;
                }
                high[i] = uint32_t(sum);
            }
            // Montgomery constants for the recombination of the two branches.
            const uint32_t mod32 = uint32_t(base::mod()), imod32 = -inv2<uint32_t>(base::mod());
            base r32 = bpow(base(2), 32);
            auto highmul = u32x8{} + uint32_t(((base(2) * base(root)).inv() * r32).getr());
            u64x4 seed_a = seed(), seed_b = seed();
            const lattice L;
            big_vector<base> a_upper(std::begin(a) + std::min(as, n), std::end(a));
            a.resize(2 * n);
            std::span<base const> a_lower = std::span(a).first(std::min(as, n));
            // A square may come with b aliasing the storage that a has just left.
            auto b_lower = square ? a_lower : std::span<base const>(b).first(std::min(bs, n));
            auto b_upper = square ? std::span<base const>(a_upper) : std::span<base const>(b).subspan(b_lower.size());
            cvector A(0), B(0);
            auto* out = reinterpret_cast<uint32_t*>(std::data(a));
            for(bool negative: {false, true}) {
                if(n == (1 << 24)) {
                    if constexpr(cvector::fuse_forward) {
                        cvector::fuse_args fa{out, as, seed_a[0], L.a, L.b, L.a / double(base::mod()), L.b / double(base::mod())};
                        cvector::fuse_args fb{reinterpret_cast<const uint32_t*>(std::data(b)), bs, seed_b[0], fa.a, fa.b, fa.a_over_p, fa.b_over_p};
                        if(negative) {A.template cache_product<true>(B, fa, fb);}
                        else {A.template cache_product<false>(B, fa, fb);}
                    } else {
                        fill<true, true>(A, a_lower, a_upper, n, negative, seed_a);
                        if(square && __uint128_t(need) * d * prime * prime <= (__uint128_t(1) << 88)) {
                            if(negative) {A.template cache_product<true, false, true>(A);}
                            else {A.template cache_product<false, false, true>(A);}
                        } else {
                            fill<true, true>(B, b_lower, b_upper, n, negative, seed_b);
                            if(negative) {A.template cache_product<true>(B);}
                            else {A.template cache_product<false>(B);}
                        }
                    }
                } else {
                    fill<false, true>(A, a_lower, a_upper, n, negative, seed_a);
                    if(!square) {fill<false, true>(B, b_lower, b_upper, n, negative, seed_b);}
                    A.multiply(square ? A : B);
                }
                auto scale = vz + double(flen) / double(n);
                for(size_t i = 0; i < n; i += 8) {
                    auto sum0 = project<true>(A.at(i) * scale, negative, L);
                    auto sum1 = project<true>(A.at(i + 4) * scale, negative, L);
                    auto sum = __builtin_shufflevector(sum0, sum1, 0, 1, 2, 3, 4, 5, 6, 7);
                    if(negative) {
                        u32x8 plus;
                        std::memcpy(&plus, out + n + i, sizeof(plus));
                        auto lo = plus + sum;
                        // Unsigned reduction: these sums pass 2^31 for moduli above 2^30.
                        lo = reduce_once(lo, mod32);
                        lo = (lo + (lo & 1) * base::mod()) >> 1;
                        auto hi = reduce_once(montgomery_mul(plus + base::mod() - sum, highmul, mod32, imod32), mod32);
                        std::memcpy(out + i, &lo, sizeof(lo));
                        std::memcpy(out + n + i, &hi, sizeof(hi));
                    } else {
                        std::memcpy(out + n + i, &sum, sizeof(sum));
                    }
                }
                checkpoint("quadratic recover");
            }
            a.resize(need);
            out = reinterpret_cast<uint32_t*>(std::data(a));
            for(size_t i = 0; i < tail; i++) {
                uint32_t sum = out[i] + high[i];
                out[i] = std::min(sum, sum - prime);
                out[2 * n + i] = high[i];
            }
        }
        // a <- a * b for d > 1, or a <- a * a with square set: i is not in the ring, so two
        // branches could only be recombined as complex numbers, which is slower and less exact
        // than one full transform.
        static void mul_single(auto& a, auto const& b, bool square) {
            size_t as = std::size(a), bs = square ? as : std::size(b), need = as + bs - 1;
            size_t n = length(as, bs), tail = need > n ? need - n : 0;
            assert(available && d > 1);
            const lattice L;
            cvector A(0), B(0);
            std::span<base const> none;
            // Coefficients from n on, in ring coordinates: they come back multiplied by i.
            std::array<point, short_tail> high{};
            auto wrapped = [&](cvector const& rhs) {
                for(size_t i = 0; i < tail; i++) {
                    for(size_t j = n + i - bs + 1; j < as; j++) {
                        high[i] += A.template get<point>(j) * rhs.template get<point>(n + i - j);
                    }
                }
            };
            if(n == (1 << 24)) {
                fill<true, false>(A, a, none, n, false, seed());
                if(square && __uint128_t(need) * d * prime * prime <= (__uint128_t(1) << 88)) {
                    wrapped(A);
                    A.template cache_product<false, false, true>(A);
                } else {
                    if(square) {fill<true, false>(B, a, none, n, false, seed());}
                    else {fill<true, false>(B, b, none, n, false, seed());}
                    wrapped(B);
                    A.template cache_product<false>(B);
                }
            } else {
                fill<false, false>(A, a, none, n, false, seed(), !tail);
                if(!square) {fill<false, false>(B, b, none, n, false, seed(), !tail);}
                if(tail) {
                    wrapped(square ? A : B);
                    A.forward();
                    if(!square) {B.forward();}
                }
                A.multiply(square ? A : B);
            }
            for(size_t i = 0; i < tail; i++) {
                A.set(i, A.template get<point>(i) - point(0, 1) * high[i] * (double(n) / double(flen)));
            }
            a.resize(n);
            auto* out = reinterpret_cast<uint32_t*>(std::data(a));
            auto scale = vz + double(flen) / double(n);
            for(size_t i = 0; i < n; i += flen) {
                auto sum = project<false>(A.at(i) * scale, false, L);
                std::memcpy(out + i, &sum, sizeof(sum));
            }
            checkpoint("quadratic recover");
            a.resize(need);
            out = reinterpret_cast<uint32_t*>(std::data(a));
            for(size_t i = 0; i < tail; i += flen) {
                vpoint lanes = {vz, vz};
                for(size_t j = i; j < std::min(tail, i + flen); j++) {
                    real(lanes)[j - i] = real(high[j]);
                    imag(lanes)[j - i] = imag(high[j]);
                }
                auto sum = project<false>(lanes, false, L);
                for(size_t j = i; j < std::min(tail, i + flen); j++) {out[n + j] = sum[j - i];}
            }
        }
        // Both routines read and write the storage as plain residues, which it is for modint<m>.
        // A runtime-modulus type keeps another form, so its operands are converted on the way.
        static void mul(auto& a, auto const& b, bool square) {
            static_assert(sizeof(std::decay_t<decltype(a[0])>) == 4);
            auto run = [&](auto const& rhs) {
                if constexpr(fixed_d == 1) {mul_branches(a, rhs, square);}
                else if constexpr(fixed_d > 1) {mul_single(a, rhs, square);}
                else if(d == 1) {mul_branches(a, rhs, square);}
                else {mul_single(a, rhs, square);}
            };
            if constexpr(fixed_mod) {run(b);}
            else {
                big_vector<base> plain;
                if(!square) {plain.assign(std::begin(b), std::end(b));}
                for(auto& x: plain) {x.setr_direct(x.getr());}
                for(auto& x: a) {x.setr_direct(x.getr());}
                run(plain);
                for(auto& x: a) {x.setr(x.getr_direct());}
            }
        }
    };

    // A polynomial transformed for repeated multiplication in the ring of quadratic<base>.
    //
    // capacity() is the number of product coefficients the transform represents, and both
    // operands of a product must fit in it. For d = 1 the polynomial is held as two conjugate
    // branches of capacity/2 points, modulo x^n - i and modulo x^n + i, whose moduli multiply
    // to x^(2n) + 1; for larger d as one transform of capacity points modulo x^n - i. Either
    // way a product of at most capacity coefficients does not wrap around.
    //
    // complete selects the layout. The default stops one radix level short, which the fused
    // product of multiply() finishes; the complete transform makes a product pointwise, which
    // is what an accumulated sum of products needs.
    //
    // A transform costs two doubles per coefficient, so the type is move-only and every copy is
    // spelled clone(). multiply() and square() consume the object and only read the other
    // operand, which is what lets one transform serve several products.
    //
    // A transform belongs to the modulus it was built under; a runtime modulus must not change
    // while one is alive.
    template<modint_type base, bool complete = false>
    struct spectrum {
        using ring = quadratic<base>;
        spectrum(auto const& a, size_t capacity): cap(std::max(2 * flen, std::bit_ceil(capacity))) {
            ring::init();
            assert(ring::available && std::size(a) <= cap);
            auto state = ring::seed();
            for(size_t j = 0; j < branches(); j++) {
                if(ring::d == 1) {ring::template lift<true>(parts[j], a, points(), j == 1, state);}
                else {ring::template lift<false>(parts[j], a, points(), false, state);}
                if constexpr(complete) {parts[j].template fft<false>();}
                else {parts[j].forward();}
            }
        }
        spectrum(spectrum&&) = default;
        spectrum& operator=(spectrum&&) = default;
        spectrum clone() const {return *this;}
        size_t capacity() const {return cap;}

        // out[0..k) = *this * other. Consumes *this and only reads other.
        void multiply(spectrum const& other, auto& out, size_t k) && requires(!complete) {
            assert(other.cap == cap && k <= cap);
            for(size_t j = 0; j < branches(); j++) {parts[j].multiply(other.parts[j]);}
            ring::recover(parts, points(), double(flen) / double(points()), out, k);
        }
        // out[0..k) = *this * *this, consuming *this.
        void square(auto& out, size_t k) && requires(!complete) {
            std::move(*this).multiply(*this, out, k);
        }
    private:
        spectrum(spectrum const&) = default;
        size_t branches() const {return ring::d == 1 ? 2 : 1;}
        size_t points() const {return ring::d == 1 ? cap / 2 : cap;}
        template<modint_type> friend struct product;
        std::array<cvector, 2> parts = {cvector(0), cvector(0)};
        size_t cap;
    };

    // A sum of products of transforms, read back once. Every term costs one pointwise pass and
    // the sum one inverse transform, which is what makes it worth keeping the operands around.
    template<modint_type base>
    struct product {
        using ring = quadratic<base>;
        using operand = spectrum<base, true>;
        explicit product(size_t capacity): cap(std::max(2 * flen, std::bit_ceil(capacity))) {
            ring::init();
            assert(ring::available);
            for(size_t j = 0; j < branches(); j++) {acc[j] = cvector(points());}
        }
        // *this += x * y.
        void add(operand const& x, operand const& y) {
            assert(x.cap == cap && y.cap == cap);
            for(size_t j = 0; j < branches(); j++) {
                for(size_t i = 0; i < points(); i += flen) {
                    acc[j].at(i) += x.parts[j].at(i) * y.parts[j].at(i);
                }
            }
            checkpoint("dot");
        }
        // out[0..k) = the accumulated sum, consuming *this.
        void recover(auto& out, size_t k) && {
            assert(k <= cap);
            for(size_t j = 0; j < branches(); j++) {acc[j].template ifft<false>();}
            ring::recover(acc, points(), 1.0, out, k);
        }
    private:
        size_t branches() const {return ring::d == 1 ? 2 : 1;}
        size_t points() const {return ring::d == 1 ? cap / 2 : cap;}
        std::array<cvector, 2> acc = {cvector(0), cvector(0)};
        size_t cap;
    };
}
#pragma GCC pop_options

#line 6 "cp-algo/math/fft.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math::fft {
    void mul_slow(auto &a, auto const& b, size_t k) {
        using base = std::decay_t<decltype(a[0])>;
        if(!std::empty(a) && std::data(a) == std::data(b)) {
            size_t n = std::min(k, std::size(a)), m = std::min(k, std::size(b));
            if(!m) {a.clear(); return;}
            a.resize(k);
            // Descending output only reads original coefficients at indices <=j.
            for(size_t j = k; j-- > 0;) {
                base sum = 0;
                size_t lo = j >= n ? j + 1 - n : 0, hi = std::min(j + 1, m);
                for(size_t i = lo; i < hi; i++) {
                    if(n == m && i > j - i) {break;}
                    auto term = a[i] * a[j - i];
                    sum += n == m && i != j - i ? term + term : term;
                }
                a[j] = sum;
            }
            return;
        }
        if(std::empty(a) || std::empty(b)) {
            a.clear();
        } else {
            size_t n = std::min(k, std::size(a));
            size_t m = std::min(k, std::size(b));
            size_t had = std::size(a);
            a.resize(k);
            // The loop below reads every coefficient it writes, so the growth is zeroed here
            // rather than relying on the caller's allocator to do it.
            if(k > had) {std::fill(std::begin(a) + had, std::end(a), base(0));}
            for(int j = int(k - 1); j >= 0; j--) {
                a[j] *= b[0];
                for(int i = std::max(j - (int)n, 0) + 1; i < std::min(j + 1, (int)m); i++) {
                    a[j] += a[j - i] * b[i];
                }
            }
        }
    }
    // Whether two ranges are backed by the same storage, which makes a product a square.
    bool same_storage(auto const& a, auto const& b) {
        if constexpr(std::ranges::contiguous_range<decltype(b)>) {
            return !std::empty(a) && (void const*)std::data(a) == (void const*)std::data(b);
        } else {
            return false;
        }
    }
    // The product of the truncated operands, keeping k coefficients. Short operands are cheaper
    // to multiply naively; every other product goes through the ring.
    void mul_truncate(auto &a, auto const& b, size_t k) {
        using base = std::decay_t<decltype(a[0])>;
        if(std::min({k, std::size(a), std::size(b)}) < magic) {
            mul_slow(a, b, k);
            return;
        }
        size_t as = std::min(k, std::size(a)), bs = std::min(k, std::size(b));
        assert(quadratic<base>::usable(as, bs) && "the ring needs an odd prime modulus below 2^31 and a product within its rounding bound");
        bool aliased = same_storage(a, b), square = aliased && as == bs;
        // Everything past the true length of the product is zero, written here rather than
        // left to the caller's allocator.
        size_t need = as + bs - 1, keep = std::min(k, need);
        auto pad = [&] {if(k > need) {std::fill(std::begin(a) + need, std::end(a), base(0));}};
        if constexpr(sizeof(base) == 4 && std::ranges::contiguous_range<decltype(b)>) {
            if(aliased && !square) {
                big_vector<base> copy(std::data(b), std::data(b) + bs);
                a.resize(as);
                quadratic<base>::mul(a, copy, false);
            } else {
                auto prefix = std::span<base const>(std::data(b), bs);
                a.resize(as);
                quadratic<base>::mul(a, prefix, square);
            }
            a.resize(k);
            pad();
        } else {
            // Wider storage or a non-contiguous operand: the reusable transform reads and writes
            // through the modint interface instead of the raw residues.
            size_t cap = std::bit_ceil(need);
            auto A = spectrum<base>(a | std::views::take(as), cap);
            a.resize(k);
            if(square) {std::move(A).square(a, keep);}
            else {std::move(A).multiply(spectrum<base>(b | std::views::take(bs), cap), a, keep);}
            pad();
        }
    }
    // Cyclic product modulo x^k - 1, in place over a.
    void cyclic_mul(auto &a, auto const& b, size_t k) {
        using base = std::decay_t<decltype(a[0])>;
        quadratic<base>::cyclic(a, b, k);
    }
    namespace impl {
        // Overlap-add for a short fixed operand; every block reuses its transform.
        void mul_unbalanced(auto &a, auto const& b) {
            using base = std::decay_t<decltype(a[0])>;
            auto x = std::span<base const>(a), y = std::span<base const>(b);
            if(x.size() < y.size()) {std::swap(x, y);}
            constexpr size_t length = 1 << 15;
            size_t step = length - y.size() + 1;
            auto fixed = spectrum<base>(y, length);
            std::decay_t<decltype(a)> result;
            // Accumulated into, so the zeros are written rather than assumed.
            result.assign(x.size() + y.size() - 1, base(0));
            big_vector<base> work(length);
            for(size_t start = 0; start < x.size(); start += step) {
                size_t count = std::min(step, x.size() - start);
                size_t need = count + y.size() - 1;
                spectrum<base>(x.subspan(start, count), length).multiply(fixed, work, need);
                for(size_t i = 0; i < need; i++) {result[start + i] += work[i];}
            }
            a = std::move(result);
        }
    }
    void mul(auto &a, auto const& b) {
        if(std::empty(a) || std::empty(b)) {a.clear(); return;}
        size_t small = std::min(std::size(a), std::size(b));
        size_t large = std::max(std::size(a), std::size(b));
        if(small >= magic && small <= 4096 && large >= (1 << 20) && large / small >= 64) {
            return impl::mul_unbalanced(a, b);
        }
        mul_truncate(a, b, std::size(a) + std::size(b) - 1);
    }
}
#pragma GCC pop_options

#line 7 "cp-algo/math/poly/base.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math {
    template<typename T> struct poly_t;
    template<typename T>
    std::array<poly_t<T>, 2> divmod(poly_t<T> p, poly_t<T> const& q);
    template<typename T>
    struct poly_t {
        using Vector = big_vector<T>;
        using base = T;
        Vector a;

        poly_t& normalize() {
            while(deg() >= 0 && lead() == base(0)) {
                a.pop_back();
            }
            return *this;
        }

        poly_t() = default;
        poly_t(T a0): a{a0} {normalize();}
        poly_t(Vector const& t): a(t) {normalize();}
        poly_t(Vector &&t): a(std::move(t)) {normalize();}

        poly_t& negate_inplace() {
            std::ranges::transform(a, begin(a), std::negate{});
            return *this;
        }
        friend poly_t operator -(poly_t p) {p.negate_inplace(); return p;}
        poly_t& operator += (poly_t const& t) {
            a.resize(std::max(size(a), size(t.a)));
            std::ranges::transform(a, t.a, begin(a), std::plus{});
            return normalize();
        }
        poly_t& operator -= (poly_t const& t) {
            a.resize(std::max(size(a), size(t.a)));
            std::ranges::transform(a, t.a, begin(a), std::minus{});
            return normalize();
        }
        friend poly_t operator + (poly_t p, poly_t const& t) {p += t; return p;}
        friend poly_t operator - (poly_t p, poly_t const& t) {p -= t; return p;}

        poly_t& mod_xk_inplace(size_t k) {
            a.resize(std::min(size(a), k));
            return normalize();
        }
        poly_t& mul_xk_inplace(size_t k) {
            if(is_zero()) {return *this;}
            a.insert(begin(a), k, T(0));
            return normalize();
        }
        poly_t& div_xk_inplace(int64_t k) {
            if(k < 0) {
                return mul_xk_inplace(-k);
            }
            a.erase(begin(a), begin(a) + std::min<size_t>(k, size(a)));
            return normalize();
        }
        poly_t &substr_inplace(size_t l, size_t k) {
            return mod_xk_inplace(l + k).div_xk_inplace(l);
        }
        poly_t mod_xk(size_t k) const & {return substr(0, k);}
        poly_t mod_xk(size_t k) && {mod_xk_inplace(k); return std::move(*this);}
        poly_t mul_xk(size_t k) const & {auto p = *this; p.mul_xk_inplace(k); return p;}
        poly_t mul_xk(size_t k) && {mul_xk_inplace(k); return std::move(*this);}
        poly_t div_xk(int64_t k) const & {return k < 0 ? mul_xk(-k) : substr(k, a.size());}
        poly_t div_xk(int64_t k) && {div_xk_inplace(k); return std::move(*this);}
        poly_t substr(size_t l, size_t k) const & {
            l = std::min(l, a.size());
            k = std::min(k, a.size() - l);
            return Vector(begin(a) + l, begin(a) + l + k);
        }
        poly_t substr(size_t l, size_t k) && {substr_inplace(l, k); return std::move(*this);}

        poly_t& operator *= (const poly_t &t) {fft::mul(a, t.a); normalize(); return *this;}
        friend poly_t operator * (poly_t p, const poly_t &t) {p *= t; return p;}

        poly_t& operator /= (const poly_t &t) {
            assert(!t.is_zero());
            if(this == &t) {return *this = T(1);}
            auto [q, r] = divmod(std::move(*this), t);
            return *this = std::move(q);
        }
        poly_t& operator %= (const poly_t &t) {
            assert(!t.is_zero());
            if(this == &t) {a.clear(); return *this;}
            auto [q, r] = divmod(std::move(*this), t);
            return *this = std::move(r);
        }
        friend poly_t operator / (poly_t p, poly_t const& t) {p /= t; return p;}
        friend poly_t operator % (poly_t p, poly_t const& t) {p %= t; return p;}

        poly_t& operator *= (T const& x) {
            for(auto &it: a) {
                it *= x;
            }
            return normalize();
        }
        poly_t& operator /= (T const& x) {return *this *= x.inv();}
        friend poly_t operator * (poly_t p, T const& x) {p *= x; return p;}
        friend poly_t operator / (poly_t p, T const& x) {p /= x; return p;}

        poly_t& reverse(size_t n) {
            a.resize(n);
            std::ranges::reverse(a);
            return normalize();
        }
        poly_t& reverse() {return reverse(size(a));}
        poly_t reversed(size_t n) const & {auto p = *this; p.reverse(n); return p;}
        poly_t reversed(size_t n) && {reverse(n); return std::move(*this);}
        poly_t reversed() const & {return reversed(a.size());}
        poly_t reversed() && {reverse(); return std::move(*this);}

        friend poly_t operator * (T const& x, poly_t p) {p *= x; return p;}

        poly_t negx() const { // A(x) -> A(-x)
            auto res = *this;
            for(int i = 1; i <= deg(); i += 2) {
                res.a[i] = -res[i];
            }
            return res;
        }

        void print(int n) const {
            for(int i = 0; i < n; i++) {
                std::cout << (*this)[i] << ' ';
            }
            std::cout << "\n";
        }

        void print() const {
            print(deg() + 1);
        }

        T eval(T x) const { // evaluates in single point x
            T res(0);
            for(int i = deg(); i >= 0; i--) {
                res *= x;
                res += a[i];
            }
            return res;
        }

        T lead() const { // leading coefficient
            assert(!is_zero());
            return a.back();
        }

        int deg() const { // degree, -1 for P(x) = 0
            return (int)a.size() - 1;
        }

        bool is_zero() const {
            return a.empty();
        }

        T operator [](int idx) const {
            return idx < 0 || idx > deg() ? T(0) : a[idx];
        }

        T& coef(size_t idx) { // mutable reference at coefficient
            return a[idx];
        }

        bool operator == (const poly_t &t) const {return a == t.a;}
        bool operator != (const poly_t &t) const {return a != t.a;}

        size_t trailing_xk() const { // Let p(x) = x^k * t(x), return k
            if(is_zero()) {
                return -1;
            }
            int res = 0;
            while(a[res] == T(0)) {
                res++;
            }
            return res;
        }

        poly_t& mul_truncate(poly_t const& t, size_t k) {
            fft::mul_truncate(a, t.a, k);
            return normalize();
        }

        static poly_t xk(size_t n) { // P(x) = x^n
            return poly_t(T(1)).mul_xk(n);
        }

        static poly_t ones(size_t n) { // P(x) = 1 + x + ... + x^{n-1}
            return Vector(n, 1);
        }

        poly_t x2() const { // P(x) -> P(x^2)
            Vector res(2 * a.size());
            for(size_t i = 0; i < a.size(); i++) {
                res[2 * i] = a[i];
            }
            return res;
        }

        // Return {P0, P1}, where P(x) = P0(x^2) + xP1(x^2)
        std::array<poly_t, 2> bisect(size_t n) const {
            n = std::min(n, size(a));
            Vector res[2];
            for(size_t i = 0; i < n; i++) {
                res[i % 2].push_back(a[i]);
            }
            return {std::move(res[0]), std::move(res[1])};
        }
        std::array<poly_t, 2> bisect() const {
            return bisect(size(a));
        }

    };
}
#pragma GCC pop_options

#line 4 "cp-algo/math/poly/series/inv.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math::poly::impl {
    template<typename poly>
    poly& inv_inplace(poly& p, size_t n) {
        using base = poly::base;
        if(n == 0) {
            p.a.clear();
            return p;
        }
        assert(p[0] != base(0));
        if(n < magic) {
            typename poly::Vector q(n);
            q[0] = base(1) / p[0];
            for(size_t i = 1; i < n; i++) {
                for(size_t j = 1; j <= std::min(i, p.a.size() - 1); j++) {
                    q[i] -= p.a[j] * q[i - j];
                }
                q[i] *= q[0];
            }
            return p = std::move(q);
        }
        size_t m = std::bit_floor(size_t(magic - 1));
        auto q = p.mod_xk(m);
        inv_inplace(q, m);
        for(; m < n; m *= 2) {
            size_t k = std::min(2 * m, n);
            typename poly::Vector error((k + fft::flen - 1) / fft::flen * fft::flen);
            auto Q = fft::spectrum<base>(q.a, 2 * m);
            // Wrapping modulo x^(2m) + 1 only changes the discarded low half.
            fft::spectrum<base>(p.a | std::views::take(k), 2 * m).multiply(Q, error, k);
            auto E = fft::spectrum<base>(error | std::views::drop(m) | std::views::take(k - m), 2 * m);
            std::move(Q).multiply(E, error, k - m);
            q.a.resize(k);
            for(size_t i = m; i < k; i++) {q.a[i] = -error[i - m];}
        }
        p = std::move(q);
        p.normalize();
        return p;
    }
}
namespace cp_algo::math {
    // Inverse modulo x^n; the constant coefficient must be invertible.
    template<typename T>
    poly_t<T> inv(poly_t<T> p, size_t n) {
        poly::impl::inv_inplace(p, n);
        return p;
    }
}
#pragma GCC pop_options

#line 4 "cp-algo/math/poly/impl/div.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math::poly::impl {
    template<typename T>
    std::array<poly_t<T>, 2> divmod_slow(poly_t<T> p, poly_t<T> const& q) {
        poly_t<T> d;
        auto qi = q.lead() == T(1) ? T(1) : q.lead().inv();
        while(p.deg() >= q.deg()) {
            d.a.push_back(p.lead() * qi);
            if(d.lead() != T(0)) {
                for(size_t i = 1; i <= q.a.size(); i++) {
                    p.a[p.a.size() - i] -= d.lead() * q.a[q.a.size() - i];
                }
            }
            p.a.pop_back();
        }
        std::ranges::reverse(d.a);
        p.normalize();
        return {std::move(d), std::move(p)};
    }
    template<typename T>
    std::array<poly_t<T>, 2> divmod_hint(poly_t<T> p, poly_t<T> const& q, poly_t<T> const& qri) {
        assert(!q.is_zero());
        int n = p.deg() - q.deg();
        if(std::min(n, q.deg()) < magic) {
            return divmod_slow(std::move(p), q);
        }
        poly_t<T> d(typename poly_t<T>::Vector(p.a.rbegin(), p.a.rbegin() + n + 1));
        d.mul_truncate(qri, n + 1).reverse(n + 1);
        // Only coefficients below deg(q) survive in the remainder.
        auto low = d.mod_xk(q.deg());
        low.mul_truncate(q, q.deg());
        p.mod_xk_inplace(q.deg());
        p -= low;
        return {std::move(d), std::move(p)};
    }
}
#pragma GCC pop_options

#line 4 "cp-algo/math/poly/div.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math {
    // Quotient and remainder; q must be nonzero.
    template<typename T>
    std::array<poly_t<T>, 2> divmod(poly_t<T> p, poly_t<T> const& q) {
        assert(!q.is_zero());
        int n = p.deg() - q.deg();
        if(std::min(n, q.deg()) < magic) {
            return poly::impl::divmod_slow(std::move(p), q);
        }
        auto qi = inv(q.reversed(), n + 1);
        return poly::impl::divmod_hint(std::move(p), q, qi);
    }
}
#pragma GCC pop_options

#line 1 "cp-algo/math/poly/series.hpp"


#line 1 "cp-algo/math/poly/series/log.hpp"


#line 1 "cp-algo/math/poly/calculus.hpp"


#line 1 "cp-algo/math/factorials.hpp"


#line 1 "cp-algo/util/bump_alloc.hpp"


#line 5 "cp-algo/util/bump_alloc.hpp"
namespace cp_algo {
    template<class T, size_t max_len>
    struct bump_alloc {
        static char* buf;
        static size_t buf_ind;
        using value_type = T;
        template <class U> struct rebind { using other = bump_alloc<U, max_len>; };
        constexpr bool operator==(const bump_alloc&) const = default;
        constexpr bool operator!=(const bump_alloc&) const = default;
        bump_alloc() = default;
        template<class U> bump_alloc(const U&) {}
        T* allocate(size_t n) {
            buf_ind -= n * sizeof(T);
            buf_ind &= 0 - alignof(T);
            return (T*)(buf + buf_ind);
        }
        void deallocate(T*, size_t) {}
    };
    template<class T, size_t max_len>
    char* bump_alloc<T, max_len>::buf = big_alloc<char>().allocate(max_len * sizeof(T));
    template<class T, size_t max_len>
    size_t bump_alloc<T, max_len>::buf_ind = max_len * sizeof(T);
}

#line 1 "cp-algo/math/combinatorics.hpp"


#line 5 "cp-algo/math/combinatorics.hpp"
#include <cassert>
#line 7 "cp-algo/math/combinatorics.hpp"
namespace cp_algo::math {
    // fact/rfact/small_inv are caching
    // Beware of usage with dynamic mod
    template<typename T>
    T fact(auto n) {
        static big_vector<T> F(maxn);
        static bool init = false;
        if(!init) {
            F[0] = T(1);
            for(int i = 1; i < maxn; i++) {
                F[i] = F[i - 1] * T(i);
            }
            init = true;
        }
        return F[n];
    }
    // Only works for modint types
    template<typename T>
    T rfact(auto n) {
        static big_vector<T> F(maxn);
        static bool init = false;
        if(!init) {
            int t = (int)std::min<int64_t>(T::mod(), maxn) - 1;
            F[t] = T(1) / fact<T>(t);
            for(int i = t - 1; i >= 0; i--) {
                F[i] = F[i + 1] * T(i + 1);
            }
            init = true;
        }
        return F[n];
    }
    template<typename T, int base>
    T pow_fixed(int n) {
        static big_vector<T> prec_low(1 << 16);
        static big_vector<T> prec_high(1 << 16);
        static bool init = false;
        if(!init) {
            init = true;
            prec_low[0] = prec_high[0] = T(1);
            T step_low = T(base);
            T step_high = bpow(T(base), 1 << 16);
            for(int i = 1; i < (1 << 16); i++) {
                prec_low[i] = prec_low[i - 1] * step_low;
                prec_high[i] = prec_high[i - 1] * step_high;
            }
        }
        return prec_low[n & 0xFFFF] * prec_high[n >> 16];
    }
    template<typename T>
    big_vector<T> bulk_invs(auto const& args) {
        big_vector<T> res(std::size(args), args[0]);
        for(size_t i = 1; i < std::size(args); i++) {
            res[i] = res[i - 1] * args[i];
        }
        auto all_invs = T(1) / res.back();
        for(size_t i = std::size(args) - 1; i > 0; i--) {
            res[i] = all_invs * res[i - 1];
            all_invs *= args[i];
        }
        res[0] = all_invs;
        return res;
    }
    template<typename T>
    T small_inv(auto n) {
        static auto F = bulk_invs<T>(std::views::iota(1, maxn));
        return F[n - 1];
    }
    template<typename T>
    T binom_large(T n, auto r) {
        assert(r < maxn);
        T ans = 1;
        for(decltype(r) i = 0; i < r; i++) {
            ans = ans * T(n - i) * small_inv<T>(i + 1);
        }
        return ans;
    }
    template<typename T>
    T binom(auto n, auto r) {
        if(r < 0 || r > n) {
            return T(0);
        } else if(n >= maxn) {
            return binom_large(T(n), r);
        } else {
            return fact<T>(n) * rfact<T>(r) * rfact<T>(n - r);
        }
    }
}

#line 9 "cp-algo/math/factorials.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math {
    template<bool use_bump_alloc = false, int maxn = -1>
    auto facts(auto const& args) {
        static_assert(!use_bump_alloc || maxn > 0, "maxn must be set if use_bump_alloc is true");
        constexpr int max_mod = 1'000'000'000;
        constexpr int accum = 4;
        constexpr int simd_size = 8;
        constexpr int block = 1 << 18;
        constexpr int subblock = block / simd_size;
        using base = std::decay_t<decltype(args[0])>;
        static_assert(modint_type<base>, "Base type must be a modint type");
        using T = std::array<int, 2>;
        using alloc = std::conditional_t<use_bump_alloc,
            bump_alloc<T, 30 * maxn>,
            big_alloc<T>>;
        std::basic_string<T, std::char_traits<T>, alloc> odd_args_per_block[max_mod / subblock];
        std::basic_string<T, std::char_traits<T>, alloc> reg_args_per_block[max_mod / subblock];
        constexpr int limit_reg = max_mod / 64;
        int limit_odd = 0;

        big_vector<base> res(size(args), 1);
        const int mod = base::mod();
        const int imod = -math::inv2(mod);
        for(auto [i, xy]: std::views::zip(args, res) | std::views::enumerate) {
            auto [x, y] = xy;
            int t = x.getr();
            if(t >= mod / 2) {
                t = mod - t - 1;
                y = t % 2 ? 1 : mod-1;
            }
            auto pw = 32ull * (t + 1);
            while(t > limit_reg) {
                limit_odd = std::max(limit_odd, (t - 1) / 2);
                odd_args_per_block[(t - 1) / 2 / subblock].push_back({int(i), (t - 1) / 2});
                t /= 2;
                pw += t;
            }
            reg_args_per_block[t / subblock].push_back({int(i), t});
            y *= pow_fixed<base, 2>(int(pw % (mod - 1)));
        }
        checkpoint("init");
        base bi2x32 = pow_fixed<base, 2>(32).inv();
        auto process = [&](int limit, auto &args_per_block, auto step, auto &&proj) {
            base fact = 1;
            for(int b = 0; b <= limit; b += accum * block) {
                u32x8 cur[accum];
                static std::array<u32x8, subblock> prods[accum];
                for(int z = 0; z < accum; z++) {
                    for(int j = 0; j < simd_size; j++) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
                        cur[z][j] = uint32_t(b + z * block + j * subblock);
                        cur[z][j] = proj(cur[z][j]);
                        prods[z][0][j] = cur[z][j] + !cur[z][j];
                        prods[z][0][j] = uint32_t(uint64_t(prods[z][0][j]) * bi2x32.getr() % mod);
#pragma GCC diagnostic pop
                    }
                }
                for(int i = 1; i < block / simd_size; i++) {
                    for(int z = 0; z < accum; z++) {
                        cur[z] += step;
                        prods[z][i] = montgomery_mul(prods[z][i - 1], cur[z], mod, imod);
                    }
                }
                checkpoint("inner loop");
                for(int z = 0; z < accum; z++) {
                    for(int j = 0; j < simd_size; j++) {
                        int bl = b + z * block + j * subblock;
                        for(auto [i, x]: args_per_block[bl / subblock]) {
                            res[i] *= fact * prods[z][x - bl][j];
                        }
                        fact *= base(prods[z].back()[j]);
                    }
                }
                checkpoint("mul ans");
            }
        };
        process(limit_reg, reg_args_per_block, 1, std::identity{});
        process(limit_odd, odd_args_per_block, 2, [](uint32_t x) {return 2 * x + 1;});
        auto invs = bulk_invs<base>(res);
        for(auto [i, x]: res | std::views::enumerate) {
            if (args[i] >= mod / 2) {
                x = invs[i];
            }
        }
        checkpoint("inv ans");
        return res;
    }
}
#pragma GCC pop_options

#line 5 "cp-algo/math/poly/calculus.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math {
    // k-th derivative; consumes temporaries and moved polynomials.
    template<typename T>
    poly_t<T> deriv(poly_t<T> p, int k = 1) {
        assert(k >= 0);
        if(k > p.deg()) {return k == 0 ? p : poly_t<T>{};}
        if(k == 0) {return p;}
        if(k == 1) {
            for(int i = 1; i <= p.deg(); i++) {p.a[i - 1] = T(i) * p.a[i];}
            p.a.pop_back();
            p.normalize();
            return p;
        }
        for(int i = k; i <= p.deg(); i++) {
            p.a[i - k] = fact<T>(i) * rfact<T>(i - k) * p.a[i];
        }
        p.a.resize(p.a.size() - k);
        p.normalize();
        return p;
    }
    // Antiderivative with zero constant coefficient.
    template<typename T>
    poly_t<T> integr(poly_t<T> p) {
        if(p.is_zero()) {return p;}
        p.a.push_back(0);
        for(int i = p.deg() - 1; i >= 0; i--) {
            p.a[i + 1] = p.a[i] * small_inv<T>(i + 1);
        }
        p.a[0] = 0;
        return p;
    }
}
#pragma GCC pop_options

#line 5 "cp-algo/math/poly/series/log.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math {
    // log(p) modulo x^n, for p[0] = 1.
    template<typename T>
    poly_t<T> log(poly_t<T> p, size_t n) {
        if(n == 0) {return {};}
        assert(p[0] == T(1));
        p.mod_xk_inplace(n);
        auto dp = deriv(p);
        size_t k = n - 1;
        if(k < magic) {
            poly::impl::inv_inplace(p, k);
            p.mul_truncate(dp, k);
            return integr(std::move(p));
        }
        // Solve p*q = p' in two halves, avoiding a full-precision reciprocal.
        size_t m = std::bit_floor(k - 1), t = k - m;
        auto r = inv(p, m);
        auto R = fft::spectrum<T>(r.a, 2 * m);
        typename poly_t<T>::Vector work(2 * m);
        fft::spectrum<T>(dp.a | std::views::take(m), 2 * m).multiply(R, work, m);
        poly_t<T> q(typename poly_t<T>::Vector(begin(work), begin(work) + m));
        {
            auto Q = fft::spectrum<T>(q.a, 2 * m);
            fft::spectrum<T>(p.a | std::views::take(k), 2 * m).multiply(Q, work, k);
        }
        // Cyclic wraparound only affects the discarded low half.
        for(size_t i = 0; i < t; i++) {work[m + i] = dp[int(m + i)] - work[m + i];}
        fft::spectrum<T>(work | std::views::drop(m) | std::views::take(t), 2 * m).multiply(R, work, t);
        q.a.resize(k);
        std::copy_n(begin(work), t, begin(q.a) + m);
        q.normalize();
        return integr(std::move(q));
    }

}
#pragma GCC pop_options

#line 1 "cp-algo/math/poly/series/exp.hpp"


#line 5 "cp-algo/math/poly/series/exp.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math {
    // exp(p) modulo x^n, for p[0] = 0.
    template<typename T>
    poly_t<T> exp(poly_t<T> p, size_t n) {
        if(n == 0) {return {};}
        assert(p[0] == T(0));
        p.mod_xk_inplace(n);
        if(p.is_zero()) {return T(1);}
        size_t m = std::min(n, std::bit_floor(size_t(magic - 1)));
        typename poly_t<T>::Vector seed(m);
        seed[0] = 1;
        for(size_t i = 1; i < m; i++) {
            for(size_t j = 1; j <= i && j < p.a.size(); j++) {
                seed[i] += T(j) * p.a[j] * seed[i - j];
            }
            seed[i] *= small_inv<T>(i);
        }
        poly_t<T> q(std::move(seed));
        if(m == n) {return q;}
        auto r = inv(q, m), dp = deriv(p);
        for(; m < n; m *= 2) {
            size_t k = std::min(2 * m, n), t = k - m;
            auto Q = fft::spectrum<T>(q.a, 2 * m), R = fft::spectrum<T>(r.a, 2 * m);
            typename poly_t<T>::Vector work(2 * m);
            fft::spectrum<T>(dp.a | std::views::take(k - 1), 2 * m).multiply(Q, work, k - 1);
            // p' q - q' vanishes below m-1. Cyclic wrap only touches <m-1.
            fft::spectrum<T>(work | std::views::drop(m - 1) | std::views::take(t), 2 * m).multiply(R, work, t);
            for(size_t i = 0; i < t; i++) {work[i] *= small_inv<T>(m + i);}
            // d = (p - log(q)) / x^m. Keep it for the reciprocal correction.
            auto d = typename poly_t<T>::Vector(begin(work), begin(work) + t);
            fft::spectrum<T>(d, 2 * m).multiply(Q, work, t);
            q.a.resize(k);
            std::copy_n(begin(work), t, begin(q.a) + m);
            if(k == n) {break;}
            // (q*(1+x^m*d))^-1 = q^-1*(1-x^m*d) modulo x^(2m).
            std::move(Q).multiply(R, work, k);
            for(size_t i = 0; i < t; i++) {work[m + i] += d[i];}
            fft::spectrum<T>(work | std::views::drop(m) | std::views::take(t), 2 * m).multiply(R, work, t);
            r.a.resize(k);
            for(size_t i = 0; i < t; i++) {r.a[m + i] = -work[i];}
        }
        q.normalize();
        return q;
    }
}
#pragma GCC pop_options

#line 1 "cp-algo/math/poly/series/pow.hpp"


#line 5 "cp-algo/math/poly/series/pow.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math {
    namespace poly::impl {
        // O(deg(p) * n), using p q' = k p' q.
        template<typename T>
        poly_t<T> pow_slow(poly_t<T> const& p, int64_t k, size_t n) {
            typename poly_t<T>::Vector q(n);
            q[0] = bpow(p[0], k);
            auto a0inv = p[0].inv();
            for(int i = 1; i < (int)n; i++) {
                for(int j = 1; j <= std::min(p.deg(), i); j++) {
                    q[i] += p[j] * q[i - j] * (T(k) * T(j) - T(i - j));
                }
                q[i] *= small_inv<T>(i) * a0inv;
            }
            return q;
        }
    }
    // Nonnegative integer power modulo x^n.
    template<typename T>
    poly_t<T> pow(poly_t<T> p, int64_t k, size_t n) {
        assert(k >= 0);
        if(n == 0) {return {};}
        if(k == 0) {return T(1);}
        p.mod_xk_inplace(n);
        if(p.is_zero()) {return p;}
        size_t shift = p.trailing_xk();
        if(shift) {
            if(uint64_t(k) > (n - 1) / shift) {return {};}
            p.div_xk_inplace(shift);
            return pow(std::move(p), k, n - shift * k).mul_xk(shift * k);
        }
        if(std::min(p.deg(), (int)n) <= magic) {
            return poly::impl::pow_slow(p, k, n);
        }
        if(k <= magic) {
            auto t = pow(p, k / 2, n);
            t.mul_truncate(t, n);
            if(k % 2) {t.mul_truncate(p, n);}
            return t;
        }
        T c = p[0];
        p /= c;
        return bpow(c, k) * exp(log(std::move(p), n) * T(k), n);
    }
}
#pragma GCC pop_options

#line 6 "cp-algo/math/poly/series.hpp"

#line 5 "cp-algo/math/poly/powmod.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math {
    // Reduce modulo x^m - 1.
    template<typename T>
    poly_t<T> circular_closure(poly_t<T> p, size_t m) {
        assert(m > 0);
        for(size_t i = p.a.size(); i > m; --i) {p.a[i - 1 - m] += p.a[i - 1];}
        p.mod_xk_inplace(m);
        return p;
    }
    template<typename T>
    poly_t<T> powmod_circular(poly_t<T> p, int64_t k, size_t m) {
        assert(k >= 0 && m > 0);
        p = circular_closure(std::move(p), m);
        return bpow(p, k, poly_t<T>(1), [m](auto const& a, auto const& b) {
            auto product = a;
            product *= &a == &b ? product : b;
            return circular_closure(std::move(product), m);
        });
    }
    // Nonnegative integer power modulo a nonzero polynomial.
    template<typename T>
    poly_t<T> powmod(poly_t<T> p, int64_t k, poly_t<T> const& md) {
        assert(k >= 0 && !md.is_zero());
        int d = md.deg();
        if(d == 0) {return {};}
        if(md == poly_t<T>::xk(d)) {return pow(std::move(p), k, d);}
        if(md == poly_t<T>::xk(d) - poly_t<T>(1)) {return powmod_circular(std::move(p), k, d);}
        auto mdri = inv(md.reversed(), d + 1);
        return bpow(p % md, k, poly_t<T>(1), [&](auto const& a, auto const& b) {
            auto product = a;
            product *= &a == &b ? product : b;
            auto [q, r] = poly::impl::divmod_hint(std::move(product), md, mdri);
            return r;
        });
    }
}
#pragma GCC pop_options

#line 1 "cp-algo/linalg/matrix.hpp"


#line 1 "cp-algo/linalg/vector.hpp"


#line 13 "cp-algo/linalg/vector.hpp"
#include <cassert>
#line 17 "cp-algo/linalg/vector.hpp"
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

#line 9 "cp-algo/linalg/matrix.hpp"
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
            for(auto [a, b]: std::views::zip(elements(), t.elements())) {
                a += b;
            }
            return *this;
        }
        matrix& operator -=(matrix const& t) {
            for(auto [a, b]: std::views::zip(elements(), t.elements())) {
                a -= b;
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
            return normalize();
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
            auto scales = [&](vec_t &a) {
                base s = -a.normalize(u) * pu;
                base t = -a.normalize(v);
                // Reverse elimination has already cleared p[v] within the pivot block.
                if constexpr(mode == normal) t -= s * p[v];
                t *= qv;
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

#line 8 "cp-algo/linalg/frobenius.hpp"
namespace cp_algo::linalg {
    enum frobenius_mode {blocks, full};
    template<frobenius_mode mode = blocks>
    auto frobenius_form(auto const& A) {
        using matrix = std::decay_t<decltype(A)>;
        using vec_t = matrix::vec_t;
        using base = matrix::base;
        using polyn = math::poly_t<base>;
        assert(A.n() == A.m());
        size_t n = A.n();
        big_vector<polyn> charps;
        big_vector<vec_t> basis, basis_init;
        while(size(basis) < n) {
            size_t start = size(basis);
            // In blocks mode, record y_i = x_i + sum_j reductions[i][j] * y_j.
            big_vector<typename polyn::Vector> reductions;
            auto generate_block = [&](auto x) {
                while(true) {
                    vec_t y = x;
                    typename polyn::Vector coefficients;
                    if constexpr(mode == full) y.reserve(2 * n + 1);
                    else coefficients.resize(size(basis) - start);
                    for(size_t i = 0; i < size(basis); i++) {
                        auto &it = basis[i];
                        if constexpr(mode == full) {
                            y.resize(it.size());
                            y.reduce_by(it);
                        } else {
                            auto [p, inv] = it.find_pivot();
                            base scale = -y.normalize(p) * inv;
                            y.add_scaled(it, scale, p);
                            if(i >= start) coefficients[i - start] = scale;
                        }
                    }
                    if constexpr(mode == full) y.push_back(1);
                    y.normalize();
                    if(y.find_pivot().first >= n) {
                        if constexpr(mode == full) {
                            return polyn(typename polyn::Vector(begin(y) + n, end(y)));
                        } else {
                            // Expand the dependency through the triangular reduction history.
                            coefficients.push_back(1);
                            for(size_t i = size(reductions); i-- > 0;) {
                                for(size_t j = 0; j < i; j++) {
                                    coefficients[j] += coefficients[i] * reductions[i][j];
                                }
                            }
                            return polyn(std::move(coefficients));
                        }
                    } else {
                        if constexpr(mode == blocks) reductions.push_back(std::move(coefficients));
                        basis.push_back(std::move(y));
                        if constexpr(mode == full) {
                            basis_init.push_back(std::move(x));
                            x = A.apply(basis_init.back());
                        } else {
                            x = A.apply(x);
                        }
                    }
                }
            };
            auto x = vec_t::random(n);
            // Choose a random representative already reduced against earlier blocks.
            for(auto &row: basis) x[row.find_pivot().first] = 0;
            auto full_rec = generate_block(std::move(x));
            // Extra trimming to make it block-diagonal (expensive)
            if constexpr (mode == full) {
                if(full_rec.mod_xk(start) != polyn()) {
                    auto charp = full_rec.div_xk(start);
                    auto x = basis_init[start];
                    auto shift = full_rec / charp;
                    for(int j = 0; j < shift.deg(); j++) {
                        x.add_scaled(basis_init[j], shift[j]);
                    }
                    basis.resize(start);
                    basis_init.resize(start);
                    full_rec = generate_block(x.normalize());
                }
            }
            charps.push_back(full_rec.div_xk(mode == full ? start : 0));
        }
        // Find transform matrices while we're at it...
        if constexpr (mode == full) {
            for(auto &row: basis) row.resize(2 * n + 1);
            for(size_t i = 0; i < n; i++) {
                for(size_t j = i + 1; j < n; j++) {
                    basis[i].reduce_by(basis[j]);
                }
                basis[i].normalize();
            }
            auto T = matrix(std::move(basis_init));
            auto Tinv = matrix(std::move(basis));
            std::ignore = Tinv.sort_classify(n);
            for(size_t i = 0; i < n; i++) {
                Tinv[i] = vec_t(
                    Tinv[i] | std::views::drop(n) | std::views::take(n)
                ) * (base(1) / Tinv[i][i]);
            }
            return std::tuple{std::move(T), std::move(Tinv), std::move(charps)};
        } else {
            return charps;
        }
    }

    template<typename base>
    auto with_frobenius(matrix<base> const& A, auto &&callback) {
        auto [T, Tinv, charps] = frobenius_form<full>(A);
        // Apply each diagonal block to its rows of T without forming S.
        matrix<base> ST(A.n());
        size_t start = 0;
        for(auto const& charp: charps) {
            size_t d = charp.deg();
            auto xk = callback(charp);
            for(size_t i = 0; i < d; i++) {
                for(size_t j = 0; j < xk.a.size(); j++) {
                    ST[start + i].add_scaled(T[start + j], xk[j]);
                }
                if(i + 1 < d) {
                    xk = xk.mul_xk(1) % charp;
                }
            }
            start += d;
        }
        return Tinv * ST.normalize();
    }

    template<typename base>
    auto frobenius_pow(matrix<base> const& A, uint64_t k) {
        if(k <= 2) return A.pow(k);
        return with_frobenius(A, [k](auto const& charp) {
            if(charp.deg() == 1) {
                return math::poly_t<base>(bpow(-charp[0] / charp[1], k));
            }
            return math::powmod(math::poly_t<base>::xk(1), k, charp);
        });
    }
};

#line 9 "verify/linalg/characteristic.test.cpp"

using namespace std;
using namespace cp_algo::math;
using namespace cp_algo::linalg;

const int64_t mod = 998244353;
using base = modint<mod>;
using polyn = poly_t<base>;

void solve() {
    size_t n;
    cin >> n;
    matrix<base> A(n);
    A.read();
    auto blocks = frobenius_form(A);
    reduce(begin(blocks), end(blocks), polyn(1), multiplies{}).print();
}

signed main() {
    //freopen("input.txt", "r", stdin);
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    //cin >> t;
    while(t--) {
        solve();
    }
}
