#include <bits/stdc++.h>
#include <immintrin.h>
#include <sys/mman.h>
#include <sys/stat.h>
using namespace std;
#define problem ""
#define multitest 0
#define debug(x) cerr << #x << " = " << x << endl;
// io from https://judge.yosupo.jp/submission/142782

namespace __io
{
using u32 = uint32_t;
using u64 = uint64_t;

namespace QIO_base
{
constexpr int O_buffer_default_size = 1 << 18;
constexpr int O_buffer_default_flush_threshold = 40;
struct _int_to_char_tab
{
    char tab[40000];
    constexpr _int_to_char_tab() : tab()
    {
        for (int i = 0; i != 10000; ++i)
        {
            for (int j = 3, n = i; ~j; --j)
            {
                tab[i * 4 + j] = n % 10 + 48, n /= 10;
            }
        }
    }
} constexpr _otab;
} // namespace QIO_base
namespace QIO_I
{
using namespace QIO_base;
struct Qinf
{
    FILE *f;
    char *bg, *ed, *p;
    struct stat Fl;
    Qinf(FILE *fi) : f(fi)
    {
        int fd = fileno(f);
        fstat(fd, &Fl);
        bg = (char *)mmap(0, Fl.st_size + 1, PROT_READ, MAP_PRIVATE, fd, 0);
        p = bg, ed = bg + Fl.st_size;
        madvise(p, Fl.st_size + 1, MADV_SEQUENTIAL);
    }
    ~Qinf()
    {
        munmap(bg, Fl.st_size + 1);
    }
    void skip_space()
    {
        while (*p <= ' ')
        {
            ++p;
        }
    }
    char get()
    {
        return *p++;
    }
    char seek()
    {
        return *p;
    }
    bool eof()
    {
        return p == ed;
    }
    Qinf &read(char *s, size_t count)
    {
        return memcpy(s, p, count), p += count, *this;
    }
    Qinf &operator>>(u32 &x)
    {
        skip_space(), x = 0;
        for (; *p > ' '; ++p)
        {
            x = x * 10 + (*p & 0xf);
        }
        return *this;
    }
    Qinf &operator>>(int &x)
    {
        skip_space();
        if (*p == '-')
        {
            for (++p, x = 48 - *p++; *p > ' '; ++p)
            {
                x = x * 10 - (*p ^ 48);
            }
        }
        else
        {
            for (x = *p++ ^ 48; *p > ' '; ++p)
            {
                x = x * 10 + (*p ^ 48);
            }
        }
        return *this;
    }
} qin(stdin);
} // namespace QIO_I
namespace QIO_O
{
using namespace QIO_base;
struct Qoutf
{
    FILE *f;
    char *bg, *ed, *p;
    char *ed_thre;
    int fp;
    u64 _fpi;
    Qoutf(FILE *fo, size_t sz = O_buffer_default_size)
        : f(fo), bg(new char[sz]), ed(bg + sz), p(bg), ed_thre(ed - O_buffer_default_flush_threshold), fp(6),
          _fpi(1000000ull)
    {
    }
    void flush()
    {
        fwrite_unlocked(bg, 1, p - bg, f), p = bg;
    }
    void chk()
    {
        if (__builtin_expect(p > ed_thre, 0))
        {
            flush();
        }
    }
    ~Qoutf()
    {
        flush();
        delete[] bg;
    }
    void put4(u32 x)
    {
        if (x > 99u)
        {
            if (x > 999u)
            {
                memcpy(p, _otab.tab + (x << 2) + 0, 4), p += 4;
            }
            else
            {
                memcpy(p, _otab.tab + (x << 2) + 1, 3), p += 3;
            }
        }
        else
        {
            if (x > 9u)
            {
                memcpy(p, _otab.tab + (x << 2) + 2, 2), p += 2;
            }
            else
            {
                *p++ = x ^ 48;
            }
        }
    }
    void put2(u32 x)
    {
        if (x > 9u)
        {
            memcpy(p, _otab.tab + (x << 2) + 2, 2), p += 2;
        }
        else
        {
            *p++ = x ^ 48;
        }
    }
    Qoutf &write(const char *s, size_t count)
    {
        if (count > 1024 || p + count > ed_thre)
        {
            flush(), fwrite_unlocked(s, 1, count, f);
        }
        else
        {
            memcpy(p, s, count), p += count, chk();
        }
        return *this;
    }
    Qoutf &operator<<(char ch)
    {
        return *p++ = ch, *this;
    }
    Qoutf &operator<<(u32 x)
    {
        if (x > 99999999u)
        {
            put2(x / 100000000u), x %= 100000000u;
            memcpy(p, _otab.tab + ((x / 10000u) << 2), 4), p += 4;
            memcpy(p, _otab.tab + ((x % 10000u) << 2), 4), p += 4;
        }
        else if (x > 9999u)
        {
            put4(x / 10000u);
            memcpy(p, _otab.tab + ((x % 10000u) << 2), 4), p += 4;
        }
        else
        {
            put4(x);
        }
        return chk(), *this;
    }
    Qoutf &operator<<(int x)
    {
        if (x < 0)
        {
            *p++ = '-', x = -x;
        }
        return *this << static_cast<u32>(x);
    }
} qout(stdout);
} // namespace QIO_O
namespace QIO
{
using QIO_I::qin;
using QIO_I::Qinf;
using QIO_O::qout;
using QIO_O::Qoutf;
} // namespace QIO
using namespace QIO;
}; // namespace __io
using namespace __io;
void init()
{
}
uint32_t x[1048576];
uint32_t y[1048576];
uint32_t z[1048576];
constexpr int NN = 524288;
alignas(32) uint32_t a[__lg(NN) + 1][NN / 2];
alignas(32) uint32_t b[__lg(NN) + 1][NN / 2];
alignas(32) uint64_t c[NN / 2];
const uint64_t MM8 = 8LL * 998244353 * 998244353;
const uint32_t MOD = 998244353;
#pragma GCC optimize("O3,unroll-loops")
template <int N> void xt(uint32_t *m)
{
    if (N > 1)
    {
        xt<N / 2>(m);
        xt<N / 2>(m + N / 2);
        for (int i = 0; i < N / 2; i++)
        {
            uint32_t a = m[i];
            m[i] = min(m[i] + m[i + N / 2], m[i] + m[i + N / 2] - MOD);
            m[i + N / 2] = min(a - m[i + N / 2], a - m[i + N / 2] + MOD);
        }
    }
}
template <int N> void xt(uint64_t *m)
{
    if (N > 1)
    {
        xt<N / 2>(m);
        xt<N / 2>(m + N / 2);
        for (int i = 0; i < N / 2; i++)
        {
            uint64_t a = m[i];
            m[i] = min(m[i] + m[i + N / 2], m[i] + m[i + N / 2] - MM8);
            m[i + N / 2] = min(a - m[i + N / 2], a - m[i + N / 2] + MM8);
        }
    }
}
template <int N> void compute_xor_single(uint32_t *__restrict__ X, uint32_t *__restrict__ Y, uint32_t *__restrict__ Z)
{
    for (int i = 0; i <= __lg(N) + 1; i++)
        for (int k = 0; k < N; k++)
            a[i][k] = b[i][k] = 0;
    for (int i = 0; i < N * 2; i++)
        a[__builtin_popcount(i)][i / 2] = X[i];
    for (int i = 0; i < N * 2; i++)
        b[__builtin_popcount(i)][i / 2] = Y[i];
    for (int i = 0; i <= __lg(N) + 1; i++)
        xt<N>(a[i]);
    for (int i = 0; i <= __lg(N) + 1; i++)
        xt<N>(b[i]);
    for (int i = 0; i <= __lg(N) + 1; i++)
    {
        for (int k = 0; k < N; k++)
            c[k] = 0;
        for (int ii = 0; ii < N; ii += 1024)
            for (int j = 0; j <= i; j++)
            {
                for (int k = ii; k < ii + 1024; k += 4)
                {
                    // c[i][k] += (uint64_t)a[j][k] * b[i - j][k];
                    __m256i va = _mm256_cvtepu32_epi64(_mm_load_si128((__m128i *)(a[j] + k)));
                    __m256i vb = _mm256_cvtepu32_epi64(_mm_load_si128((__m128i *)(b[i - j] + k)));
                    _mm256_store_si256((__m256i *)(c + k), *(__m256i *)(c + k) + _mm256_mul_epi32(va, vb));
                }
                if (j % 8 == 7)
                    for (int k = ii; k < ii + 1024; k++)
                        c[k] = min(c[k], c[k] - MM8);
            }
        for (int k = 0; k < N; k++)
            c[k] = min(c[k], c[k] - MM8);
        xt<N>(c);
        for (int k = 0; k < 2 * N; k++)
            if (__builtin_popcount(k) == i)
                Z[k] += (c[k / 2] % MOD * 998240545 % MOD), Z[k] = min(Z[k], Z[k] - MOD);
    }
}
template <int N> void brute(uint32_t *__restrict__ X, uint32_t *__restrict__ Y, uint32_t *__restrict__ Z)
{
    if (N > NN)
    {
        brute<N / 2>(X, Y, Z);
        brute<N / 2>(X + N / 2, Y, Z + N / 2);
        brute<N / 2>(X, Y + N / 2, Z + N / 2);
    }
    else
    {
        compute_xor_single<N / 2>(X, Y, Z);
    }
}
void Yoshi()
{
    int A;
    qin >> A;
    for (int i = 0; i < (1 << A); i++)
        qin >> x[i];
    for (int i = 0; i < (1 << A); i++)
        qin >> y[i];
    brute<1048576>(x, y, z);
    for (int i = 0; i < (1 << A); i++)
        qout << z[i] << ' ';
}
signed main()
{
#ifndef yoshi_likes_e4
    ios::sync_with_stdio(0);
    cin.tie(0);
    if (fopen(problem ".inp", "r"))
    {
        freopen(problem ".inp", "r", stdin);
        freopen(problem ".out", "w", stdout);
    }
#endif
    init();
    int t = 1;
#if multitest
    cin >> t;
#endif
    while (t--)
        Yoshi();
}