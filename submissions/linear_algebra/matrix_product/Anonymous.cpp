#include <bits/extc++.h>
#include <immintrin.h>

#pragma GCC target("avx2,bmi")
#pragma GCC optimize("O3,unroll-loops")

#ifdef __linux__
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;
constexpr std::size_t buf_def_size=262144;
constexpr std::size_t buf_flush_threshold=32;
constexpr std::size_t string_copy_threshold=512;
constexpr u64 E16=1e16,E12=1e12,E8=1e8,E4=1e4;
struct _io_t{
    u8 t_i[1<<15];
    int t_o[10000];
    constexpr _io_t(){
        std::fill(t_i,t_i+(1<<15),u8(-1));
        for(int i=0;i<10;++i){
            for(int j=0;j<10;++j){
                t_i[0x3030+256*j+i]=j+10*i;
            }
        }
        for(int e0=(48<<0),j=0;e0<(58<<0);e0+=(1<<0)){
			for(int e1=(48<<8);e1<(58<<8);e1+=(1<<8)){
				for(int e2=(48<<16);e2<(58<<16);e2+=(1<<16)){
					for(int e3=(48<<24);e3<(58<<24);e3+=(1<<24)){
						t_o[j++]=e0^e1^e2^e3;
					}
				}
			}
		}
    }
    void get(char*s,u32 p)const{
        *((int*)s)=t_o[p];
    }
};
constexpr _io_t _iot={};
struct Qinf{
    explicit Qinf(FILE*fi):f(fi){
        auto fd=fileno(f);
        fstat(fd,&Fl);
        bg=(char*)mmap(0,Fl.st_size+1,PROT_READ,MAP_PRIVATE,fd,0);
        p=bg,ed=bg+Fl.st_size;
    }
    ~Qinf(){
        munmap(bg,Fl.st_size+1);
    }
    template<std::unsigned_integral T>Qinf&operator>>(T&x){
        skip_space();
        x=*p++-'0';
        for(;;){
            T y=_iot.t_i[*reinterpret_cast<u16*>(p)];
            if(y>99){break;}
            x=x*100+y,p+=2;
        }
        if(*p>' '){
            x=x*10+(*p++&15);
        }
        return *this;
    }
    private:
    void skip_space(){
        while(*p<=' '){
            ++p;
        }	
    }
    FILE*f;
    char*bg,*ed,*p;
    struct stat Fl;
}cin(stdin);
struct Qoutf{
    explicit Qoutf(FILE*fi,std::size_t sz=buf_def_size):f(fi),bg(new char[sz]),ed(bg+sz-buf_flush_threshold),p(bg){}
    ~Qoutf(){
        flush();
        delete[] bg;
    }
    void flush(){
        fwrite_unlocked(bg,1,p-bg,f),p=bg;
    }
    Qoutf&operator<<(u32 x){
        if(x>=E8){
            put2(x/E8),x%=E8,putb(x/E4),putb(x%E4);
        }
        else if(x>=E4) {
            put4(x/E4),putb(x%E4);
        }
        else{
            put4(x);
        }
        chk();
        return *this;
    }
    Qoutf&operator<<(u64 x){
        if(x>=E8){
            u64 q0=x/E8,r0=x%E8;
            if(x>=E16){
                u64 q1=q0/E8,r1=q0%E8;
                put4(q1),putb(r1/E4),putb(r1%E4);
            } 
            else if(x>=E12){
                put4(q0/E4),putb(q0%E4);
            }
            else{
                put4(q0);
            }
            putb(r0/E4),putb(r0%E4);
        }
        else{
            if(x>=E4){
                put4(x/E4),putb(x%E4);
            }
            else{
                put4(x);
            }
        }
        chk();
        return *this;
    }
    Qoutf&operator<<(char ch){
        *p++=ch;
        return *this;
    }
    private:
    void putb(u32 x){
        _iot.get(p,x),p+=4;
    }
    void put4(u32 x){
        if(x>99){
            if(x>999){
                putb(x);
            }
            else{
                _iot.get(p,x*10),p+=3;
            }	
        }
        else{
            put2(x);
        }
    }
    void put2(u32 x){
        if(x>9){
            _iot.get(p,x*100),p+=2;
        }
        else{
            *p++=x+'0';
        }
    }
    void chk(){
        if(p>ed)[[unlikely]]{
            flush();
        }
    }
    FILE *f;
    char *bg,*ed,*p;
}cout(stdout);
#else
using std::cin, std::cout;
#endif

namespace mat {
using Scalar = uint32_t;
using Vector = __m256i;
using idt = std::size_t;

constexpr Scalar mod = 998244353U;
constexpr Scalar niv = 998244351U;
constexpr Scalar R2 = 932051910U;
constexpr uint64_t ngm = uint64_t(1) << 63;
constexpr idt BLK = 64;
constexpr idt VLEN = 8;

inline const Vector& vmod() {
    static const Vector M = _mm256_set1_epi32(int(mod));
    return M;
}
inline const Vector& vniv() {
    static const Vector N = _mm256_set1_epi32(int(niv));
    return N;
}
inline const Vector& vr2() {
    static const Vector R = _mm256_set1_epi32(int(R2));
    return R;
}
inline const Vector& vmod2() {
    static const Vector M2 = _mm256_set1_epi32(int(mod * 2));
    return M2;
}
inline const Vector& vngm() {
    static const Vector G = _mm256_set1_epi64x(static_cast<long long>(ngm));
    return G;
}

[[gnu::always_inline]] inline Vector shrink(Vector x) {
    return _mm256_min_epu32(x, _mm256_sub_epi32(x, vmod()));
}

[[gnu::always_inline]] inline Vector dilate(Vector x) {
    return _mm256_min_epu32(x, _mm256_add_epi32(x, vmod()));
}

[[gnu::always_inline]] inline Vector reduce(Vector a, Vector b) {
    Vector kil = _mm256_mul_epu32(a, vniv()), jok = _mm256_mul_epu32(b, vniv());
    kil = _mm256_mul_epu32(kil, vmod());
    jok = _mm256_mul_epu32(jok, vmod());
    return _mm256_blend_epi32(_mm256_srli_epi64(_mm256_add_epi64(a, kil), 32), _mm256_add_epi64(b, jok), 0xAA);
}

[[gnu::always_inline]] inline Vector mul_mont(Vector a, Vector b) {
    return reduce(_mm256_mul_epu32(a, b), _mm256_mul_epu32(_mm256_srli_epi64(a, 32), b));
}

// 64x64 AVX2 kernel；noinline 避免巨型展开污染 Strassen 调用点的寄存器分配
[[gnu::noinline]] void kernel(const Scalar* __restrict__ a, const Scalar* __restrict__ b, Scalar* __restrict__ c) {
    const Vector M2 = vmod2(), Ngm = vngm();
#define INIT_R(p) Vector r##p = _mm256_setzero_si256(), R##p = _mm256_setzero_si256();
#define WORK_R(p)                                                                                                      \
    do {                                                                                                               \
        const Vector ap = _mm256_set1_epi32(int(a[(i + (p)) * BLK + w]));                                              \
        r##p = _mm256_add_epi64(r##p, _mm256_mul_epu32(ap, z0));                                                       \
        R##p = _mm256_add_epi64(R##p, _mm256_mul_epu32(ap, Z0));                                                       \
    } while (0)
#define SHRK_R(p)                                                                                                      \
    do {                                                                                                               \
        r##p = _mm256_sub_epi64(r##p, _mm256_min_epu32(M2, _mm256_and_si256(Ngm, r##p)));                              \
        R##p = _mm256_sub_epi64(R##p, _mm256_min_epu32(M2, _mm256_and_si256(Ngm, R##p)));                              \
    } while (0)
#define STORE_R(p)                                                                                                     \
    do {                                                                                                               \
        Vector t = reduce(r##p, R##p);                                                                                  \
        t = _mm256_min_epu32(t, _mm256_sub_epi32(t, M2));                                                              \
        _mm256_store_si256(reinterpret_cast<Vector*>(c + (i + (p)) * BLK + j), shrink(t));                             \
    } while (0)

    for (idt i = 0; i < BLK; i += 8) {
        for (idt j = 0; j < BLK; j += 8) {
            INIT_R(0) INIT_R(1) INIT_R(2) INIT_R(3) INIT_R(4) INIT_R(5) INIT_R(6) INIT_R(7)
            for (idt k = 0; k < BLK; k += 8) {
                for (idt w = k; w < k + 8; ++w) {
                    const Vector z0 = _mm256_load_si256(reinterpret_cast<const Vector*>(b + w * BLK + j));
                    const Vector Z0 = _mm256_srli_epi64(z0, 32);
                    WORK_R(0);
                    WORK_R(1);
                    WORK_R(2);
                    WORK_R(3);
                    WORK_R(4);
                    WORK_R(5);
                    WORK_R(6);
                    WORK_R(7);
                }
                SHRK_R(0);
                SHRK_R(1);
                SHRK_R(2);
                SHRK_R(3);
                SHRK_R(4);
                SHRK_R(5);
                SHRK_R(6);
                SHRK_R(7);
            }
            STORE_R(0);
            STORE_R(1);
            STORE_R(2);
            STORE_R(3);
            STORE_R(4);
            STORE_R(5);
            STORE_R(6);
            STORE_R(7);
        }
    }
#undef INIT_R
#undef WORK_R
#undef SHRK_R
#undef STORE_R
}

void place(idt x, idt y, idt n, idt N, const Scalar* __restrict__ a, Scalar* __restrict__ A) {
    if (n == BLK) {
        for (idt i = 0; i < BLK; ++i)
            std::memcpy(A + i * BLK, a + (y + i) * N + x, BLK * sizeof(Scalar));
        return;
    }
    const idt nn = n / 2, D = nn * nn;
    place(x, y, nn, N, a, A);
    place(x + nn, y, nn, N, a, A + D);
    place(x, y + nn, nn, N, a, A + D * 2);
    place(x + nn, y + nn, nn, N, a, A + D * 3);
}

void antiplace(idt x, idt y, idt n, idt N, Scalar* __restrict__ a, const Scalar* __restrict__ A) {
    if (n == BLK) {
        const Vector R2x8 = vr2();
        for (idt i = 0; i < BLK; ++i) {
            for (idt j = 0; j < BLK; j += VLEN) {
                Vector t = mul_mont(_mm256_load_si256(reinterpret_cast<const Vector*>(A + i * BLK + j)), R2x8);
                _mm256_store_si256(reinterpret_cast<Vector*>(a + (y + i) * N + x + j), shrink(t));
            }
        }
        return;
    }
    const idt nn = n / 2, D = nn * nn;
    antiplace(x, y, nn, N, a, A);
    antiplace(x + nn, y, nn, N, a, A + D);
    antiplace(x, y + nn, nn, N, a, A + D * 2);
    antiplace(x + nn, y + nn, nn, N, a, A + D * 3);
}

[[gnu::always_inline]] inline void vadd(Scalar* __restrict__ d, const Scalar* __restrict__ s0,
                                        const Scalar* __restrict__ s1, idt n) {
    idt i = 0;
    for (; i + 2 * VLEN <= n; i += 2 * VLEN) {
        Vector a0 = _mm256_load_si256(reinterpret_cast<const Vector*>(s0 + i));
        Vector b0 = _mm256_load_si256(reinterpret_cast<const Vector*>(s1 + i));
        Vector a1 = _mm256_load_si256(reinterpret_cast<const Vector*>(s0 + i + VLEN));
        Vector b1 = _mm256_load_si256(reinterpret_cast<const Vector*>(s1 + i + VLEN));
        _mm256_store_si256(reinterpret_cast<Vector*>(d + i), shrink(_mm256_add_epi32(a0, b0)));
        _mm256_store_si256(reinterpret_cast<Vector*>(d + i + VLEN), shrink(_mm256_add_epi32(a1, b1)));
    }
    for (; i < n; i += VLEN) {
        Vector t = shrink(_mm256_add_epi32(_mm256_load_si256(reinterpret_cast<const Vector*>(s0 + i)),
                                           _mm256_load_si256(reinterpret_cast<const Vector*>(s1 + i))));
        _mm256_store_si256(reinterpret_cast<Vector*>(d + i), t);
    }
}

[[gnu::always_inline]] inline void vsub(Scalar* __restrict__ d, const Scalar* __restrict__ s0,
                                        const Scalar* __restrict__ s1, idt n) {
    idt i = 0;
    for (; i + 2 * VLEN <= n; i += 2 * VLEN) {
        Vector a0 = _mm256_load_si256(reinterpret_cast<const Vector*>(s0 + i));
        Vector b0 = _mm256_load_si256(reinterpret_cast<const Vector*>(s1 + i));
        Vector a1 = _mm256_load_si256(reinterpret_cast<const Vector*>(s0 + i + VLEN));
        Vector b1 = _mm256_load_si256(reinterpret_cast<const Vector*>(s1 + i + VLEN));
        _mm256_store_si256(reinterpret_cast<Vector*>(d + i), dilate(_mm256_sub_epi32(a0, b0)));
        _mm256_store_si256(reinterpret_cast<Vector*>(d + i + VLEN), dilate(_mm256_sub_epi32(a1, b1)));
    }
    for (; i < n; i += VLEN) {
        Vector t = dilate(_mm256_sub_epi32(_mm256_load_si256(reinterpret_cast<const Vector*>(s0 + i)),
                                           _mm256_load_si256(reinterpret_cast<const Vector*>(s1 + i))));
        _mm256_store_si256(reinterpret_cast<Vector*>(d + i), t);
    }
}

// aa/bb 指向当前层临时区（位于矩阵缓冲末尾），cc 使用 c+n*n
void strassen(Scalar* __restrict__ a, Scalar* __restrict__ b, Scalar* __restrict__ c, idt n, Scalar* __restrict__ aa,
              Scalar* __restrict__ bb) {
    if (n == BLK) {
        kernel(a, b, c);
        return;
    }
    const idt nn = n / 2, D = nn * nn;
    Scalar* __restrict__ cc = c + n * n;
    const idt i00 = 0, i01 = D, i10 = D * 2, i11 = D * 3;

    vsub(aa, a + i01, a + i11, D);
    vadd(bb, b + i10, b + i11, D);
    strassen(aa, bb, c + i00, nn, aa + D, bb + D);

    vadd(aa, a + i00, a + i01, D);
    strassen(aa, b + i11, c + i01, nn, aa + D, bb + D);
    vsub(c + i00, c + i00, c + i01, D);

    vsub(bb, b + i10, b + i00, D);
    strassen(a + i11, bb, c + i10, nn, aa + D, bb + D);
    vadd(c + i00, c + i00, c + i10, D);

    vsub(aa, a + i10, a + i00, D);
    vadd(bb, b + i00, b + i01, D);
    strassen(aa, bb, c + i11, nn, aa + D, bb + D);

    vadd(aa, a + i10, a + i11, D);
    strassen(aa, b + i00, cc, nn, aa + D, bb + D);
    vadd(c + i10, c + i10, cc, D);
    vsub(c + i11, c + i11, cc, D);

    vadd(aa, a + i00, a + i11, D);
    vadd(bb, b + i00, b + i11, D);
    strassen(aa, bb, cc, nn, aa + D, bb + D);
    vadd(c + i00, c + i00, cc, D);
    vadd(c + i11, c + i11, cc, D);

    vsub(bb, b + i01, b + i11, D);
    strassen(a + i00, bb, cc, nn, aa + D, bb + D);
    vadd(c + i01, c + i01, cc, D);
    vadd(c + i11, c + i11, cc, D);
}

void mul(const Scalar* __restrict__ a, const Scalar* __restrict__ b, Scalar* __restrict__ c, idt N, Scalar* __restrict__ A,
         Scalar* __restrict__ B, Scalar* __restrict__ C) {
    place(0, 0, N, N, a, A);
    place(0, 0, N, N, b, B);
    strassen(A, B, C, N, A + N * N, B + N * N);
    antiplace(0, 0, N, N, c, C);
}
} // namespace mat

auto main() -> int {
    using mat::Scalar;
    using mat::Vector;
    using mat::BLK;

    unsigned u, v, w;
    cin >> u >> v >> w;

    const auto n = std::max<mat::idt>(BLK, std::bit_ceil(std::max({u, v, w})));
    const auto align = std::align_val_t(alignof(Vector));
    const auto bytes = n * n * sizeof(Scalar);
    // 矩阵 + Strassen 临时：约 4/3 N^2（与 matrix.cpp 相同布局）
    const auto buf_bytes = (bytes / 3) * 4 + 64 * sizeof(Scalar);

    auto A = static_cast<Scalar*>(::operator new(bytes, align));
    auto B = static_cast<Scalar*>(::operator new(bytes, align));
    auto C = static_cast<Scalar*>(::operator new(bytes, align));
    auto oA = static_cast<Scalar*>(::operator new(buf_bytes, align));
    auto oB = static_cast<Scalar*>(::operator new(buf_bytes, align));
    auto oC = static_cast<Scalar*>(::operator new(buf_bytes, align));

    std::memset(A, 0, bytes);
    std::memset(B, 0, bytes);

    for (auto i = 0U; i != u; ++i)
        for (auto j = 0U; j != v; ++j)
            cin >> A[i * n + j];
    for (auto i = 0U; i != v; ++i)
        for (auto j = 0U; j != w; ++j)
            cin >> B[i * n + j];

    const auto begin = std::clock();
    mat::mul(A, B, C, n, oA, oB, oC);
    const auto end = std::clock();

    for (auto i = 0U; i != u; ++i)
        for (auto j = 0U; j != w; ++j)
            cout << C[i * n + j] << " \n"[j + 1 == w];
    std::clog << "Duration = " << end - begin << " clocks\n";
}