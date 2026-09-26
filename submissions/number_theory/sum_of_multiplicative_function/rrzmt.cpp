#include <iostream>
#include <vector>
#include <cmath>
#include <bit>

#if defined(__GNUC__) || defined(__clang__)
    #define inline __attribute__((always_inline))
#elif defined(_MSC_VER)
    #define inline __forceinline
#else
#endif

#define ilog2(x) (u64(63) - countl_zero((u64)x))
using namespace std;
using u32 = unsigned int;
using u64 = unsigned long long;
using i64 = long long;
constexpr u32 mod = 469762049;
inline constexpr u32 norm(const u32 x) { return x < mod ? x : x - mod; }
struct m32 {
	u32 x;
	m32() { }
	constexpr m32(const u32 _x) : x(_x) { }
};
inline constexpr m32 operator + (const m32 x1, const m32 x2) { return norm(x1.x + x2.x); }
inline constexpr m32 operator - (const m32 x1, const m32 x2) { return norm(x1.x + mod - x2.x); }
inline constexpr m32 operator - (const m32 x) { return x.x ? mod - x.x : 0; }
inline constexpr m32 operator * (const m32 x1, const m32 x2) { return static_cast<u64>(x1.x) * x2.x % mod; }
inline m32& operator += (m32& x1, const m32 x2) { return x1 = x1 + x2; }
inline m32& operator -= (m32& x1, const m32 x2) { return x1 = x1 - x2; }
inline m32& operator *= (m32& x1, const m32 x2) { return x1 = x1 * x2; }
inline bool operator == (const m32 x1, const m32 x2) { return x1.x == x2.x; }
inline bool operator != (const m32 x1, const m32 x2) { return x1.x != x2.x; }

struct FIArray {
    static u64 x;
    static int xsqrt;
    static int len;
    vector<m32> arr;
    int k;
    FIArray() : arr(len, 0), k(len) {} 
    static inline int get_index(u64 v) {
        return v <= xsqrt ? v - 1 : len - (x / v);
    }
    void extend_flattened_prefix(int new_len) {
        for (int i = k; i < new_len; ++i) if (int j = i & (i + 1); j) arr[i] += arr[j - 1];
        k = new_len;
    }
    void shrink_flattened_prefix(int new_len) {
        for (int i = k - 1; i >= new_len; --i) if (int j = i & (i + 1); j) arr[i] -= arr[j - 1];
        k = new_len;
    }
    m32 sum(int i) const {
        m32 ret = 0;
        for (++i; i > k; i &= i - 1) ret += arr[i - 1];
        return i ? ret + arr[i - 1] : ret;
    }
    void sub(int i, m32 v) {
        for (; i < len; i |= i + 1) arr[i] -= v;
    }
    void add(int i, m32 v) {
        for (; i < len; i |= i + 1) arr[i] += v;
    }
    void inverse_pseudo_euler_transform_lucy() {
        if (x == 1) return (void)(arr[0] = 0);
        
        const int cutoff = std::max((int)sqrt(xsqrt + 0.5), (int)cbrt(x / ilog2(x)));
        shrink_flattened_prefix(1);
        sub(0, 1);
        m32 sp = 0;
        for (int p = 2; p <= cutoff; ++p) {
            const m32 sp1 = sum(p - 1);
            if (sp1 == sp) continue;
            extend_flattened_prefix(1 + get_index(p * p));
            const m32 w = sp1 - sp;
            const u64 lim = x / p;
            u64 j = 1;
            m32 cur = sum(get_index(lim));

            for (; (j + 1) <= lim / (j + 1); ++j) {
                if (const m32 next = sum(get_index(lim / (j + 1))); next != cur) {
                    sub(len - j, w * (cur - next));
                    cur = next;
                }
            }
            for (u64 i = lim / j; i >= p; --i) {
                if (const m32 next = sum(i - 2); next != cur) {
                    sub(get_index(i * p), w * (cur - next));
                    cur = next;
                }
            }
            sp = sp1;
        }
        extend_flattened_prefix(len);
        
        for (int p = cutoff + 1; p <= xsqrt; ++p) {
            const m32 sp1 = arr[p - 1];
            if (sp1 == sp) continue;
            const m32 w = sp1 - sp;
            const int xpp = x / p / p;
            u64 ip = p;
            for (int i = 1; i <= xpp; ++i, ip += p) 
                arr[len - i] -= w * ((ip <= xsqrt ? arr[len - ip] : arr[(x / ip) - 1]) - sp);
            sp = sp1;
        }
    }

    void pseudo_euler_transform_lucy() {
        if (x == 1) return (void)(arr[0] = 1);
        const int cutoff = std::max((int)sqrt(xsqrt + 0.5), (int)cbrt(x / ilog2(x)));

        m32 sp1 = arr[xsqrt - 1];
        for (int p = xsqrt; p > cutoff; --p) {
            const m32 sp = arr[p - 2];
            if (sp1 == sp) continue;
            const m32 w = sp1 - sp;
            const int xpp = x / p / p;
            for (int i = xpp; i; --i) 
                arr[len - i] += w * ((u64(i) * p <= xsqrt ? arr[len - u64(i) * p] : arr[(x / (u64(i) * p)) - 1]) - sp);
            sp1 = sp;
        }
        
        for (int p = cutoff; p > 1; --p) {
            const m32 sp = sum(p - 2);
            if (sp1 == sp) continue;
            shrink_flattened_prefix(1 + get_index(p * p));

            const m32 w = sp1 - sp;
            const u64 lim = x / p;
            m32 prev = sp;
            u64 i = p;
            for (; i <= lim / i; ++i) {
                if (const m32 cur = sum(i - 1); prev != cur) {
                    add(get_index(i * p), w * (cur - prev));
                    prev = cur;
                }
            }
            for (int j = lim / i; j; --j) {
                if (const m32 cur = sum(get_index(lim / j)); prev != cur) {
                    add(len - j, w * (cur - prev));
                    prev = cur;
                }
            }
            sp1 = sp;
        }
        shrink_flattened_prefix(1);
        add(0, 1);
        extend_flattened_prefix(len);
    }
};

u64 FIArray::x;
int FIArray::xsqrt;
int FIArray::len;

int solve(const i64 N, const m32 A, const m32 B) {
    const u64 x = N;
    const int xsqrt = sqrt(N + 0.5);
    const int len = (xsqrt << 1) - (xsqrt == x / xsqrt);

    FIArray::x = x;
    FIArray::xsqrt = xsqrt;
    FIArray::len = len;

    FIArray id{};
    FIArray unit{};
    int i = 1;
    for (; i <= xsqrt; ++i) {
        unit.arr[i - 1] = m32(i);
        id.arr[i - 1] = m32(((u64(i) * u64(i + 1)) >> 1) % mod);
    }
    --i;
    if (int v = x / xsqrt; v != xsqrt) {
        unit.arr[i] = m32(v);
        id.arr[i] = m32(((u64(v) * u64(v + 1)) >> 1) % mod);
        ++i;
    }
    
    for (int j = xsqrt - 1; j; --j, ++i) {
        u64 v = x / u64(j);
        v %= mod;
        unit.arr[i] = m32(v);
        id.arr[i] = m32(v * (v + 1) / 2 % mod);
    }
    
    unit.inverse_pseudo_euler_transform_lucy();
    id.inverse_pseudo_euler_transform_lucy();
    
    vector<int> primes{};
    for (int p = 2; p <= xsqrt; ++p) if (unit.arr[p - 1] != unit.arr[p - 2]) primes.push_back(p);
    for (int i = 0; i < len; ++i) id.arr[i] = B * id.arr[i] + A * unit.arr[i];

    id.pseudo_euler_transform_lucy();

    auto fpp = [=](u64 pp, int p, int e) { return A * m32(e) + B * m32(p); };
    auto dfs = [&](this auto dfs, int pi, u64 lim, u64 n, m32 hn) -> m32 {
        m32 res = hn * id.arr[FIArray::get_index(x / n)];
        for (; pi < primes.size(); ++pi) {
            u64 p = primes[pi];
            if (p > lim / p) break;
            const m32 fp = fpp(p, p, 1);
            m32 prev = fp;
            u64 pp = p * p;
            u64 new_lim = lim / pp;
            for (int e = 2; ; ++e) {
                const m32 cur = fpp(pp, p, e);
                if (const m32 hp = cur - fp * prev; hp != 0) res += dfs(pi + 1, new_lim, n * pp, hn * hp);
                prev = cur;
                if (p > new_lim) break;
                pp *= p;
                new_lim /= p;
            }
        }
        return res;
    };
    return dfs(0, N, 1, 1).x;
}
int main() {
    i64 T, n;
	m32 A, B;
	cin >> T;
	while (T--) {
		cin >> n >> A.x >> B.x;
		cout << solve(n, A, B) << '\n';
	}
	return 0;
}