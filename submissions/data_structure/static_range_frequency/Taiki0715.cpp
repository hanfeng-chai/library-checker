#include<stdio.h>
#include<string.h>
#include<string>
#include<cstdlib>
#include<type_traits>
namespace fastio{
static constexpr u_int32_t buf_size=1<<17;
char ibuf[buf_size];
char obuf[buf_size];
u_int32_t pil=0,pir=0,por=0;
struct Pre{
  char t[40000];
  constexpr Pre(){
    for(int i=0;i<10000;i++){
      int n=i;
      for(int j=3;j>=0;j--){
        t[i*4+j]='0'+n%10;
        n/=10;
      }
    }
  }
}constexpr pre;
inline void load(){
  if(pir-pil<=pil)memcpy(ibuf,ibuf+pil,pir-pil);
  else memmove(ibuf,ibuf+pil,pir-pil);
  pir=pir-pil+fread(ibuf+pir-pil,1,buf_size-pir+pil,stdin);
  pil=0;
}
inline void flush(){
  fwrite(obuf,1,por,stdout);
  por=0;
}
inline void rd(char&c){c=ibuf[pil++];}
inline void rd(std::string&s){
  char c;
  s.clear();
  if(pil==pir)load();
  rd(c);
  while(!std::isspace(c)){
    s+=c;
    rd(c);
    if(pil==pir)load();
  }
}
template<typename T>
inline void rd(T&x){
  if(pil+32>pir)load();
  char c;
  do rd(c); while(c<'-');
  bool neg=false;
  if constexpr(std::is_signed_v<T>){
    if(c=='-'){
      neg=true;
      rd(c);
    }
  }
  x=0;
  while(c>='0'){
    x=x*10+(c&15);
    rd(c);
  }
  if constexpr(std::is_signed_v<T>){
    if(neg)x=-x;
  }
}
inline void wt(char x){obuf[por++]=x;}
template<typename T,std::enable_if_t<std::is_integral_v<T>,std::nullptr_t> =nullptr>
inline void wt(T x){
  if(por+32>buf_size)flush();
  if(x==0){
    wt('0');
    return;
  }
  if constexpr(std::is_signed_v<T>){
    if(x<0){
      wt('-');
      x=-x;
    }
  }
  if(x>=10000000000000000){
    u_int32_t r1=x%100000000;
    u_int64_t q1=x/100000000;
    u_int32_t r2=q1%100000000;
    u_int32_t q2=q1/100000000;
    u_int32_t n1=r1%10000,n2=r1/10000,n3=r2%10000,n4=r2/10000;
    if(x>=1000000000000000000){
      if constexpr(std::is_unsigned_v<T>){
        if(x>=10000000000000000000ull){
          obuf[por++]='1';
        }
      }
      memcpy(obuf+por,pre.t+(q2<<2)+1,3);
      memcpy(obuf+por+3,pre.t+(n4<<2),4);
      memcpy(obuf+por+7,pre.t+(n3<<2),4);
      memcpy(obuf+por+11,pre.t+(n2<<2),4);
      memcpy(obuf+por+15,pre.t+(n1<<2),4);
      por+=19;
    }
    else if(x>=100000000000000000){
      u_int32_t q3=(q2*205)>>11;
      u_int32_t r3=q2-q3*10;
      obuf[por]='0'+q3;
      obuf[por+1]='0'+r3;
      memcpy(obuf+por+2,pre.t+(n4<<2),4);
      memcpy(obuf+por+6,pre.t+(n3<<2),4);
      memcpy(obuf+por+10,pre.t+(n2<<2),4);
      memcpy(obuf+por+14,pre.t+(n1<<2),4);
      por+=18;
    }
    else{
      obuf[por]='0'+q2;
      memcpy(obuf+por+1,pre.t+(n4<<2),4);
      memcpy(obuf+por+5,pre.t+(n3<<2),4);
      memcpy(obuf+por+9,pre.t+(n2<<2),4);
      memcpy(obuf+por+13,pre.t+(n1<<2),4);
      por+=17;
    }
  }
  else{
    static char buf[12];
    int i=8;
    while(x>=10000){
      memcpy(buf+i,pre.t+((x%10000)<<2),4);
      x/=10000;
      i-=4;
    }
    if(x<100){
      if(x<10)obuf[por++]='0'+x;
      else{
        obuf[por]='0'+x/10;
        obuf[por+1]='0'+x%10;
        por+=2;
      }
    }
    else{
      if(x<1000){
        memcpy(obuf+por,pre.t+(x<<2)+1,3);
        por+=3;
      }
      else{
        memcpy(obuf+por,pre.t+(x<<2),4);
        por+=4;
      }
    }
    memcpy(obuf+por,buf+i+4,8-i);
    por+=8-i;
  }
}
inline void wt(const std::string&s){
  int sz=s.size();
  if(por+sz>buf_size)flush();
  memcpy(obuf+por,s.c_str(),sz);
  por+=sz;
}
struct Dummy{
  Dummy(){std::atexit(flush);}
}dummy;
}
using fastio::rd;
using fastio::wt;
#include<vector>
#include<limits>
#include<random>
namespace Random{
  std::mt19937_64 mt(std::random_device{}());
  template<typename T>inline std::enable_if_t<std::is_integral_v<T>,T> get(){return mt();}
  template<typename T>inline std::enable_if_t<std::is_integral_v<T>,T>range(T n){return mt()%n;}
  template<typename T>inline std::enable_if_t<std::is_integral_v<T>,T>range(T l,T r){return l+mt()%(r-l);}
  template<typename T>inline std::enable_if_t<std::is_integral_v<T>,std::pair<T,T>>distinct(T n){
    T l=mt()%n,r=mt()%(n-1);
    if(l==r)r++;
    if(l>r)std::swap(l,r);
    return std::make_pair(l,r);
  }
}
#include<stack>
template<typename T>
constexpr std::enable_if_t<std::numeric_limits<T>::digits<=32,int>msb(T n){return n==0?-1:31-__builtin_clz(n);}
template<typename T>
constexpr std::enable_if_t<(std::numeric_limits<T>::digits>32),int>msb(T n){return n==0?-1:63-__builtin_clzll(n);}

template<typename T>
constexpr std::enable_if_t<std::numeric_limits<T>::digits<=32,int>lsb(T n){return n==0?-1:__builtin_ctz(n);}
template<typename T>
constexpr std::enable_if_t<(std::numeric_limits<T>::digits>32),int>lsb(T n){return n==0?-1:__builtin_ctzll(n);}

template<typename T>
constexpr std::enable_if_t<std::is_integral_v<T>,T>floor_pow2(T n){return n==0?0:T(1)<<msb(n);}

template<typename T>
constexpr std::enable_if_t<std::is_integral_v<T>,T>ceil_pow2(T n){return n<=1?1:T(1)<<(msb(n-1)+1);}
constexpr unsigned long long binary_gcd(unsigned long long a,unsigned long long b){
  if(a==0||b==0||a==b)return a<b?b:a;
  int n=lsb(a),m=lsb(b);
  while(a!=b){
    if(a>b)a=(a-b)>>lsb(a-b);
    else b=(b-a)>>lsb(b-a);
  }
  return a<<(n<m?n:m);
}
#include<utility>
struct BarrettReduction{
private:
  using i64=long long;
  using u64=unsigned long long;
  using u32=unsigned int;
  using u128=__uint128_t;
  u32 m;
  u64 im;
public:
  BarrettReduction():m(0),im(0){}
  BarrettReduction(u32 n):m(n),im(u64(-1)/n+1){}
  inline i64 quo(u64 x)const{
    if(m==1)return x;
    u64 y=u64((u128(x)*im)>>64);
    u32 r=x-y*m;
    return m<=r?y-1:y;
  }
  inline u32 rem(u64 x)const{
    if(m==1)return 0;
    u64 y=u64((u128(x)*im)>>64);
    u32 r=x-y*m;
    return m<=r?r+m:r;
  }
  inline std::pair<u64,u32>quo_rem(u64 x)const{
    if(m==0)return std::make_pair(x,0);
    u64 y=u64((u128(x)*im)>>64);
    u32 r=x-y*m;
    return m<=r?std::make_pair(y-1,r+m):std::make_pair(y,r);
  }
  inline u32 pow(u32 a,u64 p)const{
    u32 res=m!=1;
    while(p){
      if(p&1)res=rem(u64(res)*a);
      a=rem(u64(a)*a);
      p>>=1;
    }
    return res;
  }
};
namespace prime_impl{
constexpr int table_size=1<<16;
bool table[table_size];
struct prime_table_init{
  prime_table_init(){
    table[0]=table[1]=true;
    for(int i=2;i<table_size;i++)if(!table[i]){
      for(int j=i*2;j<table_size;j+=i)table[j]=true;
    }
  }
}dummy;
}
bool isprime(unsigned long long n)noexcept{
  if(n<prime_impl::table_size)return !prime_impl::table[n];
  if(n%2==0)return false;
  if(n<(1ull<<31)){
    BarrettReduction br(n);
    unsigned long long d=n-1;
    while(!(d&1))d>>=1;
    for(unsigned long long base:{2,7,61}){
      unsigned long long t=d;
      unsigned long long y=1;
      while(t>0){
        if(t&1)y=br.rem(y*base);
        base=br.rem(base*base);
        t>>=1;
      }
      t=d;
      while(t!=n-1&&y!=1&&y!=n-1){
        y=br.rem(y*y);
        t<<=1;
      }
      if(y!=n-1&&t%2==0)return false;
    }
    return true;
  }
  unsigned long long d=n-1;
  int s=0;
  while(!(d&1))d>>=1,s++;
  int q=63;
  while(!(d>>q))q--;
  unsigned long long r=n;
  for(int i=0;i<5;i++)r*=2-r*n;
  auto redc=[&r,&n](__uint128_t x)->unsigned long long {
    x=(x+__uint128_t((unsigned long long)x*-r)*n)>>64;
    return x>=n?x-n:x;
  };
  __uint128_t r2=-__uint128_t(n)%n;
  unsigned long long one=redc(r2);
  for(unsigned long long base:{2,325,9375,28178,450775,9780504,1795265022}){
    if(base%n==0)continue;
    unsigned long long a=base=redc((base%n)*r2);
    for(int i=q-1;i>=0;i--){
      a=redc(__uint128_t(a)*a);
      if(d>>i&1)a=redc(__uint128_t(a)*base);
    }
    if(a==one)continue;
    for(int i=1;a!=n-one;i++){
      if(i>=s)return false;
      a=redc(__uint128_t(a)*a);
    }
  }
  return true;
}
std::vector<unsigned long long>factorize(unsigned long long n)noexcept{
  std::vector<unsigned long long>ret;
  auto div=[](unsigned long long x)noexcept->unsigned long long {
    unsigned long long r=x;
    for(int i=0;i<5;i++)r*=2-r*x;
    unsigned long long r2=-__uint128_t(x)%x;
    auto redc=[&r,&x](__uint128_t t)->unsigned long long {
      t=(t+__uint128_t((unsigned long long)t*-r)*x)>>64;
      return t>=x?t-x:t;
    };
    unsigned long long a=0,b=0;
    const unsigned long long one=redc(r2);
    unsigned long long e=one;
    int m=1ll<<((63-__builtin_clzll(x))>>3);
    while(true){
      unsigned long long ca=a,cb=b;
      unsigned long long sk=one;
      for(int i=0;i<m;i++){
        a=redc(__uint128_t(a)*a+e);
        b=redc(__uint128_t(b)*b+e);
        b=redc(__uint128_t(b)*b+e);
        unsigned long long c=redc(a),d=redc(b);
        sk=redc(__uint128_t(sk)*(c>d?c-d:d-c));
      }
      unsigned long long g=binary_gcd(redc(sk),x);
      if(g>1){
        if(g<x)return g;
        for(int i=0;i<m;i++){
          ca=redc(__uint128_t(ca)*ca+e);
          cb=redc(__uint128_t(cb)*cb+e);
          cb=redc(__uint128_t(cb)*cb+e);
          unsigned long long c=redc(ca),d=redc(cb);
          unsigned long long cg=binary_gcd(c>d?c-d:d-c,x);
          if(cg>1){
            if(cg<x)return cg;
            else{
              e+=one;
              a=b=0;
              break;
            }
          }
        }
      }
    }
  };
  static unsigned long long st[64];
  int p=0;
  while(!(n&1)){
    n>>=1;
    ret.push_back(2);
  }
  if(n==1)return ret;
  st[p++]=n;
  while(p){
    unsigned long long now=st[--p];
    if(isprime(now)){
      ret.push_back(now);
      continue;
    }
    unsigned long long d=div(now);
    st[p++]=d;
    now/=d;
    if(now!=1)st[p++]=now;
  }
  return ret;
}
namespace Random{
template<typename T>
inline std::enable_if_t<std::is_integral_v<T>,T>prime(T l,T r){
  T res=0;
  while(!isprime(res))res=range(l,r);
  return res;
}
}
template<typename Key,typename Val>
struct HashMap{
  static_assert(std::is_integral_v<Key>);
private:
  using uKey=std::make_unsigned_t<Key>;
  const int p;
  std::vector<uKey>key;
  std::vector<Val>value;
  BarrettReduction brp,brp1;
  static constexpr uKey none=std::numeric_limits<uKey>::max();
public:
  HashMap(){}
  HashMap(int n):p(Random::prime(n*4/3,n*4/3+1000)){
    key.resize(p,none);
    value.resize(p);
    brp=BarrettReduction(p);
    brp1=BarrettReduction(p-1);
  }
  Val get(const Key&k,Val def=Val())const{
    int now=brp.rem(k);
    int of=brp1.rem(k)+1;
    while(key[now]!=none&&key[now]!=k){
      now+=of;
      if(now>=p)now-=p;
    }
    return key[now]==none?def:value[now];
  }
  Val& set(const Key&k){
    int now=brp.rem(k);
    int of=brp1.rem(k)+1;
    while(key[now]!=none&&key[now]!=k){
      now+=of;
      if(now>=p)now-=p;
    }
    key[now]=k;
    return value[now];
  }
};
using namespace std;
struct Query{
  int l,r,x;
};
int main(){
  int n,q;
  rd(n),rd(q);
  vector<int>a(n);
  for(int i=0;i<n;i++)rd(a[i]);
  vector<int>idx(q*2),ptr(n+1);
  vector<Query>query(q);
  for(int i=0;i<q;i++){
    rd(query[i].l),rd(query[i].r),rd(query[i].x);
    if(query[i].l==query[i].r)continue;
    if(query[i].l!=0)ptr[query[i].l-1]++;
    ptr[query[i].r-1]++;
  }
  for(int i=1;i<=n;i++)ptr[i]+=ptr[i-1];
  for(int i=0;i<q;i++){
    if(query[i].l==query[i].r)continue;
    if(query[i].l!=0)idx[--ptr[query[i].l-1]]=~i;
    idx[--ptr[query[i].r-1]]=i;
  }
  vector<int>ans(q);
  HashMap<int,int>cnt(n);
  for(int i=0;i<n;i++){
    cnt.set(a[i])++;
    for(int j=ptr[i];j<ptr[i+1];j++){
      int id=idx[j];
      if(id>=0)ans[id]+=cnt.get(query[id].x);
      else ans[~id]-=cnt.get(query[~id].x);
    }
  }
  for(int i=0;i<q;i++)wt(ans[i]),wt('\n');
}
