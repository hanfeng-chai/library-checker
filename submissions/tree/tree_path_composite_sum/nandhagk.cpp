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

struct rooted_tree {
    using size_type = u32;

protected:
    size_type len;

public:
    static constexpr size_type UNSET = -1;
    std::vector<size_type> siz, tin, dep, par, tor;

    constexpr rooted_tree() noexcept = default;

    template <typename Tree>
    explicit constexpr rooted_tree(const Tree &g) noexcept
        : rooted_tree(g, 0) {}

    template <typename Tree>
    constexpr rooted_tree(const Tree &g, size_type root) noexcept {
        build(g, root);
    }

    template <typename Tree>
    constexpr void build(const Tree &g, size_type root = 0) noexcept {
        len = static_cast<size_type>(g.size());

        siz.assign(len, 1);
        dep.resize(len);
        par.resize(len);

        par[root] = UNSET;
        dep[root] = 0;

        tor.clear(), tor.reserve(len);
        tor.push_back(root);

        for (size_type i = 0; i < len; ++i) {
            size_type u = tor[i];
            for (size_type v : g[u]) {
                if (v == par[u]) continue;
                par[v] = u;
                dep[v] = dep[u] + 1;
                tor.push_back(v);
            }
        }

        for (size_type u : tor | std::views::drop(1) | std::views::reverse)
            siz[par[u]] += siz[u];

        tin = siz;
        for (size_type u : tor | std::views::drop(1))
            tin[u] = std::exchange(tin[par[u]], tin[par[u]] - tin[u]);

        for (size_type u = 0; u < len; ++u) tor[--tin[u]] = u;
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return len;
    }

    [[nodiscard]] constexpr bool is_ancestor(size_type u, size_type v) const noexcept {
        assert(u < len && v < len);
        return tin[u] <= tin[v] && tin[u] + siz[u] > tin[v];
    }
};
} // namespace mld

namespace mld {

template <typename R>
concept static_csr_input_range =
    std::ranges::input_range<R> && std::ranges::sized_range<R> &&
    std::unsigned_integral<std::remove_cvref_t<std::ranges::range_value_t<R>>>;

template <typename T>
struct static_csr {
    using value_type = T;

    using size_type = u32;
    using container_type = std::vector<value_type>;

protected:
    size_type len;
    container_type buf;
    std::vector<size_type> fst;

public:
    constexpr static_csr() noexcept = default;

    template <static_csr_input_range R>
    explicit constexpr static_csr(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    constexpr static_csr(
        size_type n, const std::vector<std::pair<size_type, value_type>> &e) noexcept {
        build(n, e);
    }

    template <static_csr_input_range R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        fst.resize(len + 1);

        std::inclusive_scan(
            std::ranges::begin(r), std::ranges::end(r), fst.begin() + 1,
            [](size_type a, size_type b) { return a + b; }, size_type{0});

        buf.clear(), buf.resize(fst.back());
    }

    // TODO: Make this better
    constexpr void
    build(size_type n,
          const std::vector<std::pair<size_type, value_type>> &e) noexcept {
        len = n;
        fst.assign(len + 1, 0);

        for (auto &&[k, _] : e) ++fst[k];
        std::inclusive_scan(fst.begin(), fst.end(), fst.begin());
        fst.back() = static_cast<size_type>(e.size());

        buf.resize(fst.back());
        for (auto &&[k, d] : e) buf[--fst[k]] = d;
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return len;
    }

    [[nodiscard]] constexpr auto data() noexcept {
        return buf;
    }

    [[nodiscard]] constexpr auto data() const noexcept {
        return buf;
    }

    [[nodiscard]] constexpr std::span<value_type> operator[](size_type k) noexcept {
        return {buf.data() + fst[k], buf.data() + fst[k + 1]};
    }

    [[nodiscard]] constexpr std::span<const value_type>
    operator[](size_type k) const noexcept {
        return {buf.data() + fst[k], buf.data() + fst[k + 1]};
    }

    struct iterator {
    private:
        size_type k;
        const static_csr &g;

    public:
        constexpr iterator(size_type j, const static_csr &h)
            : k(j), g(h) {}

        [[nodiscard]] constexpr auto operator*() const noexcept {
            return g.operator[](k);
        }

        constexpr iterator &operator++() & noexcept {
            return ++k, *this;
        }

        [[nodiscard]] friend constexpr bool operator==(const iterator &lhs,
                                                       const iterator &rhs) noexcept {
            return lhs.k == rhs.k;
        }
    };

    [[nodiscard]] constexpr iterator begin() const noexcept {
        return iterator(0, *this);
    }

    [[nodiscard]] constexpr iterator end() const noexcept {
        return iterator(len, *this);
    }
};
} // namespace mld

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

namespace mld {
template <std::unsigned_integral U, i32 id>
struct dynamic_montgomery_modint_base {
    using mint = dynamic_montgomery_modint_base;

    using V = make_double_width_t<U>;
    using S = std::make_signed_t<U>;

    static constexpr u32 W = std::numeric_limits<U>::digits;

    dynamic_montgomery_modint_base() noexcept
        : v(0) {}

    template <std::integral T>
    dynamic_montgomery_modint_base(T x) noexcept
        : v(reduce(static_cast<V>(x % m + m) * n2)) {}

    static U reduce(V b) noexcept {
        return static_cast<U>(
            (b + static_cast<V>(static_cast<U>(b) * static_cast<U>(-r)) * m) >> W);
    }

    static U get_r() noexcept {
        U p = m;
        for (; m * p != 1; p *= static_cast<U>(2) - m * p);
        return p;
    }

    static void set_mod(U mod) noexcept {
        assert(mod & 1 && mod <= static_cast<U>(1) << (W - 2));

        m = mod;
        n2 = static_cast<U>(-static_cast<V>(m) % m);
        r = get_r();
    }

    [[nodiscard]] static U mod() noexcept {
        return m;
    }

    [[nodiscard]] U val() const noexcept {
        U p = reduce(v);
        return p >= m ? p - m : p;
    }

    [[nodiscard]] mint inv() const noexcept {
        return mld::mod_inv(val(), mod());
    }

    template <std::unsigned_integral Scalar>
    [[nodiscard]] mint pow(Scalar n) const noexcept {
        return mld::pow(*this, n);
    }

    mint &operator+=(const mint &rhs) & noexcept {
        if (static_cast<S>(v += rhs.v - 2 * m) < 0) v += 2 * m;
        return *this;
    }

    mint &operator-=(const mint &rhs) & noexcept {
        if (static_cast<S>(v -= rhs.v) < 0) v += 2 * m;
        return *this;
    }

    mint &operator*=(const mint &rhs) & noexcept {
        v = reduce(static_cast<V>(v) * rhs.v);
        return *this;
    }

    mint &operator/=(const mint &rhs) & noexcept {
        return *this *= rhs.inv();
    }

    [[nodiscard]] friend mint operator+(mint lhs, const mint &rhs) noexcept {
        return lhs += rhs;
    }

    [[nodiscard]] friend mint operator-(mint lhs, const mint &rhs) noexcept {
        return lhs -= rhs;
    }

    [[nodiscard]] friend mint operator*(mint lhs, const mint &rhs) noexcept {
        return lhs *= rhs;
    }

    [[nodiscard]] friend mint operator/(mint lhs, const mint &rhs) noexcept {
        return lhs /= rhs;
    }

    [[nodiscard]] mint operator-() const noexcept {
        return mint(0) - mint(*this);
    }

    [[nodiscard]] friend bool operator==(const mint &lhs, const mint &rhs) noexcept {
        return (lhs.v >= m ? lhs.v - m : lhs.v) == (rhs.v >= m ? rhs.v - m : rhs.v);
    }

    [[nodiscard]] friend bool operator!=(const mint &lhs, const mint &rhs) noexcept {
        return !(lhs == rhs);
    }

private:
    U v;
    inline static U m, r, n2;
};

template <i32 id>
using dynamic_montgomery_modint_32 = dynamic_montgomery_modint_base<u32, id>;

template <i32 id>
using dynamic_montgomery_modint_64 = dynamic_montgomery_modint_base<u64, id>;
} // namespace mld

namespace mld {

template <std::unsigned_integral>
constexpr std::initializer_list<u64> miller_rabin_bases = {};

template <>
constexpr std::initializer_list<u64> miller_rabin_bases<u8> = {2};

template <>
constexpr std::initializer_list<u64> miller_rabin_bases<u16> = {2, 3};

template <>
constexpr std::initializer_list<u64> miller_rabin_bases<u32> = {2, 7, 61};

template <>
constexpr std::initializer_list<u64> miller_rabin_bases<u64> = {
    2, 325, 9375, 28178, 450775, 9780504, 1795265022};

template <typename R>
concept miller_rabin_input_range =
    std::ranges::input_range<R> &&
    std::unsigned_integral<std::remove_cvref_t<std::ranges::range_value_t<R>>>;

template <std::unsigned_integral U, miller_rabin_input_range R>
[[nodiscard]] constexpr bool miller_rabin(U n, R &&r) noexcept {
    if (n < 64) return (u64{1} << n) & 0x28208a20a08a28ac;
    if (n % 2 == 0) return false;

    if (const U d = (n - 1) >> std::countr_zero(n - 1); std::is_constant_evaluated()) {
        return std::ranges::all_of(r, [&](auto &&a) {
            U b = static_cast<U>(a % n);
            if (b == 0) return true;

            U t = d;
            U y = mod_pow(b, d, n);
            for (; t != n - 1 && y != 1 && y != n - 1; y = mod_mul(y, y, n), t <<= 1);

            return (y == n - 1) || (t % 2);
        });
    } else {
        using Z = dynamic_montgomery_modint_base<U, -1>;
        Z::set_mod(n);

        return std::ranges::all_of(r, [&](auto &&a) {
            Z b = a;
            if (b == 0) return true;

            U t = d;
            Z y = b.pow(d);
            for (; t != n - 1 && y != 1 && y != n - 1; y *= y, t <<= 1);

            return (y == n - 1) || (t % 2);
        });
    }
}

template <std::unsigned_integral U>
[[nodiscard]] constexpr bool miller_rabin(U n) noexcept {
    return miller_rabin(n, miller_rabin_bases<U>);
}

template <std::unsigned_integral U, U m>
constexpr bool is_prime_v = miller_rabin(m);

static_assert(is_prime_v<u32, 998'244'353>);
static_assert(is_prime_v<u32, 1'000'000'007>);

} // namespace mld

namespace mld {
template <std::unsigned_integral U, U m>
struct static_modint_base {
    using mint = static_modint_base;

    constexpr static_modint_base() noexcept
        : v(0) {}

    template <std::unsigned_integral V>
    constexpr static_modint_base(V x) noexcept
        : v(static_cast<U>(x % mod())) {}

    template <std::signed_integral T>
    constexpr static_modint_base(T x) noexcept
        : v(safe_mod(x, mod())) {}

    [[nodiscard]] static constexpr U mod() noexcept {
        return m;
    }

    [[nodiscard]] constexpr U val() const noexcept {
        return v;
    }

    template <std::unsigned_integral Scalar>
    [[nodiscard]] constexpr mint pow(Scalar n) const noexcept {
        return mld::pow(*this, n);
    }

    [[nodiscard]] constexpr mint inv() const noexcept {
        if constexpr (is_prime)
            return pow(mod() - 2);
        else
            return mld::mod_inv(val(), mod());
    }

    constexpr mint &operator+=(const mint &rhs) & noexcept {
        if ((v += rhs.val()) >= mod()) v -= mod();
        return *this;
    }

    constexpr mint &operator-=(const mint &rhs) & noexcept {
        if ((v += mod() - rhs.val()) >= mod()) v -= mod();
        return *this;
    }

    constexpr mint &operator*=(const mint &rhs) & noexcept {
        v = mod_mul(v, rhs.val(), mod());
        return *this;
    }

    constexpr mint &operator/=(const mint &rhs) & noexcept {
        return *this *= rhs.inv();
    }

    [[nodiscard]] friend constexpr mint operator+(mint lhs, const mint &rhs) noexcept {
        return lhs += rhs;
    }

    [[nodiscard]] friend constexpr mint operator-(mint lhs, const mint &rhs) noexcept {
        return lhs -= rhs;
    }

    [[nodiscard]] friend constexpr mint operator*(mint lhs, const mint &rhs) noexcept {
        return lhs *= rhs;
    }

    [[nodiscard]] friend constexpr mint operator/(mint lhs, const mint &rhs) noexcept {
        return lhs /= rhs;
    }

    [[nodiscard]] constexpr mint operator-() const noexcept {
        return mint(0) - mint(*this);
    }

    [[nodiscard]] friend constexpr bool operator==(const mint &lhs,
                                                   const mint &rhs) noexcept {
        return lhs.val() == rhs.val();
    }

    [[nodiscard]] friend constexpr bool operator!=(const mint &lhs,
                                                   const mint &rhs) noexcept {
        return lhs.val() != rhs.val();
    }

private:
    U v;
    inline static constexpr bool is_prime = is_prime_v<U, m>;
};

template <u32 m>
using static_modint_32 = static_modint_base<u32, m>;

template <u64 m>
using static_modint_64 = static_modint_base<u64, m>;

using modint998244353 = static_modint_32<998'244'353>;
using modint1000000007 = static_modint_32<1'000'000'007>;
} // namespace mld

using Z = mld::modint998244353;

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<Z> a(n);
    for (auto &&x : a) {
        u32 y;
        std::cin >> y;
        x = y;
    }

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    std::vector<std::pair<Z, Z>> c;
    c.reserve(n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v, x, y;
        std::cin >> u >> v >> x >> y;

        e.emplace_back(u, v);
        e.emplace_back(v, u);

        c.emplace_back(x, y);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree h(g);

    std::vector<std::pair<Z, Z>> d(n);
    for (u32 i = 0; i < n - 1; ++i) {
        auto &&[u, v] = e[i << 1];
        if (h.par[u] == v) std::swap(u, v);

        d[v] = c[i];
    }

    std::vector<Z> dp(a.begin(), a.end()), tp(n), kp(n);
    for (u32 u : h.tor | std::views::drop(1) | std::views::reverse) {
        auto &&[x, y] = d[u];
        tp[u] = x * dp[u] + y * h.siz[u];
        dp[h.par[u]] += tp[u];
    }

    for (u32 u : h.tor | std::views::drop(1)) {
        auto &&[x, y] = d[u];
        kp[u] = x * (kp[h.par[u]] + dp[h.par[u]] - tp[u]) + y * (n - h.siz[u]);
    }

    for (u32 u = 0; u < n; ++u) std::cout << (kp[u] + dp[u]).val() << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
