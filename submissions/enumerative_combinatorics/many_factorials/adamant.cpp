#line 1 "main.cpp"
#line 1 "cp-algorithms-aux/verify/simd/many_facts.test.cpp"
// @brief Many Factorials
#define PROBLEM "https://judge.yosupo.jp/problem/many_factorials"
#pragma GCC optimize("Ofast,unroll-loops")
#include <bits/stdc++.h>
#define CP_ALGO_CHECKPOINT
#line 1 "cp-algorithms-aux/blazingio/blazingio.min.hpp"
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
#line 41 "cp-algorithms-aux/blazingio/blazingio.min.hpp"
#include $a(<arm_neon.h>,<immintrin.h>)
#line 43 "cp-algorithms-aux/blazingio/blazingio.min.hpp"
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
#line 1 "cp-algorithms-aux/cp-algo/util/checkpoint.hpp"


#line 7 "cp-algorithms-aux/cp-algo/util/checkpoint.hpp"
namespace cp_algo {
    std::map<std::string, double> checkpoints;
    template<bool final = false>
    void checkpoint([[maybe_unused]] std::string const& msg = "") {
#ifdef CP_ALGO_CHECKPOINT
        static double last = 0;
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
}

#line 1 "cp-algorithms-aux/cp-algo/util/simd.hpp"


#include <experimental/simd>
#line 7 "cp-algorithms-aux/cp-algo/util/simd.hpp"
namespace cp_algo {
    template<typename T, size_t len>
    using simd [[gnu::vector_size(len * sizeof(T))]] = T;
    using u32x8 = simd<uint32_t, 8>;
    using i64x4 = simd<int64_t, 4>;
    using u64x4 = simd<uint64_t, 4>;
    using i32x4 = simd<int32_t, 4>;
    using u32x4 = simd<uint32_t, 4>;
    using i16x4 = simd<int16_t, 4>;
    using dx4 = simd<double, 4>;

    [[gnu::always_inline]] inline dx4 abs(dx4 a) {
    return a < 0 ? -a : a;
    }

    // https://stackoverflow.com/a/77376595
    // works for ints in (-2^51, 2^51)
    static constexpr dx4 magic = dx4() + (3ULL << 51);
    [[gnu::always_inline]] inline i64x4 lround(dx4 x) {
        return i64x4(x + magic) - i64x4(magic);
    }
    [[gnu::always_inline]] inline dx4 to_double(i64x4 x) {
        return dx4(x + i64x4(magic)) - magic;
    }

    [[gnu::always_inline]] inline dx4 round(dx4 a) {
        return dx4{
            std::nearbyint(a[0]),
            std::nearbyint(a[1]),
            std::nearbyint(a[2]),
            std::nearbyint(a[3])
        };
    }

    [[gnu::always_inline]] inline u64x4 low32(u64x4 x) {
        return x & uint32_t(-1);
    }
    [[gnu::always_inline]] inline auto swap_bytes(auto x) {
        return decltype(x)(__builtin_shufflevector(u32x8(x), u32x8(x), 1, 0, 3, 2, 5, 4, 7, 6));
    }
    [[gnu::always_inline]] inline u64x4 montgomery_reduce(u64x4 x, uint32_t mod, uint32_t imod) {
#ifdef __AVX2__
        auto x_ninv = u64x4(_mm256_mul_epu32(__m256i(x), __m256i() + imod));
        x += u64x4(_mm256_mul_epu32(__m256i(x_ninv), __m256i() + mod));
#else
        auto x_ninv = x * imod;
        x += low32(x_ninv) * mod;
#endif
        return swap_bytes(x);
    }

    [[gnu::always_inline]] inline u64x4 montgomery_mul(u64x4 x, u64x4 y, uint32_t mod, uint32_t imod) {
#ifdef __AVX2__
        return montgomery_reduce(u64x4(_mm256_mul_epu32(__m256i(x), __m256i(y))), mod, imod);
#else
        return montgomery_reduce(low32(x) * low32(y), mod, imod);
#endif
    }
    [[gnu::always_inline]] inline u32x8 montgomery_mul(u32x8 x, u32x8 y, uint32_t mod, uint32_t imod) {
        return u32x8(montgomery_mul(u64x4(x), u64x4(y), mod, imod)) |
               u32x8(swap_bytes(montgomery_mul(u64x4(swap_bytes(x)), u64x4(swap_bytes(y)), mod, imod)));
    }
    [[gnu::always_inline]] inline dx4 rotate_right(dx4 x) {
        static constexpr u64x4 shuffler = {3, 0, 1, 2};
        return __builtin_shuffle(x, shuffler);
    }

    template<std::size_t Align = 32>
    [[gnu::always_inline]] inline bool is_aligned(const auto* p) noexcept {
        return (reinterpret_cast<std::uintptr_t>(p) % Align) == 0;
    }

    template<class Target>
    [[gnu::always_inline]] inline Target& vector_cast(auto &&p) {
        return *reinterpret_cast<Target*>(std::assume_aligned<alignof(Target)>(&p));
    }
}

#line 1 "cp-algorithms-aux/cp-algo/util/bump_alloc.hpp"


#line 1 "cp-algorithms-aux/cp-algo/util/big_alloc.hpp"



#line 6 "cp-algorithms-aux/cp-algo/util/big_alloc.hpp"

// Single macro to detect POSIX platforms (Linux, Unix, macOS)
#if defined(__linux__) || defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#  define CP_ALGO_USE_MMAP 1
#  include <sys/mman.h>
#else
#  define CP_ALGO_USE_MMAP 0
#endif

namespace cp_algo {
    template <typename T, std::size_t Align = 32>
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
}

#line 5 "cp-algorithms-aux/cp-algo/util/bump_alloc.hpp"
namespace cp_algo {
    template<class T, size_t max_len>
    struct bump_alloc {
        static char* buf;
        static size_t buf_ind;
        using value_type = T;
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

#line 1 "cp-algorithms-aux/cp-algo/math/common.hpp"


#line 6 "cp-algorithms-aux/cp-algo/math/common.hpp"
namespace cp_algo::math {
#ifdef CP_ALGO_MAXN
    const int maxn = CP_ALGO_MAXN;
#else
    const int maxn = 1 << 19;
#endif
    const int magic = 64; // threshold for sizes to run the naive algo

    auto bpow(auto const& x, auto n, auto const& one, auto op) {
        if(n == 0) {
            return one;
        } else {
            auto t = bpow(x, n / 2, one, op);
            t = op(t, t);
            if(n % 2) {
                t = op(t, x);
            }
            return t;
        }
    }
    auto bpow(auto x, auto n, auto ans) {
        return bpow(x, n, ans, std::multiplies{});
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

#line 11 "cp-algorithms-aux/verify/simd/many_facts.test.cpp"

using namespace std;
using namespace cp_algo;

constexpr int mod = 998244353;
constexpr int imod = -math::inv2(mod);

template<int base, int mod>
int pow_fixed(int x) {
    static int prec_low[1 << 16];
    static int prec_high[1 << 16];
    static bool init = false;
    if(!init) {
        init = true;
        prec_low[0] = prec_high[0] = 1;
        uint64_t step_low = base;
        uint64_t step_high = math::bpow(base, 1ULL << 16, 1ULL, [](uint64_t a, uint64_t b) {
            return (a * b) % mod;
        });
        for(int i = 1; i < (1 << 16); i++) {
            prec_low[i] = int(prec_low[i - 1] * step_low % mod);
            prec_high[i] = int(prec_high[i - 1] * step_high % mod);
        }
    }
    return int(uint64_t(prec_low[x & 0xFFFF]) * prec_high[x >> 16] % mod);
}


vector<int> get_invs(vector<int> const& args) {
    vector<int> res(size(args), args[0]);
    for(int i = 1; i < size(args); i++) {
        res[i] = int(uint64_t(res[i - 1]) * args[i] % mod);
    }
    auto all_invs = math::bpow(res.back(), mod - 2, 1ULL, [](uint64_t a, uint64_t b) {
        return (a * b) % mod;
    });
    for(int i = size(args) - 1; i > 0; i--) {
        res[i] = int(uint64_t(all_invs) * res[i - 1] % mod);
        all_invs = int(uint64_t(all_invs) * args[i] % mod);
    }
    res[0] = all_invs;
    return res;
}

template<bool use_bump_alloc = true, int maxn = 100'000>
vector<int> facts(vector<int> const& args) {
    constexpr int accum = 4;
    constexpr int simd_size = 8;
    constexpr int block = 1 << 18;
    constexpr int subblock = block / simd_size;
    using T = array<int, 2>;
    using alloc = conditional_t<use_bump_alloc,
        bump_alloc<T, 30 * maxn>,
        allocator<T>>;
    basic_string<T, char_traits<T>, alloc> odd_args_per_block[mod / subblock];
    basic_string<T, char_traits<T>, alloc> reg_args_per_block[mod / subblock];
    constexpr int limit_reg = mod / 64;
    int limit_odd = 0;

    vector<int> res(size(args), 1);
    auto prod_mod = [&](uint64_t a, uint64_t b) {
        return (a * b) % mod;
    };
    vector<int64_t> pw2(size(args));
    for(auto [i, xy]: views::zip(args, res) | views::enumerate) {
        auto [x, y] = xy;
        auto t = x;
        if(t >= mod / 2) {
            t = mod - t - 1;
            y = t % 2 ? 1 : mod - 1;
        }
        pw2[i] = (t + 1) * 32ull;
        while(t > limit_reg) {
            limit_odd = max(limit_odd, (t - 1) / 2);
            odd_args_per_block[(t - 1) / 2 / subblock].push_back({int(i), (t - 1) / 2});
            t /= 2;
            pw2[i] += t;
        }
        reg_args_per_block[t / subblock].push_back({int(i), t});
        y = int(uint64_t(y) * pow_fixed<2, mod>(pw2[i] % (mod - 1)) % mod);
    }
    cp_algo::checkpoint("init");
    uint32_t b2x32 = (1ULL << 32) % mod;
    uint32_t bi2x32 = math::bpow(b2x32, mod - 2, 1ULL, prod_mod);
    auto process = [&](int limit, auto &args_per_block, auto step, auto &&proj) {
        uint64_t fact = 1;
        for(int b = 0; b <= limit; b += accum * block) {
            u32x8 cur[accum];
            static array<u32x8, subblock> prods[accum];
            for(int z = 0; z < accum; z++) {
                for(int j = 0; j < simd_size; j++) {
                    cur[z][j] = uint32_t(b + z * block + j * subblock);
                    cur[z][j] = proj(cur[z][j]);
                    prods[z][0][j] = cur[z][j] + !cur[z][j];
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
                    prods[z][0][j] = uint32_t(uint64_t(prods[z][0][j]) * bi2x32 % mod);
    #pragma GCC diagnostic pop
                }
            }
            for(int i = 1; i < block / simd_size; i++) {
                for(int z = 0; z < accum; z++) {
                    cur[z] += step;
                    prods[z][i] = montgomery_mul(prods[z][i - 1], cur[z], mod, imod);
                }
            }
            cp_algo::checkpoint("inner loop");
            for(int z = 0; z < accum; z++) {
                for(int j = 0; j < simd_size; j++) {
                    int bl = b + z * block + j * subblock;
                    for(auto [i, x]: args_per_block[bl / subblock]) {
                        auto ans = fact * prods[z][x - bl][j] % mod;
                        res[i] = int(res[i] * ans % mod);
                    }
                    fact = fact * prods[z].back()[j] % mod;
                }
            }
            cp_algo::checkpoint("mul ans");
        }
    };
    process(limit_reg, reg_args_per_block, 1, identity{});
    process(limit_odd, odd_args_per_block, 2, [](uint32_t x) {return 2 * x + 1;});
    auto invs = get_invs(res);
    cp_algo::checkpoint("get invs");
    for(auto [i, x]: res | views::enumerate) {
        if (args[i] >= mod / 2) {
            x = invs[i];
        }
    }
    cp_algo::checkpoint("inv ans");
    return res;
}

void solve() {
    int n;
    cin >> n;
    vector<int> args(n);
    for(auto &x : args) {cin >> x;}
    cp_algo::checkpoint("read");
    auto res = facts(args);
    for(auto it: res) {cout << it << "\n";}
    cp_algo::checkpoint("write");
    cp_algo::checkpoint<1>();
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
