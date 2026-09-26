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
  if(pil==pir)load();
  while(!std::isspace(c)){
    s+=c;
    rd(c);
    if(pil==pir)load();
  }
}
inline void rd(__int128_t&x){
  if(pil+50>pir)load();
  char c;
  do rd(c); while(c<'-');
  bool neg=false;
  if(c=='-'){
    neg=true;
    rd(c);
  }
  x=0;
  while(c>='0'){
    x=x*10+(c&15);
    rd(c);
  }
  if(neg)x=-x;
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
inline void wt(__int128_t x){
  if(por+50>buf_size)flush();
  if(x==0){
    wt('0');
    return;
  }
  if(x<0){
    wt('-');
    x=-x;
  }
  static constexpr __int128_t b=10000000000000000000ull;
  if(x>=b){
    wt<unsigned long long>(x/b);
    unsigned long long y=x%b;
    memcpy(obuf+por,pre.t+((y/10000000000000000)<<2)+1,3);
    memcpy(obuf+por+3,pre.t+((y/1000000000000%10000)<<2),4);
    memcpy(obuf+por+7,pre.t+((y/100000000%10000)<<2),4);
    memcpy(obuf+por+11,pre.t+((y/10000%10000)<<2),4);
    memcpy(obuf+por+15,pre.t+((y%10000)<<2),4);
    por+=19;
  }
  else wt<unsigned long long>(x);
}
struct Dummy{
  Dummy(){std::atexit(flush);}
}dummy;
}
using fastio::rd;
using fastio::wt;
#include<vector>
#include<cmath>
#include<unordered_map>
constexpr bool is_prime_constexpr(int n){
  if(n==998244353||n==1000000007)return true;
  for(int i=2;i*i<=n;i++)if(n%i==0)return false;
  return true;
}
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
#include<limits>
#include<concepts>
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

template<std::integral T>
constexpr T safe_div(T a,T b){return a/b-(a%b&&(a^b)<0);}
template<std::integral T>
constexpr T safe_ceil(T a,T b){return a/b+(a%b&&(a^b)>0);}
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
#include<cassert>
constexpr std::pair<long long,long long>ext_gcd(long long a,long long b){
  if(b==0)return std::make_pair(1,0);
  auto [x,y]=ext_gcd(b,a%b);
  std::swap(x,y);
  return std::make_pair(x,y-a/b*x);
}
constexpr long long inv_mod(long long a,long long p){
  long long b=p,u=1,v=0;
  while(b){
    long long t=a/b;
    a-=t*b;
    u-=t*v;
    std::swap(a,b);
    std::swap(u,v);
  }
  u%=p;
  if(u<0)u+=p;
  return u;
}
#include<cstdint>
std::vector<int>prime_sieve(int n){
  if(n<=6){
    if(n<=1)return std::vector<int>{};
    else if(n==2)return std::vector<int>{2};
    else if(n<=4)return std::vector<int>{2,3};
    else return std::vector<int>{2,3,5};
  }
  static constexpr int mod30table[8]={1,7,11,13,17,19,23,29};
  static constexpr uint8_t k_mask[][8] = {
    {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f},
    {0xfd, 0xdf, 0xef, 0xfe, 0x7f, 0xf7, 0xfb, 0xbf},
    {0xfb, 0xef, 0xfe, 0xbf, 0xfd, 0x7f, 0xf7, 0xdf},
    {0xf7, 0xfe, 0xbf, 0xdf, 0xfb, 0xfd, 0x7f, 0xef},
    {0xef, 0x7f, 0xfd, 0xfb, 0xdf, 0xbf, 0xfe, 0xf7},
    {0xdf, 0xf7, 0x7f, 0xfd, 0xbf, 0xfe, 0xef, 0xfb},
    {0xbf, 0xfb, 0xf7, 0x7f, 0xfe, 0xef, 0xdf, 0xfd},
    {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe},
  };
  static constexpr int c0[][8] = {
    {0, 0, 0, 0, 0, 0, 0, 1}, {1, 1, 1, 0, 1, 1, 1, 1},
    {2, 2, 0, 2, 0, 2, 2, 1}, {3, 1, 1, 2, 1, 1, 3, 1},
    {3, 3, 1, 2, 1, 3, 3, 1}, {4, 2, 2, 2, 2, 2, 4, 1},
    {5, 3, 1, 4, 1, 3, 5, 1}, {6, 4, 2, 4, 2, 4, 6, 1},
  };
  static constexpr int c1[8]={6,4,2,4,2,4,6,2};
  n++;
  int sz=(n+29)/30;
  std::vector<uint8_t>p(sz,0xff);
  {
    int r=n%30;
    if(r==0);
    else if(r==1)p.back()=0x00;
    else if(r<=7)p.back()=0x01;
    else if(r<=11)p.back()=0x03;
    else if(r<=13)p.back()=0x07;
    else if(r<=17)p.back()=0x0f;
    else if(r<=19)p.back()=0x1f;
    else if(r<=23)p.back()=0x3f;
    else if(r<=29)p.back()=0x7f;
  }
  p[0]=0xfe;
  int sq=std::min(sz-1,((int)std::sqrt(n)+29)/30);
  for(int i=0;i<=sq;i++){
    for(uint8_t f=p[i];f>0;f=f&(f-1)){
      uint8_t l=__builtin_ctz(f);
      int m=mod30table[l];
      int pm=i*30+m*2;
      for(int j=i*pm+m*m/30,k=l;j<(int)p.size();j+=i*c1[k]+c0[l][k],k=(k+1)&7){
        p[j]&=k_mask[l][k];
      }
    }
  }
  std::vector<int>res{2,3,5};
  for(int i=0;i<(int)p.size()-1;i++){
    for(int j=p[i];j>0;j=j&(j-1))res.push_back(i*30+mod30table[__builtin_ctz(j)]);
  }
  for(int j=p.back();j>0;j=j&(j-1)){
    int l=__builtin_ctz(j);
    int k=(p.size()-1)*30+mod30table[l];
    if(k<n)res.push_back(k);
    else break;
  }
  return res;
}
std::vector<int>lpf_table(int n){
  std::vector<int>res(n+1,-1);
  std::vector<int>prime=prime_sieve(n);
  for(int p:prime)res[p]=p;
  for(int i=2;i<=n;i++){
    for(int j:prime){
      if(j<=res[i]&&i*j<=n)res[i*j]=j;
      else break;
    }
  }
  return res;
}
int main(){
  int p,g,n;
  rd(p),rd(g),rd(n);
  int s=std::sqrt(p);
  std::vector<int>log(s+1);
  std::unordered_map<int,int>mp;
  std::vector<int>lpf=lpf_table(s);
  mp.reserve(s*5);
  int prod=1;
  int inv_step=pow_mod<int>(g,(long long)(p-2)*s*5%(p-1),p);
  for(int i=0;i<s*5;i++){
    if(mp.contains(prod))break;
    mp[prod]=i;
    prod=(long long)prod*g%p;
  }
  log[0]=-1;
  log[1]=0;
  for(int i=2;i<=s;i++){
    if(lpf[i]!=i){
      log[i]=log[lpf[i]]+log[i/lpf[i]];
      if(log[i]>=p-1)log[i]-=p-1;
    }
    else if(i<100){
      for(int x=i;;x=(long long)x*inv_step%p){
        auto itr=mp.find(x);
        if(itr==mp.end())log[i]+=s*5;
        else{
          log[i]+=itr->second;
          break;
        }
      }
    }
    else{
      for(int k=1,x=(long long)i*g%p;;k++,x=(long long)x*g%p){
        long long now=p-1-k;
        int y=x;
        for(int p:{2,3,5,7,11,13,17,19,23,29}){
          while(y%p==0){
            y/=p;
            now+=log[p];
          }
        }
        if(y>s)continue;
        while(y>=i){
          if(lpf[y]<i){
            now+=log[lpf[y]];
            y/=lpf[y];
          }
          else{
            now=-1;
            break;
          }
        }
        if(now==-1)continue;
        now+=log[y];
        log[i]=now%(p-1);
        break;
      }
    }
  }
  while(n--){
    int x;
    rd(x);
    long long ans=0;
    while(x>s){
      ans+=(p-1)/2-log[p/x];
      x=p%x;
    }
    ans+=log[x];
    ans%=p-1;
    if(ans<0)ans+=p-1;
    wt(ans),wt('\n');
  }
}
