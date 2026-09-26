#include <algorithm>
#include <cstring>
#if (defined(__x86_64__) || defined(__i386__)) && (defined(__GNUC__) || defined(__clang__)) && !defined(POLY998_FORCE_SCALAR)
#include <immintrin.h>
#define POLY998_SIMD 1
#define POLY998_AVX __attribute__((target("avx2")))
#else
#define POLY998_SIMD 0
#define POLY998_AVX
#endif
#include <cstdint>
#include <initializer_list>
#include <istream>
#include <limits>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace poly998 {
using u32 = std::uint32_t;
using u64 = std::uint64_t;
inline constexpr int VERSION = 3;
inline constexpr u32 MOD = 998244353;
inline constexpr int MAX_FPS = 1 << 22;
inline constexpr int MAX_NTT = 1 << 23;

class Mint {
    u32 x_ = 0; // Lazy Montgomery residue in [0,2*MOD).
    static constexpr u32 reduce(u64 x) {
        const u32 q = static_cast<u32>(x) * 998244351u;
        return static_cast<u32>((x + static_cast<u64>(q) * MOD) >> 32);
    }
    static constexpr Mint raw(u32 x) { Mint r; r.x_ = x; return r; }
public:
    constexpr Mint() = default;
    template<class Int, std::enable_if_t<std::is_integral_v<Int>, int> = 0>
    constexpr Mint(Int x) {
        u32 y = 0;
        if constexpr (std::is_signed_v<Int>) {
            std::int64_t r = static_cast<std::int64_t>(x) % MOD;
            if (r < 0) r += MOD;
            y = static_cast<u32>(r);
        } else y = static_cast<u32>(static_cast<u64>(x) % MOD);
        x_ = reduce(static_cast<u64>(y) * 932051910u);
    }
    constexpr u32 val() const {
        const u32 y = reduce(x_);
        return y >= MOD ? y - MOD : y;
    }
    constexpr bool is_zero() const { return x_ == 0 || x_ == MOD; }
    constexpr Mint& operator+=(Mint b) {
        u32 s = x_ + b.x_; x_ = s >= 2*MOD ? s - 2*MOD : s; return *this;
    }
    constexpr Mint& operator-=(Mint b) {
        x_ = x_ >= b.x_ ? x_ - b.x_ : x_ + 2*MOD - b.x_; return *this;
    }
    constexpr Mint& operator*=(Mint b) {
        x_ = reduce(static_cast<u64>(x_) * b.x_); return *this;
    }
    Mint& operator/=(Mint b) { return *this *= b.inv(); }
    friend constexpr Mint operator+(Mint a, Mint b) { return a += b; }
    friend constexpr Mint operator-(Mint a, Mint b) { return a -= b; }
    friend constexpr Mint operator*(Mint a, Mint b) { return a *= b; }
    friend Mint operator/(Mint a, Mint b) { return a /= b; }
    constexpr Mint operator-() const { return Mint{} - *this; }
    friend constexpr bool operator==(Mint a, Mint b) {
        return (a.x_ >= MOD ? a.x_ - MOD : a.x_) ==
               (b.x_ >= MOD ? b.x_ - MOD : b.x_);
    }
    friend constexpr bool operator!=(Mint a, Mint b) { return !(a == b); }
    Mint pow(u64 k) const {
        Mint a = *this, b = 1;
        for (; k; k >>= 1, a *= a) if (k & 1) b *= a;
        return b;
    }
    Mint inv() const {
        if (is_zero()) throw std::invalid_argument("inverse of zero");
        return pow(MOD - 2);
    }
    friend std::ostream& operator<<(std::ostream& os, Mint a) { return os << a.val(); }
    friend std::istream& operator>>(std::istream& is, Mint& a) {
        std::string s;
        if (!(is >> s)) return is;
        std::size_t i = 0; bool neg = false;
        if (!s.empty() && (s[0] == '-' || s[0] == '+')) { neg = s[0]=='-'; ++i; }
        if (i == s.size()) { is.setstate(std::ios::failbit); return is; }
        u32 x = 0;
        for (; i < s.size(); ++i) {
            if (s[i] < '0' || s[i] > '9') { is.setstate(std::ios::failbit); return is; }
            x = static_cast<u32>((static_cast<u64>(x)*10 + s[i]-'0') % MOD);
        }
        a = Mint(neg && x ? MOD-x : x); return is;
    }
};
static_assert(sizeof(Mint) == sizeof(u32) && std::is_trivially_copyable_v<Mint> && std::is_standard_layout_v<Mint>);

namespace detail {
using Vec = std::vector<Mint>;
constexpr Mint add(Mint a, Mint b) { return a+b; }
constexpr Mint sub(Mint a, Mint b) { return a-b; }
constexpr Mint mul(Mint a, Mint b) { return a*b; }
constexpr Mint to_mont(u32 x) { return Mint(x); }
inline Mint power(Mint a, u64 e) { return a.pow(e); }
inline Mint inverse_scalar(Mint a) { return a.inv(); }
inline constexpr Mint ONE = Mint(1);
inline int ceil_pow2(int n) {
    if (n < 0 || n > MAX_NTT) throw std::length_error("unsupported NTT size");
    int s=1; while (s<n) s<<=1; return s;
}
inline int degree(int n, std::size_t fallback) {
    if (n == -1) {
        if (fallback > MAX_FPS) throw std::length_error("FPS size exceeds 2^22");
        n = static_cast<int>(fallback);
    }
    if (n < 0 || n > MAX_FPS) throw std::length_error("FPS size must be in [0,2^22]");
    return n;
}
// Bounded scan: at most limit+1 entries, even for a dense input.
inline std::vector<std::pair<int,Mint>> sparse_terms(const Vec& a,int n,int limit=24) {
    std::vector<std::pair<int,Mint>> terms;
    for(int i=1;i<std::min<int>(n,a.size()) && static_cast<int>(terms.size())<=limit;++i)
        if(!a[i].is_zero()) terms.emplace_back(i,a[i]);
    return terms;
}

inline bool has_avx2() {
#if POLY998_SIMD
    static const bool available=__builtin_cpu_supports("avx2");
    return available;
#else
    return false;
#endif
}
#if POLY998_SIMD
// AVX2 Montgomery arithmetic and fused radix-4 schedule adapted from the
// user-supplied 1284-line reference. Preserve this attribution when copying.
struct Simd {
    __m256i mod8,mod2_8,ir8;
    POLY998_AVX Simd():mod8(_mm256_set1_epi32(MOD)),mod2_8(_mm256_set1_epi32(2*MOD)),ir8(_mm256_set1_epi32(998244351)){}
    POLY998_AVX static __m256i load(const Mint* p) { __m256i v;std::memcpy(&v,p,32);return v; }
    POLY998_AVX static void store(Mint* p,__m256i v) { std::memcpy(p,&v,32); }
    POLY998_AVX static __m256i set(Mint x) { u32 v;std::memcpy(&v,&x,4);return _mm256_set1_epi32(static_cast<int>(v)); }
    POLY998_AVX __m256i add(__m256i a,__m256i b) const {
        auto x=_mm256_add_epi32(a,b);return _mm256_min_epu32(x,_mm256_sub_epi32(x,mod2_8));
    }
    POLY998_AVX __m256i sub(__m256i a,__m256i b) const {
        auto x=_mm256_sub_epi32(a,b);return _mm256_min_epu32(x,_mm256_add_epi32(x,mod2_8));
    }
    POLY998_AVX __m256i mul(__m256i a,__m256i b) const {
        auto x0=_mm256_mul_epu32(a,b);
        auto x1=_mm256_mul_epu32(_mm256_shuffle_epi32(a,0xf5),_mm256_shuffle_epi32(b,0xf5));
        auto y0=_mm256_mul_epu32(_mm256_mul_epu32(x0,ir8),mod8);
        auto y1=_mm256_mul_epu32(_mm256_mul_epu32(x1,ir8),mod8);
        return _mm256_blend_epi32(_mm256_shuffle_epi32(_mm256_add_epi64(x0,y0),0xf5),_mm256_add_epi64(x1,y1),0xaa);
    }
    template<int mask> POLY998_AVX __m256i mul_odd(__m256i a,__m256i b) const {
        auto x=_mm256_mul_epu32(_mm256_shuffle_epi32(a,0xf5),_mm256_shuffle_epi32(b,0xf5));
        auto y=_mm256_mul_epu32(_mm256_mul_epu32(x,ir8),mod8);
        return _mm256_blend_epi32(a,_mm256_add_epi64(x,y),mask);
    }
    template<int mask> POLY998_AVX __m256i neg(__m256i a) const {
        return _mm256_blend_epi32(a,_mm256_sub_epi32(mod2_8,a),mask);
    }
    POLY998_AVX __m256i pow(__m256i a,u32 k) const {
        auto r=set(Mint(1));for(;k;k>>=1,a=mul(a,a))if(k&1)r=mul(r,a);return r;
    }
};
POLY998_AVX inline void product_avx(Mint* out,const Mint* a,const Mint* b,int n) {
    Simd s;int i=0;for(;i+8<=n;i+=8)s.store(out+i,s.mul(s.load(a+i),s.load(b+i)));
    for(;i<n;++i)out[i]=a[i]*b[i];
}
#endif
inline void product(Mint* out,const Mint* a,const Mint* b,int n) {
#if POLY998_SIMD
    if(n>=32 && has_avx2()){product_avx(out,a,b,n);return;}
#endif
    for(int i=0;i<n;++i)out[i]=a[i]*b[i];
}


#if POLY998_SIMD
POLY998_AVX inline void product_add_avx(Mint* out,const Mint* a,const Mint* b,int n){
    Simd s;int i=0;for(;i+8<=n;i+=8)s.store(out+i,s.add(s.load(out+i),s.mul(s.load(a+i),s.load(b+i))));
    for(;i<n;++i)out[i]+=a[i]*b[i];
}
#endif
inline void product_add(Mint* out,const Mint* a,const Mint* b,int n){
#if POLY998_SIMD
    if(n>=16 && has_avx2()){product_add_avx(out,a,b,n);return;}
#endif
    for(int i=0;i<n;++i)out[i]+=a[i]*b[i];
}


#if POLY998_SIMD
// Eight independent prefix-product streams, one vector inversion per extension.
// Sequential memory access replaces integer division plus inv[p%i] gathers.
POLY998_AVX inline void inverse_table_avx(Mint* inv,int begin,int end){
    const Simd v;Mint seed[8];for(int j=0;j<8;++j)seed[j]=Mint(begin+j);
    auto values=v.load(seed),step=v.set(Mint(8)),one=v.set(Mint(1)),prefix=one;
    for(int i=begin;i<end;i+=8){prefix=v.mul(prefix,values);v.store(inv+i,prefix);values=v.add(values,step);}
    auto ip=v.pow(prefix,MOD-2);
    for(int i=end;i>begin;){i-=8;values=v.sub(values,step);
        auto prev=i==begin?one:v.load(inv+i-8);v.store(inv+i,v.mul(ip,prev));ip=v.mul(ip,values);}
}
#endif

#if POLY998_SIMD
POLY998_AVX inline void scale_avx(Mint* out,const Mint* a,Mint c,int n){
    Simd s;auto cc=s.set(c);int i=0;
    for(;i+8<=n;i+=8)s.store(out+i,s.mul(s.load(a+i),cc));for(;i<n;++i)out[i]=a[i]*c;
}
POLY998_AVX inline void index_mul_avx(Mint* out,const Mint* a,int offset,int n){
    Simd s;Mint seed[8];for(int i=0;i<8;++i)seed[i]=Mint(offset+i);auto idx=s.load(seed),step=s.set(Mint(8));int i=0;
    for(;i+8<=n;i+=8){s.store(out+i,s.mul(s.load(a+i),idx));idx=s.add(idx,step);}for(;i<n;++i)out[i]=a[i]*Mint(offset+i);
}
POLY998_AVX inline void residual_avx(Mint* out,const Mint* df,const Mint* fg,int offset,int n,Mint kp){
    Simd s;Mint seed[8];for(int i=0;i<8;++i)seed[i]=Mint(offset+i);auto idx=s.load(seed),step=s.set(Mint(8)),kk=s.set(kp);int i=0;
    for(;i+8<=n;i+=8){s.store(out+i,s.sub(s.mul(s.load(df+i),kk),s.mul(s.load(fg+i),idx)));idx=s.add(idx,step);}
    for(;i<n;++i)out[i]=kp*df[i]-Mint(offset+i)*fg[i];
}
POLY998_AVX inline void fold_avx(Mint* dst,const Mint* prev,int n){
    Simd s;int i=0;for(;i+8<=n;i+=8){s.store(dst+i,s.add(s.load(dst+i),s.load(prev+i)));s.store(dst+n+i,s.sub(s.load(dst+n+i),s.load(prev+n+i)));}
    for(;i<n;++i){dst[i]+=prev[i];dst[n+i]-=prev[n+i];}
}
#endif
inline void scale_array(Mint* out,const Mint* a,Mint c,int n){
#if POLY998_SIMD
    if(n>=32&&has_avx2()){scale_avx(out,a,c,n);return;}
#endif
    for(int i=0;i<n;++i)out[i]=a[i]*c;
}
inline void index_mul(Mint* out,const Mint* a,int offset,int n){
#if POLY998_SIMD
    if(n>=32&&has_avx2()){index_mul_avx(out,a,offset,n);return;}
#endif
    for(int i=0;i<n;++i)out[i]=a[i]*Mint(offset+i);
}
inline void power_residual(Mint* out,const Mint* df,const Mint* fg,int offset,int n,Mint kp){
#if POLY998_SIMD
    if(n>=32&&has_avx2()){residual_avx(out,df,fg,offset,n,kp);return;}
#endif
    for(int i=0;i<n;++i)out[i]=kp*df[i]-Mint(offset+i)*fg[i];
}
inline void fold_pair(Mint* dst,const Mint* prev,int n){
#if POLY998_SIMD
    if(n>=32&&has_avx2()){fold_avx(dst,prev,n);return;}
#endif
    for(int i=0;i<n;++i){dst[i]+=prev[i];dst[n+i]-=prev[n+i];}
}


#if POLY998_SIMD
// Eight independent Horner chains in x^8, combined once with 1,x,...,x^7.
POLY998_AVX inline Mint evaluate_avx(const Mint* a,int n,Mint x){
    Simd v;Mint lanes[8]{};int blocks=n/8,tail=n%8;
    for(int j=0;j<tail;++j)lanes[j]=a[blocks*8+j];
    auto acc=v.load(lanes),step=v.set(x.pow(8));
    for(int i=blocks-1;i>=0;--i)acc=v.add(v.mul(acc,step),v.load(a+8*i));
    v.store(lanes,acc);Mint ans=0,p=1;
    for(int j=0;j<8;++j){ans+=lanes[j]*p;p*=x;}return ans;
}
#endif
inline Mint evaluate(const Mint* a,int n,Mint x){
#if POLY998_SIMD
    if(n>=64 && has_avx2())return evaluate_avx(a,n,x);
#endif
    Mint ans=0;for(int i=n-1;i>=0;--i)ans=ans*x+a[i];return ans;
}

class NTT {
    Vec roots_,inverse_roots_;
    Mint inv_size_[24]{};
public:
    // Inverse transforms use the same roots, then reverse coefficient indices.
    NTT() : roots_(1) {
        inv_size_[0] = ONE;
        const Mint inv2 = Mint((MOD + 1) / 2);
        for (int i=1; i<24; ++i) inv_size_[i] = inv_size_[i-1]*inv2;
    }
    void ensure(int limit) {
        if(limit<1 || limit>MAX_NTT || (limit&(limit-1)))throw std::length_error("unsupported NTT size");
        int old=static_cast<int>(roots_.size());if(limit<=old)return;
        roots_.resize(limit);
        if(has_avx2())inverse_roots_.resize(limit);
        for(int m=old;m<limit;m<<=1){
            Mint w=Mint(3).pow((MOD-1)/(2*m)),iw;
            if(has_avx2())iw=w.inv();
            if(m==1){roots_[1]=ONE;if(has_avx2())inverse_roots_[1]=ONE;continue;}
            for(int j=0;j<m;j+=2){
                roots_[m+j]=roots_[(m+j)/2];roots_[m+j+1]=roots_[m+j]*w;
                if(has_avx2()){
                    inverse_roots_[m+j]=inverse_roots_[(m+j)/2];
                    inverse_roots_[m+j+1]=inverse_roots_[m+j]*iw;
                }
            }
        }
    }


#if POLY998_SIMD
    POLY998_AVX void forward_avx(Mint* f,std::size_t n) {
        const Mint* roots=roots_.data();
        const Simd info8;
        Mint r2[8],r4[8];std::fill_n(r2,8,ONE);std::fill_n(r4,8,ONE);
        r2[3]=r2[7]=roots[3];r4[5]=roots[5];r4[6]=roots[6];r4[7]=roots[7];

        __m256i rt2=info8.load(r2),rt4=info8.load(r4);

  std::size_t l = n / 2;
  const bool n_4b = ((__builtin_ctz(static_cast<unsigned>(n)) - 3) & 1) != 0;
  if (n_4b) {
    for (std::size_t i = 0; i < n; i += l * 2) {
      for (std::size_t j = 0; j < l; j += 8) {
        __m256i x = info8.load(f + i + j);
        __m256i y = info8.load(f + i + j + l);
        info8.store(f + i + j, info8.add(x, y));
        info8.store(f + i + j + l,
                    info8.mul(info8.sub(x, y), info8.load(roots_.data() + l + j)));
      }
    }
    l >>= 1;
  }

  for (l /= 2; l >= 8; l /= 4) {
    for (std::size_t i = 0; i < n; i += l * 2 * 2) {
      for (std::size_t j = 0; j < l; j += 8) {
        __m256i x0 = info8.load(f + i + j);
        __m256i x1 = info8.load(f + i + j + l);
        __m256i x2 = info8.load(f + i + j + l * 2);
        __m256i x3 = info8.load(f + i + j + l * 3);

        __m256i s0 = info8.add(x0, x2);
        __m256i s1 = info8.add(x1, x3);
        __m256i d0 =
            info8.mul(info8.sub(x0, x2), info8.load(roots_.data() + l * 2 + j));
        __m256i d1 =
            info8.mul(info8.sub(x1, x3), info8.load(roots_.data() + l * 3 + j));
        __m256i tw = info8.load(roots_.data() + l + j);

        info8.store(f + i + j, info8.add(s0, s1));
        info8.store(f + i + j + l, info8.mul(info8.sub(s0, s1), tw));
        info8.store(f + i + l * 2 + j, info8.add(d0, d1));
        info8.store(f + i + l * 3 + j, info8.mul(info8.sub(d0, d1), tw));
      }
    }
  }

  for (std::size_t i = 0; i < n; i += 8) {
    __m256i fi = info8.load(f + i);
    fi = info8.add(info8.neg<0b11110000>(fi),
                   _mm256_permute2x128_si256(fi, fi, 0b01));
    fi = info8.mul(fi, rt4);
    fi = info8.add(info8.neg<0b11001100>(fi),
                   _mm256_shuffle_epi32(fi, 0b01001110));
    fi = info8.mul_odd<0b10001000>(fi, rt2);
    fi = info8.add(info8.neg<0b10101010>(fi),
                   _mm256_shuffle_epi32(fi, 0b10110001));
    info8.store(f + i, fi);
  }

    }
    POLY998_AVX void backward_avx(Mint* f,std::size_t n) {
        const Mint* roots=inverse_roots_.data();
        const Simd info8;
        Mint r2[8],r4[8];std::fill_n(r2,8,ONE);std::fill_n(r4,8,ONE);
        r2[3]=r2[7]=roots[3];r4[5]=roots[5];r4[6]=roots[6];r4[7]=roots[7];

        __m256i root2=info8.load(r2),root4=info8.load(r4);

  const __m256i inv8_n = Simd::set(inv_size_[__builtin_ctz(static_cast<unsigned>(n))]);
  root4 = info8.mul(root4, inv8_n);

  for (std::size_t i = 0; i < n; i += 8) {
    __m256i fi = info8.load(f + i);
    fi = info8.add(info8.neg<0b10101010>(fi),
                   _mm256_shuffle_epi32(fi, 0b10110001));
    fi = info8.mul_odd<0b10001000>(fi, root2);
    fi = info8.add(info8.neg<0b11001100>(fi),
                   _mm256_shuffle_epi32(fi, 0b01001110));
    fi = info8.mul(fi, root4);
    fi = info8.add(info8.neg<0b11110000>(fi),
                   _mm256_permute2x128_si256(fi, fi, 0b01));
    info8.store(f + i, fi);
  }

  const bool n_4b = ((__builtin_ctz(static_cast<unsigned>(n)) - 3) & 1) != 0;
  std::size_t l = 8;
  for (; l < (n_4b ? n / 2 : n); l *= 4) {
    for (std::size_t i = 0; i < n; i += l * 2 * 2) {
      for (std::size_t j = 0; j < l; j += 8) {
        __m256i x0 = info8.load(f + i + j);
        __m256i x1 = info8.load(f + i + j + l);
        __m256i x2 = info8.load(f + i + j + l * 2);
        __m256i x3 = info8.load(f + i + j + l * 3);

        __m256i tw = info8.load(roots + l + j);
        __m256i y1 = info8.mul(x1, tw);
        __m256i y3 = info8.mul(x3, tw);
        __m256i s0 = info8.add(x0, y1);
        __m256i d0 = info8.sub(x0, y1);
        __m256i s1 = info8.add(x2, y3);
        __m256i d1 = info8.sub(x2, y3);
        __m256i z0 = info8.mul(s1, info8.load(roots + l * 2 + j));
        __m256i z1 = info8.mul(d1, info8.load(roots + l * 3 + j));

        info8.store(f + i + j, info8.add(s0, z0));
        info8.store(f + i + j + l, info8.add(d0, z1));
        info8.store(f + i + l * 2 + j, info8.sub(s0, z0));
        info8.store(f + i + l * 3 + j, info8.sub(d0, z1));
      }
    }
  }

  if (n_4b) {
    for (std::size_t i = 0; i < n; i += l * 2) {
      for (std::size_t j = 0; j < l; j += 8) {
        __m256i fx = info8.load(f + i + j);
        __m256i fy =
            info8.mul(info8.load(f + i + j + l), info8.load(roots + l + j));
        info8.store(f + i + j, info8.add(fx, fy));
        info8.store(f + i + j + l, info8.sub(fx, fy));
      }
    }
  }

    }
#endif

    // DIF: natural-order coefficients -> bit-reversed frequency order.
    // For a zero-padded polynomial of length m, the first m entries of
    // its 2m-point transform equal its m-point transform. exp() uses this.
    void forward(Mint* a, int n) {
        ensure(n);
#if POLY998_SIMD
        if(n>=16 && has_avx2()){forward_avx(a,n);return;}
#endif
        for (int m = n >> 1; m > 1; m >>= 1) {
            const Mint* w = roots_.data() + m;
            for (int i = 0; i < n; i += m * 2) {
                Mint* x = a + i;
                Mint* y = x + m;
                int j = 0;
                for (; j + 3 < m; j += 4) {
                    const Mint x0 = x[j], x1 = x[j + 1], x2 = x[j + 2], x3 = x[j + 3];
                    const Mint y0 = y[j], y1 = y[j + 1], y2 = y[j + 2], y3 = y[j + 3];
                    x[j] = add(x0, y0); x[j + 1] = add(x1, y1);
                    x[j + 2] = add(x2, y2); x[j + 3] = add(x3, y3);
                    y[j] = mul(sub(x0, y0), w[j]);
                    y[j + 1] = mul(sub(x1, y1), w[j + 1]);
                    y[j + 2] = mul(sub(x2, y2), w[j + 2]);
                    y[j + 3] = mul(sub(x3, y3), w[j + 3]);
                }
                for (; j < m; ++j) {
                    const Mint u = x[j], v = y[j];
                    x[j] = add(u, v);
                    y[j] = mul(sub(u, v), w[j]);
                }
            }
        }
        // The size-two butterflies need no multiplication by one.
        if (n > 1) {
            for (int i = 0; i < n; i += 2) {
                const Mint u = a[i], v = a[i + 1];
                a[i] = add(u, v); a[i + 1] = sub(u, v);
            }
        }
    }
    void backward(Mint* a, int n) {
        ensure(n);
#if POLY998_SIMD
        if(n>=16 && has_avx2()){backward_avx(a,n);return;}
#endif
        if (n == 1) return;
        for (int i = 0; i < n; i += 2) {
            const Mint u = a[i], v = a[i + 1];
            a[i] = add(u, v); a[i + 1] = sub(u, v);
        }
        for (int m = 2; m < n; m <<= 1) {
            const Mint* w = roots_.data() + m;
            for (int i = 0; i < n; i += m * 2) {
                Mint* x = a + i;
                Mint* y = x + m;
                int j = 0;
                for (; j + 3 < m; j += 4) {
                    const Mint x0 = x[j], x1 = x[j + 1], x2 = x[j + 2], x3 = x[j + 3];
                    const Mint y0 = mul(y[j], w[j]), y1 = mul(y[j + 1], w[j + 1]);
                    const Mint y2 = mul(y[j + 2], w[j + 2]), y3 = mul(y[j + 3], w[j + 3]);
                    x[j] = add(x0, y0); x[j + 1] = add(x1, y1);
                    x[j + 2] = add(x2, y2); x[j + 3] = add(x3, y3);
                    y[j] = sub(x0, y0); y[j + 1] = sub(x1, y1);
                    y[j + 2] = sub(x2, y2); y[j + 3] = sub(x3, y3);
                }
                for (; j < m; ++j) {
                    const Mint u = x[j], v = mul(y[j], w[j]);
                    x[j] = add(u, v); y[j] = sub(u, v);
                }
            }
        }
        const Mint scale = inv_size_[__builtin_ctz(static_cast<unsigned>(n))];
        a[0]*=scale; a[n/2]*=scale;
        for (int i=1;i<n/2;++i) {
            const Mint x=a[i],y=a[n-i];a[i]=y*scale;a[n-i]=x*scale;
        }
    }
};

class Series {
    NTT ntt_;
    Vec inv_int_;
public:
    void prepare_inverses(int n) {
        if(n<0 || n>MAX_FPS)throw std::length_error("FPS size exceeds 2^22");
        int old=static_cast<int>(inv_int_.size());if(n<=old)return;
#if POLY998_SIMD && !defined(POLY998_DISABLE_BATCH_INV)
        if(n>=64 && has_avx2()){
            const int target=ceil_pow2(n);inv_int_.resize(target);
            if(old<=1)inv_int_[1]=ONE;
            const int begin=std::max(8,(old+7)&~7);
            for(int i=std::max(2,old);i<begin;++i)inv_int_[i]=-Mint(MOD/i)*inv_int_[MOD%i];
            inverse_table_avx(inv_int_.data(),begin,target);return;
        }
#endif
        inv_int_.resize(n);if(old<=1 && n>1)inv_int_[1]=ONE;
        for(int i=std::max(2,old);i<n;++i)inv_int_[i]=-Mint(MOD/i)*inv_int_[MOD%i];
    }

    Vec integrate(const Vec& a){
        int n=static_cast<int>(a.size());prepare_inverses(n+1);Vec b(n+1);
        product(b.data()+1,a.data(),inv_int_.data()+1,n);return b;
    }
    Mint integer_inverse(int i) {
        prepare_inverses(i+1); return inv_int_[i];
    }
    Vec convolution(const Vec& a, const Vec& b) {
        if (a.empty() || b.empty()) return {};
        const std::size_t result_size = a.size()+b.size()-1;
        if (result_size > MAX_NTT) throw std::length_error("convolution exceeds 2^23");
        const int need = static_cast<int>(result_size);
        if (std::min(a.size(),b.size()) <= 32) {
            Vec c(need);
            for (std::size_t i=0; i<a.size(); ++i)
                for (std::size_t j=0; j<b.size(); ++j) c[i+j] += a[i]*b[j];
            return c;
        }
        const int len=ceil_pow2(need);
        Vec x(a); x.resize(len);
        ntt_.forward(x.data(),len);
        if (&a == &b) product(x.data(),x.data(),x.data(),len);
        else {
            Vec y(b); y.resize(len); ntt_.forward(y.data(),len);
            product(x.data(),x.data(),y.data(),len);
        }
        ntt_.backward(x.data(),len); x.resize(need); return x;
    }

    // Exact prefix product. For n just above a power of two, compute the
    // lower block plus a small cross term; the high*high block cannot contribute.
    Vec multiply_low(const Vec& a,const Vec& b,int n) {
        if(!n) return {};
        const int sa=static_cast<int>(std::min<std::size_t>(a.size(),n));
        const int sb=static_cast<int>(std::min<std::size_t>(b.size(),n));
        if(!sa || !sb) return Vec(n);
        if(std::min(sa,sb)<=32) {
            Vec c(n);
            for(int i=0;i<sa;++i)
                for(int j=0;j<std::min(sb,n-i);++j) c[i+j]+=a[i]*b[j];
            return c;
        }
        const bool same=&a==&b;
        const int len=ceil_pow2(n),h=len/2,r=n-h;
        if(sa+sb-1>len && h>=64 && r<=h/4) {
            // a=a0+x^h*a1, b=b0+x^h*b1; n=h+r <= 2h.
            Vec a0(a.begin(),a.begin()+std::min(sa,h));
            Vec b0; if(!same) b0.assign(b.begin(),b.begin()+std::min(sb,h));
            Vec c=convolution(a0,same?a0:b0); c.resize(n);
            const int ah=std::max(0,sa-h),bh=std::max(0,sb-h);
            if(r<=32) {
                for(int i=0;i<r;++i) {
                    if(i<ah) for(int j=0;j<std::min({sb,r-i,h});++j) c[h+i+j]+=a[h+i]*b[j];
                    if(i<bh) for(int j=0;j<std::min({sa,r-i,h});++j) c[h+i+j]+=b[h+i]*a[j];
                }
            } else {
                const int small=ceil_pow2(2*r-1);
                Vec x(small),y(small),z;
                std::copy_n(a.begin(),std::min(sa,r),x.begin());
                if(bh) std::copy_n(b.begin()+h,bh,y.begin());
                ntt_.forward(x.data(),small); ntt_.forward(y.data(),small);
                if(same) {
                    for(int i=0;i<small;++i) x[i]*=y[i]*Mint(2);
                } else {
                    z.resize(small);
                    product(z.data(),x.data(),y.data(),small);
                    x.assign(small,0); y.assign(small,0);
                    if(ah) std::copy_n(a.begin()+h,ah,x.begin());
                    std::copy_n(b.begin(),std::min(sb,r),y.begin());
                    ntt_.forward(x.data(),small); ntt_.forward(y.data(),small);
                    for(int i=0;i<small;++i) x[i]=z[i]+x[i]*y[i];
                }
                ntt_.backward(x.data(),small);
                for(int i=0;i<r;++i) c[h+i]+=x[i];
            }
            return c;
        }
        // Do not copy a full input before convolution copies it again.
        if(sa==static_cast<int>(a.size()) && sb==static_cast<int>(b.size())) {
            Vec c=convolution(a,b); c.resize(n); return c;
        }
        Vec x(a.begin(),a.begin()+sa),y;
        if(!same) y.assign(b.begin(),b.begin()+sb);
        Vec c=convolution(x,same?x:y); c.resize(n); return c;
    }

    // Fourfold inverse lifting, adapted from reference inv_9_33e_4th.
    Vec inverse_fourth(const Vec& f,int m) {
        const int n=ceil_pow2(m),base=(__builtin_ctz(static_cast<unsigned>(n))&1)?8:16;
        Vec x(n),xh(n),fh(n),ah(n/2),dh(n/2);
        const Mint inv0=f[0].inv();x[0]=inv0;
        for(int j=1;j<std::min(base,m);++j){Mint v=0;
            for(int i=1;i<=j && i<static_cast<int>(f.size());++i)v-=f[i]*x[j-i];x[j]=v*inv0;}
        for(int t=base;t<n;t*=4){
            const int len=4*t;
            std::copy_n(x.begin(),t,xh.begin());std::fill(xh.begin()+t,xh.begin()+len,Mint(0));
            const int flen=std::min<int>({len,m,static_cast<int>(f.size())});
            std::copy_n(f.begin(),flen,fh.begin());std::fill(fh.begin()+flen,fh.begin()+len,Mint(0));
            ntt_.forward(xh.data(),len);ntt_.forward(fh.data(),len);
            product(fh.data(),fh.data(),xh.data(),len);ntt_.backward(fh.data(),len);
            std::copy_n(fh.begin()+t,t,ah.begin());std::fill(ah.begin()+t,ah.begin()+2*t,Mint(0));
            ntt_.forward(ah.data(),2*t);product(dh.data(),ah.data(),ah.data(),2*t);ntt_.backward(dh.data(),2*t);
            for(int i=0;i<t;++i){
                const Mint a0=fh[t+i],a1=fh[2*t+i],a2=fh[3*t+i],q0=dh[i],q1=dh[t+i];
                fh[i]=-a0;fh[t+i]=q0-a1;fh[2*t+i]=q1-a2;dh[i]=a1+a1-q0;
            }
            std::fill(dh.begin()+t,dh.begin()+2*t,Mint(0));ntt_.forward(dh.data(),2*t);
            product(dh.data(),dh.data(),ah.data(),2*t);ntt_.backward(dh.data(),2*t);
            for(int i=0;i<t;++i)fh[2*t+i]+=dh[i];
            std::fill(fh.begin()+3*t,fh.begin()+len,Mint(0));ntt_.forward(fh.data(),len);
            product(fh.data(),fh.data(),xh.data(),len);ntt_.backward(fh.data(),len);
            std::copy_n(fh.begin(),std::min(3*t,m-t),x.begin()+t);
        }
        x.resize(m);return x;
    }

    void block_transform(const Mint* src,int count,Mint* dst,int n) {
        if(src!=dst)std::copy_n(src,count,dst);
        std::fill(dst+count,dst+2*n,Mint(0));ntt_.forward(dst,2*n);
    }
    void fold_blocks(Vec& a,int u,int n) {
        for(int block=u-1;block>0;--block){Mint* dst=a.data()+block*2*n;const Mint* prev=dst-2*n;
            fold_pair(dst,prev,n);
        }
    }
    void block_sum(Mint* out,const Vec& pairs,const Vec& rhs,int block,int n) {
        product(out,pairs.data()+block*2*n,rhs.data(),2*n);
        for(int j=1;j<block;++j) product_add(out,pairs.data()+(block-j)*2*n,rhs.data()+j*2*n,2*n);
    }
    void block_fixed(const Mint* src,const Mint* fixed,Mint* out,int n) {
        block_transform(src,n,out,n);product(out,out,fixed,2*n);ntt_.backward(out,2*n);
    }

    // Direct normalized power via f*Dg=k*(Df)*g, D=x*d/dx.
    // Does NOT invoke full-length log and exp. n<MOD makes the block solve invertible.
    Vec power_block(const Vec& f,u32 k) {
        const int m=static_cast<int>(f.size());prepare_inverses(m);
        if(m<=32){
            Vec g(m);if(!m)return g;g[0]=1;
            for(int j=1;j<m;++j){Mint s=0;
                for(int i=1;i<=j;++i)s+=((Mint(k)+Mint(1))*Mint(i)-Mint(j))*f[i]*g[j-i];
                g[j]=s*inv_int_[j];}
            return g;
        }
        const int target=std::max(2,31-__builtin_clz(static_cast<unsigned>(m)));
        const int n=ceil_pow2((m+target-1)/target),u=(m+n-1)/n,len=2*n;
        prepare_inverses(n*u);
        Vec fbase(f.begin(),f.begin()+n),base=power_block(fbase,k),g(n*u);
        std::copy(base.begin(),base.end(),g.begin());
        Vec nf(len*u),ndf(len*u),ng(len*u),h(len),psi(len),phi(len);
        block_transform(f.data(),n,nf.data(),n);block_transform(g.data(),n,ng.data(),n);
        product(psi.data(),nf.data(),ng.data(),len);ntt_.backward(psi.data(),len);
        Vec ih=inverse(psi,n);std::copy(ih.begin(),ih.end(),h.begin());ntt_.forward(h.data(),len);
        for(int b=0;b<u;++b){
            const int off=b*n,cnt=std::min(n,m-off);
            if(b)block_transform(f.data()+off,cnt,nf.data()+b*len,n);
            Mint* df=ndf.data()+b*len;
            index_mul(df,f.data()+off,off,cnt);
            ntt_.forward(df,len);
        }
        fold_blocks(nf,u,n);fold_blocks(ndf,u,n);
        const Mint kp=Mint(k)+Mint(1);
        for(int b=1;b<u;++b){
            const int off=b*n;
            if(b>1)block_transform(g.data()+off-n,n,ng.data()+(b-1)*len,n);
            block_sum(psi.data(),nf,ng,b,n);block_sum(phi.data(),ndf,ng,b,n);
            ntt_.backward(psi.data(),len);ntt_.backward(phi.data(),len);
            power_residual(psi.data(),phi.data(),psi.data(),off,n,kp);
            block_fixed(psi.data(),h.data(),phi.data(),n);
            product(phi.data(),phi.data(),inv_int_.data()+off,n);
            block_fixed(phi.data(),ng.data(),psi.data(),n);
            std::copy_n(psi.begin(),n,g.begin()+off);
        }
        g.resize(m);return g;
    }

    Vec inverse(const Vec& a, int n) {
        if (n == 0) return {};
        Vec b(n, 0);
        const Mint inv0=inverse_scalar(a[0]);
        b[0]=inv0;
        auto terms=sparse_terms(a,n,n<=32?32:24);
        if(terms.size()<=24 || n<=32) {
            for(auto& term:terms) term.second*=inv0;
            for(int j=1;j<n;++j) {
                Mint v=0;
                for(auto [i,c]:terms) { if(i>j) break; v-=c*b[j-i]; }
                b[j]=v;
            }
            return b;
        }
#ifndef POLY998_DISABLE_FOURTH
        if(n>=512 && n>ceil_pow2(n)*3/4){Vec().swap(b);return inverse_fourth(a,n);}
#endif
        // A small schoolbook base avoids many tiny transforms.
        for(int j=1;j<32;++j) {
            Mint v=0;
            for(int i=1;i<=j && i<static_cast<int>(a.size());++i) v-=a[i]*b[j-i];
            b[j]=v*inv0;
        }
        Vec x, y;
        for (int m = 32; m < n; m <<= 1) {
            // A very short final tail is cheaper than another full NTT stage.
            if(m>=64 && n-m<=4) {
                for(int j=m;j<n;++j) {
                    Mint v=0;
                    for(int i=1;i<=j && i<static_cast<int>(a.size());++i) v-=a[i]*b[j-i];
                    b[j]=v*inv0;
                }
                break;
            }
            const int len = m * 2;
            x.assign(len, 0); y.assign(len, 0);
            std::copy_n(a.begin(), std::min<int>(a.size(), len), x.begin());
            std::copy_n(b.begin(), m, y.begin());
            ntt_.forward(x.data(), len); ntt_.forward(y.data(), len);
            product(x.data(),x.data(),y.data(),len);
            ntt_.backward(x.data(), len);
            // Only the upper half is required, and it has no cyclic aliasing.
            std::fill_n(x.begin(), m, 0);
            ntt_.forward(x.data(), len);
            product(x.data(),x.data(),y.data(),len);
            ntt_.backward(x.data(), len);
            for (int i = m; i < std::min(n, len); ++i) b[i] = sub(0, x[i]);
        }
        return b;
    }

    // f/g mod x^n. A half-precision inverse suffices: first obtain q0,
    // then solve only the upper-half residual. All NTT sizes are <= bit_ceil(n).
    Vec quotient(const Vec& f,const Vec& g,int n) {
        if(!n) return {};
        if(g.empty() || g[0].is_zero()) throw std::invalid_argument("series division requires b[0] != 0");
        const Mint inv0=g[0].inv();
        auto terms=sparse_terms(g,n,n<=32?32:24);
        if(terms.size()<=24 || n<=32) {
            for(auto& term:terms) term.second*=inv0;
            Vec q(n);
            for(int j=0;j<n;++j) {
                Mint v=j<static_cast<int>(f.size())?f[j]*inv0:Mint(0);
                for(auto [i,c]:terms) { if(i>j) break; v-=c*q[j-i]; }
                q[j]=v;
            }
            return q;
        }
        const int len=ceil_pow2(n),m=len/2;
        Vec h=inverse(g,m); h.resize(len); ntt_.forward(h.data(),len);
        Vec q(len);
        std::copy_n(f.begin(),std::min<int>(f.size(),m),q.begin());
        ntt_.forward(q.data(),len);
        product(q.data(),q.data(),h.data(),len);
        ntt_.backward(q.data(),len);
        std::fill(q.begin()+m,q.end(),Mint(0));
        Vec q_fft=q; ntt_.forward(q_fft.data(),len);
        Vec work(len);
        std::copy_n(g.begin(),std::min<int>(g.size(),n),work.begin());
        ntt_.forward(work.data(),len);
        product(work.data(),work.data(),q_fft.data(),len);
        ntt_.backward(work.data(),len);
        // deg(g*q0)<3m, hence cyclic aliasing affects only degrees <m.
        for(int i=m;i<n;++i) work[i]=(i<static_cast<int>(f.size())?f[i]:Mint(0))-work[i];
        std::fill(work.begin(),work.begin()+m,Mint(0));
        std::fill(work.begin()+n,work.end(),Mint(0));
        ntt_.forward(work.data(),len);
        product(work.data(),work.data(),h.data(),len);
        ntt_.backward(work.data(),len);
        q.resize(n);
        for(int i=m;i<n;++i) q[i]=work[i];
        return q;
    }

    Vec logarithm(const Vec& a) {
        const int n=static_cast<int>(a.size());
        prepare_inverses(n);
        if(n<=1) return Vec(n);
        Vec derivative(n-1);
        index_mul(derivative.data(),a.data()+1,1,n-1);
        Vec q=quotient(derivative,a,n-1),result(n);
        product(result.data()+1,q.data(),inv_int_.data()+1,n-1);
        return result;
    }

    // g^2=a mod x^m; h=1/g mod x^(m/2), with its m-point NTT cached.
    // Extend h only as far as needed, then append (a-g^2)/(2g)'s upper half.
    Vec square_root(const Vec& a,Mint root) {
        const int n=static_cast<int>(a.size());
        if(n<=1) return n?Vec{root}:Vec{};
        auto terms=sparse_terms(a,n,n<=32?32:24);
        if(terms.size()<=24 || n<=32) {
            const Mint inv0=a[0].inv(),alpha=Mint((MOD+1)/2);
            prepare_inverses(n);
            for(auto& term:terms) term.second*=inv0;
            Vec out(n); out[0]=root;
            std::vector<Mint> weights;
            for(auto [i,c]:terms) weights.push_back((alpha+Mint(1))*Mint(i));
            for(int j=1;j<n;++j) {
                Mint v=0,jj=j;
                for(std::size_t t=0;t<terms.size() && terms[t].first<=j;++t) {
                    auto [i,c]=terms[t];v+=(weights[t]-jj)*c*out[j-i];
                }
                out[j]=v*inv_int_[j];
            }
            return out;
        }
        Vec g{root},h{root.inv()},h_fft;
        Vec g_fft,work;
        const Mint half=Mint((MOD+1)/2);
        g.reserve(ceil_pow2(n)); h.reserve(ceil_pow2(n)/2);
        for(int m=1;m<n;m<<=1) {
            const int len=m*2,need=std::min(n,len);
            g_fft.assign(len,0); std::copy(g.begin(),g.end(),g_fft.begin());
            ntt_.forward(g_fft.data(),len);
            if(m>1) {
                work.resize(m);
                product(work.data(),g_fft.data(),h_fft.data(),m);
                ntt_.backward(work.data(),m);
                std::fill_n(work.begin(),m/2,Mint(0));
                ntt_.forward(work.data(),m);
                product(work.data(),work.data(),h_fft.data(),m);
                ntt_.backward(work.data(),m);
                h.resize(m);
                for(int i=m/2;i<m;++i) h[i]=-work[i];
            }
            h_fft.assign(len,0);std::copy(h.begin(),h.end(),h_fft.begin());
            ntt_.forward(h_fft.data(),len);
            work.resize(len);
            product(work.data(),g_fft.data(),g_fft.data(),len);
            ntt_.backward(work.data(),len);
            for(int i=m;i<need;++i) work[i]=(a[i]-work[i])*half;
            std::fill_n(work.begin(),m,Mint(0));
            std::fill(work.begin()+need,work.end(),Mint(0));
            ntt_.forward(work.data(),len);
            product(work.data(),work.data(),h_fft.data(),len);
            ntt_.backward(work.data(),len);
            g.resize(need);
            for(int i=m;i<need;++i) g[i]=work[i];
        }
        return g;
    }

    // Newton doubling with a maintained inverse and reused forward transforms.
    // At entry to a stage: g = exp(f) mod x^m, h = 1/g mod x^(m/2).
    // Block ODE solve: Dg=(Df)g. Fixed g0 and 1/g0 transforms are reused.
    Vec exponential_block(const Vec& f) {
        const int m=static_cast<int>(f.size());prepare_inverses(m);
        if(m<=32){Vec g(m);if(!m)return g;g[0]=1;
            for(int j=1;j<m;++j){Mint s=0;for(int i=1;i<=j;++i)s+=Mint(i)*f[i]*g[j-i];g[j]=s*inv_int_[j];}
            return g;
        }
        const int target=std::max(2,31-__builtin_clz(static_cast<unsigned>(m)));
        const int n=ceil_pow2((m+target-1)/target),u=(m+n-1)/n,len=2*n;
        prepare_inverses(n*u);
        Vec fbase(f.begin(),f.begin()+n),base=exponential_block(fbase),g(n*u);
        std::copy(base.begin(),base.end(),g.begin());
        Vec ndf(len*u),ng(len*u),h=inverse(base,n),psi(len),phi(len);
        h.resize(len);ntt_.forward(h.data(),len);block_transform(g.data(),n,ng.data(),n);
        for(int b=0;b<u;++b){const int off=b*n,cnt=std::min(n,m-off);Mint* df=ndf.data()+b*len;
            index_mul(df,f.data()+off,off,cnt);ntt_.forward(df,len);}
        fold_blocks(ndf,u,n);
        for(int b=1;b<u;++b){const int off=b*n;
            if(b>1)block_transform(g.data()+off-n,n,ng.data()+(b-1)*len,n);
            block_sum(psi.data(),ndf,ng,b,n);ntt_.backward(psi.data(),len);
            block_fixed(psi.data(),h.data(),phi.data(),n);
            product(phi.data(),phi.data(),inv_int_.data()+off,n);
            block_fixed(phi.data(),ng.data(),psi.data(),n);std::copy_n(psi.begin(),n,g.begin()+off);
        }
        g.resize(m);return g;
    }

    Vec exponential(const Vec& f) {
        const int n = static_cast<int>(f.size());
        prepare_inverses(n);
        if (n == 1) return Vec{ONE};
        auto terms=sparse_terms(f,n,n<=32?32:24);
        if(terms.size()<=24 || n<=32) {
            for(auto& term:terms) term.second*=Mint(term.first);
            Vec out(n); out[0]=1;
            for(int j=1;j<n;++j) {
                Mint v=0;
                for(auto [i,c]:terms) { if(i>j) break; v+=c*out[j-i]; }
                out[j]=v*inv_int_[j];
            }
            return out;
        }
#ifndef POLY998_DISABLE_BLOCK_EXP
        if(n>=512)return exponential_block(f);
#endif
        Vec g{ONE, f[1]}, h{ONE}, h_fft(2, ONE);
        Vec g_fft, work;
        g.reserve(ceil_pow2(n));
        h.reserve(ceil_pow2(n) / 2);
        for (int m = 2; m < n; m <<= 1) {
            const int len = m * 2;
            const int need = std::min(n, len);
            g_fft.assign(len, 0);
            std::copy(g.begin(), g.end(), g_fft.begin());
            ntt_.forward(g_fft.data(), len);

            // Extend the inverse to m terms, reusing cached h_fft (length m).
            work.resize(m);
            product(work.data(),g_fft.data(),h_fft.data(),m);
            ntt_.backward(work.data(), m);
            std::fill_n(work.begin(), m / 2, 0);
            ntt_.forward(work.data(), m);
            product(work.data(),work.data(),h_fft.data(),m);
            ntt_.backward(work.data(), m);
            h.resize(m);
            for (int i = m / 2; i < m; ++i) h[i] = sub(0, work[i]);
            h_fft.assign(len, 0);
            std::copy(h.begin(), h.end(), h_fft.begin());
            ntt_.forward(h_fft.data(), len);

            // R = (f mod x^m)' * g - g' is divisible by x^(m-1).
            // A length-m cyclic convolution recovers R after unwrapping.
            work.assign(m, 0);
            for (int i = 1; i < m; ++i) work[i - 1] = mul(f[i], to_mont(i));
            ntt_.forward(work.data(), m);
            product(work.data(),work.data(),g_fft.data(),m);
            ntt_.backward(work.data(), m);
            for (int i = 1; i < m; ++i) work[i - 1] = sub(work[i - 1], mul(g[i], to_mont(i)));
            work.resize(len, 0);
            for (int i = 0; i < m - 1; ++i) {
                work[m + i] = work[i];
                work[i] = 0;
            }
            ntt_.forward(work.data(), len);
            product(work.data(),work.data(),h_fft.data(),len);
            ntt_.backward(work.data(), len);

            // e = f - log(g) mod x^need. Its first m coefficients are zero.
            // Descending traversal permits integration in place.
            for (int i = need - 1; i >= m; --i)
                work[i] = add(f[i], mul(work[i - 1], inv_int_[i]));
            std::fill_n(work.begin(), m, 0);
            std::fill(work.begin() + need, work.end(), 0);
            ntt_.forward(work.data(), len);
            product(work.data(),work.data(),g_fft.data(),len);
            ntt_.backward(work.data(), len);
            g.resize(need);
            for (int i = m; i < need; ++i) g[i] = work[i];
        }
        return g;
    }
};


inline Series& engine() { static thread_local Series instance; return instance; }
struct Exponent {
    u32 mod_p=0, mod_phi=0;
    int capped=0;
    bool zero=true;
    explicit Exponent(std::string_view text,int n) {
        if (text.empty()) throw std::invalid_argument("empty exponent");
        for (char c:text) {
            if (c<'0' || c>'9') throw std::invalid_argument("exponent must be nonnegative decimal");
            u32 d=static_cast<u32>(c-'0');
            mod_p=static_cast<u32>((static_cast<u64>(mod_p)*10+d)%MOD);
            mod_phi=static_cast<u32>((static_cast<u64>(mod_phi)*10+d)%(MOD-1));
            capped=static_cast<int>(std::min<u64>(n,static_cast<u64>(capped)*10+d));
            zero=zero && d==0;
        }
    }
};
inline std::optional<Mint> scalar_sqrt(Mint a) {
    if (a.is_zero()) return Mint(0);
    if (a.pow((MOD-1)/2)!=Mint(1)) return std::nullopt;
    // MOD-1 = 119*2^23, and 3 is a quadratic nonresidue.
    Mint c=Mint(3).pow(119), r=a.pow(60), t=a.pow(119);
    int m=23;
    while (t!=Mint(1)) {
        int i=0; Mint y=t;
        while (y!=Mint(1) && i<m) { y*=y; ++i; }
        if (i==m) return std::nullopt;
        Mint b=c.pow(u64(1)<<(m-i-1));
        r*=b; c=b*b; t*=c; m=i;
    }
    return r.val()<=MOD-r.val() ? r : -r;
}
} // namespace detail

class Poly : public std::vector<Mint> {
    using Base=std::vector<Mint>;
public:
    using Base::Base;
    Poly()=default;
    Poly(Base v):Base(std::move(v)) {}
    int order() const { // first nonzero coefficient, or size() for zero
        int d=0; while (d<static_cast<int>(size()) && (*this)[d].is_zero()) ++d; return d;
    }
    Poly& trim() { while (!empty() && back().is_zero()) pop_back(); return *this; }
    Poly pre(int n) const { // exact-length truncation / zero extension
        n=detail::degree(n,size()); Poly b(n);
        std::copy_n(begin(),std::min<std::size_t>(size(),n),b.begin()); return b;
    }
    Poly& operator+=(const Poly& b) {
        if(size()<b.size()) resize(b.size());
        for(std::size_t i=0;i<b.size();++i) (*this)[i]+=b[i];
        return *this;
    }
    Poly& operator-=(const Poly& b) {
        if(size()<b.size()) resize(b.size());
        for(std::size_t i=0;i<b.size();++i) (*this)[i]-=b[i];
        return *this;
    }
    Poly& operator*=(Mint c) { detail::scale_array(data(),data(),c,static_cast<int>(size()));return *this; }
    Poly& operator/=(Mint c) { return *this *= c.inv(); }
    Poly& operator*=(const Poly& b) { return *this = detail::engine().convolution(*this,b); }
    friend Poly operator+(Poly a,const Poly& b) { return a+=b; }
    friend Poly operator-(Poly a,const Poly& b) { return a-=b; }
    friend Poly operator*(const Poly& a,const Poly& b) { return detail::engine().convolution(a,b); }
    friend Poly operator*(Poly a,Mint c) { return a*=c; }
    friend Poly operator*(Mint c,Poly a) { return a*=c; }
    friend Poly operator/(Poly a,Mint c) { return a/=c; }
    Poly operator-() const { Poly r=*this; for(auto& x:r) x=-x; return r; }
    Poly mul_trunc(const Poly& b,int n) const {
        n=detail::degree(n,size()); return detail::engine().multiply_low(*this,b,n);
    }
    Poly square(int n=-1) const {
        if(n==-1) return detail::engine().convolution(*this,*this);
        n=detail::degree(n,size()); return detail::engine().multiply_low(*this,*this,n);
    }
    Poly derivative() const {
        if(empty()) return {};
        Poly d(size()-1);detail::index_mul(d.data(),data()+1,1,static_cast<int>(size())-1);return d;
    }
    Poly integral() const {
        if(size()>=MAX_FPS) throw std::length_error("integral exceeds FPS limit");
        return detail::engine().integrate(*this);
    }
    Poly inv(int n=-1) const {
        n=detail::degree(n,size()); if(!n) return {};
        if(empty() || front().is_zero()) throw std::invalid_argument("inv requires a[0] != 0");
        return detail::engine().inverse(*this,n);
    }
    Poly log(int n=-1) const {
        n=detail::degree(n,size()); if(!n) return {};
        if(empty() || front()!=Mint(1)) throw std::invalid_argument("log requires a[0] == 1");
        return detail::engine().logarithm(pre(n));
    }
    Poly exp(int n=-1) const {
        n=detail::degree(n,size()); if(!n) return {};
        if(!empty() && front()!=Mint(0)) throw std::invalid_argument("exp requires a[0] == 0");
        return detail::engine().exponential(pre(n));
    }
    Poly div_series(const Poly& b,int n) const {
        n=detail::degree(n,size()); return detail::engine().quotient(*this,b,n);
    }
    Poly pow(std::string_view exponent,int n=-1) const;
    Poly pow(long long k,int n=-1) const {
        if(k<0) throw std::invalid_argument("negative polynomial exponent");
        return pow(std::to_string(k),n);
    }
    std::optional<Poly> sqrt(int n=-1) const;
    std::pair<Poly,Poly> divmod(const Poly& divisor) const;
    friend Poly operator/(const Poly& a,const Poly& b) { return a.divmod(b).first; }
    friend Poly operator%(const Poly& a,const Poly& b) { return a.divmod(b).second; }
    Mint eval(Mint x) const {
        return detail::evaluate(data(),static_cast<int>(size()),x);
    }
    Poly taylor_shift(Mint c) const;
};

inline Poly Poly::pow(std::string_view exponent,int n) const {
    n=detail::degree(n,size()); detail::Exponent k(exponent,n);
    Poly ans(n); if(!n) return ans;
    if(k.zero) { ans[0]=1; return ans; }
    int d=0; while(d<n && (d>=static_cast<int>(size()) || (*this)[d].is_zero())) ++d;
    if(d==n) return ans;
    const u64 shift64=static_cast<u64>(d)*k.capped;
    if(shift64>=static_cast<u64>(n)) return ans;
    const int shift=static_cast<int>(shift64), m=n-shift;
    const Mint scale=(*this)[d].pow(k.mod_phi);
    if(m==1 || k.mod_p==0) { ans[shift]=scale; return ans; }
    const Mint inv_a0=(*this)[d].inv();
    Poly a(m);
    detail::scale_array(a.data(),data()+d,inv_a0,std::min<int>(m,size()-d));
    if(k.mod_p==1) {
        detail::scale_array(ans.data()+shift,a.data(),scale,m);
        return ans;
    }
    std::vector<std::pair<int,Mint>> sparse;
    // Bounded scan: don't allocate a dense list just to reject sparse mode.
    for(int i=1;i<m && sparse.size()<=24;++i)
        if(!a[i].is_zero()) sparse.emplace_back(i,a[i]);
    if(sparse.empty()) { ans[shift]=scale; return ans; }
    Poly result;
    if(sparse.size()<=24 || m<=32) {
        // U*V' = k*U'*V, V=U^k. Requires m < MOD.
        if(m<=32 && sparse.size()>24) {
            sparse.clear(); for(int i=1;i<m;++i) if(!a[i].is_zero()) sparse.emplace_back(i,a[i]);
        }
        auto& e=detail::engine(); e.prepare_inverses(m);
        result.resize(m); result[0]=1;
        std::vector<Mint> weights; weights.reserve(sparse.size());
        for(auto [i,c]:sparse) weights.push_back((Mint(k.mod_p)+Mint(1))*Mint(i));
        for(int j=1;j<m;++j) {
            Mint total=0, jj=j;
            for(std::size_t t=0;t<sparse.size() && sparse[t].first<=j;++t) {
                auto [i,c]=sparse[t]; total+=(weights[t]-jj)*c*result[j-i];
            }
            result[j]=total*e.integer_inverse(j);
        }
    } else if(k.mod_p==MOD-1) result=a.inv(m);
    else if(k.mod_p==MOD-2) result=a.inv(m).square(m);
    else if(k.mod_p==2) result=a.square(m);
    else if(k.mod_p==3) result=a.square(m).mul_trunc(a,m);
    else if(k.mod_p==4) result=a.square(m).square(m);
    else {
        auto& e=detail::engine();
#ifndef POLY998_DISABLE_BLOCK_POW
        if(m>=512)result=e.power_block(a,k.mod_p);
        else
#endif
        {Poly f=e.logarithm(a);f*=Mint(k.mod_p);Poly().swap(a);result=e.exponential(f);}
    }
    detail::scale_array(ans.data()+shift,result.data(),scale,m);
    return ans;
}

inline std::optional<Poly> Poly::sqrt(int n) const {
    n=detail::degree(n,size()); if(!n) return Poly{};
    Poly a=pre(n), out(n);
    const int d=a.order(); if(d==n) return out;
    if(d&1) return std::nullopt;
    auto r=detail::scalar_sqrt(a[d]); if(!r) return std::nullopt;
    Poly u(a.begin()+d,a.end());
    Poly b=detail::engine().square_root(u,*r);
    std::copy(b.begin(),b.end(),out.begin()+d/2);
    return out;
}

inline std::pair<Poly,Poly> Poly::divmod(const Poly& divisor) const {
    Poly a=*this,b=divisor; a.trim(); b.trim();
    if(b.empty()) throw std::invalid_argument("polynomial division by zero");
    if(a.size()<b.size()) return {{},std::move(a)};
    const int qn=static_cast<int>(a.size()-b.size()+1);
    detail::degree(qn,0);
    if(b.size()<=32 || qn<=32) {
        Poly q(qn); Mint ib=b.back().inv();
        for(int i=qn-1;i>=0;--i) {
            Mint v=a[i+b.size()-1]*ib; q[i]=v;
            for(std::size_t j=0;j<b.size();++j) a[i+j]-=v*b[j];
        }
        a.trim(); q.trim(); return {std::move(q),std::move(a)};
    }
    Poly ra(a.rbegin(),a.rend()), rb(b.rbegin(),b.rend());
    Poly q=ra.mul_trunc(rb.inv(qn),qn); std::reverse(q.begin(),q.end());
    Poly r=a-q*b; r.resize(b.size()-1); r.trim(); q.trim();
    return {std::move(q),std::move(r)};
}

inline Poly Poly::taylor_shift(Mint c) const {
    const int n=detail::degree(-1,size()); if(!n) return {};
    Poly fact(n), invfact(n), a(n), b(n);
    fact[0]=1; for(int i=1;i<n;++i) fact[i]=fact[i-1]*Mint(i);
    invfact[n-1]=fact[n-1].inv();
    for(int i=n-1;i>0;--i) invfact[i-1]=invfact[i]*Mint(i);
    Mint cp=1;
    for(int i=0;i<n;++i) { a[n-1-i]=(*this)[i]*fact[i]; b[i]=cp*invfact[i]; cp*=c; }
    Poly r=a.mul_trunc(b,n);
    std::reverse(r.begin(),r.end());
    detail::product(r.data(),r.data(),invfact.data(),n);
    return r;
}

// Releases only this thread's cached forward roots and integer inverses.
// Caches grow on demand and otherwise remain allocated; operations use local scratch.
inline void release_cache() { detail::engine()=detail::Series{}; }
} // namespace poly998


#include <cstdio>
#include <charconv>
class FastInput {
    static constexpr std::size_t SIZE = 1 << 16;
    char buffer_[SIZE];
    std::size_t pos_ = 0, size_ = 0;
    int get() {
        if (pos_ == size_) {
            size_ = std::fread(buffer_, 1, SIZE, stdin);
            pos_ = 0;
            if (!size_) return EOF;
        }
        return static_cast<unsigned char>(buffer_[pos_++]);
    }
public:
    bool token(std::string& s) {
        s.clear();
        int c;
        do { c = get(); } while (c != EOF && c <= ' ');
        if (c == EOF) return false;
        do { s.push_back(static_cast<char>(c)); c = get(); } while (c != EOF && c > ' ');
        return true;
    }
    bool coefficient(poly998::Mint& value) {
        int c;
        do { c = get(); } while (c != EOF && c <= ' ');
        if (c == EOF) return false;
        bool negative = c == '-';
        if (negative || c == '+') c = get();
        if (c < '0' || c > '9') return false;
        // Accumulate nine decimal digits before reducing. Common <=9-digit
        // coefficients need at most one subtraction, not a remainder per digit.
        static constexpr poly998::u32 pow10[] = {1,10,100,1000,10000,100000,
                                                1000000,10000000,100000000};
        poly998::u32 x=0,block=0;
        int digits=0; bool full_block=false;
        while(c>='0' && c<='9') {
            block=block*10+static_cast<unsigned>(c-'0');
            if(++digits==9) {
                if(!full_block) { x=block>=poly998::MOD?block-poly998::MOD:block; full_block=true; }
                else x=static_cast<poly998::u32>((static_cast<poly998::u64>(x)*1000000000ull+block)%poly998::MOD);
                digits=0;block=0;
            }
            c=get();
        }
        if(digits) x=full_block?static_cast<poly998::u32>((static_cast<poly998::u64>(x)*pow10[digits]+block)%poly998::MOD):block;
        if (c != EOF && c > ' ') return false;
        value = poly998::Mint(negative && x ? poly998::MOD - x : x);
        return true;
    }
};

class FastOutput {
    static constexpr std::size_t SIZE=1<<16;
    char buffer_[SIZE];
    std::size_t used_=0;
public:
    bool flush() {
        if(!used_) return true;
        if(std::fwrite(buffer_,1,used_,stdout)!=used_) return false;
        used_=0;return true;
    }
    bool coefficient(poly998::Mint value,char separator) {
        if(SIZE-used_<12 && !flush()) return false;
        auto result=std::to_chars(buffer_+used_,buffer_+SIZE,value.val());
        if(result.ec!=std::errc{}) return false;
        used_=static_cast<std::size_t>(result.ptr-buffer_);
        buffer_[used_++]=separator;return true;
    }
    bool newline() {
        if(used_==SIZE && !flush()) return false;
        buffer_[used_++]='\n';return true;
    }
};


int main() {
    try {
        FastInput input;
        std::string token, exponent;
        if (!input.token(token)) return 0;
        int n = 0;
        for (char c : token) {
            if (c < '0' || c > '9') throw std::invalid_argument("invalid n");
            auto next=static_cast<std::uint64_t>(n)*10+(c-'0');
            if(next>poly998::MAX_FPS) throw std::length_error("n exceeds 2^22");
            n=static_cast<int>(next);
        }
        if(!input.token(exponent)) throw std::invalid_argument("missing exponent");
        poly998::Poly a(n);
        // As in the original driver, zero exponent does not require coefficients.
        if(exponent.find_first_not_of('0')!=std::string::npos)
            for(auto& x:a) if(!input.coefficient(x))
                throw std::invalid_argument("missing/invalid coefficient");
        auto b=a.pow(exponent,n);
        FastOutput out;
        for(int i=0;i<n;++i) if(!out.coefficient(b[i],i+1==n?'\n':' ')) return 1;
        if(n==0 && !out.newline()) return 1;
        if(!out.flush()) return 1;
    } catch(const std::exception& e){std::fprintf(stderr,"error: %s\n",e.what());return 1;}
}
