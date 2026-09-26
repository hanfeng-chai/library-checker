#line 2 "1-Core\\01-template.hpp"

// 知彼知己，百战不殆
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

#define fi     first
#define se     second
#define pb     push_back
using uint = uint32_t;
using lng = int64_t;    using ulng = uint64_t;
using lll = __int128_t; using ulll = __uint128_t;
template<typename T> 
using indexed_set = tree<T, null_type, std::less<T>, rb_tree_tag, tree_order_statistics_node_update>;

constexpr int INF32 = 0x3f3f3f3f;
constexpr lng INF64 = 0x3f3f3f3f3f3f3f3f;

template<typename T> 
constexpr inline bool chmax(T &a, const T &b) { return a < b ? a = b, 1 : 0; }
template<typename T> 
constexpr inline bool chmin(T &a, const T &b) { return a > b ? a = b, 1 : 0; }
#line 3 "1-Core\\05-infint.hpp"

#ifdef __AVX2__
#include <immintrin.h>
namespace FastIintMul {
    constexpr uint P1 = 2013265921, G1 = 31;
    constexpr uint P2 = 1811939329, G2 = 13;
    constexpr uint P3 = 469762049,  G3 = 3;
    constexpr uint getIMOD(uint p) {
        uint res = p;
        for (int i = 0; i < 4; ++i) { res *= 2 - p * res; }
        return -res;}
    constexpr uint getRSQ(uint p) { return (1ULL << 32) % p * (1ULL << 32) % p; }
    constexpr uint modPow(uint a, uint b, uint p) {
        uint res = 1;
        while (b > 0) {
            if (b & 1) { res = ulng(res) * a % p; }
            a = ulng(a) * a % p;
            b >>= 1;}
        return res;}
    
    template<uint MOD, uint GEN>
    struct FastNTT {
        static constexpr uint IMOD = getIMOD(MOD);
        static constexpr uint IGEN = modPow(GEN, MOD - 2, MOD);
        static constexpr uint RSQ = getRSQ(MOD);
        
        static inline uint red(ulng x) {
            uint q = uint(x) * IMOD, res = (x + ulng(q) * MOD) >> 32;
            return res - (res >= MOD) * MOD;}
        static inline uint init(uint a) { return red(ulng(a) * RSQ); }
        static inline uint mul(uint a, uint b) { return red(ulng(a) * b); }
        static inline __m256i vred(__m256i x0, __m256i x1) {
            __m256i vmod32 = _mm256_set1_epi32(MOD);
            __m256i vmod64 = _mm256_set1_epi64x(MOD);
            __m256i vimod = _mm256_set1_epi32(IMOD);
            __m256i q0 = _mm256_mul_epu32(_mm256_mul_epu32(x0, vimod), vmod64);
            __m256i q1 = _mm256_mul_epu32(_mm256_mul_epu32(x1, vimod), vmod64);
            __m256i r0 = _mm256_srli_epi64(_mm256_add_epi64(x0, q0), 32);
            __m256i r1 = _mm256_srli_epi64(_mm256_add_epi64(x1, q1), 32);
            __m256i res = _mm256_blend_epi32(r0, _mm256_slli_epi64(r1, 32), 0xAA);
            __m256i msk = _mm256_cmpeq_epi32(_mm256_max_epu32(res, vmod32), res);
            return _mm256_sub_epi32(res, _mm256_and_si256(msk, vmod32));}
        static inline __m256i vmul(__m256i a, __m256i b) {
            __m256i x0 = _mm256_mul_epu32(a, b);
            __m256i x1 = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), _mm256_srli_epi64(b, 32));
            return vred(x0, x1);}
        
        static inline vector<uint> rt, irt;
        static void calc(int n) {
            if (rt.empty()) { rt = {init(0), init(1)}; irt = {init(0), init(1)}; }
            if (rt.size() < n) {
                int m = rt.size(); rt.resize(n); irt.resize(n);
                for (int len = m; len < n; len <<= 1) {
                    uint w0 = init(modPow(GEN, (MOD - 1) / (2 * len), MOD));
                    uint iw0 = init(modPow(IGEN, (MOD - 1) / (2 * len), MOD));
                    for (int i = len / 2; i < len; ++i) {
                        rt[2 * i]  = rt[i];  rt[2 * i + 1]  = mul(rt[i], w0);
                        irt[2 * i] = irt[i]; irt[2 * i + 1] = mul(irt[i], iw0);}}}}
        static void ntt(vector<uint> &a, bool is_inv) {
            int n = a.size(); calc(n);
            uint *A = a.data();
            const uint *W = is_inv ? irt.data() : rt.data();
            __m256i vmod32 = _mm256_set1_epi32(MOD);
            if (!is_inv) {
                for (int len = n / 2; len > 0; len >>= 1) {
                    for (int i = 0; i < n; i += 2 * len) {
                        int j = 0;
                        for (; j + 7 < len; j += 8) {
                            __m256i u = _mm256_loadu_si256((__m256i*)(&A[i + j]));
                            __m256i v = _mm256_loadu_si256((__m256i*)(&A[i + j + len]));
                            __m256i w = _mm256_loadu_si256((__m256i*)(&W[len + j]));
                            __m256i add = _mm256_add_epi32(u, v);
                            __m256i nu = _mm256_sub_epi32(add, _mm256_and_si256(
                                         _mm256_cmpeq_epi32(_mm256_max_epu32(add, vmod32), add), vmod32));
                            __m256i sub = _mm256_sub_epi32(u, v);
                            __m256i nv = vmul(_mm256_add_epi32(sub, _mm256_and_si256(
                                         _mm256_srai_epi32(sub, 31), vmod32)), w);
                            _mm256_storeu_si256((__m256i*)(&A[i + j]), nu);
                            _mm256_storeu_si256((__m256i*)(&A[i + j + len]), nv);}
                        for (; j < len; ++j) {
                            uint u = A[i + j], v = A[i + j + len], w = W[len + j];
                            A[i + j]       = u + v - (u + v >= MOD) * MOD;
                            A[i + j + len] = mul(u - v + (u < v) * MOD, w);}}}} 
            else {
                for (int len = 1; len < n; len <<= 1) {
                    for (int i = 0; i < n; i += 2 * len) {
                        int j = 0;
                        for (; j + 7 < len; j += 8) {
                            __m256i u = _mm256_loadu_si256((__m256i*)(&A[i + j]));
                            __m256i v = _mm256_loadu_si256((__m256i*)(&A[i + j + len]));
                            __m256i w = _mm256_loadu_si256((__m256i*)(&W[len + j]));
                            __m256i vw = vmul(v, w);
                            __m256i add = _mm256_add_epi32(u, vw);
                            __m256i nu = _mm256_sub_epi32(add, _mm256_and_si256(
                                         _mm256_cmpeq_epi32(_mm256_max_epu32(add, vmod32), add), vmod32));
                            __m256i sub = _mm256_sub_epi32(u, vw);
                            __m256i nv = _mm256_add_epi32(sub, _mm256_and_si256(
                                         _mm256_srai_epi32(sub, 31), vmod32));
                            _mm256_storeu_si256((__m256i*)(&A[i + j]), nu);
                            _mm256_storeu_si256((__m256i*)(&A[i + j + len]), nv);}
                        for (; j < len; ++j) {
                            uint u = A[i + j], vw = mul(A[i + j + len], W[len + j]);
                            A[i + j]       = u + vw - (u + vw >= MOD) * MOD;
                            A[i + j + len] = u - vw + (u < vw) * MOD;}}}
                uint ninv = modPow(n, MOD - 2, MOD);
                for (int i = 0; i < n; ++i) { A[i] = ulng(A[i]) * ninv % MOD; }}}};
    static void multiply(const vector<uint> &a, const vector<uint> &b, vector<uint> &res) {
        using E1 = FastNTT<P1, G1>;
        using E2 = FastNTT<P2, G2>;
        using E3 = FastNTT<P3, G3>;
        constexpr ulng P1P2 = ulng(P1) * P2;
        constexpr uint IP1_P2 = modPow(P1, P2 - 2, P2);
        constexpr uint IP1P2_P3 = modPow(P1P2 % P3, P3 - 2, P3);
        
        int len = 1; 
        while (len < a.size() + b.size()) { len <<= 1; }
        static vector<uint> a1, b1, a2, b2, a3, b3;
        a1.assign(len, 0); a2.assign(len, 0); a3.assign(len, 0);
        if (&a != &b) { b1.assign(len, 0); b2.assign(len, 0); b3.assign(len, 0); }
        for (int i = 0; i < a.size(); ++i) {
            a1[i] = a[i] % P1; a2[i] = a[i] % P2; a3[i] = a[i] % P3;}
        if (&a != &b) {
            for (int i = 0; i < b.size(); ++i) {
                b1[i] = b[i] % P1; b2[i] = b[i] % P2; b3[i] = b[i] % P3;}}
        E1::ntt(a1, false); E2::ntt(a2, false); E3::ntt(a3, false);
        if (&a != &b) {
            E1::ntt(b1, false); E2::ntt(b2, false); E3::ntt(b3, false);
            for (int i = 0; i < len; ++i) {
                a1[i] = ulng(a1[i]) * b1[i] % P1;
                a2[i] = ulng(a2[i]) * b2[i] % P2;
                a3[i] = ulng(a3[i]) * b3[i] % P3;}} 
        else {
            for (int i = 0; i < len; ++i) {
                a1[i] = ulng(a1[i]) * a1[i] % P1;
                a2[i] = ulng(a2[i]) * a2[i] % P2;
                a3[i] = ulng(a3[i]) * a3[i] % P3;}}
        E1::ntt(a1, true); E2::ntt(a2, true); E3::ntt(a3, true);
        res.assign(len, 0); ulng carry = 0;
        for (int i = 0; i < len; ++i) {
            ulng v1 = a1[i];
            ulng v2 = (a2[i] + P2 - v1 % P2) * IP1_P2 % P2;
            ulng v3 = (a3[i] + P3 - (v1 + v2 * P1) % P3) * IP1P2_P3 % P3;
            ulll exact = v1 + v2 * P1 + ulll(v3) * P1P2; exact += carry;
            res[i] = uint(exact); carry = ulng(exact >> 32);}
        while (carry > 0) { res.push_back(uint(carry)); carry >>= 32; }}
}
#endif

// T: O(n) or O(n * log(n)), M: O(n)
struct InfInt {
    static constexpr const char *HEX_D = "0123456789ABCDEF";
    static constexpr ulng NTT_P = 18446744069414584321ULL;
    static constexpr ulng NTT_G = 7;
    static constexpr array<uint, 10000> itos10 = []() {
        array<uint, 10000> v{};
        for (int i = 0; i < 10000; i++) {
            int a = i / 1000 % 10, b = i / 100 % 10, c = i / 10 % 10, d = i % 10;
            v[i] = a + (b << 8) + (c << 16) + (d << 24) + 0x30303030;}
        return v;}();
    
    int sgn; bool is_inf = false;
    vector<uint> n;
    // T: O(1)
    InfInt() : sgn(1) {}
    // T: O(1)
    template<typename T>
    requires std::is_integral_v<T> || std::is_same_v<T, lll> || std::is_same_v<T, ulll>
    InfInt(T N) { *this = N; }
    // T: O(n * log(n)^2)
    InfInt(std::string_view N, uint base = 10) { read(N, base); }
    // T: O(n * log(n)^2)
    InfInt(const vector<uint> &a, uint base = 10, int sgn = 1) { init(a, base, sgn); }
    
    // T: O(1)
    explicit operator lng() const {
        if (is_inf) { return sgn == 1 ? 9223372036854775807LL : -9223372036854775807LL - 1; }
        if (isNil()) { return 0; }
        ulng res = n[0];
        if (n.size() > 1) { res |= ulng(n[1]) << 32; }
        return sgn == 1 ? lng(res) : lng(-res);}
    // T: O(1)
    explicit operator ulng() const {
        if (is_inf) { return sgn == 1 ? 0xFFFFFFFFFFFFFFFFULL : 0; }
        if (isNil()) { return 0; }
        ulng res = n[0];
        if (n.size() > 1) { res |= ulng(n[1]) << 32; }
        return sgn == 1 ? res : -res;}
    // T: O(n * log(n)^2)
    static vector<pair<InfInt, InfInt>> &getBpow2(uint base) {
        static vector<pair<InfInt, InfInt>> b10pow2;
        static unordered_map<uint, vector<pair<InfInt, InfInt>>> bspow2;
        vector<pair<InfInt, InfInt>> *res;
        if (base == 10) { res = &b10pow2; }
        else { res = &bspow2[base]; }
        if (res->empty()) {
            ulng b = base;
            while (b * base <= 0xFFFFFFFFULL) { b *= base; }
            InfInt b_norm = b; b_norm <<= __builtin_clz(uint(b));
            InfInt b_inv = newtonRaphsonInv(b_norm);
            res->push_back({std::move(b), std::move(b_inv)});}
        return *res;}
    // T: O(n * log(n)^2)
    void initFromBlks(vector<uint> &blks, uint base, int sgn) {
        ulng B = base;
        while (B * base <= 0xFFFFFFFFULL) { B *= base; }
        int p = 0;
        while ((1U << p) < blks.size()) { p++; }
        blks.resize(1U << p, 0);
        auto &pows = getBpow2(base);
        while (pows.size() <= p) {
            InfInt b = pows.back().first * pows.back().first;
            InfInt b_norm = b; b_norm <<= __builtin_clz(b.n.back());
            InfInt b_inv = newtonRaphsonInv(b_norm);
            pows.push_back({std::move(b), std::move(b_inv)});}
        auto build = [&](auto &&build, int l, int r, int p) -> InfInt {
            if (l == r) { return InfInt(blks[l]); }
            if (r - l + 1 <= 256) {
                InfInt res = 0; res.n.reserve(r - l + 2);
                for (int i = r; i >= l; i--) {
                    res *= B; ulng carry = blks[i];
                    for (int j = 0; j < res.n.size() && carry; j++) {
                        carry += res.n[j]; res.n[j] = uint(carry); carry >>= 32;}
                    while (carry) { res.n.push_back(uint(carry)); carry >>= 32; }}
                res.trim();
                return res;}
            int md = std::midpoint(l, r);
            InfInt L = build(build, l,      md, p - 1);
            InfInt R = build(build, md + 1, r,  p - 1);
            if (R.isNil()) { return L; }
            if (L.isNil()) { R *= pows[p - 1].first; return R; }
            R *= pows[p - 1].first; 
            int req = max(R.n.size(), L.n.size()) + 1; R.n.reserve(req);
            R += std::move(L);
            return R;};
        *this = build(build, 0, (1U << p) - 1, p);
        this->sgn = isNil() ? 1 : sgn;}
    // T: O(n * log(n)^2)
    void init(const vector<uint> &a, uint base = 10, int sgn = 1) {
        if (a.empty()) { *this = 0; this->sgn = sgn; return; }
        if (base == 2 || base == 8 || base == 16) {
            int len = a.size();
            if (base == 2) {
                n.assign((len + 31) >> 5, 0);
                for (int i = 0; i < len; i++) {
                    n[i >> 5] |= (a[len - i - 1] << (i & 31));}}
            else if (base == 8) {
                n.assign((len * 3 + 31) >> 5, 0);
                int bits = 0, j = 0; ulng cur = 0;
                for (int i = 0; i < len; i++) {
                    cur |= ulng(a[len - i - 1]) << bits; bits += 3;
                    if (bits >= 32) {
                        n[j++] = uint(cur & 0xFFFFFFFFULL);
                        cur >>= 32; bits -= 32;}}
                if (bits > 0 && j < n.size()) { n[j] = uint(cur); }}
            else if (base == 16) {
                n.assign((len + 7) >> 3, 0);
                for (int i = 0; i < len; i++) {
                    n[i >> 3] |= a[len - i - 1] << ((i & 7) << 2);}}
            trim(); this->sgn = isNil() ? 1 : sgn;
            return;}
        int gsz = 1;
        for (ulng i = base; i * base <= 0xFFFFFFFFULL; i *= base) { gsz++; }
        static vector<uint> blks; blks.clear();
        for (int i = int(a.size()) - 1; i >= 0; i -= gsz) {
            uint cur = 0;
            for (int j = max(i - gsz + 1, 0); j <= i; j++) { cur = cur * base + a[j]; }
            blks.push_back(cur);}
        initFromBlks(blks, base, sgn);}
    // T: O(n * log(n)^2)
    void read(std::string_view s, uint base = 10) {
        sgn = 1; is_inf = false; n.clear();
        if (s.empty()) { *this = 0; return; }
        int pos = 0;
        while (pos < s.size() && (s[pos] == '+' || s[pos] == '-')) {
            if (s[pos] == '-') { sgn *= -1; } 
            pos++;}
        if (pos == s.size()) { *this = 0; return; }
        if (s.substr(pos) == "INF" || s.substr(pos) == "inf") { is_inf = true; return; }
        if (base == 2 || base == 8 || base == 16) {
            int len = s.size() - pos;
            if (base == 2) {
                n.assign((len + 31) >> 5, 0);
                for (int i = 0; i < len; i++) {
                    n[i >> 5] |= uint(s[s.size() - i - 1] - '0') << (i & 31);}}
            else if (base == 8) {
                n.assign((len * 3 + 31) >> 5, 0);
                int bits = 0, j = 0; ulng cur = 0;
                for (int i = 0; i < len; i++) {
                    cur |= ulng(s[s.size() - i - 1] - '0') << bits; bits += 3;
                    if (bits >= 32) {
                        n[j++] = uint(cur & 0xFFFFFFFFULL);
                        cur >>= 32; bits -= 32;}}
                if (bits > 0 && j < n.size()) { n[j] = uint(cur); }} 
            else if (base == 16) {
                n.assign((len + 7) >> 3, 0);
                int i = 0;
                for (; i + 8 <= len; i += 8) {
                    ulng cur; std::memcpy(&cur, s.data() + s.size() - i - 8, 8);
                    cur = (cur & 0x0F0F0F0F0F0F0F0FULL) + ((cur & 0x4040404040404040ULL) >> 6) * 9;
                    cur = ((cur << 4) | (cur >> 8)) & 0x00FF00FF00FF00FFULL;
                    cur = ((cur << 8) | (cur >> 16)) & 0x0000FFFF0000FFFFULL;
                    n[i >> 3] = uint((cur << 16) | (cur >> 32));}
                for (; i < len; i++) {
                    char c = s[s.size() - i - 1];
                    n[i >> 3] |= ((c & 0x0F) + ((c & 0x40) > 0) * 9) << ((i & 7) << 2);}}
            trim(); sgn = isNil() ? 1 : sgn;
            return;}
        int gsz = 1;
        for (ulng i = base; i * base <= 0xFFFFFFFFULL; i *= base) { gsz++; }
        static vector<uint> blks, digs; blks.clear();
        if (base <= 16) {
            for (int i = s.size(); i > pos; i -= gsz) {
                uint cur = 0;
                for (int j = max(i - gsz, pos); j < i; j++) {
                    char c = s[j]; uint d = 0;
                    if ('0' <= c && c <= '9') { d = c - '0'; }
                    else if ('A' <= c && c <= 'F') { d = 10 + (c - 'A'); } 
                    else if ('a' <= c && c <= 'f') { d = 10 + (c - 'a'); }
                    cur = cur * base + d;}
                blks.push_back(cur);}} 
        else {
            if (pos < s.size() && s[pos] == '[') { pos++; }
            digs.clear(); bool chk = false; uint cur = 0;  
            for (; pos < s.size(); pos++) {
                char c = s[pos];
                if (c >= '0' && c <= '9') { chk = true; cur = cur * 10 + (c - '0'); } 
                else if (c == ',' || c == ']') {
                    if (chk) { digs.push_back(cur); chk = false; cur = 0; }
                    if (c == ']') { break; }}}
            if (chk) { digs.push_back(cur); }
            for (int i = int(digs.size()) - 1; i >= 0; i -= gsz) {
                cur = 0;
                for (int j = max(i - gsz + 1, 0); j <= i; j++) { cur = cur * base + digs[j]; }
                blks.push_back(cur);}}
        initFromBlks(blks, base, sgn);}
    // T: O(n * log(n)^2)
    string toString(uint base = 10) const {
        if (is_inf) { return (sgn == -1 ? "-inf" : "inf"); }
        if (isNil()) { return "0"; }
        if (base == 2) { return bin(*this); }
        if (base == 8) { return oct(*this); }
        if (base == 16) { return hex(*this); }
        int gsz = 1; ulng b = base;
        while (b * base <= 0xFFFFFFFFULL) { gsz++; b *= base; }
        int p = 0; InfInt ua = abs(*this);
        auto &pows = getBpow2(base);
        while (true) {
            if (p >= pows.size()) {
                InfInt b = pows.back().first * pows.back().first;
                InfInt b_norm = b; b_norm <<= __builtin_clz(b.n.back());
                InfInt b_inv = newtonRaphsonInv(b_norm);
                pows.push_back({std::move(b), std::move(b_inv)});}
            if (pows[p].first > ua) { break; }
            p++;}
        static vector<uint> res; res.clear();
        auto extract = [&](auto &&extract, InfInt a, int p, bool pad) {
            if (a.isNil()) { if (pad) { res.insert(res.end(), 1U << p, 0); } return; }
            if (a.n.size() <= 128) {
                int cnt = 0, id = res.size();
                if (base == 10) {
                    while (!a.isNil()) {
                        ulng r = 0;
                        for (int i = int(a.n.size()) - 1; i >= 0; i--) {
                            ulng cur = a.n[i] + (r << 32);
                            ulng q = (ulll(cur) * 18446744074ULL) >> 64; r = cur - q * 1000000000ULL;
                            if (r >= 1000000000ULL) { q--; r += 1000000000ULL; }
                            a.n[i] = uint(q);}
                        a.trim(); res.push_back(uint(r)); cnt++;}} 
                else {
                    while (!a.isNil()) {
                        ulng r = 0;
                        for (int i = int(a.n.size()) - 1; i >= 0; i--) {
                            ulng cur = a.n[i] + (r << 32);
                            a.n[i] = uint(cur / b); r = cur % b;}
                        a.trim(); res.push_back(uint(r)); cnt++;}}
                reverse(res.begin() + id, res.end());
                if (pad) { res.insert(res.begin() + id, (1U << p) - cnt, 0); }
                return;}
            auto [q, r] = divMod(std::move(a), pows[p - 1].first, &pows[p - 1].second);
            if (!q.isNil() || pad) {
                extract(extract, std::move(q), p - 1, pad);
                extract(extract, std::move(r), p - 1, true);} 
            else { extract(extract, std::move(r), p - 1, pad); }};
        extract(extract, std::move(ua), p, false);
        if (base == 10) {
            if (res.empty()) { return "0"; }
            string s; s.reserve(res.size() * 9 + 2);
            if (sgn == -1 && !isNil()) { s += '-'; }
            s += std::to_string(res[0]); char buf[12];
            for (int i = 1; i < res.size(); i++) {
                uint cur = res[i]; 
                uint lo = cur % 10000, md = cur / 10000 % 10000, hi = cur / 100000000;
                buf[0] = char('0' + hi);
                std::memcpy(buf + 1, &itos10[md], 4);
                std::memcpy(buf + 5, &itos10[lo], 4);
                s.append(buf, 9);}
            return s;}
        static vector<uint> digs; digs.clear(); digs.reserve(res.size() * gsz);
        for (int i = 0; i < res.size(); i++) {
            uint cur = res[i]; uint buf[32];
            for (int j = gsz - 1; j >= 0; j--) { buf[j] = cur % base; cur /= base; }
            int pos = 0;
            if (i == 0) {
                while (pos < gsz && buf[pos] == 0) { pos++; }
                if (pos == gsz && res.size() == 1) { pos--; }}
            for (int j = pos; j < gsz; j++) { digs.push_back(buf[j]); }}
        if (digs.empty()) { digs.push_back(0); }
        if (base <= 16) {
            string s; s.reserve(digs.size() + 1);
            if (sgn == -1 && !isNil()) { s += '-'; }
            for (uint d : digs) { s += HEX_D[d]; }
            return s;} 
        else {
            string s; s.reserve(digs.size() * 5 + 2);
            if (sgn == -1 && !isNil()) { s += '-'; }
            s += '[';
            for (uint d : digs) { s += std::to_string(d); s += ", "; }
            s.pop_back(); s.pop_back(); s += ']';
            return s;}}
    // T: O(1)
    void trim() {
        if (is_inf) { n.clear(); return; }
        if (n.empty()) { n.push_back(0); sgn = 1; return; }
        int pos = int(n.size()) - 1;
        while (pos > 0 && n[pos] == 0) { pos--; }
        if (pos + 1 < n.size()) { n.resize(pos + 1); }
        if (n.empty() || (n.size() == 1 && n[0] == 0)) { sgn = 1; }}
    // T: O(1)
    template<typename T>
    requires std::is_integral_v<T> || std::is_same_v<T, lll> || std::is_same_v<T, ulll>
    InfInt &operator=(T N) {
        sgn = N < 0 ? -1 : 1; is_inf = false; 
        ulll uN = N < 0 ? -ulll(N) : N;
        n.assign(1, uint(uN));
        while (uN >>= 32) { n.push_back(uint(uN)); }
        return *this;}
    // T: O(n * log(n)^2)
    InfInt &operator=(std::string_view N) { read(N); return *this; }
    
    InfInt &operator++() { 
        if (is_inf) { return *this; }
        if (sgn == -1) { sgn = 1; --(*this); sgn = isNil() ? 1 : -1; return *this; }
        for (int i = 0; i < n.size(); i++) { if (++n[i] != 0) { return *this; } }
        n.push_back(1);
        return *this;}
    InfInt &operator--() {
        if (is_inf) { return *this; }
        if (isNil()) { n.assign(1, 1); sgn = -1; return *this; }
        if (sgn == -1) { sgn = 1; ++(*this); sgn = -1; return *this; }
        for (int i = 0; i < n.size(); i++) { if (n[i]-- != 0) { trim(); return *this; } }
        return *this;}
    InfInt operator++(int) { InfInt res = *this; ++(*this); return res; }
    InfInt operator--(int) { InfInt res = *this; --(*this); return res; }
    InfInt &operator+=(const InfInt &o) {
        if (is_inf || o.is_inf) {
            if (is_inf && o.is_inf && sgn != o.sgn) { return *this = 0; }
            if (o.is_inf) { sgn = o.sgn; is_inf = true; }
            n.clear();
            return *this;}
        if (sgn != o.sgn) { 
            sgn = -sgn; *this -= o; sgn = -sgn;
            if (isNil()) { sgn = 1; } 
            return *this;}
        int n_len = n.size(), o_len = o.n.size();
        if (n_len < o_len) { n.resize(o_len, 0); n_len = o_len; }
        ulng carry = 0;
        for (int i = 0; i < o_len; i++) {
            ulng cur = ulng(n[i]) + o.n[i] + carry;
            n[i] = uint(cur); carry = cur >> 32;}
        for (int i = o_len; i < n_len && carry; i++) { if (++n[i] != 0) { carry = 0; } }
        if (carry) { n.push_back(1); } 
        trim();
        return *this;}
    InfInt &operator-=(const InfInt &o) {
        if (is_inf || o.is_inf) {
            if (is_inf && o.is_inf && sgn == o.sgn) { return *this = 0; }
            if (o.is_inf) { sgn = -o.sgn; is_inf = true; }
            n.clear(); 
            return *this;}
        if (sgn != o.sgn) { 
            sgn = -sgn; *this += o; sgn = -sgn;
            if (isNil()) { sgn = 1; }
            return *this;}
        int o_len = o.n.size();
        if (magCmp(*this, o) < 0) {
            n.resize(o_len, 0);
            ulng carry = 0;
            for (int i = 0; i < o_len; i++) {
                ulng cur = ulng(o.n[i]) - n[i] - carry;
                n[i] = uint(cur); carry = cur >> 63;}
            trim(); sgn = -sgn; 
            return *this;}
        int n_len = n.size();
        ulng carry = 0;
        for (int i = 0; i < o_len; i++) {
            ulng cur = ulng(n[i]) - o.n[i] - carry;
            n[i] = uint(cur); carry = cur >> 63;}
        for (int i = o_len; i < n_len && carry; i++) { if (n[i]-- != 0) { carry = 0; } }
        trim();
        return *this;}
    
    InfInt &operator*=(lng o) {
        if (isNil() || o == 0) { return *this = 0; }
        if (is_inf) { sgn *= o < 0 ? -1 : 1; n.clear(); return *this; }
        sgn *= o < 0 ? -1 : 1; 
        ulng uo = o < 0 ? -ulng(o) : o;
        if (uo == 1) { return *this; }
        int len = n.size(); ulng carry = 0;
        if (uo <= 0xFFFFFFFFULL) {
            uint uo32 = uint(uo);
            for (int i = 0; i < len; i++) {
                ulng cur = ulng(n[i]) * uo32 + carry;
                n[i] = uint(cur); carry = cur >> 32;}
            if (carry) { n.push_back(uint(carry)); }}
        else {
            for (int i = 0; i < len; i++) {
                ulll cur = ulll(n[i]) * uo + carry;
                n[i] = uint(cur); carry = cur >> 32;}
            while (carry > 0) { n.push_back(uint(carry)); carry >>= 32; }}
        trim();
        return *this;}
    // T: O(n^1.58)
    static void karatsuba(int len, const ulng *__restrict a,     const ulng *__restrict b,     ulll *__restrict res, 
                                         ulng *__restrict tmp_a,       ulng *__restrict tmp_b, ulll *__restrict tmp_res) {
        if (len <= 32) {
            std::fill(res, res + 2 * len, 0);
            for (int i = 0; i < len; i++) {
                for (int j = 0; j < len; j++) { res[i + j] += ulll(a[i]) * b[j]; }}
            return;}
        int m = len / 2;
        karatsuba(m, a,     b,     res,         tmp_a, tmp_b, tmp_res);
        karatsuba(m, a + m, b + m, res + 2 * m, tmp_a, tmp_b, tmp_res);
        for (int i = 0; i < m; i++) {
            tmp_a[i] = a[i] + a[i + m];
            tmp_b[i] = b[i] + b[i + m];}
        karatsuba(m, tmp_a, tmp_b, tmp_res, tmp_a + m, tmp_b + m, tmp_res + 2 * m);
        for (int i = 0; i < 2 * m; i++) { tmp_res[i] -= res[i] + res[i + 2 * m]; }
        for (int i = 0; i < 2 * m; i++) { res[i + m] += tmp_res[i]; }}
    // T: O(1)
    static inline ulng modP(ulll a) {
        ulng l = a, hl = uint(a >> 64), hh = a >> 96;
        ulll cur = ulll(l) + (ulll(hl) << 32); ulng sub = hl + hh;
        cur += (cur < sub) * NTT_P; cur -= sub;
        ulng res = cur;
        res += (cur >> 64) * 0xFFFFFFFFULL; res -= (res >= NTT_P) * NTT_P;
        return res;}
    // T: O(1)
    static ulng nttPow(ulng a, ulng b) {
        ulng res = 1;
        a = modP(a);
        while (b > 0) {
            if (b & 1) { res = modP(ulll(res) * a); }
            a = modP(ulll(a) * a); b >>= 1;}
        return res;}
    static void ntt(vector<ulng> &a, bool is_inv) {
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) { j ^= bit; } 
            j ^= bit;
            if (i < j) { std::swap(a[i], a[j]); }}
        static vector<ulng> w = {0, 1};
        if (w.size() < n) {
            int m = w.size(); w.resize(n);
            for (int len = m; len < n; len <<= 1) {
                ulng w0 = nttPow(NTT_G, (NTT_P - 1) / (2 * len));
                for (int i = len / 2; i < len; i++) {
                    w[2 * i] = w[i];
                    w[2 * i + 1] = modP(ulll(w[i]) * w0);}}}
        for (int len = 1; len < n; len <<= 1) {
            for (int i = 0; i < n; i += 2 * len) {
                for (int j = 0; j < len; j++) {
                    ulng u = a[i + j]; ulng v = modP(ulll(a[i + j + len]) * w[j + len]);
                    a[i + j] =       u + v - ((u + v < u) | (u + v >= NTT_P)) * NTT_P;
                    a[i + j + len] = u - v + (u < v) * NTT_P;}}}
        if (is_inv) {
            reverse(a.begin() + 1, a.end());
            ulng n_inv = nttPow(n, NTT_P - 2);
            for (ulng &x : a) { x = modP(ulll(x) * n_inv); }}}
    InfInt &operator*=(const InfInt &o) {
        if (isNil() || o.isNil()) { return *this = 0; }
        if (is_inf || o.is_inf) { sgn *= o.sgn; is_inf = true; n.clear(); return *this; }
        int n_len = n.size(), o_len = o.n.size(); sgn *= o.sgn;
        if (min(n_len, o_len) < 64) {
            static vector<uint> res;
            res.assign(n_len + o_len, 0);
            const uint *n_ptr = n.data(); const uint *o_ptr = o.n.data();
            uint *res_ptr = res.data();
            for (int i = 0; i < n_len; i++) {
                uint ni = n_ptr[i];
                if (!ni) { continue; }
                ulng carry = 0;
                for (int j = 0; j < o_len; j++) {
                    ulng cur = ulng(res_ptr[i + j]) + ulng(ni) * o_ptr[j] + carry;
                    res_ptr[i + j] = uint(cur); carry = cur >> 32;}
                res_ptr[i + o_len] = carry;}
            n.swap(res); trim();
            return *this;}
        
        #ifdef __AVX2__
        if (max(n_len, o_len) < 256) {
        #else
        if (max(n_len, o_len) < 1024) {
        #endif
            int len = 1; 
            while (len < max(n_len, o_len)) { len <<= 1; }
            static vector<ulng> a, b, tmp_a, tmp_b;
            static vector<ulll> res, tmp_res;
            a.assign(len, 0); std::copy(n.begin(), n.end(), a.begin());
            b.assign(len, 0); std::copy(o.n.begin(), o.n.end(), b.begin()); res.assign(2 * len, 0);
            tmp_a.assign(len, 0); tmp_b.assign(len, 0); tmp_res.assign(2 * len, 0);
            karatsuba(len, a.data(), b.data(), res.data(), tmp_a.data(), tmp_b.data(), tmp_res.data());
            n.assign(n_len + o_len, 0);
            ulng carry = 0;
            for (int i = 0; i < n.size(); i++) {
                ulll cur = res[i] + carry;
                n[i] = uint(cur); carry = ulng(cur >> 32);}
            trim();
            return *this;}
        
        #ifdef __AVX2__
        FastIintMul::multiply(n, o.n, n);
        #else
        int len = 1; 
        while (len < 2 * (n_len + o_len)) { len <<= 1; }
        static vector<ulng> fa, fb;
        fa.assign(len, 0); fb.assign(len, 0);
        for (int i = 0; i < n_len; i++) { fa[2 * i] = n[i] & 0xFFFF;   fa[2 * i + 1] = n[i] >> 16; }
        for (int i = 0; i < o_len; i++) { fb[2 * i] = o.n[i] & 0xFFFF; fb[2 * i + 1] = o.n[i] >> 16; }
        ntt(fa, false); ntt(fb, false);
        for (int i = 0; i < len; i++) { fa[i] = modP(ulll(fa[i]) * fb[i]); }
        ntt(fa, true);
        n.assign(len >> 1, 0);
        ulng carry = 0;
        for (int i = 0; i < len; i += 2) {
            ulng cur = fa[i] + carry, nxt = fa[i + 1] + (cur >> 16);
            n[i >> 1] = uint((cur & 0xFFFF) | ((nxt & 0xFFFF) << 16)); carry = nxt >> 16;}
        while (carry > 0) { n.push_back(uint(carry & 0xFFFFFFFFULL)); carry >>= 32; }
        #endif
        trim();
        return *this;}
    
    InfInt &operator/=(lng o) {
        assert(o != 0 && "InfInt lng division error: Division by zero.");
        if (is_inf) { sgn *= o < 0 ? -1 : 1; n.clear(); return *this; }
        sgn *= o < 0 ? -1 : 1;
        ulng uo = o < 0 ? -ulng(o) : o, rem = 0;
        if (uo <= 0xFFFFFFFFULL) {
            uint uo32 = uint(uo); ulll mgc = ((ulll(1) << 64) + uo32 - 1) / uo32;
            for (int i = int(n.size()) - 1; i >= 0; i--) {
                ulng cur = n[i] + (rem << 32);
                ulng qi = (cur * mgc) >> 64, ri = cur - qi * uo32;
                if (ri >= uo32) { qi--; ri += uo32; }
                n[i] = uint(qi); rem = ri;}}
        else {
            for (int i = int(n.size()) - 1; i >= 0; i--) {
                ulll cur = n[i] + (ulll(rem) << 32);
                n[i] = uint(cur / uo); rem = cur % uo;}}
        trim();
        return *this;}
    InfInt &operator%=(lng o) {
        assert(o != 0 && "InfInt lng modulo error: Modulo by zero.");
        if (is_inf) { return *this = 0; }
        ulng uo = o < 0 ? -ulng(o) : o, rem = 0;
        if (uo <= 0xFFFFFFFFULL) {
            uint uo32 = uint(uo); ulll mgc = ((ulll(1) << 64) + uo32 - 1) / uo32;
            for (int i = int(n.size()) - 1; i >= 0; i--) {
                ulng cur = n[i] + (rem << 32);
                ulng qi = (cur * mgc) >> 64;
                rem = cur - qi * uo32;
                if (rem >= uo32) { rem += uo32; }}}
        else {
            for (int i = int(n.size()) - 1; i >= 0; i--) {
                rem = (n[i] + (ulll(rem) << 32)) % uo;}}
        return *this = sgn * lng(rem);}
    // T: O(n^2)
    friend pair<InfInt, InfInt> divModSlow(InfInt a, InfInt b) {
        assert(!b.isNil() && "InfInt divMod error: Division by zero.");
        a.sgn = 1; b.sgn = 1;
        if (a < b) { return {0, std::move(a)}; }
        if (b.n.size() == 1) {
            InfInt q; ulng r = 0; uint d = b.n[0];
            q.n.assign(a.n.size(), 0);
            ulll mgc = ((ulll(1) << 64) + d - 1) / d;
            for (int i = int(a.n.size()) - 1; i >= 0; i--) {
                ulng cur = (r << 32) | a.n[i];
                ulng qi = (cur * mgc) >> 64, ri = cur - qi * d;
                if (ri >= d) { qi--; ri += d; }
                q.n[i] = uint(qi); r = ri;}
            q.trim();
            return {std::move(q), InfInt(r)};}
        int norm = __builtin_clz(b.n.back());
        a <<= norm; b <<= norm;
        int n = a.n.size(), m = b.n.size();
        ulll mgc = ((ulll(1) << 64) + b.n.back() - 1) / b.n.back();
        InfInt q, r = std::move(a); 
        q.n.assign(n - m + 1, 0); r.n.push_back(0);
        const uint *b_ptr = b.n.data();
        uint *r_ptr = r.n.data(); uint *q_ptr = q.n.data();
        for (int i = n - m; i >= 0; i--) {
            ulng d = ((ulng(r_ptr[i + m]) << 32) | r_ptr[i + m - 1]) * mgc >> 64;
            if (d > 0xFFFFFFFFU) { d = 0xFFFFFFFFU; }
            if (d > 0) {
                ulng carry = 0;
                for (int j = 0; j < m; j++) {
                    ulng sub = ulng(b_ptr[j]) * d + carry;
                    ulng cur = r_ptr[i + j] - (sub & 0xFFFFFFFFULL);
                    r_ptr[i + j] = uint(cur); carry = (sub >> 32) + (cur >> 63);}
                ulng rem = r_ptr[i + m] - carry;
                r_ptr[i + m] = uint(rem);
                while (r_ptr[i + m] != 0) {
                    d--; ulng carry = 0;
                    for (int j = 0; j < m; j++) {
                        ulng cur = ulng(r_ptr[i + j]) + b_ptr[j] + carry;
                        r_ptr[i + j] = uint(cur); carry = cur >> 32;}
                    r_ptr[i + m] += uint(carry);}}
            q_ptr[i] = uint(d);}
        r.n.resize(m); r.trim(); r >>= norm; q.trim();
        return {std::move(q), std::move(r)};}
    InfInt &shiftBlocksLeft(int shf) {
        if (isNil() || shf <= 0) { return *this; }
        n.insert(n.begin(), shf, 0);
        return *this;}
    InfInt &shiftBlocksRight(int shf) {
        if (isNil() || shf <= 0) { return *this; }
        if (shf >= n.size()) { n.clear(); sgn = 1; return *this; }
        n.erase(n.begin(), n.begin() + shf);
        return *this;}
    static InfInt newtonRaphsonInv(const InfInt &a) {
        int len = a.n.size();
        if (len <= 256) {
            InfInt b; b.n.assign(2 * len + 1, 0); b.n.back() = 1;
            return divModSlow(std::move(b), a).first;}
        int md = (len + 2) / 2; 
        InfInt a_hi; a_hi.sgn = a.sgn; a_hi.n.assign(a.n.begin() + (len - md), a.n.end());
        InfInt b = newtonRaphsonInv(a_hi), cur;
        cur.n.assign(2 * md + 1, 0); cur.n.back() = 1;
        cur -= a_hi * b; cur.shiftBlocksLeft(len - md);
        InfInt a_lo; a_lo.sgn = a.sgn; a_lo.n.assign(a.n.begin(), a.n.begin() + (len - md)); a_lo.trim();
        cur -= a_lo * b; cur.shiftBlocksRight(md);
        cur *= b; cur.shiftBlocksRight(md);
        b.shiftBlocksLeft(len - md); b += cur;
        return b;}
    friend pair<InfInt, InfInt> divMod(InfInt a, InfInt b, const InfInt *b_inv = nullptr) {
        assert(!b.isNil() && "InfInt divMod error: Division by zero.");
        int a_sgn = a.sgn, b_sgn = b.sgn;
        a.sgn = 1; b.sgn = 1;
        if (a < b) { a.sgn = a_sgn; return {0, std::move(a)}; }
        if (b.n.size() <= 256) {
            auto [q, r] = divModSlow(std::move(a), std::move(b));
            q.sgn = a_sgn * b_sgn; r.sgn = a_sgn;
            q.trim(); r.trim();
            return {std::move(q), std::move(r)};}
        
        int norm = __builtin_clz(b.n.back());
        a <<= norm; b <<= norm;
        int n = a.n.size(), m = b.n.size(); InfInt q;
        if (b_inv && n <= 2 * m) {
            q = (*b_inv) * a;
            if (q.n.size() > 2 * m) { q.shiftBlocksRight(2 * m); } 
            else { q = 0; }}
        else {
            InfInt b_pad = b; b_pad.shiftBlocksLeft(n - m);
            q = newtonRaphsonInv(b_pad) * a;
            if (q.n.size() > n + m) { q.shiftBlocksRight(n + m); } 
            else { q = 0; }}
        InfInt r = std::move(a); r -= q * b;
        while (r.sgn == -1) { q--; r += b; }
        while (r >= b) { q++; r -= b; }
        r >>= norm;
        q.sgn = a_sgn * b_sgn; r.sgn = a_sgn;
        q.trim(); r.trim();
        return {std::move(q), std::move(r)};}
    InfInt &operator/=(const InfInt &o) { 
        if (is_inf || o.is_inf) {
            if (is_inf && o.is_inf) { return *this = sgn * o.sgn; }
            if (o.is_inf) { return *this = 0; }
            sgn *= o < 0 ? -1 : 1; n.clear(); 
            return *this;}
        return *this = divMod(std::move(*this), o).first;}
    InfInt &operator%=(const InfInt &o) { 
        if (is_inf) { return *this = 0; }
        if (o.is_inf) { return *this; }
        return *this = divMod(std::move(*this), o).second;}
    
    InfInt operator+() const { return *this; }
    InfInt operator-() const & { 
        InfInt res = *this; 
        if (!res.isNil()) { res.sgn = -res.sgn; }
        return res;}
    // T: O(1)
    InfInt operator-() && { 
        if (!isNil()) { sgn = -sgn; }
        return std::move(*this);}
    friend InfInt operator+(InfInt a, const InfInt &b) { a += b; return a; }
    friend InfInt operator-(InfInt a, const InfInt &b) { a -= b; return a; }
    friend InfInt operator*(InfInt a, lng b) { a *= b; return a; }
    friend InfInt operator*(lng b, InfInt a) { a *= b; return a; }
    friend InfInt operator*(InfInt a, const InfInt &b) { a *= b; return a; }
    friend InfInt operator/(InfInt a, lng b) { a /= b; return a; }
    friend InfInt operator/(InfInt a, const InfInt &b) { a /= b; return a; }
    friend InfInt operator%(InfInt a, lng b) { a %= b; return a; }
    friend InfInt operator%(InfInt a, const InfInt &b) { a %= b; return a; }

    friend InfInt abs(InfInt a) { a.sgn = 1; return a; }
    // T: O(n^2)
    friend InfInt exGcd(InfInt a, InfInt b, InfInt &x, InfInt &y) {
        if (a.is_inf) { x = 0; y = 1; return b; }
        if (b.is_inf) { x = 1; y = 0; return a; }
        x = 1, y = 0; InfInt X = 0, Y = 1;
        while (b != 0) {
            auto [q, r] = divMod(std::move(a), b);
            a = std::move(b); b = std::move(r);
            InfInt nx = std::move(x); nx -= q * X;
            x = std::move(X); X = std::move(nx);
            InfInt ny = std::move(y); ny -= q * Y;
            y = std::move(Y); Y = std::move(ny);}
        return a;}
    // T: O(n^2)
    friend InfInt gcd(InfInt a, InfInt b) {
        if ((a.is_inf && !b.isNil()) || a.isNil()) { return b; }
        if ((b.is_inf && !a.isNil()) || b.isNil()) { return a; }
        a.sgn = 1; b.sgn = 1;
        int a_shf = ctz(a), b_shf = ctz(b);
        a >>= a_shf; b >>= b_shf;
        while (!b.isNil()) {
            if (a > b) { std::swap(a, b); }
            b -= a;
            if (!b.isNil()) { b >>= ctz(b); }}
        a <<= min(a_shf, b_shf);
        return a;}
    // T: O(n^2)
    friend InfInt inv(InfInt a, InfInt mod) {
        a %= mod; 
        if (a.sgn == -1) { a += mod; }
        if (a == 0) { return -1; }
        InfInt x, y, g = exGcd(std::move(a), mod, x, y);
        if (g != 1) { return -1; }
        x %= mod; x += (x.sgn == -1) * mod;
        return x;}
    // T: O(n^2)
    friend InfInt lcm(InfInt a, InfInt b) {
        if (a.is_inf || b.is_inf) { return InfInt("inf"); }
        if (a.isNil() || b.isNil()) { return 0; }
        InfInt g = gcd(a, b);
        InfInt res = (std::move(a) / g) * b; res.sgn = 1;
        return res;}
    friend InfInt pow(InfInt a, lng b) {
        assert(b >= 0 && "InfInt pow error: Exponent must be >= 0.");
        if (a.is_inf) {
            if (b == 0) { return 1; }
            if (!(b & 1)) { a.sgn = 1; }
            return a;}
        InfInt res = 1;
        while (b > 0) { 
            if (b & 1) { res *= a; } 
            b >>= 1; 
            if (b > 0) { a *= a; }}
        return res;}
    // T: O(n^2 * log(n))
    friend InfInt powMod(InfInt a, InfInt b, InfInt mod) {
        assert(!mod.isNil() && "InfInt powMod error: Modulo by zero."); 
        if (mod == 1 || mod == -1) { return 0; }
        a %= mod; mod.sgn = 1;
        if (a.sgn == -1) { a += mod; }
        if (b.sgn == -1) { 
            a = inv(a, mod);
            if (a == -1) { return -1; }
            b.sgn = 1;}
        int bits = b.isNil() ? 0 : (b.n.size() * 32 - __builtin_clz(b.n.back()));
        InfInt b_norm = mod << __builtin_clz(mod.n.back());
        InfInt b_inv = newtonRaphsonInv(b_norm);
        InfInt res = 1;
        for (int i = 0; i < bits; i++) {
            if ((b.n[i >> 5] >> (i & 31)) & 1) {
                res *= a; res = divMod(std::move(res), mod, &b_inv).second;}
            if (i + 1 < bits) {
                a *= a; a = divMod(std::move(a), mod, &b_inv).second;}}
        return res;}
    // T: O(n^2 * log(n))
    friend InfInt sqrt(const InfInt &a) {
        assert(a.sgn == 1 && "InfInt sqrt error: Cannot compute square root of negative number."); 
        if (a.isNil()) { return 0; }
        if (a.is_inf) { return a; }
        InfInt res = 1; res <<= (a.n.size() * 32 - __builtin_clz(a.n.back()) + 1) / 2;
        while (true) {
            InfInt nxt = a / res; nxt += res; nxt >>= 1;
            if (nxt >= res) { break; }
            res = std::move(nxt);}
        return res;}
    static InfInt rand(int bits) {
        if (bits <= 0) { return 0; }
        static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());        
        int q = bits >> 5, r = bits & 31;
        InfInt res; res.sgn = 1; res.n.assign(q + (r > 0), 0);
        for (int i = 0; i < q; i++) { res.n[i] = uint(rng() & 0xFFFFFFFFU); }
        if (r > 0) { res.n.back() = uint(rng() & ((1U << r) - 1)) | (1U << (r - 1)); } 
        else if (q > 0) { res.n.back() |= (1U << 31); }
        res.trim();
        return res;}
    
    // T: O(1)
    bool isNil() const { return !is_inf && (n.empty() || (n.size() == 1 && n[0] == 0)); }    
    // T: O(1)
    explicit operator bool() const { return !isNil(); }
    // T: O(1)
    bool operator!() const { return isNil(); }
    friend std::strong_ordering magCmp(const InfInt &a, const InfInt &b) {
        if (a.is_inf && b.is_inf) { return std::strong_ordering::equal; }
        if (a.is_inf || b.is_inf) { return a.is_inf ? std::strong_ordering::greater : std::strong_ordering::less; }
        if (a.n.size() != b.n.size()) { return a.n.size() <=> b.n.size(); }
        for (int i = int(a.n.size()) - 1; i >= 0; i--) {
            if (a.n[i] != b.n[i]) { return a.n[i] <=> b.n[i]; }}
        return std::strong_ordering::equal;}
    friend std::strong_ordering operator<=>(const InfInt &a, const InfInt &b) {
        if (a.is_inf && b.is_inf) { return a.sgn <=> b.sgn; }
        if (a.is_inf || b.is_inf) { return a.sgn * a.is_inf <=> b.sgn * b.is_inf; }
        if (a.sgn != b.sgn) { return a.sgn <=> b.sgn; }
        return a.sgn == 1 ? magCmp(a, b) : magCmp(b, a);}
    friend bool operator==(const InfInt &a, const InfInt &b) { 
        if (a.is_inf || b.is_inf) { return a.sgn == b.sgn && a.is_inf == b.is_inf; }
        if (a.isNil() && b.isNil()) { return true; }
        return a.sgn == b.sgn && a.n == b.n;}
    
    InfInt &operator<<=(int shf) {
        if (is_inf || isNil() || shf == 0) { return *this; }
        int len = n.size(), q = shf >> 5, r = shf & 31;
        if (r == 0) { n.insert(n.begin(), q, 0); } 
        else {
            n.resize(len + q + 1, 0); 
            for (int i = len - 1; i >= 0; i--) {
                ulng cur = ulng(n[i]) << r;
                n[i + q + 1] |= uint(cur >> 32);
                n[i + q] = uint(cur);}
            for (int i = 0; i < q; i++) { n[i] = 0; }}
        trim();
        return *this;}
    InfInt &operator>>=(int shf) {
        if (is_inf || isNil() || shf == 0) { return *this; }
        int len = n.size(), q = shf >> 5, r = shf & 31;
        if (q >= len) { 
            n.clear(); 
            if (sgn == -1) { *this = -1; } 
            return *this;}
        bool carry = false;
        for (int i = 0; i < q; i++) { if (n[i] > 0) { carry = true; break; } }
        carry |= ((r > 0) & ((n[q] & ((1U << r) - 1)) > 0));
        if (r == 0) { n.erase(n.begin(), n.begin() + q); } 
        else {
            for (int i = 0; i < len - q; i++) {
                ulng cur = i + q + 1 < len ? n[i + q + 1] : 0;
                n[i] = uint((n[i + q] >> r) | (cur << (32 - r)));}
            n.resize(len - q);}
        if (sgn == -1 && carry) { *this -= 1; }
        trim();
        return *this;}
    vector<uint> convTwosComp() const {
        if (sgn == 1) { return n; }
        vector<uint> res(n.size() + 1); ulng carry = 1;
        for (int i = 0; i < n.size(); i++) {
            ulng cur = ulng(~n[i]) + carry;
            res[i] = uint(cur); carry = cur >> 32;}
        if (carry) { res.back() = uint(carry); } 
        else { res.pop_back(); }
        return res;}
    static InfInt fromTwosComp(vector<uint> blks, int ext) {
        int sgn = 1;
        if (ext != 0) {
            sgn = -1; ulng carry = 1;
            for (int i = 0; i < blks.size(); i++) {
                ulng cur = ulng(~blks[i]) + carry;
                blks[i] = uint(cur); carry = cur >> 32;}}
        InfInt res; res.sgn = sgn; res.n = std::move(blks); res.trim(); 
        return res;}
    friend InfInt operator&(InfInt a, const InfInt &b) {
        assert(!a.is_inf && !b.is_inf && "InfInt bitwise AND error: Bitwise AND on infinity.");
        if (a.sgn == -1) {
            ulng carry = 1;
            for (int i = 0; i < a.n.size(); i++) {
                ulng cur = ulng(~a.n[i]) + carry;
                a.n[i] = uint(cur); carry = cur >> 32;}
            if (carry) { a.n.push_back(uint(carry)); }}
        uint a_ext = a.sgn == -1 ? 0xFFFFFFFFU : 0;
        uint b_ext = b.sgn == -1 ? 0xFFFFFFFFU : 0;
        auto B = b.convTwosComp();
        int len = max(a.n.size(), B.size());
        a.n.resize(len, a_ext);
        for (int i = 0; i < len; i++) { a.n[i] &= i < B.size() ? B[i] : b_ext; }
        return fromTwosComp(std::move(a.n), a_ext & b_ext);}
    friend InfInt operator^(InfInt a, const InfInt &b) {
        assert(!a.is_inf && !b.is_inf && "InfInt bitwise XOR error: Bitwise XOR on infinity.");
        if (a.sgn == -1) {
            ulng carry = 1;
            for (int i = 0; i < a.n.size(); i++) {
                ulng cur = ulng(~a.n[i]) + carry;
                a.n[i] = uint(cur); carry = cur >> 32;}
            if (carry) { a.n.push_back(uint(carry)); }}
        uint a_ext = a.sgn == -1 ? 0xFFFFFFFFU : 0;
        uint b_ext = b.sgn == -1 ? 0xFFFFFFFFU : 0;
        auto B = b.convTwosComp();
        int len = max(a.n.size(), B.size());
        a.n.resize(len, a_ext);
        for (int i = 0; i < len; i++) { a.n[i] ^= i < B.size() ? B[i] : b_ext; }
        return fromTwosComp(std::move(a.n), a_ext ^ b_ext);}
    friend InfInt operator|(InfInt a, const InfInt &b) {
        assert(!a.is_inf && !b.is_inf && "InfInt bitwise OR error: Bitwise OR on infinity.");
        if (a.sgn == -1) {
            ulng carry = 1;
            for (int i = 0; i < a.n.size(); i++) {
                ulng cur = ulng(~a.n[i]) + carry;
                a.n[i] = uint(cur); carry = cur >> 32;}
            if (carry) { a.n.push_back(uint(carry)); }}
        uint a_ext = a.sgn == -1 ? 0xFFFFFFFFU : 0;
        uint b_ext = b.sgn == -1 ? 0xFFFFFFFFU : 0;
        auto B = b.convTwosComp();
        int len = max(a.n.size(), B.size());
        a.n.resize(len, a_ext);
        for (int i = 0; i < len; i++) { a.n[i] |= i < B.size() ? B[i] : b_ext; }
        return fromTwosComp(std::move(a.n), a_ext | b_ext);}
    
    InfInt operator~() const { return -(*this) - 1; }  
    friend InfInt operator<<(InfInt a, int shf) { a <<= shf; return a; }
    friend InfInt operator>>(InfInt a, int shf) { a >>= shf; return a; }
    InfInt &operator&=(const InfInt &o) { return *this = std::move(*this) & o; }
    InfInt &operator^=(const InfInt &o) { return *this = std::move(*this) ^ o; }
    InfInt &operator|=(const InfInt &o) { return *this = std::move(*this) | o; }

    friend string bin(const InfInt &a) {
        if (a.is_inf) { return (a.sgn == -1 ? "-inf" : "inf"); }
        if (a.isNil()) { return "0"; }
        string res; res.reserve(a.n.size() * 32 + 1);
        for (int i = 0; i < a.n.size(); i++) {
            uint cur = a.n[i];
            for (int j = 0; j < 32; j++) { 
                res += char('0' + (cur & 1)); cur >>= 1;}}
        while (res.size() > 1 && res.back() == '0') { res.pop_back(); }
        if (a.sgn == -1) { res += '-'; }
        reverse(res.begin(), res.end());
        return res;}
    friend string oct(const InfInt &a) {
        if (a.is_inf) { return (a.sgn == -1 ? "-inf" : "inf"); }
        if (a.isNil()) { return "0"; }
        int bits = 0; ulng cur = 0;
        string res; res.reserve(a.n.size() * 11 + 1); 
        for (int i = 0; i < a.n.size(); i++) {
            cur |= ulng(a.n[i]) << bits; bits += 32;
            while (bits >= 3) {
                res += char('0' + (cur & 7));
                cur >>= 3; bits -= 3;}}
        if (bits > 0) { res += char('0' + cur); }
        while (res.size() > 1 && res.back() == '0') { res.pop_back(); }
        if (a.sgn == -1) { res += '-'; }
        reverse(res.begin(), res.end());
        return res;}
    friend string hex(const InfInt &a) {
        if (a.is_inf) { return (a.sgn == -1 ? "-inf" : "inf"); }
        if (a.isNil()) { return "0"; }
        string res; res.reserve(a.n.size() * 8 + 1);
        for (int i = 0; i < a.n.size(); i++) {
            ulng cur = a.n[i];
            cur = (cur | (cur << 16)) & 0x0000FFFF0000FFFFULL;
            cur = (cur | (cur << 8))  & 0x00FF00FF00FF00FFULL;
            cur = (cur | (cur << 4))  & 0x0F0F0F0F0F0F0F0FULL;
            cur = (cur + 0x3030303030303030ULL) + 
                  (((cur + 0x3636363636363636ULL) & 0x4040404040404040ULL) >> 6) * 7;
            char buf[8]; std::memcpy(buf, &cur, 8);
            res.append(buf, 8);}
        while (res.size() > 1 && res.back() == '0') { res.pop_back(); }
        if (a.sgn == -1) { res += '-'; }
        reverse(res.begin(), res.end());
        return res;}
    friend int ctz(const InfInt &a) {
        assert(!a.is_inf && "InfInt ctz error: ctz on infinity.");
        if (a.isNil()) { return 0; }
        int cnt = 0;
        for (uint x : a.n) {
            if (x == 0) { cnt += 32; } 
            else { cnt += __builtin_ctz(x); break; }}
        return cnt;}
    friend int popcount(const InfInt &a) {
        assert(!a.is_inf && "InfInt popcount error: popcount on infinity.");
        int cnt = 0;
        for (uint x : a.n) { cnt += __builtin_popcount(x); }
        return cnt;}
    
    // T: O(1), M: O(1)
    struct SetBase { uint base; SetBase(uint Base) : base(Base) {} };
    // T: O(1)
    static int getBaseId() { static const int id = std::ios_base::xalloc(); return id; }
    friend istream &operator>>(istream &is, SetBase sb) { is.iword(getBaseId()) = sb.base; return is; }
    // T: O(n * log(n)^2)
    friend istream &operator>>(istream &is, InfInt &a) {
        is >> std::ws;
        if (is.peek() == EOF) { return is; }    
        uint base = is.iword(getBaseId());
        base = base == 0 ? 10 : base;
        if (is.flags() & std::ios_base::oct) { base = 8; } 
        else if (is.flags() & std::ios_base::hex) { base = 16; }
        static string s; s.clear();
        is >> s;
        if (s.find('[') != string::npos && s.back() != ']') {
            int c;
            while ((c = is.get()) != EOF) {
                s += char(c);
                if (c == ']') { break; }}}
        a.read(s, base);
        return is;}
    // T: O(1)
    friend ostream &operator<<(ostream &os, SetBase sb) { os.iword(getBaseId()) = sb.base; return os; }
    // T: O(n * log(n)^2)
    friend ostream &operator<<(ostream &os, const InfInt &a) {
        uint base = os.iword(getBaseId());
        base = base == 0 ? 10 : base;
        if (os.flags() & std::ios_base::oct) { base = 8; } 
        else if (os.flags() & std::ios_base::hex) { base = 16; }
        return os << a.toString(base);}
};
using iint = InfInt;
#line 2 "X-Tests Abbreviated\\Yosupo-BigInteger-04-AddHexBigInt.cpp"

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;
    while (t--) {
        iint a, b;
        cin >> iint::SetBase(16) >> a >> b;
        a += b;
        cout << iint::SetBase(16) << a << '\n';
    }

    return 0;
}
