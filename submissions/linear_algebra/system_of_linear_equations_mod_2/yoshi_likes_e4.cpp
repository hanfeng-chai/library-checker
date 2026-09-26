#include <bitset>
#include <chrono>
#include <immintrin.h>
#include <iostream>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#pragma GCC optimize("Ofast,unroll-loops")
using namespace std;
#ifndef yoshi_likes_e4
#define endl '\n'
#endif
#define problem ""
#define multitest 0
#define debug(x) cerr << #x << " = " << x << endl;
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
bool rd_bool()
{
    char c = next_char();
    while ((c != '0') && (c != '1'))
        c = next_char();
    return c ^ '0';
}
}; // namespace Input
const int SZ = 4097;
chrono::high_resolution_clock Clock;
vector<bitset<SZ>> RREF(vector<bitset<SZ>> x, int N, int M)
{
    auto t1 = Clock.now();
    int h = 0, k = 0;
    while (h < N && k < M)
    {
        // Any non-zero pivot row works
        int idx = h;
        while (idx < x.size() && x[idx][k] == 0)
            idx++;
        if (idx != x.size())
        {
            swap(x[h], x[idx]);
            for (int i = h + 1; i < x.size(); i++)
                if (x[i][k])
                    x[i] ^= x[h];
            h++;
        }
        k++;
    }
    cerr << "rref time: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t1).count() << " ms" << endl;
    return x;
}
void init()
{
}
typedef array<long, 65> AsArray;
void Yoshi()
{
    struct stat fs;
    fstat(0, &fs);
    buf1 = (char *)mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    if (MAP_FAILED == buf1)
    {
        fprintf(stderr, "mmap(): error '%m' (%d)\n", errno);
        exit(1);
    }
    int N, M;
    N = read_int(), M = read_int();
    auto t0 = Clock.now();
    vector<bitset<SZ>> raw(max(N, M));
    for (int i = 0; i < N; i++)
    {
        array<long, 65> &a = *reinterpret_cast<AsArray *>(&raw[i]);
        for (int j = 0; j < M / 32; j++)
        {
            a[j >> 1] |= (uint64_t)read_32_bit_from_str(buf1 + pos) << ((j & 1) * 32);
            pos += 32;
        }
        int c = (M / 32) * 32;
        for (int j = c; j < M; j++)
        {
            a[j >> 6] |= uint64_t(buf1[pos] - '0') << (j & 63);
            pos++;
        }
        pos++;
    }
    for (int i = 0; i < N; i++)
        raw[i][M] = rd_bool();
    cerr << "input time: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t0).count() << " ms" << endl;
    auto rref = RREF(raw, N, M);
    for (int i = 0; i < max(N, M); i++)
        if (rref[i].count() == 1 && rref[i][M])
        {
            cout << -1 << endl;
            return;
        }
    vector<bitset<SZ>> pruned_rref;
    for (int i = 0; i < max(N, M); i++)
        if (rref[i].count())
            pruned_rref.push_back(rref[i]);
    rref = vector<bitset<SZ>>(M);
    for (int i = 0; i < pruned_rref.size(); i++)
    {
        int lead = 0;
        while (lead < M && !pruned_rref[i][lead])
            lead++;
        rref[lead] = pruned_rref[i];
    }
    vector<int> fv, FV(M);
    for (int i = 0; i < M; i++)
        if (!rref[i][i])
        {
            FV[i] = fv.size() + 1;
            fv.push_back(i);
        }
    int R = fv.size();
    int MM = (M + 7) / 8 * 8;
    vector<bitset<SZ>> sols(MM);
    vector<bitset<SZ>> TMP(256);
    for (int first = MM - 8; first >= 0; first -= 8)
    {
        for (int i = min(M - 1, first + 7); i >= first; i--)
        {
            if (FV[i])
                sols[i][FV[i]] = 1;
            else
            {
                sols[i][0] = sols[i][0] ^ rref[i][M];
                for (int j = i + 1; j < min(M, first + 8); j++)
                    if (rref[i][j])
                        sols[i] ^= sols[j];
            }
        }
        for (int bit = 0; bit < 8; bit++)
            for (int cur = 0; cur < (1 << bit); cur++)
                TMP[cur + (1 << bit)] = TMP[cur] ^ sols[first + bit];
        for (int i = 0; i < min(first, M); i++)
        {
            if (!FV[i])
            {
                array<long, 65> &a = *reinterpret_cast<AsArray *>(&rref[i]);
                const int mask = (a[first >> 6] >> (first & 63)) & 255;
                sols[i] ^= TMP[mask];
            }
        }
    }
    // R+1 * M matrix
    string s = to_string(R);
    s += '\n';
    write(1, s.c_str(), s.size());
    char buf[R + 1][M + 1];
    for (int j = 0; j <= R; j++)
        buf[j][M] = '\n';
    for (int i = 0; i < M; i++)
    {
        array<long, 65> &a = *reinterpret_cast<AsArray *>(&sols[i]);
        for (int j = 0; j <= R; j++)
            buf[j][i] = '0' + (bool)(a[j >> 6] & (1ll << (j & 63)));
    }
    for (int j = 0; j <= R; j++)
        write(1, buf[j], M + 1);
}
signed main()
{
#ifndef yoshi_likes_e4
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
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