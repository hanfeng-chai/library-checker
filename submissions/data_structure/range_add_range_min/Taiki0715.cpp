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
#include<iostream>
#include<vector>
#include<cassert>
#include<functional>
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
template<typename T>
struct RangeAddRangeMin{
private:
  int n,z;
  std::vector<T>seg,mn;
  static constexpr T inf=std::numeric_limits<T>::max()/2;
public:
  RangeAddRangeMin(){}
  explicit RangeAddRangeMin(int n):n(n),z(ceil_pow2(n)),seg(z*2),mn(z*2){}
  explicit RangeAddRangeMin(const std::vector<T>&init):n(init.size()),z(ceil_pow2(n)),seg(z*2),mn(z*2,inf){
    std::copy(init.begin(),init.end(),mn.begin()+z);
    for(int i=z;--i;)mn[i]=std::min(mn[i*2],mn[i*2+1]);
  }
  void add(int l,int r,T x){
    assert(0<=l&&l<=r&&r<=n);
    if(l<n){
      seg[l+=z]+=x;
      mn[l]+=x;
      while(l>>=1){
        seg[l]+=x;
        mn[l]=std::min(mn[l*2],mn[l*2+1]+seg[l*2]);
      }
    }
    if(r<n){
      seg[r+=z]-=x;
      mn[r]-=x;
      while(r>>=1){
        seg[r]-=x;
        mn[r]=std::min(mn[r*2],mn[r*2+1]+seg[r*2]);
      }
    }
  }
  T min(int l,int r)const{
    assert(0<=l&&l<=r&&r<=n);
    if(l==r)return inf;
    if(r==n)r=z;
    T lv=inf,rv=inf,s=0;
    l+=z,r+=z;
    int l2=l;
    while(l<r){
      if(l&1){
        if(lv>mn[l]+s)lv=mn[l]+s;
        s+=seg[l];
        l++;
      }
      if(r&1){
        --r;
        rv+=seg[r];
        if(rv>mn[r])rv=mn[r];
      }
      l>>=1,r>>=1;
    }
    if(lv>rv+s)lv=rv+s;
    l=l2;
    while((l>>=lsb(l))>1)lv+=seg[--l];
    return lv;
  }
  T get(int i)const{
    assert(0<=i&&i<n);
    T res=mn[i+=z];
    while((i>>=lsb(i))>1)res+=seg[--i];
    return res;
  }
  inline void set(int i,T x){
    T s=0;
    for(int j=i+z;(j>>=lsb(j))>1;)s+=seg[--j];
    mn[i+=z]=x-s;
    while(i>>=1)mn[i]=std::min(mn[i*2],mn[i*2+1]+seg[i*2]);
  }
  template<typename Func>
  int max_right(int l,const Func&f)const{
    static_assert(std::is_convertible_v<Func,std::function<bool(T)>>);
    if(l==n)return n;
    l+=z;
    T s=0;
    for(int i=l;(i>>=lsb(i))>1;)s+=seg[--i];
    do{
      l>>=lsb(l);
      T v=s+mn[l];
      if(f(v))s+=seg[l++];
      else{
        while(l<z){
          l<<=1;
          v=s+mn[l];
          if(f(v))s+=seg[l++];
        }
        return l-z;
      }
    }while(l!=(l&-l));
    return n;
  }
  template<typename Func>
  int min_left(int r,const Func&f)const{
    static_assert(std::is_convertible_v<Func,std::function<bool(T)>>);
    if(r==0)return 0;
    r+=z;
    T s=0;
    if(r==z*2)s=seg[1];
    else for(int i=r;(i>>=lsb(i))>1;)s+=seg[--i];
    do{
      r--;
      r>>=lsb(~r);
      if(r==0)r=1;
      T v=s-seg[r]+mn[r];
      if(f(v))s-=seg[r];
      else{
        while(r<z){
          r=(r<<1)+1;
          v=s-seg[r]+mn[r];
          if(f(v))s-=seg[r--];
        }
        return r-z+1;
      }
    }while(r!=(r&-r));
    return 0;
  }
  std::vector<T>get_all()const{
    std::vector<T>res(mn.begin()+z,mn.begin()+z+n);
    T s=0;
    for(int i=0;i<n;i++){
      res[i]+=s;
      s+=seg[i+z];
    }
    return res;
  }
  friend std::ostream &operator<<(std::ostream&os,const RangeAddRangeMin&seg){
    std::vector<T>a=seg.get_all();
    int n=a.size();
    os<<"{";
    for(int i=0;i<n;i++)os<<a[i]<<",}"[i+1==n];
    if(n==0)os<<"}";
    return os;
  }
  int size()const{return n;}
};
using namespace std;
int main(){
  int n,q;
  rd(n),rd(q);
  vector<long long>a(n);
  for(long long&x:a)rd(x);
  RangeAddRangeMin<long long>seg(a);
  while(q--){
    int t,l,r;
    rd(t),rd(l),rd(r);
    if(t==0){
      long long x;
      rd(x);
      seg.add(l,r,x);
    }
    else{
      wt(seg.min(l,r)),wt('\n');
    }
  }
}
