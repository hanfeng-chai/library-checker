#line 1 "main.cpp"
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

// --- Struct Modint ---
using u32 = uint32_t;
using u64 = uint64_t;
using i64 = int64_t;

template <int mod>
struct modint {
  using M = modint;
  static constexpr u32 r1 = []() {
    u32 r1 = mod;
    for (int i = 0; i < 5; ++i) r1 *= 2 - mod * r1;
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

// --- Helper Functions ---
const int LIM = 10000005;
Mint fac[LIM], invFac[LIM];

void precomputeFactorials() {
  fac[0] = 1;
  for (int i = 1; i < LIM; ++i) fac[i] = fac[i - 1] * Mint(i);
  invFac[LIM - 1] = fac[LIM - 1].inv();
  for (int i = LIM - 2; i >= 0; --i) invFac[i] = invFac[i + 1] * Mint(i + 1);
}

Mint nCk(int n, int k) {
  if (k < 0 || k > n) return 0;
  return fac[n] * invFac[k] * invFac[n - k];
}

// Tính i^d bằng Sàng tuyến tính
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

// Hàm tính tổng chính xác: Sum(r^i * i^d)
Mint sumPowerPolyLimit(Mint r, int d, const vector<Mint>& fs) {
  if (r.x == 0) return fs[0];

  Mint inv_1_r = (Mint(1) - r).inv();
  Mint C = -r * inv_1_r;      // C = -r / (1-r)
  Mint neg_inv_r = -r.inv();  // -1 / r
  Mint C_pow_d = C.pow(d);    // C^d (Hằng số trong vòng lặp)

  Mint ans = 0;
  Mint V = C_pow_d;  // Giá trị ban đầu V_d = C^d

  // Duyệt ngược từ d về 0
  for (int j = d; j >= 0; --j) {
    // Cộng term vào kết quả: j^d * (-1)^j * V_j
    Mint term = fs[j] * V;
    if (j & 1)
      ans -= term;
    else
      ans += term;

    // Tính V_{j-1} từ V_j để dùng cho vòng sau
    if (j > 0) {
      // Công thức truy hồi: V_{j-1} = (-1/r) * V_j + C(d+1, j) * C^d
      V = V * neg_inv_r + nCk(d + 1, j) * C_pow_d;
    }
  }

  return ans * inv_1_r;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  precomputeFactorials();

  int r_in, d;
  if (cin >> r_in >> d) {
    Mint r(r_in);
    // getMonomials cần kích thước d + 1 để có fs[d]
    vector<Mint> pws = getMonomials(d, d + 1);
    cout << sumPowerPolyLimit(r, d, pws) << "\n";
  }
  return 0;
}
