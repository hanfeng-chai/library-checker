#include <bits/stdc++.h>

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
#include $a(<arm_neon.h>,<immintrin.h>)
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

#ifdef NANDHAGK_LOCAL
#include "/home/nandhagk/Projects/mallard/include/debug.h"
#else
#define debug(...)
#endif // NANDHAGK_LOCAL

    using i8 = std::int8_t;
using u8 = std::uint8_t;

using i16 = std::int16_t;
using u16 = std::uint16_t;

using i32 = std::int32_t;
using u32 = std::uint32_t;

using i64 = std::int64_t;
using u64 = std::uint64_t;

using i128 = __int128;
using u128 = unsigned __int128;

using isize = std::ptrdiff_t;
using usize = std::size_t;

using f32 = float;
using f64 = double;
using f80 = long double;

namespace mld {

template <std::unsigned_integral>
struct make_double_width {};

template <>
struct make_double_width<u8> {
    using type = u16;
};

template <>
struct make_double_width<u16> {
    using type = u32;
};

template <>
struct make_double_width<u32> {
    using type = u64;
};

template <>
struct make_double_width<u64> {
    using type = u128;
};

template <std::unsigned_integral U>
using make_double_width_t = typename make_double_width<U>::type;

} // namespace mld

namespace mld {
template <std::integral T>
[[nodiscard]] constexpr T floor(T a, T b) noexcept {
    return a / b - (a % b && (a ^ b) < 0);
}

template <std::integral T>
[[nodiscard]] constexpr T ceil(T a, T b) noexcept {
    return floor(a + b - 1, a);
}

template <typename T, std::unsigned_integral Scalar,
          std::invocable<T, T> F = std::multiplies<>>
[[nodiscard]] constexpr T pow(T a, Scalar n, F mul = F(),
                              T one = static_cast<T>(1)) noexcept {
    T r = one;
    for (; n != 0; n >>= 1, a = mul(a, a))
        if (n & 1) r = mul(r, a);

    return r;
}

template <typename T>
[[nodiscard]] constexpr T gcd(T a, T b, T zero = static_cast<T>(0)) noexcept {
    for (; b != zero; a %= b, std::swap(a, b));
    return a;
}

template <typename T>
[[nodiscard]] constexpr T lcm(T a, T b, T zero = static_cast<T>(0)) noexcept {
    return a / gcd(a, b, zero) * b;
}

template <std::integral T, std::unsigned_integral U>
[[nodiscard]] constexpr U safe_mod(T x, U m) noexcept {
    x %= m;
    return static_cast<U>(x < 0 ? x + m : x);
}

template <std::unsigned_integral U>
[[nodiscard]] constexpr std::pair<U, U> inv_gcd(U a, U m) noexcept {
    assert(a < m);
    if (a == 0) return {m, 0};

    using T = std::make_signed_t<U>;

    U s = m, t = a;
    T x = 0, y = 1;
    for (; t != 0; std::swap(s, t), std::swap(x, y)) {
        U u = s / t;
        s -= t * u;
        x -= y * u;
    }

    if (x < 0) x += m / s;
    return {s, static_cast<U>(x)};
}

template <std::unsigned_integral U>
[[nodiscard]] constexpr U mod_inv(U a, U m) noexcept {
    const auto [x, y] = inv_gcd(a, m);
    assert(x == 1);

    return y;
}

template <std::unsigned_integral U>
[[nodiscard]] constexpr U mod_mul(U a, U b, U m) noexcept {
    return static_cast<U>(make_double_width_t<U>(a) * b % m);
}

template <std::unsigned_integral U, std::unsigned_integral Scalar>
[[nodiscard]] constexpr U mod_pow(U a, Scalar n, U m) noexcept {
    assert(a < m);
    return pow(a, n, [m](U x, U y) { return mod_mul(x, y, m); }, U{1} % m);
}
} // namespace mld

namespace mld::algebra {

template <typename T>
struct base {
    using value_type = T;

protected:
    value_type v;

public:
    template <typename... Args>
        requires std::constructible_from<value_type, Args...>
    base(Args &&...args) noexcept
        : v(std::forward<Args>(args)...) {}

    [[nodiscard]] constexpr auto val() const noexcept {
        return v;
    }

    [[nodiscard]] explicit constexpr operator value_type() const noexcept {
        return val();
    }

    [[nodiscard]] friend constexpr auto operator<=>(const base &lhs,
                                                    const base &rhs) noexcept {
        return lhs.val() <=> rhs.val();
    }

    [[nodiscard]] friend constexpr bool operator==(const base &lhs,
                                                   const base &rhs) noexcept {
        return lhs.val() == rhs.val();
    }
};

struct associative {};

template <typename Derived>
struct commutative {
    [[nodiscard]] friend constexpr Derived operator+(const Derived &lhs) noexcept {
        return lhs;
    }
};

template <typename Derived>
struct scalar_multipliable {
    struct identity {
        template <std::unsigned_integral Scalar>
        [[nodiscard]] friend constexpr Derived operator*(const Derived &lhs,
                                                         Scalar) noexcept {
            return lhs;
        }
    };

    struct automatic {
        template <std::unsigned_integral Scalar>
        [[nodiscard]] friend constexpr Derived operator*(const Derived &lhs,
                                                         Scalar n) noexcept {
            return pow<Derived, Scalar, std::plus<Derived>>(lhs, n, {}, {});
        }
    };
};

template <typename Derived>
struct idempotent : scalar_multipliable<Derived>::identity {};

template <typename Derived>
struct truthy {
    [[nodiscard]] explicit constexpr operator bool() const noexcept {
        return *static_cast<const Derived *>(this) != Derived();
    }
};

} // namespace mld::algebra

namespace mld::algebra {
template <typename T>
struct sum : base<T>, truthy<sum<T>>, associative, commutative<sum<T>> {
    using operand = base<T>;

    using operand::operand;
    using operand::val;

    [[nodiscard]] friend constexpr sum operator+(const sum &lhs,
                                                 const sum &rhs) noexcept {
        return lhs.val() + rhs.val();
    }

    template <std::unsigned_integral Scalar>
    [[nodiscard]] friend constexpr sum operator*(const sum &lhs, Scalar n) noexcept {
        return lhs.val() * n;
    }

    [[nodiscard]] constexpr sum operator-() const noexcept {
        return -val();
    }
};
} // namespace mld::algebra

namespace mld::internal {
// To prevent duplicate base type
template <int>
struct dummy {};
} // namespace mld::internal

namespace mld::algebra::internal {

template <typename T>
concept magma = requires { typename T::value_type; } && requires(T lhs, T rhs) {
    { lhs.val() } -> std::convertible_to<typename T::value_type>;
    { lhs + rhs } -> std::convertible_to<T>;
};

template <typename T>
concept associative = std::derived_from<T, algebra::associative>;

template <typename T>
concept commutative = std::derived_from<T, algebra::commutative<T>>;

template <typename T>
concept idempotent = std::derived_from<T, algebra::idempotent<T>>;

template <typename T>
concept semigroup = magma<T> && associative<T>;

template <typename T>
concept invertible = requires(T lhs) {
    { -lhs } -> std::convertible_to<T>;
};

template <typename T>
concept reversible = requires(T lhs) {
    { +lhs } -> std::convertible_to<T>;
};

template <typename T>
concept monoid = semigroup<T> && std::default_initializable<T>;

template <typename T>
concept abelian_monoid = monoid<T> && commutative<T>;

template <typename T>
concept group = monoid<T> && invertible<T>;

template <typename T>
concept abelian_group = group<T> && commutative<T>;

template <typename Aggregate>
concept foldable =
    requires {
        typename Aggregate::operand;
        typename Aggregate::size_type;
    } && monoid<typename Aggregate::operand> &&
    std::unsigned_integral<typename Aggregate::size_type> &&
    requires(Aggregate agg, Aggregate::size_type l, Aggregate::size_type r) {
        { agg.fold(l, r) } -> std::same_as<typename Aggregate::operand>;
    };

template <typename Aggregate>
concept appliable =
    requires {
        typename Aggregate::operation;
        typename Aggregate::size_type;
    } && monoid<typename Aggregate::operation> &&
    std::unsigned_integral<typename Aggregate::size_type> &&
    requires(Aggregate agg, Aggregate::size_type l, Aggregate::size_type r,
             Aggregate::operation f) {
        { agg.apply(l, r, f) } -> std::same_as<void>;
    };

template <typename T>
struct operation_type {
    using type = mld::internal::dummy<1>;
};

template <typename T>
    requires appliable<T>
struct operation_type<T> {
    using type = typename T::operation;
};

template <typename T>
using operation_type_t = operation_type<T>::type;

template <typename Aggregate, typename R>
concept buildable = requires(Aggregate agg, R r) {
    { agg.build(r) } -> std::same_as<void>;
};

} // namespace mld::algebra::internal

namespace mld::internal {

template <typename V, typename R>
concept sized_range_of = std::ranges::sized_range<R> &&
                         std::convertible_to<std::ranges::range_value_t<R>, V>;

template <typename V, typename R>
concept sized_input_range_of = sized_range_of<V, R> && std::ranges::input_range<R>;
} // namespace mld::internal

namespace mld::internal {

template <typename T, algebra::internal::abelian_group Group, typename handler>
struct foldable_am_tree {
    using size_type = u32;
    using cost_type = T;
    using operand = Group;

    using pointer = handler::pointer;

    struct node {
        pointer p = handler::nil;
        size_type len;
        cost_type cst;
        operand val;

        constexpr node() noexcept = default;

        explicit constexpr node(const operand &x) noexcept
            : len(1), val(x) {}
    };

    using result = std::optional<std::optional<cost_type>>;

    [[nodiscard]] static constexpr result insert(pointer u, pointer v,
                                                 cost_type w) noexcept {
        if (u == v) return std::nullopt;
        return balance(u), balance(v), link(u, v, w);
    }

    [[nodiscard]] static constexpr bool is_connected(pointer u, pointer v) noexcept {
        if (u == v) return true;
        for (balance(u), balance(v);;) {
            if (u->len > v->len) std::swap(u, v);
            if (u = u->p; u == handler::nil) return false;
            if (u == v) return true;
        }
    }

    [[nodiscard]] static constexpr std::optional<cost_type>
    max_path(pointer u, pointer v) noexcept {
        if (u == v) return std::nullopt;

        balance(u), balance(v);
        for (std::optional<cost_type> res = std::nullopt;;) {
            if (u->len > v->len) std::swap(u, v);
            if (!res || u->cst > *res) res = u->cst;

            if (u = u->p; u == handler::nil) return std::nullopt;
            if (u == v) return res;
        }
    }

    [[nodiscard]] static constexpr result erase(pointer u, pointer v,
                                                cost_type w) noexcept {
        if (u == v) return std::nullopt;
        return balance(u), balance(v), cut_max_path(u, v, w);
    }

    [[nodiscard]] static constexpr pointer root(pointer u) noexcept {
        for (balance(u); u->p != handler::nil; u = u->p);
        return u;
    }

    static constexpr void add(pointer u, const operand &x) noexcept {
        for (balance(u); u->p != handler::nil; u = u->p) u->val = u->val + x;
        u->val = u->val + x;
    }

private:
    static constexpr void promote(pointer u) noexcept {
        if (pointer p = u->p; u->cst >= p->cst && p->p != handler::nil) {
            u->p = p->p;
            p->len -= u->len, p->val = p->val + (-u->val);
        } else {
            u->p = std::exchange(p->p, u);
            std::swap(u->cst, p->cst);
            p->len -= u->len, p->val = p->val + (-u->val);
            u->len += p->len, u->val = u->val + p->val;
        }
    }

    [[nodiscard]] static constexpr result cut_max_path(pointer u, pointer v,
                                                       cost_type w) noexcept {
        assert(u != v);

        for (pointer t = handler::nil;;) {
            if (u->len > v->len) std::swap(u, v);

            pointer p = u->p;
            if (p == handler::nil) return std::optional<cost_type>(std::nullopt);

            if (t == handler::nil || u->cst > t->cst) t = u;
            if (u = p; u == v) {
                if (w >= t->cst) return std::nullopt;

                for (pointer s = t->p; s != handler::nil;
                     s->len -= t->len, s->val = s->val + (-t->val), s = s->p);
                t->p = handler::nil;

                return std::optional<cost_type>(t->cst);
            }
        }
    }

    [[nodiscard]] static constexpr result link(pointer u, pointer v,
                                               cost_type w) noexcept {
        assert(u != v);

        result res = cut_max_path(u, v, w);
        if (!res) return std::nullopt;

        operand x, y;
        for (size_type a = 0, b = 0;;) {
            for (; u->p != handler::nil && w >= u->cst;
                 u = u->p, u->len += a, u->val = u->val + x);
            for (; v->p != handler::nil && w >= v->cst;
                 v = v->p, v->len += b, v->val = v->val + y);
            if (u->len > v->len) {
                std::swap(u, v);
                std::swap(a, b), std::swap(x, y);
            }

            a -= u->len, x = x + (-u->val);
            b += u->len, y = y + u->val;
            v->len += u->len, v->val = v->val + u->val;
            std::swap(u->cst, w);

            if (u = std::exchange(u->p, v); u == handler::nil) {
                for (v = v->p; v != handler::nil;
                     v->len += b, v->val = v->val + y, v = v->p);
                return res;
            }

            u->len += a, u->val = u->val + x;
        }
    }

    static constexpr void balance(pointer u) noexcept {
        for (pointer p = u->p; p != handler::nil; p = u->p)
            if (u->len * 3 / 2 > p->len)
                promote(u);
            else
                u = p;
    }
};
} // namespace mld::internal

namespace mld::managers::container {
template <typename Container>
struct handler {
    using container_type = Container;

    using node = container_type::value_type;
    using pointer = container_type::pointer;

    inline static pointer nil;

private:
    inline static usize instance_count = 0;

public:
    constexpr handler() noexcept {
        if (instance_count++ == 0) nil = new node{};
    }

    constexpr ~handler() noexcept {
        if (--instance_count == 0) delete nil;
    }
};
} // namespace mld::managers::container

namespace mld {

template <typename T, algebra::internal::abelian_group Group>
struct foldable_am_tree {
private:
    struct node;
    using container_type = std::vector<node>;

    using handler = managers::container::handler<container_type>;
    using tree = internal::foldable_am_tree<T, Group, handler>;

public:
    using size_type = tree::size_type;
    using cost_type = tree::cost_type;
    using operand = tree::operand;
    using result = tree::result;

private:
    struct node : tree::node {
        using base = tree::node;
        using base::base;
    };

    [[no_unique_address]] handler hlr;
    size_type len, cnt;
    container_type buf;

public:
    constexpr foldable_am_tree() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    explicit constexpr foldable_am_tree(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        cnt = len = static_cast<size_type>(std::ranges::size(r));
        buf.clear();

        buf.reserve(len);
        for (auto &&a : r) buf.emplace_back(a);
    }

    constexpr result insert(size_type u, size_type v, cost_type w) noexcept {
        assert(u < len && v < len);

        auto res = tree::insert(&buf[u], &buf[v], w);
        cnt -= (res && !res->has_value());

        return res;
    }

    [[nodiscard]] constexpr bool is_connected(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        return tree::is_connected(&buf[u], &buf[v]);
    }

    constexpr void add(size_type u, const operand &x) noexcept {
        assert(u < len);
        tree::add(&buf[u], x);
    }

    [[nodiscard]] constexpr operand fold(size_type u) noexcept {
        assert(u < len);
        return tree::root(&buf[u])->val;
    }

    [[nodiscard]] constexpr size_type size(size_type u) noexcept {
        assert(u < len);
        return tree::root(&buf[u])->len;
    }

    [[nodiscard]] constexpr std::optional<cost_type> max_path(size_type u,
                                                              size_type v) noexcept {
        assert(u < len && v < len);
        return tree::max_path(&buf[u], &buf[v]);
    }

    constexpr result erase(size_type u, size_type v, cost_type w) noexcept {
        assert(u < len && v < len);

        auto res = tree::erase(&buf[u], &buf[v], w);
        cnt += res.has_value();

        return res;
    }

    [[nodiscard]] constexpr size_type ccs() const noexcept {
        return cnt;
    }
};

} // namespace mld

namespace mld::internal {

template <typename T, typename handler>
struct am_tree {
    using size_type = u32;
    using cost_type = T;

    using pointer = handler::pointer;

    struct node {
        pointer p = handler::nil;
        size_type len;
        cost_type cst;

        constexpr node() noexcept = default;

        explicit constexpr node(bool) noexcept
            : len(1) {}
    };

    using result = std::optional<std::optional<cost_type>>;

    [[nodiscard]] static constexpr result insert(pointer u, pointer v,
                                                 cost_type w) noexcept {
        if (u == v) return std::nullopt;
        return balance(u), balance(v), link(u, v, w);
    }

    [[nodiscard]] static constexpr bool is_connected(pointer u, pointer v) noexcept {
        if (u == v) return true;
        for (balance(u), balance(v);;) {
            if (u->len > v->len) std::swap(u, v);
            if (u = u->p; u == handler::nil) return false;
            if (u == v) return true;
        }
    }

    [[nodiscard]] static constexpr std::optional<cost_type>
    max_path(pointer u, pointer v) noexcept {
        if (u == v) return std::nullopt;

        balance(u), balance(v);
        for (std::optional<cost_type> res = std::nullopt;;) {
            if (u->len > v->len) std::swap(u, v);
            if (!res || u->cst > *res) res = u->cst;

            if (u = u->p; u == handler::nil) return std::nullopt;
            if (u == v) return res;
        }
    }

    [[nodiscard]] static constexpr result erase(pointer u, pointer v,
                                                cost_type w) noexcept {
        if (u == v) return std::nullopt;
        return balance(u), balance(v), cut_max_path(u, v, w);
    }

    [[nodiscard]] static constexpr pointer root(pointer u) noexcept {
        for (balance(u); u->p != handler::nil; u = u->p);
        return u;
    }

private:
    static constexpr void promote(pointer u) noexcept {
        if (pointer p = u->p; u->cst >= p->cst && p->p != handler::nil) {
            u->p = p->p;
            p->len -= u->len;
        } else {
            u->p = std::exchange(p->p, u);
            std::swap(u->cst, p->cst);
            p->len -= u->len;
            u->len += p->len;
        }
    }

    [[nodiscard]] static constexpr result cut_max_path(pointer u, pointer v,
                                                       cost_type w) noexcept {
        assert(u != v);

        for (pointer t = handler::nil;;) {
            if (u->len > v->len) std::swap(u, v);

            pointer p = u->p;
            if (p == handler::nil) return std::optional<cost_type>(std::nullopt);

            if (t == handler::nil || u->cst > t->cst) t = u;
            if (u = p; u == v) {
                if (w >= t->cst) return std::nullopt;

                for (pointer s = t->p; s != handler::nil; s->len -= t->len, s = s->p);
                t->p = handler::nil;

                return std::optional<cost_type>(t->cst);
            }
        }
    }

    [[nodiscard]] static constexpr result link(pointer u, pointer v,
                                               cost_type w) noexcept {
        assert(u != v);

        result res = cut_max_path(u, v, w);
        if (!res) return std::nullopt;

        for (size_type a = 0, b = 0;;) {
            for (; u->p != handler::nil && w >= u->cst; u = u->p, u->len += a);
            for (; v->p != handler::nil && w >= v->cst; v = v->p, v->len += b);
            if (u->len > v->len) {
                std::swap(u, v);
                std::swap(a, b);
            }

            a -= u->len;
            b += u->len;
            v->len += u->len;
            std::swap(u->cst, w);

            if (u = std::exchange(u->p, v); u == handler::nil) {
                for (v = v->p; v != handler::nil; v->len += b, v = v->p);
                return res;
            }

            u->len += a;
        }
    }

    static constexpr void balance(pointer u) noexcept {
        for (pointer p = u->p; p != handler::nil; p = u->p)
            if (u->len * 3 / 2 > p->len)
                promote(u);
            else
                u = p;
    }
};
} // namespace mld::internal

namespace mld {

template <typename T>
struct am_tree {
private:
    struct node;
    using container_type = std::vector<node>;

    using handler = managers::container::handler<container_type>;
    using tree = internal::am_tree<T, handler>;

public:
    using size_type = tree::size_type;
    using cost_type = tree::cost_type;
    using result = tree::result;

private:
    struct node : tree::node {
        using base = tree::node;
        using base::base;
    };

    [[no_unique_address]] handler hlr;
    size_type len, cnt;
    container_type buf;

public:
    constexpr am_tree() noexcept = default;

    explicit constexpr am_tree(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        cnt = len = n;
        buf.assign(len, node(true));
    }

    constexpr result insert(size_type u, size_type v, cost_type w) noexcept {
        assert(u < len && v < len);

        auto res = tree::insert(&buf[u], &buf[v], w);
        cnt -= (res && !res->has_value());

        return res;
    }

    [[nodiscard]] constexpr bool is_connected(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        return tree::is_connected(&buf[u], &buf[v]);
    }

    [[nodiscard]] constexpr size_type size(size_type u) noexcept {
        assert(u < len);
        return tree::root(&buf[u])->len;
    }

    [[nodiscard]] constexpr std::optional<cost_type> max_path(size_type u,
                                                              size_type v) noexcept {
        assert(u < len && v < len);
        return tree::max_path(&buf[u], &buf[v]);
    }

    constexpr result erase(size_type u, size_type v, cost_type w) noexcept {
        assert(u < len && v < len);

        auto res = tree::erase(&buf[u], &buf[v], w);
        cnt += res.has_value();

        return res;
    }

    [[nodiscard]] constexpr size_type ccs() const noexcept {
        return cnt;
    }
};

} // namespace mld

#include <ext/pb_ds/assoc_container.hpp>

namespace mld {
static const u64 SEED = std::chrono::steady_clock::now().time_since_epoch().count();
static std::mt19937_64 MT(SEED);
} // namespace mld

namespace mld {
template <typename T>
struct hash {};

template <typename T>
constexpr void hash_combine(u64 &seed, const T &v) noexcept {
    hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b97f4a7c15 + (seed << 12) + (seed >> 4);
};

template <std::integral T>
struct hash<T> {
    [[nodiscard]] u64 operator()(T y) const noexcept {
        u64 x = y;
        x += 0x9e3779b97f4a7c15 + SEED;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
};

template <std::ranges::range T>
struct hash<T> {
    [[nodiscard]] u64 operator()(const T &a) const noexcept {
        u64 value = SEED;
        for (auto &&x : a) hash_combine(value, x);
        return value;
    }
};

template <typename... T>
struct hash<std::tuple<T...>> {
    [[nodiscard]] u64 operator()(const std::tuple<T...> &a) const noexcept {
        u64 value = SEED;
        std::apply([&value](T &&...args) { (hash_combine(value, args), ...); }, a);
        return value;
    }
};

template <typename T, typename U>
struct hash<std::pair<T, U>> {
    [[nodiscard]] u64 operator()(const std::pair<T, U> &a) const noexcept {
        u64 value = SEED;
        hash_combine(value, a.first);
        hash_combine(value, a.second);
        return value;
    }
};
} // namespace mld

namespace mld::pbds {
using namespace __gnu_pbds;

template <typename Key, typename Value, typename Hash = mld::hash<Key>>
using hash_map =
    gp_hash_table<Key, Value, Hash, std::equal_to<Key>, direct_mask_range_hashing<>,
                  linear_probe_fn<>,
                  hash_standard_resize_policy<hash_exponential_size_policy<>,
                                              hash_load_check_resize_trigger<>, true>>;

template <typename Key, typename Hash = mld::hash<Key>>
using hash_set = hash_map<Key, null_type, Hash>;

} // namespace mld::pbds

namespace mld::offline {
template <typename AMTree>
struct dynamic_connectivity {
    using tree = AMTree;

    using size_type = tree::size_type;
    using cost_type = tree::cost_type;
    static_assert(std::same_as<cost_type, i32>);

    using query_type = std::function<void(tree &, size_type)>;

private:
    size_type len;
    std::vector<std::tuple<size_type, size_type, cost_type>> e;
    pbds::hash_map<std::pair<size_type, size_type>, size_type> f;
    std::vector<std::pair<size_type, query_type>> q;

public:
    dynamic_connectivity() noexcept = default;

    explicit dynamic_connectivity(size_type n) noexcept {
        build(n);
    }

    void build(size_type n) noexcept {
        len = n;
        e.clear(), f.clear(), q.clear();
    }

    void reserve(size_type m) noexcept {
        e.reserve(m);
        f.resize(m);
        q.reserve(m);
    }

    void link(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        if (u > v) std::swap(u, v);

        f[{u, v}] = static_cast<size_type>(e.size());
        e.emplace_back(u, v, std::numeric_limits<cost_type>::lowest());
    };

    void cut(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        if (u > v) std::swap(u, v);

        std::get<2>(e[f[{u, v}]]) = -static_cast<cost_type>(e.size());
        e.emplace_back(u, v, 1);
    };

    template <typename F>
        requires std::convertible_to<F, query_type>
    constexpr void query(F &&z) noexcept {
        q.emplace_back(static_cast<size_type>(e.size()), std::forward<F>(z));
    }

    constexpr void solve(tree &amt) noexcept {
        size_type k = 0;

        for (size_type i = 0; i < e.size(); ++i) {
            for (; k < q.size() && q[k].first == i; ++k) q[k].second(amt, k);

            auto &&[u, v, w] = e[i];
            if (w == 1)
                amt.erase(u, v, -static_cast<cost_type>(i + 1));
            else
                amt.insert(u, v, w);
        }

        for (; k < q.size(); ++k) q[k].second(amt, k);
    }
};

} // namespace mld::offline

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::foldable_am_tree<i32, mld::algebra::sum<u64>> amt(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) {
            u32 a;
            std::cin >> a;
            return a;
        }));

    mld::offline::dynamic_connectivity<decltype(amt)> dct(n);
    dct.reserve(n + 2 * q);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        dct.link(u, v);
    }

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 u, v, w, x;
            std::cin >> u >> v >> w >> x;

            dct.cut(u, v);
            dct.link(w, x);
        } else if (t == 1) {
            u32 u, x;
            std::cin >> u >> x;

            dct.query([u, x](auto &&am, u32) { am.add(u, x); });
        } else {
            u32 u, p;
            std::cin >> u >> p;

            dct.cut(u, p);
            dct.query([u](auto &&am, u32) { std::cout << am.fold(u).val() << '\n'; });
            dct.link(u, p);
        }
    }

    dct.solve(amt);
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
