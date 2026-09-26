#line 2 "convolution_mod1000000007.hpp"
#line 2 "convolution/ntt897.hpp"
#if !defined(__AVX2__) && !defined(_M_AVX2)
#error "Compile with -mavx2 (see README.md)."
#endif
#line 2 "ntt897.hpp"

#if defined(__GNUC__) && !defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#elif defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma clang attribute push( \
    __attribute__((target("avx2,bmi,bmi2,lzcnt,popcnt,ssse3"))), \
    apply_to = function)
#endif

#include <bits/stdc++.h>

#include <immintrin.h>


#line 1 "math/modint897.hpp"

#include <type_traits>


struct modint897 {
    using u32 = std::uint32_t;
    using i32 = std::int32_t;
    using u64 = std::uint64_t;

    static constexpr u32 MOD = 897581057u;
    static constexpr u32 MOD2 = MOD * 2;
    static constexpr u32 primitive_root = 3;
    static constexpr int max_power_of_two = 23;

private:
    static constexpr u32 R = 3397386241u;
    static constexpr u32 N2 = 780610957u;

    struct montgomery_tag {};

    constexpr modint897(u32 x, montgomery_tag) : a(x) {}

    static constexpr u32 reduce(u64 x) {
        return static_cast<u32>(
            (x + u64(static_cast<u32>(x) * u32(-R)) * MOD) >> 32
        );
    }

public:
    u32 a;

    static_assert(MOD < (u32(1) << 30));
    static_assert((MOD & 1) != 0);
    static_assert(R * MOD == 1);

    constexpr modint897() : a(0) {}

    template <class T, std::enable_if_t<std::is_integral_v<T> &&
                                        std::is_signed_v<T>, int> = 0>
    constexpr modint897(T x) : a(0) {
        const std::int64_t y =
            static_cast<std::int64_t>(x) % std::int64_t(MOD) + MOD;
        a = reduce(u64(y) * N2);
    }

    template <class T, std::enable_if_t<std::is_integral_v<T> &&
                                        std::is_unsigned_v<T>, int> = 0>
    constexpr modint897(T x)
        : a(reduce(((u64(x) % MOD) + MOD) * N2)) {}

    static constexpr modint897 raw(u32 x) {
        return modint897(reduce(u64(x) * N2), montgomery_tag{});
    }

    static constexpr modint897 montgomery_raw(u32 x) {
        return modint897(x, montgomery_tag{});
    }

    static constexpr u32 mod() { return MOD; }
    static constexpr u32 get_mod() { return MOD; }

    constexpr u32 val() const {
        const u32 x = reduce(a);
        return x >= MOD ? x - MOD : x;
    }

    constexpr u32 get() const { return val(); }

    constexpr modint897& operator+=(const modint897& rhs) {
        a += rhs.a - MOD2;
        if (i32(a) < 0) a += MOD2;
        return *this;
    }

    constexpr modint897& operator-=(const modint897& rhs) {
        a -= rhs.a;
        if (i32(a) < 0) a += MOD2;
        return *this;
    }

    constexpr modint897& operator*=(const modint897& rhs) {
        a = reduce(u64(a) * rhs.a);
        return *this;
    }

    constexpr modint897& operator/=(const modint897& rhs) {
        return *this *= rhs.inv();
    }

    constexpr modint897 operator+() const { return *this; }
    constexpr modint897 operator-() const { return modint897() - *this; }

    friend constexpr modint897 operator+(modint897 lhs, const modint897& rhs) {
        return lhs += rhs;
    }

    friend constexpr modint897 operator-(modint897 lhs, const modint897& rhs) {
        return lhs -= rhs;
    }

    friend constexpr modint897 operator*(modint897 lhs, const modint897& rhs) {
        return lhs *= rhs;
    }

    friend constexpr modint897 operator/(modint897 lhs, const modint897& rhs) {
        return lhs /= rhs;
    }

    friend constexpr bool operator==(const modint897& lhs, const modint897& rhs) {
        const u32 x = lhs.a >= MOD ? lhs.a - MOD : lhs.a;
        const u32 y = rhs.a >= MOD ? rhs.a - MOD : rhs.a;
        return x == y;
    }

    friend constexpr bool operator!=(const modint897& lhs, const modint897& rhs) {
        return !(lhs == rhs);
    }

    constexpr modint897& operator++() {
        return *this += raw(1);
    }

    constexpr modint897 operator++(int) {
        modint897 old = *this;
        ++*this;
        return old;
    }

    constexpr modint897& operator--() {
        return *this -= raw(1);
    }

    constexpr modint897 operator--(int) {
        modint897 old = *this;
        --*this;
        return old;
    }

    constexpr modint897 pow(u64 exponent) const {
        modint897 result = raw(1);
        modint897 base = *this;
        while (exponent != 0) {
            if (exponent & 1) result *= base;
            base *= base;
            exponent >>= 1;
        }
        return result;
    }

    constexpr modint897 inv() const {
        assert(val() != 0);
        return pow(MOD - 2);
    }

    constexpr modint897 inverse() const { return inv(); }

    friend std::ostream& operator<<(std::ostream& os, const modint897& x) {
        return os << x.val();
    }

    friend std::istream& operator>>(std::istream& is, modint897& x) {
        std::int64_t value;
        is >> value;
        x = modint897(value);
        return is;
    }
};

static_assert(sizeof(modint897) == 4);
static_assert(std::is_trivially_copyable_v<modint897>);

using mint897 = modint897;

#line 18 "ntt897.hpp"

#if defined(_MSC_VER)
#define EEZ_NTT897_ALWAYS_INLINE __forceinline
#define EEZ_NTT897_RESTRICT __restrict
#elif defined(__GNUC__) || defined(__clang__)
#define EEZ_NTT897_ALWAYS_INLINE inline __attribute__((always_inline))
#define EEZ_NTT897_RESTRICT __restrict__
#else
#define EEZ_NTT897_ALWAYS_INLINE inline
#define EEZ_NTT897_RESTRICT
#endif

namespace eez::ntt897{

using mint=modint897;
using u32=std::uint32_t;
using usize=std::size_t;

inline constexpr u32 mod=mint::MOD;
inline constexpr usize max_ntt_size=usize(1)<<23;
inline constexpr usize max_convolution_size=usize(1)<<25;
inline constexpr usize max_size=max_ntt_size;
inline constexpr usize naive_cutoff=60;

inline void forward(std::span<mint> a) noexcept;
inline void inverse(std::span<mint> a) noexcept;
inline std::vector<mint> convolution(std::span<const mint> a,std::span<const mint> b);
inline std::vector<mint> square(std::span<const mint> a);

inline std::vector<mint> convolution(const std::vector<mint>& a,const std::vector<mint>& b){
    return convolution(std::span<const mint>(a.data(),a.size()),std::span<const mint>(b.data(),b.size()));
}

inline std::vector<mint> square(const std::vector<mint>& a){
    return square(std::span<const mint>(a.data(),a.size()));
}

namespace detail{

template<class T>
class aligned_allocator{
public:
    using value_type=T;
    using is_always_equal=std::true_type;
    aligned_allocator() noexcept=default;
    template<class U> constexpr aligned_allocator(const aligned_allocator<U>&) noexcept{}
    [[nodiscard]] T* allocate(usize n){
        return static_cast<T*>(::operator new(n*sizeof(T),std::align_val_t{64}));
    }
    void deallocate(T* p,usize) noexcept{
        ::operator delete(p,std::align_val_t{64});
    }
    template<class U> struct rebind{using other=aligned_allocator<U>;};
};

template<class T,class U>
constexpr bool operator==(const aligned_allocator<T>&,const aligned_allocator<U>&) noexcept{return true;}

template<class T,class U>
constexpr bool operator!=(const aligned_allocator<T>&,const aligned_allocator<U>&) noexcept{return false;}

using aligned_vector=std::vector<mint,aligned_allocator<mint>>;

}

class workspace{
public:
    workspace()=default;
    explicit workspace(usize n){reserve(n);}
    void reserve(usize n){
        if(a_.size()<n)a_.resize(n);
        if(b_.size()<n)b_.resize(n);
    }
    [[nodiscard]] usize capacity()const noexcept{return std::min(a_.size(),b_.size());}
private:
    friend void convolution_to(std::span<const mint>,std::span<const mint>,std::span<mint>,workspace&);
    friend void square_to(std::span<const mint>,std::span<mint>,workspace&);
    detail::aligned_vector a_;
    detail::aligned_vector b_;
};

inline void convolution_to(std::span<const mint> a,std::span<const mint> b,std::span<mint> out,workspace& ws);
inline void square_to(std::span<const mint> a,std::span<mint> out,workspace& ws);

class frequency_buffer{
public:
    frequency_buffer()=default;
    [[nodiscard]] usize size()const noexcept{return data_.size();}
private:
    friend void forward_to(std::span<const mint>,frequency_buffer&,usize);
    friend void pointwise_multiply(frequency_buffer&,const frequency_buffer&);
    friend void pointwise_square(frequency_buffer&);
    friend void inverse_to(frequency_buffer&,std::span<mint>);
    std::vector<mint> data_;
};

inline void forward_to(std::span<const mint> src,frequency_buffer& dst,usize n);
inline void pointwise_multiply(frequency_buffer& lhs,const frequency_buffer& rhs);
inline void pointwise_square(frequency_buffer& a);
inline void inverse_to(frequency_buffer& src,std::span<mint> out);

constexpr usize convolution_size(usize n,usize m) noexcept{
    return n&&m?n+m-1:0;
}

constexpr usize transform_size(usize n,usize m) noexcept{
    if(!n||!m)return 0;
    if(n>max_ntt_size||m>max_ntt_size)return 0;
    if(n>max_ntt_size-m+1)return 0;
    const usize z=n+m-1;
    usize x=1;
    while(x<z)x<<=1;
    return x;
}

constexpr usize convolution_transform_size(usize n,usize m) noexcept{
    if(!n||!m)return 0;
    if(n>max_convolution_size||m>max_convolution_size)return 0;
    if(n>max_convolution_size-m+1)return 0;
    const usize z=n+m-1;
    usize x=1;
    while(x<z)x<<=1;
    return x;
}

constexpr bool valid_ntt_size(usize n) noexcept{
    return n!=0&&(n&(n-1))==0&&n<=max_ntt_size;
}

constexpr bool valid_convolution_transform_size(usize n) noexcept{
    return n>=32&&(n&(n-1))==0&&n<=max_convolution_size;
}

namespace detail{

using word=u32;
using u64=std::uint64_t;

inline constexpr word mod=mint::MOD;
inline constexpr word mod2=2*mod;
inline constexpr unsigned max_log=23;
inline constexpr word montgomery_ninv=897581055u;
inline constexpr word montgomery_one=mint::raw(1).a;

static_assert(mod<(word(1)<<30));
static_assert(word(mod*montgomery_ninv)==~word(0));
static_assert(sizeof(mint)==sizeof(word));

EEZ_NTT897_ALWAYS_INLINE constexpr word raw(const mint& x) noexcept{return x.a;}
EEZ_NTT897_ALWAYS_INLINE constexpr mint from_raw(word x) noexcept{return mint::montgomery_raw(x);}

EEZ_NTT897_ALWAYS_INLINE constexpr word mul(word a,word b) noexcept{
    const u64 x=u64(a)*b;
    const word q=static_cast<word>(x)*montgomery_ninv;
    return static_cast<word>((x+u64(q)*mod)>>32);
}

EEZ_NTT897_ALWAYS_INLINE constexpr word add(word a,word b) noexcept{
    const word x=a+b;
    return x>=mod2?x-mod2:x;
}

EEZ_NTT897_ALWAYS_INLINE constexpr word sub(word a,word b) noexcept{
    return a>=b?a-b:a+mod2-b;
}

EEZ_NTT897_ALWAYS_INLINE constexpr word canonicalize(word a) noexcept{
    return a>=mod?a-mod:a;
}

struct twiddle_table{
    std::array<word,max_log+1> root{};
    std::array<word,max_log+1> iroot{};
    std::array<word,max_log+1> rate1{};
    std::array<word,max_log+1> rate3{};
    std::array<word,max_log+1> irate3{};

    constexpr twiddle_table(){
        root[max_log]=mint::raw(mint::primitive_root).pow((mod-1)>>max_log).a;
        iroot[max_log]=mint::montgomery_raw(root[max_log]).inv().a;
        for(int i=int(max_log)-1;i>=0;--i){
            root[usize(i)]=mul(root[usize(i+1)],root[usize(i+1)]);
            iroot[usize(i)]=mul(iroot[usize(i+1)],iroot[usize(i+1)]);
        }
        word prod=montgomery_one;
        for(unsigned i=0;i+1<=max_log;++i){
            rate1[i]=mul(root[i+1],prod);
            prod=mul(prod,iroot[i+1]);
        }
        prod=montgomery_one;
        word iprod=montgomery_one;
        for(unsigned i=0;i+3<=max_log;++i){
            rate3[i]=mul(root[i+3],prod);
            irate3[i]=mul(iroot[i+3],iprod);
            prod=mul(prod,iroot[i+3]);
            iprod=mul(iprod,root[i+3]);
        }
    }
};

inline constexpr twiddle_table twiddles{};

EEZ_NTT897_ALWAYS_INLINE word forward_rate1(unsigned i) noexcept{return twiddles.rate1[i];}
EEZ_NTT897_ALWAYS_INLINE word forward_rate3(unsigned i) noexcept{return twiddles.rate3[i];}
EEZ_NTT897_ALWAYS_INLINE word inverse_rate3(unsigned i) noexcept{return twiddles.irate3[i];}

EEZ_NTT897_ALWAYS_INLINE unsigned twiddle_index(u32 block) noexcept{
    return static_cast<unsigned>(std::countr_zero(~block));
}

#if defined(__AVX2__) || defined(_M_AVX2)

using vec=__m256i;

EEZ_NTT897_ALWAYS_INLINE vec load8(const mint* p) noexcept{
    return _mm256_loadu_si256(reinterpret_cast<const __m256i*>(static_cast<const void*>(p)));
}

EEZ_NTT897_ALWAYS_INLINE void store8(mint* p,vec x) noexcept{
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(static_cast<void*>(p)),x);
}

EEZ_NTT897_ALWAYS_INLINE vec broadcast(word x) noexcept{
    return _mm256_set1_epi32(static_cast<int>(x));
}

EEZ_NTT897_ALWAYS_INLINE vec add8(vec a,vec b) noexcept{
    const vec two_p=broadcast(mod2);
    vec x=_mm256_sub_epi32(_mm256_add_epi32(a,b),two_p);
    return _mm256_add_epi32(x,_mm256_and_si256(_mm256_srai_epi32(x,31),two_p));
}

EEZ_NTT897_ALWAYS_INLINE vec sub8(vec a,vec b) noexcept{
    const vec two_p=broadcast(mod2);
    vec x=_mm256_sub_epi32(a,b);
    return _mm256_add_epi32(x,_mm256_and_si256(_mm256_srai_epi32(x,31),two_p));
}

EEZ_NTT897_ALWAYS_INLINE vec mul8(vec a,vec b) noexcept{
    const vec ninv=broadcast(montgomery_ninv);
    const vec prime=broadcast(mod);
    const vec pe=_mm256_mul_epu32(a,b);
    const vec po=_mm256_mul_epu32(_mm256_bsrli_epi128(a,4),_mm256_bsrli_epi128(b,4));
    const vec qe=_mm256_mul_epu32(pe,ninv);
    const vec qo=_mm256_mul_epu32(po,ninv);
    const vec re=_mm256_add_epi64(pe,_mm256_mul_epu32(qe,prime));
    const vec ro=_mm256_add_epi64(po,_mm256_mul_epu32(qo,prime));
    return _mm256_or_si256(_mm256_bsrli_epi128(re,4),ro);
}

EEZ_NTT897_ALWAYS_INLINE vec mul8_fixed(vec a,vec b,vec bninv) noexcept{
    const vec prime=broadcast(mod);
    const vec oa=_mm256_bsrli_epi128(a,4);
    const vec pe=_mm256_mul_epu32(a,b);
    const vec po=_mm256_mul_epu32(oa,b);
    const vec qe=_mm256_mul_epu32(a,bninv);
    const vec qo=_mm256_mul_epu32(oa,bninv);
    const vec re=_mm256_add_epi64(pe,_mm256_mul_epu32(qe,prime));
    const vec ro=_mm256_add_epi64(po,_mm256_mul_epu32(qo,prime));
    return _mm256_or_si256(_mm256_bsrli_epi128(re,4),ro);
}

EEZ_NTT897_ALWAYS_INLINE vec canonicalize8(vec x) noexcept{const vec p=broadcast(mod);return _mm256_min_epu32(x,_mm256_sub_epi32(x,p));}

EEZ_NTT897_ALWAYS_INLINE vec pack_four(word x0,word x1) noexcept{
    return _mm256_setr_epi32(
        static_cast<int>(x0),static_cast<int>(x0),static_cast<int>(x0),static_cast<int>(x0),
        static_cast<int>(x1),static_cast<int>(x1),static_cast<int>(x1),static_cast<int>(x1)
    );
}

EEZ_NTT897_ALWAYS_INLINE vec load2x4(const mint* p0,const mint* p1) noexcept{
    const __m128i lo=_mm_loadu_si128(reinterpret_cast<const __m128i*>(static_cast<const void*>(p0)));
    const __m128i hi=_mm_loadu_si128(reinterpret_cast<const __m128i*>(static_cast<const void*>(p1)));
    return _mm256_set_m128i(hi,lo);
}

EEZ_NTT897_ALWAYS_INLINE void store2x4(mint* p0,mint* p1,vec x) noexcept{
    _mm_storeu_si128(reinterpret_cast<__m128i*>(static_cast<void*>(p0)),_mm256_castsi256_si128(x));
    _mm_storeu_si128(reinterpret_cast<__m128i*>(static_cast<void*>(p1)),_mm256_extracti128_si256(x,1));
}

EEZ_NTT897_ALWAYS_INLINE void transpose_8x4_to_4x8(vec v0,vec v1,vec v2,vec v3,vec& x0,vec& x1,vec& x2,vec& x3) noexcept{
    const vec t0=_mm256_unpacklo_epi32(v0,v1);
    const vec t1=_mm256_unpackhi_epi32(v0,v1);
    const vec t2=_mm256_unpacklo_epi32(v2,v3);
    const vec t3=_mm256_unpackhi_epi32(v2,v3);
    const vec perm=_mm256_setr_epi32(0,4,1,5,2,6,3,7);
    x0=_mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(t0,t2),perm);
    x1=_mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(t0,t2),perm);
    x2=_mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(t1,t3),perm);
    x3=_mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(t1,t3),perm);
}

EEZ_NTT897_ALWAYS_INLINE void transpose_4x8_to_8x4(vec x0,vec x1,vec x2,vec x3,vec& v0,vec& v1,vec& v2,vec& v3) noexcept{
    const vec perm=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    const vec q0=_mm256_permutevar8x32_epi32(x0,perm);
    const vec q1=_mm256_permutevar8x32_epi32(x1,perm);
    const vec q2=_mm256_permutevar8x32_epi32(x2,perm);
    const vec q3=_mm256_permutevar8x32_epi32(x3,perm);
    const vec t0=_mm256_unpacklo_epi64(q0,q1);
    const vec t2=_mm256_unpackhi_epi64(q0,q1);
    const vec t1=_mm256_unpacklo_epi64(q2,q3);
    const vec t3=_mm256_unpackhi_epi64(q2,q3);
    v0=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t0),_mm256_castsi256_ps(t1),_MM_SHUFFLE(2,0,2,0)));
    v1=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t0),_mm256_castsi256_ps(t1),_MM_SHUFFLE(3,1,3,1)));
    v2=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t2),_mm256_castsi256_ps(t3),_MM_SHUFFLE(2,0,2,0)));
    v3=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t2),_mm256_castsi256_ps(t3),_MM_SHUFFLE(3,1,3,1)));
}

#endif

EEZ_NTT897_ALWAYS_INLINE void forward_butterfly(mint* b,usize stride,usize i,word r1,word r2,word r3) noexcept{
    const word x0=raw(b[i]);
    const word x1=mul(raw(b[stride+i]),r1);
    const word x2=mul(raw(b[2*stride+i]),r2);
    const word x3=mul(raw(b[3*stride+i]),r3);
    const word s02=add(x0,x2);
    const word d02=sub(x0,x2);
    const word s13=add(x1,x3);
    const word t=mul(sub(x1,x3),twiddles.root[2]);
    b[i]=from_raw(add(s02,s13));
    b[stride+i]=from_raw(sub(s02,s13));
    b[2*stride+i]=from_raw(add(d02,t));
    b[3*stride+i]=from_raw(sub(d02,t));
}

EEZ_NTT897_ALWAYS_INLINE void inverse_butterfly(mint* b,usize stride,usize i,word r1,word r2,word r3) noexcept{
    const word x0=raw(b[i]);
    const word x1=raw(b[stride+i]);
    const word x2=raw(b[2*stride+i]);
    const word x3=raw(b[3*stride+i]);
    const word s01=add(x0,x1);
    const word d01=sub(x0,x1);
    const word s23=add(x2,x3);
    const word t=mul(sub(x2,x3),twiddles.iroot[2]);
    b[i]=from_raw(add(s01,s23));
    b[stride+i]=from_raw(mul(add(d01,t),r1));
    b[2*stride+i]=from_raw(mul(sub(s01,s23),r2));
    b[3*stride+i]=from_raw(mul(sub(d01,t),r3));
}

inline void forward_radix4_scalar(mint* EEZ_NTT897_RESTRICT a,usize blocks,usize stride) noexcept{
    {
        mint* const b=a;
        for(usize i=0;i<stride;++i){
            const word x0=raw(b[i]);
            const word x1=raw(b[stride+i]);
            const word x2=raw(b[2*stride+i]);
            const word x3=raw(b[3*stride+i]);
            const word s02=add(x0,x2);
            const word d02=sub(x0,x2);
            const word s13=add(x1,x3);
            const word t=mul(sub(x1,x3),twiddles.root[2]);
            b[i]=from_raw(add(s02,s13));
            b[stride+i]=from_raw(sub(s02,s13));
            b[2*stride+i]=from_raw(add(d02,t));
            b[3*stride+i]=from_raw(sub(d02,t));
        }
    }
    if(blocks==1)return;
    word rot=forward_rate3(0);
    for(usize s=1;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        mint* const b=a+s*4*stride;
        for(usize i=0;i<stride;++i)forward_butterfly(b,stride,i,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

inline void inverse_radix4_scalar(mint* EEZ_NTT897_RESTRICT a,usize blocks,usize stride) noexcept{
    {
        mint* const b=a;
        for(usize i=0;i<stride;++i){
            const word x0=raw(b[i]);
            const word x1=raw(b[stride+i]);
            const word x2=raw(b[2*stride+i]);
            const word x3=raw(b[3*stride+i]);
            const word s01=add(x0,x1);
            const word d01=sub(x0,x1);
            const word s23=add(x2,x3);
            const word t=mul(sub(x2,x3),twiddles.iroot[2]);
            b[i]=from_raw(add(s01,s23));
            b[stride+i]=from_raw(add(d01,t));
            b[2*stride+i]=from_raw(sub(s01,s23));
            b[3*stride+i]=from_raw(sub(d01,t));
        }
    }
    if(blocks==1)return;
    word rot=inverse_rate3(0);
    for(usize s=1;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        mint* const b=a+s*4*stride;
        for(usize i=0;i<stride;++i)inverse_butterfly(b,stride,i,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

#if defined(__AVX2__) || defined(_M_AVX2)

EEZ_NTT897_ALWAYS_INLINE void forward_radix4_large_block(mint* EEZ_NTT897_RESTRICT b,usize stride,vec imag,word r1,word r2,word r3) noexcept{
    const vec w1=broadcast(r1);
    const vec w2=broadcast(r2);
    const vec w3=broadcast(r3);
    for(usize i=0;i<stride;i+=8){
        const vec x0=load8(b+i);
        const vec x1=mul8(load8(b+stride+i),w1);
        const vec x2=mul8(load8(b+2*stride+i),w2);
        const vec x3=mul8(load8(b+3*stride+i),w3);
        const vec s02=add8(x0,x2);
        const vec d02=sub8(x0,x2);
        const vec s13=add8(x1,x3);
        const vec t=mul8(sub8(x1,x3),imag);
        store8(b+i,add8(s02,s13));
        store8(b+stride+i,sub8(s02,s13));
        store8(b+2*stride+i,add8(d02,t));
        store8(b+3*stride+i,sub8(d02,t));
    }
}

EEZ_NTT897_ALWAYS_INLINE void inverse_radix4_large_block(mint* EEZ_NTT897_RESTRICT b,usize stride,vec iimag,word r1,word r2,word r3) noexcept{
    const vec w1=broadcast(r1);
    const vec w2=broadcast(r2);
    const vec w3=broadcast(r3);
    for(usize i=0;i<stride;i+=8){
        const vec x0=load8(b+i);
        const vec x1=load8(b+stride+i);
        const vec x2=load8(b+2*stride+i);
        const vec x3=load8(b+3*stride+i);
        const vec s01=add8(x0,x1);
        const vec d01=sub8(x0,x1);
        const vec s23=add8(x2,x3);
        const vec t=mul8(sub8(x2,x3),iimag);
        store8(b+i,add8(s01,s23));
        store8(b+stride+i,mul8(add8(d01,t),w1));
        store8(b+2*stride+i,mul8(sub8(s01,s23),w2));
        store8(b+3*stride+i,mul8(sub8(d01,t),w3));
    }
}

inline void forward_radix4_large(mint* EEZ_NTT897_RESTRICT a,usize blocks,usize stride) noexcept{
    const vec imag=broadcast(twiddles.root[2]);
    {
        mint* const b=a;
        for(usize i=0;i<stride;i+=8){
            const vec x0=load8(b+i);
            const vec x1=load8(b+stride+i);
            const vec x2=load8(b+2*stride+i);
            const vec x3=load8(b+3*stride+i);
            const vec s02=add8(x0,x2);
            const vec d02=sub8(x0,x2);
            const vec s13=add8(x1,x3);
            const vec t=mul8(sub8(x1,x3),imag);
            store8(b+i,add8(s02,s13));
            store8(b+stride+i,sub8(s02,s13));
            store8(b+2*stride+i,add8(d02,t));
            store8(b+3*stride+i,sub8(d02,t));
        }
    }
    if(blocks==1)return;
    word rot=forward_rate3(0);
    usize s=1;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8],r2[8],r3[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        _mm256_store_si256(reinterpret_cast<vec*>(r2),w2);
        _mm256_store_si256(reinterpret_cast<vec*>(r3),w3);
        for(unsigned lane=0;lane<8;++lane)forward_radix4_large_block(a+(s+lane)*4*stride,stride,imag,r1[lane],r2[lane],r3[lane]);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        forward_radix4_large_block(a+s*4*stride,stride,imag,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

inline void inverse_radix4_large(mint* EEZ_NTT897_RESTRICT a,usize blocks,usize stride) noexcept{
    const vec iimag=broadcast(twiddles.iroot[2]);
    {
        mint* const b=a;
        for(usize i=0;i<stride;i+=8){
            const vec x0=load8(b+i);
            const vec x1=load8(b+stride+i);
            const vec x2=load8(b+2*stride+i);
            const vec x3=load8(b+3*stride+i);
            const vec s01=add8(x0,x1);
            const vec d01=sub8(x0,x1);
            const vec s23=add8(x2,x3);
            const vec t=mul8(sub8(x2,x3),iimag);
            store8(b+i,add8(s01,s23));
            store8(b+stride+i,add8(d01,t));
            store8(b+2*stride+i,sub8(s01,s23));
            store8(b+3*stride+i,sub8(d01,t));
        }
    }
    if(blocks==1)return;
    word rot=inverse_rate3(0);
    usize s=1;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8],r2[8],r3[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        _mm256_store_si256(reinterpret_cast<vec*>(r2),w2);
        _mm256_store_si256(reinterpret_cast<vec*>(r3),w3);
        for(unsigned lane=0;lane<8;++lane)inverse_radix4_large_block(a+(s+lane)*4*stride,stride,iimag,r1[lane],r2[lane],r3[lane]);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        inverse_radix4_large_block(a+s*4*stride,stride,iimag,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

inline void forward_radix4_p4(mint* EEZ_NTT897_RESTRICT a,usize blocks) noexcept{
    if(blocks<2){
        forward_radix4_scalar(a,blocks,4);
        return;
    }
    const vec imag=broadcast(twiddles.root[2]);
    word rot=montgomery_one;
    for(usize s=0;s<blocks;s+=2){
        const word r10=rot;
        rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
        const word r11=rot;
        const vec w1=pack_four(r10,r11);
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        mint* const b0=a+s*16;
        mint* const b1=b0+16;
        const vec x0=load2x4(b0,b1);
        const vec x1=mul8(load2x4(b0+4,b1+4),w1);
        const vec x2=mul8(load2x4(b0+8,b1+8),w2);
        const vec x3=mul8(load2x4(b0+12,b1+12),w3);
        const vec s02=add8(x0,x2);
        const vec d02=sub8(x0,x2);
        const vec s13=add8(x1,x3);
        const vec t=mul8(sub8(x1,x3),imag);
        store2x4(b0,b1,add8(s02,s13));
        store2x4(b0+4,b1+4,sub8(s02,s13));
        store2x4(b0+8,b1+8,add8(d02,t));
        store2x4(b0+12,b1+12,sub8(d02,t));
        if(s+2<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+1))));
    }
}

inline void inverse_radix4_p4(mint* EEZ_NTT897_RESTRICT a,usize blocks) noexcept{
    if(blocks<2){
        inverse_radix4_scalar(a,blocks,4);
        return;
    }
    const vec iimag=broadcast(twiddles.iroot[2]);
    word rot=montgomery_one;
    for(usize s=0;s<blocks;s+=2){
        const word r10=rot;
        rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
        const word r11=rot;
        const vec w1=pack_four(r10,r11);
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        mint* const b0=a+s*16;
        mint* const b1=b0+16;
        const vec x0=load2x4(b0,b1);
        const vec x1=load2x4(b0+4,b1+4);
        const vec x2=load2x4(b0+8,b1+8);
        const vec x3=load2x4(b0+12,b1+12);
        const vec s01=add8(x0,x1);
        const vec d01=sub8(x0,x1);
        const vec s23=add8(x2,x3);
        const vec t=mul8(sub8(x2,x3),iimag);
        store2x4(b0,b1,add8(s01,s23));
        store2x4(b0+4,b1+4,mul8(add8(d01,t),w1));
        store2x4(b0+8,b1+8,mul8(sub8(s01,s23),w2));
        store2x4(b0+12,b1+12,mul8(sub8(d01,t),w3));
        if(s+2<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+1))));
    }
}

inline void forward_radix4_p1(mint* EEZ_NTT897_RESTRICT a,usize blocks) noexcept{
    const vec imag=broadcast(twiddles.root[2]);
    word rot=montgomery_one;
    usize s=0;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        mint* const b=a+4*s;
        vec x0,x1,x2,x3;
        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);
        x1=mul8(x1,w1);
        x2=mul8(x2,w2);
        x3=mul8(x3,w3);
        const vec s02=add8(x0,x2);
        const vec d02=sub8(x0,x2);
        const vec s13=add8(x1,x3);
        const vec t=mul8(sub8(x1,x3),imag);
        vec v0,v1,v2,v3;
        transpose_4x8_to_8x4(add8(s02,s13),sub8(s02,s13),add8(d02,t),sub8(d02,t),v0,v1,v2,v3);
        store8(b,v0);
        store8(b+8,v1);
        store8(b+16,v2);
        store8(b+24,v3);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        forward_butterfly(a+4*s,1,0,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

inline void inverse_radix4_p1(mint* EEZ_NTT897_RESTRICT a,usize blocks) noexcept{
    const vec iimag=broadcast(twiddles.iroot[2]);
    word rot=montgomery_one;
    usize s=0;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        mint* const b=a+4*s;
        vec x0,x1,x2,x3;
        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);
        const vec s01=add8(x0,x1);
        const vec d01=sub8(x0,x1);
        const vec s23=add8(x2,x3);
        const vec t=mul8(sub8(x2,x3),iimag);
        vec v0,v1,v2,v3;
        transpose_4x8_to_8x4(add8(s01,s23),mul8(add8(d01,t),w1),mul8(sub8(s01,s23),w2),mul8(sub8(d01,t),w3),v0,v1,v2,v3);
        store8(b,v0);
        store8(b+8,v1);
        store8(b+16,v2);
        store8(b+24,v3);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        inverse_butterfly(a+4*s,1,0,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

#endif

inline void forward_radix2_first(mint* EEZ_NTT897_RESTRICT a,usize n) noexcept{
    const usize half=n>>1;
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    for(;i+8<=half;i+=8){
        const vec x=load8(a+i);
        const vec y=load8(a+half+i);
        store8(a+i,add8(x,y));
        store8(a+half+i,sub8(x,y));
    }
#endif
    for(;i<half;++i){
        const word x=raw(a[i]);
        const word y=raw(a[half+i]);
        a[i]=from_raw(add(x,y));
        a[half+i]=from_raw(sub(x,y));
    }
}

inline void forward_radix4_stage(mint* EEZ_NTT897_RESTRICT a,usize n,int stage) noexcept{
    const int h=static_cast<int>(std::countr_zero(n));
    assert(stage>=0&&stage+2<=h);
    const usize stride=usize(1)<<(h-stage-2);
    const usize blocks=usize(1)<<stage;
#if defined(__AVX2__) || defined(_M_AVX2)
    if(stride>=8)forward_radix4_large(a,blocks,stride);
    else if(stride==4)forward_radix4_p4(a,blocks);
    else if(stride==1)forward_radix4_p1(a,blocks);
    else forward_radix4_scalar(a,blocks,stride);
#else
    forward_radix4_scalar(a,blocks,stride);
#endif
}

inline void inverse_radix4_stage(mint* EEZ_NTT897_RESTRICT a,usize n,int stage) noexcept{
    const int h=static_cast<int>(std::countr_zero(n));
    assert(stage>=0&&stage+2<=h);
    const usize stride=usize(1)<<(h-stage-2);
    const usize blocks=usize(1)<<stage;
#if defined(__AVX2__) || defined(_M_AVX2)
    if(stride>=8)inverse_radix4_large(a,blocks,stride);
    else if(stride==4)inverse_radix4_p4(a,blocks);
    else if(stride==1)inverse_radix4_p1(a,blocks);
    else inverse_radix4_scalar(a,blocks,stride);
#else
    inverse_radix4_scalar(a,blocks,stride);
#endif
}

inline void final_radix2_scale(mint* EEZ_NTT897_RESTRICT a,usize n,word scale_mont) noexcept{
    const usize half=n>>1;
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    const vec scale=broadcast(scale_mont);
    for(;i+8<=half;i+=8){
        const vec x=load8(a+i);
        const vec y=load8(a+half+i);
        store8(a+i,mul8(add8(x,y),scale));
        store8(a+half+i,mul8(sub8(x,y),scale));
    }
#endif
    for(;i<half;++i){
        const word x=raw(a[i]);
        const word y=raw(a[half+i]);
        a[i]=from_raw(mul(add(x,y),scale_mont));
        a[half+i]=from_raw(mul(sub(x,y),scale_mont));
    }
}

inline void final_radix4_scale(mint* EEZ_NTT897_RESTRICT a,usize n,word scale_mont) noexcept{
    const usize stride=n>>2;
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    const vec iimag=broadcast(twiddles.iroot[2]);
    const vec scale=broadcast(scale_mont);
    for(;i+8<=stride;i+=8){
        const vec x0=load8(a+i);
        const vec x1=load8(a+stride+i);
        const vec x2=load8(a+2*stride+i);
        const vec x3=load8(a+3*stride+i);
        const vec s01=add8(x0,x1);
        const vec d01=sub8(x0,x1);
        const vec s23=add8(x2,x3);
        const vec t=mul8(sub8(x2,x3),iimag);
        store8(a+i,mul8(add8(s01,s23),scale));
        store8(a+stride+i,mul8(add8(d01,t),scale));
        store8(a+2*stride+i,mul8(sub8(s01,s23),scale));
        store8(a+3*stride+i,mul8(sub8(d01,t),scale));
    }
#endif
    for(;i<stride;++i){
        const word x0=raw(a[i]);
        const word x1=raw(a[stride+i]);
        const word x2=raw(a[2*stride+i]);
        const word x3=raw(a[3*stride+i]);
        const word s01=add(x0,x1);
        const word d01=sub(x0,x1);
        const word s23=add(x2,x3);
        const word t=mul(sub(x2,x3),twiddles.iroot[2]);
        a[i]=from_raw(mul(add(s01,s23),scale_mont));
        a[stride+i]=from_raw(mul(add(d01,t),scale_mont));
        a[2*stride+i]=from_raw(mul(sub(s01,s23),scale_mont));
        a[3*stride+i]=from_raw(mul(sub(d01,t),scale_mont));
    }
}

inline void forward_dif(mint* EEZ_NTT897_RESTRICT a,usize n) noexcept{
    if(n<=1)return;
    const int h=static_cast<int>(std::countr_zero(n));
    int stage=0;
    if(h&1){
        forward_radix2_first(a,n);
        stage=1;
    }
    for(;stage<h;stage+=2)forward_radix4_stage(a,n,stage);
}

inline void inverse_dit(mint* EEZ_NTT897_RESTRICT a,usize n) noexcept{
    if(n<=1)return;
    const int h=static_cast<int>(std::countr_zero(n));
    const word scale=mint::raw(static_cast<u32>(n)).inv().a;
    if(h&1){
        for(int stage=h-2;stage>=1;stage-=2)inverse_radix4_stage(a,n,stage);
        final_radix2_scale(a,n,scale);
    }else{
        for(int stage=h-2;stage>=2;stage-=2)inverse_radix4_stage(a,n,stage);
        final_radix4_scale(a,n,scale);
    }
}

#if defined(__AVX2__) || defined(_M_AVX2)

class aligned_uninitialized_buffer{
    static_assert(std::is_trivially_destructible_v<mint>);
    mint* data_=nullptr;
public:
    explicit aligned_uninitialized_buffer(usize n)
        :data_(static_cast<mint*>(::operator new[](n*sizeof(mint),std::align_val_t{64}))){}
    aligned_uninitialized_buffer(const aligned_uninitialized_buffer&)=delete;
    aligned_uninitialized_buffer& operator=(const aligned_uninitialized_buffer&)=delete;
    ~aligned_uninitialized_buffer(){
        ::operator delete[](data_,std::align_val_t{64});
    }
    mint* data() noexcept{return data_;}
    const mint* data()const noexcept{return data_;}
};

EEZ_NTT897_ALWAYS_INLINE vec load8_aligned(const mint* p) noexcept{
    return _mm256_load_si256(reinterpret_cast<const __m256i*>(static_cast<const void*>(p)));
}

EEZ_NTT897_ALWAYS_INLINE void store8_aligned(mint* p,vec x) noexcept{
    _mm256_store_si256(reinterpret_cast<__m256i*>(static_cast<void*>(p)),x);
}

EEZ_NTT897_ALWAYS_INLINE vec shrink4_to_2(vec x) noexcept{
    return _mm256_min_epu32(x,_mm256_sub_epi32(x,broadcast(mod2)));
}

EEZ_NTT897_ALWAYS_INLINE vec lazy_add8(vec a,vec b) noexcept{return _mm256_add_epi32(a,b);}

EEZ_NTT897_ALWAYS_INLINE vec lazy_sub8(vec a,vec b) noexcept{
    return _mm256_add_epi32(a,_mm256_sub_epi32(broadcast(mod2),b));
}

template<bool trivial_twiddle,bool convert_input=false>
inline void forward_radix4_block_lazy(mint* b,usize stride,word r1) noexcept{
    const word imag=canonicalize(twiddles.root[2]);
    const vec vimag=broadcast(imag);
    const vec vimag_ninv=broadcast(imag*montgomery_ninv);
    const vec vr1=broadcast(r1);
    const vec vr1_ninv=broadcast(r1*montgomery_ninv);
    const word r2=canonicalize(mul(r1,r1));
    const vec vr2=broadcast(r2);
    const vec vr2_ninv=broadcast(r2*montgomery_ninv);
    const word r3=canonicalize(mul(r2,r1));
    const vec vr3=broadcast(r3);
    const vec vr3_ninv=broadcast(r3*montgomery_ninv);

    for(usize i=0;i<stride;i+=8){
        vec x0=shrink4_to_2(load8_aligned(b+i));
        vec x1=load8_aligned(b+stride+i);
        vec x2=load8_aligned(b+2*stride+i);
        vec x3=load8_aligned(b+3*stride+i);
        if constexpr(!trivial_twiddle){
            x1=mul8_fixed(x1,vr1,vr1_ninv);
            x2=mul8_fixed(x2,vr2,vr2_ninv);
            x3=mul8_fixed(x3,vr3,vr3_ninv);
        }else{
            x1=shrink4_to_2(x1);
            x2=shrink4_to_2(x2);
            x3=shrink4_to_2(x3);
        }
        vec s02=lazy_add8(x0,x2);
        vec d02=lazy_sub8(x0,x2);
        vec s13=lazy_add8(x1,x3);
        const vec t=mul8_fixed(lazy_sub8(x1,x3),vimag,vimag_ninv);
        s02=shrink4_to_2(s02);
        d02=shrink4_to_2(d02);
        s13=shrink4_to_2(s13);
        vec y0=lazy_add8(s02,s13);
        vec y1=lazy_sub8(s02,s13);
        vec y2=lazy_add8(d02,t);
        vec y3=lazy_sub8(d02,t);
        if constexpr(convert_input){
            constexpr word r2c=780610957u;
            const vec vr=broadcast(r2c);
            const vec vn=broadcast(r2c*montgomery_ninv);
            y0=mul8_fixed(y0,vr,vn);
            y1=mul8_fixed(y1,vr,vn);
            y2=mul8_fixed(y2,vr,vn);
            y3=mul8_fixed(y3,vr,vn);
        }
        store8_aligned(b+i,y0);
        store8_aligned(b+stride+i,y1);
        store8_aligned(b+2*stride+i,y2);
        store8_aligned(b+3*stride+i,y3);
    }
}

template<bool trivial_twiddle,bool convert_input=false>
EEZ_NTT897_ALWAYS_INLINE void forward_radix4_block_pair_lazy(mint* EEZ_NTT897_RESTRICT a,mint* EEZ_NTT897_RESTRICT b,usize stride,word r1) noexcept{
    forward_radix4_block_lazy<trivial_twiddle,convert_input>(a,stride,r1);
    forward_radix4_block_lazy<trivial_twiddle,convert_input>(b,stride,r1);
}

template<bool trivial_twiddle,bool apply_scale,bool convert_output=false,bool direct_output=false,bool shrink_output=false>
inline void inverse_radix4_block_lazy(mint* b,usize stride,word r1,word scale) noexcept{
    const word iimag=canonicalize(twiddles.iroot[2]);
    const vec viimag=broadcast(iimag);
    const vec viimag_ninv=broadcast(iimag*montgomery_ninv);
    const vec vr1=broadcast(r1);
    const vec vr1_ninv=broadcast(r1*montgomery_ninv);
    const word r2=canonicalize(mul(r1,r1));
    const vec vr2=broadcast(r2);
    const vec vr2_ninv=broadcast(r2*montgomery_ninv);
    const word r3=canonicalize(mul(r2,r1));
    const vec vr3=broadcast(r3);
    const vec vr3_ninv=broadcast(r3*montgomery_ninv);
    const word sc=canonicalize(scale);

    for(usize i=0;i<stride;i+=8){
        const vec x0=shrink4_to_2(load8_aligned(b+i));
        const vec x1=shrink4_to_2(load8_aligned(b+stride+i));
        const vec x2=shrink4_to_2(load8_aligned(b+2*stride+i));
        const vec x3=shrink4_to_2(load8_aligned(b+3*stride+i));
        vec s01=lazy_add8(x0,x1);
        vec d01=lazy_sub8(x0,x1);
        vec s23=lazy_add8(x2,x3);
        const vec t=mul8_fixed(lazy_sub8(x2,x3),viimag,viimag_ninv);
        s01=shrink4_to_2(s01);
        d01=shrink4_to_2(d01);
        s23=shrink4_to_2(s23);
        vec y0=lazy_add8(s01,s23);
        vec y1=lazy_add8(d01,t);
        vec y2=lazy_sub8(s01,s23);
        vec y3=lazy_sub8(d01,t);
        if constexpr(apply_scale){
            const word s0=sc;
            const word s1=trivial_twiddle?s0:canonicalize(mul(s0,r1));
            const word s2=trivial_twiddle?s0:canonicalize(mul(s0,r2));
            const word s3=trivial_twiddle?s0:canonicalize(mul(s0,r3));
            y0=mul8_fixed(y0,broadcast(s0),broadcast(s0*montgomery_ninv));
            y1=mul8_fixed(y1,broadcast(s1),broadcast(s1*montgomery_ninv));
            y2=mul8_fixed(y2,broadcast(s2),broadcast(s2*montgomery_ninv));
            y3=mul8_fixed(y3,broadcast(s3),broadcast(s3*montgomery_ninv));
        }else if constexpr(!trivial_twiddle){
            y1=mul8_fixed(y1,vr1,vr1_ninv);
            y2=mul8_fixed(y2,vr2,vr2_ninv);
            y3=mul8_fixed(y3,vr3,vr3_ninv);
        }
        if constexpr(convert_output){
            const vec one=broadcast(1);
            const vec ninv=broadcast(montgomery_ninv);
            y0=canonicalize8(mul8_fixed(y0,one,ninv));
            y1=canonicalize8(mul8_fixed(y1,one,ninv));
            y2=canonicalize8(mul8_fixed(y2,one,ninv));
            y3=canonicalize8(mul8_fixed(y3,one,ninv));
        }else if constexpr(direct_output){
            y0=canonicalize8(shrink4_to_2(y0));
            y1=canonicalize8(shrink4_to_2(y1));
            y2=canonicalize8(shrink4_to_2(y2));
            y3=canonicalize8(shrink4_to_2(y3));
        }else if constexpr(shrink_output && !apply_scale){
            // Only the final stage must expose Montgomery values in [0, 2p).

            // apply_scale already reduces each product to this range.

            y0=shrink4_to_2(y0);
            y1=shrink4_to_2(y1);
            y2=shrink4_to_2(y2);
            y3=shrink4_to_2(y3);
        }
        store8_aligned(b+i,y0);
        store8_aligned(b+stride+i,y1);
        store8_aligned(b+2*stride+i,y2);
        store8_aligned(b+3*stride+i,y3);
    }
}

inline unsigned adaptive_leaf_log(usize n) noexcept{
    const unsigned h=static_cast<unsigned>(std::countr_zero(n));
    return(h&1u)?3u:4u;
}

template<bool convert_input=false>
EEZ_NTT897_ALWAYS_INLINE void forward_cache_node(mint* EEZ_NTT897_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    const usize stride=block_size>>2;
    if constexpr(convert_input)forward_radix4_block_lazy<true,true>(base,stride,montgomery_one);
    else if(block==0)forward_radix4_block_lazy<true>(base,stride,montgomery_one);
    else forward_radix4_block_lazy<false>(base,stride,rotation[layer]);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));
}

template<bool convert_input=false>
inline void forward_cache_block(mint* EEZ_NTT897_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    forward_cache_node<convert_input>(base,block_size,layer,block,blocks_at_layer,rotation);
    const usize child_size=block_size>>2;
    for(usize child=0;child<4;++child){
        mint* const p=base+child*child_size;
        const usize cb=block*4+child;
        forward_cache_node(p,child_size,layer+1,cb,blocks_at_layer*4,rotation);
        const usize gsize=child_size>>2;
        for(usize g=0;g<4;++g)forward_cache_node(p+g*gsize,gsize,layer+2,cb*4+g,blocks_at_layer*16,rotation);
    }
}

template<bool convert_input=false>
inline void forward_cache_dfs(mint* EEZ_NTT897_RESTRICT base,usize block_size,usize leaf_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    if(block_size==leaf_size*64){
        forward_cache_block<convert_input>(base,block_size,layer,block,blocks_at_layer,rotation);
        return;
    }
    const usize stride=block_size>>2;
    if constexpr(convert_input)forward_radix4_block_lazy<true,true>(base,stride,montgomery_one);
    else if(block==0)forward_radix4_block_lazy<true>(base,stride,montgomery_one);
    else forward_radix4_block_lazy<false>(base,stride,rotation[layer]);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));
    const usize child_size=block_size>>2;
    if(child_size==leaf_size)return;
    for(usize child=0;child<4;++child)forward_cache_dfs<false>(base+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,rotation);
}

template<bool convert_input=false>
EEZ_NTT897_ALWAYS_INLINE void forward_cache_pair_node(mint* EEZ_NTT897_RESTRICT a,mint* EEZ_NTT897_RESTRICT b,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    const usize stride=block_size>>2;
    if constexpr(convert_input)forward_radix4_block_pair_lazy<true,true>(a,b,stride,montgomery_one);
    else if(block==0)forward_radix4_block_pair_lazy<true>(a,b,stride,montgomery_one);
    else forward_radix4_block_pair_lazy<false>(a,b,stride,rotation[layer]);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));
}

template<bool convert_input=false>
inline void forward_cache_pair_block(mint* EEZ_NTT897_RESTRICT a,mint* EEZ_NTT897_RESTRICT b,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    forward_cache_pair_node<convert_input>(a,b,block_size,layer,block,blocks_at_layer,rotation);
    const usize child_size=block_size>>2;
    for(usize child=0;child<4;++child){
        mint* const pa=a+child*child_size;
        mint* const pb=b+child*child_size;
        const usize cb=block*4+child;
        forward_cache_pair_node(pa,pb,child_size,layer+1,cb,blocks_at_layer*4,rotation);
        const usize gsize=child_size>>2;
        for(usize g=0;g<4;++g)forward_cache_pair_node(pa+g*gsize,pb+g*gsize,gsize,layer+2,cb*4+g,blocks_at_layer*16,rotation);
    }
}

template<bool convert_input=false>
inline void forward_cache_pair_dfs(mint* EEZ_NTT897_RESTRICT a,mint* EEZ_NTT897_RESTRICT b,usize block_size,usize leaf_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    if(block_size==leaf_size*64){
        forward_cache_pair_block<convert_input>(a,b,block_size,layer,block,blocks_at_layer,rotation);
        return;
    }
    forward_cache_pair_node<convert_input>(a,b,block_size,layer,block,blocks_at_layer,rotation);
    const usize child_size=block_size>>2;
    if(child_size==leaf_size)return;
    for(usize child=0;child<4;++child)forward_cache_pair_dfs<false>(a+child*child_size,b+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,rotation);
}

template<bool apply_scale,bool convert_output=false,bool direct_output=false,bool shrink_output=false>
EEZ_NTT897_ALWAYS_INLINE void inverse_cache_node(mint* EEZ_NTT897_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>& rotation) noexcept{
    const usize stride=block_size>>2;
    if(block==0)inverse_radix4_block_lazy<true,apply_scale,convert_output,direct_output,shrink_output>(base,stride,montgomery_one,scale);
    else inverse_radix4_block_lazy<false,apply_scale,convert_output,direct_output,shrink_output>(base,stride,rotation[layer],scale);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],inverse_rate3(twiddle_index(static_cast<u32>(block)))));
}

template<bool scale_leaf,bool convert_output=false,bool direct_output=false,bool shrink_output=false>
inline void inverse_cache_block(mint* EEZ_NTT897_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>& rotation) noexcept{
    const usize child_size=block_size>>2;
    const usize gsize=child_size>>2;
    for(usize child=0;child<4;++child){
        mint* const p=base+child*child_size;
        const usize cb=block*4+child;
        for(usize g=0;g<4;++g)inverse_cache_node<scale_leaf>(p+g*gsize,gsize,layer+2,cb*4+g,blocks_at_layer*16,scale,rotation);
        inverse_cache_node<false>(p,child_size,layer+1,cb,blocks_at_layer*4,scale,rotation);
    }
    inverse_cache_node<false,convert_output,direct_output,shrink_output>(base,block_size,layer,block,blocks_at_layer,scale,rotation);
}

template<bool scale_leaf,bool convert_output=false,bool direct_output=false,bool shrink_output=false>
inline void inverse_cache_dfs(mint* EEZ_NTT897_RESTRICT base,usize block_size,usize leaf_size,unsigned layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>& rotation) noexcept{
    if(block_size==leaf_size*64){
        inverse_cache_block<scale_leaf,convert_output,direct_output,shrink_output>(base,block_size,layer,block,blocks_at_layer,scale,rotation);
        return;
    }
    const usize child_size=block_size>>2;
    if(child_size!=leaf_size){
        for(usize child=0;child<4;++child)inverse_cache_dfs<scale_leaf,false>(base+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,scale,rotation);
    }
    const usize stride=block_size>>2;
    if constexpr(scale_leaf){
        if(child_size==leaf_size){
            if(block==0)inverse_radix4_block_lazy<true,true,convert_output,direct_output,shrink_output>(base,stride,montgomery_one,scale);
            else inverse_radix4_block_lazy<false,true,convert_output,direct_output,shrink_output>(base,stride,rotation[layer],scale);
        }else if(block==0)inverse_radix4_block_lazy<true,false,convert_output,direct_output,shrink_output>(base,stride,montgomery_one,scale);
        else inverse_radix4_block_lazy<false,false,convert_output,direct_output,shrink_output>(base,stride,rotation[layer],scale);
    }else if(block==0)inverse_radix4_block_lazy<true,false,convert_output,direct_output,shrink_output>(base,stride,montgomery_one,scale);
    else inverse_radix4_block_lazy<false,false,convert_output,direct_output,shrink_output>(base,stride,rotation[layer],scale);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],inverse_rate3(twiddle_index(static_cast<u32>(block)))));
}

EEZ_NTT897_ALWAYS_INLINE void convert_to_montgomery(mint* a,usize n) noexcept{
    constexpr word r2=780610957u;
    const vec vr2=broadcast(r2);
    const vec vn=broadcast(r2*montgomery_ninv);
    for(usize i=0;i<n;i+=8)store8_aligned(a+i,mul8_fixed(load8_aligned(a+i),vr2,vn));
}

EEZ_NTT897_ALWAYS_INLINE void convert_from_montgomery(mint* a,usize n) noexcept{
    const vec one=broadcast(1);
    const vec ninv=broadcast(montgomery_ninv);
    for(usize i=0;i<n;i+=8)store8_aligned(a+i,canonicalize8(mul8_fixed(load8_aligned(a+i),one,ninv)));
}

template<bool convert_input=false>
inline void forward_adaptive(mint* a,usize n,unsigned leaf_log) noexcept{
    const usize leaf_size=usize(1)<<leaf_log;
    if(n==leaf_size){
        if constexpr(convert_input)convert_to_montgomery(a,n);
        return;
    }
    std::array<word,max_log/2+1> rotation{};
    rotation.fill(canonicalize(montgomery_one));
    if((std::countr_zero(n)-leaf_log)&1u){
        forward_radix2_first(a,n);
        if constexpr(convert_input)convert_to_montgomery(a,n);
        forward_cache_dfs(a,n>>1,leaf_size,0,0,2,rotation);
        forward_cache_dfs(a+(n>>1),n>>1,leaf_size,0,1,2,rotation);
        return;
    }
    forward_cache_dfs<convert_input>(a,n,leaf_size,0,0,1,rotation);
}

template<bool convert_input=false>
inline void forward_adaptive_pair(mint* EEZ_NTT897_RESTRICT a,mint* EEZ_NTT897_RESTRICT b,usize n,unsigned leaf_log) noexcept{
    const usize leaf_size=usize(1)<<leaf_log;
    if(n==leaf_size){
        if constexpr(convert_input){
            convert_to_montgomery(a,n);
            convert_to_montgomery(b,n);
        }
        return;
    }
    std::array<word,max_log/2+1> rotation{};
    rotation.fill(canonicalize(montgomery_one));
    if((std::countr_zero(n)-leaf_log)&1u){
        forward_radix2_first(a,n);
        forward_radix2_first(b,n);
        if constexpr(convert_input){
            convert_to_montgomery(a,n);
            convert_to_montgomery(b,n);
        }
        forward_cache_pair_dfs(a,b,n>>1,leaf_size,0,0,2,rotation);
        forward_cache_pair_dfs(a+(n>>1),b+(n>>1),n>>1,leaf_size,0,1,2,rotation);
        return;
    }
    forward_cache_pair_dfs<convert_input>(a,b,n,leaf_size,0,0,1,rotation);
}

template<bool convert_output=false,bool direct_output=false>
inline void inverse_adaptive(mint* a,usize n,unsigned leaf_log) noexcept{
    const usize leaf_size=usize(1)<<leaf_log;
    if(n==leaf_size){
        if constexpr(convert_output)convert_from_montgomery(a,n);
        return;
    }
    const word scale=mint::raw(static_cast<word>(n>>leaf_log)).inv().a;
    std::array<word,max_log/2+1> rotation{};
    rotation.fill(canonicalize(montgomery_one));
    if((std::countr_zero(n)-leaf_log)&1u){
        inverse_cache_dfs<true>(a,n>>1,leaf_size,0,0,2,scale,rotation);
        inverse_cache_dfs<true>(a+(n>>1),n>>1,leaf_size,0,1,2,scale,rotation);
        const usize half=n>>1;
        for(usize i=0;i<half;i+=8){
            const vec x=shrink4_to_2(load8_aligned(a+i));
            const vec y=shrink4_to_2(load8_aligned(a+half+i));
            vec z0=lazy_add8(x,y);
            vec z1=lazy_sub8(x,y);
            if constexpr(convert_output){
                const vec one=broadcast(1);
                const vec ninv=broadcast(montgomery_ninv);
                z0=canonicalize8(mul8_fixed(z0,one,ninv));
                z1=canonicalize8(mul8_fixed(z1,one,ninv));
            }else if constexpr(direct_output){
                z0=canonicalize8(shrink4_to_2(z0));
                z1=canonicalize8(shrink4_to_2(z1));
            }else{
                z0=shrink4_to_2(z0);
                z1=shrink4_to_2(z1);
            }
            store8_aligned(a+i,z0);
            store8_aligned(a+half+i,z1);
        }
        return;
    }
    inverse_cache_dfs<true,convert_output,direct_output,true>(a,n,leaf_size,0,0,1,scale,rotation);
}

EEZ_NTT897_ALWAYS_INLINE __m128i reduce_four_accumulators(vec x) noexcept{
    const vec ninv=broadcast(montgomery_ninv);
    const vec prime=broadcast(mod);
    const vec q=_mm256_mul_epu32(x,ninv);
    const vec sum=_mm256_add_epi64(x,_mm256_mul_epu32(q,prime));
    const vec high=_mm256_bsrli_epi128(sum,4);
    const vec packed=_mm256_permutevar8x32_epi32(high,_mm256_setr_epi32(0,2,4,6,0,0,0,0));
    return _mm256_castsi256_si128(packed);
}

EEZ_NTT897_ALWAYS_INLINE vec reduce_eight_accumulators(vec even,vec odd) noexcept{
    const vec ninv=broadcast(montgomery_ninv);
    const vec prime=broadcast(mod);
    const vec qe=_mm256_mul_epu32(even,ninv);
    const vec qo=_mm256_mul_epu32(odd,ninv);
    const vec re=_mm256_add_epi64(even,_mm256_mul_epu32(qe,prime));
    const vec ro=_mm256_add_epi64(odd,_mm256_mul_epu32(qo,prime));
    return shrink4_to_2(_mm256_or_si256(_mm256_bsrli_epi128(re,4),ro));
}

EEZ_NTT897_ALWAYS_INLINE void leaf_copyfree8x4(mint* EEZ_NTT897_RESTRICT a,usize first_block,const std::array<word,4>& modulus) noexcept{
    alignas(64) word lhs[4][16];
    alignas(64) word rhs[4][8];
    alignas(64) vec even[4]{};
    alignas(64) vec odd[4]{};
    for(unsigned k=0;k<4;++k){
        const usize off=(first_block+k)*8;
        const vec x=canonicalize8(shrink4_to_2(load8_aligned(a+off)));
        const vec y=canonicalize8(mul8_fixed(x,broadcast(780610957u),broadcast(780610957u*montgomery_ninv)));
        const word w=canonicalize(modulus[k]);
        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]),mul8_fixed(x,broadcast(w),broadcast(w*montgomery_ninv)));
        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]+8),x);
        _mm256_store_si256(reinterpret_cast<vec*>(rhs[k]),y);
    }
    for(unsigned i=0;i<8;++i){
        for(unsigned k=0;k<4;++k){
            const usize off=(first_block+k)*8;
            const vec y=broadcast(rhs[k][i]);
            const vec x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[k]+8-i));
            even[k]=_mm256_add_epi64(even[k],_mm256_mul_epu32(y,x));
            odd[k]=_mm256_add_epi64(odd[k],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));
        }
    }
    for(unsigned k=0;k<4;++k)store8_aligned(a+(first_block+k)*8,reduce_eight_accumulators(even[k],odd[k]));
}
EEZ_NTT897_ALWAYS_INLINE void leaf_copyfree16x2(mint* EEZ_NTT897_RESTRICT a,usize first_block,const std::array<word,2>& modulus) noexcept{
    const vec split=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    alignas(64) word lhs[6][16];
    alignas(64) word rhs[6][8];
    alignas(64) vec even[6]{};
    alignas(64) vec odd[6]{};
    word w[2];
    for(unsigned k=0;k<2;++k){
        const usize off=(first_block+k)*16;
        const vec ax0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off))),split);
        const vec ax1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off+8))),split);
        const vec bx0=canonicalize8(mul8_fixed(ax0,broadcast(780610957u),broadcast(780610957u*montgomery_ninv)));
        const vec bx1=canonicalize8(mul8_fixed(ax1,broadcast(780610957u),broadcast(780610957u*montgomery_ninv)));
        const vec ae=_mm256_permute2x128_si256(ax0,ax1,0x20);
        const vec ao=_mm256_permute2x128_si256(ax0,ax1,0x31);
        const vec be=_mm256_permute2x128_si256(bx0,bx1,0x20);
        const vec bo=_mm256_permute2x128_si256(bx0,bx1,0x31);
        const vec lx[3]{ae,ao,canonicalize8(add8(ae,ao))};
        const vec ry[3]{be,bo,canonicalize8(add8(be,bo))};
        w[k]=canonicalize(modulus[k]);
        const vec vw=broadcast(w[k]);
        const vec vn=broadcast(w[k]*montgomery_ninv);
        for(unsigned j=0;j<3;++j){
            const unsigned p=k*3+j;
            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]),mul8_fixed(lx[j],vw,vn));
            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]+8),lx[j]);
            _mm256_store_si256(reinterpret_cast<vec*>(rhs[p]),ry[j]);
        }
    }
    for(unsigned i=0;i<8;++i){
        for(unsigned p=0;p<6;++p){
            const vec y=broadcast(rhs[p][i]);
            const vec x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[p]+8-i));
            even[p]=_mm256_add_epi64(even[p],_mm256_mul_epu32(y,x));
            odd[p]=_mm256_add_epi64(odd[p],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));
        }
    }
    for(unsigned k=0;k<2;++k){
        const vec p0=reduce_eight_accumulators(even[k*3],odd[k*3]);
        const vec p1=reduce_eight_accumulators(even[k*3+1],odd[k*3+1]);
        const vec p2=reduce_eight_accumulators(even[k*3+2],odd[k*3+2]);
        vec yp1=_mm256_permutevar8x32_epi32(p1,_mm256_setr_epi32(7,0,1,2,3,4,5,6));
        yp1=_mm256_insert_epi32(yp1,canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1,7)),w[k])),0);
        const vec ce=add8(p0,yp1);
        const vec co=sub8(sub8(p2,p0),p1);
        const vec lo=_mm256_unpacklo_epi32(ce,co);
        const vec hi=_mm256_unpackhi_epi32(ce,co);
        const usize off=(first_block+k)*16;
        store8_aligned(a+off,_mm256_permute2x128_si256(lo,hi,0x20));
        store8_aligned(a+off+8,_mm256_permute2x128_si256(lo,hi,0x31));
    }
}
template<unsigned leaf_size,unsigned parallel_blocks>
inline void leaf_copyfree(mint* a,usize n) noexcept{
    const usize blocks=n/leaf_size;
    word w=montgomery_one;
    for(usize s=0;s<blocks;s+=parallel_blocks){
        std::array<word,parallel_blocks> modulus{};
        for(unsigned k=0;k<parallel_blocks;++k){
            modulus[k]=w;
            const usize block=s+k;
            if(block+1<blocks)w=mul(w,forward_rate1(twiddle_index(static_cast<u32>(block))));
        }
        if constexpr(leaf_size==8)leaf_copyfree8x4(a,s,modulus);
        else leaf_copyfree16x2(a,s,modulus);
    }
}
EEZ_NTT897_ALWAYS_INLINE void leaf_product8x4(mint* EEZ_NTT897_RESTRICT a,mint* EEZ_NTT897_RESTRICT b,usize first_block,const std::array<word,4>& modulus) noexcept{
    alignas(64) word lhs[4][16];
    alignas(64) vec even[4]{};
    alignas(64) vec odd[4]{};
    for(unsigned k=0;k<4;++k){
        const usize off=(first_block+k)*8;
        const vec x=canonicalize8(shrink4_to_2(load8_aligned(a+off)));
        const vec y=canonicalize8(shrink4_to_2(load8_aligned(b+off)));
        const word w=canonicalize(modulus[k]);
        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]),mul8_fixed(x,broadcast(w),broadcast(w*montgomery_ninv)));
        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]+8),x);
        store8_aligned(b+off,y);
    }
    for(unsigned i=0;i<8;++i){
        for(unsigned k=0;k<4;++k){
            const usize off=(first_block+k)*8;
            const vec y=broadcast(raw(b[off+i]));
            const vec x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[k]+8-i));
            even[k]=_mm256_add_epi64(even[k],_mm256_mul_epu32(y,x));
            odd[k]=_mm256_add_epi64(odd[k],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));
        }
    }
    for(unsigned k=0;k<4;++k)store8_aligned(a+(first_block+k)*8,reduce_eight_accumulators(even[k],odd[k]));
}

EEZ_NTT897_ALWAYS_INLINE void leaf_product16x2_karatsuba(mint* EEZ_NTT897_RESTRICT a,const mint* EEZ_NTT897_RESTRICT b,usize first_block,const std::array<word,2>& modulus) noexcept{
    const vec split=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    alignas(64) word lhs[6][16];
    alignas(64) word rhs[6][8];
    alignas(64) vec even[6]{};
    alignas(64) vec odd[6]{};
    word w[2];
    for(unsigned k=0;k<2;++k){
        const usize off=(first_block+k)*16;
        const vec ax0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off))),split);
        const vec ax1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off+8))),split);
        const vec bx0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(b+off))),split);
        const vec bx1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(b+off+8))),split);
        const vec ae=_mm256_permute2x128_si256(ax0,ax1,0x20);
        const vec ao=_mm256_permute2x128_si256(ax0,ax1,0x31);
        const vec be=_mm256_permute2x128_si256(bx0,bx1,0x20);
        const vec bo=_mm256_permute2x128_si256(bx0,bx1,0x31);
        const vec lx[3]{ae,ao,canonicalize8(add8(ae,ao))};
        const vec ry[3]{be,bo,canonicalize8(add8(be,bo))};
        w[k]=canonicalize(modulus[k]);
        const vec vw=broadcast(w[k]);
        const vec vn=broadcast(w[k]*montgomery_ninv);
        for(unsigned j=0;j<3;++j){
            const unsigned p=k*3+j;
            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]),mul8_fixed(lx[j],vw,vn));
            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]+8),lx[j]);
            _mm256_store_si256(reinterpret_cast<vec*>(rhs[p]),ry[j]);
        }
    }
    for(unsigned i=0;i<8;++i){
        for(unsigned p=0;p<6;++p){
            const vec y=broadcast(rhs[p][i]);
            const vec x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[p]+8-i));
            even[p]=_mm256_add_epi64(even[p],_mm256_mul_epu32(y,x));
            odd[p]=_mm256_add_epi64(odd[p],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));
        }
    }
    for(unsigned k=0;k<2;++k){
        const vec p0=reduce_eight_accumulators(even[k*3],odd[k*3]);
        const vec p1=reduce_eight_accumulators(even[k*3+1],odd[k*3+1]);
        const vec p2=reduce_eight_accumulators(even[k*3+2],odd[k*3+2]);
        vec yp1=_mm256_permutevar8x32_epi32(p1,_mm256_setr_epi32(7,0,1,2,3,4,5,6));
        yp1=_mm256_insert_epi32(yp1,canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1,7)),w[k])),0);
        const vec ce=add8(p0,yp1);
        const vec co=sub8(sub8(p2,p0),p1);
        const vec lo=_mm256_unpacklo_epi32(ce,co);
        const vec hi=_mm256_unpackhi_epi32(ce,co);
        const usize off=(first_block+k)*16;
        store8_aligned(a+off,_mm256_permute2x128_si256(lo,hi,0x20));
        store8_aligned(a+off+8,_mm256_permute2x128_si256(lo,hi,0x31));
    }
}

EEZ_NTT897_ALWAYS_INLINE word twice(word x) noexcept{return x+x;}

EEZ_NTT897_ALWAYS_INLINE vec pack4_u32(word x0,word x1,word x2,word x3) noexcept{
    return _mm256_cvtepu32_epi64(_mm_setr_epi32(static_cast<int>(x0),static_cast<int>(x1),static_cast<int>(x2),static_cast<int>(x3)));
}

EEZ_NTT897_ALWAYS_INLINE vec mul4_u32(word a0,word b0,word a1,word b1,word a2,word b2,word a3,word b3) noexcept{
    return _mm256_mul_epu32(pack4_u32(a0,a1,a2,a3),pack4_u32(b0,b1,b2,b3));
}

EEZ_NTT897_ALWAYS_INLINE u64 hsum4_u64(vec x) noexcept{
    __m128i s=_mm_add_epi64(_mm256_castsi256_si128(x),_mm256_extracti128_si256(x,1));
    s=_mm_add_epi64(s,_mm_srli_si128(s,8));
    return static_cast<u64>(_mm_cvtsi128_si64(s));
}

EEZ_NTT897_ALWAYS_INLINE vec square8_packed(vec vx,word w) noexcept{
    alignas(32) word x[8],xw[8];
    vx=canonicalize8(shrink4_to_2(vx));
    w=canonicalize(w);
    _mm256_store_si256(reinterpret_cast<vec*>(x),vx);
    _mm256_store_si256(reinterpret_cast<vec*>(xw),mul8_fixed(vx,broadcast(w),broadcast(w*montgomery_ninv)));

    u64 a0=hsum4_u64(mul4_u32(x[0],x[0],twice(xw[1]),x[7],twice(xw[2]),x[6],twice(xw[3]),x[5]));
    const u64 a1=hsum4_u64(mul4_u32(twice(x[0]),x[1],twice(xw[2]),x[7],twice(xw[3]),x[6],twice(xw[4]),x[5]));
    u64 a2=hsum4_u64(mul4_u32(twice(x[0]),x[2],x[1],x[1],twice(xw[3]),x[7],twice(xw[4]),x[6]));
    const u64 a3=hsum4_u64(mul4_u32(twice(x[0]),x[3],twice(x[1]),x[2],twice(xw[4]),x[7],twice(xw[5]),x[6]));
    u64 a4=hsum4_u64(mul4_u32(twice(x[0]),x[4],twice(x[1]),x[3],x[2],x[2],twice(xw[5]),x[7]));
    const u64 a5=hsum4_u64(mul4_u32(twice(x[0]),x[5],twice(x[1]),x[4],twice(x[2]),x[3],twice(xw[6]),x[7]));
    u64 a6=hsum4_u64(mul4_u32(twice(x[0]),x[6],twice(x[1]),x[5],twice(x[2]),x[4],x[3],x[3]));
    const u64 a7=hsum4_u64(mul4_u32(twice(x[0]),x[7],twice(x[1]),x[6],twice(x[2]),x[5],twice(x[3]),x[4]));

    const vec extra=mul4_u32(xw[4],x[4],xw[5],x[5],xw[6],x[6],xw[7],x[7]);
    alignas(32) u64 e[4];
    _mm256_store_si256(reinterpret_cast<vec*>(e),extra);
    a0+=e[0];
    a2+=e[1];
    a4+=e[2];
    a6+=e[3];

    const vec lo=_mm256_setr_epi64x(static_cast<long long>(a0),static_cast<long long>(a1),static_cast<long long>(a2),static_cast<long long>(a3));
    const vec hi=_mm256_setr_epi64x(static_cast<long long>(a4),static_cast<long long>(a5),static_cast<long long>(a6),static_cast<long long>(a7));
    return shrink4_to_2(_mm256_set_m128i(reduce_four_accumulators(hi),reduce_four_accumulators(lo)));
}

EEZ_NTT897_ALWAYS_INLINE void leaf_square8x4(mint* EEZ_NTT897_RESTRICT a,usize first_block,const std::array<word,4>& modulus) noexcept{
    for(unsigned k=0;k<4;++k){
        const usize off=(first_block+k)*8;
        store8_aligned(a+off,square8_packed(load8_aligned(a+off),modulus[k]));
    }
}

EEZ_NTT897_ALWAYS_INLINE void leaf_square16x2_karatsuba(mint* EEZ_NTT897_RESTRICT a,usize first_block,const std::array<word,2>& modulus) noexcept{
    const vec split=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    for(unsigned k=0;k<2;++k){
        const usize off=(first_block+k)*16;
        const vec x0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off))),split);
        const vec x1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off+8))),split);
        const vec xe=_mm256_permute2x128_si256(x0,x1,0x20);
        const vec xo=_mm256_permute2x128_si256(x0,x1,0x31);
        const vec xs=canonicalize8(add8(xe,xo));
        const word w=canonicalize(modulus[k]);
        const vec p0=square8_packed(xe,w);
        const vec p1=square8_packed(xo,w);
        const vec p2=square8_packed(xs,w);
        vec yp1=_mm256_permutevar8x32_epi32(p1,_mm256_setr_epi32(7,0,1,2,3,4,5,6));
        yp1=_mm256_insert_epi32(yp1,canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1,7)),w)),0);
        const vec ce=add8(p0,yp1);
        const vec co=sub8(sub8(p2,p0),p1);
        const vec lo=_mm256_unpacklo_epi32(ce,co);
        const vec hi=_mm256_unpackhi_epi32(ce,co);
        store8_aligned(a+off,_mm256_permute2x128_si256(lo,hi,0x20));
        store8_aligned(a+off+8,_mm256_permute2x128_si256(lo,hi,0x31));
    }
}

template<unsigned leaf_size,unsigned parallel_blocks>
inline void leaf_products(mint* a,mint* b,usize n) noexcept{
    const usize blocks=n/leaf_size;
    word w=montgomery_one;
    for(usize s=0;s<blocks;s+=parallel_blocks){
        std::array<word,parallel_blocks> modulus{};
        for(unsigned k=0;k<parallel_blocks;++k){
            modulus[k]=w;
            const usize block=s+k;
            if(block+1<blocks)w=mul(w,forward_rate1(twiddle_index(static_cast<u32>(block))));
        }
        if constexpr(leaf_size==8)leaf_product8x4(a,b,s,modulus);
        else leaf_product16x2_karatsuba(a,b,s,modulus);
    }
}

template<unsigned leaf_size,unsigned parallel_blocks>
inline void leaf_squares(mint* a,usize n) noexcept{
    const usize blocks=n/leaf_size;
    word w=montgomery_one;
    for(usize s=0;s<blocks;s+=parallel_blocks){
        std::array<word,parallel_blocks> modulus{};
        for(unsigned k=0;k<parallel_blocks;++k){
            modulus[k]=w;
            const usize block=s+k;
            if(block+1<blocks)w=mul(w,forward_rate1(twiddle_index(static_cast<u32>(block))));
        }
        if constexpr(leaf_size==8)leaf_square8x4(a,s,modulus);
        else leaf_square16x2_karatsuba(a,s,modulus);
    }
}

inline void convolution_adaptive_inplace(mint* a,mint* b,usize n) noexcept{
    const unsigned leaf_log=adaptive_leaf_log(n);
    forward_adaptive_pair(a,b,n,leaf_log);
    if(leaf_log==3)leaf_products<8,4>(a,b,n);
    else leaf_products<16,2>(a,b,n);
    inverse_adaptive(a,n,leaf_log);
}

inline void square_adaptive_inplace(mint* a,usize n) noexcept{
    const unsigned leaf_log=adaptive_leaf_log(n);
    forward_adaptive(a,n,leaf_log);
    if(leaf_log==3)leaf_squares<8,4>(a,n);
    else leaf_squares<16,2>(a,n);
    inverse_adaptive(a,n,leaf_log);
}

inline void convolution_adaptive_normal_inplace(mint* a,mint* b,usize n) noexcept{
    const unsigned leaf_log=adaptive_leaf_log(n);
    forward_adaptive_pair<true>(a,b,n,leaf_log);
    if(leaf_log==3)leaf_products<8,4>(a,b,n);
    else leaf_products<16,2>(a,b,n);
    inverse_adaptive<true>(a,n,leaf_log);
}

inline void convolution_adaptive_mixed_normal_inplace(mint* a,mint* b,usize n) noexcept{
    const unsigned leaf_log=adaptive_leaf_log(n);
    forward_adaptive_pair(a,b,n,leaf_log);
    if(leaf_log==3)leaf_products<8,4>(a,b,n);
    else leaf_products<16,2>(a,b,n);
    inverse_adaptive<false,true>(a,n,leaf_log);
}

#endif

inline void pointwise_multiply(mint* EEZ_NTT897_RESTRICT a,const mint* EEZ_NTT897_RESTRICT b,usize n) noexcept{
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    for(;i+8<=n;i+=8)store8(a+i,mul8(load8(a+i),load8(b+i)));
#endif
    for(;i<n;++i)a[i]=from_raw(mul(raw(a[i]),raw(b[i])));
}

inline void pointwise_square(mint* EEZ_NTT897_RESTRICT a,usize n) noexcept{
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    for(;i+8<=n;i+=8){
        const vec x=load8(a+i);
        store8(a+i,mul8(x,x));
    }
#endif
    for(;i<n;++i)a[i]=from_raw(mul(raw(a[i]),raw(a[i])));
}

}

inline void forward(std::span<mint> a) noexcept{
    if(a.size()<=1)return;
    assert(valid_ntt_size(a.size()));
    detail::forward_dif(a.data(),a.size());
}

inline void inverse(std::span<mint> a) noexcept{
    if(a.size()<=1)return;
    assert(valid_ntt_size(a.size()));
    detail::inverse_dit(a.data(),a.size());
}

namespace detail{

inline std::vector<mint> convolution_naive(std::span<const mint> a,std::span<const mint> b){
    std::vector<mint> result(convolution_size(a.size(),b.size()));
    for(usize i=0;i<a.size();++i)
        for(usize j=0;j<b.size();++j)
            result[i+j]+=a[i]*b[j];
    return result;
}

inline std::vector<mint> square_naive(std::span<const mint> a){
    std::vector<mint> result(convolution_size(a.size(),a.size()));
    for(usize i=0;i<a.size();++i){
        result[2*i]+=a[i]*a[i];
        for(usize j=i+1;j<a.size();++j){
            const mint p=a[i]*a[j];
            result[i+j]+=p+p;
        }
    }
    return result;
}

inline usize checked_transform_size(usize n,usize m){
    const usize result=convolution_transform_size(n,m);
    if(n&&m&&!result)throw std::length_error("eez::ntt897: convolution exceeds the 2^25 transform limit");
    return result;
}

inline void require_ntt_size(usize n){
    if(!valid_ntt_size(n))throw std::invalid_argument("eez::ntt897: transform length must be a power of two in [1, 2^23]");
}

}

#if defined(__AVX2__) || defined(_M_AVX2)

using convolution_buffer=detail::aligned_vector;

inline void convolution_inplace(convolution_buffer& a,convolution_buffer& b){
    if(a.size()!=b.size()||!valid_convolution_transform_size(a.size()))
        throw std::invalid_argument("eez::ntt897::convolution_inplace: buffer sizes must match and be a power of two in [32, 2^25]");
    detail::convolution_adaptive_inplace(a.data(),b.data(),a.size());
}

#endif

inline std::vector<mint> convolution(std::span<const mint> a,std::span<const mint> b){
    if(a.empty()||b.empty())return {};
    if(a.data()==b.data()&&a.size()==b.size())return square(a);
    if(std::min(a.size(),b.size())<=naive_cutoff)return detail::convolution_naive(a,b);

    const usize result_size=convolution_size(a.size(),b.size());
    const usize n=detail::checked_transform_size(a.size(),b.size());
    detail::aligned_vector fa(n),fb(n);
    std::copy(a.begin(),a.end(),fa.begin());
    std::copy(b.begin(),b.end(),fb.begin());
    detail::convolution_adaptive_inplace(fa.data(),fb.data(),n);

    std::vector<mint> result(result_size);
    std::copy_n(fa.data(),result_size,result.data());
    return result;
}

inline std::vector<mint> square(std::span<const mint> a){
    if(a.empty())return {};
    if(a.size()<=naive_cutoff)return detail::square_naive(a);

    const usize result_size=convolution_size(a.size(),a.size());
    const usize n=detail::checked_transform_size(a.size(),a.size());
    detail::aligned_vector fa(n);
    std::copy(a.begin(),a.end(),fa.begin());
    detail::square_adaptive_inplace(fa.data(),n);

    std::vector<mint> result(result_size);
    std::copy_n(fa.data(),result_size,result.data());
    return result;
}

inline void convolution_to(std::span<const mint> a,std::span<const mint> b,std::span<mint> out,workspace& ws){
    if(a.empty()||b.empty())return;
    const usize result_size=convolution_size(a.size(),b.size());
    if(out.size()<result_size)throw std::invalid_argument("eez::ntt897::convolution_to: output span is too small");

    if(a.data()==b.data()&&a.size()==b.size()){
        square_to(a,out,ws);
        return;
    }

    if(std::min(a.size(),b.size())<=naive_cutoff){
        std::fill_n(out.begin(),result_size,mint{});
        for(usize i=0;i<a.size();++i)
            for(usize j=0;j<b.size();++j)
                out[i+j]+=a[i]*b[j];
        return;
    }

    const usize n=detail::checked_transform_size(a.size(),b.size());
    ws.reserve(n);
    std::fill_n(ws.a_.begin(),n,mint{});
    std::fill_n(ws.b_.begin(),n,mint{});
    std::copy(a.begin(),a.end(),ws.a_.begin());
    std::copy(b.begin(),b.end(),ws.b_.begin());
    detail::convolution_adaptive_inplace(ws.a_.data(),ws.b_.data(),n);
    std::copy_n(ws.a_.begin(),result_size,out.begin());
}

inline void square_to(std::span<const mint> a,std::span<mint> out,workspace& ws){
    if(a.empty())return;
    const usize result_size=convolution_size(a.size(),a.size());
    if(out.size()<result_size)throw std::invalid_argument("eez::ntt897::square_to: output span is too small");

    if(a.size()<=naive_cutoff){
        std::fill_n(out.begin(),result_size,mint{});
        for(usize i=0;i<a.size();++i){
            out[2*i]+=a[i]*a[i];
            for(usize j=i+1;j<a.size();++j){
                const mint p=a[i]*a[j];
                out[i+j]+=p+p;
            }
        }
        return;
    }

    const usize n=detail::checked_transform_size(a.size(),a.size());
    ws.reserve(n);
    std::fill_n(ws.a_.begin(),n,mint{});
    std::copy(a.begin(),a.end(),ws.a_.begin());
    detail::square_adaptive_inplace(ws.a_.data(),n);
    std::copy_n(ws.a_.begin(),result_size,out.begin());
}

inline void forward_to(std::span<const mint> src,frequency_buffer& dst,usize n){
    detail::require_ntt_size(n);
    if(src.size()>n)throw std::invalid_argument("eez::ntt897::forward_to: source is longer than transform");
    dst.data_.assign(n,mint{});
    std::copy(src.begin(),src.end(),dst.data_.begin());
    detail::forward_dif(dst.data_.data(),n);
}

inline void pointwise_multiply(frequency_buffer& lhs,const frequency_buffer& rhs){
    if(lhs.size()!=rhs.size())throw std::invalid_argument("eez::ntt897::pointwise_multiply: transform sizes differ");
    detail::pointwise_multiply(lhs.data_.data(),rhs.data_.data(),lhs.data_.size());
}

inline void pointwise_square(frequency_buffer& a){
    detail::pointwise_square(a.data_.data(),a.data_.size());
}

inline void inverse_to(frequency_buffer& src,std::span<mint> out){
    if(src.data_.empty()){
        if(!out.empty())throw std::invalid_argument("eez::ntt897::inverse_to: empty transform");
        return;
    }
    if(out.size()>src.data_.size())throw std::invalid_argument("eez::ntt897::inverse_to: output is longer than transform");
    detail::inverse_dit(src.data_.data(),src.data_.size());
    std::copy_n(src.data_.begin(),out.size(),out.begin());
}

}

#undef EEZ_NTT897_ALWAYS_INLINE
#undef EEZ_NTT897_RESTRICT


// Public streaming API preserving submission #393594's mixed-normal path.

// read(): next coefficient in [0, mod); called for n values, then m values.

// write(u32): receives n+m-1 canonical coefficients, or none for an empty input.

// This API never exposes normal-representation data as ordinary modint values.

namespace eez::ntt897{
template<class Reader,class Writer>
inline void convolution_normal_io(usize n,usize m,Reader&& read,Writer&& write){
    if(std::min(n,m)<=naive_cutoff){
        std::vector<mint> a(n),b(m);
        for(auto& x:a)x=read();
        for(auto& x:b)x=read();
        const auto c=convolution(a,b);
        for(const auto& x:c)write(x.get());
        return;
    }
    const usize z=detail::checked_transform_size(n,m);
    const usize result_size=n+m-1;
    detail::aligned_uninitialized_buffer a(z),b(z);
    mint* const first=n<m?b.data():a.data();
    mint* const second=n<m?a.data():b.data();
    for(usize i=0;i<n;++i)
        std::construct_at(first+i,mint::montgomery_raw(read()));
    for(usize i=0;i<m;++i)
        std::construct_at(second+i,mint::montgomery_raw(read()));
    const usize a_size=std::max(n,m),b_size=std::min(n,m);
    std::uninitialized_value_construct_n(a.data()+a_size,z-a_size);
    std::uninitialized_value_construct_n(b.data()+b_size,z-b_size);
    detail::convert_to_montgomery(b.data(),(b_size+7)&~usize(7));
    detail::convolution_adaptive_mixed_normal_inplace(a.data(),b.data(),z);
    for(usize i=0;i<result_size;++i)write(a.data()[i].a);
}
}

#line 2 "convolution/ntt880.hpp"
#if !defined(__AVX2__) && !defined(_M_AVX2)
#error "Compile with -mavx2 (see README.md)."
#endif
#line 2 "ntt880.hpp"

#if defined(__GNUC__) && !defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#elif defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma clang attribute push( \
    __attribute__((target("avx2,bmi,bmi2,lzcnt,popcnt,ssse3"))), \
    apply_to = function)
#endif

#line 1968 "convolution_mod1000000007.hpp"

#line 1970 "convolution_mod1000000007.hpp"


#line 1 "math/modint880.hpp"

#line 1975 "convolution_mod1000000007.hpp"


struct modint880 {
    using u32 = std::uint32_t;
    using i32 = std::int32_t;
    using u64 = std::uint64_t;

    static constexpr u32 MOD = 880803841u;
    static constexpr u32 MOD2 = MOD * 2;
    static constexpr u32 primitive_root = 26;
    static constexpr int max_power_of_two = 23;

private:
    static constexpr u32 R = 3414163457u;
    static constexpr u32 N2 = 464649016u;

    struct montgomery_tag {};

    constexpr modint880(u32 x, montgomery_tag) : a(x) {}

    static constexpr u32 reduce(u64 x) {
        return static_cast<u32>(
            (x + u64(static_cast<u32>(x) * u32(-R)) * MOD) >> 32
        );
    }

public:
    u32 a;

    static_assert(MOD < (u32(1) << 30));
    static_assert((MOD & 1) != 0);
    static_assert(R * MOD == 1);

    constexpr modint880() : a(0) {}

    template <class T, std::enable_if_t<std::is_integral_v<T> &&
                                        std::is_signed_v<T>, int> = 0>
    constexpr modint880(T x) : a(0) {
        const std::int64_t y =
            static_cast<std::int64_t>(x) % std::int64_t(MOD) + MOD;
        a = reduce(u64(y) * N2);
    }

    template <class T, std::enable_if_t<std::is_integral_v<T> &&
                                        std::is_unsigned_v<T>, int> = 0>
    constexpr modint880(T x)
        : a(reduce(((u64(x) % MOD) + MOD) * N2)) {}

    static constexpr modint880 raw(u32 x) {
        return modint880(reduce(u64(x) * N2), montgomery_tag{});
    }

    static constexpr modint880 montgomery_raw(u32 x) {
        return modint880(x, montgomery_tag{});
    }

    static constexpr u32 mod() { return MOD; }
    static constexpr u32 get_mod() { return MOD; }

    constexpr u32 val() const {
        const u32 x = reduce(a);
        return x >= MOD ? x - MOD : x;
    }

    constexpr u32 get() const { return val(); }

    constexpr modint880& operator+=(const modint880& rhs) {
        a += rhs.a - MOD2;
        if (i32(a) < 0) a += MOD2;
        return *this;
    }

    constexpr modint880& operator-=(const modint880& rhs) {
        a -= rhs.a;
        if (i32(a) < 0) a += MOD2;
        return *this;
    }

    constexpr modint880& operator*=(const modint880& rhs) {
        a = reduce(u64(a) * rhs.a);
        return *this;
    }

    constexpr modint880& operator/=(const modint880& rhs) {
        return *this *= rhs.inv();
    }

    constexpr modint880 operator+() const { return *this; }
    constexpr modint880 operator-() const { return modint880() - *this; }

    friend constexpr modint880 operator+(modint880 lhs, const modint880& rhs) {
        return lhs += rhs;
    }

    friend constexpr modint880 operator-(modint880 lhs, const modint880& rhs) {
        return lhs -= rhs;
    }

    friend constexpr modint880 operator*(modint880 lhs, const modint880& rhs) {
        return lhs *= rhs;
    }

    friend constexpr modint880 operator/(modint880 lhs, const modint880& rhs) {
        return lhs /= rhs;
    }

    friend constexpr bool operator==(const modint880& lhs, const modint880& rhs) {
        const u32 x = lhs.a >= MOD ? lhs.a - MOD : lhs.a;
        const u32 y = rhs.a >= MOD ? rhs.a - MOD : rhs.a;
        return x == y;
    }

    friend constexpr bool operator!=(const modint880& lhs, const modint880& rhs) {
        return !(lhs == rhs);
    }

    constexpr modint880& operator++() {
        return *this += raw(1);
    }

    constexpr modint880 operator++(int) {
        modint880 old = *this;
        ++*this;
        return old;
    }

    constexpr modint880& operator--() {
        return *this -= raw(1);
    }

    constexpr modint880 operator--(int) {
        modint880 old = *this;
        --*this;
        return old;
    }

    constexpr modint880 pow(u64 exponent) const {
        modint880 result = raw(1);
        modint880 base = *this;
        while (exponent != 0) {
            if (exponent & 1) result *= base;
            base *= base;
            exponent >>= 1;
        }
        return result;
    }

    constexpr modint880 inv() const {
        assert(val() != 0);
        return pow(MOD - 2);
    }

    constexpr modint880 inverse() const { return inv(); }

    friend std::ostream& operator<<(std::ostream& os, const modint880& x) {
        return os << x.val();
    }

    friend std::istream& operator>>(std::istream& is, modint880& x) {
        std::int64_t value;
        is >> value;
        x = modint880(value);
        return is;
    }
};

static_assert(sizeof(modint880) == 4);
static_assert(std::is_trivially_copyable_v<modint880>);

using mint880 = modint880;

#line 18 "ntt880.hpp"

#if defined(_MSC_VER)
#define EEZ_NTT880_ALWAYS_INLINE __forceinline
#define EEZ_NTT880_RESTRICT __restrict
#elif defined(__GNUC__) || defined(__clang__)
#define EEZ_NTT880_ALWAYS_INLINE inline __attribute__((always_inline))
#define EEZ_NTT880_RESTRICT __restrict__
#else
#define EEZ_NTT880_ALWAYS_INLINE inline
#define EEZ_NTT880_RESTRICT
#endif

namespace eez::ntt880{

using mint=modint880;
using u32=std::uint32_t;
using usize=std::size_t;

inline constexpr u32 mod=mint::MOD;
inline constexpr usize max_ntt_size=usize(1)<<23;
inline constexpr usize max_convolution_size=usize(1)<<25;
inline constexpr usize max_size=max_ntt_size;
inline constexpr usize naive_cutoff=60;

inline void forward(std::span<mint> a) noexcept;
inline void inverse(std::span<mint> a) noexcept;
inline std::vector<mint> convolution(std::span<const mint> a,std::span<const mint> b);
inline std::vector<mint> square(std::span<const mint> a);

inline std::vector<mint> convolution(const std::vector<mint>& a,const std::vector<mint>& b){
    return convolution(std::span<const mint>(a.data(),a.size()),std::span<const mint>(b.data(),b.size()));
}

inline std::vector<mint> square(const std::vector<mint>& a){
    return square(std::span<const mint>(a.data(),a.size()));
}

namespace detail{

template<class T>
class aligned_allocator{
public:
    using value_type=T;
    using is_always_equal=std::true_type;
    aligned_allocator() noexcept=default;
    template<class U> constexpr aligned_allocator(const aligned_allocator<U>&) noexcept{}
    [[nodiscard]] T* allocate(usize n){
        return static_cast<T*>(::operator new(n*sizeof(T),std::align_val_t{64}));
    }
    void deallocate(T* p,usize) noexcept{
        ::operator delete(p,std::align_val_t{64});
    }
    template<class U> struct rebind{using other=aligned_allocator<U>;};
};

template<class T,class U>
constexpr bool operator==(const aligned_allocator<T>&,const aligned_allocator<U>&) noexcept{return true;}

template<class T,class U>
constexpr bool operator!=(const aligned_allocator<T>&,const aligned_allocator<U>&) noexcept{return false;}

using aligned_vector=std::vector<mint,aligned_allocator<mint>>;

}

class workspace{
public:
    workspace()=default;
    explicit workspace(usize n){reserve(n);}
    void reserve(usize n){
        if(a_.size()<n)a_.resize(n);
        if(b_.size()<n)b_.resize(n);
    }
    [[nodiscard]] usize capacity()const noexcept{return std::min(a_.size(),b_.size());}
private:
    friend void convolution_to(std::span<const mint>,std::span<const mint>,std::span<mint>,workspace&);
    friend void square_to(std::span<const mint>,std::span<mint>,workspace&);
    detail::aligned_vector a_;
    detail::aligned_vector b_;
};

inline void convolution_to(std::span<const mint> a,std::span<const mint> b,std::span<mint> out,workspace& ws);
inline void square_to(std::span<const mint> a,std::span<mint> out,workspace& ws);

class frequency_buffer{
public:
    frequency_buffer()=default;
    [[nodiscard]] usize size()const noexcept{return data_.size();}
private:
    friend void forward_to(std::span<const mint>,frequency_buffer&,usize);
    friend void pointwise_multiply(frequency_buffer&,const frequency_buffer&);
    friend void pointwise_square(frequency_buffer&);
    friend void inverse_to(frequency_buffer&,std::span<mint>);
    std::vector<mint> data_;
};

inline void forward_to(std::span<const mint> src,frequency_buffer& dst,usize n);
inline void pointwise_multiply(frequency_buffer& lhs,const frequency_buffer& rhs);
inline void pointwise_square(frequency_buffer& a);
inline void inverse_to(frequency_buffer& src,std::span<mint> out);

constexpr usize convolution_size(usize n,usize m) noexcept{
    return n&&m?n+m-1:0;
}

constexpr usize transform_size(usize n,usize m) noexcept{
    if(!n||!m)return 0;
    if(n>max_ntt_size||m>max_ntt_size)return 0;
    if(n>max_ntt_size-m+1)return 0;
    const usize z=n+m-1;
    usize x=1;
    while(x<z)x<<=1;
    return x;
}

constexpr usize convolution_transform_size(usize n,usize m) noexcept{
    if(!n||!m)return 0;
    if(n>max_convolution_size||m>max_convolution_size)return 0;
    if(n>max_convolution_size-m+1)return 0;
    const usize z=n+m-1;
    usize x=1;
    while(x<z)x<<=1;
    return x;
}

constexpr bool valid_ntt_size(usize n) noexcept{
    return n!=0&&(n&(n-1))==0&&n<=max_ntt_size;
}

constexpr bool valid_convolution_transform_size(usize n) noexcept{
    return n>=32&&(n&(n-1))==0&&n<=max_convolution_size;
}

namespace detail{

using word=u32;
using u64=std::uint64_t;

inline constexpr word mod=mint::MOD;
inline constexpr word mod2=2*mod;
inline constexpr unsigned max_log=23;
inline constexpr word montgomery_ninv=880803839u;
inline constexpr word montgomery_one=mint::raw(1).a;

static_assert(mod<(word(1)<<30));
static_assert(word(mod*montgomery_ninv)==~word(0));
static_assert(sizeof(mint)==sizeof(word));

EEZ_NTT880_ALWAYS_INLINE constexpr word raw(const mint& x) noexcept{return x.a;}
EEZ_NTT880_ALWAYS_INLINE constexpr mint from_raw(word x) noexcept{return mint::montgomery_raw(x);}

EEZ_NTT880_ALWAYS_INLINE constexpr word mul(word a,word b) noexcept{
    const u64 x=u64(a)*b;
    const word q=static_cast<word>(x)*montgomery_ninv;
    return static_cast<word>((x+u64(q)*mod)>>32);
}

EEZ_NTT880_ALWAYS_INLINE constexpr word add(word a,word b) noexcept{
    const word x=a+b;
    return x>=mod2?x-mod2:x;
}

EEZ_NTT880_ALWAYS_INLINE constexpr word sub(word a,word b) noexcept{
    return a>=b?a-b:a+mod2-b;
}

EEZ_NTT880_ALWAYS_INLINE constexpr word canonicalize(word a) noexcept{
    return a>=mod?a-mod:a;
}

struct twiddle_table{
    std::array<word,max_log+1> root{};
    std::array<word,max_log+1> iroot{};
    std::array<word,max_log+1> rate1{};
    std::array<word,max_log+1> rate3{};
    std::array<word,max_log+1> irate3{};

    constexpr twiddle_table(){
        root[max_log]=mint::raw(mint::primitive_root).pow((mod-1)>>max_log).a;
        iroot[max_log]=mint::montgomery_raw(root[max_log]).inv().a;
        for(int i=int(max_log)-1;i>=0;--i){
            root[usize(i)]=mul(root[usize(i+1)],root[usize(i+1)]);
            iroot[usize(i)]=mul(iroot[usize(i+1)],iroot[usize(i+1)]);
        }
        word prod=montgomery_one;
        for(unsigned i=0;i+1<=max_log;++i){
            rate1[i]=mul(root[i+1],prod);
            prod=mul(prod,iroot[i+1]);
        }
        prod=montgomery_one;
        word iprod=montgomery_one;
        for(unsigned i=0;i+3<=max_log;++i){
            rate3[i]=mul(root[i+3],prod);
            irate3[i]=mul(iroot[i+3],iprod);
            prod=mul(prod,iroot[i+3]);
            iprod=mul(iprod,root[i+3]);
        }
    }
};

inline constexpr twiddle_table twiddles{};

EEZ_NTT880_ALWAYS_INLINE word forward_rate1(unsigned i) noexcept{return twiddles.rate1[i];}
EEZ_NTT880_ALWAYS_INLINE word forward_rate3(unsigned i) noexcept{return twiddles.rate3[i];}
EEZ_NTT880_ALWAYS_INLINE word inverse_rate3(unsigned i) noexcept{return twiddles.irate3[i];}

EEZ_NTT880_ALWAYS_INLINE unsigned twiddle_index(u32 block) noexcept{
    return static_cast<unsigned>(std::countr_zero(~block));
}

#if defined(__AVX2__) || defined(_M_AVX2)

using vec=__m256i;

EEZ_NTT880_ALWAYS_INLINE vec load8(const mint* p) noexcept{
    return _mm256_loadu_si256(reinterpret_cast<const __m256i*>(static_cast<const void*>(p)));
}

EEZ_NTT880_ALWAYS_INLINE void store8(mint* p,vec x) noexcept{
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(static_cast<void*>(p)),x);
}

EEZ_NTT880_ALWAYS_INLINE vec broadcast(word x) noexcept{
    return _mm256_set1_epi32(static_cast<int>(x));
}

EEZ_NTT880_ALWAYS_INLINE vec add8(vec a,vec b) noexcept{
    const vec two_p=broadcast(mod2);
    vec x=_mm256_sub_epi32(_mm256_add_epi32(a,b),two_p);
    return _mm256_add_epi32(x,_mm256_and_si256(_mm256_srai_epi32(x,31),two_p));
}

EEZ_NTT880_ALWAYS_INLINE vec sub8(vec a,vec b) noexcept{
    const vec two_p=broadcast(mod2);
    vec x=_mm256_sub_epi32(a,b);
    return _mm256_add_epi32(x,_mm256_and_si256(_mm256_srai_epi32(x,31),two_p));
}

EEZ_NTT880_ALWAYS_INLINE vec mul8(vec a,vec b) noexcept{
    const vec ninv=broadcast(montgomery_ninv);
    const vec prime=broadcast(mod);
    const vec pe=_mm256_mul_epu32(a,b);
    const vec po=_mm256_mul_epu32(_mm256_bsrli_epi128(a,4),_mm256_bsrli_epi128(b,4));
    const vec qe=_mm256_mul_epu32(pe,ninv);
    const vec qo=_mm256_mul_epu32(po,ninv);
    const vec re=_mm256_add_epi64(pe,_mm256_mul_epu32(qe,prime));
    const vec ro=_mm256_add_epi64(po,_mm256_mul_epu32(qo,prime));
    return _mm256_or_si256(_mm256_bsrli_epi128(re,4),ro);
}

EEZ_NTT880_ALWAYS_INLINE vec mul8_fixed(vec a,vec b,vec bninv) noexcept{
    const vec prime=broadcast(mod);
    const vec oa=_mm256_bsrli_epi128(a,4);
    const vec pe=_mm256_mul_epu32(a,b);
    const vec po=_mm256_mul_epu32(oa,b);
    const vec qe=_mm256_mul_epu32(a,bninv);
    const vec qo=_mm256_mul_epu32(oa,bninv);
    const vec re=_mm256_add_epi64(pe,_mm256_mul_epu32(qe,prime));
    const vec ro=_mm256_add_epi64(po,_mm256_mul_epu32(qo,prime));
    return _mm256_or_si256(_mm256_bsrli_epi128(re,4),ro);
}

EEZ_NTT880_ALWAYS_INLINE vec canonicalize8(vec x) noexcept{const vec p=broadcast(mod);return _mm256_min_epu32(x,_mm256_sub_epi32(x,p));}

EEZ_NTT880_ALWAYS_INLINE vec pack_four(word x0,word x1) noexcept{
    return _mm256_setr_epi32(
        static_cast<int>(x0),static_cast<int>(x0),static_cast<int>(x0),static_cast<int>(x0),
        static_cast<int>(x1),static_cast<int>(x1),static_cast<int>(x1),static_cast<int>(x1)
    );
}

EEZ_NTT880_ALWAYS_INLINE vec load2x4(const mint* p0,const mint* p1) noexcept{
    const __m128i lo=_mm_loadu_si128(reinterpret_cast<const __m128i*>(static_cast<const void*>(p0)));
    const __m128i hi=_mm_loadu_si128(reinterpret_cast<const __m128i*>(static_cast<const void*>(p1)));
    return _mm256_set_m128i(hi,lo);
}

EEZ_NTT880_ALWAYS_INLINE void store2x4(mint* p0,mint* p1,vec x) noexcept{
    _mm_storeu_si128(reinterpret_cast<__m128i*>(static_cast<void*>(p0)),_mm256_castsi256_si128(x));
    _mm_storeu_si128(reinterpret_cast<__m128i*>(static_cast<void*>(p1)),_mm256_extracti128_si256(x,1));
}

EEZ_NTT880_ALWAYS_INLINE void transpose_8x4_to_4x8(vec v0,vec v1,vec v2,vec v3,vec& x0,vec& x1,vec& x2,vec& x3) noexcept{
    const vec t0=_mm256_unpacklo_epi32(v0,v1);
    const vec t1=_mm256_unpackhi_epi32(v0,v1);
    const vec t2=_mm256_unpacklo_epi32(v2,v3);
    const vec t3=_mm256_unpackhi_epi32(v2,v3);
    const vec perm=_mm256_setr_epi32(0,4,1,5,2,6,3,7);
    x0=_mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(t0,t2),perm);
    x1=_mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(t0,t2),perm);
    x2=_mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(t1,t3),perm);
    x3=_mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(t1,t3),perm);
}

EEZ_NTT880_ALWAYS_INLINE void transpose_4x8_to_8x4(vec x0,vec x1,vec x2,vec x3,vec& v0,vec& v1,vec& v2,vec& v3) noexcept{
    const vec perm=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    const vec q0=_mm256_permutevar8x32_epi32(x0,perm);
    const vec q1=_mm256_permutevar8x32_epi32(x1,perm);
    const vec q2=_mm256_permutevar8x32_epi32(x2,perm);
    const vec q3=_mm256_permutevar8x32_epi32(x3,perm);
    const vec t0=_mm256_unpacklo_epi64(q0,q1);
    const vec t2=_mm256_unpackhi_epi64(q0,q1);
    const vec t1=_mm256_unpacklo_epi64(q2,q3);
    const vec t3=_mm256_unpackhi_epi64(q2,q3);
    v0=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t0),_mm256_castsi256_ps(t1),_MM_SHUFFLE(2,0,2,0)));
    v1=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t0),_mm256_castsi256_ps(t1),_MM_SHUFFLE(3,1,3,1)));
    v2=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t2),_mm256_castsi256_ps(t3),_MM_SHUFFLE(2,0,2,0)));
    v3=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t2),_mm256_castsi256_ps(t3),_MM_SHUFFLE(3,1,3,1)));
}

#endif

EEZ_NTT880_ALWAYS_INLINE void forward_butterfly(mint* b,usize stride,usize i,word r1,word r2,word r3) noexcept{
    const word x0=raw(b[i]);
    const word x1=mul(raw(b[stride+i]),r1);
    const word x2=mul(raw(b[2*stride+i]),r2);
    const word x3=mul(raw(b[3*stride+i]),r3);
    const word s02=add(x0,x2);
    const word d02=sub(x0,x2);
    const word s13=add(x1,x3);
    const word t=mul(sub(x1,x3),twiddles.root[2]);
    b[i]=from_raw(add(s02,s13));
    b[stride+i]=from_raw(sub(s02,s13));
    b[2*stride+i]=from_raw(add(d02,t));
    b[3*stride+i]=from_raw(sub(d02,t));
}

EEZ_NTT880_ALWAYS_INLINE void inverse_butterfly(mint* b,usize stride,usize i,word r1,word r2,word r3) noexcept{
    const word x0=raw(b[i]);
    const word x1=raw(b[stride+i]);
    const word x2=raw(b[2*stride+i]);
    const word x3=raw(b[3*stride+i]);
    const word s01=add(x0,x1);
    const word d01=sub(x0,x1);
    const word s23=add(x2,x3);
    const word t=mul(sub(x2,x3),twiddles.iroot[2]);
    b[i]=from_raw(add(s01,s23));
    b[stride+i]=from_raw(mul(add(d01,t),r1));
    b[2*stride+i]=from_raw(mul(sub(s01,s23),r2));
    b[3*stride+i]=from_raw(mul(sub(d01,t),r3));
}

inline void forward_radix4_scalar(mint* EEZ_NTT880_RESTRICT a,usize blocks,usize stride) noexcept{
    {
        mint* const b=a;
        for(usize i=0;i<stride;++i){
            const word x0=raw(b[i]);
            const word x1=raw(b[stride+i]);
            const word x2=raw(b[2*stride+i]);
            const word x3=raw(b[3*stride+i]);
            const word s02=add(x0,x2);
            const word d02=sub(x0,x2);
            const word s13=add(x1,x3);
            const word t=mul(sub(x1,x3),twiddles.root[2]);
            b[i]=from_raw(add(s02,s13));
            b[stride+i]=from_raw(sub(s02,s13));
            b[2*stride+i]=from_raw(add(d02,t));
            b[3*stride+i]=from_raw(sub(d02,t));
        }
    }
    if(blocks==1)return;
    word rot=forward_rate3(0);
    for(usize s=1;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        mint* const b=a+s*4*stride;
        for(usize i=0;i<stride;++i)forward_butterfly(b,stride,i,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

inline void inverse_radix4_scalar(mint* EEZ_NTT880_RESTRICT a,usize blocks,usize stride) noexcept{
    {
        mint* const b=a;
        for(usize i=0;i<stride;++i){
            const word x0=raw(b[i]);
            const word x1=raw(b[stride+i]);
            const word x2=raw(b[2*stride+i]);
            const word x3=raw(b[3*stride+i]);
            const word s01=add(x0,x1);
            const word d01=sub(x0,x1);
            const word s23=add(x2,x3);
            const word t=mul(sub(x2,x3),twiddles.iroot[2]);
            b[i]=from_raw(add(s01,s23));
            b[stride+i]=from_raw(add(d01,t));
            b[2*stride+i]=from_raw(sub(s01,s23));
            b[3*stride+i]=from_raw(sub(d01,t));
        }
    }
    if(blocks==1)return;
    word rot=inverse_rate3(0);
    for(usize s=1;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        mint* const b=a+s*4*stride;
        for(usize i=0;i<stride;++i)inverse_butterfly(b,stride,i,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

#if defined(__AVX2__) || defined(_M_AVX2)

EEZ_NTT880_ALWAYS_INLINE void forward_radix4_large_block(mint* EEZ_NTT880_RESTRICT b,usize stride,vec imag,word r1,word r2,word r3) noexcept{
    const vec w1=broadcast(r1);
    const vec w2=broadcast(r2);
    const vec w3=broadcast(r3);
    for(usize i=0;i<stride;i+=8){
        const vec x0=load8(b+i);
        const vec x1=mul8(load8(b+stride+i),w1);
        const vec x2=mul8(load8(b+2*stride+i),w2);
        const vec x3=mul8(load8(b+3*stride+i),w3);
        const vec s02=add8(x0,x2);
        const vec d02=sub8(x0,x2);
        const vec s13=add8(x1,x3);
        const vec t=mul8(sub8(x1,x3),imag);
        store8(b+i,add8(s02,s13));
        store8(b+stride+i,sub8(s02,s13));
        store8(b+2*stride+i,add8(d02,t));
        store8(b+3*stride+i,sub8(d02,t));
    }
}

EEZ_NTT880_ALWAYS_INLINE void inverse_radix4_large_block(mint* EEZ_NTT880_RESTRICT b,usize stride,vec iimag,word r1,word r2,word r3) noexcept{
    const vec w1=broadcast(r1);
    const vec w2=broadcast(r2);
    const vec w3=broadcast(r3);
    for(usize i=0;i<stride;i+=8){
        const vec x0=load8(b+i);
        const vec x1=load8(b+stride+i);
        const vec x2=load8(b+2*stride+i);
        const vec x3=load8(b+3*stride+i);
        const vec s01=add8(x0,x1);
        const vec d01=sub8(x0,x1);
        const vec s23=add8(x2,x3);
        const vec t=mul8(sub8(x2,x3),iimag);
        store8(b+i,add8(s01,s23));
        store8(b+stride+i,mul8(add8(d01,t),w1));
        store8(b+2*stride+i,mul8(sub8(s01,s23),w2));
        store8(b+3*stride+i,mul8(sub8(d01,t),w3));
    }
}

inline void forward_radix4_large(mint* EEZ_NTT880_RESTRICT a,usize blocks,usize stride) noexcept{
    const vec imag=broadcast(twiddles.root[2]);
    {
        mint* const b=a;
        for(usize i=0;i<stride;i+=8){
            const vec x0=load8(b+i);
            const vec x1=load8(b+stride+i);
            const vec x2=load8(b+2*stride+i);
            const vec x3=load8(b+3*stride+i);
            const vec s02=add8(x0,x2);
            const vec d02=sub8(x0,x2);
            const vec s13=add8(x1,x3);
            const vec t=mul8(sub8(x1,x3),imag);
            store8(b+i,add8(s02,s13));
            store8(b+stride+i,sub8(s02,s13));
            store8(b+2*stride+i,add8(d02,t));
            store8(b+3*stride+i,sub8(d02,t));
        }
    }
    if(blocks==1)return;
    word rot=forward_rate3(0);
    usize s=1;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8],r2[8],r3[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        _mm256_store_si256(reinterpret_cast<vec*>(r2),w2);
        _mm256_store_si256(reinterpret_cast<vec*>(r3),w3);
        for(unsigned lane=0;lane<8;++lane)forward_radix4_large_block(a+(s+lane)*4*stride,stride,imag,r1[lane],r2[lane],r3[lane]);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        forward_radix4_large_block(a+s*4*stride,stride,imag,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

inline void inverse_radix4_large(mint* EEZ_NTT880_RESTRICT a,usize blocks,usize stride) noexcept{
    const vec iimag=broadcast(twiddles.iroot[2]);
    {
        mint* const b=a;
        for(usize i=0;i<stride;i+=8){
            const vec x0=load8(b+i);
            const vec x1=load8(b+stride+i);
            const vec x2=load8(b+2*stride+i);
            const vec x3=load8(b+3*stride+i);
            const vec s01=add8(x0,x1);
            const vec d01=sub8(x0,x1);
            const vec s23=add8(x2,x3);
            const vec t=mul8(sub8(x2,x3),iimag);
            store8(b+i,add8(s01,s23));
            store8(b+stride+i,add8(d01,t));
            store8(b+2*stride+i,sub8(s01,s23));
            store8(b+3*stride+i,sub8(d01,t));
        }
    }
    if(blocks==1)return;
    word rot=inverse_rate3(0);
    usize s=1;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8],r2[8],r3[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        _mm256_store_si256(reinterpret_cast<vec*>(r2),w2);
        _mm256_store_si256(reinterpret_cast<vec*>(r3),w3);
        for(unsigned lane=0;lane<8;++lane)inverse_radix4_large_block(a+(s+lane)*4*stride,stride,iimag,r1[lane],r2[lane],r3[lane]);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        inverse_radix4_large_block(a+s*4*stride,stride,iimag,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

inline void forward_radix4_p4(mint* EEZ_NTT880_RESTRICT a,usize blocks) noexcept{
    if(blocks<2){
        forward_radix4_scalar(a,blocks,4);
        return;
    }
    const vec imag=broadcast(twiddles.root[2]);
    word rot=montgomery_one;
    for(usize s=0;s<blocks;s+=2){
        const word r10=rot;
        rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
        const word r11=rot;
        const vec w1=pack_four(r10,r11);
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        mint* const b0=a+s*16;
        mint* const b1=b0+16;
        const vec x0=load2x4(b0,b1);
        const vec x1=mul8(load2x4(b0+4,b1+4),w1);
        const vec x2=mul8(load2x4(b0+8,b1+8),w2);
        const vec x3=mul8(load2x4(b0+12,b1+12),w3);
        const vec s02=add8(x0,x2);
        const vec d02=sub8(x0,x2);
        const vec s13=add8(x1,x3);
        const vec t=mul8(sub8(x1,x3),imag);
        store2x4(b0,b1,add8(s02,s13));
        store2x4(b0+4,b1+4,sub8(s02,s13));
        store2x4(b0+8,b1+8,add8(d02,t));
        store2x4(b0+12,b1+12,sub8(d02,t));
        if(s+2<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+1))));
    }
}

inline void inverse_radix4_p4(mint* EEZ_NTT880_RESTRICT a,usize blocks) noexcept{
    if(blocks<2){
        inverse_radix4_scalar(a,blocks,4);
        return;
    }
    const vec iimag=broadcast(twiddles.iroot[2]);
    word rot=montgomery_one;
    for(usize s=0;s<blocks;s+=2){
        const word r10=rot;
        rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
        const word r11=rot;
        const vec w1=pack_four(r10,r11);
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        mint* const b0=a+s*16;
        mint* const b1=b0+16;
        const vec x0=load2x4(b0,b1);
        const vec x1=load2x4(b0+4,b1+4);
        const vec x2=load2x4(b0+8,b1+8);
        const vec x3=load2x4(b0+12,b1+12);
        const vec s01=add8(x0,x1);
        const vec d01=sub8(x0,x1);
        const vec s23=add8(x2,x3);
        const vec t=mul8(sub8(x2,x3),iimag);
        store2x4(b0,b1,add8(s01,s23));
        store2x4(b0+4,b1+4,mul8(add8(d01,t),w1));
        store2x4(b0+8,b1+8,mul8(sub8(s01,s23),w2));
        store2x4(b0+12,b1+12,mul8(sub8(d01,t),w3));
        if(s+2<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+1))));
    }
}

inline void forward_radix4_p1(mint* EEZ_NTT880_RESTRICT a,usize blocks) noexcept{
    const vec imag=broadcast(twiddles.root[2]);
    word rot=montgomery_one;
    usize s=0;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        mint* const b=a+4*s;
        vec x0,x1,x2,x3;
        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);
        x1=mul8(x1,w1);
        x2=mul8(x2,w2);
        x3=mul8(x3,w3);
        const vec s02=add8(x0,x2);
        const vec d02=sub8(x0,x2);
        const vec s13=add8(x1,x3);
        const vec t=mul8(sub8(x1,x3),imag);
        vec v0,v1,v2,v3;
        transpose_4x8_to_8x4(add8(s02,s13),sub8(s02,s13),add8(d02,t),sub8(d02,t),v0,v1,v2,v3);
        store8(b,v0);
        store8(b+8,v1);
        store8(b+16,v2);
        store8(b+24,v3);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        forward_butterfly(a+4*s,1,0,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

inline void inverse_radix4_p1(mint* EEZ_NTT880_RESTRICT a,usize blocks) noexcept{
    const vec iimag=broadcast(twiddles.iroot[2]);
    word rot=montgomery_one;
    usize s=0;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        mint* const b=a+4*s;
        vec x0,x1,x2,x3;
        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);
        const vec s01=add8(x0,x1);
        const vec d01=sub8(x0,x1);
        const vec s23=add8(x2,x3);
        const vec t=mul8(sub8(x2,x3),iimag);
        vec v0,v1,v2,v3;
        transpose_4x8_to_8x4(add8(s01,s23),mul8(add8(d01,t),w1),mul8(sub8(s01,s23),w2),mul8(sub8(d01,t),w3),v0,v1,v2,v3);
        store8(b,v0);
        store8(b+8,v1);
        store8(b+16,v2);
        store8(b+24,v3);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        inverse_butterfly(a+4*s,1,0,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

#endif

inline void forward_radix2_first(mint* EEZ_NTT880_RESTRICT a,usize n) noexcept{
    const usize half=n>>1;
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    for(;i+8<=half;i+=8){
        const vec x=load8(a+i);
        const vec y=load8(a+half+i);
        store8(a+i,add8(x,y));
        store8(a+half+i,sub8(x,y));
    }
#endif
    for(;i<half;++i){
        const word x=raw(a[i]);
        const word y=raw(a[half+i]);
        a[i]=from_raw(add(x,y));
        a[half+i]=from_raw(sub(x,y));
    }
}

inline void forward_radix4_stage(mint* EEZ_NTT880_RESTRICT a,usize n,int stage) noexcept{
    const int h=static_cast<int>(std::countr_zero(n));
    assert(stage>=0&&stage+2<=h);
    const usize stride=usize(1)<<(h-stage-2);
    const usize blocks=usize(1)<<stage;
#if defined(__AVX2__) || defined(_M_AVX2)
    if(stride>=8)forward_radix4_large(a,blocks,stride);
    else if(stride==4)forward_radix4_p4(a,blocks);
    else if(stride==1)forward_radix4_p1(a,blocks);
    else forward_radix4_scalar(a,blocks,stride);
#else
    forward_radix4_scalar(a,blocks,stride);
#endif
}

inline void inverse_radix4_stage(mint* EEZ_NTT880_RESTRICT a,usize n,int stage) noexcept{
    const int h=static_cast<int>(std::countr_zero(n));
    assert(stage>=0&&stage+2<=h);
    const usize stride=usize(1)<<(h-stage-2);
    const usize blocks=usize(1)<<stage;
#if defined(__AVX2__) || defined(_M_AVX2)
    if(stride>=8)inverse_radix4_large(a,blocks,stride);
    else if(stride==4)inverse_radix4_p4(a,blocks);
    else if(stride==1)inverse_radix4_p1(a,blocks);
    else inverse_radix4_scalar(a,blocks,stride);
#else
    inverse_radix4_scalar(a,blocks,stride);
#endif
}

inline void final_radix2_scale(mint* EEZ_NTT880_RESTRICT a,usize n,word scale_mont) noexcept{
    const usize half=n>>1;
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    const vec scale=broadcast(scale_mont);
    for(;i+8<=half;i+=8){
        const vec x=load8(a+i);
        const vec y=load8(a+half+i);
        store8(a+i,mul8(add8(x,y),scale));
        store8(a+half+i,mul8(sub8(x,y),scale));
    }
#endif
    for(;i<half;++i){
        const word x=raw(a[i]);
        const word y=raw(a[half+i]);
        a[i]=from_raw(mul(add(x,y),scale_mont));
        a[half+i]=from_raw(mul(sub(x,y),scale_mont));
    }
}

inline void final_radix4_scale(mint* EEZ_NTT880_RESTRICT a,usize n,word scale_mont) noexcept{
    const usize stride=n>>2;
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    const vec iimag=broadcast(twiddles.iroot[2]);
    const vec scale=broadcast(scale_mont);
    for(;i+8<=stride;i+=8){
        const vec x0=load8(a+i);
        const vec x1=load8(a+stride+i);
        const vec x2=load8(a+2*stride+i);
        const vec x3=load8(a+3*stride+i);
        const vec s01=add8(x0,x1);
        const vec d01=sub8(x0,x1);
        const vec s23=add8(x2,x3);
        const vec t=mul8(sub8(x2,x3),iimag);
        store8(a+i,mul8(add8(s01,s23),scale));
        store8(a+stride+i,mul8(add8(d01,t),scale));
        store8(a+2*stride+i,mul8(sub8(s01,s23),scale));
        store8(a+3*stride+i,mul8(sub8(d01,t),scale));
    }
#endif
    for(;i<stride;++i){
        const word x0=raw(a[i]);
        const word x1=raw(a[stride+i]);
        const word x2=raw(a[2*stride+i]);
        const word x3=raw(a[3*stride+i]);
        const word s01=add(x0,x1);
        const word d01=sub(x0,x1);
        const word s23=add(x2,x3);
        const word t=mul(sub(x2,x3),twiddles.iroot[2]);
        a[i]=from_raw(mul(add(s01,s23),scale_mont));
        a[stride+i]=from_raw(mul(add(d01,t),scale_mont));
        a[2*stride+i]=from_raw(mul(sub(s01,s23),scale_mont));
        a[3*stride+i]=from_raw(mul(sub(d01,t),scale_mont));
    }
}

inline void forward_dif(mint* EEZ_NTT880_RESTRICT a,usize n) noexcept{
    if(n<=1)return;
    const int h=static_cast<int>(std::countr_zero(n));
    int stage=0;
    if(h&1){
        forward_radix2_first(a,n);
        stage=1;
    }
    for(;stage<h;stage+=2)forward_radix4_stage(a,n,stage);
}

inline void inverse_dit(mint* EEZ_NTT880_RESTRICT a,usize n) noexcept{
    if(n<=1)return;
    const int h=static_cast<int>(std::countr_zero(n));
    const word scale=mint::raw(static_cast<u32>(n)).inv().a;
    if(h&1){
        for(int stage=h-2;stage>=1;stage-=2)inverse_radix4_stage(a,n,stage);
        final_radix2_scale(a,n,scale);
    }else{
        for(int stage=h-2;stage>=2;stage-=2)inverse_radix4_stage(a,n,stage);
        final_radix4_scale(a,n,scale);
    }
}

#if defined(__AVX2__) || defined(_M_AVX2)

class aligned_uninitialized_buffer{
    static_assert(std::is_trivially_destructible_v<mint>);
    mint* data_=nullptr;
public:
    explicit aligned_uninitialized_buffer(usize n)
        :data_(static_cast<mint*>(::operator new[](n*sizeof(mint),std::align_val_t{64}))){}
    aligned_uninitialized_buffer(const aligned_uninitialized_buffer&)=delete;
    aligned_uninitialized_buffer& operator=(const aligned_uninitialized_buffer&)=delete;
    ~aligned_uninitialized_buffer(){
        ::operator delete[](data_,std::align_val_t{64});
    }
    mint* data() noexcept{return data_;}
    const mint* data()const noexcept{return data_;}
};

EEZ_NTT880_ALWAYS_INLINE vec load8_aligned(const mint* p) noexcept{
    return _mm256_load_si256(reinterpret_cast<const __m256i*>(static_cast<const void*>(p)));
}

EEZ_NTT880_ALWAYS_INLINE void store8_aligned(mint* p,vec x) noexcept{
    _mm256_store_si256(reinterpret_cast<__m256i*>(static_cast<void*>(p)),x);
}

EEZ_NTT880_ALWAYS_INLINE vec shrink4_to_2(vec x) noexcept{
    return _mm256_min_epu32(x,_mm256_sub_epi32(x,broadcast(mod2)));
}

EEZ_NTT880_ALWAYS_INLINE vec lazy_add8(vec a,vec b) noexcept{return _mm256_add_epi32(a,b);}

EEZ_NTT880_ALWAYS_INLINE vec lazy_sub8(vec a,vec b) noexcept{
    return _mm256_add_epi32(a,_mm256_sub_epi32(broadcast(mod2),b));
}

template<bool trivial_twiddle,bool convert_input=false>
inline void forward_radix4_block_lazy(mint* b,usize stride,word r1) noexcept{
    const word imag=canonicalize(twiddles.root[2]);
    const vec vimag=broadcast(imag);
    const vec vimag_ninv=broadcast(imag*montgomery_ninv);
    const vec vr1=broadcast(r1);
    const vec vr1_ninv=broadcast(r1*montgomery_ninv);
    const word r2=canonicalize(mul(r1,r1));
    const vec vr2=broadcast(r2);
    const vec vr2_ninv=broadcast(r2*montgomery_ninv);
    const word r3=canonicalize(mul(r2,r1));
    const vec vr3=broadcast(r3);
    const vec vr3_ninv=broadcast(r3*montgomery_ninv);

    for(usize i=0;i<stride;i+=8){
        vec x0=shrink4_to_2(load8_aligned(b+i));
        vec x1=load8_aligned(b+stride+i);
        vec x2=load8_aligned(b+2*stride+i);
        vec x3=load8_aligned(b+3*stride+i);
        if constexpr(!trivial_twiddle){
            x1=mul8_fixed(x1,vr1,vr1_ninv);
            x2=mul8_fixed(x2,vr2,vr2_ninv);
            x3=mul8_fixed(x3,vr3,vr3_ninv);
        }else{
            x1=shrink4_to_2(x1);
            x2=shrink4_to_2(x2);
            x3=shrink4_to_2(x3);
        }
        vec s02=lazy_add8(x0,x2);
        vec d02=lazy_sub8(x0,x2);
        vec s13=lazy_add8(x1,x3);
        const vec t=mul8_fixed(lazy_sub8(x1,x3),vimag,vimag_ninv);
        s02=shrink4_to_2(s02);
        d02=shrink4_to_2(d02);
        s13=shrink4_to_2(s13);
        vec y0=lazy_add8(s02,s13);
        vec y1=lazy_sub8(s02,s13);
        vec y2=lazy_add8(d02,t);
        vec y3=lazy_sub8(d02,t);
        if constexpr(convert_input){
            constexpr word r2c=464649016u;
            const vec vr=broadcast(r2c);
            const vec vn=broadcast(r2c*montgomery_ninv);
            y0=mul8_fixed(y0,vr,vn);
            y1=mul8_fixed(y1,vr,vn);
            y2=mul8_fixed(y2,vr,vn);
            y3=mul8_fixed(y3,vr,vn);
        }
        store8_aligned(b+i,y0);
        store8_aligned(b+stride+i,y1);
        store8_aligned(b+2*stride+i,y2);
        store8_aligned(b+3*stride+i,y3);
    }
}

template<bool trivial_twiddle,bool convert_input=false>
EEZ_NTT880_ALWAYS_INLINE void forward_radix4_block_pair_lazy(mint* EEZ_NTT880_RESTRICT a,mint* EEZ_NTT880_RESTRICT b,usize stride,word r1) noexcept{
    forward_radix4_block_lazy<trivial_twiddle,convert_input>(a,stride,r1);
    forward_radix4_block_lazy<trivial_twiddle,convert_input>(b,stride,r1);
}

template<bool trivial_twiddle,bool apply_scale,bool convert_output=false,bool direct_output=false,bool shrink_output=false>
inline void inverse_radix4_block_lazy(mint* b,usize stride,word r1,word scale) noexcept{
    const word iimag=canonicalize(twiddles.iroot[2]);
    const vec viimag=broadcast(iimag);
    const vec viimag_ninv=broadcast(iimag*montgomery_ninv);
    const vec vr1=broadcast(r1);
    const vec vr1_ninv=broadcast(r1*montgomery_ninv);
    const word r2=canonicalize(mul(r1,r1));
    const vec vr2=broadcast(r2);
    const vec vr2_ninv=broadcast(r2*montgomery_ninv);
    const word r3=canonicalize(mul(r2,r1));
    const vec vr3=broadcast(r3);
    const vec vr3_ninv=broadcast(r3*montgomery_ninv);
    const word sc=canonicalize(scale);

    for(usize i=0;i<stride;i+=8){
        const vec x0=shrink4_to_2(load8_aligned(b+i));
        const vec x1=shrink4_to_2(load8_aligned(b+stride+i));
        const vec x2=shrink4_to_2(load8_aligned(b+2*stride+i));
        const vec x3=shrink4_to_2(load8_aligned(b+3*stride+i));
        vec s01=lazy_add8(x0,x1);
        vec d01=lazy_sub8(x0,x1);
        vec s23=lazy_add8(x2,x3);
        const vec t=mul8_fixed(lazy_sub8(x2,x3),viimag,viimag_ninv);
        s01=shrink4_to_2(s01);
        d01=shrink4_to_2(d01);
        s23=shrink4_to_2(s23);
        vec y0=lazy_add8(s01,s23);
        vec y1=lazy_add8(d01,t);
        vec y2=lazy_sub8(s01,s23);
        vec y3=lazy_sub8(d01,t);
        if constexpr(apply_scale){
            const word s0=sc;
            const word s1=trivial_twiddle?s0:canonicalize(mul(s0,r1));
            const word s2=trivial_twiddle?s0:canonicalize(mul(s0,r2));
            const word s3=trivial_twiddle?s0:canonicalize(mul(s0,r3));
            y0=mul8_fixed(y0,broadcast(s0),broadcast(s0*montgomery_ninv));
            y1=mul8_fixed(y1,broadcast(s1),broadcast(s1*montgomery_ninv));
            y2=mul8_fixed(y2,broadcast(s2),broadcast(s2*montgomery_ninv));
            y3=mul8_fixed(y3,broadcast(s3),broadcast(s3*montgomery_ninv));
        }else if constexpr(!trivial_twiddle){
            y1=mul8_fixed(y1,vr1,vr1_ninv);
            y2=mul8_fixed(y2,vr2,vr2_ninv);
            y3=mul8_fixed(y3,vr3,vr3_ninv);
        }
        if constexpr(convert_output){
            const vec one=broadcast(1);
            const vec ninv=broadcast(montgomery_ninv);
            y0=canonicalize8(mul8_fixed(y0,one,ninv));
            y1=canonicalize8(mul8_fixed(y1,one,ninv));
            y2=canonicalize8(mul8_fixed(y2,one,ninv));
            y3=canonicalize8(mul8_fixed(y3,one,ninv));
        }else if constexpr(direct_output){
            y0=canonicalize8(shrink4_to_2(y0));
            y1=canonicalize8(shrink4_to_2(y1));
            y2=canonicalize8(shrink4_to_2(y2));
            y3=canonicalize8(shrink4_to_2(y3));
        }else if constexpr(shrink_output && !apply_scale){
            // Only the final stage must expose Montgomery values in [0, 2p).

            // apply_scale already reduces each product to this range.

            y0=shrink4_to_2(y0);
            y1=shrink4_to_2(y1);
            y2=shrink4_to_2(y2);
            y3=shrink4_to_2(y3);
        }
        store8_aligned(b+i,y0);
        store8_aligned(b+stride+i,y1);
        store8_aligned(b+2*stride+i,y2);
        store8_aligned(b+3*stride+i,y3);
    }
}

inline unsigned adaptive_leaf_log(usize n) noexcept{
    const unsigned h=static_cast<unsigned>(std::countr_zero(n));
    return(h&1u)?3u:4u;
}

template<bool convert_input=false>
EEZ_NTT880_ALWAYS_INLINE void forward_cache_node(mint* EEZ_NTT880_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    const usize stride=block_size>>2;
    if constexpr(convert_input)forward_radix4_block_lazy<true,true>(base,stride,montgomery_one);
    else if(block==0)forward_radix4_block_lazy<true>(base,stride,montgomery_one);
    else forward_radix4_block_lazy<false>(base,stride,rotation[layer]);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));
}

template<bool convert_input=false>
inline void forward_cache_block(mint* EEZ_NTT880_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    forward_cache_node<convert_input>(base,block_size,layer,block,blocks_at_layer,rotation);
    const usize child_size=block_size>>2;
    for(usize child=0;child<4;++child){
        mint* const p=base+child*child_size;
        const usize cb=block*4+child;
        forward_cache_node(p,child_size,layer+1,cb,blocks_at_layer*4,rotation);
        const usize gsize=child_size>>2;
        for(usize g=0;g<4;++g)forward_cache_node(p+g*gsize,gsize,layer+2,cb*4+g,blocks_at_layer*16,rotation);
    }
}

template<bool convert_input=false>
inline void forward_cache_dfs(mint* EEZ_NTT880_RESTRICT base,usize block_size,usize leaf_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    if(block_size==leaf_size*64){
        forward_cache_block<convert_input>(base,block_size,layer,block,blocks_at_layer,rotation);
        return;
    }
    const usize stride=block_size>>2;
    if constexpr(convert_input)forward_radix4_block_lazy<true,true>(base,stride,montgomery_one);
    else if(block==0)forward_radix4_block_lazy<true>(base,stride,montgomery_one);
    else forward_radix4_block_lazy<false>(base,stride,rotation[layer]);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));
    const usize child_size=block_size>>2;
    if(child_size==leaf_size)return;
    for(usize child=0;child<4;++child)forward_cache_dfs<false>(base+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,rotation);
}

template<bool convert_input=false>
EEZ_NTT880_ALWAYS_INLINE void forward_cache_pair_node(mint* EEZ_NTT880_RESTRICT a,mint* EEZ_NTT880_RESTRICT b,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    const usize stride=block_size>>2;
    if constexpr(convert_input)forward_radix4_block_pair_lazy<true,true>(a,b,stride,montgomery_one);
    else if(block==0)forward_radix4_block_pair_lazy<true>(a,b,stride,montgomery_one);
    else forward_radix4_block_pair_lazy<false>(a,b,stride,rotation[layer]);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));
}

template<bool convert_input=false>
inline void forward_cache_pair_block(mint* EEZ_NTT880_RESTRICT a,mint* EEZ_NTT880_RESTRICT b,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    forward_cache_pair_node<convert_input>(a,b,block_size,layer,block,blocks_at_layer,rotation);
    const usize child_size=block_size>>2;
    for(usize child=0;child<4;++child){
        mint* const pa=a+child*child_size;
        mint* const pb=b+child*child_size;
        const usize cb=block*4+child;
        forward_cache_pair_node(pa,pb,child_size,layer+1,cb,blocks_at_layer*4,rotation);
        const usize gsize=child_size>>2;
        for(usize g=0;g<4;++g)forward_cache_pair_node(pa+g*gsize,pb+g*gsize,gsize,layer+2,cb*4+g,blocks_at_layer*16,rotation);
    }
}

template<bool convert_input=false>
inline void forward_cache_pair_dfs(mint* EEZ_NTT880_RESTRICT a,mint* EEZ_NTT880_RESTRICT b,usize block_size,usize leaf_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    if(block_size==leaf_size*64){
        forward_cache_pair_block<convert_input>(a,b,block_size,layer,block,blocks_at_layer,rotation);
        return;
    }
    forward_cache_pair_node<convert_input>(a,b,block_size,layer,block,blocks_at_layer,rotation);
    const usize child_size=block_size>>2;
    if(child_size==leaf_size)return;
    for(usize child=0;child<4;++child)forward_cache_pair_dfs<false>(a+child*child_size,b+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,rotation);
}

template<bool apply_scale,bool convert_output=false,bool direct_output=false,bool shrink_output=false>
EEZ_NTT880_ALWAYS_INLINE void inverse_cache_node(mint* EEZ_NTT880_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>& rotation) noexcept{
    const usize stride=block_size>>2;
    if(block==0)inverse_radix4_block_lazy<true,apply_scale,convert_output,direct_output,shrink_output>(base,stride,montgomery_one,scale);
    else inverse_radix4_block_lazy<false,apply_scale,convert_output,direct_output,shrink_output>(base,stride,rotation[layer],scale);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],inverse_rate3(twiddle_index(static_cast<u32>(block)))));
}

template<bool scale_leaf,bool convert_output=false,bool direct_output=false,bool shrink_output=false>
inline void inverse_cache_block(mint* EEZ_NTT880_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>& rotation) noexcept{
    const usize child_size=block_size>>2;
    const usize gsize=child_size>>2;
    for(usize child=0;child<4;++child){
        mint* const p=base+child*child_size;
        const usize cb=block*4+child;
        for(usize g=0;g<4;++g)inverse_cache_node<scale_leaf>(p+g*gsize,gsize,layer+2,cb*4+g,blocks_at_layer*16,scale,rotation);
        inverse_cache_node<false>(p,child_size,layer+1,cb,blocks_at_layer*4,scale,rotation);
    }
    inverse_cache_node<false,convert_output,direct_output,shrink_output>(base,block_size,layer,block,blocks_at_layer,scale,rotation);
}

template<bool scale_leaf,bool convert_output=false,bool direct_output=false,bool shrink_output=false>
inline void inverse_cache_dfs(mint* EEZ_NTT880_RESTRICT base,usize block_size,usize leaf_size,unsigned layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>& rotation) noexcept{
    if(block_size==leaf_size*64){
        inverse_cache_block<scale_leaf,convert_output,direct_output,shrink_output>(base,block_size,layer,block,blocks_at_layer,scale,rotation);
        return;
    }
    const usize child_size=block_size>>2;
    if(child_size!=leaf_size){
        for(usize child=0;child<4;++child)inverse_cache_dfs<scale_leaf,false>(base+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,scale,rotation);
    }
    const usize stride=block_size>>2;
    if constexpr(scale_leaf){
        if(child_size==leaf_size){
            if(block==0)inverse_radix4_block_lazy<true,true,convert_output,direct_output,shrink_output>(base,stride,montgomery_one,scale);
            else inverse_radix4_block_lazy<false,true,convert_output,direct_output,shrink_output>(base,stride,rotation[layer],scale);
        }else if(block==0)inverse_radix4_block_lazy<true,false,convert_output,direct_output,shrink_output>(base,stride,montgomery_one,scale);
        else inverse_radix4_block_lazy<false,false,convert_output,direct_output,shrink_output>(base,stride,rotation[layer],scale);
    }else if(block==0)inverse_radix4_block_lazy<true,false,convert_output,direct_output,shrink_output>(base,stride,montgomery_one,scale);
    else inverse_radix4_block_lazy<false,false,convert_output,direct_output,shrink_output>(base,stride,rotation[layer],scale);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],inverse_rate3(twiddle_index(static_cast<u32>(block)))));
}

EEZ_NTT880_ALWAYS_INLINE void convert_to_montgomery(mint* a,usize n) noexcept{
    constexpr word r2=464649016u;
    const vec vr2=broadcast(r2);
    const vec vn=broadcast(r2*montgomery_ninv);
    for(usize i=0;i<n;i+=8)store8_aligned(a+i,mul8_fixed(load8_aligned(a+i),vr2,vn));
}

EEZ_NTT880_ALWAYS_INLINE void convert_from_montgomery(mint* a,usize n) noexcept{
    const vec one=broadcast(1);
    const vec ninv=broadcast(montgomery_ninv);
    for(usize i=0;i<n;i+=8)store8_aligned(a+i,canonicalize8(mul8_fixed(load8_aligned(a+i),one,ninv)));
}

template<bool convert_input=false>
inline void forward_adaptive(mint* a,usize n,unsigned leaf_log) noexcept{
    const usize leaf_size=usize(1)<<leaf_log;
    if(n==leaf_size){
        if constexpr(convert_input)convert_to_montgomery(a,n);
        return;
    }
    std::array<word,max_log/2+1> rotation{};
    rotation.fill(canonicalize(montgomery_one));
    if((std::countr_zero(n)-leaf_log)&1u){
        forward_radix2_first(a,n);
        if constexpr(convert_input)convert_to_montgomery(a,n);
        forward_cache_dfs(a,n>>1,leaf_size,0,0,2,rotation);
        forward_cache_dfs(a+(n>>1),n>>1,leaf_size,0,1,2,rotation);
        return;
    }
    forward_cache_dfs<convert_input>(a,n,leaf_size,0,0,1,rotation);
}

template<bool convert_input=false>
inline void forward_adaptive_pair(mint* EEZ_NTT880_RESTRICT a,mint* EEZ_NTT880_RESTRICT b,usize n,unsigned leaf_log) noexcept{
    const usize leaf_size=usize(1)<<leaf_log;
    if(n==leaf_size){
        if constexpr(convert_input){
            convert_to_montgomery(a,n);
            convert_to_montgomery(b,n);
        }
        return;
    }
    std::array<word,max_log/2+1> rotation{};
    rotation.fill(canonicalize(montgomery_one));
    if((std::countr_zero(n)-leaf_log)&1u){
        forward_radix2_first(a,n);
        forward_radix2_first(b,n);
        if constexpr(convert_input){
            convert_to_montgomery(a,n);
            convert_to_montgomery(b,n);
        }
        forward_cache_pair_dfs(a,b,n>>1,leaf_size,0,0,2,rotation);
        forward_cache_pair_dfs(a+(n>>1),b+(n>>1),n>>1,leaf_size,0,1,2,rotation);
        return;
    }
    forward_cache_pair_dfs<convert_input>(a,b,n,leaf_size,0,0,1,rotation);
}

template<bool convert_output=false,bool direct_output=false>
inline void inverse_adaptive(mint* a,usize n,unsigned leaf_log) noexcept{
    const usize leaf_size=usize(1)<<leaf_log;
    if(n==leaf_size){
        if constexpr(convert_output)convert_from_montgomery(a,n);
        return;
    }
    const word scale=mint::raw(static_cast<word>(n>>leaf_log)).inv().a;
    std::array<word,max_log/2+1> rotation{};
    rotation.fill(canonicalize(montgomery_one));
    if((std::countr_zero(n)-leaf_log)&1u){
        inverse_cache_dfs<true>(a,n>>1,leaf_size,0,0,2,scale,rotation);
        inverse_cache_dfs<true>(a+(n>>1),n>>1,leaf_size,0,1,2,scale,rotation);
        const usize half=n>>1;
        for(usize i=0;i<half;i+=8){
            const vec x=shrink4_to_2(load8_aligned(a+i));
            const vec y=shrink4_to_2(load8_aligned(a+half+i));
            vec z0=lazy_add8(x,y);
            vec z1=lazy_sub8(x,y);
            if constexpr(convert_output){
                const vec one=broadcast(1);
                const vec ninv=broadcast(montgomery_ninv);
                z0=canonicalize8(mul8_fixed(z0,one,ninv));
                z1=canonicalize8(mul8_fixed(z1,one,ninv));
            }else if constexpr(direct_output){
                z0=canonicalize8(shrink4_to_2(z0));
                z1=canonicalize8(shrink4_to_2(z1));
            }else{
                z0=shrink4_to_2(z0);
                z1=shrink4_to_2(z1);
            }
            store8_aligned(a+i,z0);
            store8_aligned(a+half+i,z1);
        }
        return;
    }
    inverse_cache_dfs<true,convert_output,direct_output,true>(a,n,leaf_size,0,0,1,scale,rotation);
}

EEZ_NTT880_ALWAYS_INLINE __m128i reduce_four_accumulators(vec x) noexcept{
    const vec ninv=broadcast(montgomery_ninv);
    const vec prime=broadcast(mod);
    const vec q=_mm256_mul_epu32(x,ninv);
    const vec sum=_mm256_add_epi64(x,_mm256_mul_epu32(q,prime));
    const vec high=_mm256_bsrli_epi128(sum,4);
    const vec packed=_mm256_permutevar8x32_epi32(high,_mm256_setr_epi32(0,2,4,6,0,0,0,0));
    return _mm256_castsi256_si128(packed);
}

EEZ_NTT880_ALWAYS_INLINE vec reduce_eight_accumulators(vec even,vec odd) noexcept{
    const vec ninv=broadcast(montgomery_ninv);
    const vec prime=broadcast(mod);
    const vec qe=_mm256_mul_epu32(even,ninv);
    const vec qo=_mm256_mul_epu32(odd,ninv);
    const vec re=_mm256_add_epi64(even,_mm256_mul_epu32(qe,prime));
    const vec ro=_mm256_add_epi64(odd,_mm256_mul_epu32(qo,prime));
    return shrink4_to_2(_mm256_or_si256(_mm256_bsrli_epi128(re,4),ro));
}

EEZ_NTT880_ALWAYS_INLINE void leaf_copyfree8x4(mint* EEZ_NTT880_RESTRICT a,usize first_block,const std::array<word,4>& modulus) noexcept{
    alignas(64) word lhs[4][16];
    alignas(64) word rhs[4][8];
    alignas(64) vec even[4]{};
    alignas(64) vec odd[4]{};
    for(unsigned k=0;k<4;++k){
        const usize off=(first_block+k)*8;
        const vec x=canonicalize8(shrink4_to_2(load8_aligned(a+off)));
        const vec y=canonicalize8(mul8_fixed(x,broadcast(464649016u),broadcast(464649016u*montgomery_ninv)));
        const word w=canonicalize(modulus[k]);
        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]),mul8_fixed(x,broadcast(w),broadcast(w*montgomery_ninv)));
        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]+8),x);
        _mm256_store_si256(reinterpret_cast<vec*>(rhs[k]),y);
    }
    for(unsigned i=0;i<8;++i){
        for(unsigned k=0;k<4;++k){
            const usize off=(first_block+k)*8;
            const vec y=broadcast(rhs[k][i]);
            const vec x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[k]+8-i));
            even[k]=_mm256_add_epi64(even[k],_mm256_mul_epu32(y,x));
            odd[k]=_mm256_add_epi64(odd[k],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));
        }
    }
    for(unsigned k=0;k<4;++k)store8_aligned(a+(first_block+k)*8,reduce_eight_accumulators(even[k],odd[k]));
}
EEZ_NTT880_ALWAYS_INLINE void leaf_copyfree16x2(mint* EEZ_NTT880_RESTRICT a,usize first_block,const std::array<word,2>& modulus) noexcept{
    const vec split=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    alignas(64) word lhs[6][16];
    alignas(64) word rhs[6][8];
    alignas(64) vec even[6]{};
    alignas(64) vec odd[6]{};
    word w[2];
    for(unsigned k=0;k<2;++k){
        const usize off=(first_block+k)*16;
        const vec ax0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off))),split);
        const vec ax1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off+8))),split);
        const vec bx0=canonicalize8(mul8_fixed(ax0,broadcast(464649016u),broadcast(464649016u*montgomery_ninv)));
        const vec bx1=canonicalize8(mul8_fixed(ax1,broadcast(464649016u),broadcast(464649016u*montgomery_ninv)));
        const vec ae=_mm256_permute2x128_si256(ax0,ax1,0x20);
        const vec ao=_mm256_permute2x128_si256(ax0,ax1,0x31);
        const vec be=_mm256_permute2x128_si256(bx0,bx1,0x20);
        const vec bo=_mm256_permute2x128_si256(bx0,bx1,0x31);
        const vec lx[3]{ae,ao,canonicalize8(add8(ae,ao))};
        const vec ry[3]{be,bo,canonicalize8(add8(be,bo))};
        w[k]=canonicalize(modulus[k]);
        const vec vw=broadcast(w[k]);
        const vec vn=broadcast(w[k]*montgomery_ninv);
        for(unsigned j=0;j<3;++j){
            const unsigned p=k*3+j;
            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]),mul8_fixed(lx[j],vw,vn));
            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]+8),lx[j]);
            _mm256_store_si256(reinterpret_cast<vec*>(rhs[p]),ry[j]);
        }
    }
    for(unsigned i=0;i<8;++i){
        for(unsigned p=0;p<6;++p){
            const vec y=broadcast(rhs[p][i]);
            const vec x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[p]+8-i));
            even[p]=_mm256_add_epi64(even[p],_mm256_mul_epu32(y,x));
            odd[p]=_mm256_add_epi64(odd[p],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));
        }
    }
    for(unsigned k=0;k<2;++k){
        const vec p0=reduce_eight_accumulators(even[k*3],odd[k*3]);
        const vec p1=reduce_eight_accumulators(even[k*3+1],odd[k*3+1]);
        const vec p2=reduce_eight_accumulators(even[k*3+2],odd[k*3+2]);
        vec yp1=_mm256_permutevar8x32_epi32(p1,_mm256_setr_epi32(7,0,1,2,3,4,5,6));
        yp1=_mm256_insert_epi32(yp1,canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1,7)),w[k])),0);
        const vec ce=add8(p0,yp1);
        const vec co=sub8(sub8(p2,p0),p1);
        const vec lo=_mm256_unpacklo_epi32(ce,co);
        const vec hi=_mm256_unpackhi_epi32(ce,co);
        const usize off=(first_block+k)*16;
        store8_aligned(a+off,_mm256_permute2x128_si256(lo,hi,0x20));
        store8_aligned(a+off+8,_mm256_permute2x128_si256(lo,hi,0x31));
    }
}
template<unsigned leaf_size,unsigned parallel_blocks>
inline void leaf_copyfree(mint* a,usize n) noexcept{
    const usize blocks=n/leaf_size;
    word w=montgomery_one;
    for(usize s=0;s<blocks;s+=parallel_blocks){
        std::array<word,parallel_blocks> modulus{};
        for(unsigned k=0;k<parallel_blocks;++k){
            modulus[k]=w;
            const usize block=s+k;
            if(block+1<blocks)w=mul(w,forward_rate1(twiddle_index(static_cast<u32>(block))));
        }
        if constexpr(leaf_size==8)leaf_copyfree8x4(a,s,modulus);
        else leaf_copyfree16x2(a,s,modulus);
    }
}
EEZ_NTT880_ALWAYS_INLINE void leaf_product8x4(mint* EEZ_NTT880_RESTRICT a,mint* EEZ_NTT880_RESTRICT b,usize first_block,const std::array<word,4>& modulus) noexcept{
    alignas(64) word lhs[4][16];
    alignas(64) vec even[4]{};
    alignas(64) vec odd[4]{};
    for(unsigned k=0;k<4;++k){
        const usize off=(first_block+k)*8;
        const vec x=canonicalize8(shrink4_to_2(load8_aligned(a+off)));
        const vec y=canonicalize8(shrink4_to_2(load8_aligned(b+off)));
        const word w=canonicalize(modulus[k]);
        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]),mul8_fixed(x,broadcast(w),broadcast(w*montgomery_ninv)));
        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]+8),x);
        store8_aligned(b+off,y);
    }
    for(unsigned i=0;i<8;++i){
        for(unsigned k=0;k<4;++k){
            const usize off=(first_block+k)*8;
            const vec y=broadcast(raw(b[off+i]));
            const vec x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[k]+8-i));
            even[k]=_mm256_add_epi64(even[k],_mm256_mul_epu32(y,x));
            odd[k]=_mm256_add_epi64(odd[k],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));
        }
    }
    for(unsigned k=0;k<4;++k)store8_aligned(a+(first_block+k)*8,reduce_eight_accumulators(even[k],odd[k]));
}

EEZ_NTT880_ALWAYS_INLINE void leaf_product16x2_karatsuba(mint* EEZ_NTT880_RESTRICT a,const mint* EEZ_NTT880_RESTRICT b,usize first_block,const std::array<word,2>& modulus) noexcept{
    const vec split=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    alignas(64) word lhs[6][16];
    alignas(64) word rhs[6][8];
    alignas(64) vec even[6]{};
    alignas(64) vec odd[6]{};
    word w[2];
    for(unsigned k=0;k<2;++k){
        const usize off=(first_block+k)*16;
        const vec ax0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off))),split);
        const vec ax1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off+8))),split);
        const vec bx0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(b+off))),split);
        const vec bx1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(b+off+8))),split);
        const vec ae=_mm256_permute2x128_si256(ax0,ax1,0x20);
        const vec ao=_mm256_permute2x128_si256(ax0,ax1,0x31);
        const vec be=_mm256_permute2x128_si256(bx0,bx1,0x20);
        const vec bo=_mm256_permute2x128_si256(bx0,bx1,0x31);
        const vec lx[3]{ae,ao,canonicalize8(add8(ae,ao))};
        const vec ry[3]{be,bo,canonicalize8(add8(be,bo))};
        w[k]=canonicalize(modulus[k]);
        const vec vw=broadcast(w[k]);
        const vec vn=broadcast(w[k]*montgomery_ninv);
        for(unsigned j=0;j<3;++j){
            const unsigned p=k*3+j;
            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]),mul8_fixed(lx[j],vw,vn));
            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]+8),lx[j]);
            _mm256_store_si256(reinterpret_cast<vec*>(rhs[p]),ry[j]);
        }
    }
    for(unsigned i=0;i<8;++i){
        for(unsigned p=0;p<6;++p){
            const vec y=broadcast(rhs[p][i]);
            const vec x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[p]+8-i));
            even[p]=_mm256_add_epi64(even[p],_mm256_mul_epu32(y,x));
            odd[p]=_mm256_add_epi64(odd[p],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));
        }
    }
    for(unsigned k=0;k<2;++k){
        const vec p0=reduce_eight_accumulators(even[k*3],odd[k*3]);
        const vec p1=reduce_eight_accumulators(even[k*3+1],odd[k*3+1]);
        const vec p2=reduce_eight_accumulators(even[k*3+2],odd[k*3+2]);
        vec yp1=_mm256_permutevar8x32_epi32(p1,_mm256_setr_epi32(7,0,1,2,3,4,5,6));
        yp1=_mm256_insert_epi32(yp1,canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1,7)),w[k])),0);
        const vec ce=add8(p0,yp1);
        const vec co=sub8(sub8(p2,p0),p1);
        const vec lo=_mm256_unpacklo_epi32(ce,co);
        const vec hi=_mm256_unpackhi_epi32(ce,co);
        const usize off=(first_block+k)*16;
        store8_aligned(a+off,_mm256_permute2x128_si256(lo,hi,0x20));
        store8_aligned(a+off+8,_mm256_permute2x128_si256(lo,hi,0x31));
    }
}

EEZ_NTT880_ALWAYS_INLINE word twice(word x) noexcept{return x+x;}

EEZ_NTT880_ALWAYS_INLINE vec pack4_u32(word x0,word x1,word x2,word x3) noexcept{
    return _mm256_cvtepu32_epi64(_mm_setr_epi32(static_cast<int>(x0),static_cast<int>(x1),static_cast<int>(x2),static_cast<int>(x3)));
}

EEZ_NTT880_ALWAYS_INLINE vec mul4_u32(word a0,word b0,word a1,word b1,word a2,word b2,word a3,word b3) noexcept{
    return _mm256_mul_epu32(pack4_u32(a0,a1,a2,a3),pack4_u32(b0,b1,b2,b3));
}

EEZ_NTT880_ALWAYS_INLINE u64 hsum4_u64(vec x) noexcept{
    __m128i s=_mm_add_epi64(_mm256_castsi256_si128(x),_mm256_extracti128_si256(x,1));
    s=_mm_add_epi64(s,_mm_srli_si128(s,8));
    return static_cast<u64>(_mm_cvtsi128_si64(s));
}

EEZ_NTT880_ALWAYS_INLINE vec square8_packed(vec vx,word w) noexcept{
    alignas(32) word x[8],xw[8];
    vx=canonicalize8(shrink4_to_2(vx));
    w=canonicalize(w);
    _mm256_store_si256(reinterpret_cast<vec*>(x),vx);
    _mm256_store_si256(reinterpret_cast<vec*>(xw),mul8_fixed(vx,broadcast(w),broadcast(w*montgomery_ninv)));

    u64 a0=hsum4_u64(mul4_u32(x[0],x[0],twice(xw[1]),x[7],twice(xw[2]),x[6],twice(xw[3]),x[5]));
    const u64 a1=hsum4_u64(mul4_u32(twice(x[0]),x[1],twice(xw[2]),x[7],twice(xw[3]),x[6],twice(xw[4]),x[5]));
    u64 a2=hsum4_u64(mul4_u32(twice(x[0]),x[2],x[1],x[1],twice(xw[3]),x[7],twice(xw[4]),x[6]));
    const u64 a3=hsum4_u64(mul4_u32(twice(x[0]),x[3],twice(x[1]),x[2],twice(xw[4]),x[7],twice(xw[5]),x[6]));
    u64 a4=hsum4_u64(mul4_u32(twice(x[0]),x[4],twice(x[1]),x[3],x[2],x[2],twice(xw[5]),x[7]));
    const u64 a5=hsum4_u64(mul4_u32(twice(x[0]),x[5],twice(x[1]),x[4],twice(x[2]),x[3],twice(xw[6]),x[7]));
    u64 a6=hsum4_u64(mul4_u32(twice(x[0]),x[6],twice(x[1]),x[5],twice(x[2]),x[4],x[3],x[3]));
    const u64 a7=hsum4_u64(mul4_u32(twice(x[0]),x[7],twice(x[1]),x[6],twice(x[2]),x[5],twice(x[3]),x[4]));

    const vec extra=mul4_u32(xw[4],x[4],xw[5],x[5],xw[6],x[6],xw[7],x[7]);
    alignas(32) u64 e[4];
    _mm256_store_si256(reinterpret_cast<vec*>(e),extra);
    a0+=e[0];
    a2+=e[1];
    a4+=e[2];
    a6+=e[3];

    const vec lo=_mm256_setr_epi64x(static_cast<long long>(a0),static_cast<long long>(a1),static_cast<long long>(a2),static_cast<long long>(a3));
    const vec hi=_mm256_setr_epi64x(static_cast<long long>(a4),static_cast<long long>(a5),static_cast<long long>(a6),static_cast<long long>(a7));
    return shrink4_to_2(_mm256_set_m128i(reduce_four_accumulators(hi),reduce_four_accumulators(lo)));
}

EEZ_NTT880_ALWAYS_INLINE void leaf_square8x4(mint* EEZ_NTT880_RESTRICT a,usize first_block,const std::array<word,4>& modulus) noexcept{
    for(unsigned k=0;k<4;++k){
        const usize off=(first_block+k)*8;
        store8_aligned(a+off,square8_packed(load8_aligned(a+off),modulus[k]));
    }
}

EEZ_NTT880_ALWAYS_INLINE void leaf_square16x2_karatsuba(mint* EEZ_NTT880_RESTRICT a,usize first_block,const std::array<word,2>& modulus) noexcept{
    const vec split=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    for(unsigned k=0;k<2;++k){
        const usize off=(first_block+k)*16;
        const vec x0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off))),split);
        const vec x1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off+8))),split);
        const vec xe=_mm256_permute2x128_si256(x0,x1,0x20);
        const vec xo=_mm256_permute2x128_si256(x0,x1,0x31);
        const vec xs=canonicalize8(add8(xe,xo));
        const word w=canonicalize(modulus[k]);
        const vec p0=square8_packed(xe,w);
        const vec p1=square8_packed(xo,w);
        const vec p2=square8_packed(xs,w);
        vec yp1=_mm256_permutevar8x32_epi32(p1,_mm256_setr_epi32(7,0,1,2,3,4,5,6));
        yp1=_mm256_insert_epi32(yp1,canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1,7)),w)),0);
        const vec ce=add8(p0,yp1);
        const vec co=sub8(sub8(p2,p0),p1);
        const vec lo=_mm256_unpacklo_epi32(ce,co);
        const vec hi=_mm256_unpackhi_epi32(ce,co);
        store8_aligned(a+off,_mm256_permute2x128_si256(lo,hi,0x20));
        store8_aligned(a+off+8,_mm256_permute2x128_si256(lo,hi,0x31));
    }
}

template<unsigned leaf_size,unsigned parallel_blocks>
inline void leaf_products(mint* a,mint* b,usize n) noexcept{
    const usize blocks=n/leaf_size;
    word w=montgomery_one;
    for(usize s=0;s<blocks;s+=parallel_blocks){
        std::array<word,parallel_blocks> modulus{};
        for(unsigned k=0;k<parallel_blocks;++k){
            modulus[k]=w;
            const usize block=s+k;
            if(block+1<blocks)w=mul(w,forward_rate1(twiddle_index(static_cast<u32>(block))));
        }
        if constexpr(leaf_size==8)leaf_product8x4(a,b,s,modulus);
        else leaf_product16x2_karatsuba(a,b,s,modulus);
    }
}

template<unsigned leaf_size,unsigned parallel_blocks>
inline void leaf_squares(mint* a,usize n) noexcept{
    const usize blocks=n/leaf_size;
    word w=montgomery_one;
    for(usize s=0;s<blocks;s+=parallel_blocks){
        std::array<word,parallel_blocks> modulus{};
        for(unsigned k=0;k<parallel_blocks;++k){
            modulus[k]=w;
            const usize block=s+k;
            if(block+1<blocks)w=mul(w,forward_rate1(twiddle_index(static_cast<u32>(block))));
        }
        if constexpr(leaf_size==8)leaf_square8x4(a,s,modulus);
        else leaf_square16x2_karatsuba(a,s,modulus);
    }
}

inline void convolution_adaptive_inplace(mint* a,mint* b,usize n) noexcept{
    const unsigned leaf_log=adaptive_leaf_log(n);
    forward_adaptive_pair(a,b,n,leaf_log);
    if(leaf_log==3)leaf_products<8,4>(a,b,n);
    else leaf_products<16,2>(a,b,n);
    inverse_adaptive(a,n,leaf_log);
}

inline void square_adaptive_inplace(mint* a,usize n) noexcept{
    const unsigned leaf_log=adaptive_leaf_log(n);
    forward_adaptive(a,n,leaf_log);
    if(leaf_log==3)leaf_squares<8,4>(a,n);
    else leaf_squares<16,2>(a,n);
    inverse_adaptive(a,n,leaf_log);
}

inline void convolution_adaptive_normal_inplace(mint* a,mint* b,usize n) noexcept{
    const unsigned leaf_log=adaptive_leaf_log(n);
    forward_adaptive_pair<true>(a,b,n,leaf_log);
    if(leaf_log==3)leaf_products<8,4>(a,b,n);
    else leaf_products<16,2>(a,b,n);
    inverse_adaptive<true>(a,n,leaf_log);
}

inline void convolution_adaptive_mixed_normal_inplace(mint* a,mint* b,usize n) noexcept{
    const unsigned leaf_log=adaptive_leaf_log(n);
    forward_adaptive_pair(a,b,n,leaf_log);
    if(leaf_log==3)leaf_products<8,4>(a,b,n);
    else leaf_products<16,2>(a,b,n);
    inverse_adaptive<false,true>(a,n,leaf_log);
}

#endif

inline void pointwise_multiply(mint* EEZ_NTT880_RESTRICT a,const mint* EEZ_NTT880_RESTRICT b,usize n) noexcept{
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    for(;i+8<=n;i+=8)store8(a+i,mul8(load8(a+i),load8(b+i)));
#endif
    for(;i<n;++i)a[i]=from_raw(mul(raw(a[i]),raw(b[i])));
}

inline void pointwise_square(mint* EEZ_NTT880_RESTRICT a,usize n) noexcept{
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    for(;i+8<=n;i+=8){
        const vec x=load8(a+i);
        store8(a+i,mul8(x,x));
    }
#endif
    for(;i<n;++i)a[i]=from_raw(mul(raw(a[i]),raw(a[i])));
}

}

inline void forward(std::span<mint> a) noexcept{
    if(a.size()<=1)return;
    assert(valid_ntt_size(a.size()));
    detail::forward_dif(a.data(),a.size());
}

inline void inverse(std::span<mint> a) noexcept{
    if(a.size()<=1)return;
    assert(valid_ntt_size(a.size()));
    detail::inverse_dit(a.data(),a.size());
}

namespace detail{

inline std::vector<mint> convolution_naive(std::span<const mint> a,std::span<const mint> b){
    std::vector<mint> result(convolution_size(a.size(),b.size()));
    for(usize i=0;i<a.size();++i)
        for(usize j=0;j<b.size();++j)
            result[i+j]+=a[i]*b[j];
    return result;
}

inline std::vector<mint> square_naive(std::span<const mint> a){
    std::vector<mint> result(convolution_size(a.size(),a.size()));
    for(usize i=0;i<a.size();++i){
        result[2*i]+=a[i]*a[i];
        for(usize j=i+1;j<a.size();++j){
            const mint p=a[i]*a[j];
            result[i+j]+=p+p;
        }
    }
    return result;
}

inline usize checked_transform_size(usize n,usize m){
    const usize result=convolution_transform_size(n,m);
    if(n&&m&&!result)throw std::length_error("eez::ntt880: convolution exceeds the 2^25 transform limit");
    return result;
}

inline void require_ntt_size(usize n){
    if(!valid_ntt_size(n))throw std::invalid_argument("eez::ntt880: transform length must be a power of two in [1, 2^23]");
}

}

#if defined(__AVX2__) || defined(_M_AVX2)

using convolution_buffer=detail::aligned_vector;

inline void convolution_inplace(convolution_buffer& a,convolution_buffer& b){
    if(a.size()!=b.size()||!valid_convolution_transform_size(a.size()))
        throw std::invalid_argument("eez::ntt880::convolution_inplace: buffer sizes must match and be a power of two in [32, 2^25]");
    detail::convolution_adaptive_inplace(a.data(),b.data(),a.size());
}

#endif

inline std::vector<mint> convolution(std::span<const mint> a,std::span<const mint> b){
    if(a.empty()||b.empty())return {};
    if(a.data()==b.data()&&a.size()==b.size())return square(a);
    if(std::min(a.size(),b.size())<=naive_cutoff)return detail::convolution_naive(a,b);

    const usize result_size=convolution_size(a.size(),b.size());
    const usize n=detail::checked_transform_size(a.size(),b.size());
    detail::aligned_vector fa(n),fb(n);
    std::copy(a.begin(),a.end(),fa.begin());
    std::copy(b.begin(),b.end(),fb.begin());
    detail::convolution_adaptive_inplace(fa.data(),fb.data(),n);

    std::vector<mint> result(result_size);
    std::copy_n(fa.data(),result_size,result.data());
    return result;
}

inline std::vector<mint> square(std::span<const mint> a){
    if(a.empty())return {};
    if(a.size()<=naive_cutoff)return detail::square_naive(a);

    const usize result_size=convolution_size(a.size(),a.size());
    const usize n=detail::checked_transform_size(a.size(),a.size());
    detail::aligned_vector fa(n);
    std::copy(a.begin(),a.end(),fa.begin());
    detail::square_adaptive_inplace(fa.data(),n);

    std::vector<mint> result(result_size);
    std::copy_n(fa.data(),result_size,result.data());
    return result;
}

inline void convolution_to(std::span<const mint> a,std::span<const mint> b,std::span<mint> out,workspace& ws){
    if(a.empty()||b.empty())return;
    const usize result_size=convolution_size(a.size(),b.size());
    if(out.size()<result_size)throw std::invalid_argument("eez::ntt880::convolution_to: output span is too small");

    if(a.data()==b.data()&&a.size()==b.size()){
        square_to(a,out,ws);
        return;
    }

    if(std::min(a.size(),b.size())<=naive_cutoff){
        std::fill_n(out.begin(),result_size,mint{});
        for(usize i=0;i<a.size();++i)
            for(usize j=0;j<b.size();++j)
                out[i+j]+=a[i]*b[j];
        return;
    }

    const usize n=detail::checked_transform_size(a.size(),b.size());
    ws.reserve(n);
    std::fill_n(ws.a_.begin(),n,mint{});
    std::fill_n(ws.b_.begin(),n,mint{});
    std::copy(a.begin(),a.end(),ws.a_.begin());
    std::copy(b.begin(),b.end(),ws.b_.begin());
    detail::convolution_adaptive_inplace(ws.a_.data(),ws.b_.data(),n);
    std::copy_n(ws.a_.begin(),result_size,out.begin());
}

inline void square_to(std::span<const mint> a,std::span<mint> out,workspace& ws){
    if(a.empty())return;
    const usize result_size=convolution_size(a.size(),a.size());
    if(out.size()<result_size)throw std::invalid_argument("eez::ntt880::square_to: output span is too small");

    if(a.size()<=naive_cutoff){
        std::fill_n(out.begin(),result_size,mint{});
        for(usize i=0;i<a.size();++i){
            out[2*i]+=a[i]*a[i];
            for(usize j=i+1;j<a.size();++j){
                const mint p=a[i]*a[j];
                out[i+j]+=p+p;
            }
        }
        return;
    }

    const usize n=detail::checked_transform_size(a.size(),a.size());
    ws.reserve(n);
    std::fill_n(ws.a_.begin(),n,mint{});
    std::copy(a.begin(),a.end(),ws.a_.begin());
    detail::square_adaptive_inplace(ws.a_.data(),n);
    std::copy_n(ws.a_.begin(),result_size,out.begin());
}

inline void forward_to(std::span<const mint> src,frequency_buffer& dst,usize n){
    detail::require_ntt_size(n);
    if(src.size()>n)throw std::invalid_argument("eez::ntt880::forward_to: source is longer than transform");
    dst.data_.assign(n,mint{});
    std::copy(src.begin(),src.end(),dst.data_.begin());
    detail::forward_dif(dst.data_.data(),n);
}

inline void pointwise_multiply(frequency_buffer& lhs,const frequency_buffer& rhs){
    if(lhs.size()!=rhs.size())throw std::invalid_argument("eez::ntt880::pointwise_multiply: transform sizes differ");
    detail::pointwise_multiply(lhs.data_.data(),rhs.data_.data(),lhs.data_.size());
}

inline void pointwise_square(frequency_buffer& a){
    detail::pointwise_square(a.data_.data(),a.data_.size());
}

inline void inverse_to(frequency_buffer& src,std::span<mint> out){
    if(src.data_.empty()){
        if(!out.empty())throw std::invalid_argument("eez::ntt880::inverse_to: empty transform");
        return;
    }
    if(out.size()>src.data_.size())throw std::invalid_argument("eez::ntt880::inverse_to: output is longer than transform");
    detail::inverse_dit(src.data_.data(),src.data_.size());
    std::copy_n(src.data_.begin(),out.size(),out.begin());
}

}

#undef EEZ_NTT880_ALWAYS_INLINE
#undef EEZ_NTT880_RESTRICT


// Public streaming API preserving submission #393594's mixed-normal path.

// read(): next coefficient in [0, mod); called for n values, then m values.

// write(u32): receives n+m-1 canonical coefficients, or none for an empty input.

// This API never exposes normal-representation data as ordinary modint values.

namespace eez::ntt880{
template<class Reader,class Writer>
inline void convolution_normal_io(usize n,usize m,Reader&& read,Writer&& write){
    if(std::min(n,m)<=naive_cutoff){
        std::vector<mint> a(n),b(m);
        for(auto& x:a)x=read();
        for(auto& x:b)x=read();
        const auto c=convolution(a,b);
        for(const auto& x:c)write(x.get());
        return;
    }
    const usize z=detail::checked_transform_size(n,m);
    const usize result_size=n+m-1;
    detail::aligned_uninitialized_buffer a(z),b(z);
    mint* const first=n<m?b.data():a.data();
    mint* const second=n<m?a.data():b.data();
    for(usize i=0;i<n;++i)
        std::construct_at(first+i,mint::montgomery_raw(read()));
    for(usize i=0;i<m;++i)
        std::construct_at(second+i,mint::montgomery_raw(read()));
    const usize a_size=std::max(n,m),b_size=std::min(n,m);
    std::uninitialized_value_construct_n(a.data()+a_size,z-a_size);
    std::uninitialized_value_construct_n(b.data()+b_size,z-b_size);
    detail::convert_to_montgomery(b.data(),(b_size+7)&~usize(7));
    detail::convolution_adaptive_mixed_normal_inplace(a.data(),b.data(),z);
    for(usize i=0;i<result_size;++i)write(a.data()[i].a);
}
}

#line 2 "convolution/ntt754.hpp"
#if !defined(__AVX2__) && !defined(_M_AVX2)
#error "Compile with -mavx2 (see README.md)."
#endif
#line 2 "ntt754.hpp"

#if defined(__GNUC__) && !defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#elif defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma clang attribute push( \
    __attribute__((target("avx2,bmi,bmi2,lzcnt,popcnt,ssse3"))), \
    apply_to = function)
#endif

#line 3916 "convolution_mod1000000007.hpp"

#line 3918 "convolution_mod1000000007.hpp"


#line 1 "math/modint754.hpp"

#line 3923 "convolution_mod1000000007.hpp"


struct modint754 {
    using u32 = std::uint32_t;
    using i32 = std::int32_t;
    using u64 = std::uint64_t;

    static constexpr u32 MOD = 754974721u;
    static constexpr u32 MOD2 = MOD * 2;
    static constexpr u32 primitive_root = 11;
    static constexpr int max_power_of_two = 24;

private:
    static constexpr u32 R = 3539992577u;
    static constexpr u32 N2 = 749009521u;

    struct montgomery_tag {};

    constexpr modint754(u32 x, montgomery_tag) : a(x) {}

    static constexpr u32 reduce(u64 x) {
        return static_cast<u32>(
            (x + u64(static_cast<u32>(x) * u32(-R)) * MOD) >> 32
        );
    }

public:
    u32 a;

    static_assert(MOD < (u32(1) << 30));
    static_assert((MOD & 1) != 0);
    static_assert(R * MOD == 1);

    constexpr modint754() : a(0) {}

    template <class T, std::enable_if_t<std::is_integral_v<T> &&
                                        std::is_signed_v<T>, int> = 0>
    constexpr modint754(T x) : a(0) {
        const std::int64_t y =
            static_cast<std::int64_t>(x) % std::int64_t(MOD) + MOD;
        a = reduce(u64(y) * N2);
    }

    template <class T, std::enable_if_t<std::is_integral_v<T> &&
                                        std::is_unsigned_v<T>, int> = 0>
    constexpr modint754(T x)
        : a(reduce(((u64(x) % MOD) + MOD) * N2)) {}

    static constexpr modint754 raw(u32 x) {
        return modint754(reduce(u64(x) * N2), montgomery_tag{});
    }

    static constexpr modint754 montgomery_raw(u32 x) {
        return modint754(x, montgomery_tag{});
    }

    static constexpr u32 mod() { return MOD; }
    static constexpr u32 get_mod() { return MOD; }

    constexpr u32 val() const {
        const u32 x = reduce(a);
        return x >= MOD ? x - MOD : x;
    }

    constexpr u32 get() const { return val(); }

    constexpr modint754& operator+=(const modint754& rhs) {
        a += rhs.a - MOD2;
        if (i32(a) < 0) a += MOD2;
        return *this;
    }

    constexpr modint754& operator-=(const modint754& rhs) {
        a -= rhs.a;
        if (i32(a) < 0) a += MOD2;
        return *this;
    }

    constexpr modint754& operator*=(const modint754& rhs) {
        a = reduce(u64(a) * rhs.a);
        return *this;
    }

    constexpr modint754& operator/=(const modint754& rhs) {
        return *this *= rhs.inv();
    }

    constexpr modint754 operator+() const { return *this; }
    constexpr modint754 operator-() const { return modint754() - *this; }

    friend constexpr modint754 operator+(modint754 lhs, const modint754& rhs) {
        return lhs += rhs;
    }

    friend constexpr modint754 operator-(modint754 lhs, const modint754& rhs) {
        return lhs -= rhs;
    }

    friend constexpr modint754 operator*(modint754 lhs, const modint754& rhs) {
        return lhs *= rhs;
    }

    friend constexpr modint754 operator/(modint754 lhs, const modint754& rhs) {
        return lhs /= rhs;
    }

    friend constexpr bool operator==(const modint754& lhs, const modint754& rhs) {
        const u32 x = lhs.a >= MOD ? lhs.a - MOD : lhs.a;
        const u32 y = rhs.a >= MOD ? rhs.a - MOD : rhs.a;
        return x == y;
    }

    friend constexpr bool operator!=(const modint754& lhs, const modint754& rhs) {
        return !(lhs == rhs);
    }

    constexpr modint754& operator++() {
        return *this += raw(1);
    }

    constexpr modint754 operator++(int) {
        modint754 old = *this;
        ++*this;
        return old;
    }

    constexpr modint754& operator--() {
        return *this -= raw(1);
    }

    constexpr modint754 operator--(int) {
        modint754 old = *this;
        --*this;
        return old;
    }

    constexpr modint754 pow(u64 exponent) const {
        modint754 result = raw(1);
        modint754 base = *this;
        while (exponent != 0) {
            if (exponent & 1) result *= base;
            base *= base;
            exponent >>= 1;
        }
        return result;
    }

    constexpr modint754 inv() const {
        assert(val() != 0);
        return pow(MOD - 2);
    }

    constexpr modint754 inverse() const { return inv(); }

    friend std::ostream& operator<<(std::ostream& os, const modint754& x) {
        return os << x.val();
    }

    friend std::istream& operator>>(std::istream& is, modint754& x) {
        std::int64_t value;
        is >> value;
        x = modint754(value);
        return is;
    }
};

static_assert(sizeof(modint754) == 4);
static_assert(std::is_trivially_copyable_v<modint754>);

using mint754 = modint754;

#line 18 "ntt754.hpp"

#if defined(_MSC_VER)
#define EEZ_NTT754_ALWAYS_INLINE __forceinline
#define EEZ_NTT754_RESTRICT __restrict
#elif defined(__GNUC__) || defined(__clang__)
#define EEZ_NTT754_ALWAYS_INLINE inline __attribute__((always_inline))
#define EEZ_NTT754_RESTRICT __restrict__
#else
#define EEZ_NTT754_ALWAYS_INLINE inline
#define EEZ_NTT754_RESTRICT
#endif

namespace eez::ntt754{

using mint=modint754;
using u32=std::uint32_t;
using usize=std::size_t;

inline constexpr u32 mod=mint::MOD;
inline constexpr usize max_ntt_size=usize(1)<<23;
inline constexpr usize max_convolution_size=usize(1)<<25;
inline constexpr usize max_size=max_ntt_size;
inline constexpr usize naive_cutoff=60;

inline void forward(std::span<mint> a) noexcept;
inline void inverse(std::span<mint> a) noexcept;
inline std::vector<mint> convolution(std::span<const mint> a,std::span<const mint> b);
inline std::vector<mint> square(std::span<const mint> a);

inline std::vector<mint> convolution(const std::vector<mint>& a,const std::vector<mint>& b){
    return convolution(std::span<const mint>(a.data(),a.size()),std::span<const mint>(b.data(),b.size()));
}

inline std::vector<mint> square(const std::vector<mint>& a){
    return square(std::span<const mint>(a.data(),a.size()));
}

namespace detail{

template<class T>
class aligned_allocator{
public:
    using value_type=T;
    using is_always_equal=std::true_type;
    aligned_allocator() noexcept=default;
    template<class U> constexpr aligned_allocator(const aligned_allocator<U>&) noexcept{}
    [[nodiscard]] T* allocate(usize n){
        return static_cast<T*>(::operator new(n*sizeof(T),std::align_val_t{64}));
    }
    void deallocate(T* p,usize) noexcept{
        ::operator delete(p,std::align_val_t{64});
    }
    template<class U> struct rebind{using other=aligned_allocator<U>;};
};

template<class T,class U>
constexpr bool operator==(const aligned_allocator<T>&,const aligned_allocator<U>&) noexcept{return true;}

template<class T,class U>
constexpr bool operator!=(const aligned_allocator<T>&,const aligned_allocator<U>&) noexcept{return false;}

using aligned_vector=std::vector<mint,aligned_allocator<mint>>;

}

class workspace{
public:
    workspace()=default;
    explicit workspace(usize n){reserve(n);}
    void reserve(usize n){
        if(a_.size()<n)a_.resize(n);
        if(b_.size()<n)b_.resize(n);
    }
    [[nodiscard]] usize capacity()const noexcept{return std::min(a_.size(),b_.size());}
private:
    friend void convolution_to(std::span<const mint>,std::span<const mint>,std::span<mint>,workspace&);
    friend void square_to(std::span<const mint>,std::span<mint>,workspace&);
    detail::aligned_vector a_;
    detail::aligned_vector b_;
};

inline void convolution_to(std::span<const mint> a,std::span<const mint> b,std::span<mint> out,workspace& ws);
inline void square_to(std::span<const mint> a,std::span<mint> out,workspace& ws);

class frequency_buffer{
public:
    frequency_buffer()=default;
    [[nodiscard]] usize size()const noexcept{return data_.size();}
private:
    friend void forward_to(std::span<const mint>,frequency_buffer&,usize);
    friend void pointwise_multiply(frequency_buffer&,const frequency_buffer&);
    friend void pointwise_square(frequency_buffer&);
    friend void inverse_to(frequency_buffer&,std::span<mint>);
    std::vector<mint> data_;
};

inline void forward_to(std::span<const mint> src,frequency_buffer& dst,usize n);
inline void pointwise_multiply(frequency_buffer& lhs,const frequency_buffer& rhs);
inline void pointwise_square(frequency_buffer& a);
inline void inverse_to(frequency_buffer& src,std::span<mint> out);

constexpr usize convolution_size(usize n,usize m) noexcept{
    return n&&m?n+m-1:0;
}

constexpr usize transform_size(usize n,usize m) noexcept{
    if(!n||!m)return 0;
    if(n>max_ntt_size||m>max_ntt_size)return 0;
    if(n>max_ntt_size-m+1)return 0;
    const usize z=n+m-1;
    usize x=1;
    while(x<z)x<<=1;
    return x;
}

constexpr usize convolution_transform_size(usize n,usize m) noexcept{
    if(!n||!m)return 0;
    if(n>max_convolution_size||m>max_convolution_size)return 0;
    if(n>max_convolution_size-m+1)return 0;
    const usize z=n+m-1;
    usize x=1;
    while(x<z)x<<=1;
    return x;
}

constexpr bool valid_ntt_size(usize n) noexcept{
    return n!=0&&(n&(n-1))==0&&n<=max_ntt_size;
}

constexpr bool valid_convolution_transform_size(usize n) noexcept{
    return n>=32&&(n&(n-1))==0&&n<=max_convolution_size;
}

namespace detail{

using word=u32;
using u64=std::uint64_t;

inline constexpr word mod=mint::MOD;
inline constexpr word mod2=2*mod;
inline constexpr unsigned max_log=23;
inline constexpr word montgomery_ninv=754974719u;
inline constexpr word montgomery_one=mint::raw(1).a;

static_assert(mod<(word(1)<<30));
static_assert(word(mod*montgomery_ninv)==~word(0));
static_assert(sizeof(mint)==sizeof(word));

EEZ_NTT754_ALWAYS_INLINE constexpr word raw(const mint& x) noexcept{return x.a;}
EEZ_NTT754_ALWAYS_INLINE constexpr mint from_raw(word x) noexcept{return mint::montgomery_raw(x);}

EEZ_NTT754_ALWAYS_INLINE constexpr word mul(word a,word b) noexcept{
    const u64 x=u64(a)*b;
    const word q=static_cast<word>(x)*montgomery_ninv;
    return static_cast<word>((x+u64(q)*mod)>>32);
}

EEZ_NTT754_ALWAYS_INLINE constexpr word add(word a,word b) noexcept{
    const word x=a+b;
    return x>=mod2?x-mod2:x;
}

EEZ_NTT754_ALWAYS_INLINE constexpr word sub(word a,word b) noexcept{
    return a>=b?a-b:a+mod2-b;
}

EEZ_NTT754_ALWAYS_INLINE constexpr word canonicalize(word a) noexcept{
    return a>=mod?a-mod:a;
}

struct twiddle_table{
    std::array<word,max_log+1> root{};
    std::array<word,max_log+1> iroot{};
    std::array<word,max_log+1> rate1{};
    std::array<word,max_log+1> rate3{};
    std::array<word,max_log+1> irate3{};

    constexpr twiddle_table(){
        root[max_log]=mint::raw(mint::primitive_root).pow((mod-1)>>max_log).a;
        iroot[max_log]=mint::montgomery_raw(root[max_log]).inv().a;
        for(int i=int(max_log)-1;i>=0;--i){
            root[usize(i)]=mul(root[usize(i+1)],root[usize(i+1)]);
            iroot[usize(i)]=mul(iroot[usize(i+1)],iroot[usize(i+1)]);
        }
        word prod=montgomery_one;
        for(unsigned i=0;i+1<=max_log;++i){
            rate1[i]=mul(root[i+1],prod);
            prod=mul(prod,iroot[i+1]);
        }
        prod=montgomery_one;
        word iprod=montgomery_one;
        for(unsigned i=0;i+3<=max_log;++i){
            rate3[i]=mul(root[i+3],prod);
            irate3[i]=mul(iroot[i+3],iprod);
            prod=mul(prod,iroot[i+3]);
            iprod=mul(iprod,root[i+3]);
        }
    }
};

inline constexpr twiddle_table twiddles{};

EEZ_NTT754_ALWAYS_INLINE word forward_rate1(unsigned i) noexcept{return twiddles.rate1[i];}
EEZ_NTT754_ALWAYS_INLINE word forward_rate3(unsigned i) noexcept{return twiddles.rate3[i];}
EEZ_NTT754_ALWAYS_INLINE word inverse_rate3(unsigned i) noexcept{return twiddles.irate3[i];}

EEZ_NTT754_ALWAYS_INLINE unsigned twiddle_index(u32 block) noexcept{
    return static_cast<unsigned>(std::countr_zero(~block));
}

#if defined(__AVX2__) || defined(_M_AVX2)

using vec=__m256i;

EEZ_NTT754_ALWAYS_INLINE vec load8(const mint* p) noexcept{
    return _mm256_loadu_si256(reinterpret_cast<const __m256i*>(static_cast<const void*>(p)));
}

EEZ_NTT754_ALWAYS_INLINE void store8(mint* p,vec x) noexcept{
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(static_cast<void*>(p)),x);
}

EEZ_NTT754_ALWAYS_INLINE vec broadcast(word x) noexcept{
    return _mm256_set1_epi32(static_cast<int>(x));
}

EEZ_NTT754_ALWAYS_INLINE vec add8(vec a,vec b) noexcept{
    const vec two_p=broadcast(mod2);
    vec x=_mm256_sub_epi32(_mm256_add_epi32(a,b),two_p);
    return _mm256_add_epi32(x,_mm256_and_si256(_mm256_srai_epi32(x,31),two_p));
}

EEZ_NTT754_ALWAYS_INLINE vec sub8(vec a,vec b) noexcept{
    const vec two_p=broadcast(mod2);
    vec x=_mm256_sub_epi32(a,b);
    return _mm256_add_epi32(x,_mm256_and_si256(_mm256_srai_epi32(x,31),two_p));
}

EEZ_NTT754_ALWAYS_INLINE vec mul8(vec a,vec b) noexcept{
    const vec ninv=broadcast(montgomery_ninv);
    const vec prime=broadcast(mod);
    const vec pe=_mm256_mul_epu32(a,b);
    const vec po=_mm256_mul_epu32(_mm256_bsrli_epi128(a,4),_mm256_bsrli_epi128(b,4));
    const vec qe=_mm256_mul_epu32(pe,ninv);
    const vec qo=_mm256_mul_epu32(po,ninv);
    const vec re=_mm256_add_epi64(pe,_mm256_mul_epu32(qe,prime));
    const vec ro=_mm256_add_epi64(po,_mm256_mul_epu32(qo,prime));
    return _mm256_or_si256(_mm256_bsrli_epi128(re,4),ro);
}

EEZ_NTT754_ALWAYS_INLINE vec mul8_fixed(vec a,vec b,vec bninv) noexcept{
    const vec prime=broadcast(mod);
    const vec oa=_mm256_bsrli_epi128(a,4);
    const vec pe=_mm256_mul_epu32(a,b);
    const vec po=_mm256_mul_epu32(oa,b);
    const vec qe=_mm256_mul_epu32(a,bninv);
    const vec qo=_mm256_mul_epu32(oa,bninv);
    const vec re=_mm256_add_epi64(pe,_mm256_mul_epu32(qe,prime));
    const vec ro=_mm256_add_epi64(po,_mm256_mul_epu32(qo,prime));
    return _mm256_or_si256(_mm256_bsrli_epi128(re,4),ro);
}

EEZ_NTT754_ALWAYS_INLINE vec canonicalize8(vec x) noexcept{const vec p=broadcast(mod);return _mm256_min_epu32(x,_mm256_sub_epi32(x,p));}

EEZ_NTT754_ALWAYS_INLINE vec pack_four(word x0,word x1) noexcept{
    return _mm256_setr_epi32(
        static_cast<int>(x0),static_cast<int>(x0),static_cast<int>(x0),static_cast<int>(x0),
        static_cast<int>(x1),static_cast<int>(x1),static_cast<int>(x1),static_cast<int>(x1)
    );
}

EEZ_NTT754_ALWAYS_INLINE vec load2x4(const mint* p0,const mint* p1) noexcept{
    const __m128i lo=_mm_loadu_si128(reinterpret_cast<const __m128i*>(static_cast<const void*>(p0)));
    const __m128i hi=_mm_loadu_si128(reinterpret_cast<const __m128i*>(static_cast<const void*>(p1)));
    return _mm256_set_m128i(hi,lo);
}

EEZ_NTT754_ALWAYS_INLINE void store2x4(mint* p0,mint* p1,vec x) noexcept{
    _mm_storeu_si128(reinterpret_cast<__m128i*>(static_cast<void*>(p0)),_mm256_castsi256_si128(x));
    _mm_storeu_si128(reinterpret_cast<__m128i*>(static_cast<void*>(p1)),_mm256_extracti128_si256(x,1));
}

EEZ_NTT754_ALWAYS_INLINE void transpose_8x4_to_4x8(vec v0,vec v1,vec v2,vec v3,vec& x0,vec& x1,vec& x2,vec& x3) noexcept{
    const vec t0=_mm256_unpacklo_epi32(v0,v1);
    const vec t1=_mm256_unpackhi_epi32(v0,v1);
    const vec t2=_mm256_unpacklo_epi32(v2,v3);
    const vec t3=_mm256_unpackhi_epi32(v2,v3);
    const vec perm=_mm256_setr_epi32(0,4,1,5,2,6,3,7);
    x0=_mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(t0,t2),perm);
    x1=_mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(t0,t2),perm);
    x2=_mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(t1,t3),perm);
    x3=_mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(t1,t3),perm);
}

EEZ_NTT754_ALWAYS_INLINE void transpose_4x8_to_8x4(vec x0,vec x1,vec x2,vec x3,vec& v0,vec& v1,vec& v2,vec& v3) noexcept{
    const vec perm=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    const vec q0=_mm256_permutevar8x32_epi32(x0,perm);
    const vec q1=_mm256_permutevar8x32_epi32(x1,perm);
    const vec q2=_mm256_permutevar8x32_epi32(x2,perm);
    const vec q3=_mm256_permutevar8x32_epi32(x3,perm);
    const vec t0=_mm256_unpacklo_epi64(q0,q1);
    const vec t2=_mm256_unpackhi_epi64(q0,q1);
    const vec t1=_mm256_unpacklo_epi64(q2,q3);
    const vec t3=_mm256_unpackhi_epi64(q2,q3);
    v0=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t0),_mm256_castsi256_ps(t1),_MM_SHUFFLE(2,0,2,0)));
    v1=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t0),_mm256_castsi256_ps(t1),_MM_SHUFFLE(3,1,3,1)));
    v2=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t2),_mm256_castsi256_ps(t3),_MM_SHUFFLE(2,0,2,0)));
    v3=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t2),_mm256_castsi256_ps(t3),_MM_SHUFFLE(3,1,3,1)));
}

#endif

EEZ_NTT754_ALWAYS_INLINE void forward_butterfly(mint* b,usize stride,usize i,word r1,word r2,word r3) noexcept{
    const word x0=raw(b[i]);
    const word x1=mul(raw(b[stride+i]),r1);
    const word x2=mul(raw(b[2*stride+i]),r2);
    const word x3=mul(raw(b[3*stride+i]),r3);
    const word s02=add(x0,x2);
    const word d02=sub(x0,x2);
    const word s13=add(x1,x3);
    const word t=mul(sub(x1,x3),twiddles.root[2]);
    b[i]=from_raw(add(s02,s13));
    b[stride+i]=from_raw(sub(s02,s13));
    b[2*stride+i]=from_raw(add(d02,t));
    b[3*stride+i]=from_raw(sub(d02,t));
}

EEZ_NTT754_ALWAYS_INLINE void inverse_butterfly(mint* b,usize stride,usize i,word r1,word r2,word r3) noexcept{
    const word x0=raw(b[i]);
    const word x1=raw(b[stride+i]);
    const word x2=raw(b[2*stride+i]);
    const word x3=raw(b[3*stride+i]);
    const word s01=add(x0,x1);
    const word d01=sub(x0,x1);
    const word s23=add(x2,x3);
    const word t=mul(sub(x2,x3),twiddles.iroot[2]);
    b[i]=from_raw(add(s01,s23));
    b[stride+i]=from_raw(mul(add(d01,t),r1));
    b[2*stride+i]=from_raw(mul(sub(s01,s23),r2));
    b[3*stride+i]=from_raw(mul(sub(d01,t),r3));
}

inline void forward_radix4_scalar(mint* EEZ_NTT754_RESTRICT a,usize blocks,usize stride) noexcept{
    {
        mint* const b=a;
        for(usize i=0;i<stride;++i){
            const word x0=raw(b[i]);
            const word x1=raw(b[stride+i]);
            const word x2=raw(b[2*stride+i]);
            const word x3=raw(b[3*stride+i]);
            const word s02=add(x0,x2);
            const word d02=sub(x0,x2);
            const word s13=add(x1,x3);
            const word t=mul(sub(x1,x3),twiddles.root[2]);
            b[i]=from_raw(add(s02,s13));
            b[stride+i]=from_raw(sub(s02,s13));
            b[2*stride+i]=from_raw(add(d02,t));
            b[3*stride+i]=from_raw(sub(d02,t));
        }
    }
    if(blocks==1)return;
    word rot=forward_rate3(0);
    for(usize s=1;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        mint* const b=a+s*4*stride;
        for(usize i=0;i<stride;++i)forward_butterfly(b,stride,i,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

inline void inverse_radix4_scalar(mint* EEZ_NTT754_RESTRICT a,usize blocks,usize stride) noexcept{
    {
        mint* const b=a;
        for(usize i=0;i<stride;++i){
            const word x0=raw(b[i]);
            const word x1=raw(b[stride+i]);
            const word x2=raw(b[2*stride+i]);
            const word x3=raw(b[3*stride+i]);
            const word s01=add(x0,x1);
            const word d01=sub(x0,x1);
            const word s23=add(x2,x3);
            const word t=mul(sub(x2,x3),twiddles.iroot[2]);
            b[i]=from_raw(add(s01,s23));
            b[stride+i]=from_raw(add(d01,t));
            b[2*stride+i]=from_raw(sub(s01,s23));
            b[3*stride+i]=from_raw(sub(d01,t));
        }
    }
    if(blocks==1)return;
    word rot=inverse_rate3(0);
    for(usize s=1;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        mint* const b=a+s*4*stride;
        for(usize i=0;i<stride;++i)inverse_butterfly(b,stride,i,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

#if defined(__AVX2__) || defined(_M_AVX2)

EEZ_NTT754_ALWAYS_INLINE void forward_radix4_large_block(mint* EEZ_NTT754_RESTRICT b,usize stride,vec imag,word r1,word r2,word r3) noexcept{
    const vec w1=broadcast(r1);
    const vec w2=broadcast(r2);
    const vec w3=broadcast(r3);
    for(usize i=0;i<stride;i+=8){
        const vec x0=load8(b+i);
        const vec x1=mul8(load8(b+stride+i),w1);
        const vec x2=mul8(load8(b+2*stride+i),w2);
        const vec x3=mul8(load8(b+3*stride+i),w3);
        const vec s02=add8(x0,x2);
        const vec d02=sub8(x0,x2);
        const vec s13=add8(x1,x3);
        const vec t=mul8(sub8(x1,x3),imag);
        store8(b+i,add8(s02,s13));
        store8(b+stride+i,sub8(s02,s13));
        store8(b+2*stride+i,add8(d02,t));
        store8(b+3*stride+i,sub8(d02,t));
    }
}

EEZ_NTT754_ALWAYS_INLINE void inverse_radix4_large_block(mint* EEZ_NTT754_RESTRICT b,usize stride,vec iimag,word r1,word r2,word r3) noexcept{
    const vec w1=broadcast(r1);
    const vec w2=broadcast(r2);
    const vec w3=broadcast(r3);
    for(usize i=0;i<stride;i+=8){
        const vec x0=load8(b+i);
        const vec x1=load8(b+stride+i);
        const vec x2=load8(b+2*stride+i);
        const vec x3=load8(b+3*stride+i);
        const vec s01=add8(x0,x1);
        const vec d01=sub8(x0,x1);
        const vec s23=add8(x2,x3);
        const vec t=mul8(sub8(x2,x3),iimag);
        store8(b+i,add8(s01,s23));
        store8(b+stride+i,mul8(add8(d01,t),w1));
        store8(b+2*stride+i,mul8(sub8(s01,s23),w2));
        store8(b+3*stride+i,mul8(sub8(d01,t),w3));
    }
}

inline void forward_radix4_large(mint* EEZ_NTT754_RESTRICT a,usize blocks,usize stride) noexcept{
    const vec imag=broadcast(twiddles.root[2]);
    {
        mint* const b=a;
        for(usize i=0;i<stride;i+=8){
            const vec x0=load8(b+i);
            const vec x1=load8(b+stride+i);
            const vec x2=load8(b+2*stride+i);
            const vec x3=load8(b+3*stride+i);
            const vec s02=add8(x0,x2);
            const vec d02=sub8(x0,x2);
            const vec s13=add8(x1,x3);
            const vec t=mul8(sub8(x1,x3),imag);
            store8(b+i,add8(s02,s13));
            store8(b+stride+i,sub8(s02,s13));
            store8(b+2*stride+i,add8(d02,t));
            store8(b+3*stride+i,sub8(d02,t));
        }
    }
    if(blocks==1)return;
    word rot=forward_rate3(0);
    usize s=1;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8],r2[8],r3[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        _mm256_store_si256(reinterpret_cast<vec*>(r2),w2);
        _mm256_store_si256(reinterpret_cast<vec*>(r3),w3);
        for(unsigned lane=0;lane<8;++lane)forward_radix4_large_block(a+(s+lane)*4*stride,stride,imag,r1[lane],r2[lane],r3[lane]);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        forward_radix4_large_block(a+s*4*stride,stride,imag,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

inline void inverse_radix4_large(mint* EEZ_NTT754_RESTRICT a,usize blocks,usize stride) noexcept{
    const vec iimag=broadcast(twiddles.iroot[2]);
    {
        mint* const b=a;
        for(usize i=0;i<stride;i+=8){
            const vec x0=load8(b+i);
            const vec x1=load8(b+stride+i);
            const vec x2=load8(b+2*stride+i);
            const vec x3=load8(b+3*stride+i);
            const vec s01=add8(x0,x1);
            const vec d01=sub8(x0,x1);
            const vec s23=add8(x2,x3);
            const vec t=mul8(sub8(x2,x3),iimag);
            store8(b+i,add8(s01,s23));
            store8(b+stride+i,add8(d01,t));
            store8(b+2*stride+i,sub8(s01,s23));
            store8(b+3*stride+i,sub8(d01,t));
        }
    }
    if(blocks==1)return;
    word rot=inverse_rate3(0);
    usize s=1;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8],r2[8],r3[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        _mm256_store_si256(reinterpret_cast<vec*>(r2),w2);
        _mm256_store_si256(reinterpret_cast<vec*>(r3),w3);
        for(unsigned lane=0;lane<8;++lane)inverse_radix4_large_block(a+(s+lane)*4*stride,stride,iimag,r1[lane],r2[lane],r3[lane]);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        inverse_radix4_large_block(a+s*4*stride,stride,iimag,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

inline void forward_radix4_p4(mint* EEZ_NTT754_RESTRICT a,usize blocks) noexcept{
    if(blocks<2){
        forward_radix4_scalar(a,blocks,4);
        return;
    }
    const vec imag=broadcast(twiddles.root[2]);
    word rot=montgomery_one;
    for(usize s=0;s<blocks;s+=2){
        const word r10=rot;
        rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
        const word r11=rot;
        const vec w1=pack_four(r10,r11);
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        mint* const b0=a+s*16;
        mint* const b1=b0+16;
        const vec x0=load2x4(b0,b1);
        const vec x1=mul8(load2x4(b0+4,b1+4),w1);
        const vec x2=mul8(load2x4(b0+8,b1+8),w2);
        const vec x3=mul8(load2x4(b0+12,b1+12),w3);
        const vec s02=add8(x0,x2);
        const vec d02=sub8(x0,x2);
        const vec s13=add8(x1,x3);
        const vec t=mul8(sub8(x1,x3),imag);
        store2x4(b0,b1,add8(s02,s13));
        store2x4(b0+4,b1+4,sub8(s02,s13));
        store2x4(b0+8,b1+8,add8(d02,t));
        store2x4(b0+12,b1+12,sub8(d02,t));
        if(s+2<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+1))));
    }
}

inline void inverse_radix4_p4(mint* EEZ_NTT754_RESTRICT a,usize blocks) noexcept{
    if(blocks<2){
        inverse_radix4_scalar(a,blocks,4);
        return;
    }
    const vec iimag=broadcast(twiddles.iroot[2]);
    word rot=montgomery_one;
    for(usize s=0;s<blocks;s+=2){
        const word r10=rot;
        rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
        const word r11=rot;
        const vec w1=pack_four(r10,r11);
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        mint* const b0=a+s*16;
        mint* const b1=b0+16;
        const vec x0=load2x4(b0,b1);
        const vec x1=load2x4(b0+4,b1+4);
        const vec x2=load2x4(b0+8,b1+8);
        const vec x3=load2x4(b0+12,b1+12);
        const vec s01=add8(x0,x1);
        const vec d01=sub8(x0,x1);
        const vec s23=add8(x2,x3);
        const vec t=mul8(sub8(x2,x3),iimag);
        store2x4(b0,b1,add8(s01,s23));
        store2x4(b0+4,b1+4,mul8(add8(d01,t),w1));
        store2x4(b0+8,b1+8,mul8(sub8(s01,s23),w2));
        store2x4(b0+12,b1+12,mul8(sub8(d01,t),w3));
        if(s+2<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+1))));
    }
}

inline void forward_radix4_p1(mint* EEZ_NTT754_RESTRICT a,usize blocks) noexcept{
    const vec imag=broadcast(twiddles.root[2]);
    word rot=montgomery_one;
    usize s=0;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        mint* const b=a+4*s;
        vec x0,x1,x2,x3;
        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);
        x1=mul8(x1,w1);
        x2=mul8(x2,w2);
        x3=mul8(x3,w3);
        const vec s02=add8(x0,x2);
        const vec d02=sub8(x0,x2);
        const vec s13=add8(x1,x3);
        const vec t=mul8(sub8(x1,x3),imag);
        vec v0,v1,v2,v3;
        transpose_4x8_to_8x4(add8(s02,s13),sub8(s02,s13),add8(d02,t),sub8(d02,t),v0,v1,v2,v3);
        store8(b,v0);
        store8(b+8,v1);
        store8(b+16,v2);
        store8(b+24,v3);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        forward_butterfly(a+4*s,1,0,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

inline void inverse_radix4_p1(mint* EEZ_NTT754_RESTRICT a,usize blocks) noexcept{
    const vec iimag=broadcast(twiddles.iroot[2]);
    word rot=montgomery_one;
    usize s=0;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));
        const vec w2=mul8(w1,w1);
        const vec w3=mul8(w2,w1);
        mint* const b=a+4*s;
        vec x0,x1,x2,x3;
        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);
        const vec s01=add8(x0,x1);
        const vec d01=sub8(x0,x1);
        const vec s23=add8(x2,x3);
        const vec t=mul8(sub8(x2,x3),iimag);
        vec v0,v1,v2,v3;
        transpose_4x8_to_8x4(add8(s01,s23),mul8(add8(d01,t),w1),mul8(sub8(s01,s23),w2),mul8(sub8(d01,t),w3),v0,v1,v2,v3);
        store8(b,v0);
        store8(b+8,v1);
        store8(b+16,v2);
        store8(b+24,v3);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot);
        const word rot3=mul(rot2,rot);
        inverse_butterfly(a+4*s,1,0,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

#endif

inline void forward_radix2_first(mint* EEZ_NTT754_RESTRICT a,usize n) noexcept{
    const usize half=n>>1;
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    for(;i+8<=half;i+=8){
        const vec x=load8(a+i);
        const vec y=load8(a+half+i);
        store8(a+i,add8(x,y));
        store8(a+half+i,sub8(x,y));
    }
#endif
    for(;i<half;++i){
        const word x=raw(a[i]);
        const word y=raw(a[half+i]);
        a[i]=from_raw(add(x,y));
        a[half+i]=from_raw(sub(x,y));
    }
}

inline void forward_radix4_stage(mint* EEZ_NTT754_RESTRICT a,usize n,int stage) noexcept{
    const int h=static_cast<int>(std::countr_zero(n));
    assert(stage>=0&&stage+2<=h);
    const usize stride=usize(1)<<(h-stage-2);
    const usize blocks=usize(1)<<stage;
#if defined(__AVX2__) || defined(_M_AVX2)
    if(stride>=8)forward_radix4_large(a,blocks,stride);
    else if(stride==4)forward_radix4_p4(a,blocks);
    else if(stride==1)forward_radix4_p1(a,blocks);
    else forward_radix4_scalar(a,blocks,stride);
#else
    forward_radix4_scalar(a,blocks,stride);
#endif
}

inline void inverse_radix4_stage(mint* EEZ_NTT754_RESTRICT a,usize n,int stage) noexcept{
    const int h=static_cast<int>(std::countr_zero(n));
    assert(stage>=0&&stage+2<=h);
    const usize stride=usize(1)<<(h-stage-2);
    const usize blocks=usize(1)<<stage;
#if defined(__AVX2__) || defined(_M_AVX2)
    if(stride>=8)inverse_radix4_large(a,blocks,stride);
    else if(stride==4)inverse_radix4_p4(a,blocks);
    else if(stride==1)inverse_radix4_p1(a,blocks);
    else inverse_radix4_scalar(a,blocks,stride);
#else
    inverse_radix4_scalar(a,blocks,stride);
#endif
}

inline void final_radix2_scale(mint* EEZ_NTT754_RESTRICT a,usize n,word scale_mont) noexcept{
    const usize half=n>>1;
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    const vec scale=broadcast(scale_mont);
    for(;i+8<=half;i+=8){
        const vec x=load8(a+i);
        const vec y=load8(a+half+i);
        store8(a+i,mul8(add8(x,y),scale));
        store8(a+half+i,mul8(sub8(x,y),scale));
    }
#endif
    for(;i<half;++i){
        const word x=raw(a[i]);
        const word y=raw(a[half+i]);
        a[i]=from_raw(mul(add(x,y),scale_mont));
        a[half+i]=from_raw(mul(sub(x,y),scale_mont));
    }
}

inline void final_radix4_scale(mint* EEZ_NTT754_RESTRICT a,usize n,word scale_mont) noexcept{
    const usize stride=n>>2;
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    const vec iimag=broadcast(twiddles.iroot[2]);
    const vec scale=broadcast(scale_mont);
    for(;i+8<=stride;i+=8){
        const vec x0=load8(a+i);
        const vec x1=load8(a+stride+i);
        const vec x2=load8(a+2*stride+i);
        const vec x3=load8(a+3*stride+i);
        const vec s01=add8(x0,x1);
        const vec d01=sub8(x0,x1);
        const vec s23=add8(x2,x3);
        const vec t=mul8(sub8(x2,x3),iimag);
        store8(a+i,mul8(add8(s01,s23),scale));
        store8(a+stride+i,mul8(add8(d01,t),scale));
        store8(a+2*stride+i,mul8(sub8(s01,s23),scale));
        store8(a+3*stride+i,mul8(sub8(d01,t),scale));
    }
#endif
    for(;i<stride;++i){
        const word x0=raw(a[i]);
        const word x1=raw(a[stride+i]);
        const word x2=raw(a[2*stride+i]);
        const word x3=raw(a[3*stride+i]);
        const word s01=add(x0,x1);
        const word d01=sub(x0,x1);
        const word s23=add(x2,x3);
        const word t=mul(sub(x2,x3),twiddles.iroot[2]);
        a[i]=from_raw(mul(add(s01,s23),scale_mont));
        a[stride+i]=from_raw(mul(add(d01,t),scale_mont));
        a[2*stride+i]=from_raw(mul(sub(s01,s23),scale_mont));
        a[3*stride+i]=from_raw(mul(sub(d01,t),scale_mont));
    }
}

inline void forward_dif(mint* EEZ_NTT754_RESTRICT a,usize n) noexcept{
    if(n<=1)return;
    const int h=static_cast<int>(std::countr_zero(n));
    int stage=0;
    if(h&1){
        forward_radix2_first(a,n);
        stage=1;
    }
    for(;stage<h;stage+=2)forward_radix4_stage(a,n,stage);
}

inline void inverse_dit(mint* EEZ_NTT754_RESTRICT a,usize n) noexcept{
    if(n<=1)return;
    const int h=static_cast<int>(std::countr_zero(n));
    const word scale=mint::raw(static_cast<u32>(n)).inv().a;
    if(h&1){
        for(int stage=h-2;stage>=1;stage-=2)inverse_radix4_stage(a,n,stage);
        final_radix2_scale(a,n,scale);
    }else{
        for(int stage=h-2;stage>=2;stage-=2)inverse_radix4_stage(a,n,stage);
        final_radix4_scale(a,n,scale);
    }
}

#if defined(__AVX2__) || defined(_M_AVX2)

class aligned_uninitialized_buffer{
    static_assert(std::is_trivially_destructible_v<mint>);
    mint* data_=nullptr;
public:
    explicit aligned_uninitialized_buffer(usize n)
        :data_(static_cast<mint*>(::operator new[](n*sizeof(mint),std::align_val_t{64}))){}
    aligned_uninitialized_buffer(const aligned_uninitialized_buffer&)=delete;
    aligned_uninitialized_buffer& operator=(const aligned_uninitialized_buffer&)=delete;
    ~aligned_uninitialized_buffer(){
        ::operator delete[](data_,std::align_val_t{64});
    }
    mint* data() noexcept{return data_;}
    const mint* data()const noexcept{return data_;}
};

EEZ_NTT754_ALWAYS_INLINE vec load8_aligned(const mint* p) noexcept{
    return _mm256_load_si256(reinterpret_cast<const __m256i*>(static_cast<const void*>(p)));
}

EEZ_NTT754_ALWAYS_INLINE void store8_aligned(mint* p,vec x) noexcept{
    _mm256_store_si256(reinterpret_cast<__m256i*>(static_cast<void*>(p)),x);
}

EEZ_NTT754_ALWAYS_INLINE vec shrink4_to_2(vec x) noexcept{
    return _mm256_min_epu32(x,_mm256_sub_epi32(x,broadcast(mod2)));
}

EEZ_NTT754_ALWAYS_INLINE vec lazy_add8(vec a,vec b) noexcept{return _mm256_add_epi32(a,b);}

EEZ_NTT754_ALWAYS_INLINE vec lazy_sub8(vec a,vec b) noexcept{
    return _mm256_add_epi32(a,_mm256_sub_epi32(broadcast(mod2),b));
}

template<bool trivial_twiddle,bool convert_input=false>
inline void forward_radix4_block_lazy(mint* b,usize stride,word r1) noexcept{
    const word imag=canonicalize(twiddles.root[2]);
    const vec vimag=broadcast(imag);
    const vec vimag_ninv=broadcast(imag*montgomery_ninv);
    const vec vr1=broadcast(r1);
    const vec vr1_ninv=broadcast(r1*montgomery_ninv);
    const word r2=canonicalize(mul(r1,r1));
    const vec vr2=broadcast(r2);
    const vec vr2_ninv=broadcast(r2*montgomery_ninv);
    const word r3=canonicalize(mul(r2,r1));
    const vec vr3=broadcast(r3);
    const vec vr3_ninv=broadcast(r3*montgomery_ninv);

    for(usize i=0;i<stride;i+=8){
        vec x0=shrink4_to_2(load8_aligned(b+i));
        vec x1=load8_aligned(b+stride+i);
        vec x2=load8_aligned(b+2*stride+i);
        vec x3=load8_aligned(b+3*stride+i);
        if constexpr(!trivial_twiddle){
            x1=mul8_fixed(x1,vr1,vr1_ninv);
            x2=mul8_fixed(x2,vr2,vr2_ninv);
            x3=mul8_fixed(x3,vr3,vr3_ninv);
        }else{
            x1=shrink4_to_2(x1);
            x2=shrink4_to_2(x2);
            x3=shrink4_to_2(x3);
        }
        vec s02=lazy_add8(x0,x2);
        vec d02=lazy_sub8(x0,x2);
        vec s13=lazy_add8(x1,x3);
        const vec t=mul8_fixed(lazy_sub8(x1,x3),vimag,vimag_ninv);
        s02=shrink4_to_2(s02);
        d02=shrink4_to_2(d02);
        s13=shrink4_to_2(s13);
        vec y0=lazy_add8(s02,s13);
        vec y1=lazy_sub8(s02,s13);
        vec y2=lazy_add8(d02,t);
        vec y3=lazy_sub8(d02,t);
        if constexpr(convert_input){
            constexpr word r2c=749009521u;
            const vec vr=broadcast(r2c);
            const vec vn=broadcast(r2c*montgomery_ninv);
            y0=mul8_fixed(y0,vr,vn);
            y1=mul8_fixed(y1,vr,vn);
            y2=mul8_fixed(y2,vr,vn);
            y3=mul8_fixed(y3,vr,vn);
        }
        store8_aligned(b+i,y0);
        store8_aligned(b+stride+i,y1);
        store8_aligned(b+2*stride+i,y2);
        store8_aligned(b+3*stride+i,y3);
    }
}

template<bool trivial_twiddle,bool convert_input=false>
EEZ_NTT754_ALWAYS_INLINE void forward_radix4_block_pair_lazy(mint* EEZ_NTT754_RESTRICT a,mint* EEZ_NTT754_RESTRICT b,usize stride,word r1) noexcept{
    forward_radix4_block_lazy<trivial_twiddle,convert_input>(a,stride,r1);
    forward_radix4_block_lazy<trivial_twiddle,convert_input>(b,stride,r1);
}

template<bool trivial_twiddle,bool apply_scale,bool convert_output=false,bool direct_output=false,bool shrink_output=false>
inline void inverse_radix4_block_lazy(mint* b,usize stride,word r1,word scale) noexcept{
    const word iimag=canonicalize(twiddles.iroot[2]);
    const vec viimag=broadcast(iimag);
    const vec viimag_ninv=broadcast(iimag*montgomery_ninv);
    const vec vr1=broadcast(r1);
    const vec vr1_ninv=broadcast(r1*montgomery_ninv);
    const word r2=canonicalize(mul(r1,r1));
    const vec vr2=broadcast(r2);
    const vec vr2_ninv=broadcast(r2*montgomery_ninv);
    const word r3=canonicalize(mul(r2,r1));
    const vec vr3=broadcast(r3);
    const vec vr3_ninv=broadcast(r3*montgomery_ninv);
    const word sc=canonicalize(scale);

    for(usize i=0;i<stride;i+=8){
        const vec x0=shrink4_to_2(load8_aligned(b+i));
        const vec x1=shrink4_to_2(load8_aligned(b+stride+i));
        const vec x2=shrink4_to_2(load8_aligned(b+2*stride+i));
        const vec x3=shrink4_to_2(load8_aligned(b+3*stride+i));
        vec s01=lazy_add8(x0,x1);
        vec d01=lazy_sub8(x0,x1);
        vec s23=lazy_add8(x2,x3);
        const vec t=mul8_fixed(lazy_sub8(x2,x3),viimag,viimag_ninv);
        s01=shrink4_to_2(s01);
        d01=shrink4_to_2(d01);
        s23=shrink4_to_2(s23);
        vec y0=lazy_add8(s01,s23);
        vec y1=lazy_add8(d01,t);
        vec y2=lazy_sub8(s01,s23);
        vec y3=lazy_sub8(d01,t);
        if constexpr(apply_scale){
            const word s0=sc;
            const word s1=trivial_twiddle?s0:canonicalize(mul(s0,r1));
            const word s2=trivial_twiddle?s0:canonicalize(mul(s0,r2));
            const word s3=trivial_twiddle?s0:canonicalize(mul(s0,r3));
            y0=mul8_fixed(y0,broadcast(s0),broadcast(s0*montgomery_ninv));
            y1=mul8_fixed(y1,broadcast(s1),broadcast(s1*montgomery_ninv));
            y2=mul8_fixed(y2,broadcast(s2),broadcast(s2*montgomery_ninv));
            y3=mul8_fixed(y3,broadcast(s3),broadcast(s3*montgomery_ninv));
        }else if constexpr(!trivial_twiddle){
            y1=mul8_fixed(y1,vr1,vr1_ninv);
            y2=mul8_fixed(y2,vr2,vr2_ninv);
            y3=mul8_fixed(y3,vr3,vr3_ninv);
        }
        if constexpr(convert_output){
            const vec one=broadcast(1);
            const vec ninv=broadcast(montgomery_ninv);
            y0=canonicalize8(mul8_fixed(y0,one,ninv));
            y1=canonicalize8(mul8_fixed(y1,one,ninv));
            y2=canonicalize8(mul8_fixed(y2,one,ninv));
            y3=canonicalize8(mul8_fixed(y3,one,ninv));
        }else if constexpr(direct_output){
            y0=canonicalize8(shrink4_to_2(y0));
            y1=canonicalize8(shrink4_to_2(y1));
            y2=canonicalize8(shrink4_to_2(y2));
            y3=canonicalize8(shrink4_to_2(y3));
        }else if constexpr(shrink_output && !apply_scale){
            // Only the final stage must expose Montgomery values in [0, 2p).

            // apply_scale already reduces each product to this range.

            y0=shrink4_to_2(y0);
            y1=shrink4_to_2(y1);
            y2=shrink4_to_2(y2);
            y3=shrink4_to_2(y3);
        }
        store8_aligned(b+i,y0);
        store8_aligned(b+stride+i,y1);
        store8_aligned(b+2*stride+i,y2);
        store8_aligned(b+3*stride+i,y3);
    }
}

inline unsigned adaptive_leaf_log(usize n) noexcept{
    const unsigned h=static_cast<unsigned>(std::countr_zero(n));
    return(h&1u)?3u:4u;
}

template<bool convert_input=false>
EEZ_NTT754_ALWAYS_INLINE void forward_cache_node(mint* EEZ_NTT754_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    const usize stride=block_size>>2;
    if constexpr(convert_input)forward_radix4_block_lazy<true,true>(base,stride,montgomery_one);
    else if(block==0)forward_radix4_block_lazy<true>(base,stride,montgomery_one);
    else forward_radix4_block_lazy<false>(base,stride,rotation[layer]);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));
}

template<bool convert_input=false>
inline void forward_cache_block(mint* EEZ_NTT754_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    forward_cache_node<convert_input>(base,block_size,layer,block,blocks_at_layer,rotation);
    const usize child_size=block_size>>2;
    for(usize child=0;child<4;++child){
        mint* const p=base+child*child_size;
        const usize cb=block*4+child;
        forward_cache_node(p,child_size,layer+1,cb,blocks_at_layer*4,rotation);
        const usize gsize=child_size>>2;
        for(usize g=0;g<4;++g)forward_cache_node(p+g*gsize,gsize,layer+2,cb*4+g,blocks_at_layer*16,rotation);
    }
}

template<bool convert_input=false>
inline void forward_cache_dfs(mint* EEZ_NTT754_RESTRICT base,usize block_size,usize leaf_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    if(block_size==leaf_size*64){
        forward_cache_block<convert_input>(base,block_size,layer,block,blocks_at_layer,rotation);
        return;
    }
    const usize stride=block_size>>2;
    if constexpr(convert_input)forward_radix4_block_lazy<true,true>(base,stride,montgomery_one);
    else if(block==0)forward_radix4_block_lazy<true>(base,stride,montgomery_one);
    else forward_radix4_block_lazy<false>(base,stride,rotation[layer]);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));
    const usize child_size=block_size>>2;
    if(child_size==leaf_size)return;
    for(usize child=0;child<4;++child)forward_cache_dfs<false>(base+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,rotation);
}

template<bool convert_input=false>
EEZ_NTT754_ALWAYS_INLINE void forward_cache_pair_node(mint* EEZ_NTT754_RESTRICT a,mint* EEZ_NTT754_RESTRICT b,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    const usize stride=block_size>>2;
    if constexpr(convert_input)forward_radix4_block_pair_lazy<true,true>(a,b,stride,montgomery_one);
    else if(block==0)forward_radix4_block_pair_lazy<true>(a,b,stride,montgomery_one);
    else forward_radix4_block_pair_lazy<false>(a,b,stride,rotation[layer]);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));
}

template<bool convert_input=false>
inline void forward_cache_pair_block(mint* EEZ_NTT754_RESTRICT a,mint* EEZ_NTT754_RESTRICT b,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    forward_cache_pair_node<convert_input>(a,b,block_size,layer,block,blocks_at_layer,rotation);
    const usize child_size=block_size>>2;
    for(usize child=0;child<4;++child){
        mint* const pa=a+child*child_size;
        mint* const pb=b+child*child_size;
        const usize cb=block*4+child;
        forward_cache_pair_node(pa,pb,child_size,layer+1,cb,blocks_at_layer*4,rotation);
        const usize gsize=child_size>>2;
        for(usize g=0;g<4;++g)forward_cache_pair_node(pa+g*gsize,pb+g*gsize,gsize,layer+2,cb*4+g,blocks_at_layer*16,rotation);
    }
}

template<bool convert_input=false>
inline void forward_cache_pair_dfs(mint* EEZ_NTT754_RESTRICT a,mint* EEZ_NTT754_RESTRICT b,usize block_size,usize leaf_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{
    if(block_size==leaf_size*64){
        forward_cache_pair_block<convert_input>(a,b,block_size,layer,block,blocks_at_layer,rotation);
        return;
    }
    forward_cache_pair_node<convert_input>(a,b,block_size,layer,block,blocks_at_layer,rotation);
    const usize child_size=block_size>>2;
    if(child_size==leaf_size)return;
    for(usize child=0;child<4;++child)forward_cache_pair_dfs<false>(a+child*child_size,b+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,rotation);
}

template<bool apply_scale,bool convert_output=false,bool direct_output=false,bool shrink_output=false>
EEZ_NTT754_ALWAYS_INLINE void inverse_cache_node(mint* EEZ_NTT754_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>& rotation) noexcept{
    const usize stride=block_size>>2;
    if(block==0)inverse_radix4_block_lazy<true,apply_scale,convert_output,direct_output,shrink_output>(base,stride,montgomery_one,scale);
    else inverse_radix4_block_lazy<false,apply_scale,convert_output,direct_output,shrink_output>(base,stride,rotation[layer],scale);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],inverse_rate3(twiddle_index(static_cast<u32>(block)))));
}

template<bool scale_leaf,bool convert_output=false,bool direct_output=false,bool shrink_output=false>
inline void inverse_cache_block(mint* EEZ_NTT754_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>& rotation) noexcept{
    const usize child_size=block_size>>2;
    const usize gsize=child_size>>2;
    for(usize child=0;child<4;++child){
        mint* const p=base+child*child_size;
        const usize cb=block*4+child;
        for(usize g=0;g<4;++g)inverse_cache_node<scale_leaf>(p+g*gsize,gsize,layer+2,cb*4+g,blocks_at_layer*16,scale,rotation);
        inverse_cache_node<false>(p,child_size,layer+1,cb,blocks_at_layer*4,scale,rotation);
    }
    inverse_cache_node<false,convert_output,direct_output,shrink_output>(base,block_size,layer,block,blocks_at_layer,scale,rotation);
}

template<bool scale_leaf,bool convert_output=false,bool direct_output=false,bool shrink_output=false>
inline void inverse_cache_dfs(mint* EEZ_NTT754_RESTRICT base,usize block_size,usize leaf_size,unsigned layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>& rotation) noexcept{
    if(block_size==leaf_size*64){
        inverse_cache_block<scale_leaf,convert_output,direct_output,shrink_output>(base,block_size,layer,block,blocks_at_layer,scale,rotation);
        return;
    }
    const usize child_size=block_size>>2;
    if(child_size!=leaf_size){
        for(usize child=0;child<4;++child)inverse_cache_dfs<scale_leaf,false>(base+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,scale,rotation);
    }
    const usize stride=block_size>>2;
    if constexpr(scale_leaf){
        if(child_size==leaf_size){
            if(block==0)inverse_radix4_block_lazy<true,true,convert_output,direct_output,shrink_output>(base,stride,montgomery_one,scale);
            else inverse_radix4_block_lazy<false,true,convert_output,direct_output,shrink_output>(base,stride,rotation[layer],scale);
        }else if(block==0)inverse_radix4_block_lazy<true,false,convert_output,direct_output,shrink_output>(base,stride,montgomery_one,scale);
        else inverse_radix4_block_lazy<false,false,convert_output,direct_output,shrink_output>(base,stride,rotation[layer],scale);
    }else if(block==0)inverse_radix4_block_lazy<true,false,convert_output,direct_output,shrink_output>(base,stride,montgomery_one,scale);
    else inverse_radix4_block_lazy<false,false,convert_output,direct_output,shrink_output>(base,stride,rotation[layer],scale);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],inverse_rate3(twiddle_index(static_cast<u32>(block)))));
}

EEZ_NTT754_ALWAYS_INLINE void convert_to_montgomery(mint* a,usize n) noexcept{
    constexpr word r2=749009521u;
    const vec vr2=broadcast(r2);
    const vec vn=broadcast(r2*montgomery_ninv);
    for(usize i=0;i<n;i+=8)store8_aligned(a+i,mul8_fixed(load8_aligned(a+i),vr2,vn));
}

EEZ_NTT754_ALWAYS_INLINE void convert_from_montgomery(mint* a,usize n) noexcept{
    const vec one=broadcast(1);
    const vec ninv=broadcast(montgomery_ninv);
    for(usize i=0;i<n;i+=8)store8_aligned(a+i,canonicalize8(mul8_fixed(load8_aligned(a+i),one,ninv)));
}

template<bool convert_input=false>
inline void forward_adaptive(mint* a,usize n,unsigned leaf_log) noexcept{
    const usize leaf_size=usize(1)<<leaf_log;
    if(n==leaf_size){
        if constexpr(convert_input)convert_to_montgomery(a,n);
        return;
    }
    std::array<word,max_log/2+1> rotation{};
    rotation.fill(canonicalize(montgomery_one));
    if((std::countr_zero(n)-leaf_log)&1u){
        forward_radix2_first(a,n);
        if constexpr(convert_input)convert_to_montgomery(a,n);
        forward_cache_dfs(a,n>>1,leaf_size,0,0,2,rotation);
        forward_cache_dfs(a+(n>>1),n>>1,leaf_size,0,1,2,rotation);
        return;
    }
    forward_cache_dfs<convert_input>(a,n,leaf_size,0,0,1,rotation);
}

template<bool convert_input=false>
inline void forward_adaptive_pair(mint* EEZ_NTT754_RESTRICT a,mint* EEZ_NTT754_RESTRICT b,usize n,unsigned leaf_log) noexcept{
    const usize leaf_size=usize(1)<<leaf_log;
    if(n==leaf_size){
        if constexpr(convert_input){
            convert_to_montgomery(a,n);
            convert_to_montgomery(b,n);
        }
        return;
    }
    std::array<word,max_log/2+1> rotation{};
    rotation.fill(canonicalize(montgomery_one));
    if((std::countr_zero(n)-leaf_log)&1u){
        forward_radix2_first(a,n);
        forward_radix2_first(b,n);
        if constexpr(convert_input){
            convert_to_montgomery(a,n);
            convert_to_montgomery(b,n);
        }
        forward_cache_pair_dfs(a,b,n>>1,leaf_size,0,0,2,rotation);
        forward_cache_pair_dfs(a+(n>>1),b+(n>>1),n>>1,leaf_size,0,1,2,rotation);
        return;
    }
    forward_cache_pair_dfs<convert_input>(a,b,n,leaf_size,0,0,1,rotation);
}

template<bool convert_output=false,bool direct_output=false>
inline void inverse_adaptive(mint* a,usize n,unsigned leaf_log) noexcept{
    const usize leaf_size=usize(1)<<leaf_log;
    if(n==leaf_size){
        if constexpr(convert_output)convert_from_montgomery(a,n);
        return;
    }
    const word scale=mint::raw(static_cast<word>(n>>leaf_log)).inv().a;
    std::array<word,max_log/2+1> rotation{};
    rotation.fill(canonicalize(montgomery_one));
    if((std::countr_zero(n)-leaf_log)&1u){
        inverse_cache_dfs<true>(a,n>>1,leaf_size,0,0,2,scale,rotation);
        inverse_cache_dfs<true>(a+(n>>1),n>>1,leaf_size,0,1,2,scale,rotation);
        const usize half=n>>1;
        for(usize i=0;i<half;i+=8){
            const vec x=shrink4_to_2(load8_aligned(a+i));
            const vec y=shrink4_to_2(load8_aligned(a+half+i));
            vec z0=lazy_add8(x,y);
            vec z1=lazy_sub8(x,y);
            if constexpr(convert_output){
                const vec one=broadcast(1);
                const vec ninv=broadcast(montgomery_ninv);
                z0=canonicalize8(mul8_fixed(z0,one,ninv));
                z1=canonicalize8(mul8_fixed(z1,one,ninv));
            }else if constexpr(direct_output){
                z0=canonicalize8(shrink4_to_2(z0));
                z1=canonicalize8(shrink4_to_2(z1));
            }else{
                z0=shrink4_to_2(z0);
                z1=shrink4_to_2(z1);
            }
            store8_aligned(a+i,z0);
            store8_aligned(a+half+i,z1);
        }
        return;
    }
    inverse_cache_dfs<true,convert_output,direct_output,true>(a,n,leaf_size,0,0,1,scale,rotation);
}

EEZ_NTT754_ALWAYS_INLINE __m128i reduce_four_accumulators(vec x) noexcept{
    const vec ninv=broadcast(montgomery_ninv);
    const vec prime=broadcast(mod);
    const vec q=_mm256_mul_epu32(x,ninv);
    const vec sum=_mm256_add_epi64(x,_mm256_mul_epu32(q,prime));
    const vec high=_mm256_bsrli_epi128(sum,4);
    const vec packed=_mm256_permutevar8x32_epi32(high,_mm256_setr_epi32(0,2,4,6,0,0,0,0));
    return _mm256_castsi256_si128(packed);
}

EEZ_NTT754_ALWAYS_INLINE vec reduce_eight_accumulators(vec even,vec odd) noexcept{
    const vec ninv=broadcast(montgomery_ninv);
    const vec prime=broadcast(mod);
    const vec qe=_mm256_mul_epu32(even,ninv);
    const vec qo=_mm256_mul_epu32(odd,ninv);
    const vec re=_mm256_add_epi64(even,_mm256_mul_epu32(qe,prime));
    const vec ro=_mm256_add_epi64(odd,_mm256_mul_epu32(qo,prime));
    return shrink4_to_2(_mm256_or_si256(_mm256_bsrli_epi128(re,4),ro));
}

EEZ_NTT754_ALWAYS_INLINE void leaf_copyfree8x4(mint* EEZ_NTT754_RESTRICT a,usize first_block,const std::array<word,4>& modulus) noexcept{
    alignas(64) word lhs[4][16];
    alignas(64) word rhs[4][8];
    alignas(64) vec even[4]{};
    alignas(64) vec odd[4]{};
    for(unsigned k=0;k<4;++k){
        const usize off=(first_block+k)*8;
        const vec x=canonicalize8(shrink4_to_2(load8_aligned(a+off)));
        const vec y=canonicalize8(mul8_fixed(x,broadcast(749009521u),broadcast(749009521u*montgomery_ninv)));
        const word w=canonicalize(modulus[k]);
        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]),mul8_fixed(x,broadcast(w),broadcast(w*montgomery_ninv)));
        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]+8),x);
        _mm256_store_si256(reinterpret_cast<vec*>(rhs[k]),y);
    }
    for(unsigned i=0;i<8;++i){
        for(unsigned k=0;k<4;++k){
            const usize off=(first_block+k)*8;
            const vec y=broadcast(rhs[k][i]);
            const vec x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[k]+8-i));
            even[k]=_mm256_add_epi64(even[k],_mm256_mul_epu32(y,x));
            odd[k]=_mm256_add_epi64(odd[k],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));
        }
    }
    for(unsigned k=0;k<4;++k)store8_aligned(a+(first_block+k)*8,reduce_eight_accumulators(even[k],odd[k]));
}
EEZ_NTT754_ALWAYS_INLINE void leaf_copyfree16x2(mint* EEZ_NTT754_RESTRICT a,usize first_block,const std::array<word,2>& modulus) noexcept{
    const vec split=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    alignas(64) word lhs[6][16];
    alignas(64) word rhs[6][8];
    alignas(64) vec even[6]{};
    alignas(64) vec odd[6]{};
    word w[2];
    for(unsigned k=0;k<2;++k){
        const usize off=(first_block+k)*16;
        const vec ax0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off))),split);
        const vec ax1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off+8))),split);
        const vec bx0=canonicalize8(mul8_fixed(ax0,broadcast(749009521u),broadcast(749009521u*montgomery_ninv)));
        const vec bx1=canonicalize8(mul8_fixed(ax1,broadcast(749009521u),broadcast(749009521u*montgomery_ninv)));
        const vec ae=_mm256_permute2x128_si256(ax0,ax1,0x20);
        const vec ao=_mm256_permute2x128_si256(ax0,ax1,0x31);
        const vec be=_mm256_permute2x128_si256(bx0,bx1,0x20);
        const vec bo=_mm256_permute2x128_si256(bx0,bx1,0x31);
        const vec lx[3]{ae,ao,canonicalize8(add8(ae,ao))};
        const vec ry[3]{be,bo,canonicalize8(add8(be,bo))};
        w[k]=canonicalize(modulus[k]);
        const vec vw=broadcast(w[k]);
        const vec vn=broadcast(w[k]*montgomery_ninv);
        for(unsigned j=0;j<3;++j){
            const unsigned p=k*3+j;
            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]),mul8_fixed(lx[j],vw,vn));
            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]+8),lx[j]);
            _mm256_store_si256(reinterpret_cast<vec*>(rhs[p]),ry[j]);
        }
    }
    for(unsigned i=0;i<8;++i){
        for(unsigned p=0;p<6;++p){
            const vec y=broadcast(rhs[p][i]);
            const vec x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[p]+8-i));
            even[p]=_mm256_add_epi64(even[p],_mm256_mul_epu32(y,x));
            odd[p]=_mm256_add_epi64(odd[p],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));
        }
    }
    for(unsigned k=0;k<2;++k){
        const vec p0=reduce_eight_accumulators(even[k*3],odd[k*3]);
        const vec p1=reduce_eight_accumulators(even[k*3+1],odd[k*3+1]);
        const vec p2=reduce_eight_accumulators(even[k*3+2],odd[k*3+2]);
        vec yp1=_mm256_permutevar8x32_epi32(p1,_mm256_setr_epi32(7,0,1,2,3,4,5,6));
        yp1=_mm256_insert_epi32(yp1,canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1,7)),w[k])),0);
        const vec ce=add8(p0,yp1);
        const vec co=sub8(sub8(p2,p0),p1);
        const vec lo=_mm256_unpacklo_epi32(ce,co);
        const vec hi=_mm256_unpackhi_epi32(ce,co);
        const usize off=(first_block+k)*16;
        store8_aligned(a+off,_mm256_permute2x128_si256(lo,hi,0x20));
        store8_aligned(a+off+8,_mm256_permute2x128_si256(lo,hi,0x31));
    }
}
template<unsigned leaf_size,unsigned parallel_blocks>
inline void leaf_copyfree(mint* a,usize n) noexcept{
    const usize blocks=n/leaf_size;
    word w=montgomery_one;
    for(usize s=0;s<blocks;s+=parallel_blocks){
        std::array<word,parallel_blocks> modulus{};
        for(unsigned k=0;k<parallel_blocks;++k){
            modulus[k]=w;
            const usize block=s+k;
            if(block+1<blocks)w=mul(w,forward_rate1(twiddle_index(static_cast<u32>(block))));
        }
        if constexpr(leaf_size==8)leaf_copyfree8x4(a,s,modulus);
        else leaf_copyfree16x2(a,s,modulus);
    }
}
EEZ_NTT754_ALWAYS_INLINE void leaf_product8x4(mint* EEZ_NTT754_RESTRICT a,mint* EEZ_NTT754_RESTRICT b,usize first_block,const std::array<word,4>& modulus) noexcept{
    alignas(64) word lhs[4][16];
    alignas(64) vec even[4]{};
    alignas(64) vec odd[4]{};
    for(unsigned k=0;k<4;++k){
        const usize off=(first_block+k)*8;
        const vec x=canonicalize8(shrink4_to_2(load8_aligned(a+off)));
        const vec y=canonicalize8(shrink4_to_2(load8_aligned(b+off)));
        const word w=canonicalize(modulus[k]);
        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]),mul8_fixed(x,broadcast(w),broadcast(w*montgomery_ninv)));
        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]+8),x);
        store8_aligned(b+off,y);
    }
    for(unsigned i=0;i<8;++i){
        for(unsigned k=0;k<4;++k){
            const usize off=(first_block+k)*8;
            const vec y=broadcast(raw(b[off+i]));
            const vec x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[k]+8-i));
            even[k]=_mm256_add_epi64(even[k],_mm256_mul_epu32(y,x));
            odd[k]=_mm256_add_epi64(odd[k],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));
        }
    }
    for(unsigned k=0;k<4;++k)store8_aligned(a+(first_block+k)*8,reduce_eight_accumulators(even[k],odd[k]));
}

EEZ_NTT754_ALWAYS_INLINE void leaf_product16x2_karatsuba(mint* EEZ_NTT754_RESTRICT a,const mint* EEZ_NTT754_RESTRICT b,usize first_block,const std::array<word,2>& modulus) noexcept{
    const vec split=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    alignas(64) word lhs[6][16];
    alignas(64) word rhs[6][8];
    alignas(64) vec even[6]{};
    alignas(64) vec odd[6]{};
    word w[2];
    for(unsigned k=0;k<2;++k){
        const usize off=(first_block+k)*16;
        const vec ax0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off))),split);
        const vec ax1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off+8))),split);
        const vec bx0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(b+off))),split);
        const vec bx1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(b+off+8))),split);
        const vec ae=_mm256_permute2x128_si256(ax0,ax1,0x20);
        const vec ao=_mm256_permute2x128_si256(ax0,ax1,0x31);
        const vec be=_mm256_permute2x128_si256(bx0,bx1,0x20);
        const vec bo=_mm256_permute2x128_si256(bx0,bx1,0x31);
        const vec lx[3]{ae,ao,canonicalize8(add8(ae,ao))};
        const vec ry[3]{be,bo,canonicalize8(add8(be,bo))};
        w[k]=canonicalize(modulus[k]);
        const vec vw=broadcast(w[k]);
        const vec vn=broadcast(w[k]*montgomery_ninv);
        for(unsigned j=0;j<3;++j){
            const unsigned p=k*3+j;
            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]),mul8_fixed(lx[j],vw,vn));
            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]+8),lx[j]);
            _mm256_store_si256(reinterpret_cast<vec*>(rhs[p]),ry[j]);
        }
    }
    for(unsigned i=0;i<8;++i){
        for(unsigned p=0;p<6;++p){
            const vec y=broadcast(rhs[p][i]);
            const vec x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[p]+8-i));
            even[p]=_mm256_add_epi64(even[p],_mm256_mul_epu32(y,x));
            odd[p]=_mm256_add_epi64(odd[p],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));
        }
    }
    for(unsigned k=0;k<2;++k){
        const vec p0=reduce_eight_accumulators(even[k*3],odd[k*3]);
        const vec p1=reduce_eight_accumulators(even[k*3+1],odd[k*3+1]);
        const vec p2=reduce_eight_accumulators(even[k*3+2],odd[k*3+2]);
        vec yp1=_mm256_permutevar8x32_epi32(p1,_mm256_setr_epi32(7,0,1,2,3,4,5,6));
        yp1=_mm256_insert_epi32(yp1,canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1,7)),w[k])),0);
        const vec ce=add8(p0,yp1);
        const vec co=sub8(sub8(p2,p0),p1);
        const vec lo=_mm256_unpacklo_epi32(ce,co);
        const vec hi=_mm256_unpackhi_epi32(ce,co);
        const usize off=(first_block+k)*16;
        store8_aligned(a+off,_mm256_permute2x128_si256(lo,hi,0x20));
        store8_aligned(a+off+8,_mm256_permute2x128_si256(lo,hi,0x31));
    }
}

EEZ_NTT754_ALWAYS_INLINE word twice(word x) noexcept{return x+x;}

EEZ_NTT754_ALWAYS_INLINE vec pack4_u32(word x0,word x1,word x2,word x3) noexcept{
    return _mm256_cvtepu32_epi64(_mm_setr_epi32(static_cast<int>(x0),static_cast<int>(x1),static_cast<int>(x2),static_cast<int>(x3)));
}

EEZ_NTT754_ALWAYS_INLINE vec mul4_u32(word a0,word b0,word a1,word b1,word a2,word b2,word a3,word b3) noexcept{
    return _mm256_mul_epu32(pack4_u32(a0,a1,a2,a3),pack4_u32(b0,b1,b2,b3));
}

EEZ_NTT754_ALWAYS_INLINE u64 hsum4_u64(vec x) noexcept{
    __m128i s=_mm_add_epi64(_mm256_castsi256_si128(x),_mm256_extracti128_si256(x,1));
    s=_mm_add_epi64(s,_mm_srli_si128(s,8));
    return static_cast<u64>(_mm_cvtsi128_si64(s));
}

EEZ_NTT754_ALWAYS_INLINE vec square8_packed(vec vx,word w) noexcept{
    alignas(32) word x[8],xw[8];
    vx=canonicalize8(shrink4_to_2(vx));
    w=canonicalize(w);
    _mm256_store_si256(reinterpret_cast<vec*>(x),vx);
    _mm256_store_si256(reinterpret_cast<vec*>(xw),mul8_fixed(vx,broadcast(w),broadcast(w*montgomery_ninv)));

    u64 a0=hsum4_u64(mul4_u32(x[0],x[0],twice(xw[1]),x[7],twice(xw[2]),x[6],twice(xw[3]),x[5]));
    const u64 a1=hsum4_u64(mul4_u32(twice(x[0]),x[1],twice(xw[2]),x[7],twice(xw[3]),x[6],twice(xw[4]),x[5]));
    u64 a2=hsum4_u64(mul4_u32(twice(x[0]),x[2],x[1],x[1],twice(xw[3]),x[7],twice(xw[4]),x[6]));
    const u64 a3=hsum4_u64(mul4_u32(twice(x[0]),x[3],twice(x[1]),x[2],twice(xw[4]),x[7],twice(xw[5]),x[6]));
    u64 a4=hsum4_u64(mul4_u32(twice(x[0]),x[4],twice(x[1]),x[3],x[2],x[2],twice(xw[5]),x[7]));
    const u64 a5=hsum4_u64(mul4_u32(twice(x[0]),x[5],twice(x[1]),x[4],twice(x[2]),x[3],twice(xw[6]),x[7]));
    u64 a6=hsum4_u64(mul4_u32(twice(x[0]),x[6],twice(x[1]),x[5],twice(x[2]),x[4],x[3],x[3]));
    const u64 a7=hsum4_u64(mul4_u32(twice(x[0]),x[7],twice(x[1]),x[6],twice(x[2]),x[5],twice(x[3]),x[4]));

    const vec extra=mul4_u32(xw[4],x[4],xw[5],x[5],xw[6],x[6],xw[7],x[7]);
    alignas(32) u64 e[4];
    _mm256_store_si256(reinterpret_cast<vec*>(e),extra);
    a0+=e[0];
    a2+=e[1];
    a4+=e[2];
    a6+=e[3];

    const vec lo=_mm256_setr_epi64x(static_cast<long long>(a0),static_cast<long long>(a1),static_cast<long long>(a2),static_cast<long long>(a3));
    const vec hi=_mm256_setr_epi64x(static_cast<long long>(a4),static_cast<long long>(a5),static_cast<long long>(a6),static_cast<long long>(a7));
    return shrink4_to_2(_mm256_set_m128i(reduce_four_accumulators(hi),reduce_four_accumulators(lo)));
}

EEZ_NTT754_ALWAYS_INLINE void leaf_square8x4(mint* EEZ_NTT754_RESTRICT a,usize first_block,const std::array<word,4>& modulus) noexcept{
    for(unsigned k=0;k<4;++k){
        const usize off=(first_block+k)*8;
        store8_aligned(a+off,square8_packed(load8_aligned(a+off),modulus[k]));
    }
}

EEZ_NTT754_ALWAYS_INLINE void leaf_square16x2_karatsuba(mint* EEZ_NTT754_RESTRICT a,usize first_block,const std::array<word,2>& modulus) noexcept{
    const vec split=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    for(unsigned k=0;k<2;++k){
        const usize off=(first_block+k)*16;
        const vec x0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off))),split);
        const vec x1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off+8))),split);
        const vec xe=_mm256_permute2x128_si256(x0,x1,0x20);
        const vec xo=_mm256_permute2x128_si256(x0,x1,0x31);
        const vec xs=canonicalize8(add8(xe,xo));
        const word w=canonicalize(modulus[k]);
        const vec p0=square8_packed(xe,w);
        const vec p1=square8_packed(xo,w);
        const vec p2=square8_packed(xs,w);
        vec yp1=_mm256_permutevar8x32_epi32(p1,_mm256_setr_epi32(7,0,1,2,3,4,5,6));
        yp1=_mm256_insert_epi32(yp1,canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1,7)),w)),0);
        const vec ce=add8(p0,yp1);
        const vec co=sub8(sub8(p2,p0),p1);
        const vec lo=_mm256_unpacklo_epi32(ce,co);
        const vec hi=_mm256_unpackhi_epi32(ce,co);
        store8_aligned(a+off,_mm256_permute2x128_si256(lo,hi,0x20));
        store8_aligned(a+off+8,_mm256_permute2x128_si256(lo,hi,0x31));
    }
}

template<unsigned leaf_size,unsigned parallel_blocks>
inline void leaf_products(mint* a,mint* b,usize n) noexcept{
    const usize blocks=n/leaf_size;
    word w=montgomery_one;
    for(usize s=0;s<blocks;s+=parallel_blocks){
        std::array<word,parallel_blocks> modulus{};
        for(unsigned k=0;k<parallel_blocks;++k){
            modulus[k]=w;
            const usize block=s+k;
            if(block+1<blocks)w=mul(w,forward_rate1(twiddle_index(static_cast<u32>(block))));
        }
        if constexpr(leaf_size==8)leaf_product8x4(a,b,s,modulus);
        else leaf_product16x2_karatsuba(a,b,s,modulus);
    }
}

template<unsigned leaf_size,unsigned parallel_blocks>
inline void leaf_squares(mint* a,usize n) noexcept{
    const usize blocks=n/leaf_size;
    word w=montgomery_one;
    for(usize s=0;s<blocks;s+=parallel_blocks){
        std::array<word,parallel_blocks> modulus{};
        for(unsigned k=0;k<parallel_blocks;++k){
            modulus[k]=w;
            const usize block=s+k;
            if(block+1<blocks)w=mul(w,forward_rate1(twiddle_index(static_cast<u32>(block))));
        }
        if constexpr(leaf_size==8)leaf_square8x4(a,s,modulus);
        else leaf_square16x2_karatsuba(a,s,modulus);
    }
}

inline void convolution_adaptive_inplace(mint* a,mint* b,usize n) noexcept{
    const unsigned leaf_log=adaptive_leaf_log(n);
    forward_adaptive_pair(a,b,n,leaf_log);
    if(leaf_log==3)leaf_products<8,4>(a,b,n);
    else leaf_products<16,2>(a,b,n);
    inverse_adaptive(a,n,leaf_log);
}

inline void square_adaptive_inplace(mint* a,usize n) noexcept{
    const unsigned leaf_log=adaptive_leaf_log(n);
    forward_adaptive(a,n,leaf_log);
    if(leaf_log==3)leaf_squares<8,4>(a,n);
    else leaf_squares<16,2>(a,n);
    inverse_adaptive(a,n,leaf_log);
}

inline void convolution_adaptive_normal_inplace(mint* a,mint* b,usize n) noexcept{
    const unsigned leaf_log=adaptive_leaf_log(n);
    forward_adaptive_pair<true>(a,b,n,leaf_log);
    if(leaf_log==3)leaf_products<8,4>(a,b,n);
    else leaf_products<16,2>(a,b,n);
    inverse_adaptive<true>(a,n,leaf_log);
}

inline void convolution_adaptive_mixed_normal_inplace(mint* a,mint* b,usize n) noexcept{
    const unsigned leaf_log=adaptive_leaf_log(n);
    forward_adaptive_pair(a,b,n,leaf_log);
    if(leaf_log==3)leaf_products<8,4>(a,b,n);
    else leaf_products<16,2>(a,b,n);
    inverse_adaptive<false,true>(a,n,leaf_log);
}

#endif

inline void pointwise_multiply(mint* EEZ_NTT754_RESTRICT a,const mint* EEZ_NTT754_RESTRICT b,usize n) noexcept{
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    for(;i+8<=n;i+=8)store8(a+i,mul8(load8(a+i),load8(b+i)));
#endif
    for(;i<n;++i)a[i]=from_raw(mul(raw(a[i]),raw(b[i])));
}

inline void pointwise_square(mint* EEZ_NTT754_RESTRICT a,usize n) noexcept{
    usize i=0;
#if defined(__AVX2__) || defined(_M_AVX2)
    for(;i+8<=n;i+=8){
        const vec x=load8(a+i);
        store8(a+i,mul8(x,x));
    }
#endif
    for(;i<n;++i)a[i]=from_raw(mul(raw(a[i]),raw(a[i])));
}

}

inline void forward(std::span<mint> a) noexcept{
    if(a.size()<=1)return;
    assert(valid_ntt_size(a.size()));
    detail::forward_dif(a.data(),a.size());
}

inline void inverse(std::span<mint> a) noexcept{
    if(a.size()<=1)return;
    assert(valid_ntt_size(a.size()));
    detail::inverse_dit(a.data(),a.size());
}

namespace detail{

inline std::vector<mint> convolution_naive(std::span<const mint> a,std::span<const mint> b){
    std::vector<mint> result(convolution_size(a.size(),b.size()));
    for(usize i=0;i<a.size();++i)
        for(usize j=0;j<b.size();++j)
            result[i+j]+=a[i]*b[j];
    return result;
}

inline std::vector<mint> square_naive(std::span<const mint> a){
    std::vector<mint> result(convolution_size(a.size(),a.size()));
    for(usize i=0;i<a.size();++i){
        result[2*i]+=a[i]*a[i];
        for(usize j=i+1;j<a.size();++j){
            const mint p=a[i]*a[j];
            result[i+j]+=p+p;
        }
    }
    return result;
}

inline usize checked_transform_size(usize n,usize m){
    const usize result=convolution_transform_size(n,m);
    if(n&&m&&!result)throw std::length_error("eez::ntt754: convolution exceeds the 2^25 transform limit");
    return result;
}

inline void require_ntt_size(usize n){
    if(!valid_ntt_size(n))throw std::invalid_argument("eez::ntt754: transform length must be a power of two in [1, 2^23]");
}

}

#if defined(__AVX2__) || defined(_M_AVX2)

using convolution_buffer=detail::aligned_vector;

inline void convolution_inplace(convolution_buffer& a,convolution_buffer& b){
    if(a.size()!=b.size()||!valid_convolution_transform_size(a.size()))
        throw std::invalid_argument("eez::ntt754::convolution_inplace: buffer sizes must match and be a power of two in [32, 2^25]");
    detail::convolution_adaptive_inplace(a.data(),b.data(),a.size());
}

#endif

inline std::vector<mint> convolution(std::span<const mint> a,std::span<const mint> b){
    if(a.empty()||b.empty())return {};
    if(a.data()==b.data()&&a.size()==b.size())return square(a);
    if(std::min(a.size(),b.size())<=naive_cutoff)return detail::convolution_naive(a,b);

    const usize result_size=convolution_size(a.size(),b.size());
    const usize n=detail::checked_transform_size(a.size(),b.size());
    detail::aligned_vector fa(n),fb(n);
    std::copy(a.begin(),a.end(),fa.begin());
    std::copy(b.begin(),b.end(),fb.begin());
    detail::convolution_adaptive_inplace(fa.data(),fb.data(),n);

    std::vector<mint> result(result_size);
    std::copy_n(fa.data(),result_size,result.data());
    return result;
}

inline std::vector<mint> square(std::span<const mint> a){
    if(a.empty())return {};
    if(a.size()<=naive_cutoff)return detail::square_naive(a);

    const usize result_size=convolution_size(a.size(),a.size());
    const usize n=detail::checked_transform_size(a.size(),a.size());
    detail::aligned_vector fa(n);
    std::copy(a.begin(),a.end(),fa.begin());
    detail::square_adaptive_inplace(fa.data(),n);

    std::vector<mint> result(result_size);
    std::copy_n(fa.data(),result_size,result.data());
    return result;
}

inline void convolution_to(std::span<const mint> a,std::span<const mint> b,std::span<mint> out,workspace& ws){
    if(a.empty()||b.empty())return;
    const usize result_size=convolution_size(a.size(),b.size());
    if(out.size()<result_size)throw std::invalid_argument("eez::ntt754::convolution_to: output span is too small");

    if(a.data()==b.data()&&a.size()==b.size()){
        square_to(a,out,ws);
        return;
    }

    if(std::min(a.size(),b.size())<=naive_cutoff){
        std::fill_n(out.begin(),result_size,mint{});
        for(usize i=0;i<a.size();++i)
            for(usize j=0;j<b.size();++j)
                out[i+j]+=a[i]*b[j];
        return;
    }

    const usize n=detail::checked_transform_size(a.size(),b.size());
    ws.reserve(n);
    std::fill_n(ws.a_.begin(),n,mint{});
    std::fill_n(ws.b_.begin(),n,mint{});
    std::copy(a.begin(),a.end(),ws.a_.begin());
    std::copy(b.begin(),b.end(),ws.b_.begin());
    detail::convolution_adaptive_inplace(ws.a_.data(),ws.b_.data(),n);
    std::copy_n(ws.a_.begin(),result_size,out.begin());
}

inline void square_to(std::span<const mint> a,std::span<mint> out,workspace& ws){
    if(a.empty())return;
    const usize result_size=convolution_size(a.size(),a.size());
    if(out.size()<result_size)throw std::invalid_argument("eez::ntt754::square_to: output span is too small");

    if(a.size()<=naive_cutoff){
        std::fill_n(out.begin(),result_size,mint{});
        for(usize i=0;i<a.size();++i){
            out[2*i]+=a[i]*a[i];
            for(usize j=i+1;j<a.size();++j){
                const mint p=a[i]*a[j];
                out[i+j]+=p+p;
            }
        }
        return;
    }

    const usize n=detail::checked_transform_size(a.size(),a.size());
    ws.reserve(n);
    std::fill_n(ws.a_.begin(),n,mint{});
    std::copy(a.begin(),a.end(),ws.a_.begin());
    detail::square_adaptive_inplace(ws.a_.data(),n);
    std::copy_n(ws.a_.begin(),result_size,out.begin());
}

inline void forward_to(std::span<const mint> src,frequency_buffer& dst,usize n){
    detail::require_ntt_size(n);
    if(src.size()>n)throw std::invalid_argument("eez::ntt754::forward_to: source is longer than transform");
    dst.data_.assign(n,mint{});
    std::copy(src.begin(),src.end(),dst.data_.begin());
    detail::forward_dif(dst.data_.data(),n);
}

inline void pointwise_multiply(frequency_buffer& lhs,const frequency_buffer& rhs){
    if(lhs.size()!=rhs.size())throw std::invalid_argument("eez::ntt754::pointwise_multiply: transform sizes differ");
    detail::pointwise_multiply(lhs.data_.data(),rhs.data_.data(),lhs.data_.size());
}

inline void pointwise_square(frequency_buffer& a){
    detail::pointwise_square(a.data_.data(),a.data_.size());
}

inline void inverse_to(frequency_buffer& src,std::span<mint> out){
    if(src.data_.empty()){
        if(!out.empty())throw std::invalid_argument("eez::ntt754::inverse_to: empty transform");
        return;
    }
    if(out.size()>src.data_.size())throw std::invalid_argument("eez::ntt754::inverse_to: output is longer than transform");
    detail::inverse_dit(src.data_.data(),src.data_.size());
    std::copy_n(src.data_.begin(),out.size(),out.begin());
}

}

#undef EEZ_NTT754_ALWAYS_INLINE
#undef EEZ_NTT754_RESTRICT


// Public streaming API preserving submission #393594's mixed-normal path.

// read(): next coefficient in [0, mod); called for n values, then m values.

// write(u32): receives n+m-1 canonical coefficients, or none for an empty input.

// This API never exposes normal-representation data as ordinary modint values.

namespace eez::ntt754{
template<class Reader,class Writer>
inline void convolution_normal_io(usize n,usize m,Reader&& read,Writer&& write){
    if(std::min(n,m)<=naive_cutoff){
        std::vector<mint> a(n),b(m);
        for(auto& x:a)x=read();
        for(auto& x:b)x=read();
        const auto c=convolution(a,b);
        for(const auto& x:c)write(x.get());
        return;
    }
    const usize z=detail::checked_transform_size(n,m);
    const usize result_size=n+m-1;
    detail::aligned_uninitialized_buffer a(z),b(z);
    mint* const first=n<m?b.data():a.data();
    mint* const second=n<m?a.data():b.data();
    for(usize i=0;i<n;++i)
        std::construct_at(first+i,mint::montgomery_raw(read()));
    for(usize i=0;i<m;++i)
        std::construct_at(second+i,mint::montgomery_raw(read()));
    const usize a_size=std::max(n,m),b_size=std::min(n,m);
    std::uninitialized_value_construct_n(a.data()+a_size,z-a_size);
    std::uninitialized_value_construct_n(b.data()+b_size,z-b_size);
    detail::convert_to_montgomery(b.data(),(b_size+7)&~usize(7));
    detail::convolution_adaptive_mixed_normal_inplace(a.data(),b.data(),z);
    for(usize i=0;i<result_size;++i)write(a.data()[i].a);
}
}


namespace eez::conv1000000007 {
// Public coefficients are uint32_t, reduced modulo 1,000,000,007.
// All arithmetic is exact; no floating-point FFT or approximate CRT is used.
using u32=std::uint32_t;using u64=std::uint64_t;using usize=std::size_t;
inline constexpr u32 mod=1000000007u;
inline constexpr u32 p1=897581057,p2=880803841,p3=754974721;
constexpr u32 power(u64 a,u32 n,u32 p){u64 r=1;for(;n;n>>=1,a=a*a%p)if(n&1)r=r*a%p;return r;}
inline constexpr u32 inv_p1_mod_p2=power(p1%p2,p2-2,p2),inv_p12_mod_p3=power(u64(p1)*p2%p3,p3-2,p3),p12_mod_m=u64(p1)*p2%mod;
inline constexpr usize naive_cutoff=96;
static_assert(p1<mod&&p2<mod&&p3<mod&&p1<2*p2&&p1<2*p3);
static_assert(__uint128_t(p1)*p2*p3>__uint128_t(usize(1)<<25)*(mod-1)*(mod-1));
using vec=__m256i;
inline vec vb(u32 x){return _mm256_set1_epi32(x);}
inline vec reduce1(vec x,u32 p){return _mm256_min_epu32(x,_mm256_sub_epi32(x,vb(p)));}
inline vec addmod(vec x,vec y,u32 p){return reduce1(_mm256_add_epi32(x,y),p);}
inline vec submod(vec x,vec y,u32 p){return reduce1(_mm256_sub_epi32(_mm256_add_epi32(x,vb(p)),y),p);}
template<u32 K,u32 P> inline vec fixed_mul(vec x){
 // Shoup constant multiplication: q=floor(x*floor(K*2^32/P)/2^32).
 // For every uint32_t x, 0 <= x*K-q*P < 2*P. With P<2^30,
 // the low-word subtraction is exact modulo 2^32 and one correction suffices.
 constexpr u32 reciprocal=(u64(K)<<32)/P;
 vec q0=_mm256_srli_epi64(_mm256_mul_epu32(x,vb(reciprocal)),32);
 vec q1=_mm256_mul_epu32(_mm256_srli_epi64(x,32),vb(reciprocal));
 vec q=_mm256_blend_epi32(q0,q1,0xaa);
 vec r=_mm256_sub_epi32(_mm256_mullo_epi32(x,vb(K)),_mm256_mullo_epi32(q,vb(P)));
 return reduce1(r,P);
}
inline vec norm(vec x){x=reduce1(x,4*mod);x=reduce1(x,2*mod);return reduce1(x,mod);}
inline u32 norm(u32 x){return x>=mod?x%mod:x;}
struct arena {
 void* ptr; explicit arena(usize words):ptr(::operator new[](words*4,std::align_val_t{64})){}
 ~arena(){::operator delete[](ptr,std::align_val_t{64});}
 arena(const arena&)=delete;arena& operator=(const arena&)=delete;
};
// Begin each prime's object lifetimes explicitly in the reused raw allocation.
template<class M,u32 P> inline void fill(M* dst,std::span<const u32> src,usize z,bool mont){
 usize i=0;for(;i+8<=src.size();i+=8){
  vec x=norm(_mm256_loadu_si256(reinterpret_cast<const vec*>(src.data()+i)));
  if constexpr(1)for(unsigned k=0;k<(mod-1)/P;++k)x=reduce1(x,P);
  if(mont)x=fixed_mul<u32((u64(1)<<32)%P),P>(x);
  alignas(32) u32 lanes[8];_mm256_store_si256(reinterpret_cast<vec*>(lanes),x);
  for(unsigned j=0;j<8;++j)std::construct_at(dst+i+j,M::montgomery_raw(lanes[j]));
 }
 for(;i<src.size();++i){u32 x=norm(src[i]);if constexpr(1)x=x>=P?x%P:x;std::construct_at(dst+i,mont?M::raw(x):M::montgomery_raw(x));}
 for(;i<z;++i)std::construct_at(dst+i,M::montgomery_raw(0));
}

__attribute__((noinline)) inline void blocked897(std::span<const u32>a,std::span<const u32>b,void*wa){
 using M=modint897;using namespace eez::ntt897::detail;
 const usize z=std::bit_ceil(b.size()*4),step=z-b.size()+1,sz=a.size()+b.size()-1;
 arena tmp(2*z);M*x=static_cast<M*>(tmp.ptr),*y=x+z,*out=static_cast<M*>(wa);
 fill<M,p1>(y,b,z,true);auto l=adaptive_leaf_log(z);forward_adaptive(y,z,l);
 for(usize i=0;i<sz;++i)std::construct_at(out+i,M::montgomery_raw(0));
 for(usize start=0;start<a.size();start+=step){auto part=a.subspan(start,std::min(step,a.size()-start));fill<M,p1>(x,part,z,false);forward_adaptive(x,z,l);
  if(l==3)leaf_products<8,4>(x,y,z);else leaf_products<16,2>(x,y,z);inverse_adaptive<false,true>(x,z,l);
  usize count=part.size()+b.size()-1,i=0;for(;i+8<=count;i+=8){auto u=_mm256_loadu_si256(reinterpret_cast<const __m256i*>(out+start+i)),v=_mm256_load_si256(reinterpret_cast<const __m256i*>(x+i));auto s=_mm256_add_epi32(u,v);s=_mm256_min_epu32(s,_mm256_sub_epi32(s,_mm256_set1_epi32(p1)));_mm256_storeu_si256(reinterpret_cast<__m256i*>(out+start+i),s);}
  for(;i<count;++i){u32 s=out[start+i].a+x[i].a;out[start+i].a=s>=p1?s-p1:s;}
 }
}

__attribute__((noinline)) inline void blocked880(std::span<const u32>a,std::span<const u32>b,void*wa){
 using M=modint880;using namespace eez::ntt880::detail;
 const usize z=std::bit_ceil(b.size()*4),step=z-b.size()+1,sz=a.size()+b.size()-1;
 arena tmp(2*z);M*x=static_cast<M*>(tmp.ptr),*y=x+z,*out=static_cast<M*>(wa);
 fill<M,p2>(y,b,z,true);auto l=adaptive_leaf_log(z);forward_adaptive(y,z,l);
 for(usize i=0;i<sz;++i)std::construct_at(out+i,M::montgomery_raw(0));
 for(usize start=0;start<a.size();start+=step){auto part=a.subspan(start,std::min(step,a.size()-start));fill<M,p2>(x,part,z,false);forward_adaptive(x,z,l);
  if(l==3)leaf_products<8,4>(x,y,z);else leaf_products<16,2>(x,y,z);inverse_adaptive<false,true>(x,z,l);
  usize count=part.size()+b.size()-1,i=0;for(;i+8<=count;i+=8){auto u=_mm256_loadu_si256(reinterpret_cast<const __m256i*>(out+start+i)),v=_mm256_load_si256(reinterpret_cast<const __m256i*>(x+i));auto s=_mm256_add_epi32(u,v);s=_mm256_min_epu32(s,_mm256_sub_epi32(s,_mm256_set1_epi32(p2)));_mm256_storeu_si256(reinterpret_cast<__m256i*>(out+start+i),s);}
  for(;i<count;++i){u32 s=out[start+i].a+x[i].a;out[start+i].a=s>=p2?s-p2:s;}
 }
}

__attribute__((noinline)) inline void blocked754(std::span<const u32>a,std::span<const u32>b,void*wa){
 using M=modint754;using namespace eez::ntt754::detail;
 const usize z=std::bit_ceil(b.size()*4),step=z-b.size()+1,sz=a.size()+b.size()-1;
 arena tmp(2*z);M*x=static_cast<M*>(tmp.ptr),*y=x+z,*out=static_cast<M*>(wa);
 fill<M,p3>(y,b,z,true);auto l=adaptive_leaf_log(z);forward_adaptive(y,z,l);
 for(usize i=0;i<sz;++i)std::construct_at(out+i,M::montgomery_raw(0));
 for(usize start=0;start<a.size();start+=step){auto part=a.subspan(start,std::min(step,a.size()-start));fill<M,p3>(x,part,z,false);forward_adaptive(x,z,l);
  if(l==3)leaf_products<8,4>(x,y,z);else leaf_products<16,2>(x,y,z);inverse_adaptive<false,true>(x,z,l);
  usize count=part.size()+b.size()-1,i=0;for(;i+8<=count;i+=8){auto u=_mm256_loadu_si256(reinterpret_cast<const __m256i*>(out+start+i)),v=_mm256_load_si256(reinterpret_cast<const __m256i*>(x+i));auto s=_mm256_add_epi32(u,v);s=_mm256_min_epu32(s,_mm256_sub_epi32(s,_mm256_set1_epi32(p3)));_mm256_storeu_si256(reinterpret_cast<__m256i*>(out+start+i),s);}
  for(;i<count;++i){u32 s=out[start+i].a+x[i].a;out[start+i].a=s>=p3?s-p3:s;}
 }
}
inline void engine1(std::span<const u32>a,std::span<const u32>b,usize z,void*wa,void*wb,bool sq){
 if(!sq&&a.size()>=8*b.size()&&b.size()>=128){blocked897(a,b,wa);return;}
 using M=modint897;using namespace eez::ntt897::detail;
 M* x=static_cast<M*>(wa),*y=static_cast<M*>(wb);fill<M,p1>(x,a,z,false);
 if(sq){auto l=adaptive_leaf_log(z);forward_adaptive(x,z,l);if(z>=(usize(1)<<19)){if(l==3)leaf_copyfree<8,4>(x,z);else leaf_copyfree<16,2>(x,z);}else{for(usize i=0;i<z;++i)std::construct_at(y+i,M::montgomery_raw(x[i].a));convert_to_montgomery(y,z);if(l==3)leaf_products<8,4>(x,y,z);else leaf_products<16,2>(x,y,z);}inverse_adaptive<false,true>(x,z,l);return;}
 fill<M,p1>(y,b,z,true);
 convolution_adaptive_mixed_normal_inplace(x,y,z);}

inline void engine2(std::span<const u32>a,std::span<const u32>b,usize z,void*wa,void*wb,bool sq){
 if(!sq&&a.size()>=8*b.size()&&b.size()>=128){blocked880(a,b,wa);return;}
 using M=modint880;using namespace eez::ntt880::detail;
 M* x=static_cast<M*>(wa),*y=static_cast<M*>(wb);fill<M,p2>(x,a,z,false);
 if(sq){auto l=adaptive_leaf_log(z);forward_adaptive(x,z,l);if(z>=(usize(1)<<19)){if(l==3)leaf_copyfree<8,4>(x,z);else leaf_copyfree<16,2>(x,z);}else{for(usize i=0;i<z;++i)std::construct_at(y+i,M::montgomery_raw(x[i].a));convert_to_montgomery(y,z);if(l==3)leaf_products<8,4>(x,y,z);else leaf_products<16,2>(x,y,z);}inverse_adaptive<false,true>(x,z,l);return;}
 fill<M,p2>(y,b,z,true);
 convolution_adaptive_mixed_normal_inplace(x,y,z);}

inline void engine3(std::span<const u32>a,std::span<const u32>b,usize z,void*wa,void*wb,bool sq){
 if(!sq&&a.size()>=8*b.size()&&b.size()>=128){blocked754(a,b,wa);return;}
 using M=modint754;using namespace eez::ntt754::detail;
 M* x=static_cast<M*>(wa),*y=static_cast<M*>(wb);fill<M,p3>(x,a,z,false);
 if(sq){auto l=adaptive_leaf_log(z);forward_adaptive(x,z,l);if(z>=(usize(1)<<19)){if(l==3)leaf_copyfree<8,4>(x,z);else leaf_copyfree<16,2>(x,z);}else{for(usize i=0;i<z;++i)std::construct_at(y+i,M::montgomery_raw(x[i].a));convert_to_montgomery(y,z);if(l==3)leaf_products<8,4>(x,y,z);else leaf_products<16,2>(x,y,z);}inverse_adaptive<false,true>(x,z,l);return;}
 fill<M,p3>(y,b,z,true);
 convolution_adaptive_mixed_normal_inplace(x,y,z);}

inline std::vector<u32> small_scalar(std::span<const u32>a,std::span<const u32>b){
 // At most 16 products plus a reduced carry: strictly below UINT64_MAX.
 std::vector<u32>x(a.size()),y(b.size()),out(a.size()+b.size()-1);for(usize i=0;i<a.size();++i)x[i]=norm(a[i]);for(usize i=0;i<b.size();++i)y[i]=norm(b[i]);
 for(usize k=0;k<out.size();++k){usize lo=k>=y.size()?k-y.size()+1:0,hi=std::min(k+1,x.size());u64 sum=0;while(lo<hi){usize end=std::min(lo+16,hi);for(;lo<end;++lo)sum+=u64(x[lo])*y[k-lo];sum%=mod;}out[k]=sum;}
 return out;
}inline std::vector<u32> small_simd(std::span<const u32>a,std::span<const u32>b){
 if(a.size()>b.size())std::swap(a,b);const usize n=a.size(),m=b.size(),sz=n+m-1;
 std::vector<int32_t> x(n),y(m+2*n+8);std::vector<u32> out(sz);
 for(usize i=0;i<n;++i){u32 v=norm(a[i]);x[i]=v>mod/2?int64_t(v)-mod:v;}
 for(usize i=0;i<m;++i){u32 v=norm(b[i]);y[n+i]=v>mod/2?int64_t(v)-mod:v;}
 for(usize k=0;k<sz;k+=8){alignas(32)int64_t sums[8]{};
  for(usize j=0;j<n;){vec even=_mm256_setzero_si256(),odd=even;usize end=std::min(j+32,n);
   for(;j<end;++j){vec aa=_mm256_set1_epi32(x[j]),bb=_mm256_loadu_si256(reinterpret_cast<const vec*>(y.data()+n+k-j));even=_mm256_add_epi64(even,_mm256_mul_epi32(aa,bb));odd=_mm256_add_epi64(odd,_mm256_mul_epi32(aa,_mm256_srli_epi64(bb,32)));}
   alignas(32)int64_t ev[4],od[4];_mm256_store_si256(reinterpret_cast<vec*>(ev),even);_mm256_store_si256(reinterpret_cast<vec*>(od),odd);for(unsigned t=0;t<4;++t){sums[2*t]+=ev[t]%mod;sums[2*t+1]+=od[t]%mod;}
  }
  for(usize t=0;t<8&&k+t<sz;++t){int64_t v=n<=32?sums[t]:sums[t]%mod;out[k+t]=u32(v<0?v+mod:v);}
 }return out;
}
inline void add_scaled(u32*,std::span<const u32>,u32);
inline std::vector<u32> small(std::span<const u32>a,std::span<const u32>b){
 if(a.size()>b.size())std::swap(a,b);
 if(a.size()==1){std::vector<u32> out(b.size());add_scaled(out.data(),b,a[0]);return out;}
 if(b.size()>=1024)return small_simd(a,b);
 return small_scalar(a,b);
}

template<class Writer>inline void compute_ntt(std::span<const u32>a,std::span<const u32>b,Writer&&write){

 // Ascending primes are local to the three-prime path; the bounded
 // one/two-prime paths retain their existing prime order and constants.
 constexpr u32 p1=754974721u,p2=880803841u,p3=897581057u;
 constexpr u32 inv_p1_mod_p2=power(p1,p2-2,p2);
 constexpr u32 inv_p12_mod_p3=power(u64(p1)*p2%p3,p3-2,p3);
 constexpr u32 inv_p2_mod_p3=power(p2,p3-2,p3);
 constexpr u32 p12_mod_m=u64(p1)*p2%mod;
 if(a.empty()||b.empty())return;
 if(a.size()> (usize(1)<<25)||b.size()>(usize(1)<<25)||a.size()+b.size()-1>(usize(1)<<25))throw std::length_error("convolution exceeds 2^25 output coefficients");
 if(std::min(a.size(),b.size())<=naive_cutoff||(std::min(a.size(),b.size())<=128&&std::max(a.size(),b.size())>=8192)){auto c=small(a,b);for(u32 x:c)write(x);return;}
 // Also recognize equal, separately stored inputs (including streaming I/O).
 // Random unequal inputs normally leave std::equal after the first element.
 const bool sq=a.size()==b.size()&&(a.data()==b.data()||std::equal(a.begin(),a.end(),b.begin()));
 if(a.size()<b.size())std::swap(a,b);
 const usize sz=a.size()+b.size()-1,z=std::bit_ceil(sz),s=(sz+15)&~usize(15);
 const bool blocks=!sq&&a.size()>=8*b.size()&&b.size()>=128;const usize work_words=blocks?s:2*z;arena mem(work_words+2*s);char* raw=static_cast<char*>(mem.ptr);void* wa=raw;void* wb=raw+4*(blocks?s:z);
 u32* r1=reinterpret_cast<u32*>(raw+4*work_words),*t2=r1+s;
 engine3(a,b,z,wa,wb,sq);
 auto* a1=static_cast<modint754*>(wa);for(usize i=0;i<sz;++i)std::construct_at(r1+i,a1[i].a);
 engine2(a,b,z,wa,wb,sq);auto* a2=static_cast<modint880*>(wa);
 usize i=0;
 if constexpr(1)for(;i+8<=sz;i+=8){vec x=_mm256_load_si256(reinterpret_cast<const vec*>(r1+i));vec y=_mm256_load_si256(reinterpret_cast<const vec*>(a2+i));vec q=fixed_mul<inv_p1_mod_p2,p2>(submod(y,x,p2));alignas(32)u32 v[8];_mm256_store_si256(reinterpret_cast<vec*>(v),q);for(int j=0;j<8;++j)std::construct_at(t2+i+j,v[j]);}
 for(;i<sz;++i){u32 x=r1[i],y=a2[i].a;std::construct_at(t2+i,u32(u64(y>=x?y-x:y+p2-x)*inv_p1_mod_p2%p2));}
 engine1(a,b,z,wa,wb,sq);auto* a3=static_cast<modint897*>(wa);i=0;
 if constexpr(1)for(;i+8<=sz;i+=8){
  vec x=_mm256_load_si256(reinterpret_cast<const vec*>(r1+i)),q=_mm256_load_si256(reinterpret_cast<const vec*>(t2+i));
  vec y=_mm256_load_si256(reinterpret_cast<const vec*>(a3+i));
  // Two independent constant multiplications replace the serial
  // q*p1 -> subtraction -> inverse(p1*p2) dependency chain.
  const vec left=fixed_mul<inv_p12_mod_p3,p3>(submod(y,x,p3));
  const vec right=fixed_mul<inv_p2_mod_p3,p3>(q);
  const vec q3=submod(left,right,p3);
  // Each term is canonical in [0,mod); their sum is below 3*mod<2^32.
  const vec sum=_mm256_add_epi32(_mm256_add_epi32(x,fixed_mul<p1,mod>(q)),fixed_mul<p12_mod_m,mod>(q3));
  const vec out=reduce1(reduce1(sum,mod),mod);
  alignas(32)u32 v[8];_mm256_store_si256(reinterpret_cast<vec*>(v),out);for(u32 c:v)write(c);
 }
 for(;i<sz;++i){
  const u32 x=r1[i],q=t2[i],y=a3[i].a;
  const u32 d=y>=x?y-x:y+p3-x;
  const u32 left=u64(d)*inv_p12_mod_p3%p3;
  const u32 right=u64(q)*inv_p2_mod_p3%p3;
  const u32 q3=left>=right?left-right:left+p3-right;
  write(u32((x+u64(p1)*q+u64(p12_mod_m)*q3)%mod));
 }
}
// Absolute centered coefficient bounds. One or two primes are exact when
// their product exceeds twice the maximum absolute integer coefficient.
template<class Writer> __attribute__((noinline)) inline bool bounded_primes(std::span<const u32>a,std::span<const u32>b,Writer&& write){
 if(a.empty()||b.empty())return true;
 u32 sample_a=0,sample_b=0;for(usize i=0;i<std::min(a.size(),usize(16));++i){u32 v=norm(a[i]);sample_a=std::max(sample_a,std::min(v,mod-v));}for(usize i=0;i<std::min(b.size(),usize(16));++i){u32 v=norm(b[i]);sample_b=std::max(sample_b,std::min(v,mod-v));}
 // A conservative dispatch heuristic only: rejection falls back to 3 primes.
 if(__uint128_t(2)*std::min(a.size(),b.size())*sample_a*sample_b>=u64(p1)*p2)return false;
 u32 ma=0,mb=0;u64 sa=0,sb=0;
 for(u32 v:a){v=norm(v);v=std::min(v,mod-v);ma=std::max(ma,v);sa+=v;}
 for(u32 v:b){v=norm(v);v=std::min(v,mod-v);mb=std::max(mb,v);sb+=v;}
 __uint128_t bound=std::min(__uint128_t(sa)*mb,__uint128_t(sb)*ma);
 if(2*bound>=u64(p1)*p2)return false;
 usize sz=a.size()+b.size()-1,z=std::bit_ceil(sz),s=(sz+15)&~usize(15);bool one=2*bound<p1;
 if(!bound){for(usize i=0;i<sz;++i)write(0u);return true;}
 std::vector<u32>aa(a.size()),bb(b.size());
 auto convert=[](std::span<const u32>v,std::vector<u32>&d,u32 p){for(usize i=0;i<v.size();++i){u32 x=norm(v[i]);d[i]=x>mod/2?p-(mod-x):x;}};
 arena mem(2*z+(one?0:s));char* raw=static_cast<char*>(mem.ptr);void*wa=raw;void*wb=raw+4*z;u32*r1=reinterpret_cast<u32*>(raw+8*z);
 convert(a,aa,p1);convert(b,bb,p1);bool sq=a.size()==b.size()&&std::equal(a.begin(),a.end(),b.begin());engine1(aa,bb,z,wa,wb,sq);auto*x=static_cast<modint897*>(wa);
 if(one){for(usize i=0;i<sz;++i){u32 v=x[i].a;write(v>p1/2?mod-(p1-v):v);}return true;}
 for(usize i=0;i<sz;++i)std::construct_at(r1+i,x[i].a);
 convert(a,aa,p2);convert(b,bb,p2);engine2(aa,bb,z,wa,wb,sq);auto*y=static_cast<modint880*>(wa);
 constexpr u64 product=u64(p1)*p2;constexpr u32 product_mod=product%mod;
 usize i=0;for(;i+8<=sz;i+=8){vec x=_mm256_load_si256(reinterpret_cast<const vec*>(r1+i)),yy=_mm256_load_si256(reinterpret_cast<const vec*>(y+i));vec t=fixed_mul<inv_p1_mod_p2,p2>(submod(yy,reduce1(x,p2),p2));
  vec even=_mm256_add_epi64(_mm256_mul_epu32(t,vb(p1)),_mm256_and_si256(x,_mm256_set1_epi64x(0xffffffffull)));
  vec odd=_mm256_add_epi64(_mm256_mul_epu32(_mm256_srli_epi64(t,32),vb(p1)),_mm256_srli_epi64(x,32));
  vec half=_mm256_set1_epi64x(product/2);vec neg=_mm256_blend_epi32(_mm256_cmpgt_epi64(even,half),_mm256_cmpgt_epi64(odd,half),0xaa);
  vec out=addmod(x,fixed_mul<p1%mod,mod>(t),mod);out=submod(out,_mm256_and_si256(neg,vb(product_mod)),mod);alignas(32)u32 lanes[8];_mm256_store_si256(reinterpret_cast<vec*>(lanes),out);for(u32 v:lanes)write(v);
 }
 for(;i<sz;++i){u32 x2=r1[i]>=p2?r1[i]-p2:r1[i];u32 d=y[i].a>=x2?y[i].a-x2:y[i].a+p2-x2;u32 t=u64(d)*inv_p1_mod_p2%p2;u64 v=r1[i]+u64(p1)*t;u32 r=v%mod;if(v>product/2)r=r>=product_mod?r-product_mod:r+mod-product_mod;write(r);}return true;
}

// Add k*b to an output span. Runtime Shoup constants are prepared once per k.
inline void add_scaled(u32* dst,std::span<const u32>b,u32 k){
 k=norm(k);if(!k)return;const u32 recip=(u64(k)<<32)/mod;vec vk=vb(k),vr=vb(recip),vp=vb(mod);usize j=0;
 for(;j+8<=b.size();j+=8){vec x=norm(_mm256_loadu_si256(reinterpret_cast<const vec*>(b.data()+j)));vec qe=_mm256_srli_epi64(_mm256_mul_epu32(x,vr),32),qo=_mm256_mul_epu32(_mm256_srli_epi64(x,32),vr);vec quotient=_mm256_blend_epi32(qe,qo,0xaa);vec r=reduce1(_mm256_sub_epi32(_mm256_mullo_epi32(x,vk),_mm256_mullo_epi32(quotient,vp)),mod);vec old=_mm256_loadu_si256(reinterpret_cast<const vec*>(dst+j));_mm256_storeu_si256(reinterpret_cast<vec*>(dst+j),addmod(old,r,mod));}
 for(;j<b.size();++j){u32 x=(u64(norm(b[j]))*k)%mod;u32 y=dst[j]+x;dst[j]=y>=mod?y-mod:y;}
}
template<class Writer>inline void compute(std::span<const u32>a,std::span<const u32>b,Writer&&write){
 if(a.empty()||b.empty())return;
 if(a.size()>(usize(1)<<25)||b.size()>(usize(1)<<25)||a.size()+b.size()-1>(usize(1)<<25))throw std::length_error("convolution exceeds 2^25 output coefficients");
 const usize sz=a.size()+b.size()-1,z=std::bit_ceil(sz);
 if(std::min(a.size(),b.size())>96){
  if(bounded_primes(a,b,write))return;
  // Near a power-of-two boundary, peel a tiny high tail instead of doubling NTT.
  usize excess=sz-z/2;
  if(z>=2048&&excess<=8){
   if(a.size()<b.size())std::swap(a,b);
   usize head=a.size()-excess;std::vector<u32> c(sz);usize at=0;
   compute_ntt(a.first(head),b,[&](u32 x){c[at++]=x;});
   for(usize i=0;i<excess;++i)add_scaled(c.data()+head+i,b,a[head+i]);
   for(u32 x:c)write(x);return;
  }
 }
 compute_ntt(a,b,std::forward<Writer>(write));
}

inline std::vector<u32> convolution(std::span<const u32>a,std::span<const u32>b){if(a.empty()||b.empty())return {};if(a.size()>(usize(1)<<25)||b.size()>(usize(1)<<25)||a.size()+b.size()-1>(usize(1)<<25))throw std::length_error("convolution exceeds 2^25 output coefficients");std::vector<u32>out(a.size()+b.size()-1);usize i=0;compute(a,b,[&](u32 x){out[i++]=x;});return out;}
inline std::vector<u32> convolution(const std::vector<u32>&a,const std::vector<u32>&b){return convolution(std::span<const u32>(a),std::span<const u32>(b));}
inline std::vector<u32> square(std::span<const u32>a){return convolution(a,a);}
// Exactly n+m reads, A then B, including when either input is empty.
// Writes canonical results in increasing coefficient order; no result vector.
template<class Reader,class Writer>inline void convolution_io(usize n,usize m,Reader&&read,Writer&&write){std::vector<u32>a(n),b(m);for(auto&x:a)x=read();for(auto&x:b)x=read();compute(a,b,std::forward<Writer>(write));}
}


#line 2 "c.cpp"

#if defined(__GNUC__) && !defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#elif defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma clang attribute push( \
    __attribute__((target("avx2,bmi,bmi2,lzcnt,popcnt,ssse3"))), \
    apply_to = function)
#endif

#line 2 "IO/fastio_unsafe.hpp"

#line 11 "IO/fastio_unsafe.hpp"

#ifdef __linux__
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#ifndef FASTIO_UNSAFE_BLOCK_LOG
#define FASTIO_UNSAFE_BLOCK_LOG 14
#endif

namespace fastio_unsafe_impl {

using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;
using i128 = __int128_t;
using u128 = __uint128_t;

constexpr auto make_right_align_masks() {
    std::array<std::array<char, 16>, 16> masks{};
    for (int digits = 0; digits < 16; ++digits) {
        for (int i = 0; i < 16; ++i) {
            masks[digits][i] = i < 16 - digits
                ? static_cast<char>(0x80)
                : static_cast<char>(i - (16 - digits));
        }
    }
    return masks;
}

constexpr auto make_powers_10() {
    std::array<u64, 17> powers{};
    powers[0] = 1;
    for (std::size_t i = 1; i < powers.size(); ++i) {
        powers[i] = powers[i - 1] * 10;
    }
    return powers;
}

constexpr auto make_pair_digits() {
    std::array<unsigned char, 1 << 14> table{};
    table.fill(255);
    for (unsigned a = 0; a < 10; ++a) {
        for (unsigned b = 0; b < 10; ++b) {
            table[('0' + a) | (('0' + b) << 8)] =
                static_cast<unsigned char>(a * 10 + b);
        }
    }
    return table;
}

alignas(16) inline constexpr auto right_align_masks = make_right_align_masks();
inline constexpr auto powers_10 = make_powers_10();
inline constexpr auto pair_digits = make_pair_digits();

struct input {
    input() {
#ifdef __linux__
        struct stat info {};
        if (::fstat(0, &info) == 0 && S_ISREG(info.st_mode) && info.st_size > 0) {
            const off_t current = ::lseek(0, 0, SEEK_CUR);
            const std::size_t file_size = static_cast<std::size_t>(info.st_size);
            const std::size_t page_size = static_cast<std::size_t>(::sysconf(_SC_PAGESIZE));
            const std::size_t rounded_size =
                (file_size + page_size - 1) / page_size * page_size;
            const std::size_t reserved_size = rounded_size + page_size;

            char* region = static_cast<char*>(::mmap(
                nullptr, reserved_size, PROT_NONE,
                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
            if (region != MAP_FAILED) {
                void* file_mapping = ::mmap(
                    region, rounded_size, PROT_READ,
                    MAP_PRIVATE | MAP_FIXED, 0, 0);
                void* zero_page = file_mapping == MAP_FAILED ? MAP_FAILED : ::mmap(
                    region + rounded_size, page_size, PROT_READ,
                    MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
                if (file_mapping != MAP_FAILED && zero_page != MAP_FAILED) {
                    const std::size_t offset = current > 0
                        ? std::min(static_cast<std::size_t>(current), file_size)
                        : 0;
                    cursor_ = region + offset;
                    return;
                }
                ::munmap(region, reserved_size);
            }
        }
#endif
        read_all_fallback();
    }

    input(const input&) = delete;
    input& operator=(const input&) = delete;

    char* cursor() const noexcept { return cursor_; }

private:
    void read_all_fallback() {
        std::size_t capacity = 1u << 20;
        std::size_t size = 0;
        char* buffer = static_cast<char*>(std::malloc(capacity + 64));
        if (buffer == nullptr) std::abort();

        for (;;) {
            if (size == capacity) {
                capacity *= 2;
                char* grown = static_cast<char*>(std::realloc(buffer, capacity + 64));
                if (grown == nullptr) std::abort();
                buffer = grown;
            }
            const std::size_t count = std::fread(
                buffer + size, 1, capacity - size, stdin);
            size += count;
            if (count == 0) break;
        }
        std::memset(buffer + size, 0, 64);
        cursor_ = buffer;
    }

    char* cursor_ = nullptr;
};

__attribute__((always_inline)) inline u64 parse_16_digits(__m128i digits) noexcept {
    const __m128i pair_weights = _mm_set1_epi16(0x010A);
    const __m128i quad_weights = _mm_set1_epi32(0x00010064);
    const __m128i oct_weights = _mm_set_epi32(1, 10000, 1, 10000);
    const __m128i pairs = _mm_maddubs_epi16(digits, pair_weights);
    const __m128i quads = _mm_madd_epi16(pairs, quad_weights);
    const __m128i products = _mm_mul_epu32(quads, oct_weights);
    const __m128i odd = _mm_srli_epi64(quads, 32);
    const __m128i octets = _mm_add_epi64(products, odd);
    const u64 high = static_cast<u64>(_mm_cvtsi128_si64(octets));
    const u64 low = static_cast<u64>(_mm_extract_epi64(octets, 1));
    return high * 100000000ULL + low;
}

__attribute__((always_inline)) inline __m128i load_digits(const char* cursor) noexcept {
    return _mm_sub_epi8(
        _mm_loadu_si128(reinterpret_cast<const __m128i*>(cursor)),
        _mm_set1_epi8('0'));
}

__attribute__((always_inline)) inline u64 parse_short_digits(
    __m128i digits, u32 mask, int& length) noexcept {
    length = __builtin_ctz(mask);
    digits = _mm_shuffle_epi8(
        digits,
        _mm_load_si128(reinterpret_cast<const __m128i*>(
            right_align_masks[static_cast<std::size_t>(length)].data())));
    return parse_16_digits(digits);
}

__attribute__((always_inline)) inline u32 read_u32(char*& cursor) noexcept {
    const __m128i digits = load_digits(cursor);
    const u32 mask = static_cast<u32>(_mm_movemask_epi8(digits));
    int length;
    const u32 value = static_cast<u32>(parse_short_digits(digits, mask, length));
    cursor += length + 1;
    return value;
}

__attribute__((always_inline)) inline u32 read_u32_lt1e9(char*& cursor) noexcept {
    const auto q0 =
        pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor + 1)];
    const auto q1 =
        pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor + 3)];
    const auto q2 =
        pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor + 5)];
    const auto q3 =
        pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor + 7)];

    if (__builtin_expect((q0 | q1 | q2 | q3) < 128, 1)) {
        u32 value = static_cast<unsigned char>(cursor[0]) - '0';
        value = value * 100 + q0;
        value = value * 100 + q1;
        value = value * 100 + q2;
        value = value * 100 + q3;
        cursor += 10;
        return value;
    }

    u32 value = static_cast<unsigned char>(*cursor++) - '0';

    for (unsigned i = 0; i < 4; ++i) {
        const auto pair =
            pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor)];
        if (pair > 99) break;
        value = value * 100 + pair;
        cursor += 2;
    }

    if (*cursor > ' ') {
        value = value * 10 + static_cast<unsigned>(*cursor++ & 15);
    }

    ++cursor;
    return value;
}

__attribute__((always_inline)) inline i32 read_i32(char*& cursor) noexcept {
    const bool negative = *cursor == '-';
    cursor += static_cast<unsigned>(negative);
    const u32 magnitude = read_u32(cursor);
    const u32 bits = negative ? u32{0} - magnitude : magnitude;
    return static_cast<i32>(bits);
}

__attribute__((always_inline)) inline u64 read_u64(char*& cursor) noexcept {
    __m128i digits = load_digits(cursor);
    const u32 mask = static_cast<u32>(_mm_movemask_epi8(digits));

    if (__builtin_expect(mask != 0, 1)) {
        int length;
        const u64 value = parse_short_digits(digits, mask, length);
        cursor += length + 1;
        return value;
    }

    u64 value = parse_16_digits(digits);
    cursor += 16;
    while (*cursor >= '0') {
        value = value * 10 + static_cast<unsigned>(*cursor & 15);
        ++cursor;
    }
    ++cursor;
    return value;
}

__attribute__((always_inline)) inline i64 read_i64(char*& cursor) noexcept {
    const bool negative = *cursor == '-';
    cursor += static_cast<unsigned>(negative);
    const u64 magnitude = read_u64(cursor);
    const u64 bits = negative ? u64{0} - magnitude : magnitude;
    return static_cast<i64>(bits);
}

__attribute__((always_inline)) inline u128 read_u128(char*& cursor) noexcept {
    __m128i digits = load_digits(cursor);
    u32 mask = static_cast<u32>(_mm_movemask_epi8(digits));

    if (__builtin_expect(mask != 0, 0)) {
        int length;
        const u128 value = parse_short_digits(digits, mask, length);
        cursor += length + 1;
        return value;
    }

    u128 value = parse_16_digits(digits);
    cursor += 16;

    digits = load_digits(cursor);
    mask = static_cast<u32>(_mm_movemask_epi8(digits));
    if (mask != 0) {
        int length;
        const u64 tail = parse_short_digits(digits, mask, length);
        cursor += length + 1;
        return value * powers_10[static_cast<std::size_t>(length)] + tail;
    }

    value = value * static_cast<u128>(10000000000000000ULL)
          + parse_16_digits(digits);
    cursor += 16;

    digits = load_digits(cursor);
    mask = static_cast<u32>(_mm_movemask_epi8(digits));
    int length;
    const u64 tail = parse_short_digits(digits, mask, length);
    cursor += length + 1;
    return value * powers_10[static_cast<std::size_t>(length)] + tail;
}

__attribute__((always_inline)) inline i128 read_i128(char*& cursor) noexcept {
    const bool negative = *cursor == '-';
    cursor += static_cast<unsigned>(negative);
    const u128 magnitude = read_u128(cursor);
    const u128 bits = negative ? u128{0} - magnitude : magnitude;
    return static_cast<i128>(bits);
}

constexpr u32 pack4(char a, char b, char c, char d) noexcept {
    return static_cast<u32>(static_cast<unsigned char>(a)) |
           (static_cast<u32>(static_cast<unsigned char>(b)) << 8) |
           (static_cast<u32>(static_cast<unsigned char>(c)) << 16) |
           (static_cast<u32>(static_cast<unsigned char>(d)) << 24);
}

constexpr auto make_padded_groups() {
    std::array<u32, 10000> table{};
    for (int value = 0; value < 10000; ++value) {
        table[static_cast<std::size_t>(value)] = pack4(
            static_cast<char>('0' + value / 1000),
            static_cast<char>('0' + value / 100 % 10),
            static_cast<char>('0' + value / 10 % 10),
            static_cast<char>('0' + value % 10));
    }
    return table;
}

inline constexpr auto padded_groups = make_padded_groups();

struct output {
    output() = default;
    output(const output&) = delete;
    output& operator=(const output&) = delete;

    char* begin() noexcept { return buffer_.data(); }
    char* end() noexcept { return buffer_.data() + buffer_.size(); }

    __attribute__((noinline)) char* flush(char* cursor) noexcept {
        std::size_t remaining = static_cast<std::size_t>(cursor - buffer_.data());
        const char* data = buffer_.data();

        if (first_flush_ && remaining != 0) {
            ++data;
            --remaining;
            first_flush_ = false;
        }

#ifdef __linux__
        while (remaining != 0) {
            const ssize_t count = ::write(1, data, remaining);
            if (count > 0) {
                data += count;
                remaining -= static_cast<std::size_t>(count);
            } else if (count < 0 && errno == EINTR) {
                continue;
            } else {
                std::abort();
            }
        }
#else
        while (remaining != 0) {
            const std::size_t count = std::fwrite(data, 1, remaining, stdout);
            if (count == 0) std::abort();
            data += count;
            remaining -= count;
        }
#endif

        return buffer_.data();
    }

    void finish(char* cursor) noexcept {
        if (cursor != buffer_.data() || !first_flush_) *cursor++ = '\n';
        (void)flush(cursor);
    }

    alignas(64) std::array<char, 1u << 19> buffer_;
    bool first_flush_ = true;
};

__attribute__((always_inline)) inline void store_group(
    char*& cursor, u32 group) noexcept {
    std::memcpy(cursor, &group, sizeof(group));
    cursor += sizeof(group);
}

__attribute__((always_inline)) inline void emit_leading(
    char*& cursor, u64 value) noexcept {
    const unsigned skip =
        3u
        - static_cast<unsigned>(value >= 10)
        - static_cast<unsigned>(value >= 100)
        - static_cast<unsigned>(value >= 1000);
    const u32 group =
        padded_groups[static_cast<std::size_t>(value)] >> (skip * 8);
    std::memcpy(cursor, &group, sizeof(group));
    cursor += 4 - skip;
}

__attribute__((always_inline)) inline void emit_padded(
    char*& cursor, u64 value) noexcept {
    store_group(cursor, padded_groups[static_cast<std::size_t>(value)]);
}

__attribute__((always_inline)) inline void emit_padded_16(
    char*& cursor, u64 value) noexcept {
    emit_padded(cursor, value / 1000000000000ULL);
    emit_padded(cursor, value / 100000000ULL % 10000);
    emit_padded(cursor, value / 10000ULL % 10000);
    emit_padded(cursor, value % 10000);
}

__attribute__((always_inline)) inline void emit_u32_unchecked(
    char*& cursor, u32 value) noexcept {
    if (value >= 100000000U) {
        emit_leading(cursor, value / 100000000U);
        emit_padded(cursor, value / 10000U % 10000);
        emit_padded(cursor, value % 10000);
    } else if (value >= 10000U) {
        emit_leading(cursor, value / 10000U);
        emit_padded(cursor, value % 10000);
    } else {
        emit_leading(cursor, value);
    }
}

__attribute__((always_inline)) inline void emit_u64_unchecked(
    char*& cursor, u64 value) noexcept {
    if (value >= 10000000000000000ULL) {
        emit_leading(cursor, value / 10000000000000000ULL);
        emit_padded_16(cursor, value % 10000000000000000ULL);
    } else if (value >= 1000000000000ULL) {
        emit_leading(cursor, value / 1000000000000ULL);
        emit_padded(cursor, value / 100000000ULL % 10000);
        emit_padded(cursor, value / 10000ULL % 10000);
        emit_padded(cursor, value % 10000);
    } else if (value >= 100000000ULL) {
        emit_leading(cursor, value / 100000000ULL);
        emit_padded(cursor, value / 10000ULL % 10000);
        emit_padded(cursor, value % 10000);
    } else if (value >= 10000ULL) {
        emit_leading(cursor, value / 10000);
        emit_padded(cursor, value % 10000);
    } else {
        emit_leading(cursor, value);
    }
}

__attribute__((always_inline)) inline void emit_u128_unchecked(
    char*& cursor, u128 value) noexcept {
    constexpr u128 base = static_cast<u128>(10000000000000000ULL);
    constexpr u128 u64_max = static_cast<u128>(~u64{0});

    if (value <= u64_max) {
        emit_u64_unchecked(cursor, static_cast<u64>(value));
        return;
    }

    const u64 low = static_cast<u64>(value % base);
    const u128 upper = value / base;

    if (upper <= u64_max) {
        emit_u64_unchecked(cursor, static_cast<u64>(upper));
        emit_padded_16(cursor, low);
        return;
    }

    const u64 middle = static_cast<u64>(upper % base);
    const u32 high = static_cast<u32>(upper / base);
    emit_u32_unchecked(cursor, high);
    emit_padded_16(cursor, middle);
    emit_padded_16(cursor, low);
}

__attribute__((always_inline)) inline void write_u32_lt1e9(
    output& sink, char*& cursor, char* end, u32 value) noexcept {
    if (__builtin_expect(end - cursor < 16, 0)) cursor = sink.flush(cursor);

    *cursor++ = ' ';

    if (value >= 100000000U) {
        const u32 high = value / 100000000U;
        *cursor++ = static_cast<char>('0' + high);
        value -= high * 100000000U;
        emit_padded(cursor, value / 10000U);
        emit_padded(cursor, value % 10000U);
    } else {
        emit_u32_unchecked(cursor, value);
    }
}

__attribute__((always_inline)) inline void write_u32(
    output& sink, char*& cursor, char* end, u32 value) noexcept {
    if (__builtin_expect(end - cursor < 16, 0)) cursor = sink.flush(cursor);
    *cursor++ = ' ';
    emit_u32_unchecked(cursor, value);
}

__attribute__((always_inline)) inline void write_i32(
    output& sink, char*& cursor, char* end, i32 value) noexcept {
    if (__builtin_expect(end - cursor < 16, 0)) cursor = sink.flush(cursor);
    const bool negative = value < 0;
    const u32 bits = static_cast<u32>(value);
    const u32 magnitude = negative ? u32{0} - bits : bits;
    *cursor++ = ' ';
    if (negative) *cursor++ = '-';
    emit_u32_unchecked(cursor, magnitude);
}

__attribute__((always_inline)) inline void write_u64(
    output& sink, char*& cursor, char* end, u64 value) noexcept {
    if (__builtin_expect(end - cursor < 24, 0)) cursor = sink.flush(cursor);
    *cursor++ = ' ';
    emit_u64_unchecked(cursor, value);
}

__attribute__((always_inline)) inline void write_i64(
    output& sink, char*& cursor, char* end, i64 value) noexcept {
    if (__builtin_expect(end - cursor < 24, 0)) cursor = sink.flush(cursor);
    const bool negative = value < 0;
    const u64 bits = static_cast<u64>(value);
    const u64 magnitude = negative ? u64{0} - bits : bits;
    *cursor++ = ' ';
    if (negative) *cursor++ = '-';
    emit_u64_unchecked(cursor, magnitude);
}

__attribute__((always_inline)) inline void write_u128(
    output& sink, char*& cursor, char* end, u128 value) noexcept {
    if (__builtin_expect(end - cursor < 48, 0)) cursor = sink.flush(cursor);
    *cursor++ = ' ';
    emit_u128_unchecked(cursor, value);
}

__attribute__((always_inline)) inline void write_i128(
    output& sink, char*& cursor, char* end, i128 value) noexcept {
    if (__builtin_expect(end - cursor < 48, 0)) cursor = sink.flush(cursor);
    const bool negative = value < 0;
    const u128 bits = static_cast<u128>(value);
    const u128 magnitude = negative ? u128{0} - bits : bits;
    *cursor++ = ' ';
    if (negative) *cursor++ = '-';
    emit_u128_unchecked(cursor, magnitude);
}

}

struct fastio_unsafe {
    using i32 = fastio_unsafe_impl::i32;
    using u32 = fastio_unsafe_impl::u32;
    using i64 = fastio_unsafe_impl::i64;
    using u64 = fastio_unsafe_impl::u64;
    using i128 = fastio_unsafe_impl::i128;
    using u128 = fastio_unsafe_impl::u128;

    fastio_unsafe() = default;
    fastio_unsafe(const fastio_unsafe&) = delete;
    fastio_unsafe& operator=(const fastio_unsafe&) = delete;

    char* input_cursor() const noexcept { return in.cursor(); }
    char* output_cursor() noexcept { return out.begin(); }
    char* output_end() noexcept { return out.end(); }
    void finish(char* cursor) noexcept { out.finish(cursor); }

    __attribute__((always_inline)) u32 read_u32(char*& cursor) noexcept {
        return fastio_unsafe_impl::read_u32(cursor);
    }

    __attribute__((always_inline)) u32 read_u32_lt1e9(char*& cursor) noexcept {
        return fastio_unsafe_impl::read_u32_lt1e9(cursor);
    }

    __attribute__((always_inline)) i32 read_i32(char*& cursor) noexcept {
        return fastio_unsafe_impl::read_i32(cursor);
    }

    __attribute__((always_inline)) u64 read_u64(char*& cursor) noexcept {
        return fastio_unsafe_impl::read_u64(cursor);
    }

    __attribute__((always_inline)) i64 read_i64(char*& cursor) noexcept {
        return fastio_unsafe_impl::read_i64(cursor);
    }

    __attribute__((always_inline)) u128 read_u128(char*& cursor) noexcept {
        return fastio_unsafe_impl::read_u128(cursor);
    }

    __attribute__((always_inline)) i128 read_i128(char*& cursor) noexcept {
        return fastio_unsafe_impl::read_i128(cursor);
    }

    __attribute__((always_inline)) void write_u32(
        char*& cursor, char* end, u32 value) noexcept {
        fastio_unsafe_impl::write_u32(out, cursor, end, value);
    }

    __attribute__((always_inline)) void write_u32_lt1e9(
        char*& cursor, char* end, u32 value) noexcept {
        fastio_unsafe_impl::write_u32_lt1e9(out, cursor, end, value);
    }

    __attribute__((always_inline)) void write_i32(
        char*& cursor, char* end, i32 value) noexcept {
        fastio_unsafe_impl::write_i32(out, cursor, end, value);
    }

    __attribute__((always_inline)) void write_u64(
        char*& cursor, char* end, u64 value) noexcept {
        fastio_unsafe_impl::write_u64(out, cursor, end, value);
    }

    __attribute__((always_inline)) void write_i64(
        char*& cursor, char* end, i64 value) noexcept {
        fastio_unsafe_impl::write_i64(out, cursor, end, value);
    }

    __attribute__((always_inline)) void write_u128(
        char*& cursor, char* end, u128 value) noexcept {
        fastio_unsafe_impl::write_u128(out, cursor, end, value);
    }

    __attribute__((always_inline)) void write_i128(
        char*& cursor, char* end, i128 value) noexcept {
        fastio_unsafe_impl::write_i128(out, cursor, end, value);
    }

    fastio_unsafe_impl::input in;
    fastio_unsafe_impl::output out;
};


int main() {
    fastio_unsafe io;
    char* input_cursor = io.input_cursor();
    char* output_cursor = io.output_cursor();
    char* const output_end = io.output_end();

    const std::size_t n = io.read_u32(input_cursor);
    const std::size_t m = io.read_u32(input_cursor);

    eez::conv1000000007::convolution_io(
        n, m,
        [&]() -> std::uint32_t {
            return io.read_u32(input_cursor);
        },
        [&](std::uint32_t value) {
            io.write_u32(output_cursor, output_end, value);
        }
    );

    io.finish(output_cursor);
    return 0;
}
