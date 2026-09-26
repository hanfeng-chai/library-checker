#define NDEBUG
#pragma GCC optimize("Ofast")
#line 1 "/home/chai/workspaces/solutions/sol/data_structure/range_affine_range_sum/main.cpp"
#include <utility>
#line 2 "/home/chai/workspaces/solutions/include/io.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <array>
#include <cstring>
#line 2 "/home/chai/workspaces/solutions/include/common.h"

#include <cstdint>

using sbb = int8_t;
using shh = int16_t;
using sww = int32_t;
using sdd = int64_t;
using sqq = __int128;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using i128 = __int128;

using ubb = uint8_t;
using uhh = uint16_t;
using uww = uint32_t;
using udd = uint64_t;
using uqq = unsigned __int128;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = unsigned __int128;

using fww = float;
using fdd = double;
using fqq = long double;

using f32 = float;
using f64 = double;
using f80 = long double;

#define fun auto
#define def auto
#define let const auto
#line 9 "/home/chai/workspaces/solutions/include/io.h"

struct rd
{
    static constexpr fun all_digit(u32 x)
    {
        x ^= 0x30303030;
        x &= 0xf0f0f0f0;
        return !x;
    }
    static constexpr fun all_digit(u64 x)
    {
        x ^= 0x3030303030303030;
        x &= 0xf0f0f0f0f0f0f0f0;
        return !x;
    }

    char *I;
    rd()
    {
        struct stat st;
        fstat(0, &st);
        I = (char *)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    }

    fun u1() -> u32
    {
        u32 x = *I++ - '0';
        return ++I, x;
    }

    fun ub() -> u32
    {
        u32 x = *I++ - '0';
        for (; *I >= '0'; ++I)
            x = x * 10 + *I - '0';
        return ++I, x;
    }

    fun uh() -> u32
    {
        u32 x{};
        u32 a;
        memcpy(&a, I, sizeof(a));
        if (all_digit(a))
        {
            a ^= 0x30303030;
            a = (a * 10 + (a >> 8)) & 0x00ff00ff;
            a = (a * 100 + (a >> 16)) & 0x0000ffff;
            x = a, I += sizeof(a);
        }
        for (; *I >= '0'; ++I)
            x = x * 10 + *I - '0';
        return ++I, x;
    }

    fun uw() -> u32
    {
        u32 x{};
        u64 a;
        memcpy(&a, I, sizeof(a));
        if (all_digit(a))
        {
            a ^= 0x3030303030303030;
            a = (a * 10 + (a >> 8)) & 0x00ff00ff00ff00ff;
            a = (a * 100 + (a >> 16)) & 0x0000ffff0000ffff;
            a = (a * 10000 + (a >> 32)) & 0x00000000ffffffff;
            x = a, I += sizeof(a);
        }
        for (; *I >= '0'; ++I)
            x = x * 10 + *I - '0';
        return ++I, x;
    }

    fun ud() -> u64
    {
        u64 x{};
        union
        {
            char ch[16];
            u64 d[2];
        };
        memcpy(ch, I, sizeof(ch));
        u64 a = d[0], b = d[1];
        if (all_digit(a) && all_digit(b))
        {
            a ^= 0x3030303030303030;
            b ^= 0x3030303030303030;
            a = (a * 10 + (a >> 8)) & 0x00ff00ff00ff00ff;
            b = (b * 10 + (b >> 8)) & 0x00ff00ff00ff00ff;
            a = (a * 100 + (a >> 16)) & 0x0000ffff0000ffff;
            b = (b * 100 + (b >> 16)) & 0x0000ffff0000ffff;
            a = (a * 10000 + (a >> 32)) & 0x00000000ffffffff;
            b = (b * 10000 + (b >> 32)) & 0x00000000ffffffff;
            x = a * 100000000 + b, I += sizeof(ch);
        }
        for (; *I >= '0'; ++I)
            x = x * 10 + *I - '0';
        return ++I, x;
    }

    fun s1() -> i32 { return *I == '-' ? ++I, -u1() : u1(); }
    fun sb() -> i32 { return *I == '-' ? ++I, -ub() : ub(); }
    fun sh() -> i32 { return *I == '-' ? ++I, -uh() : uh(); }
    fun sw() -> i32 { return *I == '-' ? ++I, -uw() : uw(); }
    fun sd() -> i64 { return *I == '-' ? ++I, -ud() : ud(); }
};

struct wt
{
    static constexpr def LUT_hp = []
    {
        std::array<std::array<char, 4>, 10000> res;
        for (int i = 0; i < 10000; ++i)
        {
            res[i][0] = '0' + i / 1000;
            res[i][1] = '0' + i / 100 % 10;
            res[i][2] = '0' + i / 10 % 10;
            res[i][3] = '0' + i % 10;
            if (i < 1000)
                res[i][0] = ' ';
            if (i < 100)
                res[i][1] = ' ';
            if (i < 10)
                res[i][2] = ' ';
        }
        return res;
    }();
    static constexpr def LUT_lo = []
    {
        std::array<std::array<char, 4>, 10000> res;
        for (int i = 0; i < 10000; ++i)
        {
            res[i][0] = '0' + i / 1000;
            res[i][1] = '0' + i / 100 % 10;
            res[i][2] = '0' + i / 10 % 10;
            res[i][3] = '0' + i % 10;
        }
        return res;
    }();

    inline static char buf[1 << 20];
    static constexpr char *lim = buf + sizeof(buf) - 50;
    char *O = buf;

    ~wt() { flush(); }
    void flush() { write(1, buf, O - buf), O = buf; }
    void print_hp(u64 x) { memcpy(O, &LUT_hp[x], 4), O += 4; }
    void print_lo(u64 x) { memcpy(O, &LUT_lo[x], 4), O += 4; }

    void uw(u32 x)
    {
        if (lim < O)
            flush();
        *O++ = ' ';
        if (x > 9999'9999)
        {
            *O++ = '0' + (x / 10000 / 10000);
            print_lo(x / 10000 % 10000);
            print_lo(x % 10000);
        }
        else if (x > 9999)
        {
            print_hp(x / 10000);
            print_lo(x % 10000);
        }
        else
        {
            print_hp(x);
        }
    }

    void ud(u64 x)
    {
        if (lim < O)
            flush();
        *O++ = ' ';
        if (x > 9999'9999'9999'9999)
        {
            print_hp(x / 10000 / 10000 / 10000 / 10000);
            print_lo(x / 10000 / 10000 / 10000 % 10000);
            print_lo(x / 10000 / 10000 % 10000);
            print_lo(x / 10000 % 10000);
            print_lo(x % 10000);
        }
        else if (x > 9999'9999'9999)
        {
            print_hp(x / 10000 / 10000 / 10000);
            print_lo(x / 10000 / 10000 % 10000);
            print_lo(x / 10000 % 10000);
            print_lo(x % 10000);
        }
        else if (x > 9999'9999)
        {
            print_hp(x / 10000 / 10000);
            print_lo(x / 10000 % 10000);
            print_lo(x % 10000);
        }
        else if (x > 9999)
        {
            print_hp(x / 10000);
            print_lo(x % 10000);
        }
        else
        {
            print_hp(x);
        }
    }
};
#include<bit>
using namespace std;
constexpr uint32_t P = 998244353;
struct affine
{
    uint32_t a,b;
    affine operator+(affine t)
    {
        [[assume(a < P && t.a < P)]];
        [[assume(b < P && t.b < P)]];
        return {uint32_t(uint64_t(t.a) * a % P), uint32_t((uint64_t(t.a) * b + t.b) % P)};
    }
    void operator+=(affine t) { *this = *this + t; }
};
struct node
{
    affine aff;
    uint32_t siz,sum;
    void operator+=(affine t)
    {
        [[assume(sum < P && siz < P)]];
        aff += t;
        sum = (uint64_t(t.a) * sum + uint64_t(t.b) * siz) % P;
    }
} a[1000000];
void pushdown(uint32_t k)
{
    for (int i = bit_width(k) - 2; i >= 0; --i)
    {
        affine t = a[k >> i >> 1].aff;
        a[k >> i >> 1].aff = affine{1, 0};
        a[k >> i ^ 0] += t;
        a[k >> i ^ 1] += t;
    }
}
uint32_t mod(uint32_t x) { return x < P ? x : x - P; }
void pushup(uint32_t i)
{
    for(i>>=1;i;i>>=1)
        a[i].sum = mod(a[i<<1].sum + a[i<<1|1].sum);
}
int main()
{
    rd rd;
    wt wt;
    uint32_t n = rd.uw(),q = rd.uw();
    for (uint32_t i = 0; i < n; ++i)
        a[n + i].siz = 1, a[n + i].sum = rd.uw();
    for (int i = n - 1; i >= 0; --i)
    {
        a[i].aff.a = 1;
        a[i].siz = a[i<<1].siz + a[i<<1|1].siz;
        a[i].sum = mod(a[i<<1].sum + a[i<<1|1].sum);
    }
    while(q--)
    {
        if(uint32_t op=rd.uw();op==0)
        {
            uint32_t l = n + rd.uw(),r = n + rd.uw() - 1;
            pushdown(l--);
            pushdown(r++);
            uint32_t k = bit_width(l ^ r)-1;
            affine x = {rd.uw(), rd.uw()};
            uint32_t i=31;
            for (uint32_t t = ~l & ~((~0u) << k); t > 0; t -= 1 << i)
            {
                i = bit_width(t)-1;
                a[(l >> i) ^ 1] += x;
            }
            pushup(l >> i);
            i=31;
            for (uint32_t t = r & ~((~0u) << k); t > 0; t -= 1 << i)
            {
                i = bit_width(t)-1;
                a[(r >> i) ^ 1] += x;
            }
            pushup(r >> i);
        }
        else
        {
            uint32_t l = rd.uw(),r = rd.uw() - 1;
            uint64_t sizL = 0, sumL = 0,sizR = 0, sumR = 0;
            for (l=n+l-1,r=n+r+1; l ^ r ^ 1;)
            {
                if (~l & 1)
                    sizL += a[l ^ 1].siz, sumL += a[l ^ 1].sum;
                if (r & 1)
                    sizR += a[r ^ 1].siz, sumR += a[r ^ 1].sum;
                l >>= 1, r >>= 1;
                [[assume(sumL < P + P && sizL < P)]];
                [[assume(sumR < P + P && sizR < P)]];
                sumL = (a[l].aff.a * sumL + a[l].aff.b * sizL) % P;
                sumR = (a[r].aff.a * sumR + a[r].aff.b * sizR) % P;
            }
            uint64_t res = mod(sumL + sumR);
            sizL += sizR;
            for(l>>=1;l;l>>=1)
            {
                [[assume(sumL < P && sizL < P)]];
                res = (a[l].aff.a * res + a[l].aff.b * sizL) % P;
            }
            wt.uw(res);
        }
    }
    return 0;
}