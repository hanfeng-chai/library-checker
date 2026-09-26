#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

#define F first
#define S second
#define ep emplace
#define eb emplace_back
#define endl '\n'
#define ALL(x) x.begin(), x.end()

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3f;

void debug() { cout << endl; }
template<class T, class ...U> void debug(T a, U...b) {
  cout << a << ' '; debug(b...);
}
/*--------------------------------------------------------------*/

const ll MOD = 1e9 + 7;

// tetration_mod — compute A↑↑B mod M (handles composite M via prime-power + CRT)
// Complexity: O(#prime factors * log M) per prime power + factorization.
// Requires from your codebook: factorize_u64(u64 n, vector<pair<u64,int>>& pf)
// Uses local: pow_mod_ll (128-bit safe), simple CRT merge for coprime prime powers.

using ll = long long;
using u64 = unsigned long long;
using i128 = __int128_t;

// ---------- forward decls ----------
static inline ll tetration_mod(ll A, ll B, ll M);            // used inside prime-power case


// Pollard–Rho (64-bit) — finds a nontrivial factor of composite n
// ---------------------------------------------------------------
// Requires: miller_rabin_64(n), mul_mod_u64(a,b,mod)
// API: pollard_rho_64(n) -> factor in (1,n), may retry internally until found.
// Notes:
//   - Handles even n quickly.
//   - Uses Brent's cycle detection + batch GCD (faster in practice).
//   - For prime n, this routine may loop; typical usage first checks MR.
// Complexity (heuristic): ~ Õ(n^{1/4}) modular multiplications on random composites.

using u128 = __uint128_t;

static inline u64 splitmix64(u64& s){
  u64 z = (s += 0x9e3779b97f4a7c15ull);
  z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
  z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
  return z ^ (z >> 31);
}

static inline u64 f_rho(u64 x, u64 c, u64 n){
  return (u64)(( (u128)x * x + c ) % n);
}

static inline u64 pollard_rho_64(u64 n){
  if ((n & 1ull) == 0) return 2;
  // It’s fine if caller first ensures n is composite via Miller–Rabin.
  static u64 seed = 0x123456789abcdef0ull;
  for(;;){
    u64 c = (splitmix64(seed) % (n - 1)) + 1;       // in [1, n-1]
    u64 y = (splitmix64(seed) % (n - 2)) + 2;       // in [2, n-1]
    u64 x = y, d = 1, q = 1;
    const u64 m = 128;                               // batch size for gcd
    // Brent's algorithm
    for(u64 r = 1; d == 1; r <<= 1){
      x = y;
      for(u64 i = 0; i < r; ++i) y = f_rho(y, c, n);
      for(u64 k = 0; k < r && d == 1; k += m){
        u64 lim = (r - k < m) ? (r - k) : m;
        for(u64 i = 0; i < lim; ++i){
          y = f_rho(y, c, n);
          u64 diff = x > y ? x - y : y - x;
          q = (u64)((u128)q * diff % n);
        }
        d = std::gcd(q, n);
      }
    }
    if (d == 1){
      // rare: try another (c, y)
      continue;
    } else if (d == n){
      // fallback: walk one-by-one to extract a factor
      do {
        y = f_rho(y, c, n);
        u64 diff = x > y ? x - y : y - x;
        d = std::gcd(diff, n);
      } while(d == 1);
      if (d == n) continue;
    }
    return d; // 1 < d < n
  }
}

// Miller–Rabin Primality Test (deterministic for 64-bit)
// ------------------------------------------------------
// What it does: Quickly checks if a 64-bit integer n is prime.
// How: Write n-1 = d * 2^s (d odd). For fixed bases a, verify that
//   a^d ≡ 1 (mod n) or a^{d*2^r} ≡ -1 (mod n) for some 0 <= r < s.
// If any base fails, n is composite. With the bases below, the test is
// *deterministic* for all 0 <= n < 2^64.
// Complexity: O(k * log n) modular multiplications, where k = 7 bases
// (each base needs one binary-exponentiation with ~log n squarings/mults).
// Notes: Skip small primes first; use 128-bit mul to avoid overflow.

using i64 = int64_t; using u128 = __uint128_t;

static inline u64 mul_mod_u64(u64 a, u64 b, u64 mod){ return (u64)((u128)a * b % mod); }

static inline u64 pow_mod_u64(u64 a, u64 e, u64 mod){
  u64 r = 1 % mod, x = a % mod;
  while(e){ if(e&1) r = mul_mod_u64(r, x, mod); x = mul_mod_u64(x, x, mod); e >>= 1; }
  return r;
}

// Deterministic bases for 64-bit: {2, 325, 9375, 28178, 450775, 9780504, 1795265022}
static inline bool miller_rabin_64(u64 n){
  if(n < 2) return false;
  static const u64 small[] = {2,3,5,7,11,13,17,19,23,0};
  for(int i=0; small[i]; ++i){ if(n % small[i] == 0) return n == small[i]; }
  u64 d = n - 1, s = 0; while((d & 1) == 0){ d >>= 1; ++s; }
  auto check = [&](u64 a)->bool{
    if(a % n == 0) return true;
    u64 x = pow_mod_u64(a, d, n);
    if(x == 1 || x == n-1) return true;
    for(u64 r=1; r<s; ++r){ x = mul_mod_u64(x, x, n); if(x == n-1) return true; }
    return false;
  };
  static const u64 A[] = {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
  for(u64 a : A) if(!check(a)) return false;
  return true;
}

// factorize_u64 — 64-bit integer factorization via Miller–Rabin + Pollard–Rho
// ---------------------------------------------------------------------------
// API:
//   void factorize_u64(u64 n, std::vector<std::pair<u64,int>>& pf);
//     -> fills pf with (prime, exponent), ascending by prime; n=1 yields empty.
// Requires:
//   - miller_rabin_64(u64) from your MR snippet
//   - pollard_rho_64(u64)  from your Rho snippet
//   - mul_mod_u64(...)     (already used by MR/Rho)
// Notes:
//   - Strips a few small primes first, then uses MR to detect primes,
//     otherwise splits with Rho and recurses.
//   - Heuristic expected time ≈ Õ(n^{1/4}) on random composites.

static inline void _factor_core(u64 n, std::vector<u64>& fac){
  if (n == 1) return;
  if ((n & 1ull) == 0){ fac.push_back(2); _factor_core(n >> 1, fac); return; }
  static const u64 sp[] = {3,5,7,11,13,17,19,23,29,31,37,0};
  for (int i = 0; sp[i]; ++i){
    u64 p = sp[i];
    if (n % p == 0){ do{ fac.push_back(p); n /= p; }while(n % p == 0); if (n == 1) return; }
  }
  if (miller_rabin_64(n)){ fac.push_back(n); return; }
  u64 d = pollard_rho_64(n);
  _factor_core(d, fac); _factor_core(n / d, fac);
}

static inline void factorize_u64(u64 n, std::vector<std::pair<u64,int>>& pf){
  std::vector<u64> fac; fac.reserve(16);
  if (n >= 2) _factor_core(n, fac);
  std::sort(fac.begin(), fac.end());
  pf.clear(); pf.reserve(fac.size());
  for (size_t i = 0; i < fac.size(); ){
    size_t j = i; while (j < fac.size() && fac[j] == fac[i]) ++j;
    pf.push_back({fac[i], (int)(j - i)});
    i = j;
  }
}


// ---------- utils ----------
static inline ll norm_mod_ll(ll a, ll m){ a%=m; if(a<0) a+=m; return a; }
static inline ll pow_mod_ll(ll a, long long e, ll m){
  i128 base = norm_mod_ll(a, m), res = 1;
  while(e>0){ if(e&1) res = (res*base)%m; base = (base*base)%m; e >>= 1; }
  return (ll)res;
}
static inline ll phi_prime_power(ll p, int k){ // φ(p^k) = (p-1)*p^{k-1}
  ll pk1 = 1; for(int i=1;i<k;i++) pk1 = (ll)((i128)pk1 * p);
  return (ll)((i128)pk1 * (p - 1));
}
static inline ll ipow_ll(ll a, ll e){ // integer power (clamped)
  i128 r=1, x=a;
  while(e>0){ if(e&1){ r*=x; if(r>(i128)LLONG_MAX) return LLONG_MAX; }
              x*=x; if(x>(i128)LLONG_MAX) x=LLONG_MAX; e>>=1; }
  return (ll)r;
}
static inline ll ceil_log_ge(ll a, ll T){ // min s>=0 : a^s >= T
  if(T<=1) return 0;
  if(a<=1) return (a==1? LLONG_MAX : 1);
  ll s=0; i128 cur=1;
  while(cur < T){ ++s; cur*=a; if(s>62) break; }
  return s;
}
static bool tower_ge(ll A, ll b, ll T){ // is A↑↑b >= T ?
  if(T<=1) return true;
  if(b<=0) return 1 >= T;
  if(b==1) return A >= T;
  if(A==0) return 0 >= T;      // for b>=2, 0^exp = 0
  if(A==1) return 1 >= T;
  ll need = ceil_log_ge(A, T);
  if(need==LLONG_MAX) return false;
  return tower_ge(A, b-1, need);
}
static ll tower_bounded_value(ll A, ll b, ll CAP){ // returns min(A↑↑b, CAP+1)
  if(b<=0) return 1;
  if(b==1) return (A <= CAP ? A : CAP+1);
  if(CAP<=1){
    if(A<=1) return (A<=CAP ? A : CAP+1);
    return CAP+1;
  }
  if(A<=1) return (A==0?0:1);
  ll need = ceil_log_ge(A, (CAP+1));
  ll exp = tower_bounded_value(A, b-1, need-1);
  if(exp >= need) return CAP+1;
  ll val = ipow_ll(A, exp);
  return (val <= CAP ? val : CAP+1);
}

// ---------- CRT for coprime moduli ----------
static inline long long exgcd_ll(long long a,long long b,long long& x,long long& y){
  if(b==0){ x=(a>=0?1:-1); y=0; return a>=0?a:-a; }
  long long x1,y1; long long g=exgcd_ll(b,a%b,x1,y1); x=y1; y=x1-(a/b)*y1; return g;
}
static inline bool crt_pair(ll r1, ll m1, ll r2, ll m2, ll &r, ll &m){
  long long x,y; long long g = exgcd_ll(m1, m2, x, y);
  if(g != 1 && g != -1) return false; // prime powers of distinct primes are coprime
  i128 t = r2 - r1; t = (t % m2 + m2) % m2;
  i128 k = ((i128)x % m2 + m2) % m2;
  t = (t * k) % m2;
  i128 mod = (i128)m1 * m2;
  i128 res = (i128)r1 + t * m1;
  res %= mod; if(res<0) res += mod;
  r = (ll)res; m = (ll)mod; return true;
}

// ---------- per prime-power ----------
static ll tetration_mod_prime_power(ll A, ll B, ll p, int k){
  ll pk = 1; for(int i=0;i<k;i++) pk = (ll)((i128)pk * p);
  if(B<=0) return 1 % pk;
  if(B==1) return norm_mod_ll(A, pk);
  if(pk==1) return 0;

  ll a = norm_mod_ll(A, pk);
  if(a==0){
    // FIX: handle 0^0 = 1 when the exponent E = A↑↑(B-1) equals 0.
    // For A=0: E = 0↑↑(B-1) = (B-1 is odd ? 0 : 1).
    if (A == 0 && ((B - 1) & 1)) return 1 % pk; // E==0  → 0^0 = 1
    return 0;                                    // otherwise 0^E = 0 (E>=1)
  }
  // v_p(A)
  int s=0; ll tmp=a;
  while(tmp % p == 0){ tmp/=p; ++s; if(s>=k) break; }
  ll A1 = tmp % pk;

  if(s>0){
    ll need = (k + s - 1) / s;
    if(tower_ge(A, B-1, need)) return 0;
    ll e = tower_bounded_value(A, B-1, need-1); // small exact exponent
    ll p_pow = 1; for(int i=0;i<s*e;i++) p_pow = (ll)((i128)p_pow * p);
    ll mod2 = 1; for(int i=0;i<k - s*e;i++) mod2 = (ll)((i128)mod2 * p);
    ll part = pow_mod_ll(A1 % mod2, e, mod2);
    return (ll)(( (i128)part * p_pow ) % pk);
  }
  // coprime: use Euler reduction; exponent ≡ (A↑↑(B-1) mod φ) + φ
  ll ph = phi_prime_power(p, k);
  ll r = tetration_mod(A, B-1, ph);    // only need exponent mod φ(p^k)
  return pow_mod_ll(a, r + ph, pk);
}

// ---------- main wrapper ----------
static inline ll tetration_mod(ll A, ll B, ll M){
  if(M==1) return 0;
  if(B<=0) return 1 % M;
  if(B==1) return norm_mod_ll(A, M);

  vector<pair<u64,int>> pf; factorize_u64((u64)M, pf);

  ll R = 0, MOD = 1;
  for(auto [pp, kk] : pf){
    ll pk = 1; for(int i=0;i<kk;i++) pk = (ll)((i128)pk * (ll)pp);
    ll ri = tetration_mod_prime_power(A, B, (ll)pp, kk);
    ll r2, m2;
    bool ok = crt_pair(R, MOD, ri % pk, pk, r2, m2);
    if(!ok) return 0; // shouldn't happen for prime-power CRT
    R = r2; MOD = m2;
  }
  R %= M; if(R<0) R += M;
  return R;
}
void solve() {
  int T; cin >> T;
  while (T--) {
    ll A, B, M; cin >> A >> B >> M;
    cout << tetration_mod(A, B, M) << endl;
  }
}

signed main() {
  cin.tie(0), ios::sync_with_stdio(0);
  solve();
}

