#include <bits/stdc++.h>
#define sz(x) ((int)x.size())

using namespace std;
using ll = long long;

constexpr ll MOD = 998244353;

// needs prime MOD. a = quotients(n): distinct n/k, ascending
// f stands for the array of f(1) + ... + f(a[i]) mod MOD
// dirichlet(n, f, g): f * g. dirichlet(n, f, h, true): the g
// with f * g = h, needs f(1) != 0. O(n^(2/3) log n)
// MOD^2 * 2sqrt(n) < 2^128 (MOD < 1e16 for n <= 1e12)
// exact integers (Mertens: f = a, h = all 1): take a prime
// MOD > 2|value| (1e15 + 37), then read x > MOD/2 as x - MOD
using u128 = unsigned __int128;
vector<ll> quotients(ll n){
  vector<ll> a;
  for(ll k=n; k; k=n/(n/k+1)) a.push_back(n/k);
  return a;
}
vector<ll> dirichlet(ll n, const vector<ll> &f, vector<ll> g,
    bool inv = false){
  auto a = quotients(n);
  int m = sz(a), l = (m + 1) / 2; // l = sqrt(n), a[i<l] = i+1
  int c = max(l-1, m-1 - 6*(int)cbrtl(n)); // a[c] ~ n^(2/3)
  vector<ll> h(m), pf(m), pg(m); // pf[i] = F[i] - F[i-1]
  vector<u128> acc(m);
  ll s = 1, iv = 1;
  if(inv){
    swap(g, h);
    for(ll e=MOD-2, b=f[0]; e; e>>=1, b=(u128)b*b%MOD)
      if(e & 1) iv = (u128)iv * b % MOD;
  }
  auto pt = [&](const vector<ll> &v, int i){
    return (v[i] - (i ? v[i-1] : 0) + MOD) % MOD;
  };
  for(int i=0; i<m; i++) pf[i] = pt(f, i), pg[i] = pt(g, i);
  for(int t=0; t<m; t++){
    ll x = a[t];
    if(t <= c){ // x <= a[c]: as if f(a[i]) = pf[i], else 0
      if(inv){ // acc[t]: f(u)g(v) over u > 1, uv in block t
        pg[t] = (u128)(pt(h, t) + MOD - acc[t]%MOD) * iv % MOD;
        g[t] = ((t ? g[t-1] : 0) + pg[t]) % MOD;
      }
      for(int j=0; j<=c && a[j]<=a[c]/x; j++){ // sum of a[c]/x
        ll p = a[j] * x;
        acc[p <= l ? p - 1 : m - n/p] += (u128)pf[j] * pg[t];
      }
      if(!inv) h[t] = ((t ? h[t-1] : 0) + acc[t] % MOD) % MOD;
      continue;
    }
    while((s+1)*(s+1) <= x) s++; // s = sqrt(x), x = n/(m-t)
    // h(x) = sum_{i<=s} f(i)G(x/i) + g(i)F(x/i) - F(s)G(s)
    u128 r = MOD * (u128)MOD - (u128)f[s-1]*g[s-1];
    for(ll i=1, w=m-t; i<=s; i++, w+=m-t){
      int j = w <= l ? m - w : n/w - 1; // a[j] = n/w = x/i
      r += (u128)pf[i-1]*g[j] + (u128)pg[i-1]*f[j];
    }
    if(!inv) h[t] = r % MOD;
    else g[t] = (u128)(h[t] + MOD - r % MOD) * iv % MOD;
  }
  return inv ? g : h;
}
//! ll n=1e8-1; auto a=quotients(n),b=a; vector<ll> e(sz(a),1);
//! for(auto&x:b) x*=2; auto mu=dirichlet(n,b,e,1); // mu/2
//! assert(mu.back()==964 && dirichlet(n,mu,b)==e);
//! assert(dirichlet(n,a,dirichlet(n,a,a),1)==a); // d / 1 = 1

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--){
        ll n;
        cin >> n;

        int sz = quotients(n).size();
        
        vector<ll> f(sz), g(sz);
        for (int i=0;i<sz;i++) cin >> f[i];
        for (int i=0;i<sz;i++) cin >> g[i];

        auto h = dirichlet(n, f, g);
        for (auto &x:h) printf("%lld ", x);
        printf("\n");
    }
}