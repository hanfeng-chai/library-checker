#pragma GCC optimize("O3,unroll-loops")
#include <chrono>
#include <immintrin.h>
#include <iostream>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;
#define endl '\n'
#define debug(x) cerr << #x << " = " << x << endl;
alignas(32) long long a[4096][64];
alignas(32) long long b[4096][64];
alignas(32) long long c[4096][64];
alignas(32) long long TMP[256][64];
const __m256i add = {0x4f4f4f4f4f4f4f4fLL, 0x4f4f4f4f4f4f4f4fLL, 0x4f4f4f4f4f4f4f4fLL, 0x4f4f4f4f4f4f4f4fLL};
uint32_t read_32_bit_from_str(const char *str)
{
    __m256i tmp = _mm256_loadu_si256((__m256i *)str);
    return _mm256_movemask_epi8(tmp + add);
}
char *buf1;
inline namespace Input
{
int pos;
char next_char()
{
    return buf1[pos++];
}
int read_int()
{
    int x;
    char ch;
    int sgn = 1;
    while (!isdigit(ch = next_char()))
    {
        if (ch == '-')
        {
            sgn *= -1;
        }
    }
    x = ch - '0';
    while (isdigit(ch = next_char()))
    {
        x = x * 10 + (ch - '0');
    }
    return x * sgn;
}
}; // namespace Input
int main()
{
    struct stat fs;
    fstat(0, &fs);
    buf1 = (char *)mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    if (MAP_FAILED == buf1)
    {
        fprintf(stderr, "mmap(): error '%m' (%d)\n", errno);
        return 1;
    }
    chrono::high_resolution_clock Clock;
    auto t1 = Clock.now();
    int n, m, k;
    n = read_int(), m = read_int(), k = read_int();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m / 32; j++)
        {
            a[i][j >> 1] |= (uint64_t)read_32_bit_from_str(buf1 + pos) << ((j & 1) * 32);
            pos += 32;
        }
        int c = (m / 32) * 32;
        for (int j = c; j < m; j++)
        {
            a[i][j >> 6] |= uint64_t(buf1[pos] - '0') << (j & 63);
            pos++;
        }
        pos++;
    }
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k / 32; j++)
        {
            b[i][j >> 1] |= (uint64_t)read_32_bit_from_str(buf1 + pos) << ((j & 1) * 32);
            pos += 32;
        }
        int c = (k / 32) * 32;
        for (int j = c; j < k; j++)
        {
            b[i][j >> 6] |= uint64_t(buf1[pos] - '0') << (j & 63);
            pos++;
        }
        pos++;
    }
    cerr << "Input time: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t1).count() << " ms" << endl;
    t1 = Clock.now();
    for (int j = 0; j < 4096; j += 8)
    {
        for (int bit = 0; bit < 8; bit++)
            for (int cur = 0; cur < (1 << bit); cur++)
                for (int k = 0; k < 64; k++)
                    TMP[cur + (1 << bit)][k] = TMP[cur][k] ^ b[j + bit][k];
        for (int i = 0; i < 4096; i += 256)
            for (int X = 0; X < 256; X++)
            {
                int mask = (a[i + X][j >> 6] >> (j & 63)) & 255;
                if (mask)
                    for (int k = 0; k < 64; k++)
                        c[i + X][k] ^= TMP[mask][k];
            }
    }
    cerr << "Compute time: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t1).count() << " ms" << endl;
    t1 = Clock.now();
    char buf[k + 1];
    buf[k] = '\n';
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
            buf[j] = ((c[i][j >> 6] >> (j & 63)) & 1) + '0';
        write(1, buf, k + 1);
    }
    cerr << "Output time: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t1).count() << " ms" << endl;
}