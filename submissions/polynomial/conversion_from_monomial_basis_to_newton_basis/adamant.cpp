#line 1 "verify/poly/newton.test.cpp"
// @brief Conversion to Newton Basis
#define PROBLEM "https://judge.yosupo.jp/problem/conversion_from_monomial_basis_to_newton_basis"
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
#line 1 "cp-algo/math/poly/eval.hpp"


#line 1 "cp-algo/math/poly/div.hpp"


#line 1 "cp-algo/math/poly/impl/div.hpp"


#line 1 "cp-algo/math/poly/series/inv.hpp"


#line 1 "cp-algo/math/poly/base.hpp"


#line 1 "cp-algo/math/fft.hpp"


#line 1 "cp-algo/math/dft.hpp"


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
        constexpr static Int remod() {
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

    template<typename Int = int>
    struct dynamic_modint: modint_base<dynamic_modint<Int>, Int> {
        using Base = modint_base<dynamic_modint<Int>, Int>;
        using Base::Base;

        static Base::UInt m_reduce(Base::UInt2 ab) {
            if(mod() % 2 == 0) [[unlikely]] {
                return typename Base::UInt(ab % mod());
            } else {
                typename Base::UInt2 m = typename Base::UInt(ab) * imod();
                return typename Base::UInt((ab + m * mod()) >> Base::bits);
            }
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
        static Int remod() {return 2 * m;}
        static Base::UInt imod() {return im;}
        static Base::UInt2 pw128() {return r2;}
        static void switch_mod(Int nm) {
            m = nm;
            im = m % 2 ? inv2(-m) : 0;
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
        static thread_local Base::UInt im, r2;
    };
    template<typename Int>
    Int thread_local dynamic_modint<Int>::m = 1;
    template<typename Int>
    dynamic_modint<Int>::Base::UInt thread_local dynamic_modint<Int>::im = -1;
    template<typename Int>
    dynamic_modint<Int>::Base::UInt thread_local dynamic_modint<Int>::r2 = 0;
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
                madvise(raw, padded, MADV_POPULATE_WRITE);
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

#line 1 "cp-algo/math/cvector.hpp"


#line 1 "cp-algo/util/simd.hpp"


#include <experimental/simd>
#line 7 "cp-algo/util/simd.hpp"

#if defined(__x86_64__) && !defined(CP_ALGO_DISABLE_AVX2)
#define CP_ALGO_SIMD_AVX2_TARGET _Pragma("GCC target(\"avx2\")")
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
#include <ranges>
#line 9 "cp-algo/math/cvector.hpp"
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

    struct cvector {
        big_vector<vpoint> r;
        cvector(size_t n) {
            n = std::max(flen, std::bit_ceil(n));
            r.resize(n / flen);
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
            point factor = root(4 * step * n);
            for(size_t i = 0; i < n; i++) {
                callback(i, factor * eval_point(step * i));
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

        void dot(cvector const& t) {
            size_t n = this->size();
            exec_on_evals<1>(n / flen, [&](size_t k, point rt) __attribute__((always_inline)) {
                k *= flen;
                auto [Ax, Ay] = at(k);
                auto Bv = t.at(k);
                vpoint res = vz;
                for (size_t i = 0; i < flen; i++) {
                    vpoint Av = vpoint(vz + Ax[i], vz + Ay[i]);
                    do_dot_iter(rt, Bv, Av, res);
                }
                set(k, res);
            });
            checkpoint("dot");
        }
        // normalize=false leaves the inverse-transform scale for the caller.
        template<bool partial = true, bool normalize = true>
        void ifft() {
            size_t n = size();
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

            for(size_t leaf = 3 * flen; leaf < n; leaf += 4 * flen) {
                size_t level = std::countr_one(leaf + 3);
                for(size_t lvl = 4 + parity; lvl <= level; lvl += 2) {
                    size_t i = (1 << lvl) / 4;
                    exec_on_eval<4>(n >> lvl, leaf >> lvl, [&](size_t k, point rt) __attribute__((always_inline)) {
                        k <<= lvl;
                        vpoint v1 = {vz + real(rt), vz - imag(rt)};
                        vpoint v2 = v1 * v1;
                        vpoint v3 = v1 * v2;
                        for(size_t j = k; j < k + i; j += flen) {
                            auto A = at(j);
                            auto B = at(j + i);
                            auto C = at(j + 2 * i);
                            auto D = at(j + 3 * i);
                            at(j) = ((A + B) + (C + D));
                            at(j + 2 * i) = ((A + B) - (C + D)) * v2;
                            at(j +     i) = ((A - B) - vi(C - D)) * v1;
                            at(j + 3 * i) = ((A - B) + vi(C - D)) * v3;
                        }
                    });
                }
            }
            checkpoint("ifft");
            if constexpr(normalize) {
                auto scale = vz + ftype(partial ? flen : 1) / ftype(n);
                for(size_t k = 0; k < n; k += flen) {
                    set(k, get<vpoint>(k) * scale);
                }
            }
        }
        template<bool partial = true>
        void fft() {
            size_t n = size();
            bool parity = std::countr_zero(n) % 2;
            for(size_t leaf = 0; leaf < n; leaf += 4 * flen) {
                size_t level = std::countr_zero(n + leaf);
                level -= level % 2 != parity;
                for(size_t lvl = level; lvl >= 4; lvl -= 2) {
                    size_t i = (1 << lvl) / 4;
                    exec_on_eval<4>(n >> lvl, leaf >> lvl, [&](size_t k, point rt) __attribute__((always_inline)) {
                        k <<= lvl;
                        vpoint v1 = {vz + real(rt), vz + imag(rt)};
                        vpoint v2 = v1 * v1;
                        vpoint v3 = v1 * v2;
                        for(size_t j = k; j < k + i; j += flen) {
                            auto A = at(j);
                            auto B = at(j + i) * v1;
                            auto C = at(j + 2 * i) * v2;
                            auto D = at(j + 3 * i) * v3;
                            at(j)         = (A + C) + (B + D);
                            at(j + i)     = (A + C) - (B + D);
                            at(j + 2 * i) = (A - C) + vi(B - D);
                            at(j + 3 * i) = (A - C) - vi(B - D);
                        }
                    });
                }
            }
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
        static constexpr size_t pre_evals = 1 << 16;
        static const std::array<size_t, pre_evals> eval_args;
        static const std::array<point, pre_evals> evalp;
    private:
        static big_vector<point> extra;
        // Keep the usual table small; cache additional roots for large transforms.
        static void prepare_roots(size_t n) {
            if(n <= pre_evals + extra.size()) {return;}
            size_t old = extra.size();
            extra.resize(std::bit_ceil(n) - pre_evals);
            for(size_t i = old; i < extra.size(); i++) {
                size_t j = 4 * (i + pre_evals);
                extra[i] = polar<ftype>(1., std::numbers::pi / (ftype)std::bit_floor(j) * (ftype)eval_arg(j));
            }
        }
    };

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

#line 9 "cp-algo/math/dft.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math::fft {
    // Twist coefficients by a random factor, split near sqrt(mod), and pack pairs of halves.
    template<modint_type base>
    struct dft {
        cvector A, B;
        static base factor, ifactor;
        using Int2 = base::Int2;
        static bool _init;
        static int split() {
            static const int splt = int(std::sqrt(base::mod())) + 1;
            return splt;
        }
        static uint32_t mod, imod;

        static void init() {
            if(!_init) {
                factor = 1 + random::rng() % (base::mod() - 1);
                ifactor = base(1) / factor;
                mod = base::mod();
                imod = -inv2<uint32_t>(base::mod());
                _init = true;
            }
        }

        static std::pair<vftype, vftype>
        do_split(auto const& a, size_t idx, u64x4 mul) {
            if(idx >= std::size(a)) {
                return std::pair{vftype(), vftype()};
            }
            u64x4 au = {
                idx < std::size(a) ? a[idx].getr() : 0,
                idx + 1 < std::size(a) ? a[idx + 1].getr() : 0,
                idx + 2 < std::size(a) ? a[idx + 2].getr() : 0,
                idx + 3 < std::size(a) ? a[idx + 3].getr() : 0
            };
            au = montgomery_mul(au, mul, mod, imod);
            au = au >= base::mod() ? au - base::mod() : au;
            auto ai = to_double(i64x4(au >= base::mod() / 2 ? au - base::mod() : au));
            auto quo = round(ai * (1.0 / split()));
            return std::pair{ai - quo * split(), quo};
        }

        dft(size_t n): A(n), B(n) {init();}
        dft(auto const& a, size_t n, bool partial = true): A(n), B(n) {
            init();
            base b2x32 = bpow(base(2), 32);
            u64x4 cur = {
                (bpow(factor, 1) * b2x32).getr(),
                (bpow(factor, 2) * b2x32).getr(),
                (bpow(factor, 3) * b2x32).getr(),
                (bpow(factor, 4) * b2x32).getr()
            };
            u64x4 step4 = u64x4{} + (bpow(factor, 4) * b2x32).getr();
            u64x4 stepn = u64x4{} + (bpow(factor, n) * b2x32).getr();
            for(size_t i = 0; i < std::min(n, std::size(a)); i += flen) {
                auto [rai, qai] = do_split(a, i, cur);
                auto [rani, qani] = do_split(a, n + i, montgomery_mul(cur, stepn, mod, imod));
                A.at(i) = vpoint(rai, rani);
                B.at(i) = vpoint(qai, qani);
                cur = montgomery_mul(cur, step4, mod, imod);
            }
            checkpoint("dft init");
            if(n) {
                if(partial) {
                    A.fft();
                    B.fft();
                } else {
                    A.template fft<false>();
                    B.template fft<false>();
                }
            }
        }
        static void do_dot_iter(point rt, vpoint& Cv, vpoint& Dv, vpoint const& Av, vpoint const& Bv, vpoint& AC, vpoint& AD, vpoint& BC, vpoint& BD) {
            AC += Av * Cv; AD += Av * Dv;
            BC += Bv * Cv; BD += Bv * Dv;
            real(Cv) = rotate_right(real(Cv));
            imag(Cv) = rotate_right(imag(Cv));
            real(Dv) = rotate_right(real(Dv));
            imag(Dv) = rotate_right(imag(Dv));
            auto cx = real(Cv)[0], cy = imag(Cv)[0];
            auto dx = real(Dv)[0], dy = imag(Dv)[0];
            real(Cv)[0] = cx * real(rt) - cy * imag(rt);
            imag(Cv)[0] = cx * imag(rt) + cy * real(rt);
            real(Dv)[0] = dx * real(rt) - dy * imag(rt);
            imag(Dv)[0] = dx * imag(rt) + dy * real(rt);
        }

        // Multiply split evaluations; Cout collects the mixed low/high terms.
        template<bool overwrite = true, bool partial = true>
        void dot(auto const& C, auto const& D, auto &Aout, auto &Bout, auto &Cout) const {
            cvector::exec_on_evals<1>(A.size() / flen, [&](size_t k, point rt) __attribute__((always_inline)) {
                k *= flen;
                vpoint AC, AD, BC, BD;
                AC = AD = BC = BD = vz;
                auto Cv = C.at(k), Dv = D.at(k);
                if constexpr(partial) {
                    auto [Ax, Ay] = A.at(k);
                    auto [Bx, By] = B.at(k);
                    for (size_t i = 0; i < flen; i++) {
                        vpoint Av = {vz + Ax[i], vz + Ay[i]}, Bv = {vz + Bx[i], vz + By[i]};
                        do_dot_iter(rt, Cv, Dv, Av, Bv, AC, AD, BC, BD);
                    }
                } else {
                    AC = A.at(k) * Cv;
                    AD = A.at(k) * Dv;
                    BC = B.at(k) * Cv;
                    BD = B.at(k) * Dv;
                }
                if constexpr (overwrite) {
                    Aout.at(k) = AC;
                    Cout.at(k) = AD + BC;
                    Bout.at(k) = BD;
                } else {
                    Aout.at(k) += AC;
                    Cout.at(k) += AD + BC;
                    Bout.at(k) += BD;
                }
            });
            checkpoint("dot");
        }

        void dot(auto &&C, auto const& D) {
            dot(C, D, A, B, C);
        }

        static void do_recover_iter(size_t idx, auto A, auto B, auto C, auto mul, uint64_t splitsplit, auto &res) {
            auto A0 = lround(A), A1 = lround(C), A2 = lround(B);
            // Center signed lifts in the unsigned Montgomery input range [0, mod*2^32).
            auto Ai = A0 + A1 * split() + A2 * splitsplit + (uint64_t(base::mod()) << 31);
            auto Au = montgomery_reduce(u64x4(Ai), mod, imod);
            Au = montgomery_mul(Au, mul, mod, imod);
            Au = Au >= base::mod() ? Au - base::mod() : Au;
            for(size_t j = 0; j < flen; j++) {
                res[idx + j].setr(typename base::UInt(Au[j]));
            }
        }

        // Round the convolutions and undo twisting, optionally including the inverse-FFT scale.
        template<bool normalized = true>
        void recover_mod(auto &&C, auto &res, size_t k) {
            size_t check = (k + flen - 1) / flen * flen;
            assert(res.size() >= check);
            size_t n = A.size();
            auto scale = vz + ftype(flen) / ftype(n);
            auto const splitsplit = base(split() * split()).getr();
            base b2x32 = bpow(base(2), 32);
            base b2x64 = bpow(base(2), 64);
            u64x4 cur = {
                (bpow(ifactor, 2) * b2x64).getr(),
                (bpow(ifactor, 3) * b2x64).getr(),
                (bpow(ifactor, 4) * b2x64).getr(),
                (bpow(ifactor, 5) * b2x64).getr()
            };
            u64x4 step4 = u64x4{} + (bpow(ifactor, 4) * b2x32).getr();
            u64x4 stepn = u64x4{} + (bpow(ifactor, n) * b2x32).getr();
            for(size_t i = 0; i < std::min(n, k); i += flen) {
                auto get = [&](auto const& x) {
                    if constexpr(normalized) {return x.at(i);}
                    else {return x.at(i) * scale;}
                };
                auto [Ax, Ay] = get(A);
                auto [Bx, By] = get(B);
                auto [Cx, Cy] = get(C);
                do_recover_iter(i, Ax, Bx, Cx, cur, splitsplit, res);
                if(i + n < k) {
                    do_recover_iter(i + n, Ay, By, Cy, montgomery_mul(cur, stepn, mod, imod), splitsplit, res);
                }
                cur = montgomery_mul(cur, step4, mod, imod);
            }
            checkpoint("recover mod");
        }

        void mul(auto &&C, auto const& D, auto &res, size_t k) {
            assert(A.size() == C.size());
            size_t n = A.size();
            if(!n) {
                res = {};
                return;
            }
            dot(C, D);
            // Normalize during recovery to avoid another pass over the buffers.
            A.template ifft<true, false>();
            B.template ifft<true, false>();
            C.template ifft<true, false>();
            recover_mod<false>(C, res, k);
        }
        void mul_inplace(auto &&B, auto& res, size_t k) {
            mul(B.A, B.B, res, k);
        }
        void mul(auto const& B, auto& res, size_t k) {
            mul(cvector(B.A), B.B, res, k);
        }
        big_vector<base> operator *= (dft &B) {
            big_vector<base> res(2 * A.size());
            mul_inplace(B, res, 2 * A.size());
            return res;
        }
        big_vector<base> operator *= (dft const& B) {
            big_vector<base> res(2 * A.size());
            mul(B, res, 2 * A.size());
            return res;
        }
        auto operator * (dft const& B) const {
            return dft(*this) *= B;
        }

        point operator [](int i) const {return A.get(i);}
    };
    template<modint_type base> base dft<base>::factor = 1;
    template<modint_type base> base dft<base>::ifactor = 1;
    template<modint_type base> bool dft<base>::_init = false;
    template<modint_type base> uint32_t dft<base>::mod = {};
    template<modint_type base> uint32_t dft<base>::imod = {};

}
#pragma GCC pop_options

#line 4 "cp-algo/math/fft.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math::fft {
    void mul_slow(auto &a, auto const& b, size_t k) {
        if(!std::empty(a) && std::data(a) == std::data(b)) {
            using base = std::decay_t<decltype(a[0])>;
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
            a.resize(k);
            for(int j = int(k - 1); j >= 0; j--) {
                a[j] *= b[0];
                for(int i = std::max(j - (int)n, 0) + 1; i < std::min(j + 1, (int)m); i++) {
                    a[j] += a[j - i] * b[i];
                }
            }
        }
    }
    size_t com_size(size_t as, size_t bs) {
        if(!as || !bs) {
            return 0;
        }
        return std::max(flen, std::bit_ceil(as + bs - 1) / 2);
    }
    void mul_truncate(auto &a, auto const& b, size_t k) {
        using base = std::decay_t<decltype(a[0])>;
        if(std::min({k, std::size(a), std::size(b)}) < magic) {
            mul_slow(a, b, k);
            return;
        }
        auto n = std::max(flen, std::bit_ceil(
            std::min(k, std::size(a)) + std::min(k, std::size(b)) - 1
        ) / 2);
        size_t as = std::min(k, std::size(a)), bs = std::min(k, std::size(b));
        size_t tail = as + bs - 1 - n;
        // Correct a short wrapped tail instead of doubling the FFT size.
        if(tail <= 32 && as <= n && bs <= n) {
            std::array<base, 32> high{};
            for(size_t i = 0; i < tail; i++) {
                for(size_t j = n + i - bs + 1; j < as; j++) {
                    high[i] += a[j] * b[n + i - j];
                }
            }
            auto A = dft<base>(a | std::views::take(k), n / 2);
            if(as == bs && std::data(a) == std::data(b)) {
                a.resize((k + flen - 1) / flen * flen);
                A.mul(A, a, std::min(k, n));
            } else {
                auto B = dft<base>(b | std::views::take(k), n / 2);
                a.resize((k + flen - 1) / flen * flen);
                A.mul_inplace(B, a, std::min(k, n));
            }
            auto wrap = bpow(dft<base>::factor, n);
            for(size_t i = 0; i < tail; i++) {
                a[i] += wrap * high[i];
                if(n + i < k) {a[n + i] = high[i];}
            }
            a.resize(k);
            return;
        }
        auto A = dft<base>(a | std::views::take(k), n);
        if(as == bs && std::data(a) == std::data(b)) {
            a.resize((k + flen - 1) / flen * flen);
            A.mul(A, a, k);
        } else {
            auto B = dft<base>(b | std::views::take(k), n);
            a.resize((k + flen - 1) / flen * flen);
            A.mul_inplace(B, a, k);
        }
        a.resize(k);
    }

    // store mod x^n-k in first half, x^n+k in second half
    // inverse reconstructs the halves with k = 1/(2 * forward_k).
    template<bool inverse = false>
    void mod_split(auto &&x, size_t n, auto k) {
        using base = std::decay_t<decltype(k)>;
        dft<base>::init();
        assert(std::size(x) == 2 * n);
        u64x4 cur = u64x4{} + (k * bpow(base(2), 32)).getr();
        for(size_t i = 0; i < n; i += flen) {
            u64x4 xl = {
                x[i].getr(),
                x[i + 1].getr(),
                x[i + 2].getr(),
                x[i + 3].getr()
            };
            u64x4 xr = {
                x[n + i].getr(),
                x[n + i + 1].getr(),
                x[n + i + 2].getr(),
                x[n + i + 3].getr()
            };
            if constexpr(!inverse) {
                xr = montgomery_mul(xr, cur, dft<base>::mod, dft<base>::imod);
                xr = xr >= base::mod() ? xr - base::mod() : xr;
            }
            auto t = xr;
            xr = xl - t;
            xl += t;
            xl = xl >= base::mod() ? xl - base::mod() : xl;
            xr = xr >= base::mod() ? xr + base::mod() : xr;
            if constexpr(inverse) {
                xl = (xl + (xl & 1) * base::mod()) >> 1;
                xr = montgomery_mul(xr, cur, dft<base>::mod, dft<base>::imod);
                xr = xr >= base::mod() ? xr - base::mod() : xr;
            }
            for(size_t k = 0; k < flen; k++) {
                x[i + k].setr(typename base::UInt(xl[k]));
                x[n + i + k].setr(typename base::UInt(xr[k]));
            }
        }
        cp_algo::checkpoint(inverse ? "mod join" : "mod split");
    }
    // zero_upper skips arithmetic on the known zero padding in the first split.
    void cyclic_mul(auto &a, auto &&b, size_t k, bool zero_upper = false) {
        assert(std::popcount(k) == 1);
        assert(std::size(a) == std::size(b) && std::size(a) == k);
        using base = std::decay_t<decltype(a[0])>;
        dft<base>::init();
        bool square = std::data(a) == std::data(b);
        if(k <= (1 << 16)) {
            big_vector<base> ap(begin(a), end(a));
            if(square) {mul_truncate(ap, ap, 2 * k);}
            else {mul_truncate(ap, b, 2 * k);}
            mod_split(ap, k, bpow(dft<base>::factor, k));
            std::ranges::copy(ap | std::views::take(k), begin(a));
            return;
        }
        k /= 2;
        auto factor = bpow(dft<base>::factor, k);
        if(zero_upper) {
            std::ranges::copy(std::span(a).first(k), begin(a) + k);
            if(!square) {std::ranges::copy(std::span(b).first(k), begin(b) + k);}
        } else {
            mod_split(a, k, factor);
            if(!square) {mod_split(b, k, factor);}
        }
        auto la = std::span(a).first(k);
        auto lb = std::span(b).first(k);
        auto ra = std::span(a).last(k);
        auto rb = std::span(b).last(k);
        cyclic_mul(la, lb, k);
        auto A = dft<base>(ra, k / 2);
        if(square) {A.mul(A, ra, k);}
        else {
            auto B = dft<base>(rb, k / 2);
            A.mul_inplace(B, ra, k);
        }
        base i2 = base(2).inv();
        factor = factor.inv() * i2;
        mod_split<true>(a, k, factor);
    }
    auto make_copy(auto &&x) {
        return x;
    }
    void cyclic_mul(auto &a, auto const& b, size_t k) {
        return cyclic_mul(a, make_copy(b), k);
    }
    namespace impl {
        // Overlap-add for a short fixed operand; every block reuses its transform.
        void mul_unbalanced(auto &a, auto const& b) {
            using base = std::decay_t<decltype(a[0])>;
            auto x = std::span<base const>(a), y = std::span<base const>(b);
            if(x.size() < y.size()) {std::swap(x, y);}
            constexpr size_t length = 1 << 15;
            size_t step = length - y.size() + 1;
            auto fixed = dft<base>(y, length / 2);
            std::decay_t<decltype(a)> result(x.size() + y.size() - 1);
            big_vector<base> work(length);
            for(size_t start = 0; start < x.size(); start += step) {
                size_t count = std::min(step, x.size() - start);
                auto block = dft<base>(x.subspan(start, count), length / 2);
                size_t need = count + y.size() - 1;
                block.mul(fixed, work, need);
                for(size_t i = 0; i < need; i++) {result[start + i] += work[i];}
            }
            a = std::move(result);
        }
    }
    void mul(auto &a, auto &&b) {
        if(std::empty(a) || std::empty(b)) {a.clear(); return;}
        bool square = std::data(a) == std::data(b) && std::size(a) == std::size(b);
        if(!square && std::data(a) == std::data(b)) {
            auto copy = make_copy(b);
            return mul(a, copy);
        }
        size_t small = std::min(size(a), size(b)), large = std::max(size(a), size(b));
        if(small >= magic && small <= 4096 && large >= (1 << 20) && large / small >= 64) {
            return impl::mul_unbalanced(a, b);
        }
        size_t N = size(a) + size(b);
        if(N > (1 << 20)) {
            N--;
            size_t NN = std::bit_ceil(N);
            bool zero_upper = std::max(size(a), size(b)) <= NN / 2;
            a.resize(NN);
            if(!square) {b.resize(NN);}
            cyclic_mul(a, b, NN, zero_upper);
            a.resize(N);
        } else {
            mul_truncate(a, b, N - 1);
        }
    }
    void mul(auto &a, auto const& b) {
        if(std::empty(a) || std::empty(b)) {a.clear(); return;}
        size_t small = std::min(size(a), size(b)), large = std::max(size(a), size(b));
        if(small >= magic && small <= 4096 && large >= (1 << 20) && large / small >= 64) {
            return impl::mul_unbalanced(a, b);
        }
        size_t N = size(a) + size(b);
        if(N > (1 << 20)) {
            if(std::data(a) == std::data(b) && std::size(a) == std::size(b)) {mul(a, a);}
            else {mul(a, make_copy(b));}
        } else {
            mul_truncate(a, b, N - 1);
        }
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
            auto Q = fft::dft<base>(q.a, m);
            {
                auto P = fft::dft<base>(p.a | std::views::take(k), m);
                // Wrapping modulo x^(2m) + factor^(2m) only changes the discarded low half.
                P.mul(Q, error, k);
            }
            auto E = fft::dft<base>(error | std::views::drop(m) | std::views::take(k - m), m);
            Q.mul_inplace(E, error, k - m);
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

#line 5 "cp-algo/math/poly/eval.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math::poly::impl {
    template<typename T>
    void build(big_vector<poly_t<T>> &tree, int v, auto l, auto r) {
        if(r - l == 1) {
            tree[v] = typename poly_t<T>::Vector{-*l, 1};
        } else {
            auto m = l + (r - l) / 2;
            build(tree, 2 * v, l, m);
            build(tree, 2 * v + 1, m, r);
            tree[v] = tree[2 * v] * tree[2 * v + 1];
        }
    }
    template<typename T>
    void eval(poly_t<T> const& p, big_vector<poly_t<T>> const& tree, int v, auto l, auto r, auto out) {
        if(r - l <= 32) {
            for(; l != r; ++l, ++out) {*out = p.eval(*l);}
        } else {
            auto m = l + (r - l) / 2;
            eval(p % tree[2 * v], tree, 2 * v, l, m, out);
            eval(p % tree[2 * v + 1], tree, 2 * v + 1, m, r, out + (m - l));
        }
    }
    template<typename T>
    poly_t<T> inter(big_vector<poly_t<T>> const& tree, int v, auto l, auto r) {
        if(r - l == 1) {return *l;}
        auto m = l + (r - l) / 2;
        auto a = inter(tree, 2 * v, l, m);
        auto b = inter(tree, 2 * v + 1, m, r);
        return std::move(a) * tree[2 * v + 1] + std::move(b) * tree[2 * v];
    }
    template<typename T>
    poly_t<T> to_newton(poly_t<T> p, big_vector<poly_t<T>> const& tree, int v, size_t l, size_t r) {
        if(r - l == 1) {return p;}
        size_t m = l + (r - l) / 2;
        auto [q, rem] = divmod(std::move(p), tree[2 * v]);
        auto a = to_newton(std::move(rem), tree, 2 * v, l, m);
        auto b = to_newton(std::move(q), tree, 2 * v + 1, m, r);
        a += b.mul_xk_inplace(m - l);
        return a;
    }
}
namespace cp_algo::math {
    // Evaluate at each point, preserving the order and length of x.
    template<typename T>
    big_vector<T> eval(poly_t<T> const& p, big_vector<T> const& x) {
        big_vector<T> res(x.size());
        if(x.empty() || p.is_zero()) {return res;}
        big_vector<poly_t<T>> tree(4 * x.size());
        poly::impl::build(tree, 1, begin(x), end(x));
        poly::impl::eval(p, tree, 1, begin(x), end(x), begin(res));
        return res;
    }
    // Interpolate at distinct x[i], with values y[i].
    template<typename T>
    poly_t<T> inter(big_vector<T> const& x, big_vector<T> const& y) {
        assert(x.size() == y.size());
        if(x.empty()) {return {};}
        big_vector<poly_t<T>> tree(4 * x.size());
        poly::impl::build(tree, 1, begin(x), end(x));
        big_vector<T> values(x.size());
        poly::impl::eval(deriv(tree[1]), tree, 1, begin(x), end(x), begin(values));
        auto weights = bulk_invs<T>(values);
        for(size_t i = 0; i < y.size(); i++) {weights[i] *= y[i];}
        return poly::impl::inter(tree, 1, begin(weights), end(weights));
    }
    // Convert to the basis 1, (x-p[0]), (x-p[0])(x-p[1]), ... .
    template<typename T>
    poly_t<T> to_newton(poly_t<T> f, big_vector<T> const& p) {
        assert(f.deg() < (int)p.size());
        if(p.empty() || f.is_zero()) {return f;}
        big_vector<poly_t<T>> tree(4 * p.size());
        poly::impl::build(tree, 1, begin(p), end(p));
        return poly::impl::to_newton(std::move(f), tree, 1, 0, p.size());
    }
}
#pragma GCC pop_options

#line 9 "verify/poly/newton.test.cpp"

using namespace std;
using namespace cp_algo::math;

const int mod = 998244353;
using base = modint<mod>;
using polyn = poly_t<base>;

void solve() {
    int n;
    cin >> n;
    polyn::Vector a(n), p(n);
    for(auto &it: a) {cin >> it;}
    for(auto &it: p) {cin >> it;}
    to_newton(polyn(std::move(a)), p).print(n);
}

signed main() {
    //freopen("input.txt", "r", stdin);
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t;
    t = 1;// cin >> t;
    while(t--) {
        solve();
    }
}
