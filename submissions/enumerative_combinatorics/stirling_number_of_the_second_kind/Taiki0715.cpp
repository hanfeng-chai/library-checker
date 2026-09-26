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
#include<array>
#include<numeric>
template<typename T>
constexpr std::enable_if_t<(std::numeric_limits<T>::digits<=32),T>pow_mod(T a,T n,T mod){
  using u64=unsigned long long;
  u64 res=1;
  while(n>0){
    if(n&1)res=((u64)res*a)%mod;
    a=((u64)a*a)%mod;
    n>>=1;
  }
  return T(res);
}
template<typename T>
constexpr std::enable_if_t<(std::numeric_limits<T>::digits>32),T>pow_mod(T a,T n,T mod){
  using u128=__uint128_t;
  u128 res=1;
  while(n>0){
    if(n&1)res=((u128)res*a)%mod;
    a=((u128)a*a)%mod;
    n>>=1;
  }
  return T(res);
}
constexpr int primitive_root_constexpr(int x){
  if(x==167772161)return 3;
  if(x==469762049)return 3;
  if(x==754974721)return 11;
  if(x==880803841)return 26;
  if(x==998244353)return 3;
  if(x==2)return 1;
  int x2=x;
  int p[20];
  int c=0;
  x--;
  for(int i=2;i*i<=x;i++){
    if(x%i==0){
      p[c++]=i;
      while(x%i==0)x/=i;
    }
  }
  if(x!=1)p[c++]=x;
  x=x2;
  for(int g=2;;g++){
    bool ok=true;
    for(int i=0;i<c;i++)if(pow_mod(g,(x-1)/p[i],x)==1){
      ok=false;
      break;
    }
    if(ok)return g;
  }
}
#include<limits>
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
template<int m>
struct ntt_root{
  static constexpr int rank2=lsb(m-1);
  static constexpr int g=primitive_root_constexpr(m);
  std::array<int,rank2+1>root,invroot;
  std::array<int,std::max(0,rank2-1)>rate2,invrate2;
  std::array<int,std::max(0,rank2-2)>rate3,invrate3;
  constexpr ntt_root(){
    root[rank2]=pow_mod(g,m>>rank2,m);
    invroot[rank2]=pow_mod(root[rank2],m-2,m);
    for(int i=rank2-1;i>=0;i--){
      root[i]=(long long)root[i+1]*root[i+1]%m;
      invroot[i]=(long long)invroot[i+1]*invroot[i+1]%m;
    }
    int prod=1,invprod=1;
    for(int i=0;i<rank2-1;i++){
      rate2[i]=(long long)root[i+2]*prod%m;
      invrate2[i]=(long long)invroot[i+2]*invprod%m;
      prod=(long long)prod*invroot[i+2]%m;
      invprod=(long long)invprod*root[i+2]%m;
    }
    prod=invprod=1;
    for(int i=0;i<rank2-2;i++){
      rate3[i]=(long long)root[i+3]*prod%m;
      invrate3[i]=(long long)invroot[i+3]*invprod%m;
      prod=(long long)prod*invroot[i+3]%m;
      invprod=(long long)invprod*root[i+3]%m;
    }
  }
};
template<typename T>
void dft(std::vector<T>&a){
  static constexpr ntt_root<T::mod()>r;
  static constexpr unsigned long long mod2=(unsigned long long)T::mod()*T::mod();
  int n=a.size();
  int h=lsb(n);
  int len=0;
  while(len<h){
    if(h-len==1){
      T rot=T::raw(1);
      for(int s=0;s<(1<<len);s++){
        int of=s*2;
        T u=a[of],v=a[of+1]*rot;
        a[of]=u+v;
        a[of+1]=u-v;
        rot*=T::raw(r.rate2[lsb(~(unsigned int)s)]);
      }
      len++;
    }
    else{
      int p=1<<(h-len-2);
      T rot=T::raw(1),imag=T::raw(r.root[2]);
      for(int s=0;s<(1<<len);s++){
        const unsigned long long rot1=rot.val(),rot2=rot1*rot1%T::mod(),rot3=rot1*rot2%T::mod();
        int of=s<<(h-len);
        for(int i=0;i<p;i++){
          const unsigned long long a0=a[i+of].val(),a1=(unsigned long long)a[i+of+p].val()*rot1,a2=(unsigned long long)a[i+of+p*2].val()*rot2,a3=(unsigned long long)a[i+of+p*3].val()*rot3;
          const unsigned long long m=(unsigned long long)T(a1+mod2-a3).val()*imag.val();
          const unsigned long long k=mod2-a2;
          a[i+of]=a0+a2+a1+a3;
          a[i+of+p]=a0+a2+(mod2*2-a1-a3);
          a[i+of+p*2]=a0+k+m;
          a[i+of+p*3]=a0+k+(mod2-m);
        }
        rot*=T::raw(r.rate3[lsb(~(unsigned int)s)]);
      }
      len+=2;
    }
  }
}
template<typename T>
void idft(std::vector<T>&a){
  static constexpr ntt_root<T::mod()>r;
  static constexpr unsigned long long mod2=(unsigned long long)T::mod()*T::mod();
  int n=a.size();
  int h=lsb(n);
  int len=h;
  while(len){
    if(len==1){
      int p=1<<(h-1);
      for(int i=0;i<p;i++){
        T u=a[i],v=a[i+p];
        a[i]=u+v;
        a[i+p]=u-v;
      }
      len--;
    }
    else{
      int p=1<<(h-len);
      T rot=T::raw(1),imag=T::raw(r.invroot[2]);
      for(int s=0;s<(1<<(len-2));s++){
        const unsigned long long rot1=rot.val(),rot2=rot1*rot1%T::mod(),rot3=rot1*rot2%T::mod();
        int of=s<<(h-len+2);
        for(int i=0;i<p;i++){
          const unsigned long long a0=a[i+of].val(),a1=a[i+of+p].val(),a2=a[i+of+p*2].val(),a3=a[i+of+p*3].val();
          const unsigned long long k=T((T::mod()+a2-a3)*imag.val()).val();
          a[i+of]=a0+a1+a2+a3;
          a[i+of+p]=(a0+T::mod()-a1+k)*rot1;
          a[i+of+p*2]=(a0+a1+T::mod()*2-a2-a3)*rot2;
          a[i+of+p*3]=(a0+T::mod()*2-a1-k)*rot3;
        }
        rot*=T::raw(r.invrate3[lsb(~(unsigned int)s)]);
      }
      len-=2;
    }
  }
}
template<typename T>
std::vector<T>ntt_convolution(std::vector<T> a,std::vector<T> b){
  int n=a.size(),m=b.size(),s=n+m-1;
  if(std::min(n,m)<60){
    std::vector<T>ret(s,0);
    if(n<m)for(int i=0;i<m;i++)for(int j=0;j<n;j++)ret[i+j]+=a[j]*b[i];
    else for(int i=0;i<n;i++)for(int j=0;j<m;j++)ret[i+j]+=a[i]*b[j];
    return ret;
  }
  int z=ceil_pow2(s);
  a.resize(z,0);
  b.resize(z,0);
  dft(a),dft(b);
  std::vector<T>c(z);
  for(int i=0;i<z;i++)c[i]=a[i]*b[i];
  idft(c);
  T g=T::raw(z).inv();
  for(int i=0;i<s;i++)c[i]*=g;
  return {c.begin(),c.begin()+s};
}
#include<initializer_list>
template<typename T>
struct F{
private:
  static int capacity;
  static std::vector<T>fact,factinv,inv;
public:
  static void resize(int n){
    if(capacity>=n)return;
    fact.resize(n+1),factinv.resize(n+1),inv.resize(n+1);
    for(int i=capacity+1;i<=n;i++){
      fact[i]=fact[i-1]*i;
      inv[i]=-inv[T::mod()%i]*(T::mod()/i);
      factinv[i]=factinv[i-1]*inv[i];
    }
    capacity=n;
  }
  static T C(int n,int k){
    if(n<k)return 0;
    if(k<0)return 0;
    resize(n);
    return fact[n]*factinv[k]*factinv[n-k];
  }
  static T P(int n,int k){
    if(n<k)return 0;
    if(k<0)return 0;
    resize(n);
    return fact[n]*factinv[n-k];
  }
  static T H(int n,int k){
    if(n==0&&k==0)return 1;
    return C(n+k-1,k);
  }
  static T factorial(int n){
    resize(n);
    return fact[n];
  }
  static T factorial_inv(int n){
    resize(n);
    return factinv[n];
  }
  static T S(long long n,int k){
    if(n<0)return 0;
    if(n<k)return 0;
    T ret=0;
    resize(k);
    for(int i=0;i<=k;i++){
      ret+=fact[k]*factinv[i]*factinv[k-i]*T::raw(i).pow(n)*((k-i)&1?-1:1);
    }
    return ret*factinv[k];
  }
  template<typename... INT>
  static T O(INT...k){
    int n=0;
    for(int i:std::initializer_list<int>{k...}){
      if(i<0)return 0;
      n+=i;
    }
    resize(n);
    T ret=fact[n];
    for(int i:std::initializer_list<int>{k...})ret*=factinv[i];
    return ret;
  }
};
template<typename T>int F<T>::capacity=1;
template<typename T>std::vector<T>F<T>::fact{1,1};
template<typename T>std::vector<T>F<T>::factinv{1,1};
template<typename T>std::vector<T>F<T>::inv{0,1};
#include<functional>
template<typename T,typename Func>
std::vector<T>enumerate_fully_multiplicative_function(int n,const Func&f){
  static_assert(std::is_convertible_v<Func,std::function<T(int)>>);
  std::vector<int>prime,lpf(n+1,-1);
  std::vector<T>res(n+1);
  res[0]=f(0),res[1]=f(1);
  for(int i=2;i<=n;i++){
    if(lpf[i]==-1){
      lpf[i]=i;
      prime.push_back(i);
      res[i]=f(i);
    }
    for(int j:prime){
      if(j<=lpf[i]&&i*j<=n){
        lpf[i*j]=j;
        res[i*j]=res[i]*res[j];
      }
      else break;
    }
  }
  return res;
}
template<typename T>
std::vector<T>stirling2_fixed_n(int n){
  int s=ceil_pow2(n*2+1);
  std::vector<T>f=enumerate_fully_multiplicative_function<T>(n,[&n](int x)->T {return T::raw(x).pow(n);});
  f.resize(s);
  std::vector<T>g(s);
  for(int i=0;i<=n;i++){
    T finv=F<T>::factorial_inv(i);
    f[i]*=finv;
    g[i]=(i&1)?-finv:finv;
  }
  T inv=T::raw(s).inv();
  dft(f),dft(g);
  for(int i=0;i<s;i++)f[i]*=g[i];
  idft(f);
  f.resize(n+1);
  for(int i=0;i<=n;i++)f[i]*=inv;
  return f;
}
#include<iostream>
#include<cassert>
#include<optional>
#include<algorithm>
#include<stack>
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
template<typename T>
std::enable_if_t<std::is_integral_v<T>,T>carmichael(T n){
  auto f=factorize(n);
  std::sort(f.begin(),f.end());
  T res=1;
  for(int l=0,r=0;l<f.size();l=r){
    while(r<f.size()&&f[l]==f[r])r++;
    if(f[l]==2){
      if(r-l==2)res=2;
      else if(r-l>=3)res=T(1)<<(r-l-2);
    }
    else{
      T prod=f[l]-1;
      for(int i=0;i<r-l-1;i++)prod*=f[l];
      res=std::lcm(res,prod);
    }
  }
  return res;
}
constexpr int carmichael_constexpr(int n){
  if(n==998244353)return 998244352;
  if(n==1000000007)return 1000000006;
  if(n<=1)return n;
  int res=1;
  int t=0;
  while(n%2==0){
    n/=2;
    t++;
  }
  if(t==2)res=2;
  else if(t>=3)res=1<<(t-2);
  for(int i=3;i*i<=n;i++)if(n%i==0){
    int c=0;
    while(n%i==0){
      n/=i;
      c++;
    }
    int prod=i-1;
    for(int j=0;j<c-1;j++)prod*=i;
    res=std::lcm(res,prod);
  }
  if(n!=1)res=std::lcm(res,n-1);
  return res;
}
template<int m>
struct mod_int{
private:
  static constexpr unsigned int umod=static_cast<unsigned int>(m);
  static constexpr unsigned int car=carmichael_constexpr(m);
  using uint=unsigned int;
  using mint=mod_int;
  uint v;
  static_assert(m<uint(1)<<31);
  mint sqrt_impl()const{
    if(this->val()<=1)return *this;
    if constexpr(m%8==1){
      mint b=2;
      while(b.pow((m-1)/2).val()==1)b++;
      int m2=m-1,e=0;
      while(m2%2==0)m2>>=1,e++;
      mint x=this->pow((m2-1)/2);
      mint y=(*this)*x*x;
      x*=*this;
      mint z=b.pow(m2);
      while(y.val()!=1){
        int j=0;
        mint t=y;
        while(t.val()!=1)t*=t,j++;
        z=z.pow(1<<(e-j-1));
        x*=z;
        z*=z;
        y*=z;e=j;
      }
      return x;
    }
    else if constexpr(m%8==5){
      mint ret=this->pow((m+3)/8);
      if((ret*ret).val()==this->val())return ret;
      else return ret*mint::raw(2).pow((m-1)/4);
    }
    else{
      return this->pow((m+1)/4);
    }
  }
public:
  using value_type=uint;
  mod_int():v(0){}
  template<typename T,std::enable_if_t<std::is_signed_v<T>,std::nullptr_t> =nullptr>
  mod_int(T a){
    a%=m;
    if(a<0)v=a+umod;
    else v=a;
  }
  template<typename T,std::enable_if_t<std::is_unsigned_v<T>,std::nullptr_t> =nullptr>
  mod_int(T a):v(a%umod){}
  static constexpr mint raw(int a){
    mint ret;
    ret.v=a;
    return ret;
  }
  inline uint val()const{return this->v;}
  static constexpr int mod(){return m;}
  inline mint &operator+=(const mint &b){
    this->v+=b.v;
    if(this->v>=umod)this->v-=umod;
    return *this;
  }
  inline mint &operator-=(const mint &b){
    this->v-=b.v;
    if(this->v>=umod)this->v+=umod;
    return *this;
  }
  inline mint &operator*=(const mint &b){
    this->v=((unsigned long long)this->v*b.v)%umod;
    return *this;
  }
  inline mint &operator/=(const mint &b){
    *this*=b.inv();
    return *this;
  }
  inline mint operator+()const{return *this;}
  inline mint operator-()const{return mint()-*this;}
  friend inline mint operator+(const mint &a,const mint &b){return mint(a)+=b;}
  friend inline mint operator-(const mint &a,const mint &b){return mint(a)-=b;}
  friend inline mint operator*(const mint &a,const mint &b){return mint(a)*=b;}
  friend inline mint operator/(const mint &a,const mint &b){return mint(a)/=b;}
  friend inline bool operator==(const mint &a,const mint &b){return a.val()==b.val();}
  friend inline bool operator!=(const mint &a,const mint &b){return !(a==b);}
  inline mint operator++(int){
    mint ret=*this;
    *this+=1;
    return ret;
  }
  inline mint operator--(int){
    mint ret=*this;
    *this-=1;
    return ret;
  }
  mint pow(long long n)const{
    mint ret=mint::raw(1),a(*this);
    while(n){
      if(n&1)ret*=a;
      a*=a;
      n>>=1;
    }
    return ret;
  }
  inline mint inv()const{
    assert(this->v!=0);
    return pow(car-1);
  }
  std::optional<mint>sqrt()const{
    if(this->val()<=1||this->pow((m-1)/2)==1)return std::make_optional(this->sqrt_impl());
    else return std::nullopt;
  }
  static constexpr unsigned int order(){return car;}
  friend std::istream &operator>>(std::istream &is,mint &b){
    long long a;
    is>>a;
    b=mint(a);
    return is;
  }
  friend std::ostream &operator<<(std::ostream &os,const mint &b){
    os<<b.val();
    return os;
  }
};
template<int m>
struct std::hash<mod_int<m>>{
  std::size_t operator()(mod_int<m>x)const{
    return std::hash<unsigned int>()(x.val());
  }
};
using mint998=mod_int<998244353>;
using mint107=mod_int<1000000007>;
using mint=mint998;
int main(){
  int n;
  rd(n);
  auto ans=stirling2_fixed_n<mint>(n);
  for(int i=0;i<=n;i++)wt(ans[i].val()),wt(" \n"[i==n]);
}
