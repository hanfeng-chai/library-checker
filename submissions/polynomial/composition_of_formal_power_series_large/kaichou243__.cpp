#include<bits/stdc++.h>
#include <immintrin.h>
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#define FOR1(a) for (ll _ = 0; _ < ll(a); ++_)
#define FOR2(i, a) for (ll i = 0; i < ll(a); ++i)
#define FOR3(i, a, b) for (ll i = a; i < ll(b); ++i)
#define FOR4(i, a, b, c) for (ll i = a; i < ll(b); i += (c))
#define FOR1_R(a) for (ll i = (a)-1; i >= ll(0); --i)
#define FOR2_R(i, a) for (ll i = (a)-1; i >= ll(0); --i)
#define FOR3_R(i, a, b) for (ll i = (b)-1; i >= ll(a); --i)
#define overload4(a, b, c, d, e, ...) e
#define overload3(a, b, c, d, ...) d
#define FOR(...) overload4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define FOR_R(...) overload3(__VA_ARGS__, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)
#define sz(c) ((int)(c).size())
#define ten(x) ((int)1e##x)
#define all(v) (v).begin(), (v).end()
using namespace std;
using ll=long long;
using P = pair<ll,ll>;
const long double PI=acos(-1);
const ll INF=1e18;
const int inf=1e9;
struct Edge {
    ll to;
    ll cost;
};
using Graph=vector<vector<Edge>>;
template <typename T>
bool chmax(T &a,const T& b){
  if (a<b){
    a=b;
    return true;
  }
  return false;
}
template <typename T>
bool chmin(T &a,const T& b){
  if (a>b){
    a=b;
    return true;
  }
  return false;
}
template<int MOD> struct Fp{
  ll val;
  constexpr Fp(long long v = 0) noexcept : val(v % MOD) {
    if (val < 0) val += MOD;
  }
  static constexpr int getmod() { return MOD; }
  constexpr Fp operator - () const noexcept {
    return val ? MOD - val : 0;
  }
  constexpr Fp operator + (const Fp& r) const noexcept { return Fp(*this) += r; }
  constexpr Fp operator - (const Fp& r) const noexcept { return Fp(*this) -= r; }
  constexpr Fp operator * (const Fp& r) const noexcept { return Fp(*this) *= r; }
  constexpr Fp operator / (const Fp& r) const noexcept { return Fp(*this) /= r; }
  constexpr Fp& operator += (const Fp& r) noexcept {
    val += r.val;
    if (val >= MOD) val -= MOD;
    return *this;
  }
  constexpr Fp& operator -= (const Fp& r) noexcept {
    val -= r.val;
    if (val < 0) val += MOD;
    return *this;
  }
  constexpr Fp& operator *= (const Fp& r) noexcept {
    val = val * r.val % MOD;
    return *this;
  }
  constexpr Fp& operator /= (const Fp& r) noexcept {
    ll a = r.val, b = MOD, u = 1, v = 0;
    while (b) {
      ll t = a / b;
      a -= t * b, swap(a, b);
      u -= t * v, swap(u, v);
    }
    val = val * u % MOD;
    if (val < 0) val += MOD;
    return *this;
  }
  constexpr bool operator == (const Fp& r) const noexcept {
    return this->val == r.val;
  }
  constexpr bool operator != (const Fp& r) const noexcept {
    return this->val != r.val;
  }
  constexpr bool operator < (const Fp& r) const noexcept {
    return this->val < r.val;
  }
  friend constexpr istream& operator >> (istream& is, Fp<MOD>& x) noexcept {
    is >> x.val;
    x.val %= MOD;
    if (x.val < 0) x.val += MOD;
    return is;
  }
  friend constexpr ostream& operator << (ostream& os, const Fp<MOD>& x) noexcept {
    return os << x.val;
  }
  friend constexpr Fp<MOD> modpow(const Fp<MOD>& a, long long n) noexcept {
    Fp<MOD> res=1,r=a;
    while(n){
      if(n&1) res*=r;
      r*=r;
      n>>=1;
    }
    return res;
  }
  friend constexpr Fp<MOD> modinv(const Fp<MOD>& r) noexcept {
        long long a = r.val, b = MOD, u = 1, v = 0;
        while (b) {
            long long t = a / b;
            a -= t * b, swap(a, b);
            u -= t * v, swap(u, v);
        }
        return Fp<MOD>(u);
  }
  ll get(){
      return val;
  }
  explicit operator bool()const{
		return val;
  }
};
template< uint32_t mod, bool fast = false >
struct MontgomeryModInt {
  using mint = MontgomeryModInt;
  using i32 = int32_t;
  using i64 = int64_t;
  using u32 = uint32_t;
  using u64 = uint64_t;
 
  static constexpr u32 get_r() {
    u32 ret = mod;
    for(i32 i = 0; i < 4; i++) ret *= 2 - mod * ret;
    return ret;
  }
 
  static constexpr u32 r = get_r();
  static constexpr u32 n2 = -u64(mod) % mod;
 
  static_assert(r * mod == 1, "invalid, r * mod != 1");
  static_assert(mod < (1 << 30), "invalid, mod >= 2 ^ 30");
  static_assert((mod & 1) == 1, "invalid, mod % 2 == 0");
 
  u32 a;
 
  MontgomeryModInt() : a{} {}
 
  MontgomeryModInt(const i64 &x)
      : a(reduce(u64(fast ? x : (x % mod + mod)) * n2)) {}
 
  static constexpr u32 reduce(const u64 &b) {
    return u32(b >> 32) + mod - u32((u64(u32(b) * r) * mod) >> 32);
  }
 
  constexpr mint& operator+=(const mint &p) {
    if(i32(a += p.a - 2 * mod) < 0) a += 2 * mod;
    return *this;
  }
 
  constexpr mint& operator-=(const mint &p) {
    if(i32(a -= p.a) < 0) a += 2 * mod;
    return *this;
  }
 
  constexpr mint& operator*=(const mint &p) {
    a = reduce(u64(a) * p.a);
    return *this;
  }
 
  constexpr mint& operator/=(const mint &p) {
    *this *= modinv(p);
    return *this;
  }
 
  constexpr mint operator-() const { return mint() - *this; }
 
  constexpr mint operator+(const mint &p) const { return mint(*this) += p; }
 
  constexpr mint operator-(const mint &p) const { return mint(*this) -= p; }
 
  constexpr mint operator*(const mint &p) const { return mint(*this) *= p; }
 
  constexpr mint operator/(const mint &p) const { return mint(*this) /= p; }
 
  constexpr bool operator==(const mint &p) const { return (a >= mod ? a - mod : a) == (p.a >= mod ? p.a - mod : p.a); }
 
  constexpr bool operator!=(const mint &p) const { return (a >= mod ? a - mod : a) != (p.a >= mod ? p.a - mod : p.a); }
 
  u32 get() const {
    u32 ret = reduce(a);
    return ret >= mod ? ret - mod : ret;
  }
 
  friend constexpr MontgomeryModInt<mod> modpow(const MontgomeryModInt<mod> &x,u64 n) noexcept {
    MontgomeryModInt<mod> ret(1), mul(x);
    while(n > 0) {
      if(n & 1) ret *= mul;
      mul *= mul;
      n >>= 1;
    }
    return ret;
  }
 
  friend constexpr MontgomeryModInt<mod> modinv(const MontgomeryModInt<mod> &r) noexcept {
        u64 a = r.get(), b = mod, u = 1, v = 0;
        while (b) {
            long long t = a / b;
            a -= t * b, swap(a, b);
            u -= t * v, swap(u, v);
        }
        return MontgomeryModInt<mod>(u);
  }
 
  friend ostream &operator<<(ostream &os, const mint &p) {
    return os << p.get();
  }
 
  friend istream &operator>>(istream &is, mint &a) {
    i64 t;
    is >> t;
    a = mint(t);
    return is;
  }
  static constexpr u32 getmod() { return mod; }
};
struct SCCgraph{
  // input
  vector<vector<int>> G, rG;
  // result
  vector<vector<int>> scc;
  vector<int> cmp;
  vector<vector<int>> dag;
  // constructor
  SCCgraph(int N) : G(N), rG(N) {}
  // add edge
  void add_edge(int u, int v) {
    G[u].push_back(v);
    rG[v].push_back(u);
  }
  // decomp
  vector<bool> seen;
  vector<int> vs, rvs;
  void dfs(int v) {
    seen[v] = true;
    for (auto e : G[v]) if (!seen[e]) dfs(e);
    vs.push_back(v);
  }
  void rdfs(int v, int k) {
    seen[v] = true;
    cmp[v] = k;
    for (auto e : rG[v]) if (!seen[e]) rdfs(e, k);
    rvs.push_back(v);
  }
  // reconstruct
  set<pair<int,int>> newEdges;
  void reconstruct() {
    int N = (int)G.size();
    int dV = (int)scc.size();
    dag.resize(dV);
    newEdges.clear();
    for (int i = 0; i < N; ++i) {
      int u = cmp[i];
      for (auto e : G[i]) {
        int v = cmp[e];
        if (u == v) continue;
        if (!newEdges.count({u, v})) {
          dag[u].push_back(v);
          newEdges.insert({u, v});
        }
      }
    }
  }
  void solve() {
    // first dfs
    int N = (int)G.size();
    seen.assign(N, false);
    vs.clear();
    for (int v = 0; v < N; ++v) if (!seen[v]) dfs(v);
    // back dfs
    int k = 0;
    scc.clear();
    cmp.assign(N, -1);
    seen.assign(N, false);
    for (int i = N - 1; i >= 0; --i) {
      if (!seen[vs[i]]) {
        rvs.clear();
        rdfs(vs[i], k++);
        scc.push_back(rvs);
      }
    }
    // reconstruct
    reconstruct();
  }
};
template<class T,T (*op)(T,T),T (*e)()> struct SegmentTree{
  int n;
  vector<T> dat;
  SegmentTree(int N){
    n=1;
    while(n<N)n*=2;
    dat.assign(2*n,e());
  }
  void add(int k,T x){
    k+=n;
    dat[k]+=x;
    while(k){
      k>>=1;
      dat[k]=op(dat[k*2],dat[k*2+1]);
    }
  }
  void apply(int k,T x){
    k+=n;
    dat[k]=op(dat[k],x);
    while(k){
      k>>=1;
      dat[k]=op(dat[k*2],dat[k*2+1]);
    }
  }
  void set(int k,T x){
    k+=n;
    dat[k]=x;
    while(k){
      k>>=1;
      dat[k]=op(dat[k*2],dat[k*2+1]);
    }
  }
  T query(int l,int r){
    T prodl=e(),prodr=e();
    l+=n;
    r+=n;
    while(l<r){
      if(l&1) prodl=op(prodl,dat[l++]);
      if(r&1) prodr=op(dat[--r],prodr);
      l>>=1;
      r>>=1;
    }
    return op(prodl,prodr);
  }
};
struct FenwickTree{
    int n;
    vector<ll> dat;
    FenwickTree(int N) : n(N+1), dat(n,0ll) {}
    void add(int k,ll x){
      k+=1;
      while(k<n){
          dat[k]+=x;
          k+=k&-k;
      }
    }
  //sum[0,k)
  ll sum(int k){
    ll ans=0;
    while(k){
      ans+=dat[k];
      k-=k&-k;
    }
    return ans;
  }
  //sum[l,r)
  ll sum(int l,int r){
    return sum(r)-sum(l);
  }
};
template<class S,S (*op)(S,S),S (*e)(),class F,S (*mapping)(F,S),F (*composition)(F,F),F (*id)()>
struct LazySegTree{
  private:
  int _n,size=1,idx=0;
  vector<S>seq;
  vector<F>lazy;
  void update(int k){seq[k]=op(seq[2*k],seq[2*k+1]);}
  void all_apply(int k,F f){
    seq[k]=mapping(f,seq[k]);
    if(k<size)lazy[k]=composition(f,lazy[k]);
  }
  void eval(int k){
    all_apply(2*k,lazy[k]);
    all_apply(2*k+1,lazy[k]);
    lazy[k]=id();
  }
  public:
  LazySegTree(int n):LazySegTree(vector<S>(n,e())){}
  LazySegTree(const vector<S>&v):_n(int(v.size())){
    while(size<_n)size<<=1,idx++;
    seq=vector<S>(2*size,e());
    lazy=vector<F>(2*size,id());
    for(int i=0;i<_n;i++)seq[size+i]=v[i];
    for(int i=size-1;i>=1;i--)update(i);
  }
  void set(int p,S x){
    p+=size;
    for(int i=idx;i>=1;i--)eval(p>>i);
    seq[p]=x;
    for(int i=1;i<=idx;i++)update(p>>i);
  }
  void add(int p,S x){
    p+=size;
    for(int i=idx;i>=1;i--)eval(p>>i);
    seq[p]+=x;
    for(int i=1;i<=idx;i++)update(p>>i);
  }
  S operator[](int p){
    p+=size;
    for(int i=idx;i>=1;i--)eval(p>>i);
    return seq[p];
  }
  S query(int l,int r){
    if(l==r)return e();
    S sml=e(),smr=e();
    l+=size,r+=size;
    for(int i=idx;i>=1;i--){
      if(((l>>i)<<i)!=l)eval(l>>i);
      if(((r>>i)<<i)!=r)eval(r>>i);
    }
    while(l<r){
      if(l&1)sml=op(sml,seq[l++]);
      if(r&1)smr=op(seq[--r],smr);
      l>>=1,r>>=1;
    }
    return op(sml,smr);
  }
  S all_query()const{return seq[1];}
  void apply(int p,F f){
    p+=size;
    for(int i=idx;i>=1;i--)eval(p>>i);
    seq[p]=mapping(f,seq[p]);
    for(int i=1;i<=idx;i++)update(p>>i);
  }
  void apply(int l,int r,F f){
    if(l==r)return ;
    l+=size;
    r+=size;
    for(int i=idx;i>=1;i--){
      if(((l>>i)<<i)!=l)eval(l>>i);
      if(((r>>i)<<i)!=r)eval((r-1)>>i);
    }
    int l2=l,r2=r;
    while(l<r){
      if(l&1)all_apply(l++,f);
      if(r&1)all_apply(--r,f);
      l>>=1;
      r>>=1;
    }
    l=l2,r=r2;
    for(int i=1;i<=idx;i++){
      if(((l>>i)<<i)!=l)update(l>>i);
      if(((r>>i)<<i)!=r)update((r-1)>>i);
    }
  }
};
struct UnionFind{
  int n;
  vector<int> data;
  vector<int> edge_num;
  UnionFind(int N) : n(N) , data(N,-1) , edge_num(N,0){}
  int root(int x){ // データxが属する木の根を再帰で得る：root(x) = {xの木の根}
    return data[x]<0?x:data[x]=root(data[x]);
  }
  void unite(int x, int y) {
    x=root(x);
    y=root(y);
    if(x!=y){
      if (data[x]>data[y]) swap(x, y);
      data[x]+=data[y];
      data[y]=x;
    }
    if(x!=y){
      edge_num[x]+=edge_num[y];
      edge_num[y]=0;
    }
    edge_num[x]+=1;
  }
  bool same(int x, int y){ // 2つのデータx, yが属する木が同じならtrueを返す
    return root(x)==root(y);
  }
  int size(int x){
    return -data[root(x)];
  }
  int edge(int x){
    return edge_num[root(x)];
  }
  vector<int> get_roots(){
    vector<int> res;
    for(int i=0;i<n;i++){
      if(data[i]<0) res.push_back(i);
    }
    return res;
  }
};
template< typename T >
struct WeightedUnionFind{
  vector<int> data;
  vector<T> we;
  WeightedUnionFind(int n) : data(n, -1), we(n) {}
  int root(int k) {
    if(data[k] < 0) return k;
    int par = root(data[k]);
    we[k] += we[data[k]];
    return data[k] = par;
  }
  T weight(int t) {
    root(t);
    return we[t];
  }
  bool unite(int x, int y, T w) {
    w += weight(x);
    w -= weight(y);
    x = root(x), y = root(y);
    if(x == y) return false;
    if(data[x] > data[y]) {
      swap(x, y);
      w *= -1;
    }
    data[x] += data[y];
    data[y] = x;
    we[y] = w;
    return true;
  }
  bool same(int x,int y){
    return root(x)==root(y);
  }
  int size(int x){
    return -data[root(x)];
  }
  T diff(int x, int y) {
    return weight(y) - weight(x);
  }
};
template< typename flow_t >
struct MFGraph{
  static_assert(is_integral< flow_t >::value, "template parameter flow_t must be integral type");
  const flow_t INF;
  struct edge {
    int to;
    flow_t cap;
    int rev;
    bool isrev;
    int idx;
  };

  vector< vector< edge > > graph;
  vector< int > min_cost, iter;
  flow_t max_cap;
  int SZ;

  explicit MFGraph(int V) : INF(numeric_limits< flow_t >::max()), graph(V), max_cap(0), SZ(V) {}

  void add_edge(int from, int to, flow_t cap, int idx = -1) {
    max_cap = max(max_cap, cap);
    graph[from].emplace_back((edge) {to, cap, (int) graph[to].size(), false, idx});
    graph[to].emplace_back((edge) {from, 0, (int) graph[from].size() - 1, true, idx});
  }

  bool build_augment_path(int s, int t, const flow_t &base) {
    min_cost.assign(graph.size(), -1);
    queue< int > que;
    min_cost[s] = 0;
    que.push(s);
    while(!que.empty() && min_cost[t] == -1) {
      int p = que.front();
      que.pop();
      for(auto &e : graph[p]) {
        if(e.cap >= base && min_cost[e.to] == -1) {
          min_cost[e.to] = min_cost[p] + 1;
          que.push(e.to);
        }
      }
    }
    return min_cost[t] != -1;
  }

  flow_t find_augment_path(int idx, const int t, flow_t base, flow_t flow) {
    if(idx == t) return flow;
    flow_t sum = 0;
    for(int &i = iter[idx]; i < (int)graph[idx].size(); i++) {
      edge &e = graph[idx][i];
      if(e.cap >= base && min_cost[idx] < min_cost[e.to]) {
        flow_t d = find_augment_path(e.to, t, base, min(flow - sum, e.cap));
        if(d > 0) {
          e.cap -= d;
          graph[e.to][e.rev].cap += d;
          sum += d;
          if(flow - sum < base) break;
        }
      }
    }
    return sum;
  }

  flow_t max_flow(int s, int t) {
    if(max_cap == flow_t(0)) return flow_t(0);
    flow_t flow = 0;
    for(int i = 63 - __builtin_clzll(max_cap); i >= 0; i--) {
      flow_t now = flow_t(1) << i;
      while(build_augment_path(s, t, now)) {
        iter.assign(graph.size(), 0);
        flow += find_augment_path(s, t, now, INF);
      }
    }
    return flow;
  }

  //after max_flow(s,t)
  vector<bool> min_cut(int s) {
        vector<bool> visited(SZ);
        queue<int> que;
        que.push(s);
        while (!que.empty()) {
            int p = que.front();
            que.pop();
            visited[p] = true;
            for (auto e : graph[p]) {
                if (e.cap && !visited[e.to]) {
                    visited[e.to] = true;
                    que.push(e.to);
                }
            }
        }
        return visited;
    }
  
  void output() {
    for(int i = 0; i < graph.size(); i++) {
      for(auto &e : graph[i]) {
        if(e.isrev) continue;
        auto &rev_e = graph[e.to][e.rev];
        cout << i << "->" << e.to << " (flow: " << rev_e.cap << "/" << e.cap + rev_e.cap << ")" << endl;
      }
    }
  }
};
template< typename flow_t, typename cost_t >
struct MCFGraph{
  struct edge {
    int to;
    flow_t cap;
    cost_t cost;
    int rev;
    bool isrev;
  };

  vector< vector< edge > > graph;
  vector< cost_t > potential, min_cost;
  vector< int > prevv, preve;
  const cost_t INF;

  MCFGraph(int V) : graph(V), INF(numeric_limits< cost_t >::max()) {}

  void add_edge(int from, int to, flow_t cap, cost_t cost) {
    graph[from].emplace_back((edge) {to, cap, cost, (int) graph[to].size(), false});
    graph[to].emplace_back((edge) {from, 0, -cost, (int) graph[from].size() - 1, true});
  }

  cost_t min_cost_flow(int s, int t, flow_t f) {
    int V = (int) graph.size();
    cost_t ret = 0;
    using Pi = pair< cost_t, int >;
    priority_queue< Pi, vector< Pi >, greater< Pi > > que;
    potential.assign(V, 0);
    preve.assign(V, -1);
    prevv.assign(V, -1);

    while(f > 0) {
      min_cost.assign(V, INF);
      que.emplace(0, s);
      min_cost[s] = 0;
      while(!que.empty()) {
        Pi p = que.top();
        que.pop();
        if(min_cost[p.second] < p.first) continue;
        for(int i = 0; i < (int)graph[p.second].size(); i++) {
          edge &e = graph[p.second][i];
          cost_t nextCost = min_cost[p.second] + e.cost + potential[p.second] - potential[e.to];
          if(e.cap > 0 && min_cost[e.to] > nextCost) {
            min_cost[e.to] = nextCost;
            prevv[e.to] = p.second, preve[e.to] = i;
            que.emplace(min_cost[e.to], e.to);
          }
        }
      }
      if(min_cost[t] == INF) return -1;
      for(int v = 0; v < V; v++) potential[v] += min_cost[v];
      flow_t addflow = f;
      for(int v = t; v != s; v = prevv[v]) {
        addflow = min(addflow, graph[prevv[v]][preve[v]].cap);
      }
      f -= addflow;
      ret += addflow * potential[t];
      for(int v = t; v != s; v = prevv[v]) {
        edge &e = graph[prevv[v]][preve[v]];
        e.cap -= addflow;
        graph[v][e.rev].cap += addflow;
      }
    }
    return ret;
  }

  void output() {
    for(int i = 0; i < graph.size(); i++) {
      for(auto &e : graph[i]) {
        if(e.isrev) continue;
        auto &rev_e = graph[e.to][e.rev];
        cout << i << "->" << e.to << " (flow: " << rev_e.cap << "/" << rev_e.cap + e.cap << ")" << endl;
      }
    }
  }
};
ll mod(ll a, ll mod) {
    return (a%mod+mod)%mod;
}
ll modpow(ll a,ll n,ll mod){
  ll res=1;
  a%=mod;
  while (n>0){
    if (n & 1) res*=a;
    a *= a;
    a%=mod;
    n >>= 1;
    res%=mod;
  }
  return res;
}
vector<P> prime_factorize(ll N) {
  vector<P> res;
  for (ll a = 2; a * a <= N; ++a) {
    if (N % a != 0) continue;
    ll ex = 0;
    while(N % a == 0){
      ++ex;
      N /= a;
    }
    res.push_back({a, ex});
  }
  if (N != 1) res.push_back({N, 1});
  return res;
}
ll modinv(ll a, ll mod) {
    ll b = mod, u = 1, v = 0;
    while (b) {
        ll t = a/b;
        a -= t * b, swap(a, b);
        u -= t * v, swap(u, v);
    }
    u %= mod;
    if (u < 0) u += mod;
    return u;
}
ll extGcd(ll a, ll b, ll &p, ll &q) {  
    if (b == 0) { p = 1; q = 0; return a; }  
    ll d = extGcd(b, a%b, q, p);  
    q -= a/b * p;  
    return d;  
}
P ChineseRem(const vector<ll> &b, const vector<ll> &m) {
  ll r = 0, M = 1;
  for (int i = 0; i < (int)b.size(); ++i) {
    ll p, q;
    ll d = extGcd(M, m[i], p, q);
    if ((b[i] - r) % d != 0) return make_pair(0, -1);
    ll tmp = (b[i] - r) / d * p % (m[i]/d);
    r += M * tmp;
    M *= m[i]/d;
  }
  return make_pair(mod(r, M), M);
}
template <typename Key, typename Val, uint32_t N = 1 << 20,
          Val DefaultValue = Val()>
struct HashMap {
  using u32 = uint32_t;
  using u64 = uint64_t;

 private:
  Key* keys;
  Val* vals;
  bitset<N> flag;
  static constexpr u32 shift = 64 - __lg(N);

  static u64 rng() {
    u64 m = chrono::duration_cast<chrono::nanoseconds>(
                chrono::high_resolution_clock::now().time_since_epoch())
                .count();
    m ^= m >> 16;
    m ^= m << 32;
    return m;
  }

 public:
  HashMap() : keys(new Key[N]), vals(new Val[N]) {
    static_assert((N & (N - 1)) == 0 && N > 0);
  }

  Val& operator[](const Key& i) {
    static u64 r = rng();
    u32 hash = (u64(i) * r) >> shift;
    while (true) {
      if (!flag[hash]) {
        keys[hash] = i;
        flag[hash] = 1;
        return vals[hash] = DefaultValue;
      }
      if (keys[hash] == i) return vals[hash];
      hash = (hash + 1) & (N - 1);
    }
  }
};
template<int N>
struct Comb{
  HashMap<int,int,N> idx;
  vector<tuple<int,vector<ll>,vector<ll>>> mp;
  int n_,m;
  vector<P> pf;
  Comb(int n,int m_) : n_(n), m(m_){ 
    pf=prime_factorize(m);
    mp.resize(N);
    COMinit();
  }
  void init(int p, int pm, int cur) {
    idx[p]=cur;
    auto&[pms,ord,fac]=mp[cur];
    pms=pm;
    ord.resize(n_);
    fac.resize(n_);
    ord[0]=ord[1]=0;
    fac[0]=fac[1]=1;
    for (int i=2;i<n_;i++) {
      long long add=0;
      long long ni=i;
      while (ni % p == 0) add++,ni/=p;
      ord[i]=ord[i-1]+add;
      fac[i]=fac[i-1]*ni%pm;
    }
  }
  void COMinit(){
    int cur=0;
    for(auto& [p,e] : pf){
      int pfs=pow(p,e);
      init(p,pfs,cur);
      cur++;
    }
  }
  void com(ll n, ll r,int p,int i,vector<ll> &vb,vector<ll> &vm) {
    auto&[pms,ord,fac]=mp[idx[p]];
    if (n<0 || r<0 || n<r){
      vb[i]=0;
      vm[i]=pms;
      return;
    }
    ll e=ord[n]-ord[r]-ord[n-r];
    ll res=fac[n]*modinv(fac[r]*fac[n-r]%pms,pms)%pms;
    res=res*modpow(p,e,pms)%pms;
    vb[i]=res;
    vm[i]=pms;
  }
  ll operator()(int n, int k){
    if(n<0 || k<0 || n<k) return 0;
    int siz=pf.size();
    vector<long long> vb(siz), vm(siz);
    for (int i=0;i<siz;i++) {
      com(n,k,pf[i].first,i,vb,vm);
    }
    auto res = ChineseRem(vb,vm);
    return res.first;
  }
};
void dfs(const vector<vector<int>> &G,int v,vector<bool> &seen){
  seen[v]=true;
  for(auto nv : G[v]){
    if(seen[nv]) continue;
    dfs(G,nv,seen);
  }
}
template<typename T>
void dijkstra(const Graph &G,int s,vector<ll> &dist,vector<T> &cnt){
  int N = G.size();
  dist.assign(N, INF);
  cnt.assign(N,0);
  priority_queue<P, vector<P>, greater<P>> pq;
  dist[s] = 0;
  cnt[s] = 1;
  pq.emplace(dist[s], s);
  while (!pq.empty()){
    P p = pq.top();
    pq.pop();
    int v = p.second;
    if (dist[v] < p.first){
      continue;
    }
    for (auto e : G[v]){
      if (dist[e.to] > dist[v] + e.cost){
        dist[e.to] = dist[v] + e.cost;
        pq.emplace(dist[e.to], e.to);
        cnt[e.to]=cnt[v];
      }else if (dist[e.to] == dist[v] + e.cost){
        cnt[e.to]+=cnt[v];
      }
    }
  }
}
void bfs(const vector<vector<int>> &G,int s,vector<int> &dist){
  int N=G.size();
  dist.assign(N,-1);
  dist[s]=0;
  deque<int> dq;
  dq.push_back(s);
  while(dq.size()){
    int v=dq[0];
    dq.pop_front();
    for(auto nv : G[v]){
      if(dist[nv]!=-1) continue;
      dist[nv]=dist[v]+1;
      dq.push_back(nv);
    }
  }
}
vector<ll> enum_divisors(ll N){
  vector<ll> res;
  for (ll i = 1; i * i <= N; ++i){
    if (N % i == 0){
       res.push_back(i);
       // 重複しないならば i の相方である N/i も push
       if (N/i != i) res.push_back(N/i);
    }
  }
  // 小さい順に並び替える
  sort(res.begin(), res.end());
  return res;
}
ll Euler(ll n){
  vector<pair<ll,ll>> pf=prime_factorize(n);
  ll res=n;
  for(auto p : pf){
    res*=(p.first-1);
    res/=p.first;
  }
  return res;
}
//dist must be initialized with INF
void warshall_floyd(vector<vector<ll>> &dist,int n){
  for(int i=0;i<n;i++) dist[i][i]=0;
  for (int k = 0; k < n; k++){
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
      }
    }
  }
}
//[1,N]
vector<int> Eratosthenes(int N) {
  vector<int> isprime(N+1,1);
  isprime[1]=0;
  for (int p=2;p<=N;++p){
    if (!isprime[p]) continue;
    for (int q=p*2;q<=N;q+=p) {
      isprime[q]=0;
    }
  }
  return isprime;
}
//log X 素因数分解
struct PrimeFact{
  vector<int> era;
  PrimeFact(int maxA) : era(maxA+1,-1){
    for(int p=2;p<=maxA;++p){
      if(era[p]!=-1) continue;
      for(int q=p;q<=maxA;q+=p){
        era[q]=p;
      }
    }
  }
  vector<P> prime_fact(int x){
    vector<P> res;
    while(1<x){
      int p=era[x],ex=0;
      while(x%p==0){
        x/=p;
        ex++;
      }
      res.push_back({p,ex});
    }
    reverse(res.begin(),res.end());
    return res;
  }
};
namespace FFT {
  using Real = long double;
  struct Comp{
    Real re, im;
    Comp(){};
    Comp(Real re, Real im) : re(re), im(im) {}
    Real slen() const { return re*re+im*im; }
    Real real() { return re; }
    inline Comp operator+(const Comp &c) { return Comp(re+c.re, im+c.im); }
    inline Comp operator-(const Comp &c) { return Comp(re-c.re, im-c.im); }
    inline Comp operator*(const Comp &c) { return Comp(re*c.re-im*c.im, re*c.im+im*c.re); }
    inline Comp operator/(const Real &c) { return Comp(re/c, im/c); } 
    inline Comp conj() const { return Comp(re, -im); }
  };
  void fft(vector<Comp>& a, bool inverse){
    int n = a.size();
    static bool prepared = false;
    static vector<Comp> z(30);
    if(!prepared){
      prepared = true;
      for(int i=0; i<30; i++){
        Real ang = 2*PI/(1 << (i+1));
        z[i] = Comp(cos(ang), sin(ang));
      }
    }
    for (size_t i = 0, j = 1; j < n; ++j) {
      for (size_t k = n >> 1; k > (i ^= k); k >>= 1);
      if (i > j) swap(a[i], a[j]);
    }
    for (int i = 0, t = 1; t < n; ++i, t <<= 1) {
      Comp bw = z[i];
      if(inverse) bw = bw.conj();
      for (int i = 0; i < n; i += t * 2) {
        Comp w(1,0);
        for (int j = 0; j < t; ++j) {
          int l = i + j, r = i + j + t;
          Comp c = a[l], d = a[r] * w;
          a[l] = c + d;
          a[r] = c - d;
          w = w * bw;
        }
      }
    }
    if(inverse)
      for(int i=0; i<n; i++) a[i] = a[i]/(Real)n;
  }
  template<typename T>
  vector<T> mul(vector<T> &a, vector<T> &b, bool issquare){
    int deg = a.size() + b.size();
    int n = 1;
    while(n < deg) n <<= 1;
    vector<Comp> fa,fb;
    fa.resize(n); fb.resize(n);
    for(int i=0;i<n;i++){
      if(i < a.size()) fa[i] = Comp(a[i], 0);
      else fa[i] = Comp(0,0);
      if(i < b.size()) fb[i] = Comp(b[i], 0);
      else fb[i] = Comp(0,0);
    }
    for(int i=0;i<b.size();i++) fb[i] = Comp(b[i], 0);
    fft(fa, false); 
    if(issquare) fb = fa;
    else fft(fb, false);
    for(int i=0;i<n;i++) fa[i] = fa[i] * fb[i];
    fft(fa, true);
    vector<T> res(n);
    for(int i=0;i<n;i++) res[i] = round(fa[i].re);
    return res;
  }
};
namespace NTT {
using i64 = int64_t;
using u32 = unsigned int;
using u64 = unsigned long long;
using i128 = __int128;
using u128 = unsigned __int128;
using f128 = __float128;
__attribute__((target("sse4.2"))) inline __m128i my128_mullo_epu32(
    const __m128i &a, const __m128i &b) {
  return _mm_mullo_epi32(a, b);
}
 
__attribute__((target("sse4.2"))) inline __m128i my128_mulhi_epu32(
    const __m128i &a, const __m128i &b) {
  __m128i a13 = _mm_shuffle_epi32(a, 0xF5);
  __m128i b13 = _mm_shuffle_epi32(b, 0xF5);
  __m128i prod02 = _mm_mul_epu32(a, b);
  __m128i prod13 = _mm_mul_epu32(a13, b13);
  __m128i prod = _mm_unpackhi_epi64(_mm_unpacklo_epi32(prod02, prod13),
                                    _mm_unpackhi_epi32(prod02, prod13));
  return prod;
}
 
__attribute__((target("sse4.2"))) inline __m128i montgomery_mul_128(
    const __m128i &a, const __m128i &b, const __m128i &r, const __m128i &m1) {
  return _mm_sub_epi32(
      _mm_add_epi32(my128_mulhi_epu32(a, b), m1),
      my128_mulhi_epu32(my128_mullo_epu32(my128_mullo_epu32(a, b), r), m1));
}
 
__attribute__((target("sse4.2"))) inline __m128i montgomery_add_128(
    const __m128i &a, const __m128i &b, const __m128i &m2, const __m128i &m0) {
  __m128i ret = _mm_sub_epi32(_mm_add_epi32(a, b), m2);
  return _mm_add_epi32(_mm_and_si128(_mm_cmpgt_epi32(m0, ret), m2), ret);
}
 
__attribute__((target("sse4.2"))) inline __m128i montgomery_sub_128(
    const __m128i &a, const __m128i &b, const __m128i &m2, const __m128i &m0) {
  __m128i ret = _mm_sub_epi32(a, b);
  return _mm_add_epi32(_mm_and_si128(_mm_cmpgt_epi32(m0, ret), m2), ret);
}
 
__attribute__((target("avx2"))) inline __m256i my256_mullo_epu32(
    const __m256i &a, const __m256i &b) {
  return _mm256_mullo_epi32(a, b);
}
 
__attribute__((target("avx2"))) inline __m256i my256_mulhi_epu32(
    const __m256i &a, const __m256i &b) {
  __m256i a13 = _mm256_shuffle_epi32(a, 0xF5);
  __m256i b13 = _mm256_shuffle_epi32(b, 0xF5);
  __m256i prod02 = _mm256_mul_epu32(a, b);
  __m256i prod13 = _mm256_mul_epu32(a13, b13);
  __m256i prod = _mm256_unpackhi_epi64(_mm256_unpacklo_epi32(prod02, prod13),
                                       _mm256_unpackhi_epi32(prod02, prod13));
  return prod;
}
 
__attribute__((target("avx2"))) inline __m256i montgomery_mul_256(
    const __m256i &a, const __m256i &b, const __m256i &r, const __m256i &m1) {
  return _mm256_sub_epi32(
      _mm256_add_epi32(my256_mulhi_epu32(a, b), m1),
      my256_mulhi_epu32(my256_mullo_epu32(my256_mullo_epu32(a, b), r), m1));
}
 
__attribute__((target("avx2"))) inline __m256i montgomery_add_256(
    const __m256i &a, const __m256i &b, const __m256i &m2, const __m256i &m0) {
  __m256i ret = _mm256_sub_epi32(_mm256_add_epi32(a, b), m2);
  return _mm256_add_epi32(_mm256_and_si256(_mm256_cmpgt_epi32(m0, ret), m2),
                          ret);
}
 
__attribute__((target("avx2"))) inline __m256i montgomery_sub_256(
    const __m256i &a, const __m256i &b, const __m256i &m2, const __m256i &m0) {
  __m256i ret = _mm256_sub_epi32(a, b);
  return _mm256_add_epi32(_mm256_and_si256(_mm256_cmpgt_epi32(m0, ret), m2),
                          ret);
}
    int calc_primitive_root(int mod) {
        if (mod == 2) return 1;
        if (mod == 167772161) return 3;
        if (mod == 469762049) return 3;
        if (mod == 754974721) return 11;
        if (mod == 998244353) return 3;
        int divs[20] = {};
        divs[0] = 2;
        int cnt = 1;
        long long x = (mod - 1) / 2;
        while (x % 2 == 0) x /= 2;
        for (long long i = 3; i * i <= x; i += 2) {
            if (x % i == 0) {
                divs[cnt++] = i;
                while (x % i == 0) x /= i;
            }
        }
        if (x > 1) divs[cnt++] = x;
        for (int g = 2;; g++) {
            bool ok = true;
            for (int i = 0; i < cnt; i++) {
                if (modpow(g, (mod - 1) / divs[i], mod) == 1) {
                    ok = false;
                    break;
                }
            }
            if (ok) return g;
        }
    }
 
    int get_fft_size(int N, int M) {
        int size_a = 1, size_b = 1;
        while (size_a < N) size_a <<= 1;
        while (size_b < M) size_b <<= 1;
        return max(size_a, size_b) << 1;
    }
    constexpr int bsf_constexpr(unsigned int n) {
      int x = 0;
      while (!(n & (1 << x))) x++;
      return x;
    }
    int bsf(unsigned int n) {
      #ifdef _MSC_VER
      unsigned long index;
      _BitScanForward(&index, n);
      return index;
      #else
      return __builtin_ctz(n);
      #endif
    }
    template <class mint>
    struct fft_info{
      static constexpr int rank2 = bsf_constexpr(mint::getmod() - 1);
      std::array<mint, rank2 + 1> root;   // root[i]^(2^i) == 1
      std::array<mint, rank2 + 1> iroot;  // root[i] * iroot[i] == 1
      std::array<mint, std::max(0, rank2 - 2 + 1)> rate2;
      std::array<mint, std::max(0, rank2 - 2 + 1)> irate2;
 
      std::array<mint, std::max(0, rank2 - 3 + 1)> rate3;
      std::array<mint, std::max(0, rank2 - 3 + 1)> irate3;
      int g;
      fft_info(){
        int MOD=mint::getmod();
        g=calc_primitive_root(MOD);
        root[rank2] = modpow(mint(g),(MOD - 1) >> rank2);
        iroot[rank2] = modinv(root[rank2]);
        for (int i = rank2 - 1; i >= 0; i--) {
            root[i] = root[i + 1] * root[i + 1];
            iroot[i] = iroot[i + 1] * iroot[i + 1];
        }
 
        {
            mint prod = 1, iprod = 1;
            for (int i = 0; i <= rank2 - 2; i++) {
                rate2[i] = root[i + 2] * prod;
                irate2[i] = iroot[i + 2] * iprod;
                prod *= iroot[i + 2];
                iprod *= root[i + 2];
            }
        }
        {
            mint prod = 1, iprod = 1;
            for (int i = 0; i <= rank2 - 3; i++) {
                rate3[i] = root[i + 3] * prod;
                irate3[i] = iroot[i + 3] * iprod;
                prod *= iroot[i + 3];
                iprod *= root[i + 3];
            }
        }
      }
    };
    int ceil_pow2(int n) {
      int x = 0;
      while ((1U << x) < (unsigned int)(n)) x++;
      return x;
    }
    // number-theoretic transform
    template <class mint>
    void trans(std::vector<mint>& a) {
      int n = int(a.size());
      int h = ceil_pow2(n);
      int MOD=a[0].getmod();
      static const fft_info<mint> info;
 
      int len = 0;  // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed
      while (len < h) {
        if (h - len == 1) {
            int p = 1 << (h - len - 1);
            mint rot = 1;
            for (int s = 0; s < (1 << len); s++) {
                int offset = s << (h - len);
                for (int i = 0; i < p; i++) {
                    auto l = a[i + offset];
                    auto r = a[i + offset + p] * rot;
                    a[i + offset] = l + r;
                    a[i + offset + p] = l - r;
                }
                if (s + 1 != (1 << len))
                    rot *= info.rate2[bsf(~(unsigned int)(s))];
            }
            len++;
        } else {
            // 4-base
            int p = 1 << (h - len - 2);
            mint rot = 1, imag = info.root[2];
            for (int s = 0; s < (1 << len); s++) {
                mint rot2 = rot * rot;
                mint rot3 = rot2 * rot;
                int offset = s << (h - len);
                for (int i = 0; i < p; i++) {
                    auto mod2 = 1ULL * MOD * MOD;
                    auto a0 = 1ULL * a[i + offset].get();
                    auto a1 = 1ULL * a[i + offset + p].get() * rot.get();
                    auto a2 = 1ULL * a[i + offset + 2 * p].get() * rot2.get();
                    auto a3 = 1ULL * a[i + offset + 3 * p].get() * rot3.get();
                    auto a1na3imag =
                        1ULL * mint(a1 + mod2 - a3).get() * imag.get();
                    auto na2 = mod2 - a2;
                    a[i + offset] = a0 + a2 + a1 + a3;
                    a[i + offset + 1 * p] = a0 + a2 + (2 * mod2 - (a1 + a3));
                    a[i + offset + 2 * p] = a0 + na2 + a1na3imag;
                    a[i + offset + 3 * p] = a0 + na2 + (mod2 - a1na3imag);
                }
                if (s + 1 != (1 << len))
                    rot *= info.rate3[bsf(~(unsigned int)(s))];
            }
            len += 2;
        }
      }
    }
    template <class mint>
    void trans_inv(std::vector<mint>& a) {
      int n = int(a.size());
      int h = ceil_pow2(n);
 
      static const fft_info<mint> info;
      int MOD=a[0].getmod();
      int len = h;  // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed
      while (len) {
        if (len == 1) {
            int p = 1 << (h - len);
            mint irot = 1;
            for (int s = 0; s < (1 << (len - 1)); s++) {
                int offset = s << (h - len + 1);
                for (int i = 0; i < p; i++) {
                    auto l = a[i + offset];
                    auto r = a[i + offset + p];
                    a[i + offset] = l + r;
                    a[i + offset + p] =
                        (unsigned long long)(MOD + l.get() - r.get()) *
                        irot.get();
                    ;
                }
                if (s + 1 != (1 << (len - 1)))
                    irot *= info.irate2[bsf(~(unsigned int)(s))];
            }
            len--;
        } else {
            // 4-base
            int p = 1 << (h - len);
            mint irot = 1, iimag = info.iroot[2];
            for (int s = 0; s < (1 << (len - 2)); s++) {
                mint irot2 = irot * irot;
                mint irot3 = irot2 * irot;
                int offset = s << (h - len + 2);
                for (int i = 0; i < p; i++) {
                    auto a0 = 1ULL * a[i + offset + 0 * p].get();
                    auto a1 = 1ULL * a[i + offset + 1 * p].get();
                    auto a2 = 1ULL * a[i + offset + 2 * p].get();
                    auto a3 = 1ULL * a[i + offset + 3 * p].get();
 
                    auto a2na3iimag =
                        1ULL *
                        mint((MOD + a2 - a3) * iimag.get()).get();
 
                    a[i + offset] = a0 + a1 + a2 + a3;
                    a[i + offset + 1 * p] =
                        (a0 + (MOD - a1) + a2na3iimag) * irot.get();
                    a[i + offset + 2 * p] =
                        (a0 + a1 + (MOD - a2) + (MOD - a3)) *
                        irot2.get();
                    a[i + offset + 3 * p] =
                        (a0 + (MOD - a1) + (MOD - a2na3iimag)) *
                        irot3.get();
                }
                if (s + 1 != (1 << (len - 2)))
                    irot *= info.irate3[bsf(~(unsigned int)(s))];
            }
            len -= 2;
        }
      }
    }
namespace ntt_inner {
using u64 = uint64_t;
constexpr uint32_t get_pr(uint32_t mod) {
  if (mod == 2) return 1;
  u64 ds[32] = {};
  int idx = 0;
  u64 m = mod - 1;
  for (u64 i = 2; i * i <= m; ++i) {
    if (m % i == 0) {
      ds[idx++] = i;
      while (m % i == 0) m /= i;
    }
  }
  if (m != 1) ds[idx++] = m;
 
  uint32_t pr = 2;
  while (1) {
    int flg = 1;
    for (int i = 0; i < idx; ++i) {
      u64 a = pr, b = (mod - 1) / ds[i], r = 1;
      while (b) {
        if (b & 1) r = r * a % mod;
        a = a * a % mod;
        b >>= 1;
      }
      if (r == 1) {
        flg = 0;
        break;
      }
    }
    if (flg == 1) break;
    ++pr;
  }
  return pr;
}
 
constexpr int SZ_FFT_BUF = 1 << 23;
uint32_t _buf1[SZ_FFT_BUF] __attribute__((aligned(64)));
uint32_t _buf2[SZ_FFT_BUF] __attribute__((aligned(64)));
}  // namespace ntt_inner
 
template <typename mint>
struct NumberTheoreticTransform {
  static constexpr uint32_t mod = mint::getmod();
  static constexpr uint32_t pr = ntt_inner::get_pr(mint::getmod());
  static constexpr int level = __builtin_ctzll(mod - 1);
  mint dw[level], dy[level];
  mint *buf1, *buf2;
 
  constexpr NumberTheoreticTransform() {
    setwy(level);
    union raw_cast {
      mint dat;
      uint32_t _;
    };
    buf1 = &(((raw_cast *)(ntt_inner::_buf1))->dat);
    buf2 = &(((raw_cast *)(ntt_inner::_buf2))->dat);
  }
 
  constexpr void setwy(int k) {
    mint w[level], y[level];
    w[k - 1] = modpow(mint(pr),(mod - 1) / (1 << k));
    y[k - 1] = modinv(w[k - 1]);
    for (int i = k - 2; i > 0; --i)
      w[i] = w[i + 1] * w[i + 1], y[i] = y[i + 1] * y[i + 1];
    dw[0] = dy[0] = w[1] * w[1];
    dw[1] = w[1], dy[1] = y[1], dw[2] = w[2], dy[2] = y[2];
    for (int i = 3; i < k; ++i) {
      dw[i] = dw[i - 1] * y[i - 2] * w[i];
      dy[i] = dy[i - 1] * w[i - 2] * y[i];
    }
  }
 
  __attribute__((target("avx2"))) void ntt(mint *a, int n) {
    int k = n ? __builtin_ctz(n) : 0;
    if (k == 0) return;
    if (k == 1) {
      mint a1 = a[1];
      a[1] = a[0] - a[1];
      a[0] = a[0] + a1;
      return;
    }
    if (k & 1) {
      int v = 1 << (k - 1);
      if (v < 8) {
        for (int j = 0; j < v; ++j) {
          mint ajv = a[j + v];
          a[j + v] = a[j] - ajv;
          a[j] += ajv;
        }
      } else {
        const __m256i m0 = _mm256_set1_epi32(0);
        const __m256i m2 = _mm256_set1_epi32(mod + mod);
        int j0 = 0;
        int j1 = v;
        for (; j0 < v; j0 += 8, j1 += 8) {
          __m256i T0 = _mm256_loadu_si256((__m256i *)(a + j0));
          __m256i T1 = _mm256_loadu_si256((__m256i *)(a + j1));
          __m256i naj = montgomery_add_256(T0, T1, m2, m0);
          __m256i najv = montgomery_sub_256(T0, T1, m2, m0);
          _mm256_storeu_si256((__m256i *)(a + j0), naj);
          _mm256_storeu_si256((__m256i *)(a + j1), najv);
        }
      }
    }
    int u = 1 << (2 + (k & 1));
    int v = 1 << (k - 2 - (k & 1));
    mint one = mint(1);
    mint imag = dw[1];
    while (v) {
      if (v == 1) {
        mint ww = one, xx = one, wx = one;
        for (int jh = 0; jh < u;) {
          ww = xx * xx, wx = ww * xx;
          mint t0 = a[jh + 0], t1 = a[jh + 1] * xx;
          mint t2 = a[jh + 2] * ww, t3 = a[jh + 3] * wx;
          mint t0p2 = t0 + t2, t1p3 = t1 + t3;
          mint t0m2 = t0 - t2, t1m3 = (t1 - t3) * imag;
          a[jh + 0] = t0p2 + t1p3, a[jh + 1] = t0p2 - t1p3;
          a[jh + 2] = t0m2 + t1m3, a[jh + 3] = t0m2 - t1m3;
          xx *= dw[__builtin_ctz((jh += 4))];
        }
      } else if (v == 4) {
        const __m128i m0 = _mm_set1_epi32(0);
        const __m128i m1 = _mm_set1_epi32(mod);
        const __m128i m2 = _mm_set1_epi32(mod + mod);
        const __m128i r = _mm_set1_epi32(mint::r);
        const __m128i Imag = _mm_set1_epi32(imag.a);
        mint ww = one, xx = one, wx = one;
        for (int jh = 0; jh < u;) {
          if (jh == 0) {
            int j0 = 0;
            int j1 = v;
            int j2 = j1 + v;
            int j3 = j2 + v;
            int je = v;
            for (; j0 < je; j0 += 4, j1 += 4, j2 += 4, j3 += 4) {
              const __m128i T0 = _mm_loadu_si128((__m128i *)(a + j0));
              const __m128i T1 = _mm_loadu_si128((__m128i *)(a + j1));
              const __m128i T2 = _mm_loadu_si128((__m128i *)(a + j2));
              const __m128i T3 = _mm_loadu_si128((__m128i *)(a + j3));
              const __m128i T0P2 = montgomery_add_128(T0, T2, m2, m0);
              const __m128i T1P3 = montgomery_add_128(T1, T3, m2, m0);
              const __m128i T0M2 = montgomery_sub_128(T0, T2, m2, m0);
              const __m128i T1M3 = montgomery_mul_128(
                  montgomery_sub_128(T1, T3, m2, m0), Imag, r, m1);
              _mm_storeu_si128((__m128i *)(a + j0),
                               montgomery_add_128(T0P2, T1P3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j1),
                               montgomery_sub_128(T0P2, T1P3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j2),
                               montgomery_add_128(T0M2, T1M3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j3),
                               montgomery_sub_128(T0M2, T1M3, m2, m0));
            }
          } else {
            ww = xx * xx, wx = ww * xx;
            const __m128i WW = _mm_set1_epi32(ww.a);
            const __m128i WX = _mm_set1_epi32(wx.a);
            const __m128i XX = _mm_set1_epi32(xx.a);
            int j0 = jh * v;
            int j1 = j0 + v;
            int j2 = j1 + v;
            int j3 = j2 + v;
            int je = j1;
            for (; j0 < je; j0 += 4, j1 += 4, j2 += 4, j3 += 4) {
              const __m128i T0 = _mm_loadu_si128((__m128i *)(a + j0));
              const __m128i T1 = _mm_loadu_si128((__m128i *)(a + j1));
              const __m128i T2 = _mm_loadu_si128((__m128i *)(a + j2));
              const __m128i T3 = _mm_loadu_si128((__m128i *)(a + j3));
              const __m128i MT1 = montgomery_mul_128(T1, XX, r, m1);
              const __m128i MT2 = montgomery_mul_128(T2, WW, r, m1);
              const __m128i MT3 = montgomery_mul_128(T3, WX, r, m1);
              const __m128i T0P2 = montgomery_add_128(T0, MT2, m2, m0);
              const __m128i T1P3 = montgomery_add_128(MT1, MT3, m2, m0);
              const __m128i T0M2 = montgomery_sub_128(T0, MT2, m2, m0);
              const __m128i T1M3 = montgomery_mul_128(
                  montgomery_sub_128(MT1, MT3, m2, m0), Imag, r, m1);
              _mm_storeu_si128((__m128i *)(a + j0),
                               montgomery_add_128(T0P2, T1P3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j1),
                               montgomery_sub_128(T0P2, T1P3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j2),
                               montgomery_add_128(T0M2, T1M3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j3),
                               montgomery_sub_128(T0M2, T1M3, m2, m0));
            }
          }
          xx *= dw[__builtin_ctz((jh += 4))];
        }
      } else {
        const __m256i m0 = _mm256_set1_epi32(0);
        const __m256i m1 = _mm256_set1_epi32(mod);
        const __m256i m2 = _mm256_set1_epi32(mod + mod);
        const __m256i r = _mm256_set1_epi32(mint::r);
        const __m256i Imag = _mm256_set1_epi32(imag.a);
        mint ww = one, xx = one, wx = one;
        for (int jh = 0; jh < u;) {
          if (jh == 0) {
            int j0 = 0;
            int j1 = v;
            int j2 = j1 + v;
            int j3 = j2 + v;
            int je = v;
            for (; j0 < je; j0 += 8, j1 += 8, j2 += 8, j3 += 8) {
              const __m256i T0 = _mm256_loadu_si256((__m256i *)(a + j0));
              const __m256i T1 = _mm256_loadu_si256((__m256i *)(a + j1));
              const __m256i T2 = _mm256_loadu_si256((__m256i *)(a + j2));
              const __m256i T3 = _mm256_loadu_si256((__m256i *)(a + j3));
              const __m256i T0P2 = montgomery_add_256(T0, T2, m2, m0);
              const __m256i T1P3 = montgomery_add_256(T1, T3, m2, m0);
              const __m256i T0M2 = montgomery_sub_256(T0, T2, m2, m0);
              const __m256i T1M3 = montgomery_mul_256(
                  montgomery_sub_256(T1, T3, m2, m0), Imag, r, m1);
              _mm256_storeu_si256((__m256i *)(a + j0),
                                  montgomery_add_256(T0P2, T1P3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j1),
                                  montgomery_sub_256(T0P2, T1P3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j2),
                                  montgomery_add_256(T0M2, T1M3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j3),
                                  montgomery_sub_256(T0M2, T1M3, m2, m0));
            }
          } else {
            ww = xx * xx, wx = ww * xx;
            const __m256i WW = _mm256_set1_epi32(ww.a);
            const __m256i WX = _mm256_set1_epi32(wx.a);
            const __m256i XX = _mm256_set1_epi32(xx.a);
            int j0 = jh * v;
            int j1 = j0 + v;
            int j2 = j1 + v;
            int j3 = j2 + v;
            int je = j1;
            for (; j0 < je; j0 += 8, j1 += 8, j2 += 8, j3 += 8) {
              const __m256i T0 = _mm256_loadu_si256((__m256i *)(a + j0));
              const __m256i T1 = _mm256_loadu_si256((__m256i *)(a + j1));
              const __m256i T2 = _mm256_loadu_si256((__m256i *)(a + j2));
              const __m256i T3 = _mm256_loadu_si256((__m256i *)(a + j3));
              const __m256i MT1 = montgomery_mul_256(T1, XX, r, m1);
              const __m256i MT2 = montgomery_mul_256(T2, WW, r, m1);
              const __m256i MT3 = montgomery_mul_256(T3, WX, r, m1);
              const __m256i T0P2 = montgomery_add_256(T0, MT2, m2, m0);
              const __m256i T1P3 = montgomery_add_256(MT1, MT3, m2, m0);
              const __m256i T0M2 = montgomery_sub_256(T0, MT2, m2, m0);
              const __m256i T1M3 = montgomery_mul_256(
                  montgomery_sub_256(MT1, MT3, m2, m0), Imag, r, m1);
              _mm256_storeu_si256((__m256i *)(a + j0),
                                  montgomery_add_256(T0P2, T1P3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j1),
                                  montgomery_sub_256(T0P2, T1P3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j2),
                                  montgomery_add_256(T0M2, T1M3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j3),
                                  montgomery_sub_256(T0M2, T1M3, m2, m0));
            }
          }
          xx *= dw[__builtin_ctz((jh += 4))];
        }
      }
      u <<= 2;
      v >>= 2;
    }
  }
 
  __attribute__((target("avx2"))) void intt(mint *a, int n,
                                            int normalize = true) {
    int k = n ? __builtin_ctz(n) : 0;
    if (k == 0) return;
    if (k == 1) {
      mint a1 = a[1];
      a[1] = a[0] - a[1];
      a[0] = a[0] + a1;
      if (normalize) {
        a[0] *= modinv(mint(2));
        a[1] *= modinv(mint(2));
      }
      return;
    }
    int u = 1 << (k - 2);
    int v = 1;
    mint one = mint(1);
    mint imag = dy[1];
    while (u) {
      if (v == 1) {
        mint ww = one, xx = one, yy = one;
        u <<= 2;
        for (int jh = 0; jh < u;) {
          ww = xx * xx, yy = xx * imag;
          mint t0 = a[jh + 0], t1 = a[jh + 1];
          mint t2 = a[jh + 2], t3 = a[jh + 3];
          mint t0p1 = t0 + t1, t2p3 = t2 + t3;
          mint t0m1 = (t0 - t1) * xx, t2m3 = (t2 - t3) * yy;
          a[jh + 0] = t0p1 + t2p3, a[jh + 2] = (t0p1 - t2p3) * ww;
          a[jh + 1] = t0m1 + t2m3, a[jh + 3] = (t0m1 - t2m3) * ww;
          xx *= dy[__builtin_ctz(jh += 4)];
        }
      } else if (v == 4) {
        const __m128i m0 = _mm_set1_epi32(0);
        const __m128i m1 = _mm_set1_epi32(mod);
        const __m128i m2 = _mm_set1_epi32(mod + mod);
        const __m128i r = _mm_set1_epi32(mint::r);
        const __m128i Imag = _mm_set1_epi32(imag.a);
        mint ww = one, xx = one, yy = one;
        u <<= 2;
        for (int jh = 0; jh < u;) {
          if (jh == 0) {
            int j0 = 0;
            int j1 = v;
            int j2 = v + v;
            int j3 = j2 + v;
            for (; j0 < v; j0 += 4, j1 += 4, j2 += 4, j3 += 4) {
              const __m128i T0 = _mm_loadu_si128((__m128i *)(a + j0));
              const __m128i T1 = _mm_loadu_si128((__m128i *)(a + j1));
              const __m128i T2 = _mm_loadu_si128((__m128i *)(a + j2));
              const __m128i T3 = _mm_loadu_si128((__m128i *)(a + j3));
              const __m128i T0P1 = montgomery_add_128(T0, T1, m2, m0);
              const __m128i T2P3 = montgomery_add_128(T2, T3, m2, m0);
              const __m128i T0M1 = montgomery_sub_128(T0, T1, m2, m0);
              const __m128i T2M3 = montgomery_mul_128(
                  montgomery_sub_128(T2, T3, m2, m0), Imag, r, m1);
              _mm_storeu_si128((__m128i *)(a + j0),
                               montgomery_add_128(T0P1, T2P3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j2),
                               montgomery_sub_128(T0P1, T2P3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j1),
                               montgomery_add_128(T0M1, T2M3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j3),
                               montgomery_sub_128(T0M1, T2M3, m2, m0));
            }
          } else {
            ww = xx * xx, yy = xx * imag;
            const __m128i WW = _mm_set1_epi32(ww.a);
            const __m128i XX = _mm_set1_epi32(xx.a);
            const __m128i YY = _mm_set1_epi32(yy.a);
            int j0 = jh * v;
            int j1 = j0 + v;
            int j2 = j1 + v;
            int j3 = j2 + v;
            int je = j1;
            for (; j0 < je; j0 += 4, j1 += 4, j2 += 4, j3 += 4) {
              const __m128i T0 = _mm_loadu_si128((__m128i *)(a + j0));
              const __m128i T1 = _mm_loadu_si128((__m128i *)(a + j1));
              const __m128i T2 = _mm_loadu_si128((__m128i *)(a + j2));
              const __m128i T3 = _mm_loadu_si128((__m128i *)(a + j3));
              const __m128i T0P1 = montgomery_add_128(T0, T1, m2, m0);
              const __m128i T2P3 = montgomery_add_128(T2, T3, m2, m0);
              const __m128i T0M1 = montgomery_mul_128(
                  montgomery_sub_128(T0, T1, m2, m0), XX, r, m1);
              __m128i T2M3 = montgomery_mul_128(
                  montgomery_sub_128(T2, T3, m2, m0), YY, r, m1);
              _mm_storeu_si128((__m128i *)(a + j0),
                               montgomery_add_128(T0P1, T2P3, m2, m0));
              _mm_storeu_si128(
                  (__m128i *)(a + j2),
                  montgomery_mul_128(montgomery_sub_128(T0P1, T2P3, m2, m0), WW,
                                     r, m1));
              _mm_storeu_si128((__m128i *)(a + j1),
                               montgomery_add_128(T0M1, T2M3, m2, m0));
              _mm_storeu_si128(
                  (__m128i *)(a + j3),
                  montgomery_mul_128(montgomery_sub_128(T0M1, T2M3, m2, m0), WW,
                                     r, m1));
            }
          }
          xx *= dy[__builtin_ctz(jh += 4)];
        }
      } else {
        const __m256i m0 = _mm256_set1_epi32(0);
        const __m256i m1 = _mm256_set1_epi32(mod);
        const __m256i m2 = _mm256_set1_epi32(mod + mod);
        const __m256i r = _mm256_set1_epi32(mint::r);
        const __m256i Imag = _mm256_set1_epi32(imag.a);
        mint ww = one, xx = one, yy = one;
        u <<= 2;
        for (int jh = 0; jh < u;) {
          if (jh == 0) {
            int j0 = 0;
            int j1 = v;
            int j2 = v + v;
            int j3 = j2 + v;
            for (; j0 < v; j0 += 8, j1 += 8, j2 += 8, j3 += 8) {
              const __m256i T0 = _mm256_loadu_si256((__m256i *)(a + j0));
              const __m256i T1 = _mm256_loadu_si256((__m256i *)(a + j1));
              const __m256i T2 = _mm256_loadu_si256((__m256i *)(a + j2));
              const __m256i T3 = _mm256_loadu_si256((__m256i *)(a + j3));
              const __m256i T0P1 = montgomery_add_256(T0, T1, m2, m0);
              const __m256i T2P3 = montgomery_add_256(T2, T3, m2, m0);
              const __m256i T0M1 = montgomery_sub_256(T0, T1, m2, m0);
              const __m256i T2M3 = montgomery_mul_256(
                  montgomery_sub_256(T2, T3, m2, m0), Imag, r, m1);
              _mm256_storeu_si256((__m256i *)(a + j0),
                                  montgomery_add_256(T0P1, T2P3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j2),
                                  montgomery_sub_256(T0P1, T2P3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j1),
                                  montgomery_add_256(T0M1, T2M3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j3),
                                  montgomery_sub_256(T0M1, T2M3, m2, m0));
            }
          } else {
            ww = xx * xx, yy = xx * imag;
            const __m256i WW = _mm256_set1_epi32(ww.a);
            const __m256i XX = _mm256_set1_epi32(xx.a);
            const __m256i YY = _mm256_set1_epi32(yy.a);
            int j0 = jh * v;
            int j1 = j0 + v;
            int j2 = j1 + v;
            int j3 = j2 + v;
            int je = j1;
            for (; j0 < je; j0 += 8, j1 += 8, j2 += 8, j3 += 8) {
              const __m256i T0 = _mm256_loadu_si256((__m256i *)(a + j0));
              const __m256i T1 = _mm256_loadu_si256((__m256i *)(a + j1));
              const __m256i T2 = _mm256_loadu_si256((__m256i *)(a + j2));
              const __m256i T3 = _mm256_loadu_si256((__m256i *)(a + j3));
              const __m256i T0P1 = montgomery_add_256(T0, T1, m2, m0);
              const __m256i T2P3 = montgomery_add_256(T2, T3, m2, m0);
              const __m256i T0M1 = montgomery_mul_256(
                  montgomery_sub_256(T0, T1, m2, m0), XX, r, m1);
              const __m256i T2M3 = montgomery_mul_256(
                  montgomery_sub_256(T2, T3, m2, m0), YY, r, m1);
              _mm256_storeu_si256((__m256i *)(a + j0),
                                  montgomery_add_256(T0P1, T2P3, m2, m0));
              _mm256_storeu_si256(
                  (__m256i *)(a + j2),
                  montgomery_mul_256(montgomery_sub_256(T0P1, T2P3, m2, m0), WW,
                                     r, m1));
              _mm256_storeu_si256((__m256i *)(a + j1),
                                  montgomery_add_256(T0M1, T2M3, m2, m0));
              _mm256_storeu_si256(
                  (__m256i *)(a + j3),
                  montgomery_mul_256(montgomery_sub_256(T0M1, T2M3, m2, m0), WW,
                                     r, m1));
            }
          }
          xx *= dy[__builtin_ctz(jh += 4)];
        }
      }
      u >>= 4;
      v <<= 2;
    }
    if (k & 1) {
      v = 1 << (k - 1);
      if (v < 8) {
        for (int j = 0; j < v; ++j) {
          mint ajv = a[j] - a[j + v];
          a[j] += a[j + v];
          a[j + v] = ajv;
        }
      } else {
        const __m256i m0 = _mm256_set1_epi32(0);
        const __m256i m2 = _mm256_set1_epi32(mod + mod);
        int j0 = 0;
        int j1 = v;
        for (; j0 < v; j0 += 8, j1 += 8) {
          const __m256i T0 = _mm256_loadu_si256((__m256i *)(a + j0));
          const __m256i T1 = _mm256_loadu_si256((__m256i *)(a + j1));
          __m256i naj = montgomery_add_256(T0, T1, m2, m0);
          __m256i najv = montgomery_sub_256(T0, T1, m2, m0);
          _mm256_storeu_si256((__m256i *)(a + j0), naj);
          _mm256_storeu_si256((__m256i *)(a + j1), najv);
        }
      }
    }
    if (normalize) {
      mint invn = modinv(mint(n));
      for (int i = 0; i < n; i++) a[i] *= invn;
    }
  }
 
  __attribute__((target("avx2"))) void inplace_multiply(
      int l1, int l2, int zero_padding = true) {
    int l = l1 + l2 - 1;
    int M = 4;
    while (M < l) M <<= 1;
    if (zero_padding) {
      for (int i = l1; i < M; i++) ntt_inner::_buf1[i] = 0;
      for (int i = l2; i < M; i++) ntt_inner::_buf2[i] = 0;
    }
    const __m256i m0 = _mm256_set1_epi32(0);
    const __m256i m1 = _mm256_set1_epi32(mod);
    const __m256i r = _mm256_set1_epi32(mint::r);
    const __m256i N2 = _mm256_set1_epi32(mint::n2);
    for (int i = 0; i < l1; i += 8) {
      __m256i a = _mm256_loadu_si256((__m256i *)(ntt_inner::_buf1 + i));
      __m256i b = montgomery_mul_256(a, N2, r, m1);
      _mm256_storeu_si256((__m256i *)(ntt_inner::_buf1 + i), b);
    }
    for (int i = 0; i < l2; i += 8) {
      __m256i a = _mm256_loadu_si256((__m256i *)(ntt_inner::_buf2 + i));
      __m256i b = montgomery_mul_256(a, N2, r, m1);
      _mm256_storeu_si256((__m256i *)(ntt_inner::_buf2 + i), b);
    }
    ntt(buf1, M);
    ntt(buf2, M);
    for (int i = 0; i < M; i += 8) {
      __m256i a = _mm256_loadu_si256((__m256i *)(ntt_inner::_buf1 + i));
      __m256i b = _mm256_loadu_si256((__m256i *)(ntt_inner::_buf2 + i));
      __m256i c = montgomery_mul_256(a, b, r, m1);
      _mm256_storeu_si256((__m256i *)(ntt_inner::_buf1 + i), c);
    }
    intt(buf1, M, false);
    const __m256i INVM = _mm256_set1_epi32((mint(M).inverse()).a);
    for (int i = 0; i < l; i += 8) {
      __m256i a = _mm256_loadu_si256((__m256i *)(ntt_inner::_buf1 + i));
      __m256i b = montgomery_mul_256(a, INVM, r, m1);
      __m256i c = my256_mulhi_epu32(my256_mullo_epu32(b, r), m1);
      __m256i d = _mm256_and_si256(_mm256_cmpgt_epi32(c, m0), m1);
      __m256i e = _mm256_sub_epi32(d, c);
      _mm256_storeu_si256((__m256i *)(ntt_inner::_buf1 + i), e);
    }
  }
 
  void ntt(vector<mint> &a) {
    int M = (int)a.size();
    for (int i = 0; i < M; i++) buf1[i].a = a[i].a;
    ntt(buf1, M);
    for (int i = 0; i < M; i++) a[i].a = buf1[i].a;
  }
 
  void intt(vector<mint> &a) {
    int M = (int)a.size();
    for (int i = 0; i < M; i++) buf1[i].a = a[i].a;
    intt(buf1, M, true);
    for (int i = 0; i < M; i++) a[i].a = buf1[i].a;
  }
 
  vector<mint> multiply(const vector<mint> &a, const vector<mint> &b) {
    if (a.size() == 0 && b.size() == 0) return vector<mint>{};
    int l = a.size() + b.size() - 1;
    if (min<int>(a.size(), b.size()) <= 40) {
      vector<mint> s(l);
      for (int i = 0; i < (int)a.size(); ++i)
        for (int j = 0; j < (int)b.size(); ++j) s[i + j] += a[i] * b[j];
      return s;
    }
    assert(l <= ntt_inner::SZ_FFT_BUF);
    int M = 4;
    while (M < l) M <<= 1;
    for (int i = 0; i < (int)a.size(); ++i) buf1[i].a = a[i].a;
    for (int i = (int)a.size(); i < M; ++i) buf1[i].a = 0;
    for (int i = 0; i < (int)b.size(); ++i) buf2[i].a = b[i].a;
    for (int i = (int)b.size(); i < M; ++i) buf2[i].a = 0;
    ntt(buf1, M);
    ntt(buf2, M);
    for (int i = 0; i < M; ++i)
      buf1[i].a = mint::reduce(uint64_t(buf1[i].a) * buf2[i].a);
    intt(buf1, M, false);
    vector<mint> s(l);
    mint invm = modinv(mint(M));
    for (int i = 0; i < l; ++i) s[i] = buf1[i] * invm;
    return s;
  }
 
  void ntt_doubling(vector<mint> &a) {
    int M = (int)a.size();
    for (int i = 0; i < M; i++) buf1[i].a = a[i].a;
    intt(buf1, M);
    mint r = 1, zeta = modpow(mint(pr),(mint::getmod() - 1) / (M << 1));
    for (int i = 0; i < M; i++) buf1[i] *= r, r *= zeta;
    ntt(buf1, M);
    a.resize(2 * M);
    for (int i = 0; i < M; i++) a[M + i].a = buf1[i].a;
  }
};
    // for garner
    static constexpr int m0 = 167772161;
    static constexpr int m1 = 469762049;
    static constexpr int m2 = 754974721;
    using mint0 = MontgomeryModInt<m0>;
    using mint1 = MontgomeryModInt<m1>;
    using mint2 = MontgomeryModInt<m2>;
    static constexpr int r01 = 104391568;
    static constexpr int r02 = 323560596;
    static constexpr int r12 = 399692502;
    static constexpr int r02r12 = 190329765;
    static constexpr i64 w1 = m0;
    static constexpr i64 w2 = i64(m0) * m1;
    using mint998 = MontgomeryModInt<998244353>;
    NumberTheoreticTransform<mint998> ntt998;
    NumberTheoreticTransform<mint0> ntt0;
    NumberTheoreticTransform<mint1> ntt1;
    NumberTheoreticTransform<mint2> ntt2;
    // small case (T = mint, long long)
    template<class T> vector<T> naive_mul 
    (const vector<T> &A, const vector<T> &B) {
        if (A.empty() || B.empty()) return {};
        int N = (int)A.size(), M = (int)B.size();
        vector<T> res(N + M - 1);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                res[i + j] += A[i] * B[j];
        return res;
    }
 
    // mint
    template<class mint>
    vector<mint> mul(vector<mint> A,vector<mint> B) {
        if (A.empty() || B.empty()) return {};
        int n = int(A.size()), m = int(B.size());
        if (min(n, m) < 30) return naive_mul(A, B);
        int MOD = A[0].getmod();
        if (MOD == 998244353) {
            vector<mint998> a(n),b(m);
            for(int i=0;i<n;i++) a[i]=mint998(A[i].get());
            for(int i=0;i<m;i++) b[i]=mint998(B[i].get());
            vector<mint998> c=ntt998.multiply(a,b);
            vector<mint> res(n+m-1);
            for(int i=0;i<n+m-1;i++) res[i]=c[i].get();
            return res;
        }
        vector<mint0> a0(n), b0(m);
        vector<mint1> a1(n), b1(m);
        vector<mint2> a2(n), b2(m);
        for (int i = 0; i < n; ++i)
            a0[i] = mint0(A[i].get()), a1[i] = mint1(A[i].get()), a2[i] = mint2(A[i].get());
        for (int i = 0; i < m; ++i)
            b0[i] = mint0(B[i].get()), b1[i] = mint1(B[i].get()), b2[i] = mint2(B[i].get());
        static const int W1 = w1%MOD, W2 = w2%MOD;
        vector<mint0> c0=ntt0.multiply(a0,b0);
        vector<mint1> c1=ntt1.multiply(a1,b1);
        vector<mint2> c2=ntt2.multiply(a2,b2);
        vector<mint> res(n + m - 1);
        for (int i = 0; i < n + m - 1; ++i) {
            int n1 = c1[i].get(), n2 = c2[i].get(), a = c0[i].get();
            int b = i64(n1 + m1 - a) * r01 % m1;
            int c = (i64(n2 + m2 - a) * r02r12 + i64(m2 - b) * r12) % m2;
            res[i] = mint(i64(a) + i64(b) * W1 + i64(c) * W2);
        }
        return res;
    }
};
// Formal Power Series
template <typename mint> struct FPS : vector<mint> {
    using vector<mint>::vector;
 /*
    template<class...Args>
    FPS(Args...args) : vector<mint>(args...){}
  */
    // constructor
    FPS(const vector<mint>& r) : vector<mint>(r) {}
 
    // core operator
    inline FPS pre(int siz) const {
        return FPS(begin(*this), begin(*this) + min((int)this->size(), siz));
    }
    inline FPS rev() const {
        FPS res = *this;
        reverse(begin(res), end(res));
        return res;
    }
    inline FPS& normalize() {
        while (!this->empty() && this->back() == 0) this->pop_back();
        return *this;
    }
 
    // basic operator
    inline FPS operator - () const noexcept {
        FPS res = (*this);
        for (int i = 0; i < (int)res.size(); ++i) res[i] = -res[i];
        return res;
    }
    
    
    inline void ntt() {
        NTT::ntt998.ntt(*this);
    }
    
    inline void intt() {
        NTT::ntt998.intt(*this);
    }
    
    inline void ntt_doubling(){
        NTT::ntt998.ntt_doubling(*this);
    }
    //*/
    
    inline FPS operator + (const mint& v) const { return FPS(*this) += v; }
    inline FPS operator + (const FPS& r) const { return FPS(*this) += r; }
    inline FPS operator - (const mint& v) const { return FPS(*this) -= v; }
    inline FPS operator - (const FPS& r) const { return FPS(*this) -= r; }
    inline FPS operator * (const mint& v) const { return FPS(*this) *= v; }
    inline FPS operator * (const FPS& r) const { return FPS(*this) *= r; }
    inline FPS operator / (const mint& v) const { return FPS(*this) /= v; }
    inline FPS operator << (int x) const { return FPS(*this) <<= x; }
    inline FPS operator >> (int x) const { return FPS(*this) >>= x; }
    inline FPS& operator += (const mint& v) {
        if (this->empty()) this->resize(1);
        (*this)[0] += v;
        return *this;
    }
    inline FPS& operator += (const FPS& r) {
        if (r.size() > this->size()) this->resize(r.size());
        for (int i = 0; i < (int)r.size(); ++i) (*this)[i] += r[i];
        return this->normalize();
    }
    inline FPS& operator -= (const mint& v) {
        if (this->empty()) this->resize(1);
        (*this)[0] -= v;
        return *this;
    }
    inline FPS& operator -= (const FPS& r) {
        if (r.size() > this->size()) this->resize(r.size());
        for (int i = 0; i < (int)r.size(); ++i) (*this)[i] -= r[i];
        return this->normalize();
    }
    inline FPS& operator *= (const mint& v) {
        for (int i = 0; i < (int)this->size(); ++i) (*this)[i] *= v;
        return *this;
    }
    inline FPS& operator *= (const FPS& r) {
        return *this = NTT::ntt998.multiply((*this), r);
    }
    inline FPS& operator /= (const mint& v) {
        assert(v != 0);
        mint iv = modinv(v);
        for (int i = 0; i < (int)this->size(); ++i) (*this)[i] *= iv;
        return *this;
    }
    inline FPS& operator <<= (int x) {
        FPS res(x, 0);
        res.insert(res.end(), begin(*this), end(*this));
        return *this = res;
    }
    inline FPS& operator >>= (int x) {
        FPS res;
        res.insert(res.end(), begin(*this) + x, end(*this));
        return *this = res;
    }
    inline mint eval(const mint& v){
        mint res = 0;
        for (int i = (int)this->size()-1; i >= 0; --i) {
            res *= v;
            res += (*this)[i];
        }
        return res;
    }
    inline friend FPS gcd(const FPS& f, const FPS& g) {
        if (g.empty()) return f;
        return gcd(g, f % g);
    }

    // advanced operation
    // df/dx
    inline friend FPS diff(const FPS& f) {
        int n = (int)f.size();
        FPS res(n-1);
        for (int i = 1; i < n; ++i) res[i-1] = f[i] * i;
        return res;
    }

    // \int f dx
    inline friend FPS integrate(const FPS& f) {
        int n = (int)f.size();
        FPS res(n+1, 0);
        for (int i = 0; i < n; ++i) res[i+1] = f[i] / (i+1);
        return res;
    }

    // inv(f), f[0] must not be 0
    /*inline friend FPS inv(const FPS& f, int deg) {
        assert(f[0] != 0);
        if (deg < 0) deg = (int)f.size();
        FPS res({mint(1) / f[0]});
        for (int i = 1; i < deg; i <<= 1) {
            res = (res + res - res * res * f.pre(i << 1)).pre(i << 1);
        }
        res.resize(deg);
        return res;
    }
    //*/
    
    inline friend FPS inv(const FPS& f, int deg) {
        assert(f[0]!=mint(0));
        if (deg < 0) deg = (int)f.size();
        FPS res(deg);
        res[0] = {mint(1)/f[0]};
        for (int d = 1; d < deg; d<<=1) {
            FPS g(2*d), h(2*d);
            for (int j = 0; j < min((int)f.size(),2*d); j++) g[j] = f[j];
            for (int j = 0; j < d; j++) h[j] = res[j];
            g.ntt();
            h.ntt();
            for (int j = 0; j < 2*d; j++) g[j]*=h[j];
            g.intt();
            for (int j = 0; j < d; j++) g[j]=0;
            g.ntt();
            for (int j = 0; j < 2*d; j++) g[j]*=h[j];
            g.intt();
            for (int j = d; j < min(2*d, deg); j++) res[j] = -g[j];
        }
        return res.pre(deg);
    }
    //*/
    inline friend FPS inv(const FPS& f) {
        return inv(f, f.size());
    }

    // division, r must be normalized (r.back() must not be 0)
    inline FPS& operator /= (const FPS& r) {
        const int n=(*this).size(),m=r.size();
        if(n<m){
            (*this).clear();
            return *this;
        }
        assert(r.back() != 0);
        this->normalize();
        if (this->size() < r.size()) {
            this->clear();
            return *this;
        }
        int need = (int)this->size() - (int)r.size() + 1;
        *this = ((*this).rev().pre(need) * inv(r.rev(), need)).pre(need).rev();
        return *this;
    }
    inline FPS& operator %= (const FPS &r) {
        const int n=(*this).size(),m=r.size();
        if(n<m) return (*this);
        assert(r.back() != 0);
        this->normalize();
        FPS q = (*this) / r;
        return *this -= q * r;
    }
    inline FPS operator / (const FPS& r) const { return FPS(*this) /= r; }
    inline FPS operator % (const FPS& r) const { return FPS(*this) %= r; }

    // log(f) = \int f'/f dx, f[0] must be 1
    inline friend FPS log(const FPS& f, int deg) {
        assert(f[0] == 1);
        FPS res = integrate((diff(f) * inv(f, deg)).pre(deg-1));
        return res;
    }
    inline friend FPS log(const FPS& f) {
        return log(f, f.size());
    }

    // exp(f), f[0] must be 0
    /*inline friend FPS exp(const FPS& f, int deg) {
        assert(f[0] == 0);
        FPS res(1, 1);
        for (int i = 1; i < deg; i <<= 1) {
            res = res * (f.pre(i<<1) - log(res, i<<1) + 1).pre(i<<1);
        }
        res.resize(deg);
        return res;
    }
    //*/
    
    inline friend FPS exp(const FPS& f, int deg) {
        assert(f.size()==0 || f[0]==mint(0));
        if(deg<0) deg=(int)f.size();
        FPS rf;
        rf.reserve(deg+1);
        rf.push_back(mint(0));
        rf.push_back(mint(1));
        
        auto inplace_integral = [&](FPS& F) -> void{
            const int n=(int)F.size();
            auto MOD=mint::getmod();
            while((int)rf.size()<=n){
                int i=rf.size();
                rf.push_back((-rf[MOD%i])*(MOD/i));
            }
            F.insert(begin(F),mint(0));
            for(int i=1;i<=n;i++) F[i]*=rf[i];
        };
        
        auto inplace_diff = [&](FPS& F) -> void{
            if(F.empty()) return;
            F.erase(begin(F));
            mint coeff=1,one=1;
            for(int i=0;i<(int)F.size();i++){
                F[i]*=coeff;
                coeff+=one;
            }
        };
        
        FPS b{1,(1<(int)f.size()?f[1]:0)},c{1},z1,z2{1,1};
        for(int m=2;m<deg;m<<=1){
            auto y=b;
            y.resize(2*m);
            y.ntt();
            z1=z2;
            FPS z(m);
            for(int i=0;i<m;i++) z[i]=y[i]*z1[i];
            z.intt();
            fill(begin(z),begin(z)+m/2,mint(0));
            z.ntt();
            for(int i=0;i<m;i++) z[i]*=-z1[i];
            z.intt();
            c.insert(end(c),begin(z)+m/2,end(z));
            z2=c;
            z2.resize(2*m);
            z2.ntt();
            FPS x(begin(f),begin(f)+min((int)f.size(),m));
            x.resize(m);
            inplace_diff(x);
            x.push_back(mint(0));
            x.ntt();
            for(int i=0;i<m;i++) x[i]*=y[i];
            x.intt();
            x-=diff(b);
            x.resize(2*m);
            for(int i=0;i<m-1;i++) x[m+i]=x[i],x[i]=mint(0);
            x.ntt();
            for(int i=0;i<2*m;i++) x[i]*=z2[i];
            x.intt();
            x.pop_back();
            inplace_integral(x);
            for(int i=m;i<min((int)f.size(),2*m);i++) x[i]+=f[i];
            fill(begin(x),begin(x)+m,mint(0));
            x.ntt();
            for(int i=0;i<2*m;i++) x[i]*=y[i];
            x.intt();
            b.insert(end(b),begin(x)+m,end(x));
        }
        return FPS{begin(b),begin(b)+deg};
    }
    
    inline friend FPS exp(const FPS& f) {
        return exp(f, f.size());
    }

    // pow(f) = exp(e * log f)
    inline friend FPS pow(const FPS& f, long long e, int deg) {
        long long i = 0;
        if(e==0){
            FPS res(deg);
            res[0]=1;
            return res;
        }
        while (i < (int)f.size() && f[i] == 0 && i * e < deg) ++i;
        if (i == (int)f.size()) return FPS(deg, 0);
        if (i * e >= deg) return FPS(deg, 0);
        mint k = f[i];
        FPS res = exp(log((f >> i) / k, deg) * mint(e), deg) * modpow(k, e) << (e * i);
        res.resize(deg);
        return res;
    }
    inline friend FPS pow(const FPS& f, long long e) {
        return pow(f, e, f.size());
    }

    // sqrt(f), f[0] must be 1
    inline friend FPS sqrt_base(const FPS& f, int deg) {
        assert(f[0] == 1);
        mint inv2 = mint(1) / 2;
        FPS res(1, 1);
        for (int i = 1; i < deg; i <<= 1) {
            res = (res + f.pre(i << 1) * inv(res, i << 1)).pre(i << 1);
            for (mint& x : res) x *= inv2;
        }
        res.resize(deg);
        return res;
    }
    inline friend FPS sqrt_base(const FPS& f) {
        return sqrt_base(f, f.size());
    }
    FPS taylor_shift(mint c) const {
      int n = (int) this->size();
      vector<mint> fact(n), rfact(n);
      fact[0] = rfact[0] = mint(1);
      for(int i = 1; i < n; i++) fact[i] = fact[i - 1] * mint(i);
      rfact[n - 1] = mint(1) / fact[n - 1];
      for(int i = n - 1; i > 1; i--) rfact[i - 1] = rfact[i] * mint(i);
      FPS p(*this);
      for(int i = 0; i < n; i++) p[i] *= fact[i];
      p = p.rev();
      FPS bs(n, mint(1));
      for(int i = 1; i < n; i++) bs[i] = bs[i - 1] * c * rfact[i] * fact[i - 1];
      p = (p * bs).pre(n);
      p = p.rev();
      for(int i = 0; i < n; i++) p[i] *= rfact[i];
      return p;
    }
};
struct SegP{
    int n;
    vector<P> dat;
    SegP(){}
    SegP(vector<P> v){
        int sz=v.size();
        n=1;
        while(n<sz) n*=2;
        dat.assign(2*n-1,{1e9,1e9});
        for(int i=0;i<sz;i++) dat[i+n-1]=v[i];
        for(int i=n-2;i>=0;i--) dat[i]=min(dat[2*i+1],dat[2*i+2]);
    }
    void resize(int N){
        n=1;
        while(n<N) n*=2;
        dat.assign(2*n-1,{1e9,1e9});
    }
    void vecset(vector<P> v){
        for(int i=0;i<v.size();i++){
            dat[i+n-1]=v[i];
        }
        for(int i=n-2;i>=0;i--) dat[i]=min(dat[2*i+1],dat[2*i+2]);
    }
    void update(int k,ll x){
        k+=(n-1);
        dat[k]={x,k};
        while(k>0){
            k=(k-1)/2;
            dat[k]=min(dat[2*k+1],dat[2*k+2]);
        }
    }
    P query(int l,int r,int k=0,int a=0,int b=-1){
        if(b<0) b=n;
        if(b<=l||r<=a) return make_pair(1e9,1e9);
        if(l<=a&&b<=r) return dat[k];
        P vl=query(l,r,2*k+1,a,(a+b)/2);
        P vr=query(l,r,2*k+2,(a+b)/2,b);
        return min(vl,vr);
    }
};
void Edfs(const Graph &G,int v,int p,vector<int> &ET){
  ET.push_back(v);
  for(auto nv : G[v]){
    if(nv.to==p) continue;
    Edfs(G,nv.to,v,ET);
    ET.push_back(v);
  }
}
void Ddfs(const Graph &G,int v,int p,vector<int> &depth){
  for(auto nv : G[v]){
    if(nv.to==p) continue;
    depth[nv.to]=depth[v]+1;
    Ddfs(G,nv.to,v,depth);
  }
}
//s=0
void Tbfs(const Graph &G,int s,vector<ll> &dist){
  int N=G.size();
  dist.assign(N,-1);
  dist[s]=0;
  deque<int> dq;
  dq.push_back(s);
  while(dq.size()){
    int v=dq[0];
    dq.pop_front();
    for(auto nv : G[v]){
      if(dist[nv.to]!=-1) continue;
      dist[nv.to]=dist[v]+nv.cost;
      dq.push_back(nv.to);
    }
  }
}
//first : p=-1
void dfs_sz(const vector<vector<int>> &G,int v,int p,vector<int> &sz){
  int ret=1;
  for(int nv : G[v]){
    if(nv==p) continue;
    dfs_sz(G,nv,v,sz);
    ret+=sz[nv];
  }
  sz[v]=ret;
}
struct LCA{
  vector<int> fst,ET,depth;
  vector<P> pv;
  SegP seg;
  vector<ll> dist;
  LCA(const Graph G){
    int n=G.size();
    fst.assign(n,-1);
    depth.resize(n);
    Edfs(G,0,-1,ET);
    Ddfs(G,0,-1,depth);
    for(int i=0;i<ET.size();i++){
      if(fst[ET[i]]==-1) fst[ET[i]]=i;
    }
    pv.resize(ET.size());
    for(int i=0;i<ET.size();i++){
      pv[i].first=depth[ET[i]];
      pv[i].second=ET[i];
    }
    seg.resize(pv.size());
    seg.vecset(pv);
    Tbfs(G,0,dist);
  }
  P lca(int u,int v){
    if(fst[u]>fst[v]) swap(u,v);
    P ret=seg.query(fst[u],fst[v]+1);
    return ret;
  }
  ll dis(int u,int v){
    P ca=lca(u,v);
    return dist[u]+dist[v]-2*dist[ca.second];
  }
};
vector<int> topo_sort(const vector<vector<int>> &G) { 
  vector<int> ret;
  int n = (int)G.size();
  vector<int> ind(n);      
  for (int i = 0; i < n; i++) {  
    for (auto e : G[i]) {
      ind[e]++;
    }
  }
  priority_queue<int,vector<int>,greater<int>> que;
  for (int i = 0; i < n; i++) {
    if (ind[i] == 0) {
      que.push(i);
    }
  }
  while (!que.empty()) { 
    int now = que.top();
    ret.push_back(now);
    que.pop();
    for (auto e : G[now]) {
      ind[e]--;
      if (ind[e] == 0) {
        que.push(e);
      }
    }
  }
  return ret;
}
template <typename mint> FPS<mint> product(vector<FPS<mint>> &a){
  int n=a.size();
  auto calc=[&](auto rec,int l,int r) -> FPS<mint>{
      if(l>=n) return FPS<mint>{1};
      if(l==r-1) return a[l];
      int m=l+(r-l)/2;
      return rec(rec,l,m)*rec(rec,m,r);
  };
  return calc(calc,0,n);
}
template<typename mint> FPS<mint> inv_sum(vector<FPS<mint>> &p,vector<FPS<mint>> &q,int M){
  int n=p.size();
  auto calc=[&](auto rec,int l,int r) -> pair<FPS<mint>,FPS<mint>>{
      if(l==r-1) return make_pair(p[l],q[l]);
      int m=l+(r-l)/2;
      auto [mol1,dem1]=rec(rec,l,m);
      auto [mol2,dem2]=rec(rec,m,r);
      return make_pair(mol1*dem2+mol2*dem1,dem1*dem2);
  };
  auto [mol,dem]=calc(calc,0,n);
  mol*=inv(dem,M);
  mol.resize(M);
  return mol;
}
template <typename mint> FPS<mint> rev(FPS<mint> p) { reverse(p.begin(),p.end()); return p; }
template <typename mint> FPS<mint> RSZ(FPS<mint> p, int x) { p.resize(x); return p; }
template<int m> struct Perm{
  unordered_map<int,tuple<ll,vector<ll>,vector<ll>>> mp;
  int n_;
  ll p_, pm_;
  vector<ll> ord_, fact_;
  vector<P> pf;
  Perm(int n) : n_(n), ord_(n), fact_(n) { 
    pf=prime_factorize(m); 
    PERMinit();
  }
  void init(int n) {
    ord_.resize(n);
    fact_.resize(n);
  }
  void init(long long p, long long pm) {
    p_=p,pm_=pm;
    ord_[0]=ord_[1]=0;
    fact_[0]=fact_[1]=1;
    auto&[pms,ord,fac]=mp[p];
    pms=pm;
    ord.resize(n_);
    fac.resize(n_);
    ord[0]=ord[1]=0;
    fac[0]=fac[1]=1;
    for (int i=2;i<n_;i++) {
      long long add=0;
      long long ni=i;
      while (ni % p == 0) add++,ni/=p;
      ord_[i]=ord_[i-1]+add;
      fact_[i]=fact_[i-1]*ni%pm;
      ord[i]=ord_[i];
      fac[i]=fact_[i];
    }
  }
  void init(long long p, long long pm, int n) {
    init(n);
    init(p, pm);
  }
  void PERMinit(){
    for(auto p : pf){
      ll ps=p.first,pfs=pow(p.first,p.second);
      init(n_);
      init(ps,pfs);
    }
  }
  ll perm(ll n, ll r,int p) {
    if (n<0 || r<0 || n<r) return 0;
    auto&[pms,ord,fac]=mp[p];
    ll e=ord[n]-ord[n-r];
    ll res=fac[n]*modinv(fac[n-r]%pms,pms)%pms;
    res=res*modpow(p,e,pms)%pms;
    return res;
  }
  ll operator()(int n, int k){
    if(n<0 || k<0 || n<k) return 0;
    vector<long long> vb, vm;
    for (auto ps : pf) {
        long long p = ps.first, e = ps.second;
        long long pm = pow(p,e);
        long long b = 1;
        b *= perm(n, k ,p) % pm;
        b %= pm;
        vm.push_back(pm);
        vb.push_back(b);
    }
    auto res = ChineseRem(vb,vm);
    return res.first;
  }
};
struct HLD{
  Graph G;
  vector<int> siz,a,ind,edge,dep,vertex,par;
  void dfs_sz(int v,int p){
    int ret=1;
    for(auto nv : G[v]){
      if(nv.to==p) continue;
      dfs_sz(nv.to,v);
      ret+=siz[nv.to];
    }
    siz[v]=ret;
  }
  void Ddfs(int v,int p){
    for(auto nv : G[v]){
      if(nv.to==p) continue;
      dep[nv.to]=dep[v]+1;
      Ddfs(nv.to,v);
    }
  }
  void hld(int v,int k,int p){
    ind[v]=vertex.size();
    a[v]=k;
    vertex.push_back(v);
    if(G[v].size()==1&&v!=0) return;
    int mxind=-1;
    int mx=0;
    for(int i=0;i<(int)G[v].size();i++){
      auto nv=G[v][i];
      if(nv.to==p) continue;
      if(siz[nv.to]>mx){
        mx=siz[nv.to];
        mxind=i;
      }
    }
    hld(G[v][mxind].to,k,v);
    for(int i=0;i<(int)G[v].size();i++){
      auto nv=G[v][i];
      if(nv.to==p) continue;
      if(i!=mxind) hld(nv.to,nv.to,v);
    }
  }
  void getpar(int v,int p){
    par[v]=p;
    for(auto nv : G[v]){
      if(nv.to==p) continue;
      getpar(nv.to,v);
      edge[ind[nv.to]]=nv.cost;
    }
  }
  HLD(const Graph &g) : siz((int)g.size()), a((int)g.size()), ind((int)g.size()), edge((int)g.size()), dep((int)g.size()), par((int)g.size()){
    G=g;
    dfs_sz(0,-1);
    hld(0,0,-1);
    getpar(0,-1);
    Ddfs(0,-1);
  }
  //[l,r]
  pair<int,int> getsubtree(int v){
    return make_pair(ind[v],ind[v]+siz[v]-1);
  }
  //[l,r]...
  vector<pair<int,int>> getpath(int u,int v){
    vector<pair<int,int>> ret;
    while(a[u]!=a[v]){
      if(dep[a[u]]<=dep[a[v]]){
        ret.push_back({ind[a[v]],ind[v]});
        v=par[a[v]];
      }else{
        ret.push_back({ind[a[u]],ind[u]});
        u=par[a[u]];
      }
    }
    ret.push_back({min(ind[u],ind[v]),max(ind[u],ind[v])});
    return ret;
  }
  int lca(int u,int v){
    vector<pair<int,int>> path=getpath(u,v);
    return vertex[path[path.size()-1].first];
  }
};
template <typename T>
vector<int> compress(vector<T> &x){
  vector<T> vals=x;
  vector<int> res;
  sort(vals.begin(),vals.end());
  auto it=unique(vals.begin(),vals.end());
  vals.erase(it,vals.end());
  for(auto xx : x){
    int ret=lower_bound(vals.begin(),vals.end(),xx) - vals.begin();
    res.push_back(ret);
  }
  return res;
}
/**
 * @brief Succinct Indexable Dictionary(完備辞書)
 */
struct SuccinctIndexableDictionary {
  size_t length;
  size_t blocks;
  vector< unsigned > bit, sum;

  SuccinctIndexableDictionary() = default;

  SuccinctIndexableDictionary(size_t length) : length(length), blocks((length + 31) >> 5) {
    bit.assign(blocks, 0U);
    sum.assign(blocks, 0U);
  }

  void set(int k) {
    bit[k >> 5] |= 1U << (k & 31);
  }

  void build() {
    sum[0] = 0U;
    for(int i = 1; i < blocks; i++) {
      sum[i] = sum[i - 1] + __builtin_popcount(bit[i - 1]);
    }
  }

  bool operator[](int k) {
    return (bool((bit[k >> 5] >> (k & 31)) & 1));
  }

  int rank(int k) {
    return (sum[k >> 5] + __builtin_popcount(bit[k >> 5] & ((1U << (k & 31)) - 1)));
  }

  int rank(bool val, int k) {
    return (val ? rank(k) : k - rank(k));
  }
};

/*
 * @brief Wavelet Matrix(ウェーブレット行列)
 * @docs docs/wavelet-matrix.md
 */
template< typename T, int MAXLOG >
struct WaveletMatrix {
  size_t length;
  SuccinctIndexableDictionary matrix[MAXLOG];
  int mid[MAXLOG];

  WaveletMatrix() = default;

  WaveletMatrix(vector< T > v) : length(v.size()) {
    vector< T > l(length), r(length);
    for(int level = MAXLOG - 1; level >= 0; level--) {
      matrix[level] = SuccinctIndexableDictionary(length + 1);
      int left = 0, right = 0;
      for(int i = 0; i < length; i++) {
        if(((v[i] >> level) & 1)) {
          matrix[level].set(i);
          r[right++] = v[i];
        } else {
          l[left++] = v[i];
        }
      }
      mid[level] = left;
      matrix[level].build();
      v.swap(l);
      for(int i = 0; i < right; i++) {
        v[left + i] = r[i];
      }
    }
  }

  pair< int, int > succ(bool f, int l, int r, int level) {
    return {matrix[level].rank(f, l) + mid[level] * f, matrix[level].rank(f, r) + mid[level] * f};
  }

  // v[k]
  T access(int k) {
    T ret = 0;
    for(int level = MAXLOG - 1; level >= 0; level--) {
      bool f = matrix[level][k];
      if(f) ret |= T(1) << level;
      k = matrix[level].rank(f, k) + mid[level] * f;
    }
    return ret;
  }

  T operator[](const int &k) {
    return access(k);
  }

  // count i s.t. (0 <= i < r) && v[i] == x
  int rank(const T &x, int r) {
    int l = 0;
    for(int level = MAXLOG - 1; level >= 0; level--) {
      tie(l, r) = succ((x >> level) & 1, l, r, level);
    }
    return r - l;
  }

  // k-th(0-indexed) smallest number in v[l,r)
  T kth_smallest(int l, int r, int k) {
    assert(0 <= k && k < r - l);
    T ret = 0;
    for(int level = MAXLOG - 1; level >= 0; level--) {
      int cnt = matrix[level].rank(false, r) - matrix[level].rank(false, l);
      bool f = cnt <= k;
      if(f) {
        ret |= T(1) << level;
        k -= cnt;
      }
      tie(l, r) = succ(f, l, r, level);
    }
    return ret;
  }

  // k-th(0-indexed) largest number in v[l,r)
  T kth_largest(int l, int r, int k) {
    return kth_smallest(l, r, r - l - k - 1);
  }

  // count i s.t. (l <= i < r) && (v[i] < upper)
  int range_freq(int l, int r, T upper) {
    int ret = 0;
    for(int level = MAXLOG - 1; level >= 0; level--) {
      bool f = ((upper >> level) & 1);
      if(f) ret += matrix[level].rank(false, r) - matrix[level].rank(false, l);
      tie(l, r) = succ(f, l, r, level);
    }
    return ret;
  }

  // count i s.t. (l <= i < r) && (lower <= v[i] < upper)
  int range_freq(int l, int r, T lower, T upper) {
    return range_freq(l, r, upper) - range_freq(l, r, lower);
  }

  // max v[i] s.t. (l <= i < r) && (v[i] < upper)
  T prev_value(int l, int r, T upper) {
    int cnt = range_freq(l, r, upper);
    return cnt == 0 ? T(-1) : kth_smallest(l, r, cnt - 1);
  }

  // min v[i] s.t. (l <= i < r) && (lower <= v[i])
  T next_value(int l, int r, T lower) {
    int cnt = range_freq(l, r, lower);
    return cnt == r - l ? T(-1) : kth_smallest(l, r, cnt);
  }
};

template< typename T, int MAXLOG >
struct CompressedWaveletMatrix {
  WaveletMatrix< int, MAXLOG > mat;
  vector< T > ys;

  CompressedWaveletMatrix(const vector< T > &v) : ys(v) {
    sort(begin(ys), end(ys));
    ys.erase(unique(begin(ys), end(ys)), end(ys));
    vector< int > t(v.size());
    for(int i = 0; i < v.size(); i++) t[i] = get(v[i]);
    mat = WaveletMatrix< int, MAXLOG >(t);
  }

  inline int get(const T& x) {
    return lower_bound(begin(ys), end(ys), x) - begin(ys);
  }

  T access(int k) {
    return ys[mat.access(k)];
  }

  T operator[](const int &k) {
    return access(k);
  }

  int rank(const T &x, int r) {
    auto pos = get(x);
    if(pos == ys.size() || ys[pos] != x) return 0;
    return mat.rank(pos, r);
  }

  T kth_smallest(int l, int r, int k) {
    return ys[mat.kth_smallest(l, r, k)];
  }

  T kth_largest(int l, int r, int k) {
    return ys[mat.kth_largest(l, r, k)];
  }

  int range_freq(int l, int r, T upper) {
    return mat.range_freq(l, r, get(upper));
  }

  int range_freq(int l, int r, T lower, T upper) {
    return mat.range_freq(l, r, get(lower), get(upper));
  }

  T prev_value(int l, int r, T upper) {
    auto ret = mat.prev_value(l, r, get(upper));
    return ret == -1 ? T(-1) : ys[ret];
  }

  T next_value(int l, int r, T lower) {
    auto ret = mat.next_value(l, r, get(lower));
    return ret == -1 ? T(-1) : ys[ret];
  }
};
struct MST{
  struct MSTEdge{
    ll u,v;
    ll cost;
    bool used;
    bool operator<(const MSTEdge& o) const {
      return cost < o.cost;
    }
  };
  int n;
  vector<MSTEdge> edges;
  MST(int sz) : n(sz){}
  void add_edge(int u,int v,ll c){
    edges.push_back({u,v,c,false});
  }
  ll kruskal(){
    UnionFind uf(n);
    sort(all(edges));
    ll min_cost=0;
    for(int i=0;i<sz(edges);i++){
      auto& [u,v,c,used]=edges[i];
      if(!uf.same(u,v)){
        uf.unite(u,v);
        used=true;
        min_cost+=c;
      }else{
          used=false;
      }
    }
    return min_cost;
  }
  Graph Tree(){
    kruskal();
    Graph G(n);
    for(int i=0;i<sz(edges);i++){
      if(edges[i].used){
        G[edges[i].u].push_back({edges[i].v,edges[i].cost});
        G[edges[i].v].push_back({edges[i].u,edges[i].cost});
      }
    }
    return G;
  }
};
template<typename T>
vector<T> shortest_path_faster_algorithm(const Graph &G, int s) {
  vector<T> dist(G.size(), INF);
  vector<int> pending(G.size(), 0), times(G.size(), 0);
  queue<int> que;
  que.emplace(s);
  pending[s]=true;
  ++times[s];
  dist[s]=0;
  while(!que.empty()) {
    int p = que.front();
    que.pop();
    pending[p] = false;
    for(auto &e : G[p]) {
      T next_cost = dist[p] + e.cost;
      if(next_cost >= dist[e.to]) continue;
      dist[e.to] = next_cost;
      if(!pending[e.to]) {
        if(++times[e.to] >= (int)G.size()) return vector<T>();
        pending[e.to] = true;
        que.emplace(e.to);
      }
    }
  }
  return dist;
}
template<typename mint>
struct subproduct_tree{
  using poly=FPS<mint>;
  vector<poly> tree;
  int n,siz;
  subproduct_tree(const vector<mint> &x){
    n=1;
    siz=sz(x);
    while(n<siz) n*=2;;
    tree.resize(2*n,{mint(1)});
    for(int i=0;i<siz;i++) tree[i+n]={-x[i],mint(1)};
    for(int i=n-1;i>0;i--) tree[i]=tree[2*i]*tree[2*i+1];
  }
  vector<mint> multieval(const poly &f){
    vector<poly> remainder(2*n);
    remainder[1]=f%tree[1];
    for(int i=1;i<n;i++){
      remainder[2*i]=remainder[i]%tree[2*i];
      remainder[2*i+1]=remainder[i]%tree[2*i+1];
    }
    vector<mint> ret(siz);
    for(int i=0;i<siz;i++){
        if(remainder[i+n].empty()) ret[i]=0;
        else ret[i]=remainder[i+n][0];
    }
    return ret;
  }
  poly interpolate(const vector<mint> &y){
    poly g=diff(tree[1]);
    vector<mint> evaled=multieval(g);
    vector<poly> mol(2*n),dem(2*n,{1});
    for(int i=0;i<siz;++i){
      mol[i+n]={y[i]};
      dem[i+n]=tree[i+n]*evaled[i];
    }
    for(int i=n-1;i>0;--i){
      dem[i]=dem[2*i]*dem[2*i+1];
      mol[i]=mol[2*i]*dem[2*i+1]+mol[2*i+1]*dem[2*i];
    }
    mol[1]*=inv(dem[1]);
    return RSZ(tree[1]*mol[1],siz);
  }
};
template <typename mint> vector<mint> multieval(const FPS<mint> &f,const vector<mint> &x){
  subproduct_tree<mint> tree(x);
  return tree.multieval(f);
}
template <typename mint> FPS<mint> interpolate(const vector<mint> &x,const vector<mint> &y){
  assert(sz(x)==sz(y));
  if(sz(x)==1) return {y[0]};
  subproduct_tree<mint> tree(x);
  return tree.interpolate(y);
}
template <typename mint> struct PartitionNumber{
    using poly=FPS<mint>;
    poly f;
    PartitionNumber(int maxN,vector<int> st) : f(maxN+1){
        vector<mint> invs(maxN+1);
        for(int i=1;i<=maxN;i++) invs[i]=modinv(mint(i));
        for(auto& e:st){
            for(int j=1;j<=maxN/e;j++){
                f[e*j]+=invs[j];
            }
        }
        f=exp(f);
    }
    PartitionNumber(int maxN) : f(maxN+1){
        f[0]=mint(1);
        for(int k=1;k<=maxN;k++){
            if(1ll*k*(3*k+1)/2<=maxN) f[k*(3*k+1)/2]+=(k % 2 ? mint(-1) : mint(1));
            if(1ll*k*(3*k-1)/2<=maxN) f[k*(3*k-1)/2]+=(k % 2 ? mint(-1) : mint(1));
        }
        f=inv(f);
    }
    mint operator()(int n){
        return f[n];
    }
};
template< typename T >
struct Combination {
  vector< T > _fact, _rfact, _inv;

  Combination(int sz) : _fact(sz + 1), _rfact(sz + 1), _inv(sz + 1) {
    _fact[0] = _rfact[sz] = _inv[0] = 1;
    for(int i = 1; i <= sz; i++) _fact[i] = _fact[i - 1] * i;
    _rfact[sz] /= _fact[sz];
    for(int i = sz - 1; i >= 0; i--) _rfact[i] = _rfact[i + 1] * (i + 1);
    for(int i = 1; i <= sz; i++) _inv[i] = _rfact[i] * _fact[i - 1];
  }

  inline T fact(int k) const { return _fact[k]; }

  inline T rfact(int k) const { return _rfact[k]; }

  inline T inv(int k) const { return _inv[k]; }

  T P(int n, int r) const {
    if(r < 0 || n < r) return 0;
    return fact(n) * rfact(n - r);
  }

  T C(int p, int q) const {
    if(q < 0 || p < q) return 0;
    return fact(p) * rfact(q) * rfact(p - q);
  }

  T H(int n, int r) const {
    if(n < 0 || r < 0) return (0);
    return r == 0 ? 1 : C(n + r - 1, r);
  }
};
template< typename T >
T lagrange_poly(const vector< T > &y,ll t) {
  int N = y.size() - 1;
  Combination< T > comb(N);
  if(t <= N) return y[t];
  T ret(0);
  vector< T > dp(N + 1, 1), pd(N + 1, 1);
  for(int i = 0; i < N; i++) dp[i + 1] = dp[i] * (t - i);
  for(int i = N; i > 0; i--) pd[i - 1] = pd[i] * (t - i);
  for(int i = 0; i <= N; i++) {
    T tmp = y[i] * dp[i] * pd[i] * comb.rfact(i) * comb.rfact(N - i);
    if((N - i) & 1) ret -= tmp;
    else ret += tmp;
  }
  return ret;
}
/**
 * @brief Binomial(二項係数)
 * @docs docs/binomial.md
 */
template< typename T >
T binomial(ll N,ll K) {
  if(K < 0 || N < K) return 0;
  static vector< T > invs;
  if(invs.size() < K + 1) {
    int pre_sz = max(1, (int) invs.size());
    invs.resize(K + 1);
    for(int i = pre_sz; i <= K; i++) {
      invs[i] = T(1) / i;
    }
  }
  T ret = 1;
  for(int64_t i = 1; i <= K; ++i) {
    ret *= N;
    N--;
    ret *= invs[i];
  }
  return ret;
}
/*template<typename mint>
mint Bostan_Mori(ll n,FPS<mint> P,FPS<mint> Q){
    while(n){
        auto C=Q;
        for(int i=1;i<C.size();i+=2) C[i]*=-1;
        P*=C;
        Q*=C;
        FPS<mint> H;
        for(int i=(n&1ll);i<P.size();i+=2) H.push_back(P[i]);
        P=H;
        FPS<mint> L;
        for(int i=0;i<Q.size();i+=2) L.push_back(Q[i]);
        Q=L;
        n>>=1;
    }
    return P[0]/Q[0];
}
//*/
template <typename mint>
mint Bostan_Mori(ll n,FPS<mint> P,FPS<mint> Q){
    Q.normalize();
    mint ret=0;
    if(P.size()>=Q.size()){
        auto R=P/Q;
        P-=R*Q;
        P.normalize();
        if(n<(int)R.size()) ret+=R[n];
    }
    if((int)P.size()==0) return ret;
    int N=1;
    while(N<(int)Q.size()) N<<=1;
    P.resize(2*N);
    Q.resize(2*N);
    P.ntt();
    Q.ntt();
    vector<mint> S(2*N),T(2*N);
    vector<int> btr(N);
    for(int i=0,logn=__builtin_ctz(N);i<(1<<logn);i++){
        btr[i]=(btr[i>>1]>>1)+((i&1)<<(logn-1));
    }
    mint dw=modpow(modinv(mint(NTT::ntt998.pr)),(mint::getmod()-1)/(2*N));
    while(n){
        mint inv2=modinv(mint(2));
        T.resize(N);
        for(int i=0;i<N;i++) T[i]=Q[(i<<1)|0]*Q[(i<<1)|1];
        S.resize(N);
        if(n&1){
            for(auto& i : btr){
                S[i]=(P[(i<<1)|0]*Q[(i<<1)|1]-P[(i<<1)|1]*Q[(i<<1)|0])*inv2;
                inv2*=dw;
            }
        }else{
            for(int i=0;i<N;i++){
                S[i]=(P[(i<<1)|0]*Q[(i<<1)|1]+P[(i<<1)|1]*Q[(i<<1)|0])*inv2;
            }
        }
        swap(P,S);
        swap(Q,T);
        n>>=1;
        if(n<N) break;
        P.ntt_doubling();
        Q.ntt_doubling();
    }
    P.intt();
    Q.intt();
    return ret+(P*inv(Q))[n];
}
//*/
template<typename mint>
FPS<mint> BerlekampMassey(const FPS<mint> &f){
    const int n=(int)f.size();
    FPS<mint> b,c;
    b.reserve(n+1);
    c.reserve(n+1);
    b.push_back(mint(1));
    c.push_back(mint(1));
    mint y=mint(1);
    for(int ed=1;ed<=n;ed++){
        int l=int(c.size()),m=int(b.size());
        mint x=0;
        for(int i=0;i<l;i++) x+=c[i]*f[ed-l+i];
        b.emplace_back(mint(0));
        m++;
        if(x==mint(0)) continue;
        mint freq=x/y;
        if(l<m){
            auto tmp=c;
            c.insert(begin(c),m-l,mint(0));
            for(int i=0;i<m;i++) c[m-1-i]-=freq*b[m-1-i];
            b=tmp;
            y=x;
        }else{
            for(int i=0;i<m;i++) c[l-1-i]-=freq*b[m-1-i];
        }
    }
    reverse(begin(c),end(c));
    return c;
}
template<typename mint>
mint BMBM(ll n,const vector<mint> &a){
    using fps=FPS<mint>;
    if(n<(int)a.size()) return a[n];
    fps A=a;
    fps f=BerlekampMassey(A);
    assert(!f.empty()&&f[0]!=0);
    fps g=A.pre((int)f.size()-1)*f;
    g.resize(f.size()-1);
    return Bostan_Mori(n,g,f);
}
void Dijkstra(const Graph &G, int s, vector<long long> &dis, vector<int> &prev) {
    int N = G.size();
    dis.resize(N, INF);
    prev.resize(N, -1); // 初期化
    priority_queue<P, vector<P>, greater<P>> pq; 
    dis[s] = 0;
    pq.emplace(dis[s], s);
    while (!pq.empty()) {
        P p = pq.top();
        pq.pop();
        int v = p.second;
        if (dis[v] < p.first) {
            continue;
        }
        for (auto &e : G[v]) {
            if (dis[e.to] > dis[v] + e.cost) {
                dis[e.to] = dis[v] + e.cost;
                prev[e.to] = v; // 頂点 v を通って e.to にたどり着いた
                pq.emplace(dis[e.to], e.to);
            }
        }
    }
}
template <class E, class V, E (*merge)(E, E), E (*e)(), E (*put_edge)(V, int), V (*put_vertex)(E, int)>
struct RerootingDP {
    struct edge {
        int to, idx, xdi;
    };
    RerootingDP(int n_ = 0) : n(n_), inner_edge_id(0) {
        es.resize(2*n-2);
        start.resize(2*n-2);
        if (n == 1) es_build();
    }
    void add_edge(int u, int v, int idx, int xdi){
        start[inner_edge_id] = u;
        es[inner_edge_id] = {v,idx,xdi};
        inner_edge_id++;
        start[inner_edge_id] = v;
        es[inner_edge_id] = {u,xdi,idx};
        inner_edge_id++;
        if (inner_edge_id == 2*n-2){
            es_build();
        }
    }
    vector<V> build(int root_ = 0){
        root = root_;
        vector<V> subdp(n); subdp[0] = put_vertex(e(),0);
        outs.resize(n);
        vector<int> geta(n+1,0);
        for (int i = 0; i < n; i++) geta[i+1] = start[i+1] - start[i] - 1;
        geta[root+1]++;
        for (int i = 0; i < n; i++) geta[i+1] += geta[i];
        auto dfs = [&](auto sfs, int v, int f) -> void {
            E val = e();
            for (int i = start[v]; i < start[v+1]; i++){
                if (es[i].to == f){
                    swap(es[start[v+1]-1],es[i]);
                }
                if (es[i].to == f) continue;
                sfs(sfs,es[i].to,v);
                E nval = put_edge(subdp[es[i].to],es[i].idx);
                outs[geta[v]++] = nval;
                val = merge(val,nval);
            }
            subdp[v] = put_vertex(val, v);
        };
        dfs(dfs,root,-1);
        return subdp;
    }
    vector<V> reroot(){
        vector<E> reverse_edge(n);
        reverse_edge[root] = e();
        vector<V> answers(n);
        auto dfs = [&](auto sfs, int v) -> void {
            int le = outs_start(v);
            int ri = outs_start(v+1);
            int siz = ri - le;
            vector<E> rui(siz+1);
            rui[siz] = e();
            for (int i = siz-1; i >= 0; i--){
                rui[i] = merge(outs[le+i],rui[i+1]);
            }
            answers[v] = put_vertex(merge(rui[0],reverse_edge[v]),v);
            E lui = e();
            for (int i = 0; i < siz; i++){
                V rdp = put_vertex(merge(merge(lui,rui[i+1]),reverse_edge[v]),v);
                reverse_edge[es[start[v]+i].to] = put_edge(rdp,es[start[v]+i].xdi);
                lui = merge(lui,outs[le+i]);
                sfs(sfs,es[start[v]+i].to);
            }
        };
        dfs(dfs,root);
        return answers;
    }
    private:
    int n, root, inner_edge_id;
    vector<E> outs;
    vector<edge> es;
    vector<int> start;
    int outs_start(int v){
        int res = start[v] - v;
        if (root < v) res++;
        return res;
    }
    void es_build(){
        vector<edge> nes(2*n-2);
        vector<int> nstart(n+2,0);
        for (int i = 0; i < 2*n-2; i++) nstart[start[i]+2]++;
        for (int i = 0; i < n; i++) nstart[i+1] += nstart[i];
        for (int i = 0; i < 2*n-2; i++) nes[nstart[start[i]+1]++] = es[i];
        swap(es,nes);
        swap(start,nstart);
    }
};
/*
    Knuth-Yao Speedup
    W : Monge,単調
    dp[i][j]=min[i<=s<j]{ dp[i][s]+dp[s+1][j] } + W[i][j]
    ただしdp[i][i]=x
    本質 : 
    Monge性を満たすf,gについて、K(i,j) := argmin{ f(i,s) + g(s+1,j) }を定めた時
    K(i,j) <= K(i,j+1) <= K(i+1,j+1)
    となる
*/
template<typename T> struct MongeDP{
    vector<vector<T>> dp,w;
    vector<vector<int>> K;
    int N;
    MongeDP(int siz,T Inf,T x,const vector<vector<T>> &W) : N(siz), w(W) {
        assert(siz==w.size()&&siz==w[0].size());
        dp.assign(siz,vector<T>(siz,Inf));
        K.assign(siz,vector<int>(siz));
        for(int i=0;i<siz;i++){
            K[i][i]=i;
            dp[i][i]=x;
        }
    }
    T solve(){
        for(int l=N-2;l>=0;l--){
            for(int r=l+1;r<N;r++){
                T C=w[l][r];
                for(int i=K[l][r-1];i<=min(r-1,K[l+1][r]);i++){
                    if(dp[l][r]>=dp[l][i]+dp[i+1][r]+C){
                        K[l][r]=i;
                        dp[l][r]=dp[l][i]+dp[i+1][r]+C;
                    }
                }
            }
        }
        return dp[0][N-1];
    }
};
//fast Input by yosupo
#include <unistd.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cstring>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
namespace fastio{
/*
  quote from yosupo's submission in Library Checker
*/
int bsr(unsigned int n) {
    return 8 * (int)sizeof(unsigned int) - 1 - __builtin_clz(n);
}
// @param n `1 <= n`
// @return maximum non-negative `x` s.t. `(n & (1 << x)) != 0`
int bsr(unsigned long n) {
    return 8 * (int)sizeof(unsigned long) - 1 - __builtin_clzl(n);
}
// @param n `1 <= n`
// @return maximum non-negative `x` s.t. `(n & (1 << x)) != 0`
int bsr(unsigned long long n) {
    return 8 * (int)sizeof(unsigned long long) - 1 - __builtin_clzll(n);
}
// @param n `1 <= n`
// @return minimum non-negative `x` s.t. `(n & (1 << x)) != 0`
int bsr(unsigned __int128 n) {
    unsigned long long low = (unsigned long long)(n);
    unsigned long long high = (unsigned long long)(n >> 64);
    return high ? 127 - __builtin_clzll(high) : 63 - __builtin_ctzll(low);
}
 
namespace internal {
 
template <class T>
using is_signed_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value ||
                                  std::is_same<T, __int128>::value,
                              std::true_type,
                              std::false_type>::type;
 
template <class T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value ||
                                  std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;
 
template <class T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value,
                              __uint128_t,
                              unsigned __int128>;
 
template <class T>
using is_integral =
    typename std::conditional<std::is_integral<T>::value ||
                                  internal::is_signed_int128<T>::value ||
                                  internal::is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;
 
template <class T>
using is_signed_int = typename std::conditional<(is_integral<T>::value &&
                                                 std::is_signed<T>::value) ||
                                                    is_signed_int128<T>::value,
                                                std::true_type,
                                                std::false_type>::type;
 
template <class T>
using is_unsigned_int =
    typename std::conditional<(is_integral<T>::value &&
                               std::is_unsigned<T>::value) ||
                                  is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;
 
template <class T>
using to_unsigned = typename std::conditional<
    is_signed_int128<T>::value,
    make_unsigned_int128<T>,
    typename std::conditional<std::is_signed<T>::value,
                              std::make_unsigned<T>,
                              std::common_type<T>>::type>::type;
 
template <class T>
using is_integral_t = std::enable_if_t<is_integral<T>::value>;
 
template <class T>
using is_signed_int_t = std::enable_if_t<is_signed_int<T>::value>;
 
template <class T>
using is_unsigned_int_t = std::enable_if_t<is_unsigned_int<T>::value>;
 
template <class T> using to_unsigned_t = typename to_unsigned<T>::type;
 
}  // namespace internal
struct Scanner {
  public:
    Scanner(const Scanner&) = delete;
    Scanner& operator=(const Scanner&) = delete;
 
    Scanner(FILE* fp) : fd(fileno(fp)) {}
 
    void read() {}
    template <class H, class... T> void read(H& h, T&... t) {
        bool f = read_single(h);
        assert(f);
        read(t...);
    }
 
    int read_unsafe() { return 0; }
    template <class H, class... T> int read_unsafe(H& h, T&... t) {
        bool f = read_single(h);
        if (!f) return 0;
        return 1 + read_unsafe(t...);
    }
 
    int close() { return ::close(fd); }
 
  private:
    static constexpr int SIZE = 1 << 15;
 
    int fd = -1;
    std::array<char, SIZE + 1> line;
    int st = 0, ed = 0;
    bool eof = false;
 
    bool read_single(std::string& ref) {
        if (!skip_space()) return false;
        ref = "";
        while (true) {
            char c = top();
            if (c <= ' ') break;
            ref += c;
            st++;
        }
        return true;
    }
    bool read_single(double& ref) {
        std::string s;
        if (!read_single(s)) return false;
        ref = std::stod(s);
        return true;
    }
 
    template <class T,
              std::enable_if_t<std::is_same<T, char>::value>* = nullptr>
    bool read_single(T& ref) {
        if (!skip_space<50>()) return false;
        ref = top();
        st++;
        return true;
    }
 
    template <class T,
              internal::is_signed_int_t<T>* = nullptr,
              std::enable_if_t<!std::is_same<T, char>::value>* = nullptr>
    bool read_single(T& sref) {
        using U = internal::to_unsigned_t<T>;
        if (!skip_space<50>()) return false;
        bool neg = false;
        if (line[st] == '-') {
            neg = true;
            st++;
        }
        U ref = 0;
        do {
            ref = 10 * ref + (line[st++] & 0x0f);
        } while (line[st] >= '0');
        sref = neg ? -ref : ref;
        return true;
    }
    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<!std::is_same<U, char>::value>* = nullptr>
    bool read_single(U& ref) {
        if (!skip_space<50>()) return false;
        ref = 0;
        do {
            ref = 10 * ref + (line[st++] & 0x0f);
        } while (line[st] >= '0');
        return true;
    }
 
    bool reread() {
        if (ed - st >= 50) return true;
        if (st > SIZE / 2) {
            std::memmove(line.data(), line.data() + st, ed - st);
            ed -= st;
            st = 0;
        }
        if (eof) return false;
        auto u = ::read(fd, line.data() + ed, SIZE - ed);
        if (u == 0) {
            eof = true;
            line[ed] = '\0';
            u = 1;
        }
        ed += int(u);
        line[ed] = char(127);
        return true;
    }
 
    char top() {
        if (st == ed) {
            bool f = reread();
            assert(f);
        }
        return line[st];
    }
 
    template <int TOKEN_LEN = 0>
    bool skip_space() {
        while (true) {
            while (line[st] <= ' ') st++;   
            if (ed - st > TOKEN_LEN) return true;
            if (st > ed) st = ed;
            for (auto i = st; i < ed; i++) {
                if (line[i] <= ' ') return true;
            }
            if (!reread()) return false;
        }
    }
};

//fast Output by ei1333
/**
 * @brief Printer(高速出力)
 */
struct Printer {
public:
  explicit Printer(FILE *fp) : fp(fp) {}

  ~Printer() { flush(); }

  template< bool f = false, typename T, typename... E >
  void write(const T &t, const E &... e) {
    if(f) write_single(' ');
    write_single(t);
    write< true >(e...);
  }

  template< typename... T >
  void writeln(const T &...t) {
    write(t...);
    write_single('\n');
  }

  void flush() {
    fwrite(line, 1, st - line, fp);
    st = line;
  }

private:
  FILE *fp = nullptr;
  static constexpr size_t line_size = 1 << 16;
  static constexpr size_t int_digits = 20;
  char line[line_size + 1] = {};
  char small[32] = {};
  char *st = line;

  template< bool f = false >
  void write() {}

  void write_single(const char &t) {
    if(st + 1 >= line + line_size) flush();
    *st++ = t;
  }

  template< typename T, enable_if_t< is_integral< T >::value, int > = 0 >
  void write_single(T s) {
    if(st + int_digits >= line + line_size) flush();
    if(s == 0) {
      write_single('0');
      return;
    }
    if(s < 0) {
      write_single('-');
      s = -s;
    }
    char *mp = small + sizeof(small);
    typename make_unsigned< T >::type y = s;
    size_t len = 0;
    while(y > 0) {
      *--mp = y % 10 + '0';
      y /= 10;
      ++len;
    }
    memmove(st, mp, len);
    st += len;
  }

  void write_single(const string &s) {
    for(auto &c : s) write_single(c);
  }

  void write_single(const char *s) {
    while(*s != 0) write_single(*s++);
  }

  template< typename T >
  void write_single(const vector< T > &s) {
    for(size_t i = 0; i < s.size(); i++) {
      if(i) write_single(' ');
      write_single(s[i]);
    }
  }
};

}; //namespace fastio
struct dynamic_Fp{
  static int &mod(){
    static int mod_=0;
    return mod_;
  }
  ll val;
  static void set_mod(int m){ mod() = m; }
  static int getmod() { return mod(); }
  dynamic_Fp(long long v = 0) noexcept : val(v % getmod()) {
    if (val < 0) val += getmod();
  }
  dynamic_Fp operator - () const noexcept {
    return val ? getmod() - val : 0;
  }
  dynamic_Fp operator + (const dynamic_Fp& r) const noexcept { return dynamic_Fp(*this) += r; }
  dynamic_Fp operator - (const dynamic_Fp& r) const noexcept { return dynamic_Fp(*this) -= r; }
  dynamic_Fp operator * (const dynamic_Fp& r) const noexcept { return dynamic_Fp(*this) *= r; }
  dynamic_Fp operator / (const dynamic_Fp& r) const noexcept { return dynamic_Fp(*this) /= r; }
  dynamic_Fp& operator += (const dynamic_Fp& r) noexcept {
    val += r.val;
    if (val >= getmod()) val -= getmod();
    return *this;
  }
  dynamic_Fp& operator -= (const dynamic_Fp& r) noexcept {
    val -= r.val;
    if (val < 0) val += getmod();
    return *this;
  }
  dynamic_Fp& operator *= (const dynamic_Fp& r) noexcept {
    val = val * r.val % getmod();
    return *this;
  }
  dynamic_Fp& operator /= (const dynamic_Fp& r) noexcept {
    ll a = r.val, b = getmod(), u = 1, v = 0;
    while (b) {
      ll t = a / b;
      a -= t * b, swap(a, b);
      u -= t * v, swap(u, v);
    }
    val = val * u % getmod();
    if (val < 0) val += getmod();
    return *this;
  }
  bool operator == (const dynamic_Fp& r) const noexcept {
    return this->val == r.val;
  }
  bool operator != (const dynamic_Fp& r) const noexcept {
    return this->val != r.val;
  }
  bool operator < (const dynamic_Fp& r) const noexcept {
    return this->val < r.val;
  }
  friend istream& operator >> (istream& is, dynamic_Fp& x) noexcept {
    is >> x.val;
    x.val %= getmod();
    if (x.val < 0) x.val += getmod();
    return is;
  }
  friend ostream& operator << (ostream& os, const dynamic_Fp& x) noexcept {
    return os << x.val;
  }
  friend dynamic_Fp modpow(const dynamic_Fp& a, long long n) noexcept {
    dynamic_Fp res=1,r=a;
    while(n){
      if(n&1) res*=r;
      r*=r;
      n>>=1;
    }
    return res;
  }
  friend dynamic_Fp modinv(const dynamic_Fp& r) noexcept {
    long long a = r.val, b = getmod(), u = 1, v = 0;
    while (b) {
      long long t = a / b;
      a -= t * b, swap(a, b);
      u -= t * v, swap(u, v);
    }
    return dynamic_Fp(u);
  }
};
template< class T >
struct Matrix {
  vector< vector< T > > A;

  Matrix() {}

  Matrix(size_t n, size_t m) : A(n, vector< T >(m, 0)) {}

  Matrix(size_t n) : A(n, vector< T >(n, 0)) {};

  size_t size() const {
     if(A.empty()) return 0;
     assert(A.size() == A[0].size());
     return A.size();
  }

  size_t height() const {
    return (A.size());
  }

  size_t width() const {
    return (A[0].size());
  }

  inline const vector< T > &operator[](int k) const {
    return (A.at(k));
  }

  inline vector< T > &operator[](int k) {
    return (A.at(k));
  }

  static Matrix I(size_t n) {
    Matrix mat(n);
    for(int i = 0; i < n; i++) mat[i][i] = 1;
    return (mat);
  }

  Matrix &operator+=(const Matrix &B) {
    size_t n = height(), m = width();
    assert(n == B.height() && m == B.width());
    for(int i = 0; i < n; i++)
      for(int j = 0; j < m; j++)
        (*this)[i][j] += B[i][j];
    return (*this);
  }

  Matrix &operator-=(const Matrix &B) {
    size_t n = height(), m = width();
    assert(n == B.height() && m == B.width());
    for(int i = 0; i < n; i++)
      for(int j = 0; j < m; j++)
        (*this)[i][j] -= B[i][j];
    return (*this);
  }

  Matrix &operator*=(const Matrix &B) {
    size_t n = height(), m = B.width(), p = width();
    assert(p == B.height());
    vector< vector< T > > C(n, vector< T >(m, 0));
    for(int i = 0; i < n; i++)
      for(int j = 0; j < m; j++)
        for(int k = 0; k < p; k++)
          C[i][j] = (C[i][j] + (*this)[i][k] * B[k][j]);
    A.swap(C);
    return (*this);
  }

  friend Matrix pow(Matrix C,long long k) {
    Matrix B = Matrix::I(C.height());
    while(k > 0) {
      if(k & 1) B *= C;
      C *= C;
      k >>= 1LL;
    }
    return B;
  }

  Matrix operator+(const Matrix &B) const {
    return (Matrix(*this) += B);
  }

  Matrix operator-(const Matrix &B) const {
    return (Matrix(*this) -= B);
  }

  Matrix operator*(const Matrix &B) const {
    return (Matrix(*this) *= B);
  }

  Matrix operator^(const long long k) const {
    return (Matrix(*this) ^= k);
  }

  friend ostream &operator<<(ostream &os, Matrix &p) {
    size_t n = p.height(), m = p.width();
    for(int i = 0; i < n; i++) {
      os << "[";
      for(int j = 0; j < m; j++) {
        os << p[i][j] << (j + 1 == m ? "]\n" : ",");
      }
    }
    return (os);
  }


  T determinant() {
    Matrix B(*this);
    assert(width() == height());
    T ret = 1;
    for(int i = 0; i < width(); i++) {
      int idx = -1;
      for(int j = i; j < width(); j++) {
        if(B[j][i] != 0) idx = j;
      }
      if(idx == -1) return (0);
      if(i != idx) {
        ret *= -1;
        swap(B[i], B[idx]);
      }
      ret *= B[i][i];
      T vv = B[i][i];
      for(int j = 0; j < width(); j++) {
        B[i][j] /= vv;
      }
      for(int j = i + 1; j < width(); j++) {
        T a = B[j][i];
        for(int k = 0; k < width(); k++) {
          B[j][k] -= B[i][k] * a;
        }
      }
    }
    return (ret);
  }
};
template <typename T>
int matrix_rank(const int n, const int m, vector<vector<T>> a) {
  int rk = 0;
  FOR(j, m) {
    if (rk == n) break;
    if (a[rk][j] == 0) {
      for(int i=rk + 1;i<n;i++) if (a[i][j] != T(0)) {
        swap(a[rk], a[i]);
        break;
      }
    }
    if (a[rk][j] == 0) continue;
    T c = T(1) / a[rk][j];
    for(int k=j;k<m;k++) a[rk][k] *= c;
    for(int i=rk + 1;i<n;i++) {
      T c = a[i][j];
      for(int k=j;k<m;k++) { a[i][k] -= a[rk][k] * c; }
    }
    ++rk;
  }
  return rk;
}
template <typename mint>
vector<mint> middle_product_naive(vector<mint>& a, vector<mint>& b) {
  vector<mint> res(a.size() - b.size() + 1);
  for(int i=0;i<res.size();i++) for(int j=0;j<b.size();j++) res[i] += b[j] * a[i + j];
  return res;
}
template <typename mint>
vector<mint> middle_product(vector<mint>& a, vector<mint>& b) { //c[i] = sum a[i+j] b[j]
  assert(a.size() >= b.size());
  if (b.empty()) return vector<mint>(a.size() - b.size() + 1);
  if (min(b.size(), a.size() - b.size() + 1) <= 60) {
    return middle_product_naive(a, b);
  }
  int n = 1 << __lg(2 * a.size() - 1);
  vector<mint> fa(n), fb(n);
  copy(a.begin(), a.end(), fa.begin());
  copy(b.rbegin(), b.rend(), fb.begin());
  NTT::ntt998.ntt(fa);
  NTT::ntt998.ntt(fb);
  for(int i=0;i<n;i++) fa[i] *= fb[i];
  NTT::ntt998.intt(fa);
  fa.resize(a.size());
  fa.erase(fa.begin(), fa.begin() + b.size() - 1);
  return fa;
}
namespace NTT{
template <typename mint>
struct TransposedNumberTheoreticTransform {
  static constexpr uint32_t mod = mint::getmod();
  static constexpr uint32_t pr = ntt_inner::get_pr(mint::getmod());
  static constexpr int level = __builtin_ctzll(mod - 1);
  mint dw[level], dy[level];
  mint *buf1, *buf2;
 
  constexpr TransposedNumberTheoreticTransform() {
    setwy(level);
    union raw_cast {
      mint dat;
      uint32_t _;
    };
    buf1 = &(((raw_cast *)(ntt_inner::_buf1))->dat);
    buf2 = &(((raw_cast *)(ntt_inner::_buf2))->dat);
  }
 
  constexpr void setwy(int k) {
    mint w[level], y[level];
    w[k - 1] = modpow(mint(pr),(mod - 1) / (1 << k));
    y[k - 1] = modinv(w[k - 1]);
    for (int i = k - 2; i > 0; --i)
      w[i] = w[i + 1] * w[i + 1], y[i] = y[i + 1] * y[i + 1];
    dw[0] = dy[0] = w[1] * w[1];
    dw[1] = w[1], dy[1] = y[1], dw[2] = w[2], dy[2] = y[2];
    for (int i = 3; i < k; ++i) {
      dw[i] = dw[i - 1] * y[i - 2] * w[i];
      dy[i] = dy[i - 1] * w[i - 2] * y[i];
    }
  }
 
  __attribute__((target("avx2"))) void ntt(mint *a, int n) {
    int k = n ? __builtin_ctz(n) : 0;
    if (k == 0) return;
    if (k == 1) {
      mint a1 = a[1];
      a[1] = a[0] - a[1];
      a[0] = a[0] + a1;
      return;
    }
    if (k & 1) {
      int v = 1 << (k - 1);
      if (v < 8) {
        for (int j = 0; j < v; ++j) {
          mint ajv = a[j + v];
          a[j + v] = a[j] - ajv;
          a[j] += ajv;
        }
      } else {
        const __m256i m0 = _mm256_set1_epi32(0);
        const __m256i m2 = _mm256_set1_epi32(mod + mod);
        int j0 = 0;
        int j1 = v;
        for (; j0 < v; j0 += 8, j1 += 8) {
          __m256i T0 = _mm256_loadu_si256((__m256i *)(a + j0));
          __m256i T1 = _mm256_loadu_si256((__m256i *)(a + j1));
          __m256i naj = montgomery_add_256(T0, T1, m2, m0);
          __m256i najv = montgomery_sub_256(T0, T1, m2, m0);
          _mm256_storeu_si256((__m256i *)(a + j0), naj);
          _mm256_storeu_si256((__m256i *)(a + j1), najv);
        }
      }
    }
    int u = 1 << (2 + (k & 1));
    int v = 1 << (k - 2 - (k & 1));
    mint one = mint(1);
    mint imag = dw[1];
    while (v) {
      if (v == 1) {
        mint ww = one, xx = one, wx = one;
        for (int jh = 0; jh < u;) {
          ww = xx * xx, wx = ww * xx;
          mint t0 = a[jh + 0], t1 = a[jh + 1] * xx;
          mint t2 = a[jh + 2] * ww, t3 = a[jh + 3] * wx;
          mint t0p2 = t0 + t2, t1p3 = t1 + t3;
          mint t0m2 = t0 - t2, t1m3 = (t1 - t3) * imag;
          a[jh + 0] = t0p2 + t1p3, a[jh + 1] = t0p2 - t1p3;
          a[jh + 2] = t0m2 + t1m3, a[jh + 3] = t0m2 - t1m3;
          xx *= dw[__builtin_ctz((jh += 4))];
        }
      } else if (v == 4) {
        const __m128i m0 = _mm_set1_epi32(0);
        const __m128i m1 = _mm_set1_epi32(mod);
        const __m128i m2 = _mm_set1_epi32(mod + mod);
        const __m128i r = _mm_set1_epi32(mint::r);
        const __m128i Imag = _mm_set1_epi32(imag.a);
        mint ww = one, xx = one, wx = one;
        for (int jh = 0; jh < u;) {
          if (jh == 0) {
            int j0 = 0;
            int j1 = v;
            int j2 = j1 + v;
            int j3 = j2 + v;
            int je = v;
            for (; j0 < je; j0 += 4, j1 += 4, j2 += 4, j3 += 4) {
              const __m128i T0 = _mm_loadu_si128((__m128i *)(a + j0));
              const __m128i T1 = _mm_loadu_si128((__m128i *)(a + j1));
              const __m128i T2 = _mm_loadu_si128((__m128i *)(a + j2));
              const __m128i T3 = _mm_loadu_si128((__m128i *)(a + j3));
              const __m128i T0P2 = montgomery_add_128(T0, T2, m2, m0);
              const __m128i T1P3 = montgomery_add_128(T1, T3, m2, m0);
              const __m128i T0M2 = montgomery_sub_128(T0, T2, m2, m0);
              const __m128i T1M3 = montgomery_mul_128(
                  montgomery_sub_128(T1, T3, m2, m0), Imag, r, m1);
              _mm_storeu_si128((__m128i *)(a + j0),
                               montgomery_add_128(T0P2, T1P3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j1),
                               montgomery_sub_128(T0P2, T1P3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j2),
                               montgomery_add_128(T0M2, T1M3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j3),
                               montgomery_sub_128(T0M2, T1M3, m2, m0));
            }
          } else {
            ww = xx * xx, wx = ww * xx;
            const __m128i WW = _mm_set1_epi32(ww.a);
            const __m128i WX = _mm_set1_epi32(wx.a);
            const __m128i XX = _mm_set1_epi32(xx.a);
            int j0 = jh * v;
            int j1 = j0 + v;
            int j2 = j1 + v;
            int j3 = j2 + v;
            int je = j1;
            for (; j0 < je; j0 += 4, j1 += 4, j2 += 4, j3 += 4) {
              const __m128i T0 = _mm_loadu_si128((__m128i *)(a + j0));
              const __m128i T1 = _mm_loadu_si128((__m128i *)(a + j1));
              const __m128i T2 = _mm_loadu_si128((__m128i *)(a + j2));
              const __m128i T3 = _mm_loadu_si128((__m128i *)(a + j3));
              const __m128i MT1 = montgomery_mul_128(T1, XX, r, m1);
              const __m128i MT2 = montgomery_mul_128(T2, WW, r, m1);
              const __m128i MT3 = montgomery_mul_128(T3, WX, r, m1);
              const __m128i T0P2 = montgomery_add_128(T0, MT2, m2, m0);
              const __m128i T1P3 = montgomery_add_128(MT1, MT3, m2, m0);
              const __m128i T0M2 = montgomery_sub_128(T0, MT2, m2, m0);
              const __m128i T1M3 = montgomery_mul_128(
                  montgomery_sub_128(MT1, MT3, m2, m0), Imag, r, m1);
              _mm_storeu_si128((__m128i *)(a + j0),
                               montgomery_add_128(T0P2, T1P3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j1),
                               montgomery_sub_128(T0P2, T1P3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j2),
                               montgomery_add_128(T0M2, T1M3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j3),
                               montgomery_sub_128(T0M2, T1M3, m2, m0));
            }
          }
          xx *= dw[__builtin_ctz((jh += 4))];
        }
      } else {
        const __m256i m0 = _mm256_set1_epi32(0);
        const __m256i m1 = _mm256_set1_epi32(mod);
        const __m256i m2 = _mm256_set1_epi32(mod + mod);
        const __m256i r = _mm256_set1_epi32(mint::r);
        const __m256i Imag = _mm256_set1_epi32(imag.a);
        mint ww = one, xx = one, wx = one;
        for (int jh = 0; jh < u;) {
          if (jh == 0) {
            int j0 = 0;
            int j1 = v;
            int j2 = j1 + v;
            int j3 = j2 + v;
            int je = v;
            for (; j0 < je; j0 += 8, j1 += 8, j2 += 8, j3 += 8) {
              const __m256i T0 = _mm256_loadu_si256((__m256i *)(a + j0));
              const __m256i T1 = _mm256_loadu_si256((__m256i *)(a + j1));
              const __m256i T2 = _mm256_loadu_si256((__m256i *)(a + j2));
              const __m256i T3 = _mm256_loadu_si256((__m256i *)(a + j3));
              const __m256i T0P2 = montgomery_add_256(T0, T2, m2, m0);
              const __m256i T1P3 = montgomery_add_256(T1, T3, m2, m0);
              const __m256i T0M2 = montgomery_sub_256(T0, T2, m2, m0);
              const __m256i T1M3 = montgomery_mul_256(
                  montgomery_sub_256(T1, T3, m2, m0), Imag, r, m1);
              _mm256_storeu_si256((__m256i *)(a + j0),
                                  montgomery_add_256(T0P2, T1P3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j1),
                                  montgomery_sub_256(T0P2, T1P3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j2),
                                  montgomery_add_256(T0M2, T1M3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j3),
                                  montgomery_sub_256(T0M2, T1M3, m2, m0));
            }
          } else {
            ww = xx * xx, wx = ww * xx;
            const __m256i WW = _mm256_set1_epi32(ww.a);
            const __m256i WX = _mm256_set1_epi32(wx.a);
            const __m256i XX = _mm256_set1_epi32(xx.a);
            int j0 = jh * v;
            int j1 = j0 + v;
            int j2 = j1 + v;
            int j3 = j2 + v;
            int je = j1;
            for (; j0 < je; j0 += 8, j1 += 8, j2 += 8, j3 += 8) {
              const __m256i T0 = _mm256_loadu_si256((__m256i *)(a + j0));
              const __m256i T1 = _mm256_loadu_si256((__m256i *)(a + j1));
              const __m256i T2 = _mm256_loadu_si256((__m256i *)(a + j2));
              const __m256i T3 = _mm256_loadu_si256((__m256i *)(a + j3));
              const __m256i MT1 = montgomery_mul_256(T1, XX, r, m1);
              const __m256i MT2 = montgomery_mul_256(T2, WW, r, m1);
              const __m256i MT3 = montgomery_mul_256(T3, WX, r, m1);
              const __m256i T0P2 = montgomery_add_256(T0, MT2, m2, m0);
              const __m256i T1P3 = montgomery_add_256(MT1, MT3, m2, m0);
              const __m256i T0M2 = montgomery_sub_256(T0, MT2, m2, m0);
              const __m256i T1M3 = montgomery_mul_256(
                  montgomery_sub_256(MT1, MT3, m2, m0), Imag, r, m1);
              _mm256_storeu_si256((__m256i *)(a + j0),
                                  montgomery_add_256(T0P2, T1P3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j1),
                                  montgomery_sub_256(T0P2, T1P3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j2),
                                  montgomery_add_256(T0M2, T1M3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j3),
                                  montgomery_sub_256(T0M2, T1M3, m2, m0));
            }
          }
          xx *= dw[__builtin_ctz((jh += 4))];
        }
      }
      u <<= 2;
      v >>= 2;
    }
  }
 
  __attribute__((target("avx2"))) void intt(mint *a, int n) {
    mint invn = modinv(mint(n));
    for (int i = 0; i < n; i++) a[i] *= invn;
    int k = n ? __builtin_ctz(n) : 0;
    if (k == 0) return;
    if (k == 1) {
      mint a1 = a[1];
      a[1] = a[0] - a[1];
      a[0] = a[0] + a1;
      return;
    }
    int u = 1 << (k - 2);
    int v = 1;
    mint one = mint(1);
    mint imag = dy[1];
    while (u) {
      if (v == 1) {
        mint ww = one, xx = one, yy = one;
        u <<= 2;
        for (int jh = 0; jh < u;) {
          ww = xx * xx, yy = xx * imag;
          mint t0 = a[jh + 0], t1 = a[jh + 1];
          mint t2 = a[jh + 2], t3 = a[jh + 3];
          mint t0p1 = t0 + t1, t2p3 = t2 + t3;
          mint t0m1 = (t0 - t1) * xx, t2m3 = (t2 - t3) * yy;
          a[jh + 0] = t0p1 + t2p3, a[jh + 2] = (t0p1 - t2p3) * ww;
          a[jh + 1] = t0m1 + t2m3, a[jh + 3] = (t0m1 - t2m3) * ww;
          xx *= dy[__builtin_ctz(jh += 4)];
        }
      } else if (v == 4) {
        const __m128i m0 = _mm_set1_epi32(0);
        const __m128i m1 = _mm_set1_epi32(mod);
        const __m128i m2 = _mm_set1_epi32(mod + mod);
        const __m128i r = _mm_set1_epi32(mint::r);
        const __m128i Imag = _mm_set1_epi32(imag.a);
        mint ww = one, xx = one, yy = one;
        u <<= 2;
        for (int jh = 0; jh < u;) {
          if (jh == 0) {
            int j0 = 0;
            int j1 = v;
            int j2 = v + v;
            int j3 = j2 + v;
            for (; j0 < v; j0 += 4, j1 += 4, j2 += 4, j3 += 4) {
              const __m128i T0 = _mm_loadu_si128((__m128i *)(a + j0));
              const __m128i T1 = _mm_loadu_si128((__m128i *)(a + j1));
              const __m128i T2 = _mm_loadu_si128((__m128i *)(a + j2));
              const __m128i T3 = _mm_loadu_si128((__m128i *)(a + j3));
              const __m128i T0P1 = montgomery_add_128(T0, T1, m2, m0);
              const __m128i T2P3 = montgomery_add_128(T2, T3, m2, m0);
              const __m128i T0M1 = montgomery_sub_128(T0, T1, m2, m0);
              const __m128i T2M3 = montgomery_mul_128(
                  montgomery_sub_128(T2, T3, m2, m0), Imag, r, m1);
              _mm_storeu_si128((__m128i *)(a + j0),
                               montgomery_add_128(T0P1, T2P3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j2),
                               montgomery_sub_128(T0P1, T2P3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j1),
                               montgomery_add_128(T0M1, T2M3, m2, m0));
              _mm_storeu_si128((__m128i *)(a + j3),
                               montgomery_sub_128(T0M1, T2M3, m2, m0));
            }
          } else {
            ww = xx * xx, yy = xx * imag;
            const __m128i WW = _mm_set1_epi32(ww.a);
            const __m128i XX = _mm_set1_epi32(xx.a);
            const __m128i YY = _mm_set1_epi32(yy.a);
            int j0 = jh * v;
            int j1 = j0 + v;
            int j2 = j1 + v;
            int j3 = j2 + v;
            int je = j1;
            for (; j0 < je; j0 += 4, j1 += 4, j2 += 4, j3 += 4) {
              const __m128i T0 = _mm_loadu_si128((__m128i *)(a + j0));
              const __m128i T1 = _mm_loadu_si128((__m128i *)(a + j1));
              const __m128i T2 = _mm_loadu_si128((__m128i *)(a + j2));
              const __m128i T3 = _mm_loadu_si128((__m128i *)(a + j3));
              const __m128i T0P1 = montgomery_add_128(T0, T1, m2, m0);
              const __m128i T2P3 = montgomery_add_128(T2, T3, m2, m0);
              const __m128i T0M1 = montgomery_mul_128(
                  montgomery_sub_128(T0, T1, m2, m0), XX, r, m1);
              __m128i T2M3 = montgomery_mul_128(
                  montgomery_sub_128(T2, T3, m2, m0), YY, r, m1);
              _mm_storeu_si128((__m128i *)(a + j0),
                               montgomery_add_128(T0P1, T2P3, m2, m0));
              _mm_storeu_si128(
                  (__m128i *)(a + j2),
                  montgomery_mul_128(montgomery_sub_128(T0P1, T2P3, m2, m0), WW,
                                     r, m1));
              _mm_storeu_si128((__m128i *)(a + j1),
                               montgomery_add_128(T0M1, T2M3, m2, m0));
              _mm_storeu_si128(
                  (__m128i *)(a + j3),
                  montgomery_mul_128(montgomery_sub_128(T0M1, T2M3, m2, m0), WW,
                                     r, m1));
            }
          }
          xx *= dy[__builtin_ctz(jh += 4)];
        }
      } else {
        const __m256i m0 = _mm256_set1_epi32(0);
        const __m256i m1 = _mm256_set1_epi32(mod);
        const __m256i m2 = _mm256_set1_epi32(mod + mod);
        const __m256i r = _mm256_set1_epi32(mint::r);
        const __m256i Imag = _mm256_set1_epi32(imag.a);
        mint ww = one, xx = one, yy = one;
        u <<= 2;
        for (int jh = 0; jh < u;) {
          if (jh == 0) {
            int j0 = 0;
            int j1 = v;
            int j2 = v + v;
            int j3 = j2 + v;
            for (; j0 < v; j0 += 8, j1 += 8, j2 += 8, j3 += 8) {
              const __m256i T0 = _mm256_loadu_si256((__m256i *)(a + j0));
              const __m256i T1 = _mm256_loadu_si256((__m256i *)(a + j1));
              const __m256i T2 = _mm256_loadu_si256((__m256i *)(a + j2));
              const __m256i T3 = _mm256_loadu_si256((__m256i *)(a + j3));
              const __m256i T0P1 = montgomery_add_256(T0, T1, m2, m0);
              const __m256i T2P3 = montgomery_add_256(T2, T3, m2, m0);
              const __m256i T0M1 = montgomery_sub_256(T0, T1, m2, m0);
              const __m256i T2M3 = montgomery_mul_256(
                  montgomery_sub_256(T2, T3, m2, m0), Imag, r, m1);
              _mm256_storeu_si256((__m256i *)(a + j0),
                                  montgomery_add_256(T0P1, T2P3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j2),
                                  montgomery_sub_256(T0P1, T2P3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j1),
                                  montgomery_add_256(T0M1, T2M3, m2, m0));
              _mm256_storeu_si256((__m256i *)(a + j3),
                                  montgomery_sub_256(T0M1, T2M3, m2, m0));
            }
          } else {
            ww = xx * xx, yy = xx * imag;
            const __m256i WW = _mm256_set1_epi32(ww.a);
            const __m256i XX = _mm256_set1_epi32(xx.a);
            const __m256i YY = _mm256_set1_epi32(yy.a);
            int j0 = jh * v;
            int j1 = j0 + v;
            int j2 = j1 + v;
            int j3 = j2 + v;
            int je = j1;
            for (; j0 < je; j0 += 8, j1 += 8, j2 += 8, j3 += 8) {
              const __m256i T0 = _mm256_loadu_si256((__m256i *)(a + j0));
              const __m256i T1 = _mm256_loadu_si256((__m256i *)(a + j1));
              const __m256i T2 = _mm256_loadu_si256((__m256i *)(a + j2));
              const __m256i T3 = _mm256_loadu_si256((__m256i *)(a + j3));
              const __m256i T0P1 = montgomery_add_256(T0, T1, m2, m0);
              const __m256i T2P3 = montgomery_add_256(T2, T3, m2, m0);
              const __m256i T0M1 = montgomery_mul_256(
                  montgomery_sub_256(T0, T1, m2, m0), XX, r, m1);
              const __m256i T2M3 = montgomery_mul_256(
                  montgomery_sub_256(T2, T3, m2, m0), YY, r, m1);
              _mm256_storeu_si256((__m256i *)(a + j0),
                                  montgomery_add_256(T0P1, T2P3, m2, m0));
              _mm256_storeu_si256(
                  (__m256i *)(a + j2),
                  montgomery_mul_256(montgomery_sub_256(T0P1, T2P3, m2, m0), WW,
                                     r, m1));
              _mm256_storeu_si256((__m256i *)(a + j1),
                                  montgomery_add_256(T0M1, T2M3, m2, m0));
              _mm256_storeu_si256(
                  (__m256i *)(a + j3),
                  montgomery_mul_256(montgomery_sub_256(T0M1, T2M3, m2, m0), WW,
                                     r, m1));
            }
          }
          xx *= dy[__builtin_ctz(jh += 4)];
        }
      }
      u >>= 4;
      v <<= 2;
    }
    if (k & 1) {
      v = 1 << (k - 1);
      if (v < 8) {
        for (int j = 0; j < v; ++j) {
          mint ajv = a[j] - a[j + v];
          a[j] += a[j + v];
          a[j + v] = ajv;
        }
      } else {
        const __m256i m0 = _mm256_set1_epi32(0);
        const __m256i m2 = _mm256_set1_epi32(mod + mod);
        int j0 = 0;
        int j1 = v;
        for (; j0 < v; j0 += 8, j1 += 8) {
          const __m256i T0 = _mm256_loadu_si256((__m256i *)(a + j0));
          const __m256i T1 = _mm256_loadu_si256((__m256i *)(a + j1));
          __m256i naj = montgomery_add_256(T0, T1, m2, m0);
          __m256i najv = montgomery_sub_256(T0, T1, m2, m0);
          _mm256_storeu_si256((__m256i *)(a + j0), naj);
          _mm256_storeu_si256((__m256i *)(a + j1), najv);
        }
      }
    }
  }
  void ntt(vector<mint> &a) {
    int M = (int)a.size();
    for (int i = 0; i < M; i++) buf1[i].a = a[i].a;
    ntt(buf1, M);
    for (int i = 0; i < M; i++) a[i].a = buf1[i].a;
  }
  void intt(vector<mint> &a) {
    int M = (int)a.size();
    for (int i = 0; i < M; i++) buf1[i].a = a[i].a;
    intt(buf1, M);
    for (int i = 0; i < M; i++) a[i].a = buf1[i].a;
  }
  void transposed_ntt(vector<mint> &a) {
    int M = (int)a.size();
    for (int i = 0; i < M; i++) a[i] *= M;
    for (int i = 0; i < M; i++) buf1[i].a = a[i].a;
    intt(buf1, M);
    for (int i = 0; i < M; i++) a[i].a = buf1[i].a;
    reverse(a.begin()+1,a.end());
  }
  void transposed_intt(vector<mint> &a) {
    int M = (int)a.size();
    mint invM = modinv(mint(M));
    for (int i = 0; i < M; i++) a[i] *= invM;
    reverse(a.begin()+1,a.end());
    for (int i = 0; i < M; i++) buf1[i].a = a[i].a;
    ntt(buf1, M);
    for (int i = 0; i < M; i++) a[i].a = buf1[i].a;
  }
};
TransposedNumberTheoreticTransform<MontgomeryModInt<998244353>> tntt998;
/*template <typename mint>
struct TransposedNTT{
    NumberTheoreticTransform<mint> transNTT;
    void transposed_ntt(vector<mint> &a){
        int n=a.size();
        //transNTT.intt(a);
        trans_inv(a);
        reverse(a.begin()+1,a.end());
        for(auto& x:a) x*=n;
    }
    void transposed_intt(vector<mint> &a){
        int n=a.size();
        reverse(a.begin()+1,a.end());
        //transNTT.ntt(a);
        trans(a);
        mint invlen=modinv(mint(n));
        for(auto& x:a) x*=invlen;
    }
};
TransposedNTT<MontgomeryModInt<998244353>> tntt998;*/
int popcnt(int x) { return __builtin_popcount(x); }
int popcnt(u32 x) { return __builtin_popcount(x); }
int popcnt(ll x) { return __builtin_popcountll(x); }
int popcnt(u64 x) { return __builtin_popcountll(x); }
int popcnt_mod_2(int x) { return __builtin_parity(x); }
int popcnt_mod_2(u32 x) { return __builtin_parity(x); }
int popcnt_mod_2(ll x) { return __builtin_parityll(x); }
int popcnt_mod_2(u64 x) { return __builtin_parityll(x); }
// (0, 1, 2, 3, 4) -> (-1, 0, 1, 1, 2)
int topbit(int x) { return (x == 0 ? -1 : 31 - __builtin_clz(x)); }
int topbit(u32 x) { return (x == 0 ? -1 : 31 - __builtin_clz(x)); }
int topbit(ll x) { return (x == 0 ? -1 : 63 - __builtin_clzll(x)); }
int topbit(u64 x) { return (x == 0 ? -1 : 63 - __builtin_clzll(x)); }
// (0, 1, 2, 3, 4) -> (-1, 0, 1, 0, 2)
int lowbit(int x) { return (x == 0 ? -1 : __builtin_ctz(x)); }
int lowbit(u32 x) { return (x == 0 ? -1 : __builtin_ctz(x)); }
int lowbit(ll x) { return (x == 0 ? -1 : __builtin_ctzll(x)); }
int lowbit(u64 x) { return (x == 0 ? -1 : __builtin_ctzll(x)); }
fft_info<MontgomeryModInt<998244353>> info998;
constexpr auto bcl(int x){
    return (x<2)?1:2<<std::__lg(x-1);
}
template <typename mint>
struct DODFT{
fft_info<mint> INFO;
int fft_len=0;
mint _g=INFO.g;
std::vector<mint> _w{1},iw{1},_w22{1},iw22{1};
auto init_w(int lm){
    _w.resize(lm),iw.resize(lm),_w22.resize(lm),iw22.resize(lm);
    for(auto i=1;i<lm;i<<=1){
        _w[i]=modpow(_g,((mint::getmod()-1)>>2)/i);
        iw[i]=modpow(_g,mint::getmod()-1-((mint::getmod()-1)>>2)/i);
    }
    for(auto i=1;i<lm;++i){
        _w[i]=_w[i&(i-1)]*_w[i&-i];
        iw[i]=iw[i&(i-1)]*iw[i&-i];
    }
    for(auto i=1,i2=2;i<lm;i=i2,i2<<=1){
		mint _G=modpow(_g,(mint::getmod()-1)/i2),_r=mint::getmod()-(mint::getmod()-1)/i;
        mint iG=modpow(_G,mint::getmod()-2),ir=mint::getmod()-(mint::getmod()-1)/i;
		for(auto j=i;j<i2;++j){
		    _w22[j]=_r,_r=_r*_G;
            iw22[j]=ir,ir=ir*iG;
		}
	}
}
inline auto chk_w(int lm){
    if((lm>>=1)>int(_w.size())){
        init_w(lm);
    }
}
inline auto rot_R(mint*f,int L,mint r){
    for(auto i=0;i<L;++i){
        auto x=f[i],y=f[i+L]*r;
        f[i]=x+y,f[i+L]=x-y;
    }
}
inline auto rot_L(mint*f,int L,mint r){
    for(auto i=0;i<L;++i){
        auto x=f[i],y=f[i+L];
        f[i]=x+y,f[i+L]=(x-y)*r;
    }
}
inline auto rrot_R(mint*f,int L,int lm){
    for(auto j=0,k=0;j<lm;j+=L*2,++k){
        rot_R(f+j,L,_w[k]);
    }
}
inline auto rrot_L(mint*f,int L,int lm){
    for(auto j=0,k=0;j<lm;j+=L*2,++k){
        rot_L(f+j,L,iw[k]);
    }
}
auto dif(mint*f,int lm){
    fft_len+=lm;
    chk_w(lm);
    for(auto L=lm>>1;L;L>>=1){
        rrot_R(f,L,lm);
    }
}
auto fft_2D(mint*f,int n,int m){
    auto lm=n*m;
    fft_len+=lm;
    chk_w(lm);
    for(auto j=0;j<lm;j+=m){
        for(auto L=m>>1;L;L>>=1){
            rrot_R(f+j,L,m);
        }
    }
    for(auto L=lm>>1;L>=m;L>>=1){
        rrot_R(f,L,lm);
    }
}
auto dit(mint*f,int lm){
    fft_len+=lm;
    for(auto L=1;L<lm;L<<=1){
        rrot_L(f,L,lm);
    }
}
template<bool fx=true>auto ifft_2D(mint*f,int n,int m){
    auto lm=n*m;
    fft_len+=lm;
    for(auto j=0;j<lm;j+=m){
        for(auto L=1;L<m;L<<=1){
            rrot_L(f+j,L,m);
        }
    }
    for(auto L=m;L<lm;L<<=1){
        rrot_L(f,L,lm);
    }
    if constexpr(fx){
        const mint iv=mint::getmod()-(mint::getmod()-1)/lm;
        for(auto i=0;i<lm;++i){
            f[i]=f[i]*iv;
        }
    }
}
void __PowerYX(mint*P,mint*tQ,int n,int m,mint&OneP,mint&OneQ){
    if(m==1){
        dif(P,n);
        for(int i=n-1;i>=0;--i){
            mint x=P[i];
            P[i*2]=x,P[i*2+1]=x;
        }
        return;
    }
    mint*Q=new mint[4*n*m];
    if(n==1){
        for(int i=0;i<m;++i){Q[i]=-tQ[i];}
        std::fill(Q+m,Q+m*2,0),dif(Q,m*2);
        std::fill(Q+m*2,Q+m*4,1),rot_R(Q,m*2,1);
    }
    else{
        fft_len += 4*n*m;
        for(int i=0;i<2*n*m;++i){
            Q[i]=tQ[i*2]*tQ[i*2+1];
        }
        OneQ=OneQ*OneQ;
        for(auto i=0;i<n;++i){
            auto dft=Q+i*m*2;
            for(auto L=1;L<=m;L<<=1){
                rrot_L(dft,L,m*2);
            }
            std::fill_n(dft+m,m,0);
            for(auto L=m;L;L>>=1){
                rrot_R(dft,L,m*2);
            }
        }
        auto g=Q+2*n*m;
        std::copy(Q,g,g);
        for(auto L=m*2;L<=n*m;L<<=1){
            rrot_L(g,L,2*n*m);
        }
        for(int j=0,k=0,diff=n;j<n*m*2;j+=m*2,++k){
            for(int i=0;i<m*2;++i){
                g[j+i]=g[j+i]*_w22[diff+k];
            }
        }
        OneQ=OneQ*m*2;
        mint Two=OneQ+OneQ;
        for(int i=0;i<m*2;++i){
            g[i]=g[i]-Two;
        }
        for(auto L=n*m;L>m;L>>=1){
            rrot_R(g,L,2*n*m);
        }
    }
    const mint oo=OneQ;
    __PowerYX(P,Q,n*2,m/2,OneP,OneQ);
    for(int i=0;i<2*n*m;++i){
        auto x=Q[i*2],y=Q[i*2+1];
        Q[i*2]=P[i]*y;
        Q[i*2+1]=P[i]*x;
    }
    OneP=OneP*oo;
    if(n==1){
        ifft_2D<false>(Q,2*n,2*m);
        mint fx=modinv(OneP*n*m*4);
        for(int i=0;i<n;++i){
            for(int j=0;j<m;++j){
                P[i*m+j]=Q[(i+n)*(2*m)+j]*fx;
            }
        }
    }
    else{
        fft_len += 4*n*m;
        auto g=Q+2*n*m;
        for(auto L=2*m;L<=n*m;L<<=1){
            rrot_L(g,L,2*n*m);
        }
        for(int j=0,k=0,diff=n;j<n*m*2;j+=m*2,++k){
            for(int i=0;i<m*2;++i){
                g[j+i]=g[j+i]*iw22[diff+k];
            }
        }
        for(auto L=n*m;L>m;L>>=1){
            rrot_R(g,L,2*n*m);
        }
        OneP=OneP*2;
        for(auto i=0;i<2*n*m;++i){
            P[i]=Q[i]-g[i];
        }
        for(auto i=0;i<n;++i){
            auto dft=P+i*m*2;
            for(auto L=1;L<=m;L<<=1){
                rrot_L(dft,L,m*2);
            }
            std::fill_n(dft+m,m,0);
            for(auto L=m;L;L>>=1){
                rrot_R(dft,L,m*2);
            }
        }
        OneP=OneP*m*2;
    }
    delete []Q;
}
};
DODFT<MontgomeryModInt<998244353>> ddft998;
}
template <typename mint>
FPS<mint> composition(FPS<mint> f,FPS<mint> g){
    int n=f.size();
    auto lm=NTT::bcl(std::max<int>(f.size(),n));
    f.resize(lm),g.resize(lm);

    if(g[0]!=mint(0)){
        f=f.taylor_shift(g[0]);
        g[0]=0;
    }//taylor shift.

    if(lm!=1){
        f.resize(lm*2);
        mint op=1,oq=1;
        NTT::ddft998.__PowerYX(f.data(),g.data(),1,lm,op,oq);
    }

    f.resize(n);
    return f;
}
template <typename mint> struct online_multiply{
    using poly=FPS<mint>;
    int n=0;
    poly ans={0,0};
    poly x={},y={};
    mint operator()(mint a,mint b){
        x.emplace_back(a);
        y.emplace_back(b);
        ++n;
        if(ans.size()<=2*n)ans.resize(n*4);
        int m=(n+1)&(-n-1);
        int s=0,val=1;
        if(m==n+1)m/=2;
        while(val<=m){
            auto add=poly(x.begin()+s,x.begin()+s+val)*poly(y.end()-val,y.end());
            if(s+val!=n)add+=poly(y.begin()+s,y.begin()+s+val)*poly(x.end()-val,x.end());
            s+=val,val*=2;
            for(int i=0;i<(int)add.size();++i){
                assert(n-1+i<n*2);
                ans[n-1+i]+=add[i];
            }
        }
        return ans[n-1];
    }
};
template <typename mint> struct online_exp{
    mint pre;
    int n=0;
    online_multiply<mint> Mul;
    mint operator()(mint a){
        ++n;
        if(n==1){
            assert(a==0);
            return pre=1;
        }else{
            return pre=Mul(pre,a*(n-1))/mint(n-1);
        }
    }
};

template <typename mint> struct online_inv{
    mint pref=0,preg=0,fstf,fstg;
    int n=0;
    online_multiply<mint> Mul;
    // (f/x)*(g/x)=0
    mint operator()(mint a){
        ++n;
        if(n==1){
            assert(a!=0);
            pref=fstf=a;
            preg=fstg=mint(1)/a;
        }else{
            preg=-((n==2?mint(0):Mul(pref,preg))+fstg*a)*fstg;
            pref=a;
        }
        return preg;
    }
};

template <typename mint> struct online_log{
    mint pref=0,preg=0;
    int n=0;
    online_inv<mint> Inv;
    online_multiply<mint> Mul;
    mint operator()(mint a){
        ++n;
        if(n==1){
            assert(a==1);
            pref=a;
            preg=0;
        }else{
            preg=Mul(a*(n-1),Inv(pref))/mint(n-1);
            pref=a;
        }
        return preg;
    }
};
using mint=MontgomeryModInt<998244353>;
int main(){
    fastio::Scanner sc(stdin);
    fastio::Printer pr(stdout);
    #define in(...) sc.read(__VA_ARGS__)
    #define LL(...) ll __VA_ARGS__;in(__VA_ARGS__)
    #define INT(...) int __VA_ARGS__;in(__VA_ARGS__)
    #define STR(...) string __VA_ARGS__;in(__VA_ARGS__)
    #define out(...) pr.write(__VA_ARGS__)
    #define outln(...) pr.writeln(__VA_ARGS__)
    #define outspace(...) pr.write(__VA_ARGS__),pr.write(' ')
    #define rall(v) (v).rbegin(), (v).rend()
    #define fi first
    #define se second
    /*
         
    */
    using poly=FPS<mint>;
    INT(n);
    poly f(n),g(n);
    FOR(i,n){
        INT(x);
        f[i]=x;
    }
    FOR(i,n){
        INT(x);
        g[i]=x;
    }
    poly h=composition(f,g);
    FOR(i,n) outln(h[i].get());
}