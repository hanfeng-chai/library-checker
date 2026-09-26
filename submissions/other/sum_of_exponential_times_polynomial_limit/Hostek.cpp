// https://judge.yosupo.jp/problem/sum_of_exponential_times_polynomial_limit

// based on: https://judge.yosupo.jp/submission/330500

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

namespace nachia {

struct CInStream {
private:
    static const unsigned int INPUT_BUF_SIZE = 1 << 17;
    unsigned int p = INPUT_BUF_SIZE;
    static char Q[INPUT_BUF_SIZE];

public:
    using MyType = CInStream;
    char seekChar() {
        if (p == INPUT_BUF_SIZE) {
            size_t len = fread(Q, 1, INPUT_BUF_SIZE, stdin);
            if (len != INPUT_BUF_SIZE) Q[len] = '\0';
            p = 0;
        }
        return Q[p];
    }
    void skipSpace() {
        while (isspace(seekChar()))
            p++;
    }

private:
    template<class T, int sp = 1>
    T nextUInt() {
        if constexpr (sp) skipSpace();
        T buf = 0;
        while (true) {
            char tmp = seekChar();
            if ('9' < tmp || tmp < '0') break;
            buf = buf * 10 + (tmp - '0');
            p++;
        }
        return buf;
    }

public:
    uint32_t nextU32() { return nextUInt<uint32_t>(); }
    int32_t nextI32() {
        skipSpace();
        if (seekChar() == '-') {
            p++;
            return (int32_t)(-nextUInt<uint32_t, 0>());
        }
        return (int32_t)nextUInt<uint32_t, 0>();
    }
    uint64_t nextU64() { return nextUInt<uint64_t>(); }
    int64_t nextI64() {
        skipSpace();
        if (seekChar() == '-') {
            p++;
            return (int64_t)(-nextUInt<int64_t, 0>());
        }
        return (int64_t)nextUInt<int64_t, 0>();
    }
    template<class T>
    T nextInt() {
        skipSpace();
        if (seekChar() == '-') {
            p++;
            return -nextUInt<T, 0>();
        }
        return nextUInt<T, 0>();
    }
    char nextChar() {
        skipSpace();
        char buf = seekChar();
        p++;
        return buf;
    }
    std::string nextToken() {
        skipSpace();
        std::string buf;
        while (true) {
            char ch = seekChar();
            if (isspace(ch) || ch == '\0') break;
            buf.push_back(ch);
            p++;
        }
        return buf;
    }
    MyType& operator>>(unsigned int& dest) {
        dest = nextU32();
        return *this;
    }
    MyType& operator>>(int& dest) {
        dest = nextI32();
        return *this;
    }
    MyType& operator>>(unsigned long& dest) {
        dest = nextU64();
        return *this;
    }
    MyType& operator>>(long& dest) {
        dest = nextI64();
        return *this;
    }
    MyType& operator>>(unsigned long long& dest) {
        dest = nextU64();
        return *this;
    }
    MyType& operator>>(long long& dest) {
        dest = nextI64();
        return *this;
    }
    MyType& operator>>(std::string& dest) {
        dest = nextToken();
        return *this;
    }
    MyType& operator>>(char& dest) {
        dest = nextChar();
        return *this;
    }
} cin;

struct FastOutputTable {
    char LZ[1000][4] = {};
    char NLZ[1000][4] = {};
    constexpr FastOutputTable() {
        using u32 = uint_fast32_t;
        for (u32 d = 0; d < 1000; d++) {
            LZ[d][0] = ('0' + d / 100 % 10);
            LZ[d][1] = ('0' + d / 10 % 10);
            LZ[d][2] = ('0' + d / 1 % 10);
            LZ[d][3] = '\0';
        }
        for (u32 d = 0; d < 1000; d++) {
            u32 i = 0;
            if (d >= 100) NLZ[d][i++] = ('0' + d / 100 % 10);
            if (d >= 10) NLZ[d][i++] = ('0' + d / 10 % 10);
            if (d >= 1) NLZ[d][i++] = ('0' + d / 1 % 10);
            NLZ[d][i++] = '\0';
        }
    }
};

struct COutStream {
private:
    using u32 = uint32_t;
    using u64 = uint64_t;
    using MyType = COutStream;
    static const u32 OUTPUT_BUF_SIZE = 1 << 17;
    static char Q[OUTPUT_BUF_SIZE];
    static constexpr FastOutputTable TB = FastOutputTable();
    u32 p = 0;
    static constexpr u32 P10(u32 d) { return d ? P10(d - 1) * 10 : 1; }
    static constexpr u64 P10L(u32 d) { return d ? P10L(d - 1) * 10 : 1; }
    template<class T, class U>
    static void Fil(T& m, U& l, U x) {
        m = l / x;
        l -= m * x;
    }

public:
    void next_dig9(u32 x) {
        u32 y;
        Fil(y, x, P10(6));
        nextCstr(TB.LZ[y]);
        Fil(y, x, P10(3));
        nextCstr(TB.LZ[y]);
        nextCstr(TB.LZ[x]);
    }
    void nextChar(char c) {
        Q[p++] = c;
        if (p == OUTPUT_BUF_SIZE) {
            fwrite(Q, p, 1, stdout);
            p = 0;
        }
    }
    void nextEoln() { nextChar('\n'); }
    void nextCstr(const char* s) {
        while (*s)
            nextChar(*(s++));
    }
    void nextU32(uint32_t x) {
        u32 y = 0;
        if (x >= P10(9)) {
            Fil(y, x, P10(9));
            nextCstr(TB.NLZ[y]);
            next_dig9(x);
        } else if (x >= P10(6)) {
            Fil(y, x, P10(6));
            nextCstr(TB.NLZ[y]);
            Fil(y, x, P10(3));
            nextCstr(TB.LZ[y]);
            nextCstr(TB.LZ[x]);
        } else if (x >= P10(3)) {
            Fil(y, x, P10(3));
            nextCstr(TB.NLZ[y]);
            nextCstr(TB.LZ[x]);
        } else if (x >= 1)
            nextCstr(TB.NLZ[x]);
        else
            nextChar('0');
    }
    void nextI32(int32_t x) {
        if (x >= 0)
            nextU32(x);
        else {
            nextChar('-');
            nextU32((u32)-x);
        }
    }
    void nextU64(uint64_t x) {
        u32 y = 0;
        if (x >= P10L(18)) {
            Fil(y, x, P10L(18));
            nextU32(y);
            Fil(y, x, P10L(9));
            next_dig9(y);
            next_dig9(x);
        } else if (x >= P10L(9)) {
            Fil(y, x, P10L(9));
            nextU32(y);
            next_dig9(x);
        } else
            nextU32(x);
    }
    void nextI64(int64_t x) {
        if (x >= 0)
            nextU64(x);
        else {
            nextChar('-');
            nextU64((u64)-x);
        }
    }
    template<class T>
    void nextInt(T x) {
        if (x < 0) {
            nextChar('-');
            x = -x;
        }
        if (!(0 < x)) {
            nextChar('0');
            return;
        }
        std::string buf;
        while (0 < x) {
            buf.push_back('0' + (int)(x % 10));
            x /= 10;
        }
        for (int i = (int)buf.size() - 1; i >= 0; i--) {
            nextChar(buf[i]);
        }
    }
    void writeToFile(bool flush = false) {
        fwrite(Q, p, 1, stdout);
        if (flush) fflush(stdout);
        p = 0;
    }
    COutStream() { Q[0] = 0; }
    ~COutStream() { writeToFile(); }
    MyType& operator<<(unsigned int tg) {
        nextU32(tg);
        return *this;
    }
    MyType& operator<<(unsigned long tg) {
        nextU64(tg);
        return *this;
    }
    MyType& operator<<(unsigned long long tg) {
        nextU64(tg);
        return *this;
    }
    MyType& operator<<(int tg) {
        nextI32(tg);
        return *this;
    }
    MyType& operator<<(long tg) {
        nextI64(tg);
        return *this;
    }
    MyType& operator<<(long long tg) {
        nextI64(tg);
        return *this;
    }
    MyType& operator<<(const std::string& tg) {
        nextCstr(tg.c_str());
        return *this;
    }
    MyType& operator<<(const char* tg) {
        nextCstr(tg);
        return *this;
    }
    MyType& operator<<(char tg) {
        nextChar(tg);
        return *this;
    }
} cout;

char CInStream::Q[INPUT_BUF_SIZE];
char COutStream::Q[OUTPUT_BUF_SIZE];

} // namespace nachia

using u32 = uint32_t;
using u64 = uint64_t;
using i64 = int64_t;

template<int mod>
struct modint {
    using M = modint;
    static constexpr u32 r1 = []() {
        u32 r1 = mod;
        for (int i = 0; i < 5; ++i)
            r1 *= 2 - mod * r1;
        return -r1;
    }();
    static constexpr u32 r2 = -u64(mod) % mod;
    static u32 reduce(u64 x) {
        u32 y = u32(x) * r1, r = (x + u64(y) * mod) >> 32;
        return r >= mod ? r - mod : r;
    }
    u32 x;
    modint() : x(0) {}
    modint(i64 x) : x(reduce(u64(x % mod + mod) * r2)) {}
    M& operator+=(const M& a) {
        if ((x += a.x) >= mod) x -= mod;
        return *this;
    }
    M& operator-=(const M& a) {
        if ((x += mod - a.x) >= mod) x -= mod;
        return *this;
    }
    M& operator*=(const M& a) {
        x = reduce(u64(x) * a.x);
        return *this;
    }
    M& operator/=(const M& a) { return *this *= a.inv(); }
    M operator-() const { return M(0) - *this; }
    M operator+(const M& a) const { return M(*this) += a; }
    M operator-(const M& a) const { return M(*this) -= a; }
    M operator*(const M& a) const { return M(*this) *= a; }
    bool operator==(const M& a) const { return x == a.x; }
    M pow(u64 k) const {
        M res(1), b = *this;
        while (k) {
            if (k & 1) res *= b;
            b *= b, k >>= 1;
        }
        return res;
    }
    M inv() const { return pow(mod - 2); }
    friend ostream& operator<<(ostream& os, const M& a) { return os << reduce(a.x); }
    friend istream& operator>>(istream& is, M& a) {
        i64 v;
        is >> v;
        a = M(v);
        return is;
    }
};

using Mint = modint<998244353>;

const int LIM = 10000005;
Mint fac[LIM], invFac[LIM];

void precomputeFactorials() {
    fac[0] = 1;
    for (int i = 1; i < LIM; ++i)
        fac[i] = fac[i - 1] * Mint(i);
    invFac[LIM - 1] = fac[LIM - 1].inv();
    for (int i = LIM - 2; i >= 0; --i)
        invFac[i] = invFac[i + 1] * Mint(i + 1);
}

Mint nCk(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fac[n] * invFac[k] * invFac[n - k];
}

vector<Mint> getMonomials(long long d, int n) {
    vector<Mint> pws(n);
    vector<int> min_prime(n, 0);
    vector<int> primes;

    pws[1] = 1;
    if (n > 0) pws[0] = (d == 0 ? 1 : 0);

    for (int i = 2; i < n; ++i) {
        if (min_prime[i] == 0) {
            min_prime[i] = i;
            primes.push_back(i);
            pws[i] = Mint(i).pow(d);
        }
        for (int p : primes) {
            if (p > min_prime[i] || i * p >= n) break;
            min_prime[i * p] = p;
            pws[i * p] = pws[i] * pws[p];
        }
    }
    return pws;
}

Mint sumPowerPolyLimit(Mint r, int d, const vector<Mint>& fs) {
    if (r.x == 0) return fs[0];

    Mint inv_1_r = (Mint(1) - r).inv();
    Mint C = -r * inv_1_r;
    Mint neg_inv_r = -r.inv();
    Mint C_pow_d = C.pow(d);

    Mint ans = 0;
    Mint V = C_pow_d;

    for (int j = d; j >= 0; --j) {
        Mint term = fs[j] * V;
        if (j & 1)
            ans -= term;
        else
            ans += term;

        if (j > 0) {
            V = V * neg_inv_r + nCk(d + 1, j) * C_pow_d;
        }
    }

    return ans * inv_1_r;
}

int main() {
    // ios_base::sync_with_stdio(false);
    // cin.tie(NULL);

    precomputeFactorials();

    int r_in, d;
    nachia::cin >> r_in >> d;
    Mint r(r_in);
    vector<Mint> pws = getMonomials(d, d + 1);
    auto ans = Mint::reduce(sumPowerPolyLimit(r, d, pws).x);
    nachia::cout << ans << "\n";

    return 0;
}
