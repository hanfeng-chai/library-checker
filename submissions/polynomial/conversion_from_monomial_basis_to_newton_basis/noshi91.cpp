#include <algorithm>
#include <cassert>
#include <vector>

#ifndef NOSHI91_POLYNOMIAL_INTERNAL
#define NOSHI91_POLYNOMIAL_INTERNAL

#include <vector>

#include <atcoder/convolution>

namespace noshi91 {

namespace polynomial {

namespace internal {

template <class T> const atcoder::internal::fft_info<T> info;

template <class T> void dft(std::vector<T> &a) {
  atcoder::internal::butterfly(a);
}

template <class T> void inv_dft(std::vector<T> &a) {
  atcoder::internal::butterfly_inv(a);
  T c = (1 - T::mod()) / int(a.size());
  for (auto &e : a)
    e *= c;
}

} // namespace internal

} // namespace polynomial

} // namespace noshi91

#endif

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>

namespace noshi91 {

namespace polynomial {

template <class T> std::vector<T> inverse(const std::vector<T> &f) {
  int n = f.size();
  if (n == 0)
    return {};
  assert(f[0] != T(0));
  std::vector<T> g(n);
  g[0] = T(1) / f[0];
  int p = 1;
  while (p < n) {
    std::vector<T> f_(2 * p, T(0)), g_(2 * p, T(0));
    std::copy(f.begin(), f.begin() + std::min(2 * p, n), f_.begin());
    internal::dft(f_);
    std::copy(g.begin(), g.begin() + p, g_.begin());
    internal::dft(g_);
    for (int i = 0; i < 2 * p; i++)
      f_[i] *= g_[i];
    internal::inv_dft(f_);
    std::fill(f_.begin(), f_.begin() + p, T(0));
    internal::dft(f_);
    for (int i = 0; i < 2 * p; i++)
      f_[i] *= g_[i];
    internal::inv_dft(f_);
    std::transform(f_.begin() + p, f_.begin() + std::min(2 * p, n),
                   g.begin() + p, std::negate<>());
    p *= 2;
  }
  return g;
}

} // namespace polynomial

} // namespace noshi91

#include <algorithm>
#include <vector>

namespace noshi91 {

namespace polynomial {

template <class T> class subproduct_tree {
public:
  std::vector<std::vector<T>> d;
  subproduct_tree(const std::vector<T> &p) : d() {
    int n = p.size();
    if (n == 0) {
      d.assign(1, {});
      return;
    }
    int h = 31 - __builtin_clz(n * 2 - 1);
    d.assign(h + 1, {});
    d[0].assign(n * 2, {});
    std::vector<T> buf;
    for (int i = 0; i < n; i++) {
      buf = {-p[i], 1};
      internal::dft(buf);
      d[0][i * 2] = buf[0], d[0][i * 2 + 1] = buf[1];
    }
    for (int i = 1; i <= h; i++) {
      int w = 1 << i;
      int m = (n + w - 1) / w * w * 2;
      d[i].resize(m);
      auto st = d[i - 1].begin();
      for (int l = 0; l < m; l += w * 2) {
        buf.assign(st + l, st + l + w);
        if (l + w < 2 * n) {
          for (int j = 0; j < w; j++)
            buf[j] *= st[l + w + j];
        }
        std::copy(buf.begin(), buf.end(), d[i].begin() + l);
        internal::inv_dft(buf);
        if (l + w * 2 <= n * 2)
          buf[0] -= 2;
        T r = 1, q = internal::info<T>.root[i + 1];
        for (int j = 0; j < w; j++)
          buf[j] *= r, r *= q;
        internal::dft(buf);
        std::copy(buf.begin(), buf.end(), d[i].begin() + l + w);
      }
    }
  }
  int size() const { return d[0].size() / 2; }
};

} // namespace polynomial

} // namespace noshi91

namespace noshi91 {

namespace polynomial {

template <class T>
std::vector<T> monomial_to_newton(const subproduct_tree<T> &t,
                                  std::vector<T> f) {
  assert(int(f.size()) == t.size());
  int n = t.size();
  if (n == 0)
    return {};
  int h = t.d.size() - 1;
  std::vector<T> buf(t.d[h].begin(), t.d[h].begin() + (1 << h));
  internal::inv_dft(buf);
  buf.erase(buf.begin()), buf.push_back(1);
  std::reverse(buf.begin(), buf.begin() + n);
  buf = inverse(buf);
  std::reverse(f.begin(), f.end());
  buf.resize(2 << h), internal::dft(buf);
  f.resize(2 << h), internal::dft(f);
  for (int i = 0; i < 2 << h; i++)
    f[i] *= buf[i];
  internal::inv_dft(f);
  f.resize(1 << h);
  std::reverse(f.begin(), f.end());
  for (int i = h; i > 0; i--) {
    int w = 1 << i, m = (n + w - 1) / w * w;
    auto st = t.d[i - 1].begin();
    for (int l = 0; l < m; l += w) {
      if (l + w / 2 < n) {
        buf.assign(f.begin() + l, f.begin() + l + w);
        internal::dft(buf);
        for (int j = 0; j < w; j++)
          buf[j] *= st[l * 2 + w + j];
        internal::inv_dft(buf);
        std::copy(buf.begin() + w / 2, buf.end(), f.begin() + l);
      } else {
        std::copy(f.begin() + l + w / 2, f.begin() + l + w, f.begin() + l);
      }
    }
  }
  f.resize(n);
  return f;
}

} // namespace polynomial

} // namespace noshi91

#include <iostream>

#include <atcoder/modint>

int main() {
  using mint = atcoder::modint998244353;
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  std::vector<mint> a(N), p(N);
  for (auto &e : a) {
    int t;
    std::cin >> t;
    e = t;
  }
  for (auto &e : p) {
    int t;
    std::cin >> t;
    e = t;
  }
  const auto t = noshi91::polynomial::subproduct_tree(p);
  const auto ans = noshi91::polynomial::monomial_to_newton(t, a);
  for (const auto &e : ans) {
    std::cout << e.val() << " ";
  }
  return 0;
}
