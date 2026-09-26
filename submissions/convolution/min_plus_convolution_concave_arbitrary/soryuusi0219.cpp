#ifdef _DEBUG
#define _GLIBCXX_DEBUG
#endif
#include <bits/stdc++.h>
// #include <atcoder/all>
using namespace std;
// using namespace atcoder;
#define GET4(_1,_2,_3,NAME,...) NAME
#define GET5(_1,_2,_3,_4,NAME,...) NAME
#define GET6(_1,_2,_3,_4,_5,NAME,...) NAME
#define GET11(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,NAME,...) NAME
#define rep1(i, n)  for(long long i=0;i<(long long)(n);i++)
#define rep2(i,k,n) for(long long i=k;i<(long long)(n);i++)
#define rep3(i,k,n,s) for(long long i=k;i<(long long)(n);i+=(s))
#define rep(...) GET5(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)
#define per1(i,n) for(long long i=(n)-1ll;i>-1ll;i--)
#define per2(i,k,n) for(long long i=(n)-1ll;i>(long long)(k)-1ll;i--)
#define per3(i,k,n,s) for(long long i=(n)-1ll;i>(long long)(k)-1ll;i-=(s))
#define per(...) GET5(__VA_ARGS__, per3, per2, per1)(__VA_ARGS__)
#define perm(c) sort(all(c));for(bool c##p=1;c##p;c##p=next_permutation(all(c)))
#define pb emplace_back
#define lb(v,k) (lower_bound(all(v),(k))-v.begin())
#define ub(v,k) (upper_bound(all(v),(k))-v.begin())
#define fi first
#define se second
#define dame(...) {out(__VA_ARGS__);return 0;}
#define decimal cout<<fixed<<setprecision(15);
#define all(a) a.begin(),a.end()
#define rsort(a) {sort(all(a));reverse(all(a));}
#define dupli(a) {sort(all(a));a.erase(unique(all(a)),a.end());}
#define readi1(n0) ll n0; cin >> (n0);
#define readi2(n0,n1) ll n0,n1; cin >> (n0) >> (n1);
#define readi3(n0,n1,n2) ll n0,n1,n2; cin >> (n0) >> (n1) >> (n2);
#define readi4(n0,n1,n2,n3) ll n0,n1,n2,n3; cin >> (n0) >> (n1) >> (n2) >> (n3);
#define readi5(n0,n1,n2,n3,n4) ll n0,n1,n2,n3,n4; cin >> (n0) >> (n1) >> (n2) >> (n3) >> (n4);
#define readi6(n0,n1,n2,n3,n4,n5) ll n0,n1,n2,n3,n4,n5; cin >> (n0) >> (n1) >> (n2) >> (n3) >> (n4) >> (n5);
#define readi7(n0,n1,n2,n3,n4,n5,n6) ll n0,n1,n2,n3,n4,n5,n6; cin >> (n0) >> (n1) >> (n2) >> (n3) >> (n4) >> (n5) >> (n6);
#define readi8(n0,n1,n2,n3,n4,n5,n6,n7) ll n0,n1,n2,n3,n4,n5,n6,n7; cin >> (n0) >> (n1) >> (n2) >> (n3) >> (n4) >> (n5) >> (n6) >> (n7);
#define readi9(n0,n1,n2,n3,n4,n5,n6,n7,n8) ll n0,n1,n2,n3,n4,n5,n6,n7,n8; cin >> (n0) >> (n1) >> (n2) >> (n3) >> (n4) >> (n5) >> (n6) >> (n7) >> (n8);
#define readi10(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9) ll n0,n1,n2,n3,n4,n5,n6,n7,n8,n9; cin >> (n0) >> (n1) >> (n2) >> (n3) >> (n4) >> (n5) >> (n6) >> (n7) >> (n8) >> (n9);
#define readi(...) GET11(__VA_ARGS__, readi10, readi9, readi8, readi7, readi6, readi5, readi4, readi3, readi2, readi1)(__VA_ARGS__)
#define readvi1(v,n) vi v(n); rep(i,(n)) cin >> (v)[i];
#define readvi2(v,w,n) vi v(n),w(n); rep(i,(n)) cin >> (v)[i] >> (w)[i];
#define readvi3(v,w,x,n) vi v(n),w(n),x(n); rep(i,(n)) cin >> (v)[i] >> (w)[i] >> (x)[i];
#define readvi4(v,w,x,y,n) vi v(n),w(n),x(n); rep(i,(n)) cin >> (v)[i] >> (w)[i] >> (x)[i] >> (y)[i];
#define readvi(...) GET6(__VA_ARGS__, readvi4, readvi3, readvi2, readvi1)(__VA_ARGS__)
#define reads1(n) string n; cin >> (n);
#define reads2(n,m) string n,m; cin >> (n) >> (m);
#define reads3(n,m,l) string n,m,l; cin >> (n) >> (m) >> (l);
#define reads4(n,m,l,k) string n,m,l,k; cin >> (n) >> (m) >> (l) >> (k);
#define reads(...) GET5(__VA_ARGS__, reads4, reads3, reads2, reads1)(__VA_ARGS__)
#define readvs(v,n) vs v(n); rep(i,(n)) cin >> (v)[i];
#define readd1(n) ld n; cin >> (n);
#define readd2(n,m) ld n,m; cin >> (n) >> (m);
#define readd3(n,m,l) ld n,m,l; cin >> (n) >> (m) >> (l);
#define readd4(n,m,l,k) ld n,m,l; cin >> (n) >> (m) >> (l) >> (k);
#define readd(...) GET5(__VA_ARGS__, readd4, readd3, readd2, readd1)(__VA_ARGS__)
#define readvvi(v,n,m) vvi v((n),vi(m)); rep(i,(n))rep(j,(m)) cin >> (v)[i][j];
#define readvvs(v,n,m) vvs v((n),vs(m)); rep(i,(n))rep(j,(m)) cin >> (v)[i][j];
#define readvpi(v,n) vpi v(n); rep(i,(n)) cin >> (v)[i].fi >> (v)[i].se;
#define readg(G,n,m) vvi G(n);rep(i,m){readi(a,b);a--;b--;G[a].push_back(b);G[b].push_back(a);}
#define readgd(G,n,m) vvi G(n);rep(i,m){readi(a,b);a--;b--;G[a].push_back(b);}
#define readgdrev(G,n,m) vvi G(n);rep(i,m){readi(a,b);a--;b--;G[b].push_back(a);}
#define readt(G,n) vvi G(n);rep(i,n-1){readi(a,b);a--;b--;G[a].push_back(b);G[b].push_back(a);}
using ll = long long;
using ull = unsigned long long;
using ld = long double;
template<class T> using P = pair<T,T>;
template<class T> using PP = tuple<T,T,T>;
template<class T> using PPP = tuple<T,T,T,T>;
using pi = P<ll>;
using ppi = PP<ll>;
using pppi = PPP<ll>;
template<class T> using V = vector<T>;
template<class T> using VV = vector<vector<T>>;
template<class T> using VVV = vector<vector<vector<T>>>;
template<class T> using VVVV = vector<vector<vector<vector<T>>>>;
using vi=V<ll>;
using vvi=VV<ll>;
using vvvi=VVV<ll>;
using vvvvi=VVVV<ll>;
using vpi=V<pi>;
using vvpi=VV<pi>;
using vppi=V<ppi>;
using vvppi=VV<ppi>;
using vpppi=V<pppi>;
using vvpppi=VV<pppi>;
using vb=V<bool>;
using vvb=VV<bool>;
using vs=V<string>;
using vvs=VV<string>;
const ll inf=(1 << 30);
const ll Inf=((ll)1 << 40);
const ll INf=((ll)1 << 50);
const ll INF=((ll)1 << 60);
const double eps=1e-10;
template<class T> using PQ = priority_queue<T>;
template<class T> using SPQ = priority_queue<T,vector<T>,greater<T>>;
/////////////////////////////////////////////////////////
using lll = __int128_t;
using ulll = __uint128_t;
const lll infinf=((lll)1 << 65);
const lll Infinf=((lll)1 << 75);
const lll INfinf=((lll)1 << 85);
const lll INFinf=((lll)1 << 95);
const lll INFInf=((lll)1 << 105);
const lll INFINf=((lll)1 << 115);
const lll INFINF=((lll)1 << 125);
istream &operator>>(istream &is, lll& v) {
  string s; cin >> s;
  v = 0;
  for(char c:s){if(isdigit(c))v=v*10+(c-'0');}
  if(s[0]=='-') v*=-1;
  return is;
}
ostream &operator<<(ostream &os, lll v) {
  if(!ostream::sentry(os)) return os;
  char buf[64];
  char *d=end(buf);
  ulll tmp = (v<0?-v:v);
  do{d--;*d=char(tmp%10+'0');tmp/=10;}while(tmp);
  if(v<0) {d--;*d = '-';}
  int len=end(buf)-d;
  if(os.rdbuf()->sputn(d, len)!=len){os.setstate(ios_base::badbit);}
  return os;
}
istream &operator>>(istream &is, ulll& v) {
  string s; cin >> s;
  v = 0;
  for(char c:s){if(isdigit(c))v=v*10+(c-'0');}
  return is;
}
ostream &operator<<(ostream &os, ulll v) {
  if(!ostream::sentry(os)) return os;
  char buf[64];
  char *d=end(buf);
  do{d--;*d=char(v%10+'0');v/=10;}while(v);
  int len=end(buf)-d;
  if(os.rdbuf()->sputn(d, len)!=len){os.setstate(ios_base::badbit);}
  return os;
}
/////////////////////////////////////////////////////////
// using mint = modint998244353;
// const ll mod = 998244353;
//using mint = modint1000000007;
//const ll mod = 1000000007;
// using vm=V<mint>;
// using vvm=VV<mint>;
// using vvvm=VVV<mint>;
// using vvvvm=VVVV<mint>;
// using pm = P<mint>;
// using ppm = PP<mint>;
// using pppm = PPP<mint>;
// using vpm=V<pm>;
// using vvpm=VV<pm>;
// using vppm=V<ppm>;
// using vvppm=VV<ppm>;
// using vpppm=V<pppm>;
// using vvpppm=VV<pppm>;
// template<int M> istream &operator>>(istream &is, static_modint<M>& m){ll x; is >> x; m = x; return is;}
// template<int M> istream &operator>>(istream &is, dynamic_modint<M>&  m){ll x; is >> x; m = x; return is;}
// template<int M> ostream &operator<<(ostream &os, static_modint<M> m){os << m.val(); return os;}
// template<int M> ostream &operator<<(ostream &os, dynamic_modint<M> m){os << m.val(); return os;}
// #define readm1(n) mint n; cin >> (n);
// #define readm2(n,m) mint n,m; cin >> (n) >> (m);
// #define readm3(n,m,l) mint n,m,l; cin >> (n) >> (m) >> (l);
// #define readm4(n,m,l,k) mint n,m,l,k; cin >> (n) >> (m) >> (l) >> (k);
// #define readm(...) GET5(__VA_ARGS__, readm4, readm3, readm2, readm1)(__VA_ARGS__)
// #define readvm1(v,n) vm v(n); rep(i,(n)) cin >> (v)[i];
// #define readvm2(v,w,n) vm v(n),w(n); rep(i,(n)) cin >> (v)[i] >> (w)[i];
// #define readvm3(v,w,x,n) vm v(n),w(n),x(n); rep(i,(n)) cin >> (v)[i] >> (w)[i] >> (x)[i];
// #define readvm4(v,w,x,y,n) vm v(n),w(n),x(n); rep(i,(n)) cin >> (v)[i] >> (w)[i] >> (x)[i] >> (y)[i];
// #define readvm(...) GET6(__VA_ARGS__, readvm4, readvm3, readvm2, readvm1)(__VA_ARGS__)
//////////////////////////////////////////////////////////  
template<class T> ostream &operator<<(ostream &os, vector<vector<vector<T>>> v);
template<class T> ostream &operator<<(ostream &os, vector<vector<T>> v);
template<class T> istream &operator>>(istream &is, vector<T>& v);
template<class T> ostream &operator<<(ostream &os, vector<T> v);
template<class S, class T> istream &operator>>(istream &is, pair<S,T>& p);
template<class S, class T> ostream &operator<<(ostream &os, pair<S,T> p);
template<class... T> istream &operator>> (istream &is, tuple<T...>& p);
template<class... T> ostream &operator<< (ostream &os, tuple<T...> p);
template<class T> ostream &operator<<(ostream &os, set<T> s);
template<class T> ostream &operator<<(ostream &os, multiset<T> s);
template<class S, class T> ostream &operator<<(ostream &os, map<S,T> m);
template<class S, class T> ostream &operator<<(ostream &os, multimap<S,T> m);
template<class T> ostream &operator<<(ostream &os, queue<T> qu);
template<class T> ostream &operator<<(ostream &os, stack<T> st);
template<class T, class Container, class Compare> ostream &operator<<(ostream &os, priority_queue<T, Container, Compare> qu);
//////////////////////////////////////////////////////////  
template<class T> ostream &operator<<(ostream &os, vector<vector<vector<T>>> v){for(vector<vector<T>> &w : v)for(vector<T> &x : w) os << x << "\n"; return os;}
template<class T> ostream &operator<<(ostream &os, vector<vector<T>> v){for(vector<T> &x : v) os << x << "\n"; return os;}
template<class T> istream &operator>>(istream &is, vector<T>& v){for(T &x : v) is >> x; return is;}
template<class T> ostream &operator<<(ostream &os, vector<T> v){for(T &x : v) os << x << " "; return os;}
template<class S, class T> istream &operator>>(istream &is, pair<S,T>& p){is >> p.fi >> p.se; return is;}
template<class S, class T> ostream &operator<<(ostream &os, pair<S,T> p){os << p.fi << " " << p.se; return os;}
template<class Tuple, size_t ...I> array<int,sizeof...(I)>tuple_cin(istream &is, Tuple&& t, index_sequence<I...>){return {{ (void(is >> get<I>(t)), 0)... }};}
template<class... T> istream &operator>> (istream &is, tuple<T...>& p){tuple_cin(is, p, make_index_sequence<tuple_size<decay_t<tuple<T...>>>::value>{}); return is;}
template<class Tuple, size_t ...I> array<int,sizeof...(I)>tuple_cout(ostream &os, Tuple&& t, index_sequence<I...>){return {{ (void(os << get<I>(t) << " "), 0)... }};}
template<class... T> ostream &operator<< (ostream &os, tuple<T...> p){tuple_cout(os, p, make_index_sequence<tuple_size<decay_t<tuple<T...>>>::value>{}); return os;}
template<class T> ostream &operator<<(ostream &os, set<T> s){for(T x : s) os << x << " "; return os;}
template<class T> ostream &operator<<(ostream &os, multiset<T> s){for(T x : s) os << x << " "; return os;}
template<class S, class T> ostream &operator<<(ostream &os, map<S,T> m){for(auto [k, v] : m) os << "(" << k << "," << v << ") "; return os;}
template<class S, class T> ostream &operator<<(ostream &os, multimap<S,T> m){for(auto [k, v] : m) os << "(" << k << "," << v << ") "; return os;}
template<class T> ostream &operator<<(ostream &os, queue<T> qu){while (!qu.empty()){os << qu.front() << " ";qu.pop();}return os;}
template<class T> ostream &operator<<(ostream &os, stack<T> st){while (!st.empty()){os << st.top() << " ";st.pop();}return os;}
template<class T, class Container, class Compare> ostream &operator<<(ostream &os, priority_queue<T, Container, Compare> qu){while (!qu.empty()){os << qu.top() << " ";qu.pop();}return os;}
template<class... Ts> void read(Ts&... a) {(cin >> ... >> a);}
//////////////////////////////////////////////////////////
void out0(float a){cout<<fixed<<setprecision(15)<<a;}
void out0(double a){cout<<fixed<<setprecision(15)<<a;}
void out0(ld a){cout<<fixed<<setprecision(15)<<a;}
template<class T> void out0(T a){cout<<a;}
template<class T>void out(T a) {out0(a); cout << "\n";}
template<class T, class... Ts>void out(T a, Ts... b) {out0(a); cout << " "; out(b...);}
template<class... Ts>void outf(Ts... a) {out(a...); cout << flush;}
void out0d(ll a){cout<<a;}
void out0d(int a){cout<<a;}
void out0d(unsigned int a){cout<<a;}
void out0d(long unsigned int a){cout<<a;}
void out0d(long int a){cout<<a;}
void out0d(unsigned long long a){cout<<a;}
void out0d(lll a){cout<<a;}
void out0d(ulll a){cout<<a;}
void out0d(char a){cout<<a;}
void out0d(char* a){cout<<a;}
void out0d(string a){cout<<a;}
void out0d(float a){cout<<fixed<<setprecision(15)<<a;}
void out0d(double a){cout<<fixed<<setprecision(15)<<a;}
void out0d(ld a){cout<<fixed<<setprecision(15)<<a;}
// template<int M> void out0d(static_modint<M> a){cout<<a.val();}
// template<int M> void out0d(dynamic_modint<M> a){cout<<a.val();}
template<class T> void out0d(vector<T> v);
template<class T> void out0d(set<T> v);
template<class S, class T> void out0d(map<S,T> v);
template<class T> void out0d(multiset<T> v);
template<class T> void out0d(queue<T> qu);
template<class T> void out0d(stack<T> st);
template<class T> void out0sd(T a);
template<class T, class Container, class Compare> void out0d(priority_queue<T, Container, Compare> qu);
template<class S, class T> void out0d(pair<S,T> p){cout << "("; out0d(p.fi); cout << ", "; out0d(p.se); cout << ")";}
template < typename Tuple, size_t ...I >
array<int,sizeof...(I)>
tuple_printd(Tuple&& t, index_sequence<I...>){return {{ (void( out0sd(get<I>(t))), 0)... }};}
template<class... T> void out0d(tuple<T...> p){cout << "(";tuple_printd(p,make_index_sequence<tuple_size<decay_t<tuple<T...>>>::value>{});cout << ")";}
template<class T> void out0d(vector<T> v){for(T& c : v) out0sd<T>(c);}
template<class T> void out0d(set<T> v){cout << "set["; for(T c : v) out0sd(c); cout << "]";}
template<class T> void out0d(multiset<T> v){cout << "multiset["; for(T c : v) out0sd(c); cout << "]";}
template<class S, class T> void out0d(map<S,T> v){cout << "map["; for(auto [k, x] : v) { out0d(k); cout << ":" ; out0d(x); cout << " ";}cout << "]";}
template<class S, class T> void out0d(multimap<S,T> v){cout << "multimap["; for(auto [k,x] : v) { out0d(k); cout << ":" ; out0d(x); cout << ", ";}cout << "]";}
template<class T> void out0d(queue<T> qu){cout << "queue["; while (!qu.empty()){out0sd(qu.front());qu.pop();}cout << "]";}
template<class T> void out0d(stack<T> st){cout << "stack["; while (!st.empty()){out0sd(st.top());st.pop();}cout << "]";}
template<class T, class Container, class Compare> void out0d(priority_queue<T, Container, Compare> qu){cout << "priority_queue["; while (!qu.empty()){out0sd(qu.top()); qu.pop();}cout << "]";}
template<class T> void out0sd(T a){out0d(a); cout << " ";}
template<class T> void outd(VV<T> v){for(auto c : v) {out0d(c); cout << endl;}}
template<class T> void outd(T a) {out0d(a); cout << endl;}
template<class T, class... Ts>void outd(T a, Ts... b) {out0d(a); cout << " "; outd(b...);}
//////////////////////////////////////////////////////////
template<class T> vector<T> &operator++(vector<T>& v){for(T& x : v) x++; return v;}
template<class T> vector<T> &operator--(vector<T>& v){for(T& x : v) x--; return v;}
template<class T> vector<T> operator++(vector<T>& v, signed){auto res = v; for(T& x : v) x++; return res;}
template<class T> vector<T> operator--(vector<T>& v, signed){auto res = v; for(T& x : v) x--; return res;}
template<class T> vector<T> operator+=(vector<T>& v, const vector<T>& w){if(v.size() < w.size()) v.resize(w.size()); for(int i = 0; i < (int)w.size(); i++) v[i] += w[i]; return v;}
template<class T> vector<T> operator-=(vector<T>& v, const vector<T>& w){if(v.size() < w.size()) v.resize(w.size()); for(int i = 0; i < (int)w.size(); i++) v[i] -= w[i]; return v;}
template<class T> vector<T> operator*=(vector<T>& v, const vector<T>& w){if(v.size() < w.size()) v.resize(w.size()); for(int i = 0; i < (int)w.size(); i++) v[i] *= w[i]; return v;}
template<class T> vector<T> operator/=(vector<T>& v, const vector<T>& w){if(v.size() < w.size()) v.resize(w.size()); for(int i = 0; i < (int)w.size(); i++) v[i] /= w[i]; return v;}
template<class T> vector<T> operator+(vector<T> v, const vector<T>& w){return (v += w);}
template<class T> vector<T> operator-(vector<T> v, const vector<T>& w){return (v -= w);}
template<class T> vector<T> operator*(vector<T> v, const vector<T>& w){return (v *= w);}
template<class T> vector<T> operator/(vector<T> v, const vector<T>& w){return (v /= w);}
template<class T> vector<T> operator*=(vector<T>& v, T w){for(T& x : v) x*=w; return v;}
template<class T> vector<T> operator/=(vector<T>& v, T w){for(T& x : v) x/=w; return v;}
template<class T> vector<T> operator*(vector<T> v, T x){return (v *= x);}
template<class T> vector<T> operator/(vector<T> v, T x){return (v /= x);}

template<class S, class T> pair<S,T> operator+=(pair<S,T>& p, const pair<S,T>& q){p.fi+=q.fi, p.se+=q.se; return p;}
template<class S, class T> pair<S,T> operator+(pair<S,T> p, const pair<S,T>& q){return (p += q);}
template<class S, class T> pair<S,T> operator-=(pair<S,T>& p, const pair<S,T>& q){p.fi-=q.fi, p.se-=q.se; return p;}
template<class S, class T> pair<S,T> operator-(pair<S,T> p, const pair<S,T>& q){return (p -= q);}
//////////////////////////////////////////////////////////
template<class T> bool isin(T x,T l,T r){return (l)<=(x)&&(x)<=(r);}
template<class T> void yesno(T b){if(b)out("yes");else out("no");}
template<class T> void YesNo(T b){if(b)out("Yes");else out("No");}
template<class T> void YESNO(T b){if(b)out("YES");else out("NO");}
template<class T> void posimp(T b){if(b)out("possible");else out("impossible");}
template<class T> void PosImp(T b){if(b)out("Possible");else out("Impossible");}
template<class T> void POSIMP(T b){if(b)out("POSSIBLE");else out("IMPOSSIBLE");}
template<class T> bool chmin(T&a,T b){if(a>b){a=b;return true;}return false;}
template<class T> bool chmax(T&a,T b){if(a<b){a=b;return true;}return false;}
template<class T> T dist_sq(P<T> x, P<T> y){return (x.fi-y.fi)*(x.fi-y.fi)+(x.se-y.se)*(x.se-y.se);}
template<class T> T cross_product(P<T> x, P<T> y){return x.fi * y.se - x.se * y.fi;}
template<class T> bool compare_by_arg(P<T> x, P<T> y){if(x.fi == 0 && x.se == 0) return true; if(y.fi == 0 && y.se == 0) return false; if(x.se >= 0 && y.se < 0) return true; if(x.se < 0 && y.se >= 0) return false; if(x.se == 0 && y.se == 0) return x.fi > y.fi; return x.fi * y.se > x.se * y.fi;}
template<class T> void argsort(V<P<T>>& a){sort(all(a),compare_by_arg<T>);} 
const vpi adj = {{1,0},{0,1},{-1,0},{0,-1}};
const vpi king = {{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};
const vpi knight = {{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2},{2,-1}};
void outs(ll a,ll b,ll i){if(abs(a)>=i-100)out(b);else out(a);}
ll gcd(ll a,ll b){if(b==0)return abs(a);return gcd(abs(b),abs(a)%abs(b));}
ll lcm(ll a,ll b){return abs((a / gcd(a,b)) * b);}
template<class T> T POW(T a, ll b){T res=1;while(b){if(b&1)res=res*a;a=a*a;b>>=1;}return res;}
ll divfloor(ll a, ll b){return (a >= 0 ? a/b : (a-b+1)/b);}
ll divceil(ll a, ll b){return (a >= 0 ? (a+b-1)/b : a/b);}
ll modpow(ll a,ll b,ll modd){ll res=1;a%=modd;while(b){if(b&1)res=res*a%modd;a=a*a%modd;b>>=1;}return res;}
ll sqrtll(ll a){assert(a >= 0); ll r = (ll)sqrtl((ld)a)-1; while(r < 0 || (r+1)*(r+1) <= a) r++; return r;}
ll cbrtll(ll a){assert(a >= 0); ll r = (ll)cbrtl((ld)a)-1; while(r < 0 || (r+1)*(r+1)*(r+1) <= a) r++; return r;}
ll modinv(ll a, ll b){assert(a); if(a == 1) return 1; if(b == 1) return 0; ll ret = (1ll-b*modinv(b%a, a))/a; ret %= b; if(ret < 0) ret += b; return ret;}

/*
Copyright (c) 2025 - Rac75116
Except as otherwise noted, these codes are licensed under the CC0 license.
*/
#if !defined(__clang__) && defined(__GNUC__)
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#endif
#ifdef EVAL
#define ONLINE_JUDGE
#endif
#ifdef ONLINE_JUDGE
#define GSH_USE_COMPILE_TIME_CALCULATION
#define NDEBUG
#else
//#define GSH_USE_COMPILE_TIME_CALCULATION
#define GSH_DIAGNOSTICS_COLOR
//#define NDEBUG
#endif


#include <cstdlib>         // std::exit
#include <cstring>         // std::memcpy, std::memmove
#include <utility>         // std::forward
#include <tuple>           // std::tuple, std::make_tuple

#include <type_traits>
#include <cstdint>
#include <limits>
#if __has_include(<stdfloat>)
#include <stdfloat>
#endif

namespace gsh {

namespace itype {
#if !defined(INT8_MAX) || !defined(UINT8_MAX)
    static_assert(false, "This library needs std::int8_t and std::uint8_t.");
#endif
#if !defined(INT16_MAX) || !defined(UINT16_MAX)
    static_assert(false, "This library needs std::int16_t and std::uint16_t.");
#endif
#if !defined(INT32_MAX) || !defined(UINT32_MAX)
    static_assert(false, "This library needs std::int32_t and std::uint32_t.");
#endif
#if !defined(INT64_MAX) || !defined(UINT64_MAX)
    static_assert(false, "This library needs std::int64_t and std::uint64_t.");
#endif
    using i8 = std::int8_t;
    using u8 = std::uint8_t;
    using i16 = std::int16_t;
    using u16 = std::uint16_t;
    using i32 = std::int32_t;
    using u32 = std::uint32_t;
    using i64 = std::int64_t;
    using u64 = std::uint64_t;
}  // namespace itype

namespace ftype {
    class InvalidFloat16Tag;
    class InvalidBfloat16Tag;
    class InvalidFloat128Tag;
#ifdef __STDCPP_FLOAT16_T__
    using f16 = std::float16_t;
#else
    using f16 = InvalidFloat16Tag;
#endif
#ifdef __STDCPP_FLOAT32_T__
    using f32 = std::float32_t;
#else
    static_assert(std::numeric_limits<float>::is_iec559, "There are no types compliant with IEC 559 binary32.");
    using f32 = float;
#endif
#ifdef __STDCPP_FLOAT64_T__
    using f64 = std::float64_t;
#else
    static_assert(std::numeric_limits<double>::is_iec559, "There are no types compliant with IEC 559 binary64.");
    using f64 = double;
#endif
#ifdef __STDCPP_FLOAT128_T__
    using f128 = std::float128_t;
#elif defined(__SIZEOF_FLOAT128__)
    using f128 = std::conditional_t<std::numeric_limits<long double>::is_iec559 && sizeof(long double) == 16, long double, __float128>;
#else
    using f128 = std::conditional_t<std::numeric_limits<long double>::is_iec559 && sizeof(long double) == 16, long double, InvalidFloat128Tag>;
#endif
#ifdef __STDCPP_BFLOAT16_T__
    using bf16 = std::bfloat16_t;
#else
    using bf16 = InvalidBfloat16Tag;
#endif
}  // namespace ftype

namespace ctype {
    using c8 = char;
    using wc = wchar_t;
    using utf8 = char8_t;
    using utf16 = char16_t;
    using utf32 = char32_t;
}  // namespace ctype

}  // namespace gsh

#include <bit>            // std::countr_zero
#include <ranges>         // std::ranges::forward_range


#define GSH_INTERNAL_SELECT1(a, ...)                         a
#define GSH_INTERNAL_SELECT2(a, b, ...)                      b
#define GSH_INTERNAL_SELECT3(a, b, c, ...)                   c
#define GSH_INTERNAL_SELECT4(a, b, c, d, ...)                d
#define GSH_INTERNAL_SELECT5(a, b, c, d, e, ...)             e
#define GSH_INTERNAL_SELECT6(a, b, c, d, e, f, ...)          f
#define GSH_INTERNAL_SELECT7(a, b, c, d, e, f, g, ...)       g
#define GSH_INTERNAL_SELECT8(a, b, c, d, e, f, g, h, ...)    h
#define GSH_INTERNAL_SELECT9(a, b, c, d, e, f, g, h, i, ...) i

#define GSH_INTERNAL_STR(s)       #s
#define GSH_INTERNAL_CONCAT(a, b) a##b
#define GSH_INTERNAL_VA_SIZE(...) GSH_INTERNAL_SELECT8(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0)
#if defined(__clang__) || defined(__ICC)
#define GSH_INTERNAL_UNROLL(n) _Pragma(GSH_INTERNAL_STR(unroll n))
#elif defined __GNUC__
#define GSH_INTERNAL_UNROLL(n) _Pragma(GSH_INTERNAL_STR(GCC unroll n))
#else
#define GSH_INTERNAL_UNROLL(n)
#endif
#ifdef __GNUC__
#define GSH_INTERNAL_INLINE          __attribute__((always_inline))
#define GSH_INTERNAL_NOINLINE        __attribute__((noinline))
#define GSH_INTERNAL_INLINE_LAMBDA   __attribute__((always_inline))
#define GSH_INTERNAL_NOINLINE_LAMBDA __attribute__((noinline))
#elif defined _MSC_VER
#define GSH_INTERNAL_INLINE   [[msvc::forceinline]]
#define GSH_INTERNAL_NOINLINE [[msvc::noinline]]
#define GSH_INTERNAL_INLINE_LAMBDA
#define GSH_INTERNAL_NOINLINE_LAMBDA
#else
#define GSH_INTERNAL_INLINE
#define GSH_INTERNAL_NOINLINE
#define GSH_INTERNAL_INLINE_LAMBDA
#define GSH_INTERNAL_NOINLINE_LAMBDA
#endif
#if defined(__GNUC__) || defined(__ICC)
#define GSH_INTERNAL_RESTRICT __restrict__
#elif defined _MSC_VER
#define GSH_INTERNAL_RESTRICT __restrict
#else
#define GSH_INTERNAL_RESTRICT
#endif
#ifdef __clang__
#define GSH_INTERNAL_PUSH_ATTRIBUTE(apply, ...) _Pragma(GSH_INTERNAL_STR(clang attribute push(__attribute__((__VA_ARGS__)), apply_to = apply)))
#define GSH_INTERNAL_POP_ATTRIBUTE              _Pragma("clang attribute pop")
#elif defined __GNUC__
#define GSH_INTERNAL_PUSH_ATTRIBUTE(apply, ...) _Pragma("GCC push_options") _Pragma(GSH_INTERNAL_STR(GCC __VA_ARGS__))
#define GSH_INTERNAL_POP_ATTRIBUTE              _Pragma("GCC pop_options")
#else
#define GSH_INTERNAL_PUSH_ATTRIBUTE(apply, ...)
#define GSH_INTERNAL_POP_ATTRIBUTE
#endif


namespace gsh {

namespace internal {
    template<class D> class ArithmeticInterface {
        constexpr D& derived() { return *static_cast<D*>(this); }
        constexpr const D& derived() const { return *static_cast<const D*>(this); }
    public:
        constexpr D operator++(int) noexcept(std::is_nothrow_copy_constructible_v<D> && noexcept(++derived())) {
            D copy = derived();
            ++derived();
            return copy;
        }
        constexpr D operator--(int) noexcept(std::is_nothrow_copy_constructible_v<D> && noexcept(--derived())) {
            D copy = derived();
            --derived();
            return copy;
        }
        constexpr D operator+() const noexcept(std::is_nothrow_copy_constructible_v<D>)
            requires requires(D x) { -x; }
        {
            return derived();
        }
        constexpr bool operator!() const noexcept(noexcept(static_cast<bool>(derived()))) { return !static_cast<bool>(derived()); }
        friend constexpr auto operator+(const D& t1, const D& t2) noexcept(noexcept(D(t1) += t2)) { return D(t1) += t2; }
        friend constexpr auto operator-(const D& t1, const D& t2) noexcept(noexcept(D(t1) -= t2)) { return D(t1) -= t2; }
        friend constexpr auto operator*(const D& t1, const D& t2) noexcept(noexcept(D(t1) *= t2)) { return D(t1) *= t2; }
        friend constexpr auto operator/(const D& t1, const D& t2) noexcept(noexcept(D(t1) /= t2)) { return D(t1) /= t2; }
        friend constexpr auto operator%(const D& t1, const D& t2) noexcept(noexcept(D(t1) %= t2)) { return D(t1) %= t2; }
        friend constexpr auto operator&(const D& t1, const D& t2) noexcept(noexcept(D(t1) &= t2)) { return D(t1) &= t2; }
        friend constexpr auto operator|(const D& t1, const D& t2) noexcept(noexcept(D(t1) |= t2)) { return D(t1) |= t2; }
        friend constexpr auto operator^(const D& t1, const D& t2) noexcept(noexcept(D(t1) ^= t2)) { return D(t1) ^= t2; }
        template<class T> friend constexpr auto operator<<(const D& t1, const T& t2) noexcept(noexcept(D(t1) <<= t2)) { return D(t1) <<= t2; }
        template<class T> friend constexpr auto operator>>(const D& t1, const T& t2) noexcept(noexcept(D(t1) >>= t2)) { return D(t1) >>= t2; }
    };

    template<class D> class IteratorInterface {
        constexpr D& derived() { return *static_cast<D*>(this); }
        constexpr const D& derived() const { return *static_cast<const D*>(this); }
    public:
        using size_type = itype::u32;
        using difference_type = itype::i32;
        constexpr D operator++(int) noexcept(std::is_nothrow_copy_constructible_v<D> && noexcept(++derived())) {
            D copy = derived();
            ++derived();
            return copy;
        }
        constexpr D operator--(int) noexcept(std::is_nothrow_copy_constructible_v<D> && noexcept(--derived())) {
            D copy = derived();
            --derived();
            return copy;
        }
        constexpr auto operator->() noexcept(noexcept(&*derived())) { return &*derived(); }
        constexpr auto operator->() const noexcept(noexcept(&*derived())) { return &*derived(); }
        template<class T> friend constexpr D operator+(const D& a, T&& n) noexcept(noexcept(D(a) += std::forward<T>(n))) { return D(a) += std::forward<T>(n); }
        template<class T> friend constexpr D operator-(const D& a, T&& n) noexcept(noexcept(D(a) -= std::forward<T>(n))) { return D(a) -= std::forward<T>(n); }
    };
}  // namespace internal

}  // namespace gsh


namespace gsh {

[[noreturn]] void Unreachable() {
#if defined __GNUC__ || defined __clang__
    __builtin_unreachable();
#elif _MSC_VER
    __assume(false);
#else
    [[maybe_unused]] itype::u32 n = 1 / 0;
#endif
};
GSH_INTERNAL_INLINE constexpr void Assume(const bool f) {
    if (std::is_constant_evaluated()) return;
#if defined __clang__
    __builtin_assume(f);
#elif defined __GNUC__
    if (!f) __builtin_unreachable();
#elif _MSC_VER
    __assume(f);
#else
    if (!f) Unreachable();
#endif
}
template<bool Likely = true> GSH_INTERNAL_INLINE constexpr bool Expect(const bool f) {
    if (std::is_constant_evaluated()) return f;
#if defined __GNUC__ || defined __clang__
    return __builtin_expect(f, Likely);
#else
    if constexpr (Likely) {
        if (f) [[likely]]
            return true;
        else return false;
    } else {
        if (f) [[unlikely]]
            return false;
        else return true;
    }
#endif
}
GSH_INTERNAL_INLINE constexpr bool Unpredictable(const bool f) {
    if (std::is_constant_evaluated()) return f;
#if defined __clang__
    return __builtin_unpredictable(f);
#elif defined __GNUC__
    return __builtin_expect_with_probability(f, 1, 0.5);
#else
    return f;
#endif
}

class InPlaceTag {};
[[maybe_unused]] constexpr InPlaceTag InPlace;

template<class T>
    requires std::is_trivial_v<T>
GSH_INTERNAL_INLINE constexpr void MemorySet(T* p, ctype::c8 byte, itype::u32 len) {
    if (std::is_constant_evaluated()) {
        struct mem {
            ctype::c8 buf[sizeof(T)] = {};
        };
        mem init;
        for (itype::u32 i = 0; i != sizeof(T); ++i) init.buf[i] = byte;
        for (itype::u32 i = 0; i != len / sizeof(T); ++i) p[i] = std::bit_cast<T>(init);
        if (len % sizeof(T) != 0) {
            auto& ref = p[len / sizeof(T)];
            mem tmp = std::bit_cast<mem>(ref);
            for (itype::u32 i = 0; i != len % sizeof(T); ++i) tmp.buf[i] = byte;
            ref = std::bit_cast<T>(tmp);
        }
    } else std::memset(p, byte, len);
}
template<class T>
    requires std::is_trivial_v<T>
GSH_INTERNAL_INLINE constexpr itype::u32 MemoryChar(T* p, ctype::c8 byte, itype::u32 len) {
    if (std::is_constant_evaluated()) {
        struct mem {
            ctype::c8 buf[sizeof(T)] = {};
        };
        for (itype::u32 i = 0; i != len / sizeof(T); ++i) {
            mem tmp = std::bit_cast<mem>(p[i]);
            for (itype::u32 j = 0; j != sizeof(T); ++j) {
                if (tmp.buf[j] == byte) return i * sizeof(T) + j;
            }
        }
        if (len % sizeof(T) != 0) {
            mem tmp = std::bit_cast<mem>(p[len / sizeof(T)]);
            for (itype::u32 i = 0; i != len % sizeof(T); ++i) {
                if (tmp.buf[i] == byte) return len / sizeof(T) * sizeof(T) + i;
            }
        }
        return 0xffffffff;
    } else {
        const void* tmp = std::memchr(p, byte, len);
        return (tmp == nullptr ? 0xffffffff : static_cast<const ctype::c8*>(tmp) - reinterpret_cast<const ctype::c8*>(p));
    }
}
template<class T, class U>
    requires std::is_trivial_v<T>
GSH_INTERNAL_INLINE constexpr void MemoryCopy(T* GSH_INTERNAL_RESTRICT dst, U* GSH_INTERNAL_RESTRICT src, itype::u32 len) {
    if (std::is_constant_evaluated()) {
        struct mem1 {
            ctype::c8 buf[sizeof(T)] = {};
        };
        struct mem2 {
            ctype::c8 buf[sizeof(U)] = {};
        };
        mem1 tmp1;
        mem2 tmp2;
        for (itype::u32 i = 0; i != len; ++i) {
            if (i % sizeof(U) == 0) tmp2 = std::bit_cast<mem2>(src[i / sizeof(U)]);
            tmp1.buf[i % sizeof(T)] = tmp2.buf[i % sizeof(U)];
            if ((i + 1) % sizeof(T) == 0) {
                dst[i / sizeof(T)] = std::bit_cast<T>(tmp1);
                tmp1 = mem1{};
            }
        }
        if (len % sizeof(T) != 0) {
            mem1 tmp3 = std::bit_cast<mem1>(dst[len / sizeof(T)]);
            for (itype::u32 i = 0; i != len % sizeof(T); ++i) tmp3.buf[i] = tmp1.buf[i];
            dst[len / sizeof(T)] = std::bit_cast<T>(tmp3);
        }
    } else std::memcpy(dst, src, len);
}
/*
template<class T, class U>
    requires std::is_trivially_copyable_v<T> && std::is_trivially_copyable_v<U>
GSH_INTERNAL_INLINE constexpr void MemoryMove(T* dst, U* src, itype::u32 len) {
    if (std::is_constant_evaluated()) {
    } else std::memmove(dst, src, len);
}
*/
GSH_INTERNAL_INLINE constexpr itype::u32 StrLen(const ctype::c8* p) {
    if (std::is_constant_evaluated()) {
        auto q = p;
        while (*q != '\0') ++q;
        return q - p;
    } else return std::strlen(p);
}

template<class T, class U, class V> constexpr bool InRange(const T& l, const U& x, const V& r) {
    return l <= x && x < r;
}

namespace internal {
    template<itype::u32 N, class First, class... Tail> class TypeAtImpl : public TypeAtImpl<N - 1, Tail...> {};
    template<class T, class... Types> class TypeAtImpl<0, T, Types...> {
    public:
        using type = T;
    };
}  // namespace internal
template<itype::u32 N, class... Types> using TypeAt = typename internal::TypeAtImpl<N, Types...>::type;

template<class... Types> class TypeArr {
public:
    constexpr static itype::u32 size() noexcept { return sizeof...(Types); }
    template<itype::u32 N> using type = std::conditional_t<(N < sizeof...(Types)), TypeAt<N, Types...>, void>;
};
template<> class TypeArr<> {
public:
    constexpr static itype::u32 size() noexcept { return 0; }
    template<itype::u32 N> using type = void;
};

}  // namespace gsh

#include <compare>  // std::strong_ordering

#ifdef _MSC_VER
#include <intrin.h>
#include <immintrin.h>
#pragma intrinsic(_umul128, __umulh, _udiv128, __shiftleft128, __shiftright128)
#endif

namespace gsh {

namespace internal {
    GSH_INTERNAL_INLINE constexpr std::pair<itype::u64, itype::u64> Mulu128(itype::u64 muler, itype::u64 mulnd) noexcept {
#if defined(__SIZEOF_INT128__)
        __uint128_t tmp = static_cast<__uint128_t>(muler) * mulnd;
        return { tmp >> 64, tmp };
#else
#if defined(_MSC_VER)
        if (!std::is_constant_evaluated()) {
            itype::u64 high;
            itype::u64 low = _umul128(muler, mulnd, &high);
            return { high, low };
        }
#endif
        itype::u64 u1 = (muler & 0xffffffff);
        itype::u64 v1 = (mulnd & 0xffffffff);
        itype::u64 t = (u1 * v1);
        itype::u64 w3 = (t & 0xffffffff);
        itype::u64 k = (t >> 32);
        muler >>= 32;
        t = (muler * v1) + k;
        k = (t & 0xffffffff);
        itype::u64 w1 = (t >> 32);
        mulnd >>= 32;
        t = (u1 * mulnd) + k;
        k = (t >> 32);
        return { (muler * mulnd) + w1 + k, (t << 32) + w3 };
#endif
    }
    GSH_INTERNAL_INLINE constexpr itype::u64 Mulu128High(itype::u64 muler, itype::u64 mulnd) noexcept {
#if defined(__SIZEOF_INT128__)
        return static_cast<itype::u64>((static_cast<__uint128_t>(muler) * mulnd) >> 64);
#else
#if defined(_MSC_VER)
        if (!std::is_constant_evaluated()) return __umulh(muler, mulnd);
#endif
        return Mulu128(muler, mulnd).first;
#endif
    }
    GSH_INTERNAL_INLINE constexpr std::pair<itype::u64, itype::u64> Divu128(itype::u64 high, itype::u64 low, itype::u64 div) noexcept {
#if (defined(__GNUC__) || defined(__ICC)) && defined(__x86_64__)
        if constexpr (sizeof(void*) == 8) {
            if (!std::is_constant_evaluated()) {
                itype::u64 res, rem;
                __asm__("divq %[v]" : "=a"(res), "=d"(rem) : [v] "r"(div), "a"(low), "d"(high));
                return { res, rem };
            }
        }
#elif defined(_MSC_VER)
        if (!std::is_constant_evaluated()) {
            itype::u64 rem;
            itype::u64 res = _udiv128(high, low, div, &rem);
            return { res, rem };
        }
#endif
#if defined(__SIZEOF_INT128__)
        __uint128_t n = (static_cast<__uint128_t>(high) << 64 | low);
        __uint128_t res = n / div;
        return { res, n - res * div };
#else
        itype::u64 res = 0;
        itype::u64 cur = high;
        for (itype::u64 i = 0; i != 64; ++i) {
            itype::u64 large = cur >> 63;
            cur = cur << 1 | (low >> 63);
            low <<= 1;
            large |= (cur >= div);
            res = res << 1 | large;
            cur -= div & (0 - large);
        }
        return { res, cur };
#endif
    }
    GSH_INTERNAL_INLINE constexpr std::pair<itype::u64, itype::u64> Divu128(itype::u64 high, itype::u64 low, itype::u64 dhigh, itype::u64 dlow) noexcept {
        if (dhigh == 0) {
            if (high >= dlow) {
                itype::u64 qh = high / dlow, r = high % dlow;
                itype::u64 ql = internal::Divu128(r, low, dlow).first;
                high = qh, low = ql;
            } else {
                low = internal::Divu128(high, low, dlow).first;
                high = 0;
            }
        } else if (high >= dhigh) {
            Assume(dhigh != 0);
            itype::i32 s = std::countl_zero(dhigh);
            if (s != 0) {
                itype::u64 yh = dhigh << s | dlow >> (64 - s), yl = dlow << s;
                auto [q, r] = internal::Divu128(high >> (64 - s), high << s | low >> (64 - s), yh);
                auto [mh, ml] = internal::Mulu128(q, yl);
                low = q - (mh >= r && (q >= (low << s) || mh != r));
                high = 0;
            } else {
                low = (high > dhigh || low >= dlow);
                high = 0;
            }
        } else {
            low = 0;
            high = 0;
        }
        return { high, low };
    }
    GSH_INTERNAL_INLINE constexpr std::pair<itype::u64, itype::u64> Modu128(itype::u64 high, itype::u64 low, itype::u64 dhigh, itype::u64 dlow) noexcept {
        if (dhigh == 0) {
            low = internal::Divu128(high % dlow, low, dlow).second;
            high = 0;
        } else if (high >= dhigh) {
            Assume(dhigh != 0);
            itype::i32 s = std::countl_zero(dhigh);
            if (s != 0) {
                itype::u64 yh = dhigh << s | dlow >> (64 - s), yl = dlow << s;
                auto [q, r] = internal::Divu128(high >> (64 - s), high << s | low >> (64 - s), yh);
                auto [mh, ml] = internal::Mulu128(q, yl);
                itype::u64 d = q - (mh >= r && (q >= (low << s) || mh != r));
                auto [dh, dl] = internal::Mulu128(d, dlow);
                high -= dh + d * dhigh;
                high -= low < dl;
                low -= dl;
            } else if (high > dhigh || low >= dlow) {
                high -= dhigh;
                high -= low < dlow;
                low -= dlow;
            }
        }
        return { high, low };
    }
    GSH_INTERNAL_INLINE constexpr itype::u64 ShiftLeft128High(itype::u64 high, itype::u64 low, itype::i32 shift) noexcept {
        Assume(0 <= shift && shift < 64);
#ifdef _MSC_VER
        if (!std::is_constant_evaluated()) return __shiftleft128(low, high, shift);
#endif
        return high << shift | (low >> 1 >> (63 - shift));
    }
    GSH_INTERNAL_INLINE constexpr itype::u64 ShiftRight128Low(itype::u64 high, itype::u64 low, itype::i32 shift) noexcept {
        Assume(0 <= shift && shift < 64);
#ifdef _MSC_VER
        if (!std::is_constant_evaluated()) return __shiftright128(low, high, shift);
#endif
        return low >> shift | (high << 1 << (63 - shift));
    }
}  // namespace internal

#if defined(__SIZEOF_INT128__)
namespace itype {
    using i128 = __int128_t;
    using u128 = __uint128_t;
}  // namespace itype
#else
namespace internal {
    struct LittleEndian128 {
        itype::u64 high, low;
    };
    struct BigEndian128 {
        itype::u64 low, high;
    };
    using SwitchEndian128 = std::conditional_t<std::endian::big != std::endian::native, internal::LittleEndian128, internal::BigEndian128>;
}  // namespace internal

namespace itype {
    class alignas(16) i128;
    class alignas(16) u128 : protected internal::SwitchEndian128, public internal::ArithmeticInterface<u128> {
    public:
        constexpr u128() noexcept { high = 0, low = 0; }
        constexpr u128(const i128& n) noexcept;
        template<std::unsigned_integral T> constexpr u128(const T& n) noexcept { high = 0, low = n; }
        template<std::signed_integral T> constexpr u128(const T& n) noexcept {
            if (n < 0) {
                high = -1, low = ~(0 - n);
                operator++();
            } else {
                high = 0, low = n;
            }
        }
        constexpr u128(const u128&) noexcept = default;
        constexpr u128& operator=(const u128&) noexcept = default;
        constexpr u128 operator-() const noexcept {
            u128 res = ~*this;
            ++res;
            return res;
        }
        constexpr u128& operator+=(const u128& n) noexcept {
            high += n.high;
            low += n.low;
            high += low < n.low;
            return *this;
        }
        constexpr u128& operator-=(const u128& n) noexcept {
            high -= n.high;
            high -= low < n.low;
            low -= n.low;
            return *this;
        }
        constexpr u128& operator*=(const u128& n) noexcept {
            auto [hi, lw] = internal::Mulu128(low, n.low);
            high = low * n.high + high * n.low + hi;
            low = lw;
            return *this;
        }
        constexpr u128& operator/=(const u128& n) noexcept {
            auto [hi, lo] = internal::Divu128(high, low, n.high, n.low);
            high = hi, low = lo;
            return *this;
        }
        constexpr u128& operator%=(const u128& n) noexcept {
            auto [hi, lo] = internal::Modu128(high, low, n.high, n.low);
            high = hi, low = lo;
            return *this;
        }
        constexpr u128& operator++() noexcept {
            ++low;
            high += (low == 0);
            return *this;
        }
        constexpr u128& operator--() noexcept {
            high -= (low == 0);
            --low;
            return *this;
        }
        constexpr u128 operator~() const noexcept {
            u128 res;
            res.high = ~high;
            res.low = ~low;
            return res;
        }
        constexpr u128& operator&=(const u128& n) noexcept {
            high &= n.high;
            low &= n.low;
            return *this;
        }
        constexpr u128& operator|=(const u128& n) noexcept {
            high |= n.high;
            low |= n.low;
            return *this;
        }
        constexpr u128& operator^=(const u128& n) noexcept {
            high ^= n.high;
            low ^= n.low;
            return *this;
        }
        constexpr u128& operator<<=(itype::i32 shift) noexcept {
            if (shift >= 64) {
                high = low << (shift - 64);
                low = 0;
            } else {
                high = internal::ShiftLeft128High(high, low, shift);
                low <<= shift;
            }
            return *this;
        }
        constexpr u128& operator>>=(itype::i32 shift) noexcept {
            if (shift >= 64) {
                low = high >> (shift - 64);
                high = 0;
            } else {
                low = internal::ShiftRight128Low(high, low, shift);
                high >>= shift;
            }
            return *this;
        }
        friend constexpr bool operator==(const u128& a, const u128& b) noexcept { return a.high == b.high && a.low == b.low; }
        friend constexpr std::strong_ordering operator<=>(const u128& a, const u128& b) noexcept {
            if (a.high < b.high || (a.high == b.high && a.low < b.low)) return std::strong_ordering::less;
            if (a.high == b.high && a.low == b.low) return std::strong_ordering::equal;
            if (a.high > b.high || (a.high == b.high && a.low > b.low)) return std::strong_ordering::greater;
            Unreachable();
        }
        constexpr operator bool() const noexcept { return low != 0 || high != 0; }
        template<std::integral T>
            requires(!std::same_as<T, i128>)
        constexpr operator T() const noexcept {
            return static_cast<T>(low);
        }
    };

    class i128 : private u128, public internal::ArithmeticInterface<i128> {
        friend class u128;
    public:
        constexpr i128() noexcept : u128() {}
        constexpr i128(const u128& n) noexcept : u128(n) {}
        template<std::integral T> constexpr i128(const T& n) noexcept : u128(n) {}
        constexpr i128(const i128&) noexcept = default;
        constexpr i128& operator=(const i128&) noexcept = default;
        constexpr i128 operator-() const noexcept { return u128::operator-(); }
        constexpr i128& operator+=(const i128& n) noexcept { return static_cast<i128&>(u128::operator+=(n)); }
        constexpr i128& operator-=(const i128& n) noexcept { return static_cast<i128&>(u128::operator-=(n)); }
        constexpr i128& operator*=(const i128& n) noexcept { return static_cast<i128&>(u128::operator*=(n)); }
        constexpr i128& operator/=(const i128&) noexcept {
            // TODO
            return *this;
        }
        constexpr i128& operator%=(const i128&) noexcept {
            // TODO
            return *this;
        }
        constexpr i128& operator++() noexcept { return static_cast<i128&>(u128::operator++()); }
        constexpr i128& operator--() noexcept { return static_cast<i128&>(u128::operator--()); }
        constexpr i128 operator~() const noexcept { return static_cast<i128>(u128::operator~()); }
        constexpr i128& operator&=(const i128& n) noexcept { return static_cast<i128&>(u128::operator&=(n)); }
        constexpr i128& operator|=(const i128& n) noexcept { return static_cast<i128&>(u128::operator|=(n)); }
        constexpr i128& operator^=(const i128& n) noexcept { return static_cast<i128&>(u128::operator^=(n)); }
        constexpr i128& operator<<=(itype::i32 shift) noexcept { return static_cast<i128&>(u128::operator<<=(shift)); }
        constexpr i128& operator>>=(itype::i32 shift) noexcept { return static_cast<i128&>(u128::operator>>=(shift)); }
        constexpr operator bool() const noexcept { return static_cast<bool>(static_cast<const u128&>(*this)); }
        template<std::integral T>
            requires(!std::same_as<T, u128>)
        constexpr operator T() const noexcept {
            return static_cast<T>(static_cast<const u128&>(*this));
        }
        friend constexpr bool operator==(const i128& a, const i128& b) noexcept { return a.high == b.high && a.low == b.low; }
        friend constexpr std::strong_ordering operator<=>(const i128& a, const i128& b) noexcept {
            constexpr itype::u64 mask = static_cast<itype::u64>(1) << 63;
            itype::u64 ahigh = a.high ^ mask, bhigh = b.high ^ mask;
            if (ahigh < bhigh || (ahigh == bhigh && a.low < b.low)) return std::strong_ordering::less;
            if (ahigh == bhigh && a.low == b.low) return std::strong_ordering::equal;
            if (ahigh > bhigh || (ahigh == bhigh && a.low > b.low)) return std::strong_ordering::greater;
            Unreachable();
        }
    };
    constexpr u128::u128(const i128& n) noexcept {
        high = n.high, low = n.low;
    }
}  // namespace itype
}

namespace std {
template<> struct common_type<gsh::itype::u128, gsh::itype::i128> {
    using type = gsh::itype::u128;
};
template<> struct common_type<gsh::itype::i128, gsh::itype::u128> {
    using type = gsh::itype::u128;
};
template<std::integral T> struct common_type<gsh::itype::u128, T> {
    using type = gsh::itype::u128;
};
template<std::integral T> struct common_type<T, gsh::itype::u128> {
    using type = gsh::itype::u128;
};
template<std::floating_point T> struct common_type<gsh::itype::u128, T> {
    using type = T;
};
template<std::floating_point T> struct common_type<T, gsh::itype::u128> {
    using type = T;
};
template<std::integral T> struct common_type<gsh::itype::i128, T> {
    using type = gsh::itype::i128;
};
template<std::integral T> struct common_type<T, gsh::itype::i128> {
    using type = gsh::itype::i128;
};
template<std::floating_point T> struct common_type<gsh::itype::i128, T> {
    using type = T;
};
template<std::floating_point T> struct common_type<T, gsh::itype::i128> {
    using type = T;
};
}  // namespace std

namespace gsh {

namespace internal {
    template<class T, class U> constexpr bool U128OrI128 = (std::same_as<T, itype::u128> || std::same_as<T, itype::i128> || std::same_as<U, itype::u128> || std::same_as<U, itype::i128>);
}

namespace itype {
    template<class T, class U>
        requires internal::U128OrI128<T, U>
    constexpr auto operator+(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) + static_cast<std::common_type_t<T, U>>(b))) {
        return static_cast<std::common_type_t<T, U>>(a) + static_cast<std::common_type_t<T, U>>(b);
    }
    template<class T, class U>
        requires internal::U128OrI128<T, U>
    constexpr auto operator-(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) - static_cast<std::common_type_t<T, U>>(b))) {
        return static_cast<std::common_type_t<T, U>>(a) - static_cast<std::common_type_t<T, U>>(b);
    }
    template<class T, class U>
        requires internal::U128OrI128<T, U>
    constexpr auto operator*(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) * static_cast<std::common_type_t<T, U>>(b))) {
        return static_cast<std::common_type_t<T, U>>(a) * static_cast<std::common_type_t<T, U>>(b);
    }
    template<class T, class U>
        requires internal::U128OrI128<T, U>
    constexpr auto operator/(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) / static_cast<std::common_type_t<T, U>>(b))) {
        return static_cast<std::common_type_t<T, U>>(a) / static_cast<std::common_type_t<T, U>>(b);
    }
    template<class T, class U>
        requires internal::U128OrI128<T, U>
    constexpr auto operator&(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) & static_cast<std::common_type_t<T, U>>(b))) {
        return static_cast<std::common_type_t<T, U>>(a) & static_cast<std::common_type_t<T, U>>(b);
    }
    template<class T, class U>
        requires internal::U128OrI128<T, U>
    constexpr auto operator|(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) | static_cast<std::common_type_t<T, U>>(b))) {
        return static_cast<std::common_type_t<T, U>>(a) | static_cast<std::common_type_t<T, U>>(b);
    }
    template<class T, class U>
        requires internal::U128OrI128<T, U>
    constexpr auto operator^(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) ^ static_cast<std::common_type_t<T, U>>(b))) {
        return static_cast<std::common_type_t<T, U>>(a) ^ static_cast<std::common_type_t<T, U>>(b);
    }
    template<class T, class U>
        requires internal::U128OrI128<T, U>
    constexpr auto operator==(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) == static_cast<std::common_type_t<T, U>>(b))) {
        return static_cast<std::common_type_t<T, U>>(a) == static_cast<std::common_type_t<T, U>>(b);
    }
    template<class T, class U>
        requires internal::U128OrI128<T, U>
    constexpr auto operator<=>(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) <=> static_cast<std::common_type_t<T, U>>(b))) {
        return static_cast<std::common_type_t<T, U>>(a) <=> static_cast<std::common_type_t<T, U>>(b);
    }
}  // namespace itype
#endif

}  // namespace gsh


namespace gsh {

class Exception {
    char str[512];
    char* cur = str;
    void write(const char* x) {
        for (int i = 0; i != 512; ++i, ++cur) {
            if (x[i] == '\0') break;
            *cur = x[i];
        }
    }
    void write(long long x) {
        if (x == 0) *(cur++) = '0';
        else {
            if (x < 0) {
                *(cur++) = '-';
                x = -x;
            }
            char buf[20];
            int i = 0;
            while (x != 0) buf[i++] = x % 10 + '0', x /= 10;
            while (i--) *(cur++) = buf[i];
        }
    }
    template<class T, class... Args> void generate_message(T x, Args... args) {
        write(x);
        if constexpr (sizeof...(Args) > 0) generate_message(args...);
    }
public:
    Exception() noexcept { *cur = '\0'; }
    Exception(const Exception& x) noexcept {
        for (int i = 0; i != 512; ++i) str[i] = x.str[i];
        cur = x.cur;
    }
    explicit Exception(const char* what_arg) noexcept {
        for (int i = 0; i != 512; ++i, ++cur) {
            *cur = what_arg[i];
            if (what_arg[i] == '\0') break;
        }
    }
    template<class... Args> explicit Exception(Args... args) noexcept {
        generate_message(args...);
        *cur = '\0';
    }
    Exception& operator=(const Exception& x) noexcept {
        for (int i = 0; i != 512; ++i) str[i] = x.str[i];
        cur = x.cur;
        return *this;
    }
    const char* what() const noexcept { return str; }
};

}  // namespace gsh


namespace gsh {

namespace itype {
    struct i4dig;
    struct u4dig;
    struct i8dig;
    struct u8dig;
    struct i16dig;
    struct u16dig;
}  // namespace itype

template<class T> class Parser;

namespace internal {
    template<class Stream> constexpr itype::u16 Parseu4dig(Stream&& stream) {
        itype::u32 v;
        MemoryCopy(&v, stream.current(), 4);
        v ^= 0x30303030;
        itype::i32 tmp = std::countr_zero(v & 0xf0f0f0f0) >> 3;
        v <<= (32 - (tmp << 3));
        v = (v * 10 + (v >> 8)) & 0x00ff00ff;
        v = (v * 100 + (v >> 16)) & 0x0000ffff;
        stream.skip(tmp + 1);
        return v;
    }
    template<class Stream> constexpr itype::u32 Parseu8dig(Stream&& stream) {
        itype::u64 v;
        MemoryCopy(&v, stream.current(), 8);
        v ^= 0x3030303030303030;
        itype::i32 tmp = std::countr_zero(v & 0xf0f0f0f0f0f0f0f0) >> 3;
        v <<= (64 - (tmp << 3));
        v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
        v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
        v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
        stream.skip(tmp + 1);
        return v;
    }
    template<class Stream> constexpr itype::u8 Parseu8(Stream&& stream) {
        return Parseu4dig(stream);
    }
    template<class Stream> constexpr itype::u16 Parseu16(Stream&& stream) {
        return Parseu8dig(stream);
    }
    template<class Stream> constexpr itype::u32 Parseu32(Stream&& stream) {
        itype::u32 res = 0;
        itype::u64 buf[2];
        MemoryCopy(buf, stream.current(), 16);
        itype::u64 rem;
        {
            buf[0] ^= 0x3030303030303030, buf[1] ^= 0x3030303030303030;
            itype::u64 v = buf[0];
            rem = v;
            if (!(v & 0xf0f0f0f0f0f0f0f0)) [[likely]] {
                stream.skip(8);
                rem = buf[1];
                v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
                v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
                v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
                res = v;
            }
        }
        {
            itype::u32 v = rem;
            if (!(v & 0xf0f0f0f0)) {
                rem >>= 32;
                v = (v * 10 + (v >> 8)) & 0x00ff00ff;
                v = (v * 100 + (v >> 16)) & 0x0000ffff;
                res = 10000 * res + v;
                stream.skip(4);
            }
        }
        {
            itype::u32 v = rem & 0xffff;
            if (!(v & 0xf0f0)) {
                rem >>= 16;
                v = (v * 10 + (v >> 8)) & 0x00ff;
                res = 100 * res + v;
                stream.skip(2);
            }
        }
        {
            const bool f = !(rem & 0xf0);
            res = f ? 10 * res + (rem & 0xff) : res;
            stream.skip(f + 1);
        }
        return res;
    };
    template<class Stream> constexpr itype::u64 Parseu64(Stream&& stream) {
        ctype::c8 const* cur = stream.current();
        itype::u64 v;
        MemoryCopy(&v, cur, 8);
        if (!((v ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
            itype::u64 u;
            MemoryCopy(&u, cur + 8, 8);
            v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
            v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
            v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
            if (!((u ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
                u = (u * 10 + (u >> 8)) & 0x00ff00ff00ff00ff;
                u = (u * 100 + (u >> 16)) & 0x0000ffff0000ffff;
                u = (u * 10000 + (u >> 32)) & 0x00000000ffffffff;
                v = v * 100000000 + u;
                itype::u32 rem;
                MemoryCopy(&rem, cur + 16, 4);
                rem ^= 0x30303030;
                if ((rem & 0xf0f0f0f0) == 0) [[unlikely]] {
                    rem = (rem * 10 + (rem >> 8)) & 0x00ff00ff;
                    rem = (rem * 100 + (rem >> 16)) & 0x0000ffff;
                    v = v * 10000 + rem;
                    stream.skip(21);
                } else if ((rem & 0xf0f0f0) == 0) {
                    v = v * 1000 + ((rem & 0xff) * 100) + (((rem * 2561) & 0xff0000) >> 16);
                    stream.skip(20);
                } else if ((rem & 0xf0f0) == 0) {
                    v = v * 100 + (((rem >> 8) + (rem * 10)) & 0xff);
                    stream.skip(19);
                } else if ((rem & 0xf0) == 0) {
                    v = v * 10 + (rem & 0x0000000f);
                    stream.skip(18);
                } else {
                    stream.skip(17);
                }
                return v;
            } else {
                itype::u32 c = 9;
                while (!(u & 0xf0)) {
                    v = v * 10 + (u & 0xff);
                    u >>= 8;
                    ++c;
                }
                stream.skip(c);
                return v;
            }
        } else {
            itype::i32 tmp = std::countr_zero(v & 0xf0f0f0f0f0f0f0f0) >> 3;
            v <<= (64 - (tmp << 3));
            v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
            v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
            v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
            stream.skip(tmp + 1);
            return v;
        }
    }
    template<class Stream> constexpr itype::u128 Parseu128(Stream&& stream) {
        itype::u128 res = 0;
        for (itype::u32 i = 0; i != 4; ++i) {
            itype::u64 v;
            MemoryCopy(&v, stream.current(), 8);
            if (((v ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0) != 0) break;
            v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
            v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
            v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
            if (i == 0) res = v;
            else res = res * 100000000 + v;
            stream.skip(8);
        }
        itype::u64 buf;
        MemoryCopy(&buf, stream.current(), 8);
        buf ^= 0x3030303030303030;
        itype::u64 res2 = 0, pw = 1;
        {
            itype::u32 v = buf;
            if (!(v & 0xf0f0f0f0)) {
                buf >>= 32;
                v = (v * 10 + (v >> 8)) & 0x00ff00ff;
                v = (v * 100 + (v >> 16)) & 0x0000ffff;
                res2 = v;
                pw = 10000;
                stream.skip(4);
            }
        }
        {
            itype::u32 v = buf & 0xffff;
            if (!(v & 0xf0f0)) {
                buf >>= 16;
                v = (v * 10 + (v >> 8)) & 0x00ff;
                res2 = res2 * 100 + v;
                pw *= 100;
                stream.skip(2);
            }
        }
        {
            const ctype::c8 v = buf;
            const bool f = (v & 0xf0) == 0;
            const volatile auto tmp1 = pw * 10, tmp2 = res2 * 10 + v;
            const auto tmp3 = tmp1, tmp4 = tmp2;
            pw = f ? tmp3 : pw;
            res2 = f ? tmp4 : res2;
            stream.skip(f + 1);
        }
        return res * pw + res2;
    }
}  // namespace internal

template<> class Parser<itype::u8> {
public:
    template<class Stream> constexpr itype::u8 operator()(Stream&& stream) const {
        stream.reload(8);
        return internal::Parseu8(stream);
    }
};
template<> class Parser<itype::i8> {
public:
    template<class Stream> constexpr itype::i8 operator()(Stream&& stream) const {
        stream.reload(8);
        bool neg = *stream.current() == '-';
        stream.skip(neg);
        itype::i8 tmp = internal::Parseu8(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u16> {
public:
    template<class Stream> constexpr itype::u16 operator()(Stream&& stream) const {
        stream.reload(8);
        return internal::Parseu16(stream);
    }
};
template<> class Parser<itype::i16> {
public:
    template<class Stream> constexpr itype::i16 operator()(Stream&& stream) const {
        stream.reload(8);
        bool neg = *stream.current() == '-';
        stream.skip(neg);
        itype::i16 tmp = internal::Parseu16(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u32> {
public:
    template<class Stream> constexpr itype::u32 operator()(Stream&& stream) const {
        stream.reload(16);
        return internal::Parseu32(stream);
    }
};
template<> class Parser<itype::i32> {
public:
    template<class Stream> constexpr itype::i32 operator()(Stream&& stream) const {
        stream.reload(16);
        bool neg = *stream.current() == '-';
        stream.skip(neg);
        itype::i32 tmp = internal::Parseu32(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u64> {
public:
    template<class Stream> constexpr itype::u64 operator()(Stream&& stream) const {
        stream.reload(32);
        return internal::Parseu64(stream);
    }
};
template<> class Parser<itype::i64> {
public:
    template<class Stream> constexpr itype::i64 operator()(Stream&& stream) const {
        stream.reload(32);
        bool neg = *stream.current() == '-';
        stream.skip(neg);
        itype::i64 tmp = internal::Parseu64(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u128> {
public:
    template<class Stream> constexpr itype::u128 operator()(Stream&& stream) const {
        stream.reload(64);
        return internal::Parseu128(stream);
    }
};
template<> class Parser<itype::i128> {
public:
    template<class Stream> constexpr itype::i128 operator()(Stream&& stream) const {
        stream.reload(64);
        bool neg = *stream.current() == '-';
        stream.skip(neg);
        itype::i128 tmp = internal::Parseu128(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u4dig> {
public:
    using value_type = itype::u16;
    template<class Stream> constexpr itype::u16 operator()(Stream&& stream) const {
        stream.reload(8);
        return internal::Parseu4dig(stream);
    }
};
template<> class Parser<itype::i4dig> {
public:
    using value_type = itype::i16;
    template<class Stream> constexpr itype::i16 operator()(Stream&& stream) const {
        stream.reload(8);
        bool neg = *stream.current() == '-';
        stream.skip(neg);
        itype::i16 tmp = internal::Parseu4dig(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u8dig> {
public:
    using value_type = itype::u32;
    template<class Stream> constexpr itype::u32 operator()(Stream&& stream) const {
        stream.reload(16);
        return internal::Parseu8dig(stream);
    }
};
template<> class Parser<itype::i8dig> {
public:
    using value_type = itype::i32;
    template<class Stream> constexpr itype::i32 operator()(Stream&& stream) const {
        stream.reload(16);
        bool neg = *stream.current() == '-';
        stream.skip(neg);
        itype::i32 tmp = internal::Parseu8dig(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<ctype::c8> {
public:
    template<class Stream> constexpr ctype::c8 operator()(Stream&& stream) const {
        stream.reload(2);
        ctype::c8 tmp = *stream.current();
        stream.skip(2);
        return tmp;
    }
};
template<> class Parser<ctype::c8*> {
public:
    template<class Stream> constexpr ctype::c8* operator()(Stream&& stream, ctype::c8* s) const {
        stream.reload(16);
        ctype::c8* c = s;
        while (true) {
            const ctype::c8* e = stream.current();
            while (*e >= '!') ++e;
            const itype::u32 len = e - stream.current();
            MemoryCopy(c, stream.current(), len);
            stream.skip(len);
            c += len;
            if (stream.avail() == 0) stream.reload();
            else break;
        }
        stream.skip(1);
        *c = '\0';
        return s;
    }
    template<class Stream> constexpr ctype::c8* operator()(Stream&& stream, ctype::c8* s, itype::u32 n) const {
        itype::u32 rem = n;
        ctype::c8* c = s;
        itype::u32 avail = stream.avail();
        while (avail <= rem) {
            MemoryCopy(c, stream.current(), avail);
            c += avail;
            rem -= avail;
            stream.skip(avail);
            if (rem == 0) {
                *c = '\0';
                return s;
            }
            stream.reload();
            avail = stream.avail();
        }
        MemoryCopy(c, stream.current(), rem);
        c += rem;
        stream.skip(rem + 1);
        *c = '\0';
        return s;
    }
};

template<> class Parser<float> {
public:
    template<class Stream> constexpr float operator()(Stream&& stream) const {
        stream.reload(128);
        const ctype::c8* cur = stream.current();
        ctype::c8* end = nullptr;
        float res = std::strtof(cur, &end);
        if (errno) {
            throw Exception("gsh::Parser<float>::operator() / Failed to parse.");
        }
        stream.skip(end - cur + 1);
        return res;
    }
};
template<> class Parser<double> {
public:
    template<class Stream> constexpr double operator()(Stream&& stream) const {
        stream.reload(128);
        const ctype::c8* cur = stream.current();
        ctype::c8* end = nullptr;
        double res = std::strtod(cur, &end);
        if (errno) {
            throw Exception("gsh::Parser<double>::operator() / Failed to parse.");
        }
        stream.skip(end - cur + 1);
        return res;
    }
};
template<> class Parser<long double> {
public:
    template<class Stream> constexpr long double operator()(Stream&& stream) const {
        stream.reload(128);
        const ctype::c8* cur = stream.current();
        ctype::c8* end = nullptr;
        long double res = std::strtold(cur, &end);
        if (errno) {
            throw Exception("gsh::Parser<long double>::operator() / Failed to parse.");
        }
        stream.skip(end - cur + 1);
        return res;
    }
};


namespace internal {
    template<class T, class P, class Stream, class... Args> struct ParsingIterator {
        using value_type = T;
        using difference_type = itype::i32;
        using pointer = T*;
        using reference = T&;
        itype::u32 n;
        P* ref;
        Stream* stream;
        std::tuple<Args...>* args;
        constexpr ParsingIterator() noexcept : ParsingIterator(0, nullptr, nullptr, nullptr) {}
        constexpr ParsingIterator(itype::u32 m, P* r, Stream* s, std::tuple<Args...>* a) noexcept : n(m), ref(r), stream(s), args(a) {}
        GSH_INTERNAL_INLINE friend constexpr itype::u32 operator-(const ParsingIterator& a, const ParsingIterator& b) noexcept { return a.n - b.n; }
        GSH_INTERNAL_INLINE friend constexpr bool operator==(const ParsingIterator& a, const ParsingIterator& b) noexcept { return a.n == b.n; }
        GSH_INTERNAL_INLINE constexpr ParsingIterator& operator++() noexcept { return ++n, *this; }
        GSH_INTERNAL_INLINE constexpr ParsingIterator operator++(int) noexcept { return { n++, *ref, *stream, *args }; }
        GSH_INTERNAL_INLINE constexpr T operator*() const {
            return [this]<itype::u32... I>(std::integer_sequence<itype::u32, I...>) -> T {
                return (*ref)(*stream, std::get<I>(*args)...);
            }(std::make_integer_sequence<itype::u32, sizeof...(Args)>());
        }
    };
}  // namespace internal
template<class R> concept ParsableRange = std::ranges::forward_range<R> && requires { sizeof(Parser<std::decay_t<std::ranges::range_value_t<R>>>) != 0; };
template<ParsableRange R> class Parser<R> {
public:
    template<class Stream, class... Args> constexpr R operator()(Stream&& stream, itype::u32 len, Args&&... args) const {
        Parser<std::ranges::range_value_t<R>> p;
        std::tuple<Args...> a(std::forward<Args>(args)...);
        using iter = internal::ParsingIterator<std::ranges::range_value_t<R>, decltype(p), std::remove_cvref_t<Stream>, Args...>;
        return R(iter(0, &p, &stream, &a), iter(len, nullptr, nullptr, nullptr));
    }
};

/*
namespace internal {
    template<class T, class U> constexpr bool ParsableTupleImpl = false;
    template<class T, std::size_t... I> constexpr bool ParsableTupleImpl<T, std::integer_sequence<std::size_t, I...>> = (... && requires { sizeof(Parser<std::decay_t<typename std::tuple_element<I, T>::type>>) != 0; });
}  // namespace internal
template<class T> concept ParsableTuple = requires { std::tuple_size<T>::value; } && internal::ParsableTupleImpl<T, std::make_index_sequence<std::tuple_size<T>::value>>;
template<ParsableTuple T>
    requires(!ParsableRange<T>)
class Parser<T> {
public:
    template<class Stream> constexpr T operator()(Stream&&& stream) const {}
};
*/

}  // namespace gsh

#include <charconv>       // std::to_chars, std::chars_format, std::errc


namespace gsh {

namespace itype {
    struct i4dig;
    struct u4dig;
    struct i8dig;
    struct u8dig;
    struct i16dig;
    struct u16dig;
    struct i8_pad;
    struct u8_pad;
    struct u16_pad;
    struct u32_pad;
    struct u64_pad;
    struct u128_pad;
    struct u4dig_pad;
    struct u8dig_pad;
    struct u16dig_pad;
}  // namespace itype

template<class T> class Formatter;

namespace internal {
#ifndef GSH_USE_COMPILE_TIME_CALCULATION
    template<bool Flag> auto InttoStr = [] {
        struct {
            ctype::c8* table;
        } res;
        static ctype::c8 table[40004] = {};
        res.table = table;
        if constexpr (Flag) {
            for (itype::u32 i = 0; i != 10000; ++i) {
                res.table[4 * i + 0] = i < 1000 ? ' ' : (i / 1000 + '0');
                res.table[4 * i + 1] = i < 100 ? ' ' : (i / 100 % 10 + '0');
                res.table[4 * i + 2] = i < 10 ? ' ' : (i / 10 % 10 + '0');
                res.table[4 * i + 3] = (i % 10 + '0');
            }
        } else {
            for (itype::u32 i = 0; i != 10000; ++i) {
                res.table[4 * i + 0] = (i / 1000 + '0');
                res.table[4 * i + 1] = (i / 100 % 10 + '0');
                res.table[4 * i + 2] = (i / 10 % 10 + '0');
                res.table[4 * i + 3] = (i % 10 + '0');
            }
        }
        return res;
    }();
#else
    template<bool Flag> constexpr auto InttoStr = [] {
        struct {
            ctype::c8 table[40004] = {};
        } res;
        if constexpr (Flag) {
            for (itype::u32 i = 0; i != 10000; ++i) {
                res.table[4 * i + 0] = i < 1000 ? ' ' : (i / 1000 + '0');
                res.table[4 * i + 1] = i < 100 ? ' ' : (i / 100 % 10 + '0');
                res.table[4 * i + 2] = i < 10 ? ' ' : (i / 10 % 10 + '0');
                res.table[4 * i + 3] = (i % 10 + '0');
            }
        } else {
            for (itype::u32 i = 0; i != 10000; ++i) {
                res.table[4 * i + 0] = (i / 1000 + '0');
                res.table[4 * i + 1] = (i / 100 % 10 + '0');
                res.table[4 * i + 2] = (i / 10 % 10 + '0');
                res.table[4 * i + 3] = (i % 10 + '0');
            }
        }
        return res;
    }();
#endif
    template<bool Flag = false, class Stream> constexpr void Formatu16(Stream&& stream, itype::u16 n) {
        auto *cur = stream.current(), *p = cur;
        auto copy1 = [&](itype::u16 x) {
            if constexpr (Flag) {
                MemoryCopy(p, InttoStr<Flag>.table + 4 * x, 4);
                p += 4;
            } else {
                itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
                MemoryCopy(p, InttoStr<false>.table + (4 * x + off), 4);
                p += 4 - off;
            }
        };
        auto copy2 = [&](itype::u16 x) {
            MemoryCopy(p, InttoStr<false>.table + 4 * x, 4);
            p += 4;
        };
        if (n < 10000) copy1(n);
        else {
            copy1(n / 10000);
            copy2(n % 10000);
        }
        stream.skip(p - cur);
    }
    template<bool Flag = false, class Stream> constexpr void Formatu32(Stream&& stream, itype::u32 n) {
        auto *cur = stream.current(), *p = cur;
        auto copy1 = [&](itype::u32 x) {
            if constexpr (Flag) {
                MemoryCopy(p, InttoStr<Flag>.table + 4 * x, 4);
                p += 4;
            } else {
                itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
                MemoryCopy(p, InttoStr<false>.table + (4 * x + off), 4);
                p += 4 - off;
            }
        };
        auto copy2 = [&](itype::u32 x) {
            MemoryCopy(p, InttoStr<false>.table + 4 * x, 4);
            p += 4;
        };
        if (n < 100000000) {
            if (n < 10000) copy1(n);
            else {
                copy1(n / 10000);
                copy2(n % 10000);
            }
        } else {
            copy1(n / 100000000);
            copy2(n / 10000 % 10000);
            copy2(n % 10000);
        }
        stream.skip(p - cur);
    }
    template<bool Flag = false, class Stream> constexpr void Formatu64(Stream&& stream, itype::u64 n) {
        auto *cur = stream.current(), *p = cur;
        auto copy1 = [&](itype::u64 x) {
            if constexpr (Flag) {
                MemoryCopy(p, InttoStr<Flag>.table + 4 * x, 4);
                p += 4;
            } else {
                itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
                MemoryCopy(p, InttoStr<false>.table + (4 * x + off), 4);
                p += 4 - off;
            }
        };
        auto copy2 = [&](itype::u64 x) {
            MemoryCopy(p, InttoStr<false>.table + 4 * x, 4);
            p += 4;
        };
        if (n >= 10000000000000000) {
            itype::u64 a = n / 100000000, b = n % 100000000;
            itype::u64 c = a / 10000, d = a % 10000, e = b / 10000, f = b % 10000;
            itype::u64 g = c / 10000, h = c % 10000;
            copy1(g), copy2(h), copy2(d), copy2(e), copy2(f);
        } else if (n >= 1000000000000) {
            itype::u64 a = n / 100000000, b = n % 100000000;
            itype::u64 c = a / 10000, d = a % 10000, e = b / 10000, f = b % 10000;
            copy1(c), copy2(d), copy2(e), copy2(f);
        } else if (n >= 100000000) {
            itype::u64 a = n / 100000000, b = n % 100000000;
            itype::u64 c = b / 10000, d = b % 10000;
            copy1(a), copy2(c), copy2(d);
        } else if (n >= 10000) {
            itype::u64 a = n / 10000, b = n % 10000;
            copy1(a), copy2(b);
        } else {
            copy1(n);
        }
        stream.skip(p - cur);
    }
    template<bool Flag = false, class Stream> constexpr void Formatu128(Stream&& stream, itype::u128 n) {
        auto *cur = stream.current(), *p = cur;
        auto copy1 = [&](itype::u32 x) {
            if constexpr (Flag) {
                MemoryCopy(p, InttoStr<Flag>.table + 4 * x, 4);
                p += 4;
            } else {
                itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
                MemoryCopy(p, InttoStr<false>.table + (4 * x + off), 4);
                p += 4 - off;
            }
        };
        auto copy2 = [&](itype::u32 x) {
            MemoryCopy(p, InttoStr<false>.table + 4 * x, 4);
            p += 4;
        };
        constexpr itype::u128 t = static_cast<itype::u128>(10000000000000000) * 10000000000000000;
        if (n >= t) {
            const itype::u32 dv = n / t;
            n -= dv * t;
            if (dv >= 10000) {
                copy1(dv / 10000);
                copy2(dv % 10000);
            } else copy1(dv);
            auto [a, b] = Divu128(n >> 64, n, 10000000000000000);
            const itype::u32 c = a / 100000000, d = a % 100000000, e = b / 100000000, f = b % 100000000;
            copy2(c / 10000), copy2(c % 10000);
            copy2(d / 10000), copy2(d % 10000);
            copy2(e / 10000), copy2(e % 10000);
            copy2(f / 10000), copy2(f % 10000);
        } else {
            auto [a, b] = Divu128(n >> 64, n, 10000000000000000);
            const itype::u32 c = a / 100000000, d = a % 100000000, e = b / 100000000, f = b % 100000000;
            const itype::u32 g = c / 10000, h = c % 10000, i = d / 10000, j = d % 10000, k = e / 10000, l = e % 10000, m = f / 10000, n = f % 10000;
            if (a == 0) {
                if (e == 0) {
                    if (m == 0) copy1(n);
                    else copy1(m), copy2(n);
                } else {
                    if (k == 0) copy1(l), copy2(m), copy2(n);
                    else copy1(k), copy2(l), copy2(m), copy2(n);
                }
            } else {
                if (c == 0) {
                    if (i == 0) copy1(j), copy2(k), copy2(l), copy2(m), copy2(n);
                    else copy1(i), copy2(j), copy2(k), copy2(l), copy2(m), copy2(n);
                } else {
                    if (g == 0) copy1(h), copy2(i), copy2(j), copy2(k), copy2(l), copy2(m), copy2(n);
                    else copy1(g), copy2(h), copy2(i), copy2(j), copy2(k), copy2(l), copy2(m), copy2(n);
                }
            }
        }
        stream.skip(p - cur);
    }
    template<bool Flag = false, class Stream> constexpr void Formatu4dig(Stream&& stream, itype::u16 x) {
        if constexpr (Flag) {
            MemoryCopy(stream.current(), InttoStr<Flag>.table + 4 * x, 4);
            stream.skip(4);
        } else {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            MemoryCopy(stream.current(), InttoStr<false>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        }
    }
    template<bool Flag = false, class Stream> constexpr void Formatu8dig(Stream&& stream, itype::u32 n) {
        auto *cur = stream.current(), *p = cur;
        auto copy1 = [&](itype::u32 x) {
            if constexpr (Flag) {
                MemoryCopy(p, InttoStr<Flag>.table + 4 * x, 4);
                p += 4;
            } else {
                itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
                MemoryCopy(p, InttoStr<false>.table + (4 * x + off), 4);
                p += 4 - off;
            }
        };
        auto copy2 = [&](itype::u32 x) {
            MemoryCopy(p, InttoStr<false>.table + 4 * x, 4);
            p += 4;
        };
        if (n < 10000) copy1(n);
        else {
            copy1(n / 10000);
            copy2(n % 10000);
        }
        stream.skip(p - cur);
    }
    template<bool Flag = false, class Stream> constexpr void Formatu16dig(Stream&& stream, itype::u64 n) {
        auto *cur = stream.current(), *p = cur;
        auto copy1 = [&](itype::u64 x) {
            if constexpr (Flag) {
                MemoryCopy(p, InttoStr<Flag>.table + 4 * x, 4);
                p += 4;
            } else {
                itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
                MemoryCopy(p, InttoStr<false>.table + (4 * x + off), 4);
                p += 4 - off;
            }
        };
        auto copy2 = [&](itype::u64 x) {
            MemoryCopy(p, InttoStr<false>.table + 4 * x, 4);
            p += 4;
        };
        if (n < 1000000000000) {
            if (n < 100000000) {
                if (n < 10000) copy1(n);
                else {
                    copy1(n / 10000);
                    copy2(n % 10000);
                }
            } else {
                copy1(n / 100000000);
                copy2(n / 10000 % 10000);
                copy2(n % 10000);
            }
        } else {
            copy1(n / 1000000000000);
            copy2(n / 100000000 % 10000);
            copy2(n / 10000 % 10000);
            copy2(n % 10000);
        }
        stream.skip(p - cur);
    }
}  // namespace internal

template<> class Formatter<itype::u16> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::u16 n) const {
        stream.reload(8);
        internal::Formatu16(stream, n);
    }
};
template<> class Formatter<itype::i16> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::i16 n) const {
        stream.reload(8);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu16(stream, n < 0 ? -n : n);
    }
};
template<> class Formatter<itype::u32> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::u32 n) const {
        stream.reload(16);
        internal::Formatu32(stream, n);
    }
};
template<> class Formatter<itype::i32> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::i32 n) const {
        stream.reload(16);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu32(stream, n < 0 ? -n : n);
    }
};
template<> class Formatter<itype::u64> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::u64 n) const {
        stream.reload(32);
        internal::Formatu64(stream, n);
    }
};
template<> class Formatter<itype::i64> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::i64 n) const {
        stream.reload(32);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu64(stream, n < 0 ? -n : n);
    }
};
template<> class Formatter<itype::u128> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::u128 n) const {
        stream.reload(64);
        internal::Formatu128(stream, n);
    }
};
template<> class Formatter<itype::i128> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::i128 n) const {
        stream.reload(64);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu128(stream, n < 0 ? -n : n);
    }
};
template<> class Formatter<itype::u4dig> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::u16 n) const {
        stream.reload(4);
        internal::Formatu4dig(stream, n);
    }
};
template<> class Formatter<itype::i4dig> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::i16 n) const {
        stream.reload(5);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu4dig(stream, static_cast<itype::u16>(n < 0 ? -n : n));
    }
};
template<> class Formatter<itype::u8dig> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::u32 n) const {
        stream.reload(8);
        internal::Formatu8dig(stream, n);
    }
};
template<> class Formatter<itype::i8dig> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::i32 n) const {
        stream.reload(9);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu8dig(stream, static_cast<itype::u32>(n < 0 ? -n : n));
    }
};
template<> class Formatter<itype::u16dig> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::u64 n) const {
        stream.reload(16);
        internal::Formatu16dig(stream, n);
    }
};
template<> class Formatter<itype::i16dig> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::i64 n) const {
        stream.reload(17);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu16dig(stream, static_cast<itype::u64>(n < 0 ? -n : n));
    }
};
template<> class Formatter<itype::u16_pad> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::u16 n) const {
        stream.reload(8);
        internal::Formatu16<true>(stream, n);
    }
};
template<> class Formatter<itype::u32_pad> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::u32 n) const {
        stream.reload(16);
        internal::Formatu32<true>(stream, n);
    }
};
template<> class Formatter<itype::u64_pad> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::u64 n) const {
        stream.reload(32);
        internal::Formatu64<true>(stream, n);
    }
};
template<> class Formatter<itype::u128_pad> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::u128 n) const {
        stream.reload(64);
        internal::Formatu128<true>(stream, n);
    }
};
template<> class Formatter<itype::u4dig_pad> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::u16 n) const {
        stream.reload(4);
        internal::Formatu4dig<true>(stream, n);
    }
};
template<> class Formatter<itype::u8dig_pad> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::u32 n) const {
        stream.reload(8);
        internal::Formatu8dig<true>(stream, n);
    }
};
template<> class Formatter<itype::u16dig_pad> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, itype::u64 n) const {
        stream.reload(16);
        internal::Formatu16dig<true>(stream, n);
    }
};
template<> class Formatter<ctype::c8> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, ctype::c8 c) const {
        stream.reload(1);
        *stream.current() = c;
        stream.skip(1);
    }
};

namespace io {

    enum class FormatterOption : std::underlying_type_t<std::chars_format> {};
    constexpr FormatterOption operator|(FormatterOption a, FormatterOption b) noexcept {
        return static_cast<FormatterOption>(static_cast<std::underlying_type_t<FormatterOption>>(a) | static_cast<std::underlying_type_t<FormatterOption>>(b));
    }
    constexpr auto Fixed = static_cast<FormatterOption>(std::chars_format::fixed);
    constexpr auto General = static_cast<FormatterOption>(std::chars_format::general);
    constexpr auto Hex = static_cast<FormatterOption>(std::chars_format::hex);
    constexpr auto Scientific = static_cast<FormatterOption>(std::chars_format::scientific);

}  // namespace io

namespace internal {
    template<class T> class FloatFormatter {
    public:
        template<class Stream> constexpr void operator()(Stream&& stream, T f, io::FormatterOption fmt = io::Fixed, itype::i32 precision = 12) {
            stream.reload(32);
            auto [ptr, err] = std::to_chars(stream.current(), stream.current() + stream.avail(), f, static_cast<std::chars_format>(fmt), precision);
            if (err != std::errc{}) [[unlikely]] {
                stream.reload();
                auto [ptr, err] = std::to_chars(stream.current(), stream.current() + stream.avail(), f, static_cast<std::chars_format>(fmt), precision);
                if (err != std::errc{}) throw Exception("gsh::internal::FloatFormatter::operator() / The value is too large.");
                stream.skip(ptr - stream.current());
            } else {
                stream.skip(ptr - stream.current());
            }
        }
    };
}  // namespace internal
template<> class Formatter<float> : public internal::FloatFormatter<float> {};
template<> class Formatter<double> : public internal::FloatFormatter<double> {};
template<> class Formatter<long double> : public internal::FloatFormatter<long double> {};
#ifdef __STDCPP_FLOAT16_T__
template<> class Formatter<std::float16_t> : public internal::FloatFormatter<std::float16_t> {};
#endif
#ifdef __STDCPP_FLOAT32_T__
template<> class Formatter<std::float32_t> : public internal::FloatFormatter<std::float32_t> {};
#endif
#ifdef __STDCPP_FLOAT64_T__
template<> class Formatter<std::float64_t> : public internal::FloatFormatter<std::float64_t> {};
#endif
#ifdef __STDCPP_FLOAT128_T__
template<> class Formatter<std::float128_t> : public internal::FloatFormatter<std::float128_t> {};
#endif
#ifdef __STDCPP_BFLOAT16_T__
template<> class Formatter<std::bfloat16_t> : public internal::FloatFormatter<std::bfloat16_t> {};
#endif
#ifdef __SIZEOF_FLOAT128__
template<> class Formatter<__float128> : public internal::FloatFormatter<__float128> {};
#endif
template<> class Formatter<ftype::InvalidFloat16Tag> {};
template<> class Formatter<ftype::InvalidFloat128Tag> {};
template<> class Formatter<ftype::InvalidBfloat16Tag> {};
template<> class Formatter<bool> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, bool b) const {
        stream.reload(1);
        *stream.current() = '0' + b;
        stream.skip(1);
    }
};
template<> class Formatter<const ctype::c8*> {
public:
    template<class Stream> constexpr void operator()(Stream&& stream, const ctype::c8* s) const { operator()(stream, s, StrLen(s)); }
    template<class Stream> constexpr void operator()(Stream&& stream, const ctype::c8* s, itype::u32 len) const {
        itype::u32 avail = stream.avail();
        if (avail >= len) [[likely]] {
            MemoryCopy(stream.current(), s, len);
            stream.skip(len);
        } else {
            MemoryCopy(stream.current(), s, avail);
            len -= avail;
            s += avail;
            stream.skip(avail);
            while (len != 0) {
                stream.reload();
                avail = stream.avail();
                const itype::u32 tmp = len < avail ? len : avail;
                MemoryCopy(stream.current(), s, tmp);
                len -= tmp;
                s += tmp;
                stream.skip(tmp);
            }
        }
    }
};
template<> class Formatter<ctype::c8*> : public Formatter<const ctype::c8*> {};

class NoOutTag {};
constexpr NoOutTag NoOut;
template<> class Formatter<NoOutTag> {
public:
    template<class Stream> constexpr void operator()(Stream&&, NoOutTag) const {}
};

template<class R> concept FormatableRange = std::ranges::forward_range<R> && requires { sizeof(Formatter<std::decay_t<std::ranges::range_value_t<R>>>) != 0; };
template<FormatableRange R> class Formatter<R> {
    template<class Stream, class T, class U> constexpr void print(Stream&& stream, T&& r, U&& sep) const {
        auto first = std::ranges::begin(r);
        auto last = std::ranges::end(r);
        if (!(first != last)) return;
        Formatter<std::decay_t<std::ranges::range_value_t<R>>> formatter;
        while (true) {
            formatter(stream, *first);
            ++first;
            if (first != last) {
                Formatter<std::decay_t<U>>()(stream, sep);
            } else break;
        }
    }
public:
    template<class Stream, class T>
        requires std::same_as<std::decay_t<T>, R>
    constexpr void operator()(Stream&& stream, T&& r) const {
        print(std::forward<Stream>(stream), std::forward<T>(r), ' ');
    }
    template<class Stream, class T, class U>
        requires std::same_as<std::decay_t<T>, R>
    constexpr void operator()(Stream&& stream, T&& r, U&& sep) const {
        print(std::forward<Stream>(stream), std::forward<T>(r), std::forward<U>(sep));
    }
};

namespace internal {
    template<class T, class U> constexpr bool FormatableTupleImpl = false;
    template<class T, std::size_t... I> constexpr bool FormatableTupleImpl<T, std::integer_sequence<std::size_t, I...>> = (... && requires { sizeof(Formatter<std::decay_t<typename std::tuple_element<I, T>::type>>) != 0; });
}  // namespace internal
template<class T> concept FormatableTuple = requires { std::tuple_size<T>::value; } && internal::FormatableTupleImpl<T, std::make_index_sequence<std::tuple_size<T>::value>>;
template<FormatableTuple T>
    requires(!FormatableRange<T>)
class Formatter<T> {
    template<itype::u32 I, class Stream, class U, class Sep> constexpr void print_element(Stream&& stream, U&& x, Sep&& sep) const {
        using std::get;
        using element_type = std::decay_t<std::tuple_element_t<I, T>>;
        if constexpr (requires { x.template get<I>(); }) Formatter<element_type>()(stream, x.template get<I>());
        else Formatter<element_type>()(stream, get<I>(x));
        if constexpr (I < std::tuple_size_v<T> - 1) Formatter<std::decay_t<Sep>>()(stream, sep);
    }
    template<class Stream, class U, class Sep> constexpr void print(Stream&& stream, U&& x, Sep&& sep) const {
        [&]<itype::u32... I>(std::integer_sequence<itype::u32, I...>) {
            (..., print_element<I>(stream, x, sep));
        }(std::make_integer_sequence<itype::u32, std::tuple_size_v<T>>());
    }
public:
    template<class Stream, class U> constexpr void operator()(Stream&& stream, U&& x) const { print(std::forward<Stream>(stream), std::forward<U>(x), ' '); }
    template<class Stream, class U, class Sep> constexpr void operator()(Stream&& stream, U&& x, Sep&& sep) const { print(std::forward<Stream>(stream), std::forward<U>(x), std::forward<Sep>(sep)); }
};

}  // namespace gsh

#include <concepts>                // std::totally_ordered_with, std::same_as, std::integral, std::floating_point

#include <cstddef>                 // std::nullptr_t

#include <typeindex>               // std::hash


namespace gsh {

namespace internal {
    template<class T> constexpr bool IsReferenceWrapper = false;
    template<class U> constexpr bool IsReferenceWrapper<std::reference_wrapper<U>> = true;
    // https://en.cppreference.com/w/cpp/utility/functional/invoke
    template<class C, class Pointed, class Object, class... Args> GSH_INTERNAL_INLINE constexpr decltype(auto) InvokeMemPtr(Pointed C::*member, Object&& object, Args&&... args) {
        using object_t = std::remove_cvref_t<Object>;
        constexpr bool is_member_function = std::is_function_v<Pointed>;
        constexpr bool is_wrapped = IsReferenceWrapper<object_t>;
        constexpr bool is_derived_object = std::is_same_v<C, object_t> || std::is_base_of_v<C, object_t>;
        if constexpr (is_member_function) {
            if constexpr (is_derived_object) return (std::forward<Object>(object).*member)(std::forward<Args>(args)...);
            else if constexpr (is_wrapped) return (object.get().*member)(std::forward<Args>(args)...);
            else return ((*std::forward<Object>(object)).*member)(std::forward<Args>(args)...);
        } else {
            static_assert(std::is_object_v<Pointed> && sizeof...(args) == 0);
            if constexpr (is_derived_object) return std::forward<Object>(object).*member;
            else if constexpr (is_wrapped) return object.get().*member;
            else return (*std::forward<Object>(object)).*member;
        }
    }
}  // namespace internal
template<class F, class... Args> GSH_INTERNAL_INLINE constexpr std::invoke_result_t<F, Args...> Invoke(F&& f, Args&&... args) noexcept(std::is_nothrow_invocable_v<F, Args...>) {
    if constexpr (std::is_member_function_pointer_v<std::remove_cvref_t<F>>) return internal::InvokeMemPtr(f, std::forward<Args>(args)...);
    else return std::forward<F>(f)(std::forward<Args>(args)...);
}

namespace internal {
    template<typename T, typename U> concept LessPtrCmp = requires(T&& t, U&& u) {
        { t < u } -> std::same_as<bool>;
    } && std::convertible_to<T, const volatile void*> && std::convertible_to<U, const volatile void*> && (!requires(T&& t, U&& u) { operator<(std::forward<T>(t), std::forward<U>(u)); } && !requires(T&& t, U&& u) { std::forward<T>(t).operator<(std::forward<U>(u)); });
}  // namespace internal
class Less {
public:
    template<class T, class U>
        requires std::totally_ordered_with<T, U>
    GSH_INTERNAL_INLINE constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<T>() < std::declval<U>())) {
        if constexpr (internal::LessPtrCmp<T, U>) {
            if (std::is_constant_evaluated()) return t < u;
            auto x = reinterpret_cast<itype::u64>(static_cast<const volatile void*>(std::forward<T>(t)));
            auto y = reinterpret_cast<itype::u64>(static_cast<const volatile void*>(std::forward<U>(u)));
            return x < y;
        } else return std::forward<T>(t) < std::forward<U>(u);
    }
    using is_transparent = void;
};
class Greater {
public:
    template<class T, class U>
        requires std::totally_ordered_with<T, U>
    GSH_INTERNAL_INLINE constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<U>() < std::declval<T>())) {
        if constexpr (internal::LessPtrCmp<U, T>) {
            if (std::is_constant_evaluated()) return u < t;
            auto x = reinterpret_cast<itype::u64>(static_cast<const volatile void*>(std::forward<T>(t)));
            auto y = reinterpret_cast<itype::u64>(static_cast<const volatile void*>(std::forward<U>(u)));
            return y < x;
        } else return std::forward<U>(u) < std::forward<T>(t);
    }
    using is_transparent = void;
};
class EqualTo {
public:
    template<class T, class U>
        requires std::equality_comparable_with<T, U>
    GSH_INTERNAL_INLINE constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<T>() == std::declval<U>())) {
        return std::forward<T>(t) == std::forward<U>(u);
    }
    using is_transparent = void;
};

class Identity {
public:
    template<class T> [[nodiscard]]
    GSH_INTERNAL_INLINE constexpr T&& operator()(T&& t) const noexcept {
        return std::forward<T>(t);
    }
    using is_transparent = void;
};

template<class F> class SwapArgs : public F {
public:
    constexpr SwapArgs() noexcept(std::is_nothrow_default_constructible_v<F>) : F() {}
    constexpr SwapArgs(const F& f) noexcept(std::is_nothrow_copy_constructible_v<F>) : F(f) {}
    constexpr SwapArgs(F&& f) noexcept(std::is_nothrow_move_constructible_v<F>) : F(std::move(f)) {}
    constexpr SwapArgs& operator=(const F& f) noexcept(std::is_nothrow_copy_assignable_v<F>) {
        F::operator=(f);
        return *this;
    }
    constexpr SwapArgs& operator=(F&& f) noexcept(std::is_nothrow_move_assignable_v<F>) {
        F::operator=(std::move(f));
        return *this;
    }
    constexpr SwapArgs& operator=(const SwapArgs&) noexcept(std::is_nothrow_copy_assignable_v<F>) = default;
    constexpr SwapArgs& operator=(SwapArgs&&) noexcept(std::is_nothrow_move_assignable_v<F>) = default;
    template<class T, class U> GSH_INTERNAL_INLINE constexpr decltype(auto) operator()(T&& x, U&& y) noexcept(noexcept(F::operator()(std::declval<U>(), std::declval<T>()))) { return F::operator()(std::forward<U>(y), std::forward<T>(x)); }
    template<class T, class U> GSH_INTERNAL_INLINE constexpr decltype(auto) operator()(T&& x, U&& y) const noexcept(noexcept(F::operator()(std::declval<U>(), std::declval<T>()))) { return F::operator()(std::forward<U>(y), std::forward<T>(x)); }
};

template<class F, class... G> class BindFront {
    [[no_unique_address]] F func;
    [[no_unique_address]] BindFront<G...> bind;
public:
    constexpr BindFront() noexcept(std::is_nothrow_default_constructible_v<F> && noexcept(BindFront<G...>())) : func(), bind() {}
    template<class Arg, class... Args>
        requires(sizeof...(Args) == sizeof...(G))
    constexpr BindFront(Arg&& arg, Args&&... args) noexcept(std::is_nothrow_constructible_v<F, Arg> && noexcept(BindFront<G...>(std::forward<Args>(args)...))) : func(std::forward<Arg>(arg)),
                                                                                                                                                                 bind(std::forward<Args>(args)...) {}
    template<class... Args> constexpr decltype(auto) operator()(Args&&... args) & noexcept(std::is_nothrow_invocable_v<F, Args...>) { return Invoke(bind, Invoke(func, std::forward<Args>(args)...)); }
    template<class... Args> constexpr decltype(auto) operator()(Args&&... args) && noexcept(std::is_nothrow_invocable_v<F, Args...>) { return Invoke(std::move(bind), Invoke(std::move(func), std::forward<Args>(args)...)); }
    template<class... Args> constexpr decltype(auto) operator()(Args&&... args) const& noexcept(std::is_nothrow_invocable_v<F, Args...>) { return Invoke(bind, Invoke(func, std::forward<Args>(args)...)); }
    template<class... Args> constexpr decltype(auto) operator()(Args&&... args) const&& noexcept(std::is_nothrow_invocable_v<F, Args...>) { return Invoke(std::move(bind), Invoke(std::move(func), std::forward<Args>(args)...)); }
};
template<class F> class BindFront<F> : public F {
public:
    constexpr BindFront() noexcept(std::is_nothrow_default_constructible_v<F>) : F() {}
    template<class... Args> constexpr BindFront(Args&&... args) noexcept(std::is_nothrow_constructible_v<F, Args...>) : F(std::forward<Args>(args)...) {}
};

template<class T> class CustomizedHash;

namespace internal {
    template<class T> concept Nocvref = std::same_as<T, std::remove_cv_t<T>> && !std::is_reference_v<T>;
    constexpr itype::u64 MixIntegers(itype::u64 a, itype::u64 b) {
        itype::u128 tmp = static_cast<itype::u128>(a) * b;
        return static_cast<itype::u64>(tmp) ^ static_cast<itype::u64>(tmp >> 64);
    }
    constexpr itype::u64 HashBytes(const ctype::c8* ptr, itype::u32 len) noexcept {
        constexpr itype::u64 m = 0xc6a4a7935bd1e995;
        constexpr itype::u64 seed = 0xe17a1465;
        constexpr itype::u32 r = 47;
        itype::u64 h = seed ^ (len * m);
        const itype::u32 n_blocks = len / 8;
        for (itype::u64 i = 0; i < n_blocks; ++i) {
            itype::u64 k;
            const auto p = ptr + i * 8;
            if (std::is_constant_evaluated()) {
                k = 0;
                for (itype::u32 j = 0; j != 8; ++j) k |= static_cast<itype::u64>(p[j]) << (8 * j);
            } else {
                for (int j = 0; j != 8; ++j) *(reinterpret_cast<ctype::c8*>(&k) + j) = *(p + j);
            }
            k *= m;
            k ^= k >> r;
            k *= m;
            h ^= k;
            h *= m;
        }
        const auto data8 = ptr + n_blocks * 8;
        switch (len & 7u) {
        case 7 : h ^= static_cast<itype::u64>(data8[6]) << 48U; [[fallthrough]];
        case 6 : h ^= static_cast<itype::u64>(data8[5]) << 40U; [[fallthrough]];
        case 5 : h ^= static_cast<itype::u64>(data8[4]) << 32U; [[fallthrough]];
        case 4 : h ^= static_cast<itype::u64>(data8[3]) << 24U; [[fallthrough]];
        case 3 : h ^= static_cast<itype::u64>(data8[2]) << 16U; [[fallthrough]];
        case 2 : h ^= static_cast<itype::u64>(data8[1]) << 8U; [[fallthrough]];
        case 1 :
            h ^= static_cast<itype::u64>(data8[0]);
            h *= m;
            [[fallthrough]];
        default : break;
        }
        h ^= h >> r;
        return h;
    }
    constexpr itype::u64 HashBytes(const ctype::c8* ptr) noexcept {
        auto last = ptr;
        while (*last != '\0') ++last;
        return HashBytes(ptr, last - ptr);
    }
    template<class T> concept StdHashCallable = requires(T x) {
        { std::hash<T>{}(x) } -> std::integral;
    };
    template<class T> concept CustomizedHashCallable = requires(T x) {
        { CustomizedHash<T>{}(x) } -> std::integral;
    };
}  // namespace internal

// https://raw.githubusercontent.com/martinus/unordered_dense/v1.3.0/include/ankerl/unordered_dense.h
class Hash {
public:
    template<class T>
        requires internal::CustomizedHashCallable<T>
    constexpr itype::u64 operator()(const T& x) const {
        return static_cast<itype::u64>(CustomizedHash<T>{}(x));
    }
    template<class T>
        requires internal::CustomizedHashCallable<T>
    constexpr itype::u64 operator()(const T& x, const CustomizedHash<T>& h) const {
        return static_cast<itype::u64>(h(x));
    }
    template<class T>
        requires(!internal::CustomizedHashCallable<T> && !std::is_volatile_v<T>)
    constexpr itype::u64 operator()(const T& x) const {
        if constexpr (std::same_as<T, std::nullptr_t>) return operator()(static_cast<void*>(x));
        else if constexpr (std::is_pointer_v<T>) {
            static_assert(sizeof(x) == 4 || sizeof(x) == 8);
            if constexpr (sizeof(x) == 8) return operator()(std::bit_cast<itype::u64>(x));
            else return operator()(std::bit_cast<itype::u32>(x));
        } else if constexpr (std::same_as<T, itype::u64>) return internal::MixIntegers(x, 0x9e3779b97f4a7c15);
        else if constexpr (std::same_as<T, itype::u128>) {
            itype::u64 a = internal::MixIntegers(static_cast<itype::u64>(x), 0x9e3779b97f4a7c15);
            itype::u64 b = internal::MixIntegers(static_cast<itype::u64>(x >> 64), 12638153115695167455ull);
            return a ^ b;
        } else if constexpr (std::integral<T>) {
            static_assert(sizeof(T) <= 16);
            if constexpr (sizeof(T) <= 8) return operator()(static_cast<itype::u64>(x));
            else return operator()(static_cast<itype::u128>(x));
        } else if constexpr (std::floating_point<T>) {
            static_assert(sizeof(T) <= 16);
            if constexpr (sizeof(T) == 2) return operator()(std::bit_cast<itype::u16>(x));
            else if constexpr (sizeof(T) == 4) return operator()(std::bit_cast<itype::u32>(x));
            else if constexpr (sizeof(T) == 8) return operator()(std::bit_cast<itype::u64>(x));
            else if constexpr (sizeof(T) == 16) return operator()(std::bit_cast<itype::u128>(x));
            else if constexpr (sizeof(T) < 8) {
                struct a {
                    ctype::c8 b[sizeof(T)];
                };
                struct c {
                    a d;
                    ctype::c8 e[8 - sizeof(T)]{};
                } f;
                f.d = std::bit_cast<a>(x);
                return operator()(std::bit_cast<itype::u64>(f));
            } else {
                struct a {
                    struct b {
                        ctype::c8 c[sizeof(T)];
                    } d;
                    ctype::c8 e[16 - sizeof(T)]{};
                } f;
                f.d = std::bit_cast<a::b>(x);
                return operator()(std::bit_cast<itype::u128>(f));
            }
        } else if constexpr (internal::StdHashCallable<std::remove_cvref_t<T>>) return static_cast<itype::u64>(std::hash<std::remove_cvref_t<T>>{}(static_cast<std::remove_cvref_t<T>>(x)));
        else {
            static_assert((std::declval<T>(), false), "Cannot find the appropriate hash function.");
            return 0ull;
        }
    }
    using is_transparent = void;
};

class Plus {
public:
    template<class T, class U> constexpr decltype(auto) operator()(T&& t, U&& u) const noexcept(noexcept(std::forward<T>(t) + std::forward<U>(u))) { return std::forward<T>(t) + std::forward<U>(u); }
    using is_transparent = void;
};
class Negate {
public:
    template<class T> constexpr decltype(auto) operator()(T&& t) const noexcept(noexcept(-std::forward<T>(t))) { return -std::forward<T>(t); }
    using is_transparent = void;
};

}  // namespace gsh


#include <unistd.h>
#if defined(__linux__)
#include <sys/mman.h>  // mmap
#include <sys/stat.h>  // stat, fstat
#endif

namespace gsh {

namespace internal {
    template<class D> class IstreamInterface;
}  // namespace internal

template<class D, class Types, class... Args> class ParsingChain;

class NoParsingResult {
    template<class D, class Types, class... Args> friend class ParsingChain;
    constexpr NoParsingResult() noexcept {}
    NoParsingResult(const NoParsingResult&) = delete;
    NoParsingResult(NoParsingResult&&) = delete;
};
class CustomParser {
    ~CustomParser() = delete;
};

template<class D, class... Types, class... Args> class ParsingChain<D, TypeArr<Types...>, Args...> {
    friend class internal::IstreamInterface<D>;
    template<class D2, class Types2, class... Args2> friend class ParsingChain;
    D& ref;
    [[no_unique_address]] std::tuple<Args...> args;
    GSH_INTERNAL_INLINE constexpr ParsingChain(D& r, std::tuple<Args...>&& a) : ref(r), args(std::move(a)) {}
    template<class... Options>
        requires(sizeof...(Args) < sizeof...(Types))
    GSH_INTERNAL_INLINE constexpr auto next_chain(Options&&... options) const {
        return ParsingChain<D, TypeArr<Types...>, Args..., std::tuple<Options...>>(ref, std::tuple_cat(args, std::make_tuple(std::forward_as_tuple(std::forward<Options>(options)...))));
    };
public:
    ParsingChain() = delete;
    ParsingChain(const ParsingChain&) = delete;
    ParsingChain(ParsingChain&&) = delete;
    ParsingChain& operator=(const ParsingChain&) = delete;
    ParsingChain& operator=(ParsingChain&&) = delete;
    template<class... Options>
        requires(sizeof...(Args) == 0)
    [[nodiscard]] constexpr auto option(Options&&... options) const {
        return next_chain(std::forward<Options>(options)...);
    }
    template<class... Options>
        requires(sizeof...(Args) != 0)
    [[nodiscard]] constexpr auto operator()(Options&&... options) const {
        return next_chain(std::forward<Options>(options)...);
    }
    template<std::size_t N> friend constexpr decltype(auto) get(const ParsingChain& chain) {
        auto get_result = [](auto&& parser, auto&&... args) GSH_INTERNAL_INLINE -> decltype(auto) {
            if constexpr (std::is_void_v<std::invoke_result_t<decltype(parser), decltype(args)...>>) {
                Invoke(std::forward<decltype(parser)>(parser), std::forward<decltype(args)>(args)...);
                return NoParsingResult{};
            } else {
                return Invoke(std::forward<decltype(parser)>(parser), std::forward<decltype(args)>(args)...);
            }
        };
        using value_type = typename TypeArr<Types...>::template type<N>;
        if constexpr (N < sizeof...(Args)) {
            if constexpr (std::same_as<CustomParser, value_type>) {
                return std::apply([get_result, &chain](auto&& parser, auto&&... args) -> decltype(auto) { return get_result(std::forward<decltype(parser)>(parser), chain.ref, std::forward<decltype(args)>(args)...); }, std::get<N>(chain.args));
            } else {
                return std::apply([get_result, &chain](auto&&... args) -> decltype(auto) { return get_result(Parser<value_type>(), chain.ref, std::forward<decltype(args)>(args)...); }, std::get<N>(chain.args));
            }
        } else {
            return get_result(Parser<value_type>(), chain.ref);
        }
    }
    constexpr void ignore() const {
        [this]<itype::u32... I>(std::integer_sequence<itype::u32, I...>) {
            (..., get<I>(*this));
        }(std::make_integer_sequence<itype::u32, sizeof...(Types)>());
    }
    template<class T> constexpr operator T() const {
        static_assert(sizeof...(Types) == 1);
        return static_cast<T>(get<0>(*this));
    }
    constexpr decltype(auto) val() const {
        static_assert(sizeof...(Types) == 1);
        return get<0>(*this);
    }
    template<class... To>
        requires(sizeof...(To) == 0 || sizeof...(To) == sizeof...(Types))
    constexpr auto bind() const {
        if constexpr (sizeof...(To) == 0) {
            return [this]<itype::u32... I>(std::integer_sequence<itype::u32, I...>) {
                return std::tuple{ get<I>(*this)... };
            }(std::make_integer_sequence<itype::u32, sizeof...(Types)>());
        } else {
            return [this]<itype::u32... I>(std::integer_sequence<itype::u32, I...>) {
                return std::tuple<To...>{ static_cast<To>(get<I>(*this))... };
            }(std::make_integer_sequence<itype::u32, sizeof...(Types)>());
        }
    }
};

namespace internal {
    template<class D> class IstreamInterface {
        constexpr D& derived() { return *static_cast<D*>(this); }
    public:
        template<class T, class... Types> [[nodiscard]] constexpr auto read() { return ParsingChain<D, TypeArr<T, Types...>>(derived(), std::tuple<>()); }
    };
}  // namespace internal

}  // namespace gsh

namespace std {
template<class D, class... Types, class... Args> class tuple_size<gsh::ParsingChain<D, gsh::TypeArr<Types...>, Args...>> : public integral_constant<size_t, sizeof...(Types)> {};
template<size_t N, class D, class... Types, class... Args> class tuple_element<N, gsh::ParsingChain<D, gsh::TypeArr<Types...>, Args...>> {
public:
    using type = decltype(get<N>(std::declval<const gsh::ParsingChain<D, gsh::TypeArr<Types...>, Args...>&>()));
};
}  // namespace std

namespace gsh {

namespace internal {
    template<class D> class OstreamInterface {
        constexpr D& derived() { return *static_cast<D*>(this); }
    public:
        template<class Sep, class... Args> constexpr void write_sep(Sep&& sep, Args&&... args) {
            [&]<itype::u32... I>(std::integer_sequence<itype::u32, I...>) {
                auto print_value = [&]<itype::u32 Idx>(std::integral_constant<itype::u32, Idx>, auto&& val) {
                    Formatter<std::decay_t<decltype(val)>>()(derived(), val);
                    if constexpr (Idx != sizeof...(Args) - 1) Formatter<std::decay_t<Sep>>()(derived(), std::forward<Sep>(sep));
                };
                (..., print_value(std::integral_constant<itype::u32, I>(), std::forward<Args>(args)));
            }(std::make_integer_sequence<itype::u32, sizeof...(Args)>());
        }
        template<class Sep, class... Args> constexpr void writeln_sep(Sep&& sep, Args&&... args) {
            write_sep(std::forward<Sep>(sep), std::forward<Args>(args)...);
            Formatter<ctype::c8>()(derived(), '\n');
        }
        template<class... Args> constexpr void write(Args&&... args) { write_sep(' ', std::forward<Args>(args)...); }
        template<class... Args> constexpr void writeln(Args&&... args) {
            write_sep(' ', std::forward<Args>(args)...);
            Formatter<ctype::c8>()(derived(), '\n');
        }
    };
}  // namespace internal

template<itype::u32 Bufsize = (1 << 18)> class BasicReader : public internal::IstreamInterface<BasicReader<Bufsize>> {
    itype::i32 fd = 0;
    ctype::c8 buf[Bufsize + 1] = {};
    ctype::c8 *cur = buf, *eof = buf;
public:
    BasicReader() {}
    BasicReader(itype::i32 filehandle) : fd(filehandle) {}
    BasicReader(const BasicReader& rhs) {
        fd = rhs.fd;
        std::memcpy(buf, rhs.buf, rhs.eof - rhs.cur);
        cur = buf + (rhs.cur - rhs.buf);
        eof = buf + (rhs.cur - rhs.eof);
    }
    BasicReader& operator=(const BasicReader& rhs) {
        fd = rhs.fd;
        std::memcpy(buf, rhs.buf, rhs.eof - rhs.cur);
        cur = buf + (rhs.cur - rhs.buf);
        eof = buf + (rhs.cur - rhs.eof);
        return *this;
    }
    void reload() {
        if (eof == buf + Bufsize || eof == cur || [&] {
                auto p = cur;
                while (*p >= '!') ++p;
                return p;
            }() == eof) [[likely]] {
            itype::u32 rem = eof - cur;
            std::memmove(buf, cur, rem);
            *(eof = buf + rem + read(fd, buf + rem, Bufsize - rem)) = '\0';
            cur = buf;
        }
    }
    void reload(itype::u32 len) {
        if (avail() < len) [[unlikely]]
            reload();
    }
    itype::u32 avail() const { return eof - cur; }
    const ctype::c8* current() const { return cur; }
    void skip(itype::u32 n) { cur += n; }
};
class StaticStrReader : public internal::IstreamInterface<StaticStrReader> {
    const ctype::c8* cur;
public:
    constexpr StaticStrReader() {}
    constexpr StaticStrReader(const ctype::c8* c) : cur(c) {}
    constexpr void reload() const {}
    constexpr void reload(itype::u32) const {}
    constexpr itype::u32 avail() const { return static_cast<itype::u32>(-1); }
    constexpr const ctype::c8* current() { return cur; }
    constexpr void skip(itype::u32 n) { cur += n; }
};

template<itype::u32 Bufsize = (1 << 18)> class BasicWriter : public internal::OstreamInterface<BasicWriter<Bufsize>> {
    itype::i32 fd = 1;
    ctype::c8 buf[Bufsize + 1] = {};
    ctype::c8 *cur = buf, *eof = buf + Bufsize;
public:
    BasicWriter() {}
    BasicWriter(itype::i32 filehandle) : fd(filehandle) {}
    BasicWriter(const BasicWriter& rhs) {
        fd = rhs.fd;
        std::memcpy(buf, rhs.buf, rhs.cur - rhs.buf);
        cur = buf + (rhs.cur - rhs.buf);
    }
    BasicWriter& operator=(const BasicWriter& rhs) {
        fd = rhs.fd;
        std::memcpy(buf, rhs.buf, rhs.cur - rhs.buf);
        cur = buf + (rhs.cur - rhs.buf);
        return *this;
    }
    void reload() {
        [[maybe_unused]] itype::i32 tmp = write(fd, buf, cur - buf);
        cur = buf;
    }
    void reload(itype::u32 len) {
        if (eof - cur < len) [[unlikely]]
            reload();
    }
    itype::u32 avail() const { return eof - cur; }
    ctype::c8* current() { return cur; }
    void skip(itype::u32 n) { cur += n; }
};
class StaticStrWriter : public internal::OstreamInterface<StaticStrWriter> {
    ctype::c8* cur;
public:
    constexpr StaticStrWriter() {}
    constexpr StaticStrWriter(ctype::c8* c) : cur(c) {}
    constexpr void reload() const {}
    constexpr void reload(itype::u32) const {}
    constexpr itype::u32 avail() const { return static_cast<itype::u32>(-1); }
    constexpr ctype::c8* current() { return cur; }
    constexpr void skip(itype::u32 n) { cur += n; }
};

class MmapReader : public internal::IstreamInterface<MmapReader> {
    [[maybe_unused]] const itype::i32 fh;
    ctype::c8 *buf, *cur, *eof;
public:
    MmapReader() : fh(0) {
#if !defined(__linux__)
        buf = nullptr;
        BasicWriter<128> wt(2);
        wt.write("gsh::MmapReader / gsh::MmapReader is not available for Windows.\n");
        wt.reload();
        std::exit(1);
#else
        struct stat st;
        fstat(0, &st);
        buf = reinterpret_cast<ctype::c8*>(mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0));
        cur = buf;
        eof = buf + st.st_size;
#endif
    }
    void reload() const {}
    void reload(itype::u32) const {}
    itype::u32 avail() const { return eof - cur; }
    const ctype::c8* current() const { return cur; }
    void skip(itype::u32 n) { cur += n; }
};

}  // namespace gsh


#if defined(ONLINE_JUDGE)
gsh::MmapReader rd;
#else
gsh::BasicReader rd;
#endif
gsh::BasicWriter<1 << 19> wt;

void Main();
int main() {
    Main();
    wt.reload();
}


//c_k = min_{k=i+j}(a_i+b_j), a:concave, b:arbitrary
template<class T, class Compare = less<T>>
vector<T> min_plus_convolution(const vector<T>& A, const vector<T>& B, const Compare &comp = Compare(), T INF = numeric_limits<T>::max()){
  int n = A.size();
  int m = B.size();
  int h = n+m-1;
  vector<T> c(h,INF);
  if(n == 1){
    for(int i = 0; i < m; ++i) c[i] = A[0] + B[i];
    return c;
  }
  if(m == 1){
    for(int i = 0; i < n; ++i) c[i] = A[i] + B[0];
    return c;
  }
  int k = __bit_floor((unsigned)h) << 1;
  int mn = k >> (__bit_width((unsigned)(n-1)));
  vector<int> seg(k,-1);
  for(int l = 0; l < m; ++l){
    int r = l + n;
    r &= -__bit_floor((unsigned)(l^r));
    int s = __bit_width((unsigned)(r-l-1));
    int i = (k + l) >> s;
    int f = l;
    while(s && ~f){
      int m = ((i << 1 | 1) << --s) - k - 1;
      if(m < l) i = i << 1 | 1;
      else if(~seg[i] && comp(B[seg[i]] + A[m-seg[i]],B[f] + A[m-f])) i = i << 1;
      else{
        swap(seg[i],f);
        i = i << 1 | 1; 
      }
    }
    if(~f && comp(B[f] + A[i-f-k], c[i-k])) c[i-k] = B[f] + A[i-f-k];
    for(int j = (l + k) >> 1; j >= mn; j >>= 1) if(~seg[j] && comp(B[seg[j]] + A[l-seg[j]], c[l])) c[l] = B[seg[j]] + A[l-seg[j]];
  }
  for(int i = m; i < h; ++i) for(int j = (i + k) >> 1; j >= mn; j >>= 1) if(~seg[j] && comp(B[seg[j]] + A[i-seg[j]], c[i])) c[i] = B[seg[j]] + A[i-seg[j]];
  fill(all(seg),-1);
  for(int r = h; r >= n;){
    int f = r - n;
    int l = r & -__bit_floor((unsigned)(f^r));
    if(l != r){
      int s = __bit_width((unsigned)(r-l-1));
      int i = (k + l) >> s;
      while(s && ~f){
        int m = ((i << 1 | 1) << --s) - k;
        if(m >= r) i = i << 1;
        else if(~seg[i] && comp(B[seg[i]] + A[m-seg[i]], B[f] + A[m-f])) i = i << 1 | 1;
        else{
          swap(seg[i],f);
          i = i << 1; 
        }
      }
      if(~f && comp(B[f] + A[i-f-k], c[i-k])) c[i-k] = B[f] + A[i-f-k];
    }
    --r;
    for(int j = (r + k) >> 1; j >= mn; j >>= 1) if(~seg[j] && comp(B[seg[j]] + A[r-seg[j]], c[r])) c[r] = B[seg[j]] + A[r-seg[j]];
  }
  for(int i = 0; i < n; ++i) for(int j = (i + k) >> 1; j >= mn; j >>= 1) if(~seg[j] && comp(B[seg[j]] + A[i-seg[j]], c[i])) c[i] = B[seg[j]] + A[i-seg[j]];
  return c;
}

// int main(){
//   std::cin.tie(nullptr), std::ios_base::sync_with_stdio(false);
//   readi(n,m);
//   readvi(a,n);
//   readvi(b,m);
//   out(min_plus_convolution(a,b));
// }


using namespace std;
using namespace gsh;
using namespace gsh::itype;
using namespace gsh::ftype;
using namespace gsh::ctype;
void Main() {
    rd.reload(64);
    u32 n = internal::Parseu64(rd);
    u32 m = internal::Parseu64(rd);
    vector<int> a(n);
    vector<int> b(m);
    for (u32 i = 0; i != n; ++i) {
      rd.reload(32);
      a[i] = internal::Parseu64(rd);
    }
    for (u32 i = 0; i != m; ++i) {
      rd.reload(32);
      b[i] = internal::Parseu64(rd);
    }
    auto c = min_plus_convolution(a,b);
    for (u32 i = 0; i != n+m-1; ++i) {
      wt.reload(32);
      internal::Formatu64<true>(wt, c[i]);
      *wt.current() = ' ';
      wt.skip(1);
    }
    *wt.current() = '\n';
    wt.skip(1);
}