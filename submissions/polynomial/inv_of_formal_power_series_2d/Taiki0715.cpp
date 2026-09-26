#include<stdio.h>
#include<string.h>
#include<string>
#include<cstdlib>
#include<cstdint>
#include<type_traits>
namespace fastio{
static constexpr uint32_t buf_size=1<<17;
char ibuf[buf_size];
char obuf[buf_size];
uint32_t pil=0,pir=0,por=0;
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
    uint32_t r1=x%100000000;
    uint64_t q1=x/100000000;
    uint32_t r2=q1%100000000;
    uint32_t q2=q1/100000000;
    uint32_t n1=r1%10000,n2=r1/10000,n3=r2%10000,n4=r2/10000;
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
      uint32_t q3=(q2*205)>>11;
      uint32_t r3=q2-q3*10;
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
#define NTT_SIMD
#include<vector>
#include<immintrin.h>
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
  int p[20]={};
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
template<unsigned m>
struct Montgomery{
  static_assert(m%2==1);
  static_assert(m>=3);
  static constexpr unsigned r=[](){
    unsigned res=m;
    for(int i=0;i<4;i++)res*=2-m*res;
    return res;
  }();
  static constexpr unsigned r2=-((unsigned long long)m)%m;
  static constexpr unsigned reduce(unsigned long long x){
    return (x+(unsigned long long)((unsigned)x*(unsigned)(-r))*m)>>32;
  }
  static constexpr unsigned mod(){return m;}
};
namespace ntt_simd_impl{
alignas(32) unsigned b[1<<24];
template<typename MM>
__attribute__((target("avx2")))
inline __m256i add_simd(__m256i x,__m256i y){
  x=_mm256_add_epi32(x,y);
  x=_mm256_sub_epi32(x,_mm256_set1_epi32(MM::mod()*2));
  __m256i mask=_mm256_cmpgt_epi32(_mm256_set1_epi32(0),x);
  x=_mm256_add_epi32(x,_mm256_and_si256(_mm256_set1_epi32(MM::mod()*2),mask));
  return x;
}
template<typename MM>
__attribute__((target("avx2")))
inline __m256i sub_simd(__m256i x,__m256i y){
  x=_mm256_sub_epi32(x,y);
  __m256i mask=_mm256_cmpgt_epi32(_mm256_set1_epi32(0),x);
  x=_mm256_add_epi32(x,_mm256_and_si256(_mm256_set1_epi32(MM::mod()*2),mask));
  return x;
}
template<typename MM>
__attribute__((target("avx2")))
inline void mul_simd(__m256i&x,int y){
  __m256i x0246=_mm256_mul_epu32(x,_mm256_set1_epi32(y));
  __m256i x0246_2=_mm256_mul_epu32(x0246,_mm256_set1_epi32(-MM::r));
  x0246_2=_mm256_mul_epu32(x0246_2,_mm256_set1_epi32(MM::mod()));
  x0246=_mm256_add_epi64(x0246,x0246_2);
  __m256i x1357=_mm256_mul_epu32(_mm256_srli_epi64(x,32),_mm256_set1_epi32(y));
  __m256i x1357_2=_mm256_mul_epu32(x1357,_mm256_set1_epi32(-MM::r));
  x1357_2=_mm256_mul_epu32(x1357_2,_mm256_set1_epi32(MM::mod()));
  x1357=_mm256_add_epi64(x1357,x1357_2);
  x=_mm256_blend_epi32(_mm256_srli_epi64(x0246,32),x1357,0b10101010);
}
template<typename MM>
__attribute__((target("avx2")))
inline void mul_simd(__m256i&x,const __m256i&y){
  __m256i x0246=_mm256_mul_epu32(x,y);
  __m256i x0246_2=_mm256_mul_epu32(x0246,_mm256_set1_epi32(-MM::r));
  x0246_2=_mm256_mul_epu32(x0246_2,_mm256_set1_epi32(MM::mod()));
  x0246=_mm256_add_epi64(x0246,x0246_2);
  __m256i x1357=_mm256_mul_epu32(_mm256_srli_epi64(x,32),_mm256_srli_epi64(y,32));
  __m256i x1357_2=_mm256_mul_epu32(x1357,_mm256_set1_epi32(-MM::r));
  x1357_2=_mm256_mul_epu32(x1357_2,_mm256_set1_epi32(MM::mod()));
  x1357=_mm256_add_epi64(x1357,x1357_2);
  x=_mm256_blend_epi32(_mm256_srli_epi64(x0246,32),x1357,0b10101010);
}
template<typename T>
__attribute__((target("avx2")))
void dft_simd(std::vector<T>&a){
  using MM=Montgomery<T::mod()>;
  static constexpr ntt_root<T::mod()>r;
  static constexpr std::array<unsigned,r.rate2.size()>rate2_m=[](){
    std::array<unsigned,r.rate2.size()>res;
    for(int i=0;i<(int)res.size();i++)res[i]=MM::reduce((unsigned long long)r.rate2[i]*MM::r2);
    return res;
  }();
  static constexpr std::array<unsigned,r.rate3.size()>rate3_m=[](){
    std::array<unsigned,r.rate3.size()>res;
    for(int i=0;i<(int)res.size();i++)res[i]=MM::reduce((unsigned long long)r.rate3[i]*MM::r2);
    return res;
  }();
  alignas(32) static constexpr std::array<unsigned,(r.rank2-3)*8>rate4_m=[](){
    std::array<unsigned,(r.rank2-3)*8>res;
    unsigned prod=1;
    for(int i=0;i<=r.rank2-4;i++){
      unsigned v=(unsigned long long)r.root[i+4]*prod%MM::mod();
      v=MM::reduce((unsigned long long)v*MM::r2);
      res[i*8]=MM::reduce(MM::r2);
      for(int j=1;j<8;j++)res[i*8+j]=MM::reduce((unsigned long long)res[i*8+j-1]*v);
      prod=(unsigned long long)prod*r.invroot[i+4]%MM::mod();
    }
    return res;
  }();
  int h=lsb(a.size());
  int len=0;
  for(int i=0;i<(int)a.size();i++)b[i]=a[i].val();
  for(int i=0;i<(int)a.size();i+=8){
    __m256i u=_mm256_loadu_si256((__m256i*)(b+i));
    mul_simd<MM>(u,MM::r2);
    _mm256_storeu_si256((__m256i*)(b+i),u);
  }
  while(len+4<h){
    int p=1<<(h-len-2);
    unsigned rot=rate3_m[0],imag=MM::reduce((unsigned long long)r.root[2]*MM::r2);
    {
      for(int i=0;i<p;i+=8){
        __m256i b0=_mm256_loadu_si256((__m256i*)(b+i));
        __m256i b1=_mm256_loadu_si256((__m256i*)(b+i+p));
        __m256i b2=_mm256_loadu_si256((__m256i*)(b+i+p*2));
        __m256i b3=_mm256_loadu_si256((__m256i*)(b+i+p*3));
        __m256i m=sub_simd<MM>(b1,b3);
        mul_simd<MM>(m,imag);
        __m256i b02=add_simd<MM>(b0,b2);
        __m256i b0n2=sub_simd<MM>(b0,b2);
        __m256i b13=add_simd<MM>(b1,b3);
        _mm256_storeu_si256((__m256i*)(b+i),add_simd<MM>(b02,b13));
        _mm256_storeu_si256((__m256i*)(b+i+p),sub_simd<MM>(b02,b13));
        _mm256_storeu_si256((__m256i*)(b+i+p*2),add_simd<MM>(b0n2,m));
        _mm256_storeu_si256((__m256i*)(b+i+p*3),sub_simd<MM>(b0n2,m));
      }
    }
    for(int s=1;s<(1<<len);s++){
      unsigned rot2=MM::reduce((unsigned long long)rot*rot);
      unsigned rot3=MM::reduce((unsigned long long)rot2*rot);
      int offset=s<<(h-len);
      for(int i=0;i<p;i+=8){
        __m256i b0=_mm256_loadu_si256((__m256i*)(b+offset+i));
        __m256i b1=_mm256_loadu_si256((__m256i*)(b+offset+i+p));
        mul_simd<MM>(b1,rot);
        __m256i b2=_mm256_loadu_si256((__m256i*)(b+offset+i+p*2));
        mul_simd<MM>(b2,rot2);
        __m256i b3=_mm256_loadu_si256((__m256i*)(b+offset+i+p*3));
        mul_simd<MM>(b3,rot3);
        __m256i m=sub_simd<MM>(b1,b3);
        mul_simd<MM>(m,imag);
        __m256i b02=add_simd<MM>(b0,b2);
        __m256i b0n2=sub_simd<MM>(b0,b2);
        __m256i b13=add_simd<MM>(b1,b3);
        _mm256_storeu_si256((__m256i*)(b+offset+i),add_simd<MM>(b02,b13));
        _mm256_storeu_si256((__m256i*)(b+offset+i+p),sub_simd<MM>(b02,b13));
        _mm256_storeu_si256((__m256i*)(b+offset+i+p*2),add_simd<MM>(b0n2,m));
        _mm256_storeu_si256((__m256i*)(b+offset+i+p*3),sub_simd<MM>(b0n2,m));
      }
      rot=MM::reduce((unsigned long long)rot*rate3_m[lsb(~(unsigned)s)]);
    }
    len+=2;
  }
  if(len+4==h){
    unsigned rot=MM::reduce(MM::r2);
    for(int s=0;s<(1<<len);s++){
      int offset=s<<(h-len);
      __m256i b0=_mm256_loadu_si256((__m256i*)(b+offset));
      __m256i b1=_mm256_loadu_si256((__m256i*)(b+offset+8));
      mul_simd<MM>(b1,rot);
      _mm256_storeu_si256((__m256i*)(b+offset),add_simd<MM>(b0,b1));
      _mm256_storeu_si256((__m256i*)(b+offset+8),sub_simd<MM>(b0,b1));
      rot=MM::reduce((unsigned long long)rot*rate2_m[lsb(~(unsigned)s)]);
    }
  }
  unsigned one=MM::reduce(MM::r2);
  unsigned r2=MM::reduce((unsigned long long)r.root[2]*MM::r2);
  unsigned r3=MM::reduce((unsigned long long)r.root[3]*MM::r2);
  unsigned r3_2=MM::reduce((unsigned long long)r3*r3);
  unsigned r3_3=MM::reduce((unsigned long long)r3_2*r3);
  __m256i p1=_mm256_set_epi32(r3_3,r3_2,r3,one,one,one,one,one);
  __m256i p2=_mm256_set_epi32(r2,one,one,one,r2,one,one,one);
  __m256i rot=_mm256_set1_epi32(one);
  for(int s=0;s<(int)a.size()/8;s++){
    __m256i u=_mm256_loadu_si256((__m256i*)(b+s*8));
    mul_simd<MM>(u,rot);
    __m256i v=u;
    u=_mm256_blend_epi32(u,sub_simd<MM>(_mm256_setzero_si256(),u),0b11110000);
    v=_mm256_permute2x128_si256(v,v,0b01);
    u=add_simd<MM>(u,v);
    mul_simd<MM>(u,p1);
    v=u;
    u=_mm256_blend_epi32(u,sub_simd<MM>(_mm256_setzero_si256(),u),0b11001100);
    v=_mm256_shuffle_epi32(v,0b01001110);
    u=add_simd<MM>(u,v);
    mul_simd<MM>(u,p2);
    v=u;
    u=_mm256_blend_epi32(u,sub_simd<MM>(_mm256_setzero_si256(),u),0b10101010);
    v=_mm256_shuffle_epi32(v,0b10110001);
    u=add_simd<MM>(u,v);
    mul_simd<MM>(u,1);
    _mm256_storeu_si256((__m256i*)(b+s*8),u);
    for(int i=0;i<8;i++){
      if(b[s*8+i]>=T::mod())b[s*8+i]-=T::mod();
      a[s*8+i]=T::raw(b[s*8+i]);
    }
    mul_simd<MM>(rot,_mm256_loadu_si256((__m256i*)(rate4_m.data()+8*lsb(~(unsigned)s))));
  }
}
template<typename T>
__attribute__((target("avx2")))
void idft_simd(std::vector<T>&a){
  using MM=Montgomery<T::mod()>;
  static constexpr ntt_root<T::mod()>r;
  static constexpr std::array<unsigned,r.invrate3.size()>invrate3_m=[](){
    std::array<unsigned,r.invrate3.size()>res;
    for(int i=0;i<(int)res.size();i++)res[i]=MM::reduce((unsigned long long)r.invrate3[i]*MM::r2);
    return res;
  }();
  alignas(32) static constexpr std::array<unsigned,(r.rank2-3)*8>invrate4_m=[](){
    std::array<unsigned,(r.rank2-3)*8>res;
    unsigned prod=1;
    for(int i=0;i<=r.rank2-4;i++){
      unsigned v=(unsigned long long)r.invroot[i+4]*prod%MM::mod();
      v=MM::reduce((unsigned long long)v*MM::r2);
      res[i*8]=MM::reduce(MM::r2);
      for(int j=1;j<8;j++)res[i*8+j]=MM::reduce((unsigned long long)res[i*8+j-1]*v);
      prod=(unsigned long long)prod*r.root[i+4]%MM::mod();
    }
    return res;
  }();
  int h=lsb(a.size());
  int len=h;
  for(int i=0;i<(int)a.size();i++)b[i]=a[i].val();
  unsigned one=MM::reduce(MM::r2);
  unsigned r2=MM::reduce((unsigned long long)r.invroot[2]*MM::r2);
  unsigned r3=MM::reduce((unsigned long long)r.invroot[3]*MM::r2);
  unsigned r3_2=MM::reduce((unsigned long long)r3*r3);
  unsigned r3_3=MM::reduce((unsigned long long)r3_2*r3);
  __m256i p1=_mm256_set_epi32(r3_3,r3_2,r3,one,one,one,one,one);
  __m256i p2=_mm256_set_epi32(r2,one,one,one,r2,one,one,one);
  __m256i rot=_mm256_set1_epi32(one);
  for(int s=0;s<(int)a.size()/8;s++){
    __m256i u=_mm256_loadu_si256((__m256i*)(b+s*8));
    mul_simd<MM>(u,MM::r2);
    __m256i v=u;
    u=_mm256_blend_epi32(u,sub_simd<MM>(_mm256_setzero_si256(),u),0b10101010);
    v=_mm256_shuffle_epi32(v,0b10110001);
    u=add_simd<MM>(u,v);
    mul_simd<MM>(u,p2);
    v=u;
    u=_mm256_blend_epi32(u,sub_simd<MM>(_mm256_setzero_si256(),u),0b11001100);
    v=_mm256_shuffle_epi32(v,0b01001110);
    u=add_simd<MM>(u,v);
    mul_simd<MM>(u,p1);
    v=u;
    u=_mm256_blend_epi32(u,sub_simd<MM>(_mm256_setzero_si256(),u),0b11110000);
    v=_mm256_permute2x128_si256(v,v,0b01);
    u=add_simd<MM>(u,v);
    mul_simd<MM>(u,rot);
    _mm256_storeu_si256((__m256i*)(b+s*8),u);
    mul_simd<MM>(rot,_mm256_loadu_si256((__m256i*)(invrate4_m.data()+8*lsb(~(unsigned)s))));
  }
  len-=3;
  while(len>=2){
    int p=1<<(h-len);
    unsigned rot=invrate3_m[0],imag=MM::reduce((unsigned long long)r.invroot[2]*MM::r2);
    {
      for(int i=0;i<p;i+=8){
        __m256i b0=_mm256_loadu_si256((__m256i*)(b+i));
        __m256i b1=_mm256_loadu_si256((__m256i*)(b+i+p));
        __m256i b2=_mm256_loadu_si256((__m256i*)(b+i+p*2));
        __m256i b3=_mm256_loadu_si256((__m256i*)(b+i+p*3));
        __m256i b01=add_simd<MM>(b0,b1);
        __m256i b0n1=sub_simd<MM>(b0,b1);
        __m256i b23=add_simd<MM>(b2,b3);
        __m256i k=sub_simd<MM>(b2,b3);
        mul_simd<MM>(k,imag);
        _mm256_storeu_si256((__m256i*)(b+i),add_simd<MM>(b01,b23));
        b1=add_simd<MM>(b0n1,k);
        _mm256_storeu_si256((__m256i*)(b+i+p),b1);
        b2=sub_simd<MM>(b01,b23);
        _mm256_storeu_si256((__m256i*)(b+i+p*2),b2);
        b3=sub_simd<MM>(b0n1,k);
        _mm256_storeu_si256((__m256i*)(b+i+p*3),b3);
      }
    }
    for(int s=1;s<(1<<(len-2));s++){
      int offset=s<<(h-len+2);
      unsigned rot2=MM::reduce((unsigned long long)rot*rot);
      unsigned rot3=MM::reduce((unsigned long long)rot2*rot);
      for(int i=0;i<p;i+=8){
        __m256i b0=_mm256_loadu_si256((__m256i*)(b+offset+i));
        __m256i b1=_mm256_loadu_si256((__m256i*)(b+offset+i+p));
        __m256i b2=_mm256_loadu_si256((__m256i*)(b+offset+i+p*2));
        __m256i b3=_mm256_loadu_si256((__m256i*)(b+offset+i+p*3));
        __m256i b01=add_simd<MM>(b0,b1);
        __m256i b0n1=sub_simd<MM>(b0,b1);
        __m256i b23=add_simd<MM>(b2,b3);
        __m256i k=sub_simd<MM>(b2,b3);
        mul_simd<MM>(k,imag);
        _mm256_storeu_si256((__m256i*)(b+offset+i),add_simd<MM>(b01,b23));
        b1=add_simd<MM>(b0n1,k);
        mul_simd<MM>(b1,rot);
        _mm256_storeu_si256((__m256i*)(b+offset+i+p),b1);
        b2=sub_simd<MM>(b01,b23);
        mul_simd<MM>(b2,rot2);
        _mm256_storeu_si256((__m256i*)(b+offset+i+p*2),b2);
        b3=sub_simd<MM>(b0n1,k);
        mul_simd<MM>(b3,rot3);
        _mm256_storeu_si256((__m256i*)(b+offset+i+p*3),b3);
      }
      rot=MM::reduce((unsigned long long)rot*invrate3_m[lsb(~(unsigned)s)]);
    }
    len-=2;
  }
  if(len==1){
    for(int i=0;i<(1<<(h-1));i+=8){
      __m256i u=_mm256_loadu_si256((__m256i*)(b+i));
      __m256i v=_mm256_loadu_si256((__m256i*)(b+(1<<(h-1))+i));
      _mm256_storeu_si256((__m256i*)(b+i),add_simd<MM>(u,v));
      _mm256_storeu_si256((__m256i*)(b+(1<<(h-1))+i),sub_simd<MM>(u,v));
    }
  }
  for(int i=0;i<(int)a.size();i+=8){
    __m256i u=_mm256_loadu_si256((__m256i*)(b+i));
    mul_simd<MM>(u,1);
    _mm256_storeu_si256((__m256i*)(b+i),u);
    for(int j=0;j<8;j++){
      if(b[i+j]>=T::mod())b[i+j]-=T::mod();
      a[i+j]=T::raw(b[i+j]);
    }
  }
}
}
using ntt_simd_impl::dft_simd;
using ntt_simd_impl::idft_simd;
#include<iostream>
#include<cassert>
#include<optional>
#include<algorithm>
template<typename T>
void dft(std::vector<T>&a){
  #ifdef NTT_SIMD
  if((int)a.size()>=32){
    dft_simd(a);
    return;
  }
  #endif
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
  #ifdef NTT_SIMD
  if((int)a.size()>=32){
    idft_simd(a);
    return;
  }
  #endif
  static constexpr ntt_root<T::mod()>r;
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
template<typename T>
std::vector<std::vector<T>>convolution2d(const std::vector<std::vector<T>>&a,const std::vector<std::vector<T>>&b){
  int x=a.size()+b.size()-1,y=a[0].size()+b[0].size()-1;
  std::vector<T>f(a.size()*y),g(b.size()*y);
  for(int i=0;i<a.size();i++)for(int j=0;j<a[i].size();j++)f[i*y+j]=a[i][j];
  for(int i=0;i<b.size();i++)for(int j=0;j<b[i].size();j++)g[i*y+j]=b[i][j];
  auto c=ntt_convolution(f,g);
  std::vector<std::vector<T>>ret(x,std::vector<T>(y));
  for(int i=0;i<x;i++)for(int j=0;j<y;j++)ret[i][j]=c[i*y+j];
  return ret;
}
template<typename T>
std::vector<T> fps_inv(const std::vector<T> &a,int deg=-1){
  int n=a.size();
  if(deg==-1)deg=n;
  const T zero=T::raw(0);
  assert(a[0]!=zero);
  std::vector<T> ret(ceil_pow2(deg));
  ret[0]=a[0].inv();
  for(int m=1;m<deg;m<<=1){
    std::vector<T> f(a.begin(),a.begin()+std::min(n,m*2));
    if(f.size()<m*2)f.resize(m*2,0);
    std::vector<T> g(ret);
    f.resize(m*2);
    dft(f);
    g.resize(m*2);
    dft(g);
    for(int i=0;i<m*2;i++)f[i]*=g[i];
    idft(f);
    T inv=T::raw(m*2).inv();
    for(int i=0;i<m;i++)f[i]=zero;
    for(int i=m;i<m*2;i++)f[i]*=inv;
    dft(f);
    for(int i=0;i<m*2;i++)f[i]*=g[i];
    idft(f);
    for(int i=0;i<m*2;i++)f[i]*=inv;
    for(int i=m;i<m*2;i++)ret[i]-=f[i];
  }
  ret.resize(deg);
  return ret;
}
template<typename T>
std::vector<T> fps_exp(const std::vector<T>&a,int deg=-1){
  assert(a.empty()||a[0]==0);
  int n=a.size();
  if(deg==-1)deg=n;
  std::vector<T>inv(ceil_pow2(deg));
  inv[0]=0,inv[1]=1;
  for(int i=2;i<inv.size();i++)inv[i]=-inv[T::mod()%i]*(T::mod()/i);
  std::vector<T>b{1,1<n?a[1]:T()},c{T::raw(1)},f1,f2(2,T::raw(1));
  T sinv=T::raw(2).inv();
  for(int m=2;m<deg;m*=2){
    std::vector<T>y(b);
    y.resize(m*2);
    dft(y);
    f1=f2;
    std::vector<T>z(m);
    for(int i=0;i<m;i++)z[i]=y[i]*f1[i];
    idft(z);
    for(int i=m/2;i<m;i++)z[i]*=sinv;
    std::fill(z.begin(),z.begin()+m/2,T());
    dft(z);
    for(int i=0;i<m;i++)z[i]*=-f1[i];
    idft(z);
    for(int i=m/2;i<m;i++)z[i]*=sinv;
    c.insert(c.end(),z.begin()+m/2,z.end());
    f2=c;
    f2.resize(m*2);
    dft(f2);
    std::vector<T>x(m);
    std::copy(a.begin(),a.begin()+std::min(n,m),x.begin());
    for(int i=0;i<m-1;i++)x[i]=x[i+1]*T::raw(i+1);
    x.back()=T();
    dft(x);
    for(int i=0;i<m;i++)x[i]*=y[i];
    idft(x);
    for(int i=0;i<m;i++)x[i]*=sinv;
    x.resize(m*2);
    for(int i=0;i<m-1;i++)x[m+i]=x[i]-b[i+1]*T::raw(i+1),x[i]=T();
    dft(x);
    for(int i=0;i<m*2;i++)x[i]*=f2[i];
    idft(x);
    sinv/=2;
    for(int i=0;i<m*2;i++)x[i]*=sinv;
    for(int i=(int)x.size()-1;i>=1;i--)x[i]=x[i-1]*inv[i];
    x[0]=T();
    for(int i=m;i<std::min(n,m*2);i++)x[i]+=a[i];
    std::fill(x.begin(),x.begin()+m,T());
    dft(x);
    for(int i=0;i<m*2;i++)x[i]*=y[i];
    idft(x);
    for(int i=m;i<m*2;i++)x[i]*=sinv;
    b.insert(b.end(),x.begin()+m,x.end());
  }
  b.resize(deg);
  return b;
}
template<typename T>
std::vector<T> fps_diff(const std::vector<T>&a){
  int n=a.size();
  std::vector<T>b(std::max(0,n-1));
  for(int i=1;i<n;i++)b[i-1]=a[i]*T::raw(i);
  return b;
}
template<typename T>
std::vector<T> fps_integral(const std::vector<T>&a){
  int n=a.size();
  std::vector<T>b(n+1);
  b[0]=0;
  if(n)b[1]=1;
  for(int i=2;i<=n;i++)b[i]=-b[T::mod()%i]*(T::mod()/i);
  for(int i=0;i<n;i++)b[i+1]*=a[i];
  return b;
}
template<typename T>
std::vector<T> fps_log(const std::vector<T>&a,int deg=-1){
  int n=a.size();
  if(deg==-1)deg=n;
  std::vector<T>b=fps_integral(ntt_convolution(fps_diff(a),fps_inv(a,deg)));
  return {b.begin(),b.begin()+deg};
}
template<typename T>
std::vector<T> fps_pow(std::vector<T> a,unsigned long long k,int deg=-1){
  int n=a.size();
  if(deg==-1)deg=n;
  if(k==0){
    std::vector<T>ret(deg,0);
    ret[0]=1;
    return ret;
  }
  int of=0;
  while(a[of]==0&&of!=n)of++;
  if(of==n)return std::vector<T>(deg,0);
  if(of!=0&&k>=deg)return std::vector<T>(deg,0);
  if(of*k>=deg)return std::vector<T>(deg,0);
  a.erase(a.begin(),a.begin()+of);
  n=a.size();
  T a0=a[0];
  T inv=a[0].inv();
  for(int i=0;i<n;i++)a[i]*=inv;
  std::vector<T>lg=fps_log(a,deg);
  T tk=T(k);
  for(int i=0;i<deg;i++)lg[i]*=tk;
  std::vector<T>ep=fps_exp(lg,deg);
  T pw=a0.pow(k);
  for(int i=0;i<deg;i++)ep[i]*=pw;
  std::vector<T>ret(deg,0);
  for(int i=of*k;i<deg;i++)ret[i]=ep[i-of*k];
  return ret;
}
template<typename T>
std::vector<T>fps_divmod(std::vector<T>f,std::vector<T>g){
  if(f.empty())return f;
  int n=f.size(),m=g.size();
  std::vector<T>r(f);
  std::reverse(f.begin(),f.end()),std::reverse(g.begin(),g.end());
  std::vector<T>q=ntt_convolution(f,fps_inv(g,n));
  q.resize(std::max(0,n-m+1));
  auto p=ntt_convolution(g,q);
  std::reverse(p.begin(),p.end());
  for(int i=0;i<std::min<int>(p.size(),m);i++)r[i]-=p[i];
  r.resize(m-1);
  while(!r.empty()&&r.back().val()==0)r.pop_back();
  return r;
}
template<typename T>
std::optional<std::vector<T>>fps_sqrt(std::vector<T>f,int deg=-1){
  if(deg==-1)deg=f.size();
  int prefix_zero=0;
  while(prefix_zero<f.size()&&f[prefix_zero].val()==0)prefix_zero++;
  if(prefix_zero==f.size())return std::make_optional(std::vector<T>(deg,0));
  if(prefix_zero&1)return std::nullopt;
  f.erase(f.begin(),f.begin()+prefix_zero);
  prefix_zero/=2;
  auto opt_sq=f[0].sqrt();
  if(!opt_sq)return std::nullopt;
  T sq=*opt_sq;
  T inv0=f[0].inv();
  for(int i=0;i<f.size();i++)f[i]*=inv0;
  std::vector<T>g{1};
  T inv2=T::raw(2).inv();
  while(g.size()<deg-prefix_zero){
    std::vector<T>fp(f.begin(),f.begin()+std::min(f.size(),g.size()*2));
    fp=ntt_convolution(fp,fps_inv(g,g.size()*2));
    std::vector<T>nxtg(g);
    nxtg.resize(g.size()*2);
    for(int i=0;i<nxtg.size();i++){
      if(i<fp.size())nxtg[i]+=fp[i];
      nxtg[i]*=inv2;
    }
    std::swap(g,nxtg);
  }
  g.insert(g.begin(),prefix_zero,0);
  g.resize(deg);
  for(int i=0;i<g.size();i++)g[i]*=sq;
  return g;
}
template<typename T>
struct fps2d{
private:
  int n,m;
  std::vector<T>a;
public:
  fps2d():n(0),m(0){}
  fps2d(int n,int m):n(n),m(m),a(n*m){}
  fps2d(const std::vector<std::vector<T>>&init):n(init.size()),m(init.empty()?0:init[0].size()),a(n*m){
    if(n>0){
      assert(std::all_of(init.begin(),init.end(),[&](const std::vector<T>&x){return (int)x.size()==m;}));
      for(int i=0;i<n;i++)for(int j=0;j<m;j++)a[i*m+j]=init[i][j];
    }
  }
  fps2d(int n,int m,const std::vector<T>&init):n(n),m(m),a(init){
    assert((long long)n*m==(long long)init.size());
  }
  void resize(int n_,int m_){
    std::vector<T>na(n_*m_);
    for(int i=0;i<std::min(n,n_);i++)for(int j=0;j<std::min(m,m_);j++){
      na[i*m_+j]=a[i*m+j];
    }
    a=std::move(na);
    n=n_,m=m_;
  }
  fps2d &operator+=(const fps2d&rhs){
    assert(n==rhs.n&&m==rhs.m);
    for(int i=0;i<n*m;i++)a[i]+=rhs[i];
    return *this;
  }
  fps2d &operator-=(const fps2d&rhs){
    assert(n==rhs.n&&m==rhs.m);
    for(int i=0;i<n*m;i++)a[i]-=rhs[i];
    return *this;
  }
  fps2d &operator*=(const fps2d&rhs){
    int mm=m+rhs.m-1;
    int z=ceil_pow2((n+rhs.n)*mm-1);
    std::vector<T>f(z),g(z);
    for(int i=0;i<n;i++)for(int j=0;j<m;j++)f[i*mm+j]=a[i*m+j];
    for(int i=0;i<rhs.n;i++)for(int j=0;j<rhs.m;j++)g[i*mm+j]=rhs.a[i*rhs.m+j];
    dft(f),dft(g);
    for(int i=0;i<z;i++)f[i]*=g[i];
    idft(f);
    T inv=T(z).inv();
    f.resize((n+rhs.n-1)*mm);
    for(T&x:f)x*=inv;
    a=std::move(f);
    n+=rhs.n-1;
    m+=rhs.m-1;
    return *this;
  }
  fps2d &operator/=(const fps2d&rhs){
    return *this*=rhs.inv();
  }
  friend fps2d operator+(const fps2d&lhs,const fps2d&rhs){return fps2d(lhs)+=rhs;}
  friend fps2d operator-(const fps2d&lhs,const fps2d&rhs){return fps2d(lhs)-=rhs;}
  friend fps2d operator*(const fps2d&lhs,const fps2d&rhs){return fps2d(lhs)*=rhs;}
  friend fps2d operator/(const fps2d&lhs,const fps2d&rhs){return fps2d(lhs)/=rhs;}
  fps2d inv()const{
    fps2d res(1,m,fps_inv(std::vector<T>(a.begin(),a.begin()+m)));
    for(int deg=1;deg<n;deg<<=1){
      int n2=std::min(n,deg*2);
      fps2d f(n2,m,std::vector<T>(a.begin(),a.begin()+n2*m));
      f*=res;
      f.resize(n2,m);
      for(T&x:f.a)x=-x;
      f.a[0]++;
      f.a[0]++;
      res*=f;
      res.resize(n2,m);
    }
    return res;
  }
  T&operator[](int x,int y){return a[x*m+y];}
  const T& operator[](int x,int y)const{return a[x*m+y];}
};
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
  friend inline bool operator<(const mint &a,const mint &b){return a.val()<b.val();}
  friend inline bool operator>(const mint &a,const mint &b){return a.val()>b.val();}
  friend inline bool operator<=(const mint &a,const mint &b){return a.val()<=b.val();}
  friend inline bool operator>=(const mint &a,const mint &b){return a.val()>=b.val();}
  inline mint operator++(int){
    mint ret=*this;
    *this+=mint::raw(1);
    return ret;
  }
  inline mint operator--(int){
    mint ret=*this;
    *this-=mint::raw(1);
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
  int n,m;
  rd(n),rd(m);
  std::vector<mint>a(n*m);
  for(mint&x:a){
    int v;
    rd(v);
    x=mint::raw(v);
  }
  fps2d f(n,m,std::move(a));
  f=f.inv();
  for(int i=0;i<n;i++)for(int j=0;j<m;j++){
    wt(f[i,j].val());
    wt(" \n"[j+1==m]);
  }
}
