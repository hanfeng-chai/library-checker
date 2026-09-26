#line 2 "library/template/template.hpp"
//#pragma GCC target("avx")
//#pragma GCC optimize("O3")
//#pragma GCC optimize("unroll-loops")
#include<bits/stdc++.h>
#define overload4(a,b,c,d,e,...) e
#define overload3(a,b,c,d,...) d
#define rep1(a) for(ll i=0;i<(ll)(a);i++)
#define rep2(i,a) for(ll i=0;i<(ll)(a);i++)
#define rep3(i,a,b) for(ll i=(ll)(a);i<(ll)(b);i++)
#define rep4(i,a,b,c) for(ll i=(ll)(a);i<(ll)(b);i+=(ll)(c))
#define rep(...) overload4(__VA_ARGS__, rep4, rep3, rep2, rep1)(__VA_ARGS__)
#define rrep1(a) for(ll i=(ll)(a)-1;i>=0;i--)
#define rrep2(i,a) for(ll i=(ll)(a)-1;i>=0;i--)
#define rrep3(i,a,b) for(ll i=(ll)(b)-1;i>=(ll)(a);i--)
#define rrep(...) overload3(__VA_ARGS__,rrep3,rrep2,rrep1)(__VA_ARGS__)
#define fore(...) for (auto&& __VA_ARGS__)
#define all1(i) begin(i),end(i)
#define all2(i,a) begin(i),begin(i)+a
#define all3(i,a,b) begin(i)+a,begin(i)+b
#define all(...) overload3(__VA_ARGS__,all3,all2,all1)(__VA_ARGS__)
#define rall(n) (n).rbegin(),(n).rend()
#define INT(...) int __VA_ARGS__;scan(__VA_ARGS__)
#define LL(...) ll __VA_ARGS__;scan(__VA_ARGS__)
#define STR(...) string __VA_ARGS__;scan(__VA_ARGS__)
#define CHR(...) char __VA_ARGS__;scan(__VA_ARGS__)
#define DBL(...) double __VA_ARGS__;scan(__VA_ARGS__)
#define LD(...) ld __VA_ARGS__;scan(__VA_ARGS__)
#define pb push_back
#define eb emplace_back
#define END(...) {print(__VA_ARGS__);return;}
using namespace std;
using ll=long long;
using ull=unsigned long long;
using ld=long double;
using vl=vector<ll>;
using vi=vector<int>;
using vs=vector<string>;
using vc=vector<char>;
using vvl=vector<vl>;
using pi=pair<int,int>;
using pl=pair<ll,ll>;
using vvc=vector<vc>;
using vd=vector<double>;
using vp=vector<pl>;
using vb=vector<bool>;
constexpr int dx[8]={1,0,-1,0,1,-1,-1,1};
constexpr int dy[8]={0,1,0,-1,1,1,-1,-1};
constexpr ll MOD=1000000007;
constexpr ll mod=998244353;
constexpr ld EPS=1e-8;
constexpr ld PI=3.1415926535897932384626;
template<typename T,typename U>
ostream &operator<<(ostream&os,const pair<T,U>&p){os<<p.first<<" "<<p.second;return os;}
template<typename T,typename U>
istream &operator>>(istream&is,pair<T,U>&p){is>>p.first>>p.second;return is;}
template<typename T>
ostream &operator<<(ostream&os,const vector<T>&v){for(auto it=v.begin();it!=v.end();){os<<*it<<((++it)!=v.end()?" ":"");}return os;}
template<typename T>
istream &operator>>(istream&is,vector<T>&v){for(T &in:v){is>>in;}return is;}
void scan(){}
template<class Head,class... Tail>
void scan(Head&head,Tail&... tail){cin>>head;scan(tail...);}
template<class T>
void print(const T &t){cout<<t<<'\n';}
template<class Head, class... Tail>
void print(const Head &head, const Tail &... tail){cout<<head<<' ';print(tail...);}
template<class... T>
void fin(const T &... a){print(a...);exit(0);}
template<typename T,typename U>
inline bool chmax(T&a,U b){return a<b&&(a=b,true);}
template<typename T,typename U>
inline bool chmin(T&a,U b){return a>b&&(a=b,true);}
template<typename T>
class infinity{
  public:
  static constexpr T MAX=numeric_limits<T>::max();
  static constexpr T MIN=numeric_limits<T>::min();
  static constexpr T value=numeric_limits<T>::max()/2;
  static constexpr T mvalue=numeric_limits<T>::min()/2;
};
#if __cplusplus <= 201402L
template<class T>constexpr T infinity<T>::value;
template<class T>constexpr T infinity<T>::mvalue;
template<class T>constexpr T infinity<T>::MAX;
template<class T>constexpr T infinity<T>::MIN;
#endif
template<typename T>constexpr T INF=infinity<T>::value;
constexpr long long inf=INF<ll>;
inline int popcnt(ull x){
#if __cplusplus>=202002L
return popcount(x);
#endif
x=(x&0x5555555555555555)+((x>>1)&0x5555555555555555);x=(x&0x3333333333333333)+((x>>2)&0x3333333333333333);x=(x&0x0f0f0f0f0f0f0f0f)+((x>>4)&0x0f0f0f0f0f0f0f0f);x=(x&0x00ff00ff00ff00ff)+((x>>8)&0x00ff00ff00ff00ff);x=(x&0x0000ffff0000ffff)+((x>>16)&0x0000ffff0000ffff);return (x&0x00000000ffffffff)+((x>>32)&0x00000000ffffffff);
}
template<typename T,typename=void>
struct is_specialize:false_type{};
template<typename T>
struct is_specialize<T,typename conditional<false,typename T::iterator, void>::type>:true_type{};
template<typename T>
struct is_specialize<T,typename conditional<false,decltype(T::first),void>::type>:true_type{};
template<typename T>
struct is_specialize<T,enable_if_t<is_integral<T>::value,void>>:true_type{};
void dump(const char&t){cerr<<t;}
void dump(const string&t){cerr<<t;}
void dump(const bool&t){cerr<<(t?"true":"false");}
template <typename T,enable_if_t<!is_specialize<T>::value,nullptr_t> =nullptr>
void dump(const T&t){cerr<<t;}
template<typename T>
void dump(const T&t,enable_if_t<is_integral<T>::value>* =nullptr){string tmp;if(t==infinity<T>::value||t==infinity<T>::MAX)tmp="inf";if(is_signed<T>::value&&(t==infinity<T>::mvalue||t==infinity<T>::MIN))tmp="-inf";if(tmp.empty())tmp=to_string(t);cerr<<tmp;}
template<typename T,typename U>
void dump(const pair<T,U>&);
template<typename T>
void dump(const T&t,enable_if_t<!is_void<typename T::iterator>::value>* =nullptr){cerr<<"{";for(auto it=t.begin();it!=t.end();){dump(*it);cerr<<(++it==t.end()?"":",");}cerr<<"}";}
template<typename T,typename U>
void dump(const pair<T,U>&t){cerr<<"(";dump(t.first);cerr<<",";dump(t.second);cerr<<")";}
void trace(){cerr<<endl;}
template<typename Head,typename... Tail>
void trace(Head&&head,Tail&&... tail){dump(head);if(sizeof...(tail))cerr<<",";trace(forward<Tail>(tail)...);}
#ifdef ONLINE_JUDGE
#define debug(...) (void(0))
#else
#define debug(...) do{cerr<<#__VA_ARGS__<<"=";trace(__VA_ARGS__);}while(0)
#endif
struct IOSetup{IOSetup(){cin.tie(nullptr);ios::sync_with_stdio(false);cout.tie(0);cout<<fixed<<setprecision(12);cerr<<fixed<<setprecision(12);}};
template<class F>struct REC{F f;REC(F&&f_):f(std::forward<F>(f_)){}template<class...Args>auto operator()(Args&&...args)const{return f(*this, std::forward<Args>(args)...);}};
/**
 * @brief Template(テンプレート)
*/
#line 2 "code.cpp"
template<typename Key,typename Val>
struct hash_map{
  using u32=uint32_t;
  using u64=uint64_t;
  using Data=pair<Key,Val>;
  protected:
  template <typename K>
  inline u64 randomized(const K&key)const{
    return u64(key)^r;
  }
  template<typename K,enable_if_t<is_integral<K>::value,nullptr_t> = nullptr>
  inline u64 inner_hash(const K&key)const{
    return (randomized(key)*11995408973635179863ULL);
  }
  template<typename K,enable_if_t<is_integral<decltype(K::first)>::value,nullptr_t> = nullptr,enable_if_t<is_integral<decltype(K::second)>::value,nullptr_t> = nullptr>
  inline u64 inner_hash(const K&key)const{
    u64 a=randomized(key.first),b=randomized(key.second);
    a*=11995408973635179863ULL;
    b*=10150724397891781847ULL;
    return (a+b);
  }
  template <typename K,enable_if_t<is_integral<typename K::value_type>::value,nullptr_t> = nullptr>
  inline u64 inner_hash(const K&key)const{
    static constexpr u64 mod=(1LL << 61)-1;
    static constexpr u64 base=950699498548472943ULL;
    u64 res=0;
    for(auto&elem:key) {
      __uint128_t x=__uint128_t(res)*base+(randomized(elem)&mod);
      res=(x&mod)+(x>>61);
    }
    __uint128_t x=__uint128_t(res)*base;
    res=(x&mod)+(x >> 61);
    if(res>=mod)res-=mod;
    return (res<<3);
  }
  inline u32 hash(const Key&key)const{
    return inner_hash(key)>>shift;
  }
  Key*key;
  Val*val;
  bitset<1<<19>flag;
  static constexpr u32 shift = 45;
  static u64 r;
  public:
  explicit hash_map():key(new Key[1<<19]),val(new Val[1<<19]){}
  pair<bool,Val&>operator[](const Key&k){
    u32 h=hash(k);
    while(true){
      if(!flag[h]){
        key[h]=k;
        flag[h]=1;
        return {false,val[h]=Val()};
      }
      if(key[h]==k)return {true,val[h]};
      h=(h+1)&((1<<19)-1);
    }
  }
};
template<typename Key,typename Val>uint64_t hash_map<Key,Val>::r=chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
struct modint{
  private:
  static constexpr int m=998244353;
  unsigned int x;
  static constexpr unsigned int umod(){return m;}
  public:
  static modint raw(int v){
    modint ret;
    ret.x=v;
    return ret;
  }
  modint():x(0){}
  modint(int y){
    int v=y%m;
    if(v<0)v+=m;
    x=(unsigned int)v;
  }
  modint(long long y){
    long long v=y%(long long)m;
    if(v<0)v+=m;
    x=(unsigned int)v;
  }
  modint(unsigned int y){
    x=(unsigned int)(y%umod());
  }
  modint& operator++(){x++;if(x==umod())x=0;return *this;}
  modint& operator--(){if(x==0)x=umod();x--;return *this;}
  modint operator++(int){
    modint ret=*this;
    ++*this;
    return ret;
  }
  modint operator--(int){
    modint ret=*this;
    --*this;
    return ret;
  }
  modint& operator+=(const modint&p){if((x+=p.x)>=umod())x-=umod();return *this;}
  modint& operator-=(const modint&p){if((x-=p.x)>=umod())x+=umod();return *this;}
  modint& operator*=(const modint&p){
    unsigned long long y=x;
    y*=p.x;
    x=(unsigned int)(y%umod());
    return *this;
  }
  modint& operator/=(const modint&p){return *this*=p.inv();}
  modint operator+()const{return *this;}
  modint operator-()const{return modint()-*this;}
  modint pow(long long n)const{
    modint ret(1),mul=*this;
    while(n){
      if(n&1)ret*=mul;
      mul*=mul;
      n>>=1;
    }
    return ret;
  }
  modint inv()const{
    long long a=x,b=m,u=1,v=0;
    while(b){
      long long t=a/b;
      swap(a-=t*b,b);
      swap(u-=t*v,v);
    }
    return modint(u);
  }
  friend modint operator+(const modint&l,const modint&r){return modint(l)+=r;}
  friend modint operator-(const modint&l,const modint&r){return modint(l)-=r;}
  friend modint operator*(const modint&l,const modint&r){return modint(l)*=r;}
  friend modint operator/(const modint&l,const modint&r){return modint(l)/=r;}
  friend bool operator==(const modint&l,const modint&r){return l.x==r.x;}
  friend bool operator!=(const modint&l,const modint&r){return l.x!=r.x;}
  friend ostream &operator<<(ostream &os,const modint&p) {
    return os<<p.val();
  }
  friend istream &operator>>(istream &is, modint &a) {
    long long t;
    is>>t;
    a=modint(t);
    return (is);
  }
  static constexpr int get_mod(){return m;}
  int val()const{return (int)x;}
};
/**
 * @brief modint
*/
#include <unistd.h>
#line 156 "code.cpp"
#include <type_traits>
#line 158 "code.cpp"
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
int main(){
  fastio::Scanner sc(stdin);
  fastio::Printer pr(stdout);
  using mint=modint;
  int n;sc.read(n);
  hash_map<int,int>mp;
  vector<mint>sum(n+1);
  const mint two=mint::raw(2),zero=mint();
  sum[0]=mint::raw(1);
  int a;
  for(int i=0;i<n;i++){
    sc.read(a);
    auto tmp=mp[a];
    sum[i+1]=two*sum[i]+(tmp.first?-sum[tmp.second]:zero);
    tmp.second=i;
  }
  sum[n]--;
  pr.writeln(sum[n].val());
}
