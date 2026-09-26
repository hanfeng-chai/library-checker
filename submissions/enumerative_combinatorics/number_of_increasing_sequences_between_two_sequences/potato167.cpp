#line 2 "fps/count_increasing_sequences.hpp"
#include <atcoder/convolution>
#line 2 "math/Binomial.hpp"

#include<vector>
#include<assert.h>

namespace po167{
template<class T>
struct Binomial{
    std::vector<T> fact_vec, fact_inv_vec;
    void extend(int m = -1){
        int n = fact_vec.size();
        if (m == -1) m = n * 2;
        if (n >= m) return;
        fact_vec.resize(m);
        fact_inv_vec.resize(m);
        for (int i = n; i < m; i++){
            fact_vec[i] = fact_vec[i - 1] * T(i);
        }
        fact_inv_vec[m - 1] = T(1) / fact_vec[m - 1];
        for (int i = m - 1; i > n; i--){
            fact_inv_vec[i - 1] = fact_inv_vec[i] * T(i);
        }
    }
    Binomial(int MAX = 0){
        fact_vec.resize(1, T(1));
        fact_inv_vec.resize(1, T(1));
        extend(MAX + 1);
    }

    T fact(int i){
        if (i < 0) return 0;
        while (int(fact_vec.size()) <= i) extend();
        return fact_vec[i];
    }
    T invfact(int i){
        if (i < 0) return 0;
        while (int(fact_inv_vec.size()) <= i) extend();
        return fact_inv_vec[i];
    }
    T C(int a, int b){
        if (a < b || b < 0) return 0;
        return fact(a) * invfact(b) * invfact(a - b);
    }
    T invC(int a, int b){
        if (a < b || b < 0) return 0;
        return fact(b) * fact(a - b) *invfact(a);
    }
    T P(int a, int b){
        if (a < b || b < 0) return 0;
        return fact(a) * invfact(a - b);
    }
    T inv(int a){
        if (a < 0) return inv(-a) * T(-1);
        if (a == 0) return 1;
        return fact(a - 1) * invfact(a);
    }
    T Catalan(int n){
        if (n < 0) return 0;
        return fact(2 * n) * invfact(n + 1) * invfact(n);
    }
    T narayana(int n, int k){
        if (n <= 0 || n < k || k < 1) return 0;
        return C(n, k) *  C(n, k - 1) * inv(n);
    }
    T Catalan_pow(int n,int d){
        if (n < 0 || d < 0) return 0;
        if (d == 0){
            if (n == 0) return 1;
            return 0;
        }
        return T(d) * inv(d + n) * C(2 * n + d - 1, n);
    }
    // retrun [x^a] 1/(1-x)^b
    T ruiseki(int a,int b){
        if (a < 0 || b < 0) return 0;
        if (a == 0){
            return 1;
        }
        return C(a + b - 1, b - 1);
    }
    // (a, b) -> (c, d)
    // always x + e >= y
    T mirror(int a, int b, int c, int d, int e = 0){
        if (a + e < b || c + e < d) return 0;
        if (a > c || b > d) return 0;
        a += e;
        c += e;
        return C(c + d - a - b, c - a) - C(c + d - a - b, c - b + 1); 
    }
    // return sum_{i = 0, ... , a} sum_{j = 0, ... , b} C(i + j, i)
    // return C(a + b + 2, a + 1) - 1;
    T gird_sum(int a, int b){
        if (a < 0 || b < 0) return 0;
        return C(a + b + 2, a + 1) - 1;
    }
    // return sum_{i = a, ..., b - 1} sum_{j = c, ... , d - 1} C(i + j, i)
    // AGC 018 E
    T gird_sum_2(int a, int b, int c, int d){
        if (a >= b || c >= d) return 0;
        a--, b--, c--, d--;
        return gird_sum(a, c) - gird_sum(a, d) - gird_sum(b, c) + gird_sum(b, d);
    }

    // the number of diagonal dissections of a convex n-gon into k+1 regions.
    // OEIS A033282
    // AGC065D
    T diagonal(int n, int k){
        if (n <= 2 || n - 3 < k || k < 0) return 0;
        return C(n - 3, k) * C(n + k - 1, k) * inv(k + 1);
    }
};
}
#line 4 "fps/FPS_cyclic_convolution.hpp"

namespace po167{
// |f| = |g| = 2 ^ n
template<class T>
std::vector<T> FPS_cyclic_convolution(std::vector<T> f, std::vector<T> g){
    atcoder::internal::butterfly(f);
    atcoder::internal::butterfly(g);
    for (int i = 0; i < (int)f.size(); i++) f[i] *= g[i];
    atcoder::internal::butterfly_inv(f);
    T iz = (T)(1) / (T)(f.size());
    for (int i = 0; i < (int)f.size(); i++) f[i] *= iz;
    return f;
}
}
#line 5 "fps/count_increasing_sequences.hpp"
namespace po167{
template<class T>
std::pair<std::vector<T>, std::vector<T>> count_square(std::vector<T> L, std::vector<T> D){
    assert(!L.empty() && !D.empty());
    int N = L.size();
    int M = D.size();
    if (std::min(N, M) <= 200){
        int sw = 0;
        if (N > M) std::swap(N, M), std::swap(L, D), sw = 1;
        std::vector<T> R(N);
        for (int i = 0; i < N; i++){
            D[0] += L[i];
            for (int j = 1; j < M; j++) D[j] += D[j - 1];
            R[i] = D.back();
        }
        if (sw) std::swap(R, D);
        return {R, D};
    }
    po167::Binomial<T> table(N + M);
    std::vector<T> R(N), U(M);
    int z = 0;
    while ((1 << z) < (N + M - 1)) z++;
    // 左から右
    {
        std::vector<T> tmp(N);
        for (int i = 0; i < N; i++) tmp[i] = table.C(M - 1 + i, i);
        tmp = atcoder::convolution(tmp, L);
        for (int i = 0; i < N; i++) R[i] += tmp[i];
    }
    // 左から上
    {
        std::vector<T> tmp(1 << z);
        for (int i = 0; i < N; i++) L[i] *= table.invfact(N - 1 - i);
        for (int i = 0; i < N + M - 1; i++) tmp[i] = table.fact(i);
        L.resize(1 << z, 0);
        tmp = po167::FPS_cyclic_convolution(tmp, L);
        for (int i = 0; i < M; i++) U[i] += tmp[N - 1 + i] * table.invfact(i);
    }
    // 下から上
    {
        std::vector<T> tmp(M);
        for (int i = 0; i < M; i++) tmp[i] = table.C(N - 1 + i, i);
        tmp = atcoder::convolution(tmp, D);
        for (int i = 0; i < M; i++) U[i] += tmp[i];
    }
    // 下から右
    {
        std::vector<T> tmp(1 << z);
        for (int i = 0; i < M; i++) D[i] *= table.invfact(M - i - 1);
        for (int i = 0; i < N + M - 1; i++) tmp[i] = table.fact(i);
        D.resize(1 << z, 0);
        tmp = po167::FPS_cyclic_convolution(tmp, D);
        for (int i = 0; i < N; i++) R[i] += tmp[M - 1 + i] * table.invfact(i);
    }
    return {R, U};
}
template<class T>
/*
 * g(A, x) を
 * 0 <= B[i] < A[i] かつ B[i] = x を満たす
 * 広義単調増加列 B の数とする
 * res[x] = sum C[i] * g(A[i:N], x)
 * を返す
 */
std::vector<T> count_increase_sequences_with_upper_bounds(std::vector<int> A, std::vector<T> C){
    int N = A.size();
    assert((int)C.size() == N);
    assert(N);
    for (int i = (int)(A.size()) - 1; i > 0; i--) A[i - 1] = std::min(A[i - 1], A[i]);
    if (A.back() == 0) return {};
    if (std::min(A.back(), N) <= 200){
        std::vector<T> dp(0);
        dp.reserve(A.back());
        for (int i = 0; i < N; i++){
            dp.resize(A[i], 0);
            if (A[i]) dp[0] += C[i];
            for (int j = 1; j < (int)dp.size(); j++){
                dp[j] += dp[j - 1];
            }
        }
        return dp;
    }
    if (N == 1){
        std::vector<T> res(A[0]);
        for (int i = 0; i < A[0]; i++) res[i] = C[0];
        return res;
    }
    int m = N / 2;
    std::vector<int> LA(m), RA(N - m);
    std::vector<T> LC(m), RC(N - m);
    for (int i = 0; i < m; i++){
        LA[i] = A[i];
        LC[i] = C[i];
    }
    for (int i = 0; i < N - m; i++){
        RA[i] = A[i + m] - A[m - 1];
        RC[i] = C[i + m];
    }
    std::vector<T> res;
    res.reserve(A.back());
    auto L = count_increase_sequences_with_upper_bounds(LA, LC);
    if (!L.empty()){
        auto [R, U] = count_square(L, RC);
        for (int i = 0; i < (int)R.size(); i++) res.push_back(R[i]);
        std::swap(U, RC);
    }
    auto R = count_increase_sequences_with_upper_bounds(RA, RC);
    for (auto x : R) res.push_back(x);
    return res;
}
template<class T>
std::vector<T> NAIVE_count_increase_sequences_with_upper_lower_bounds(std::vector<int> A, std::vector<int> B, std::vector<T> C = {}){
    std::vector<T> tmp(B.back() - A[0]);
    if (C.empty()){
        int b = B[0];
        for (int i = 1; i < (int)B.size(); i++) b = std::min(b, B[i]);
        for (int i = 0; i < b - A[0]; i++) tmp[i] = 1;
    }
    else for (int i = 0; i < (int)std::min(tmp.size(), C.size()); i++) tmp[i] = C[i];
    int N = A.size();
    for (int i = 1; i < N; i++){
        for (int j = 1; j < (int)tmp.size(); j++){
            tmp[j] += tmp[j - 1];
        }
        for (int j = 0; j < (int)tmp.size(); j++){
            if (j < A[i] - A[0] || B[i] - A[0] <= j) tmp[j] = 0;
        }
    }
    std::vector<T> res(B.back() - A.back());
    for (int i = 0; i < B.back() - A.back(); i++){
        res[i] = tmp[A.back() - A[0] + i];
    }
    return res;
}

template<class T>
/*
 * f(a, b) を X[0] = a, X[N - 1] = b であるような、A, B に挟まれたものとする
 * 長さ B[N - 1] - A[N - 1] を返す
 * res[b - A.back()] = sum C[a - A[0]] * f(a, b)
 * A, B は広義単調増加が嬉しい
 */
std::vector<T> count_increase_sequences_with_upper_lower_bounds(std::vector<int> A, std::vector<int> B, std::vector<T> C = {}){
    int N = A.size();
    assert(A.size() == B.size());
    for (int i = 0; i < N - 1; i++){
        A[i + 1] = std::max(A[i], A[i + 1]);
    }
    for (int i = N - 1; i > 0; i--){
        B[i - 1] = std::min(B[i], B[i - 1]);
    }
    if (A.back() >= B.back()) return {};
    // A[0] == 0 にする
    std::vector<T> res(B.back() - A.back(), 0);
    {
        int tmp = A[0];
        for (int i = 0; i < N; i++){
            A[i] -= tmp;
            B[i] -= tmp;
            if (A[i] >= B[i]) return res;
        }
    }
    if (C.empty()){
        C.resize(B[0] - A[0], 1);
    }
    else assert((int)(C.size()) == B[0] - A[0]);
    int l = 0;
    while (B[l] <= A.back()){
        for (int i = (int)(C.size()) - 1; i > 0; i--) C[i] -= C[i - 1];
        int nl = l;
        while (A[nl] < B[l]) nl++;
        std::vector<int> tmp(B[l] - A[l]);
        tmp[0] = 1;
        for (int i = l; i < nl; i++){
            tmp[A[i] - A[l]]++;
        }
        for (int i = 1; i < B[l] - A[l]; i++) tmp[i] += tmp[i - 1];
        auto X = count_increase_sequences_with_upper_bounds(tmp, C);
        std::vector<int> nB(nl - l + 1);
        for (int i = l; i <= nl; i++){
            nB[i - l] = B[i] - B[l];
        }
        auto Y = count_increase_sequences_with_upper_bounds(nB, X);
        C.resize(B[nl] - A[nl]);
        for (int i = 0; i < B[nl] - A[nl]; i++){
            C[i] = Y[i + A[nl] - B[l]];
        }
        l = nl;
    }
    // A を揃えてしまえ
    {
        int a = A[l];
        for (int i = l; i < N; i++){
            A[i] -= a;
            B[i] -= a;
        }
    }
    for (int i = (int)(C.size()) - 1; i > 0; i--) C[i] -= C[i - 1];
    std::vector<T> D(N - l, 0);
    if (A.back() != 0){
        std::vector<T> L(A.back());
        for (int i = 0; i < (int)L.size(); i++) L[i] = C[i];
        std::vector<int> tmp(L.size());
        tmp[0] = 1;
        for (int i = l; i < N; i++){
            if (A[i] < (int)tmp.size()) tmp[A[i]]++;
        }
        for (int i = 1; i < (int)tmp.size(); i++){
            tmp[i] += tmp[i - 1];
        }
        auto nD = count_increase_sequences_with_upper_bounds(tmp, L);
        for (int i = 0; i < (int)nD.size(); i++) D[i] = nD[i];
    }
    for (int i = A.back(); i < B[l]; i++) C[i - A.back()] = C[i];
    C.resize(B[l] - A.back());
    auto [R, U] = count_square(C, D);
    res = R;
    std::vector<int> nB(N - l);
    for (int i = 0; i < N - l; i++) nB[i] = B[i + l] - B[l];
    R = count_increase_sequences_with_upper_bounds(nB, U);
    for (auto x : R) res.push_back(x);
    return res;
}
}
#line 2 "a.cpp"
#include <iostream>
int main() {
    int N, M;
    std::cin >> N >> M;
    std::vector<int> A(N), B(N);
    for (int i = 0; i < N; i++) std::cin >> A[i];
    for (int i = 0; i < N; i++) std::cin >> B[i];
    using mint = atcoder::modint998244353;
    auto tmp = po167::count_increase_sequences_with_upper_lower_bounds<mint>(A, B);
    mint ans = 0;
    for (auto x : tmp) ans += x;
    std::cout << ans.val() << "\n";
}
