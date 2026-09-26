// https://judge.yosupo.jp/problem/find_linear_recurrence
// based on: https://judge.yosupo.jp/submission/282926

#include <bits/stdc++.h>
#define LL long long
#define LLL __int128
#define uint unsigned
#define ldb long double
#define uLL unsigned long long
using namespace std;
namespace BasicMath {
typedef vector<int> poly;
typedef vector<int> Vec;
typedef vector<Vec> Mat;
typedef tuple<poly, poly, poly, poly> Mat2;
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());
const int Mod = 998244353, Mod_G = 3;
const LL Mod2 = (LL)Mod * Mod;
poly frc({1, 1}), inv({0, 1}), ivf({1, 1});
inline int qpow(int x, int y, int z = 1) {
    for (; y; (y >>= 1) && (x = (LL)x * x % Mod))
        if (y & 1) z = (LL)z * x % Mod;
    return z;
}
inline void Init(const int& n) {
    for (int i = frc.size(); i <= n; ++i)
        frc.emplace_back((LL)frc.back() * i % Mod), inv.emplace_back(Mod - Mod / i * (LL)inv[Mod % i] % Mod),
            ivf.emplace_back((LL)ivf.back() * inv.back() % Mod);
}
inline int Binom(const int& n, const int& m) {
    if (n < m || m < 0) return 0;
    return Init(n), (LL)frc[n] * ivf[m] % Mod * ivf[n - m] % Mod;
}
inline poly invLinear(poly P) {
    const int n = P.size();
    poly Q(n + 1, 1);
    for (int i = 0; i < n; ++i)
        Q[i + 1] = (LL)Q[i] * P[i] % Mod;
    int t = qpow(Q[n], Mod - 2);
    Q.pop_back();
    for (int i = n; i--;)
        Q[i] = (LL)Q[i] * t % Mod, t = (LL)t * P[i] % Mod;
    return Q;
}
inline uLL trans(const uLL& x) {
    constexpr uLL A = -(uLL)Mod / Mod + 1;
    constexpr uLL Q = (((__uint128_t)(-(uLL)Mod % Mod) << 64) + Mod - 1) / Mod;
    return x * A + (uLL)((__uint128_t)x * Q >> 64) + 1;
}
inline uLL mul(const uLL& x, const uLL& y) {
    return x * y * (__uint128_t)Mod >> 64;
}
inline int add(int x, const int& y) {
    return ((x += y) - Mod) >= 0 ? x - Mod : x;
}
inline int sub(int x, const int& y) {
    return (x -= y) < 0 ? x + Mod : x;
}
inline int neg(const int& x) {
    return x ? Mod - x : 0;
}
inline int div2(const int& x) {
    return x & 1 ? (x + Mod) >> 1 : x >> 1;
}
} // namespace BasicMath
namespace Polynomial {
using namespace BasicMath;
vector<uLL> Grt, iGrt;
inline bool Empty(poly& P) {
    for (; !P.empty() && !P.back(); P.pop_back())
        ;
    return P.empty();
}
inline poly Slice(poly& P, int l, int r) {
    if (r <= 0 || l >= (int)P.size()) return poly(r - l);
    if (0 <= l && r <= (int)P.size()) return poly(P.begin() + l, P.begin() + r);
    poly Q;
    if (l < 0) Q.insert(Q.end(), -l, 0), l = 0;
    if (r <= (int)P.size())
        Q.insert(Q.end(), P.begin() + l, P.begin() + r);
    else
        Q.insert(Q.end(), P.begin() + l, P.end()), Q.insert(Q.end(), r - P.size(), 0);
    return Q;
}
inline void Reduce(poly& P, int n) {
    for (int i = P.size() - 1; i >= n; --i)
        P[i - n] = add(P[i - n], P[i]);
    P.resize(n);
}
inline poly Add(poly P, poly Q) {
    if (P.size() < Q.size()) P.swap(Q);
    for (int i = Q.size(); i--;)
        P[i] = add(P[i], Q[i]);
    return P;
}
inline poly Add_Empty(poly P, poly Q) {
    if (P.size() < Q.size()) P.swap(Q);
    for (int i = Q.size(); i--;)
        P[i] = add(P[i], Q[i]);
    return Empty(P), P;
}
inline poly Sub(poly P, poly Q) {
    if (P.size() < Q.size()) P.resize(Q.size());
    for (int i = Q.size(); i--;)
        P[i] = sub(P[i], Q[i]);
    return P;
}
inline poly Sub_Empty(poly P, poly Q) {
    if (P.size() < Q.size()) P.resize(Q.size());
    for (int i = Q.size(); i--;)
        P[i] = sub(P[i], Q[i]);
    return Empty(P), P;
}
inline poly Mulx(poly P, int x) {
    const uLL v = trans(x);
    for (int& i : P)
        i = mul(i, v);
    return P;
}
inline poly Neg(poly P) {
    for (int i = P.size(); i--;)
        P[i] && (P[i] = Mod - P[i]);
    return P;
}
inline int Eval(poly& P, int x) {
    int z = 0;
    for (int i = P.size(); i--;)
        z = ((LL)z * x + P[i]) % Mod;
    return z;
}
inline void extend(const int& n) {
    if (Grt.empty()) Grt.emplace_back(trans(1)), iGrt.emplace_back(trans(1));
    if ((int)Grt.size() < n) {
        int L = Grt.size();
        for (Grt.resize(n), iGrt.resize(n); L < n; L *= 2) {
            const int w = qpow(Mod_G, Mod / (L * 4)), iw = qpow(w, Mod - 2);
            for (int i = 0; i < L; ++i)
                Grt[i + L] = trans(mul(Grt[i], w)), iGrt[i + L] = trans(mul(iGrt[i], iw));
        }
    }
}
template<int A, int B, int C = 0, class fun>
inline void Butterrep(int i, int j, int k, fun F) {
    if (A != C) F(i, j + C / B, k + C * 2 - C % B), Butterrep<A, B, C + (C < A)>(i, j, k, F);
}
template<int i, class fun>
inline void Butter(int n, fun F) {
    if (n > 32)
        for (int j = 0; 2 * i * j < n; j += 32 / i)
            Butterrep<32, i>(i, j, 2 * i * j, F);
    else if (i < n)
        for (int j = 0; 2 * i * j < n; ++j)
            for (int k = 0; k < i; ++k)
                F(i, j, k + 2 * i * j);
}
template<class T>
inline void DFT(T P, int n) {
    extend(n);
    const auto F = [&](int x, int y, int z) {
        const int a = P[z], b = mul(P[z + x], Grt[y]);
        P[z] = add(a, b), P[z + x] = sub(a, b);
    };
    for (int i = n >> 1; i > 16; i >>= 1)
        for (int j = 0; 2 * i * j < n; ++j)
            for (int k = 0; k < i; k += 32)
                Butterrep<32, 32>(i, j, k + 2 * i * j, F);
    Butter<16>(n, F), Butter<8>(n, F), Butter<4>(n, F), Butter<2>(n, F), Butter<1>(n, F);
}
template<class T>
inline void IDFT(T P, int n) {
    const uLL ni = trans(Mod - (Mod - 1) / n);
    for (int i = 0; i < n; ++i)
        P[i] = mul(P[i], ni);
    extend(n);
    const auto F = [&](int x, int y, int z) {
        const int a = P[z], b = P[z + x];
        P[z] = add(a, b), P[z + x] = mul(a - b + Mod, iGrt[y]);
    };
    Butter<1>(n, F), Butter<2>(n, F), Butter<4>(n, F), Butter<8>(n, F), Butter<16>(n, F);
    for (int i = 32; i < n; i <<= 1)
        for (int j = 0; 2 * i * j < n; ++j)
            for (int k = 0; k < i; k += 32)
                Butterrep<32, 32>(i, j, k + 2 * i * j, F);
}
template<class T>
inline void rDFT(T P, int n) {
    extend(n);
    const auto F = [&](int x, int y, int z) {
        const int a = P[z], b = mul(P[z + x], Grt[y]);
        P[z] = add(a, b), P[z + x] = sub(a, b);
    };
    for (int i = n >> 1, t = 1; i; i >>= 1, t <<= 1)
        for (int j = 0; 2 * i * j < n; ++j)
            for (int k = 0; k < i; ++k)
                F(i, j + t, k + 2 * i * j);
}
template<class T>
inline void rIDFT(T P, int n) {
    const uLL ni = trans(Mod - (Mod - 1) / n);
    for (int i = 0; i < n; ++i)
        P[i] = mul(P[i], ni);
    extend(n);
    const auto F = [&](int x, int y, int z) {
        const int a = P[z], b = P[z + x];
        P[z] = add(a, b), P[z + x] = mul(a - b + Mod, iGrt[y]);
    };
    for (int i = 1, t = n >> 1; i < n; i <<= 1, t >>= 1)
        for (int j = 0; 2 * i * j < n; ++j)
            for (int k = 0; k < i; ++k)
                F(i, j + t, k + 2 * i * j);
}
inline void DFT(poly& P) {
    DFT(P.begin(), P.size());
}
inline void IDFT(poly& P) {
    IDFT(P.begin(), P.size());
}
inline void rDFT(poly& P) {
    rDFT(P.begin(), P.size());
}
inline void rIDFT(poly& P) {
    rIDFT(P.begin(), P.size());
}
poly Mul(poly P, poly Q) {
    if (P.empty() || Q.empty()) return poly();
    const int pn = P.size(), qn = Q.size(), rn = pn + qn - 1;
    if (min(pn, qn) <= 32 || max(pn, qn) <= 64) {
        if (pn <= qn) {
            vector<uLL> H(rn);
            for (int i = 0; i < pn; ++i) {
                if (i % 8 == 0)
                    for (int j = qn; j--;)
                        (H[i + j] += 1ll * P[i] * Q[j]) >= (Mod2 << 3) && (H[i + j] -= Mod2 << 3);
                else
                    for (int j = qn; j--;)
                        H[i + j] += 1ll * P[i] * Q[j];
            }
            Q.resize(rn);
            for (int i = rn; i--;)
                Q[i] = H[i] % Mod;
            return Q;
        } else {
            vector<uLL> H(rn);
            for (int i = 0; i < qn; ++i) {
                if (i % 8 == 0)
                    for (int j = pn; j--;)
                        (H[i + j] += 1ll * Q[i] * P[j]) >= (Mod2 << 3) && (H[i + j] -= Mod2 << 3);
                else
                    for (int j = pn; j--;)
                        H[i + j] += 1ll * Q[i] * P[j];
            }
            P.resize(rn);
            for (int i = rn; i--;)
                P[i] = H[i] % Mod;
            return P;
        }
    }
    if (rn <= 256) {
        const int k = max(pn, qn) / 2;
        poly A = (k < pn ? poly(P.begin() + k, P.end()) : poly());
        poly B = (k < pn ? poly(P.begin(), P.begin() + k) : P);
        poly C = (k < qn ? poly(Q.begin() + k, Q.end()) : poly());
        poly D = (k < qn ? poly(Q.begin(), Q.begin() + k) : Q);
        poly AC = Mul(A, C), BD = Mul(B, D), H = Sub(Mul(Add(A, B), Add(C, D)), Add(AC, BD));
        AC.insert(AC.begin(), k * 2, 0), H.insert(H.begin(), k, 0);
        H = Add(AC, Add(H, BD));
        return H.resize(rn), H;
    }
    const int m = 2 << __lg(max(1, rn - 1));
    P.resize(m), Q.resize(m);
    DFT(P), DFT(Q);
    for (int i = m; i--;)
        P[i] = (LL)P[i] * Q[i] % Mod;
    IDFT(P);
    return P.resize(rn), P;
}
poly MulT(poly P, poly Q) {
    if (P.empty() || Q.empty()) return poly();
    reverse(Q.begin(), Q.end());
    const int pn = P.size(), qn = Q.size(), m = 2 << __lg(max(1, pn - 1));
    P.resize(m), Q.resize(m);
    DFT(P), DFT(Q);
    for (int i = m; i--;)
        P[i] = (LL)P[i] * Q[i] % Mod;
    IDFT(P);
    return Slice(P, qn - 1, pn);
}
poly Inv(poly P) {
    const int pn = P.size();
    const int m = 2 << __lg(max(1, pn - 1));
    poly Q({qpow(P[0], Mod - 2)}), F, dQ;
    Q.reserve(m);
    for (int n = 1; n < m; n *= 2) {
        F = Slice(P, 0, n * 2), dQ = Q;
        dQ.resize(n * 2), DFT(dQ), DFT(F);
        for (int i = 0; i < n * 2; ++i)
            F[i] = (LL)(Mod - F[i]) * dQ[i] % Mod;
        IDFT(F), fill_n(F.begin(), n, 0), DFT(F);
        for (int i = 0; i < n * 2; ++i)
            dQ[i] = (LL)F[i] * dQ[i] % Mod;
        IDFT(dQ), Q.insert(Q.end(), dQ.begin() + n, dQ.end());
    }
    return Q.resize(pn), Q;
}
poly Quo(poly F, poly P) {
    const int pn = P.size();
    if (pn <= 64) {
        const uLL r = trans(qpow(P[0], Mod - 2));
        for (int i = 0; i < pn; ++i) {
            LLL v = F[i];
            for (int j = 0; j < i; ++j)
                v -= (LLL)F[j] * P[i - j];
            if ((F[i] = v % Mod) < 0) F[i] += Mod;
            F[i] = mul(F[i], r);
        }
        return F;
    }
    const int BL = max(1, __lg(pn)), m = 2 << __lg(max(1, (pn - 1) / BL)), L = (pn - 1) / m + 1;
    poly H(Slice(F, 0, m)), Q = Inv(Slice(P, 0, m));
    vector<poly> A(L), B(L - 1);
    Q.resize(m * 2), H.resize(m * 2), DFT(Q), DFT(H);
    for (int i = 0; i < m * 2; ++i)
        H[i] = (LL)H[i] * Q[i] % Mod;
    IDFT(H), H.resize(m);
    A[0] = Slice(P, 0, m), A[0].resize(m * 2), DFT(A[0]);
    for (int k = 1; k < L; ++k) {
        A[k] = Slice(P, k * m, (k + 1) * m), A[k].resize(m * 2), DFT(A[k]);
        B[k - 1] = Slice(H, (k - 1) * m, k * m), B[k - 1].resize(m * 2), DFT(B[k - 1]);
        poly C(m * 2);
        for (int j = 0; j < k; ++j) {
            for (int i = 0; i < m; ++i)
                C[i] = (C[i] + (LL)(A[k - j][i] + A[k - 1 - j][i]) * (Mod - B[j][i])) % Mod;
            for (int i = m; i < m * 2; ++i)
                C[i] = (C[i] + (LL)(A[k - j][i] + Mod - A[k - 1 - j][i]) * (Mod - B[j][i])) % Mod;
        }
        IDFT(C), fill_n(C.begin() + m, m, 0), C = Add(C, Slice(F, k * m, (k + 1) * m)), DFT(C);
        for (int i = 0; i < m * 2; ++i)
            C[i] = (LL)C[i] * Q[i] % Mod;
        IDFT(C), H.insert(H.end(), C.begin(), C.begin() + m);
    }
    return H.resize(pn), H;
}
poly Div(poly, poly);
pair<poly, poly> DivMod(poly, poly);
poly ModPow(poly, LL, poly);
Mat2 Hgcd(poly, poly, int);
poly Gcd(poly, poly);
poly Qpow(poly, int);
} // namespace Polynomial
namespace Polynomial {
poly Deriv(poly P) {
    const int n = P.size();
    for (int i = 1; i < n; ++i)
        P[i - 1] = (LL)P[i] * i % Mod;
    return P.pop_back(), P;
}
poly Integ(poly P) {
    P.emplace_back(0);
    const int n = P.size();
    Init(n);
    for (int i = n; --i;)
        P[i] = (LL)P[i - 1] * inv[i] % Mod;
    return P[0] = 0, P;
}
poly Ln(poly P) {
    const int n = P.size();
    poly Q = Deriv(P);
    Q.resize(n), Q = Quo(Q, P);
    return Q.resize(n - 1), Integ(Q);
}
template<bool op>
pair<poly, poly> Expi(poly P) {
    const int pn = P.size();
    const int m = 2 << __lg(max(1, pn - 1));
    P.resize(m);
    poly Q({1}), H({1}), dQ({1}), nF, nQ, dH, dnF;
    Q.reserve(m), H.reserve(m), dQ.reserve(m);
    for (int n = 1; n < m; n *= 2) {
        nF = Deriv(Slice(P, 0, n)), dnF = nF, dnF.resize(n), DFT(dnF), nQ.resize(n);
        for (int i = 0; i < n; ++i)
            nQ[i] = (LL)dnF[i] * dQ[i] % Mod;
        IDFT(nQ), nQ = Sub(Deriv(Q), nQ), nQ.insert(nQ.begin(), n, 0), swap(nQ[n - 1], nQ[n * 2 - 1]);
        DFT(nQ), dH = H, dH.resize(n * 2), DFT(dH);
        for (int i = 0; i < n * 2; ++i)
            nQ[i] = (LL)nQ[i] * dH[i] % Mod;
        IDFT(nQ), copy(nF.begin(), nF.end(), nQ.begin());
        nQ = Sub(Integ(nQ), Slice(P, 0, n * 2)), nQ.resize(n * 2), DFT(nQ);
        dQ.insert(dQ.end(), Q.begin(), Q.end()), rDFT(dQ.begin() + n, n);
        for (int i = 0; i < n * 2; ++i)
            nQ[i] = (LL)(Mod - nQ[i]) * dQ[i] % Mod;
        IDFT(nQ), Q.insert(Q.end(), nQ.begin() + n, nQ.end());
        if (!op && n * 2 == m) break;
        dQ = Q, DFT(dQ);
        for (int i = 0; i < n * 2; ++i)
            nQ[i] = (LL)(Mod - dQ[i]) * dH[i] % Mod;
        IDFT(nQ), fill_n(nQ.begin(), n, 0), DFT(nQ);
        for (int i = 0; i < n * 2; ++i)
            nQ[i] = (LL)nQ[i] * dH[i] % Mod;
        IDFT(nQ), H.insert(H.end(), nQ.begin() + n, nQ.end());
    }
    return Q.resize(pn), H.resize(pn), make_pair(Q, H);
}
poly Exp(poly P) {
    const int pn = P.size();
    if (pn <= 64) return Expi<0>(P).first;
    const int BL = max(1, __lg(pn)), m = 2 << __lg(max(1, (pn - 1) / BL)), L = (pn - 1) / m + 1;
    auto [Q, H] = Expi<1>(Slice(P, 0, m));
    H.resize(m * 2), DFT(H);
    vector<poly> A(L), B(L - 1);
    P.resize(m * L), Init(m * (L + 1));
    for (int i = 0; i < pn; ++i)
        P[i] = (LL)P[i] * i % Mod;
    A[0] = poly(Slice(P, 0, m)), A[0].resize(m * 2), DFT(A[0]);
    for (int k = 1; k < L; ++k) {
        A[k] = Slice(P, k * m, (k + 1) * m), A[k].resize(m * 2), DFT(A[k]);
        B[k - 1] = Slice(Q, (k - 1) * m, k * m), B[k - 1].resize(m * 2), DFT(B[k - 1]);
        poly C(m * 2);
        for (int j = 0; j < k; ++j) {
            for (int i = 0; i < m; ++i)
                C[i] = (C[i] + (LL)(A[k - j][i] + A[k - 1 - j][i]) * B[j][i]) % Mod;
            for (int i = m; i < m * 2; ++i)
                C[i] = (C[i] + (LL)(A[k - j][i] + Mod - A[k - 1 - j][i]) * B[j][i]) % Mod;
        }
        IDFT(C), fill_n(C.begin() + m, m, 0), DFT(C);
        for (int i = 0; i < m * 2; ++i)
            C[i] = (LL)C[i] * H[i] % Mod;
        IDFT(C), fill_n(C.begin() + m, m, 0);
        for (int i = 0; i < m * 2; ++i)
            C[i] = (LL)C[i] * inv[m * k + i] % Mod;
        DFT(C);
        for (int i = 0; i < m * 2; ++i)
            C[i] = (LL)C[i] * B[0][i] % Mod;
        IDFT(C), Q.insert(Q.end(), C.begin(), C.begin() + m);
    }
    return Q.resize(pn), Q;
}
poly Qpow(poly P, int k) {
    return Exp(Mulx(Ln(P), k));
}
poly SafePow(poly P, int k_mod_p, int k_mod_phi, int k_chk_mn) {
    const int n = P.size();
    int i = 0;
    while (i < n && !P[i])
        ++i;
    if (1ll * i * k_chk_mn >= n) return poly(n);
    P = Slice(P, i, i + (n - i * k_chk_mn));
    const int r = P[0];
    P = Mulx(P, qpow(r, Mod - 2));
    P = Qpow(P, k_mod_p), P = Mulx(P, qpow(r, k_mod_phi));
    return P.insert(P.begin(), i * k_chk_mn, 0), P;
}
template<class T>
poly SafePow(poly P, T k) {
    return SafePow(P, k % Mod, k % (Mod - 1), k < P.size() ? k : P.size());
}
poly ModPow(poly X, LL k, poly P) {
    const int m = P.size();
    if (m == 1) return poly();
    poly nP = P;
    reverse(nP.begin(), nP.end()), nP = Inv(nP);
    const int L = 2 << __lg(m * 2 - 1);
    P.resize(L), nP.resize(L), DFT(P), DFT(nP);
    int Xn = X.size();
    X.resize(L), DFT(X);
    const auto MoD = [&](poly F) {
        poly nF = F;
        IDFT(nF);
        nF.resize(m * 2 - 3), reverse(nF.begin(), nF.end()), nF.resize(m - 2);
        nF.resize(L), DFT(nF);
        for (int i = 0; i < L; ++i)
            nF[i] = (LL)nF[i] * nP[i] % Mod;
        IDFT(nF), nF.resize(m - 2), reverse(nF.begin(), nF.end());
        nF.resize(L), DFT(nF);
        for (int i = 0; i < L; ++i)
            nF[i] = (LL)nF[i] * P[i] % Mod;
        return Sub(F, nF);
    };
    poly Q({1});
    int Qn = Q.size();
    Q.resize(L), DFT(Q);
    while (k) {
        if (k & 1) {
            for (int i = 0; i < L; ++i)
                Q[i] = (LL)Q[i] * X[i] % Mod;
            Qn += Xn - 1;
            if (Qn >= m) Q = MoD(Q), Qn = m - 1;
        }
        if (k >>= 1) {
            for (int i = 0; i < L; ++i)
                X[i] = (LL)X[i] * X[i] % Mod;
            Xn += Xn - 1;
            if (Xn >= m) X = MoD(X), Xn = m - 1;
        }
    }
    return IDFT(Q), Q.resize(m - 1), Q;
}
} // namespace Polynomial
namespace Polynomial {
poly Div(poly P, poly Q) {
    const int n = P.size(), m = Q.size();
    if (n < m) return poly();
    if (n - m + 1 > m * 2 && m <= 16) {
        poly H(n - m + 1);
        vector<uLL> nQ(m - 1);
        for (int i = 0; i < m - 1; ++i)
            nQ[i] = trans(Q[i]);
        const uLL v = trans(qpow(Q.back(), Mod - 2));
        for (int i = n - m + 1; i--;) {
            H[i] = mul(P[i + m - 1], v);
            for (int j = max(0, m - 1 - i); j < m - 1; ++j)
                P[i + j] = sub(P[i + j], mul(H[i], nQ[j]));
        }
        return H;
    }
    reverse(P.begin(), P.end());
    reverse(Q.begin(), Q.end());
    P.resize(n - m + 1), Q.resize(n - m + 1);
    P = Quo(P, Q), reverse(P.begin(), P.end());
    return P;
}
pair<poly, poly> DivMod(poly P, poly Q) {
    const int n = P.size(), m = Q.size();
    if (n < m) return make_pair(poly(), P);
    if (min(m, n - m + 1) <= 64) {
        poly H(n - m + 1);
        vector<uLL> nQ(m - 1);
        for (int i = 0; i < m - 1; ++i)
            nQ[i] = trans(Q[i]);
        const uLL v = trans(qpow(Q.back(), Mod - 2));
        for (int i = n - m + 1; i--;) {
            H[i] = mul(P[i + m - 1], v);
            for (int j = 0; j < m - 1; ++j)
                P[i + j] = sub(P[i + j], mul(H[i], nQ[j]));
        }
        return P.resize(m - 1), Empty(P), make_pair(H, P);
    }
    poly H = Div(P, Q), R = H;
    const int q = 2 << __lg(max(1, m - 2));
    Reduce(P, q), Reduce(Q, q), Reduce(R, q), DFT(Q), DFT(R);
    for (int i = 0; i < q; ++i)
        Q[i] = (LL)Q[i] * R[i] % Mod;
    return IDFT(Q), make_pair(H, Sub_Empty(P, Q));
}
} // namespace Polynomial
namespace Polynomial {
vector<poly> _EIBuild(poly Q) {
    const int n = Q.size(), lgm = __lg(max(1, n - 1)) + 1, m = 1 << lgm;
    vector<poly> T(lgm + 1, poly(m * 2));
    Q.resize(m);
    for (int i = 0; i < m; ++i)
        T[0][i * 2] = Mod + 1 - Q[i], T[0][i * 2 + 1] = Mod - 1 - Q[i];
    for (int d = 0, i = 2; d < lgm; ++d, i *= 2) {
        poly A(i);
        for (int j = 0; j < m * 2; j += i * 2) {
            for (int k = 0; k < i; ++k)
                A[k] = (LL)T[d][j + k] * T[d][j + i + k] % Mod;
            if (d < lgm - 1)
                copy_n(A.begin(), i, T[d + 1].begin() + j), IDFT(A), A[0] = sub(A[0], 2), rDFT(A), copy_n(A.begin(), i, T[d + 1].begin() + j + i);
            else
                IDFT(A), copy_n(A.begin(), i, T[d + 1].begin() + j), T[d + 1][j] = sub(T[d + 1][j], 1), T[d + 1][i] = add(T[d + 1][i], 1);
        }
    }
    return T;
}
poly _Eval(poly P, int n, vector<poly> T) {
    const int pn = P.size(), lgm = T.size() - 1, m = 1 << lgm;
    poly Q = T[lgm];
    if (pn > n) P = DivMod(P, Slice(T[lgm], m - n, m + 1)).second;
    P.resize(m * 2), rotate(P.begin(), P.end() - 1, P.end());
    reverse(Q.begin(), Q.begin() + m + 1), Q.resize(m), Q = Inv(Q);
    reverse(Q.begin(), Q.end()), Q.resize(m * 2), DFT(P), DFT(Q);
    for (int i = 0; i < m * 2; ++i)
        Q[i] = (LL)P[i] * Q[i] % Mod;
    for (int d = lgm, i = m; d--; i >>= 1) {
        poly A(i);
        for (int j = 0; j < m * 2; j += i * 2) {
            copy_n(Q.begin() + i + j, i, A.begin());
            rIDFT(A), DFT(A), copy_n(A.begin(), i, Q.begin() + i + j);
            for (int k = 0; k < i; ++k) {
                const int w = sub(Q[j + k], Q[i + j + k]);
                Q[j + k] = (LL)w * T[d][i + j + k] % Mod, Q[i + j + k] = (LL)w * T[d][j + k] % Mod;
            }
        }
    }
    const int ni = Mod - (Mod - 1) / (m * 2);
    for (int i = 0; i < n; ++i)
        P[i] = (LL)(Q[i * 2] - Q[i * 2 + 1] + Mod) * ni % Mod;
    return P.resize(n), P;
}
poly Eval(poly P, poly Q) {
    return _Eval(P, Q.size(), _EIBuild(Q));
}
poly Inter(poly P, poly Q) {
    const int n = P.size();
    vector<poly> T = _EIBuild(P);
    const int lgm = T.size() - 1, m = 1 << lgm;
    P = T[lgm], P.erase(P.begin(), P.begin() + m - n), P = invLinear(_Eval(Deriv(P), n, T));
    P.resize(m * 2), Q.resize(m * 2);
    for (int i = m; i--;)
        P[i * 2] = P[i * 2 + 1] = (LL)Q[i] * P[i] % Mod;
    for (int d = 0, i = 2; d < lgm; ++d, i *= 2, swap(P, Q)) {
        poly A(i);
        for (int j = 0; j < m * 2; j += i * 2) {
            for (int k = 0; k < i; ++k)
                A[k] = ((LL)T[d][j + k] * P[j + i + k] + (LL)T[d][j + i + k] * P[j + k]) % Mod;
            if (d < lgm - 1)
                copy_n(A.begin(), i, Q.begin() + j), IDFT(A), rDFT(A), copy_n(A.begin(), i, Q.begin() + j + i);
            else
                IDFT(A), copy_n(A.begin(), i, Q.begin());
        }
    }
    return Slice(P, m - n, m);
}
poly ChirpZ(poly P, int X, int m) {
    if (!X) {
        poly Q(m, Eval(P, 0));
        return Q[0] = Eval(P, 1), Q;
    }
    const int n = P.size();
    const uLL x = trans(X), y = trans(qpow(X, Mod - 2));
    int pwX = 1, ipwX = 1;
    poly pwXX(n + m - 1, 1), ipwXX(n + m - 1, 1);
    for (int i = 0; i + 2 < n + m; ++i)
        pwXX[i + 1] = (LL)pwXX[i] * pwX % Mod, ipwXX[i + 1] = (LL)ipwXX[i] * ipwX % Mod, pwX = mul(pwX, x), ipwX = mul(ipwX, y);
    for (int i = 0; i < n; ++i)
        P[i] = (LL)P[i] * ipwXX[i] % Mod;
    P = MulT(pwXX, P);
    for (int i = 0; i < m; ++i)
        P[i] = (LL)P[i] * ipwXX[i] % Mod;
    return P;
}
poly ChirpZInter(poly P, int X) {
    const int n = P.size();
    if (n < 2) return P;
    const uLL x = trans(X), y = trans(qpow(X, Mod - 2));
    poly pwXX(n + n - 1, 1), ipwXX(n + n - 1, 1);
    int pwX = 1, ipwX = 1;
    for (int i = 0; i + 2 < n + n; ++i)
        pwXX[i + 1] = (LL)pwXX[i] * pwX % Mod, ipwXX[i + 1] = (LL)ipwXX[i] * ipwX % Mod, pwX = mul(pwX, x), ipwX = mul(ipwX, y);
    poly Q(n, 1);
    int t = X;
    for (int i = 1; i < n; ++i)
        Q[i] = (LL)(Mod + 1 - t) * Q[i - 1] % Mod, t = mul(t, x);
    t = (LL)(Mod + 1 - t) * Q[n - 1] % Mod;
    Q = invLinear(Q);
    for (int i = 0; i < n; ++i)
        P[i] = (LL)(i & 1 ? Mod - P[i] : P[i]) * pwXX[n - 1 - i] % Mod * ipwXX[n - 1] % Mod * Q[i] % Mod * Q[n - 1 - i] % Mod * ipwXX[i] % Mod;
    P = MulT(pwXX, P);
    for (int i = 0; i < n; ++i)
        P[i] = (LL)P[i] * ipwXX[i] % Mod;
    poly H(n, 1);
    for (int i = 1; i < n; ++i)
        H[i] = (LL)(i & 1 ? Mod - t : t) * pwXX[i] % Mod * Q[i] % Mod * Q[n - i] % Mod;
    P = Mul(P, H), P.resize(n), reverse(P.begin(), P.end());
    return P;
}
} // namespace Polynomial
namespace Polynomial {
poly Shift(poly P, int k) {
    if (!k) return P;
    const int n = P.size();
    Init(n);
    for (int i = 0; i < n; ++i)
        P[i] = (LL)frc[i] * P[i] % Mod;
    poly Q(n, 1);
    for (int i = 1; i < n; ++i)
        Q[i] = (LL)Q[i - 1] * k % Mod * inv[i] % Mod;
    P.resize(n + n - 1), Q = MulT(P, Q);
    for (int i = 0; i < n; ++i)
        Q[i] = (LL)Q[i] * ivf[i] % Mod;
    return Q;
}
poly Multi(poly P, int k) {
    const uLL v = trans(k);
    const int n = P.size();
    for (int i = 0, w = 1; i < n; ++i, w = mul(w, v))
        P[i] = (LL)P[i] * w % Mod;
    return P;
}
poly Comp_solve(poly& P, poly& Q, int d, int n, int v) {
    if (n == 1) {
        poly H(d + 1);
        for (int i = 0, w = 1; i <= d; ++i)
            H[i] = (LL)Binom(d + i - 1, d - 1) * w % Mod, w = (LL)w * v % Mod;
        H = MulT(P, H);
        return H;
    }
    poly F(d * n * 4);
    for (int i = 0; i < d; ++i)
        copy_n(Q.begin() + i * n, n, F.begin() + i * n * 2);
    F[d * n * 2] = 1, DFT(F);
    poly H(d * n * 2);
    for (int i = 0; i < d * n * 4; i += 2)
        H[i / 2] = (LL)F[i] * F[i + 1] % Mod;
    IDFT(H), --H[0];
    for (int i = 1; i < d * 2; ++i)
        copy_n(H.begin() + i * n, n / 2, H.begin() + i * (n / 2));
    H.resize(d * n);
    H = Comp_solve(P, H, d * 2, n / 2, v);
    poly nH(d * n * 2);
    for (int i = 0; i < d * 2; ++i)
        copy_n(H.begin() + i * (n / 2), n / 2, nH.begin() + i * n);
    DFT(nH);
    for (int i = 0; i < d * n * 4; i += 2)
        swap(F[i], F[i + 1]), F[i] = (LL)nH[i / 2] * F[i] % Mod, F[i + 1] = (LL)nH[i / 2] * F[i + 1] % Mod;
    IDFT(F);
    for (int i = 0; i < d; ++i)
        copy_n(F.begin() + (i + d) * n * 2, n, F.begin() + i * n);
    return F.resize(d * n), F;
}
poly Comp(poly P, poly Q) {
    if (P.empty() || Q.empty()) return poly();
    const int pn = P.size(), m = 2 << __lg(max(1, pn - 1)), v = Q[0];
    P.resize(m * 2), Q = Neg(Q), Q.resize(m);
    Q = Comp_solve(P, Q, 1, m, v);
    return Q.resize(pn), Q;
}
poly Compinv(poly P) {
    const int n = P.size(), t = qpow(P[1], Mod - 2);
    for (int i = 1, w = t; i < n; ++i)
        P[i] = (LL)P[i] * w % Mod, w = (LL)w * t % Mod;
    const int m = 2 << __lg(n - 1);
    poly Q(m);
    Q[0] = 1, P = Neg(P), P.resize(m);
    P.swap(Q);
    int d = 1;
    for (int k = n - 1; k; d *= 2, k /= 2) {
        const int L = 2 << __lg(d * (k + 1) * 4 - 1);
        poly nP(L), nQ(L);
        for (int i = 0; i < d; ++i)
            copy_n(P.begin() + i * (k + 1), k + 1, nP.begin() + i * (k + 1) * 2),
                copy_n(Q.begin() + i * (k + 1), k + 1, nQ.begin() + i * (k + 1) * 2);
        nQ[d * (k + 1) * 2] = 1, DFT(nP), DFT(nQ);
        P.resize(L / 2), Q.resize(L / 2);
        if (k & 1)
            for (int i = 0; i < L; i += 2)
                P[i / 2] = div2(mul(((LL)nP[i] * nQ[i + 1] + (LL)(Mod - nP[i + 1]) * nQ[i]) % Mod, iGrt[i / 2])),
                      Q[i / 2] = (LL)nQ[i] * nQ[i + 1] % Mod;
        else
            for (int i = 0; i < L; i += 2)
                P[i / 2] = div2(((LL)nP[i] * nQ[i + 1] + (LL)nP[i + 1] * nQ[i]) % Mod), Q[i / 2] = (LL)nQ[i] * nQ[i + 1] % Mod;
        IDFT(P), IDFT(Q);
        if (d * (k + 1) * 4 >= L) --Q[d * (k + 1) * 4 % L];
        for (int i = 1; i < d * 2; ++i)
            copy_n(P.begin() + i * (k + 1), k / 2 + 1, P.begin() + i * (k / 2 + 1)),
                copy_n(Q.begin() + i * (k + 1), k / 2 + 1, Q.begin() + i * (k / 2 + 1));
        P.resize(d * 2 * (k / 2 + 1)), Q.resize(d * 2 * (k / 2 + 1));
    }
    Init(n);
    Q = P, reverse(Q.begin(), Q.end()), Q.resize(n);
    for (int i = 1; i < n; ++i)
        Q[i] = Q[i] * (n - 1ll) % Mod * inv[i] % Mod;
    reverse(Q.begin(), Q.end()), Q = Mulx(Qpow(Q, Mod - inv[n - 1]), t), Q.insert(Q.begin(), 0);
    return Q.resize(n), Q;
}
} // namespace Polynomial
namespace Polynomial {
poly PointShift(poly P, int l, int r) {
    const int n = P.size();
    Init(n);
    if (l < n) {
        if (r < n)
            return Slice(P, l, r + 1);
        else {
            poly Q = PointShift(P, n, r);
            Q.insert(Q.begin(), P.begin() + l, P.end());
            return Q;
        }
    }
    if (r >= Mod) {
        r -= Mod;
        poly Q = PointShift(P, l, Mod - 1);
        if (r < n)
            Q.insert(Q.end(), P.begin(), P.begin() + r + 1);
        else
            Q.insert(Q.end(), P.begin(), P.end()), P = PointShift(P, n, r), Q.insert(Q.end(), P.begin(), P.end());
        return Q;
    }
    poly Q(r - l + n), H(n);
    for (int i = 0; i < n; ++i)
        H[i] = (LL)((n - 1 - i) & 1 ? Mod - P[i] : P[i]) * ivf[i] % Mod * ivf[n - 1 - i] % Mod;
    for (int i = 0; i < r - l + n; ++i)
        Q[i] = l - n + i + 1;
    Q = invLinear(Q), reverse(H.begin(), H.end()), Q = MulT(Q, H);
    poly fac(r - l + 1 + n);
    fac[0] = 1;
    for (int i = 0; i < r - l + n; ++i)
        fac[i + 1] = max(1ll, (LL)fac[i] * (l + i - n + 1) % Mod);
    poly ifac = invLinear(fac);
    for (int i = 0; i <= r - l; ++i) {
        const int p = (i + l) % Mod;
        if (p >= 0 && p < n)
            Q[i] = P[p];
        else
            Q[i] = (LL)Q[i] * ifac[i] % Mod * fac[i + n] % Mod;
    }
    return Q;
}
} // namespace Polynomial
namespace Polynomial {
pair<Mat2, Mat2> Hgcdi(poly P, poly Q, int d, int L) {
    const int n = P.size() - 1, m = Q.size() - 1;
    const auto calc = [&](Mat2 A) {
        Mat2 B = A;
        Reduce(get<0>(B), L), DFT(get<0>(B));
        Reduce(get<1>(B), L), DFT(get<1>(B));
        Reduce(get<2>(B), L), DFT(get<2>(B));
        Reduce(get<3>(B), L), DFT(get<3>(B));
        return make_pair(A, B);
    };
    if (m < n - d) return calc(Mat2(poly({1}), poly(), poly(), poly({1})));
    if (d == 1) return calc(Mat2(poly(), poly({1}), poly({1}), Neg(Div(poly(P.begin() + n - 2, P.end()), poly(Q.begin() + n - 2, Q.end())))));
    const int h = L / 2;
    if (d <= h) {
        auto [A, B] = Hgcdi(P, Q, d, h);
        poly C;
        C = get<0>(A);
        for (int i = h; i < C.size(); ++i)
            C[i & (h - 1)] = sub(C[i & (h - 1)], C[i]);
        C.resize(h), rDFT(C), get<0>(B).insert(get<0>(B).end(), C.begin(), C.end());
        C = get<1>(A);
        for (int i = h; i < C.size(); ++i)
            C[i & (h - 1)] = sub(C[i & (h - 1)], C[i]);
        C.resize(h), rDFT(C), get<1>(B).insert(get<1>(B).end(), C.begin(), C.end());
        C = get<2>(A);
        for (int i = h; i < C.size(); ++i)
            C[i & (h - 1)] = sub(C[i & (h - 1)], C[i]);
        C.resize(h), rDFT(C), get<2>(B).insert(get<2>(B).end(), C.begin(), C.end());
        C = get<3>(A);
        for (int i = h; i < C.size(); ++i)
            C[i & (h - 1)] = sub(C[i & (h - 1)], C[i]);
        C.resize(h), rDFT(C), get<3>(B).insert(get<3>(B).end(), C.begin(), C.end());
        return make_pair(A, B);
    }
    const int sx = max(0, n - h - h);
    auto [A, B] = Hgcdi(poly(P.begin() + sx, P.end()), poly(Q.begin() + sx, Q.end()), h, L);
    const int t = h - get<3>(A).size() + 1;
    poly Px = Slice(P, n - h + t - L, n - h + t), Py = Slice(P, n - h - h - L, n - h - h);
    poly Qx = Slice(Q, n - h + t - L, n - h + t), Qy = Slice(Q, n - h - h - L, n - h - h);
    DFT(Px), DFT(Qx), DFT(Py), DFT(Qy);
    for (int i = 0, x, y; i < L; ++i) {
        x = ((LL)Px[i] * get<0>(B)[i] + (LL)Qx[i] * get<1>(B)[i]) % Mod;
        y = ((LL)Px[i] * get<2>(B)[i] + (LL)Qx[i] * get<3>(B)[i]) % Mod;
        Px[i] = x, Qx[i] = y;
        x = ((LL)Py[i] * get<0>(B)[i] + (LL)Qy[i] * get<1>(B)[i]) % Mod;
        y = ((LL)Py[i] * get<2>(B)[i] + (LL)Qy[i] * get<3>(B)[i]) % Mod;
        Py[i] = x, Qy[i] = y;
    }
    IDFT(Px), IDFT(Qx), IDFT(Py), IDFT(Qy);
    poly Pz(Py.end() - h - t, Py.end()), Qz(Qy.end() - h - t, Qy.end());
    Pz.insert(Pz.end(), Px.end() - h - t, Px.end());
    Qz.insert(Qz.end(), Qx.end() - h - t, Qx.end()), Empty(Qz);
    int v = 0;
    for (int i = 0, j = n - h + t; i <= j; ++i)
        v = (v + (LL)(i < P.size() ? P[i] : 0) * (j - i < get<0>(A).size() ? get<0>(A)[j - i] : 0) +
             (LL)(i < Q.size() ? Q[i] : 0) * (j - i < get<1>(A).size() ? get<1>(A)[j - i] : 0)) %
            Mod;
    Pz.emplace_back(v);
    if (Qz.size() <= h * 3 + t - d) return make_pair(A, B);
    int hh = d - get<3>(A).size() + 1, w = get<3>(A).back(), k = get<3>(A).size() - 1;
    if (t > 0) {
        const int r = max(0, h * 3 + t - n);
        auto [C, D] = DivMod(poly(Pz.begin() + r, Pz.end()), poly(Qz.begin() + r, Qz.end()));
        w = (LL)w * (Mod - C.back()) % Mod, k += C.size() - 1, hh -= C.size() - 1;
        Reduce(C, L), DFT(C);
        for (int i = 0; i < L; ++i) {
            get<0>(B)[i] = (get<0>(B)[i] + (LL)(Mod - C[i]) * get<2>(B)[i]) % Mod;
            get<1>(B)[i] = (get<1>(B)[i] + (LL)(Mod - C[i]) * get<3>(B)[i]) % Mod;
        }
        swap(get<0>(B), get<2>(B)), swap(get<1>(B), get<3>(B));
        Pz.swap(Qz), Qz.swap(D), Qz.insert(Qz.begin(), r, 0);
    }
    const int sy = max(0, h * 3 + t - d - hh);
    auto [C, D] = Hgcdi(poly(Pz.begin() + sy, Pz.end()), (sy <= Qz.size() ? poly(Qz.begin() + sy, Qz.end()) : poly()), hh, L);
    for (int i = 0; i < L; ++i) {
        int a0 = ((LL)get<0>(D)[i] * get<0>(B)[i] + (LL)get<1>(D)[i] * get<2>(B)[i]) % Mod;
        int a1 = ((LL)get<0>(D)[i] * get<1>(B)[i] + (LL)get<1>(D)[i] * get<3>(B)[i]) % Mod;
        int a2 = ((LL)get<2>(D)[i] * get<0>(B)[i] + (LL)get<3>(D)[i] * get<2>(B)[i]) % Mod;
        int a3 = ((LL)get<2>(D)[i] * get<1>(B)[i] + (LL)get<3>(D)[i] * get<3>(B)[i]) % Mod;
        get<0>(B)[i] = a0, get<1>(B)[i] = a1, get<2>(B)[i] = a2, get<3>(B)[i] = a3;
    }
    D = B;
    IDFT(get<0>(D)), get<0>(D).resize(d), Empty(get<0>(D));
    IDFT(get<1>(D)), get<1>(D).resize(d), Empty(get<1>(D));
    IDFT(get<2>(D)), get<2>(D).resize(d), Empty(get<2>(D));
    IDFT(get<3>(D));
    k += get<3>(C).size() - 1;
    if (k == L) {
        get<3>(D).resize(d + 1);
        const int x = (LL)w * get<3>(C).back() % Mod;
        get<3>(D)[d] = x, get<3>(D)[0] = sub(get<3>(D)[0], x);
    }
    Empty(get<3>(D));
    return make_pair(D, B);
}
Mat2 Hgcd(poly P, poly Q, int d) {
    if (P.size() == Q.size()) {
        const auto [X, Y] = DivMod(P, Q);
        const Mat2 A = Hgcdi(Q, Y, d, 2 << __lg(max(1, d - 1))).first;
        return make_tuple(get<1>(A), Sub_Empty(get<0>(A), Mul(get<1>(A), X)), get<3>(A), Sub_Empty(get<2>(A), Mul(X, get<3>(A))));
    } else if (P.size() < Q.size()) {
        const Mat2 A = Hgcdi(Q, P, d, 2 << __lg(max(1, d - 1))).first;
        return make_tuple(get<1>(A), get<0>(A), get<3>(A), get<2>(A));
    } else
        return Hgcdi(P, Q, d, 2 << __lg(max(1, d - 1))).first;
}
tuple<poly, poly, poly> Exgcd(poly P, poly Q) {
    Empty(P), Empty(Q);
    const Mat2 M = Hgcd(P, Q, max(P.size(), Q.size()) - 1);
    const poly H = Add_Empty(Mul(get<0>(M), P), Mul(get<1>(M), Q));
    const int v = qpow(H.back(), Mod - 2);
    return make_tuple(Mulx(get<0>(M), v), Mulx(get<1>(M), v), Mulx(H, v));
}
poly Gcd(poly P, poly Q) {
    Empty(P), Empty(Q);
    const Mat2 M = Hgcd(P, Q, max(P.size(), Q.size()) - 1);
    const poly H = Add_Empty(Mul(get<0>(M), P), Mul(get<1>(M), Q));
    return Mulx(H, qpow(H.back(), Mod - 2));
}
poly Modinv(poly P, poly Q) {
    Empty(P), Empty(Q);
    if (P.size() >= Q.size()) P = DivMod(P, Q).second;
    const Mat2 M = Hgcd(Q, P, Q.size() - 1);
    const poly H = Add_Empty(Mul(get<0>(M), Q), Mul(get<1>(M), P));
    if (H.size() > 1) return poly({-1});
    return Mulx(get<1>(M), qpow(H.back(), Mod - 2));
}
} // namespace Polynomial
namespace Polynomial {
poly BerlekampMassey(poly P) {
    const int n = P.size();
    reverse(P.begin(), P.end()), Empty(P);
    poly Q(n + 1);
    Q[n] = 1;
    Mat2 M = Hgcd(Q, P, n / 2);
    const poly C = Add_Empty(Mul(get<0>(M), Q), Mul(get<1>(M), P));
    const poly D = Add_Empty(Mul(get<2>(M), Q), Mul(get<3>(M), P));
    if ((int)(D.size() + C.size()) > n + 1) get<3>(M) = Sub_Empty(get<1>(M), Mul(Div(C, D), get<3>(M)));
    Q = get<3>(M), Q = Mulx(Q, qpow(Mod - Q.back(), Mod - 2));
    Q.pop_back(), reverse(Q.begin(), Q.end());
    return Q;
}
void FindRoot_solve(poly P, poly& W) {
    if (P.size() == 1) return;
    if (P.size() == 2) {
        W.emplace_back(qpow(P[1], Mod - 2, Mod - P[0]));
        return;
    }
    poly Q({uniform_int_distribution<int>(0, Mod - 1)(rng), 1});
    Q = ModPow(Q, Mod >> 1, P);
    if (Empty(Q)) return FindRoot_solve(P, W);
    Q[0] = sub(Q[0], 1);
    Q = Gcd(P, Q), P = Div(P, Q);
    FindRoot_solve(P, W), FindRoot_solve(Q, W);
}
poly FindRoot(poly P) {
    Empty(P);
    if (P.size() < 2) return poly();
    poly Q = ModPow(poly({0, 1}), Mod, P);
    if (Q.size() < 2) Q.resize(2);
    Q[1] = sub(Q[1], 1);
    P = Gcd(P, Q);
    poly W;
    FindRoot_solve(P, W);
    sort(W.begin(), W.end());
    return W;
}
} // namespace Polynomial
using namespace BasicMath;
using namespace Polynomial;
signed main() {
    cin.tie(0)->sync_with_stdio(0);
    int n;
    cin >> n;
    poly P(n);
    for (int& i : P)
        cin >> i;
    P = BerlekampMassey(P);
    cout << (P.size()) << '\n';
    for (int i : P)
        cout << i << ' ';
    cout << '\n';
    return 0;
}
