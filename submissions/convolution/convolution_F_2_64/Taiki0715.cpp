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
#include<iostream>
#include<concepts>
#include<immintrin.h>
struct gf2{
private:
  unsigned long long v;
  static constexpr unsigned long long table[16]={0,27,45,54,90,65,119,108,175,180,130,153,245,238,216,195};
public:
  gf2():v(){}
  template<std::integral T>
  gf2(T x):v(x){}
  inline gf2 operator+()const{return *this;}
  inline gf2 operator-()const{return *this;}
  inline gf2 &operator+=(const gf2&rhs){v^=rhs.v;return *this;}
  inline gf2 &operator-=(const gf2&rhs){v^=rhs.v;return *this;}
  __attribute__((target("pclmul,sse4.2")))
  inline gf2 &operator*=(const gf2&rhs){
    __m128i m=_mm_clmulepi64_si128(_mm_set_epi64x(0,v),_mm_set_epi64x(0,rhs.v),0);
    unsigned long long high=_mm_extract_epi64(m,1);
    unsigned long long low=_mm_extract_epi64(m,0);
    v=high^(high<<1);
    low^=v^(v<<3);
    v=low^table[high>>60];
    return *this;
  }
  inline gf2 &operator/=(const gf2&rhs){return *this*=rhs.inv();}
  friend gf2 operator+(const gf2&lhs,const gf2&rhs){return gf2(lhs)+=rhs;}
  friend gf2 operator-(const gf2&lhs,const gf2&rhs){return gf2(lhs)-=rhs;}
  friend gf2 operator*(const gf2&lhs,const gf2&rhs){return gf2(lhs)*=rhs;}
  friend gf2 operator/(const gf2&lhs,const gf2&rhs){return gf2(lhs)/=rhs;}
  auto operator<=>(const gf2&)const=default;
  gf2 pow(unsigned long long k)const{
    gf2 res=1,a=*this;
    while(k){
      if(k&1)res*=a;
      a*=a;
      k>>=1;
    }
    return res;
  }
  inline gf2 inv()const{return pow(-2);}
  unsigned long long val()const{return v;}
  friend std::istream &operator>>(std::istream&is,gf2&rhs){is>>rhs.v;return is;}
  friend std::ostream &operator<<(std::ostream&os,const gf2&rhs){os<<rhs.v;return os;}
};
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

template<std::integral T>
constexpr T safe_div(T a,T b){return a/b-(a%b&&(a^b)<0);}
template<std::integral T>
constexpr T safe_ceil(T a,T b){return a/b+(a%b&&(a^b)>0);}
namespace nim_convolution_impl{
struct nim_fft_data{
  std::vector<std::vector<gf2>>a;
  void init(int n){
    if((int)a.size()>n)return;
    a.resize(n+1);
    std::vector<gf2>b;
    b.push_back(2);
    while(true){
      gf2 x=b.back();
      if(x.val()==0)break;
      b.push_back(x*x+x);
    }
    b.pop_back();
    b.erase(b.begin(),b.end()-n);
    for(int i=n;;i--){
      std::vector<gf2>&na=a[i];
      na.resize(1<<i);
      gf2 inv=b.back().inv();
      for(gf2&x:b)x*=inv;
      for(int j=0;j<(1<<i);j++){
        for(int k=0;k<i;k++)if(j>>k&1)na[j]+=b[k];
      }
      if(i==0)break;
      b.pop_back();
      for(gf2&x:b)x=x*x+x;
    }
  }
}nim_data;
void nim_fft(std::vector<gf2>&f){
  int n=f.size();
  std::vector<gf2>f2(n);
  int len=n;
  while(len>1){
    for(int l=0;l<n;l+=len){
      for(int m=len/4;m>=1;m/=2){
        for(int t=0;t<len;t+=m*4){
          for(int i=0;i<m;i++){
            gf2 b=f[l+t+m+i],c=f[l+t+m*2+i],d=f[l+t+m*3+i];
            f[l+t+m+i]=b+c+d;
            f[l+t+m*2+i]=c+d;
          }
        }
      }
      for(int i=0;i<len/2;i++){
        f2[l+i]=f[l+i*2];
        f2[l+i+len/2]=f[l+i*2+1];
      }
    }
    std::swap(f,f2);
    len/=2;
  }
  while(len<n){
    len*=2;
    const std::vector<gf2>&g=nim_data.a[msb(len)];
    for(int l=0;l<n;l+=len){
      for(int i=0;i<len/2;i++){
        f2[l+i]=f[l+i]+f[l+i+len/2]*g[i];
        f2[l+i+len/2]=f[l+i]+f[l+i+len/2]*g[i+len/2];
      }
    }
    std::swap(f,f2);
  }
}
void nim_ifft(std::vector<gf2>&f){
  int n=f.size();
  std::vector<gf2>f2(n);
  int len=n;
  while(len>1){
    const std::vector<gf2>&g=nim_data.a[msb(len)];
    for(int l=0;l<n;l+=len){
      for(int i=0;i<len/2;i++){
        f2[l+i]=f[l+i]*g[i+len/2]+f[l+i+len/2]*g[i];
        f2[l+i+len/2]=f[l+i]+f[l+i+len/2];
      }
    }
    std::swap(f,f2);
    len/=2;
  }
  while(len<n){
    len*=2;
    for(int l=0;l<n;l+=len){
      for(int i=0;i<len/2;i++){
        f2[l+i*2]=f[l+i];
        f2[l+i*2+1]=f[l+i+len/2];
      }
    }
    std::swap(f,f2);
    for(int l=0;l<n;l+=len){
      for(int m=1;m<=len/4;m*=2){
        for(int t=0;t<len;t+=m*4){
          for(int i=0;i<m;i++){
            gf2 b=f[l+t+m+i],c=f[l+t+m*2+i],d=f[l+t+m*3+i];
            f[l+t+m+i]=b+c;
            f[l+t+m*2+i]=c+d;
          }
        }
      }
    }
  }
}
std::vector<gf2>nim_convolution(std::vector<gf2>f,std::vector<gf2>g){
  int n=f.size(),m=g.size();
  int s=ceil_pow2(n+m-1);
  f.resize(s),g.resize(s);
  nim_data.init(msb(s));
  nim_fft(f),nim_fft(g);
  for(int i=0;i<s;i++)f[i]*=g[i];
  nim_ifft(f);
  f.resize(n+m-1);
  return f;
}
}
using nim_convolution_impl::nim_convolution;
int main(){
  int n,m;
  rd(n),rd(m);
  std::vector<gf2>a(n),b(m);
  for(int i=0;i<n;i++){
    unsigned long long x;
    rd(x);
    a[i]=x;
  }
  for(int i=0;i<m;i++){
    unsigned long long x;
    rd(x);
    b[i]=x;
  }
  a=nim_convolution(std::move(a),std::move(b));
  for(int i=0;i<n+m-1;i++)wt(a[i].val()),wt(" \n"[i+1==n+m-1]);
}
