// copied from https://judge.yosupo.jp/submission/346278
// fastest solution without Frobenius form thus far
// modification: window trick for less matrix multiplications

// @brief Matrix multiplication
#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"
#pragma GCC optimize("Ofast,unroll-loops")
#include <chrono>
#include <cstring>
#include <immintrin.h>
#include <iostream>
#include <type_traits>
#include <vector>
using namespace std;
namespace fastio
{
static constexpr size_t buf_size = 1 << 19;
static constexpr size_t integer_size = 20;
static constexpr size_t block_size = 10000;

static char inbuf[buf_size + 1] = {};
static char outbuf[buf_size + 1] = {};
static char block_str[block_size * 4 + 1] = {};

static constexpr uint64_t power10[] = {1,
                                       10,
                                       100,
                                       1000,
                                       10000,
                                       100000,
                                       1000000,
                                       10000000,
                                       100000000,
                                       1000000000,
                                       10000000000,
                                       100000000000,
                                       1000000000000,
                                       10000000000000,
                                       100000000000000,
                                       1000000000000000,
                                       10000000000000000,
                                       100000000000000000,
                                       1000000000000000000,
                                       10000000000000000000u};

struct Scanner
{
  private:
    size_t pos, end;

    void load()
    {
        end = fread(inbuf, 1, buf_size, stdin);
        inbuf[end] = '\0';
    }
    void reload()
    {
        size_t len = end - pos;
        memmove(inbuf, inbuf + pos, len);
        end = len + fread(inbuf + len, 1, buf_size - len, stdin);
        inbuf[end] = '\0';
        pos = 0;
    }
    void skip_space()
    {
        while (inbuf[pos] <= ' ')
        {
            if (__builtin_expect(++pos == end, 0))
                reload();
        }
    }
    char get_next()
    {
        return inbuf[pos++];
    }
    char get_next_nonspace()
    {
        skip_space();
        return inbuf[pos++];
    }

  public:
    Scanner()
    {
        load();
    }

    void scan(char &c)
    {
        c = get_next_nonspace();
    }
    void scan(std::string &s)
    {
        skip_space();
        s = "";
        do
        {
            size_t start = pos;
            while (inbuf[pos] > ' ')
                pos++;
            s += std::string(inbuf + start, inbuf + pos);
            if (inbuf[pos] != '\0')
                break;
            reload();
        } while (true);
    }

    template <class T> typename std::enable_if<std::is_integral<T>::value, void>::type scan(T &x)
    {
        char c = get_next_nonspace();
        if (__builtin_expect(pos + integer_size >= end, 0))
            reload();
        bool neg = false;
        if (c == '-')
            neg = true, x = 0;
        else
            x = c & 15;
        while ((c = get_next()) >= '0')
            x = x * 10 + (c & 15);
        if (neg)
            x = -x;
    }

    template <class Head, class... Others> void scan(Head &head, Others &...others)
    {
        scan(head);
        scan(others...);
    }

    template <class T> Scanner &operator>>(T &x)
    {
        scan(x);
        return *this;
    }
};

struct Printer
{
  private:
    size_t pos = 0;

    void flush()
    {
        fwrite(outbuf, 1, pos, stdout);
        pos = 0;
    }

    void pre_calc()
    {
        for (size_t i = 0; i < block_size; i++)
        {
            size_t j = 4, k = i;
            while (j--)
            {
                block_str[i * 4 + j] = k % 10 + '0';
                k /= 10;
            }
        }
    }

    static constexpr size_t get_integer_size(uint64_t n)
    {
        if (n >= power10[10])
        {
            if (n >= power10[19])
                return 20;
            if (n >= power10[18])
                return 19;
            if (n >= power10[17])
                return 18;
            if (n >= power10[16])
                return 17;
            if (n >= power10[15])
                return 16;
            if (n >= power10[14])
                return 15;
            if (n >= power10[13])
                return 14;
            if (n >= power10[12])
                return 13;
            if (n >= power10[11])
                return 12;
            return 11;
        }
        else
        {
            if (n >= power10[9])
                return 10;
            if (n >= power10[8])
                return 9;
            if (n >= power10[7])
                return 8;
            if (n >= power10[6])
                return 7;
            if (n >= power10[5])
                return 6;
            if (n >= power10[4])
                return 5;
            if (n >= power10[3])
                return 4;
            if (n >= power10[2])
                return 3;
            if (n >= power10[1])
                return 2;
            return 1;
        }
    }

  public:
    Printer()
    {
        pre_calc();
    }
    ~Printer()
    {
        flush();
    }

    void print(char c)
    {
        outbuf[pos++] = c;
        if (__builtin_expect(pos == buf_size, 0))
            flush();
    }
    void print(const char *s)
    {
        while (*s != 0)
        {
            outbuf[pos++] = *s++;
            // if (pos == buf_size) flush();
            if (__builtin_expect(pos == buf_size, 0))
                flush();
        }
    }
    void print(const std::string &s)
    {
        for (auto c : s)
        {
            outbuf[pos++] = c;
            // if (pos == buf_size) flush();
            if (__builtin_expect(pos == buf_size, 0))
                flush();
        }
    }

    template <class T> typename std::enable_if<std::is_integral<T>::value, void>::type print(T x)
    {
        if (__builtin_expect(pos + integer_size >= buf_size, 0))
            flush();
        if (x < 0)
            print('-'), x = -x;
        size_t digit = get_integer_size(x);
        size_t len = digit;
        while (len >= 4)
        {
            len -= 4;
            memcpy(outbuf + pos + len, block_str + (x % block_size) * 4, 4);
            x /= block_size;
        }
        memcpy(outbuf + pos, block_str + x * 4 + (4 - len), len);
        pos += digit;
    }

    template <class Head, class... Others> void print(const Head &head, const Others &...others)
    {
        print(head);
        print(' ');
        print(others...);
    }

    template <class... Args> void println(const Args &...args)
    {
        print(args...);
        print('\n');
    }

    template <class T> Printer &operator<<(const T &x)
    {
        print(x);
        return *this;
    }
};
}; // namespace fastio

fastio::Scanner fin;
fastio::Printer fout;
#include <cassert>
using namespace std;
const int SZ = 224;
const int s1 = 32, s3 = 32, NAIVE = 32;
chrono::high_resolution_clock Clock;
int64_t modmod8 = 8ULL * 998244353 * 998244353;
alignas(32) unsigned A1[SZ * SZ], A2[SZ * SZ], A3[SZ * SZ], A5[SZ * SZ], A7[SZ * SZ], B[SZ * SZ];
alignas(32) uint64_t PROD[SZ * SZ];
alignas(32) uint64_t bb[NAIVE * NAIVE];
alignas(32) __m256i tmp[NAIVE * NAIVE / 4], *Bv = (__m256i *)bb;
const __m256i MM8 = {modmod8, modmod8, modmod8, modmod8};
static inline __attribute__((always_inline)) __m256i shrink(__m256i a)
{
    __m256i mask = _mm256_cmpgt_epi64(a - MM8, __m256i{});
    return a - (MM8 & mask);
}
#define LOAD(x) t##x = _mm256_load_si256((__m256i *)(c + ((I_TILE * NAIVE + i) * l + K_TILE * NAIVE + i3) + x * 4))
#define MADD(x) t##x = _mm256_add_epi64(t##x, _mm256_mul_epu32(aik, tmp[(i3 / s3) * 8 * NAIVE + 8 * k + x]))
#define STOR(x) _mm256_store_si256((__m256i *)(c + ((I_TILE * NAIVE + i) * l + K_TILE * NAIVE + i3) + x * 4), t##x)
#define TMP(x) tmp[(i3 / s3) * 8 * NAIVE + 8 * k + x] = Bv[k * NAIVE / 4 + i3 / 4 + x]

void write_b_to_tmp(const unsigned *b, int J_TILE, int K_TILE, int k)
{
    for (int i = 0; i < NAIVE; i++)
        for (int j = 0; j < NAIVE; j++)
            bb[i * NAIVE + j] = b[(J_TILE * NAIVE + i) * k + K_TILE * NAIVE + j];
    for (int i3 = 0; i3 < NAIVE; i3 += s3)
        for (int k = 0; k < NAIVE; k++)
            TMP(0), TMP(1), TMP(2), TMP(3), TMP(4), TMP(5), TMP(6), TMP(7);
}
void avx2_emm_tile(const unsigned *a, uint64_t *c, int I_TILE, int J_TILE, int K_TILE, int n, int l)
{
    for (int i3 = 0; i3 < NAIVE; i3 += s3)
        for (int i1 = 0; i1 < NAIVE; i1 += s1)
        {
            for (int i = 0; i < NAIVE; i++)
            {
                __m256i t0, t1, t2, t3, t4, t5, t6, t7;
                LOAD(0), LOAD(1), LOAD(2), LOAD(3);
                LOAD(4), LOAD(5), LOAD(6), LOAD(7);
                for (int k = i1; k < i1 + s1; k++)
                {
                    if (k % 8 == 0)
                    {
                        t0 = shrink(t0), t1 = shrink(t1), t2 = shrink(t2), t3 = shrink(t3), t4 = shrink(t4),
                        t5 = shrink(t5), t6 = shrink(t6), t7 = shrink(t7);
                    }
                    __m256i aik = _mm256_set1_epi32(a[(I_TILE * NAIVE + i) * n + J_TILE * NAIVE + k]);
                    MADD(0), MADD(1), MADD(2), MADD(3);
                    MADD(4), MADD(5), MADD(6), MADD(7);
                }
                STOR(0), STOR(1), STOR(2), STOR(3);
                STOR(4), STOR(5), STOR(6), STOR(7);
            }
        }
}
void emm(const unsigned *a, const unsigned *b, uint64_t *c, int m = SZ, int n = SZ, int k = SZ)
{
    for (int J_TILE = 0; J_TILE * NAIVE < n; J_TILE++)
        for (int K_TILE = 0; K_TILE * NAIVE < k; K_TILE++)
        {
            write_b_to_tmp(b, J_TILE, K_TILE, k);
            for (int I_TILE = 0; I_TILE * NAIVE < m; I_TILE++)
                avx2_emm_tile(a, c, I_TILE, J_TILE, K_TILE, n, k);
        }
    for (int i = 0; i < m * k; i++)
        c[i] %= 998244353;
}
signed main()
{
    int n;
    long long k;
    fin >> n >> k;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            fin >> A1[i * SZ + j];
#define MAT_MUL_EQ(A, B, C) emm(B, C, PROD); for (int i = 0; i < SZ * SZ; i++) A[i] = PROD[i], PROD[i] = 0;
#define EQ(A, B) for (int i = 0; i < SZ * SZ; i++) A[i] = B[i];
    MAT_MUL_EQ(A2, A1, A1)
    MAT_MUL_EQ(A3, A1, A2)
    MAT_MUL_EQ(A5, A2, A3)
    MAT_MUL_EQ(A7, A2, A5)
    auto wpow = [&](auto self, long long exp) {
        if (exp < 16) {
            switch (exp) {
                case 0: for (int i = 0; i < SZ; ++i) B[i * SZ + i] = 1; break;
                case 1: EQ(B, A1) break;
                case 2: EQ(B, A2) break;
                case 3: EQ(B, A3) break;
                case 4: MAT_MUL_EQ(B, A1, A3) break;
                case 5: EQ(B, A5) break;
                case 6: MAT_MUL_EQ(B, A1, A5) break;
                case 7: EQ(B, A7) break;
                case 8: MAT_MUL_EQ(B, A1, A7) break;
                case 9: MAT_MUL_EQ(B, A2, A7) break;
                case 10: MAT_MUL_EQ(B, A5, A5) break;
                case 11: MAT_MUL_EQ(B, A1, A3) MAT_MUL_EQ(B, B, A7) break;
                case 12: MAT_MUL_EQ(B, A5, A7) break;
                case 13: MAT_MUL_EQ(B, A1, A5) MAT_MUL_EQ(B, B, A7) break;
                case 14: MAT_MUL_EQ(B, A7, A7) break;
                case 15: MAT_MUL_EQ(B, A1, A7) MAT_MUL_EQ(B, B, A7) break;
            }
            return;
        }
        switch (exp & 7) {
            case 0: case 2: case 4: case 6:
            self(self, exp / 2); MAT_MUL_EQ(B, B, B); break;
            case 1: self(self, exp - 1); MAT_MUL_EQ(B, B, A1) break;
            case 3: self(self, exp - 3); MAT_MUL_EQ(B, B, A3) break;
            case 5: self(self, exp - 5); MAT_MUL_EQ(B, B, A5) break;
            case 7: self(self, exp - 7); MAT_MUL_EQ(B, B, A7) break;
        }
    };
    wpow(wpow, k);
#undef MAT_MUL_EQ
#undef EQ
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            fout << B[i * SZ + j];
            fout << (j == n - 1 ? '\n' : ' ');
        }
}
