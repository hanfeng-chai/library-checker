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
struct SegmentTreeBeats{
private:
  static constexpr T inf=std::numeric_limits<T>::max();
  static constexpr T minf=std::numeric_limits<T>::min();
  struct node{
    T sum,add;
    T mx,mx2,mn,mn2;
    int mxcnt,mncnt;
    node():sum(0),add(0),mx(minf),mx2(minf),mn(inf),mn2(inf),mxcnt(0),mncnt(0){}
    node(T x):sum(x),add(0),mx(x),mx2(minf),mn(x),mn2(inf),mxcnt(1),mncnt(1){}
  };
  std::vector<node>dat;
  int z,log2n;
  inline void update(int i){
    node&nd=dat[i];
    nd.sum=dat[i*2].sum+dat[i*2+1].sum;
    if(dat[i*2].mx==dat[i*2+1].mx){
      nd.mx=dat[i*2].mx;
      nd.mx2=std::max(dat[i*2].mx2,dat[i*2+1].mx2);
      nd.mxcnt=dat[i*2].mxcnt+dat[i*2+1].mxcnt;
    }
    else{
      int large=i*2+(dat[i*2].mx<dat[i*2+1].mx);
      nd.mx=dat[large].mx;
      nd.mx2=std::max(dat[large].mx2,dat[large^1].mx);
      nd.mxcnt=dat[large].mxcnt;
    }
    if(dat[i*2].mn==dat[i*2+1].mn){
      nd.mn=dat[i*2].mn;
      nd.mn2=std::min(dat[i*2].mn2,dat[i*2+1].mn2);
      nd.mncnt=dat[i*2].mncnt+dat[i*2+1].mncnt;
    }
    else{
      int small=i*2+(dat[i*2].mn>dat[i*2+1].mn);
      nd.mn=dat[small].mn;
      nd.mn2=std::min(dat[small].mn2,dat[small^1].mn);
      nd.mncnt=dat[small].mncnt;
    }
  }
  inline void push(int i){
    node&nd=dat[i];
    if(nd.add){
      propagate_add(i*2,nd.add);
      propagate_add(i*2+1,nd.add);
      nd.add=0;
    }
    if(nd.mx<dat[i*2].mx)propagate_chmin(i*2,nd.mx);
    if(nd.mn>dat[i*2].mn)propagate_chmax(i*2,nd.mn);
    if(nd.mx<dat[i*2+1].mx)propagate_chmin(i*2+1,nd.mx);
    if(nd.mn>dat[i*2+1].mn)propagate_chmax(i*2+1,nd.mn);
  }
  inline void propagate_add(int i,T v){
    node&nd=dat[i];
    nd.add+=v;
    nd.sum+=v<<(log2n-msb(i));
    nd.mx+=v;
    nd.mn+=v;
    if(nd.mx2!=minf)nd.mx2+=v;
    if(nd.mn2!=inf)nd.mn2+=v;
  }
  inline void propagate_chmin(int i,T v){
    node&nd=dat[i];
    nd.sum+=(v-nd.mx)*nd.mxcnt;
    if(nd.mn==nd.mx)nd.mn=v;
    else if(nd.mn2==nd.mx)nd.mn2=v;
    nd.mx=v;
  }
  inline void propagate_chmax(int i,T v){
    node&nd=dat[i];
    nd.sum+=(v-nd.mn)*nd.mncnt;
    if(nd.mx==nd.mn)nd.mx=v;
    else if(nd.mx2==nd.mn)nd.mx2=v;
    nd.mn=v;
  }
  inline void rec_chmin(int i,T v){
    if(dat[i].mx<=v)return;
    if(dat[i].mx2<=v)return propagate_chmin(i,v);
    push(i);
    rec_chmin(i*2,v);
    rec_chmin(i*2+1,v);
    update(i);
  }
  inline void rec_chmax(int i,T v){
    if(dat[i].mn>=v)return;
    if(dat[i].mn2>=v)return propagate_chmax(i,v);
    push(i);
    rec_chmax(i*2,v);
    rec_chmax(i*2+1,v);
    update(i);
  }
  template<int com>
  inline void propagate(int i,T v){
    static_assert(com==0||com==1||com==2);
    if constexpr(com==0)propagate_add(i,v);
    else if constexpr(com==1)rec_chmin(i,v);
    else rec_chmax(i,v);
  }
  void path_push(int i){
    int l=lsb(i);
    for(int j=log2n;j>l;j--)push(i>>j);
  }
  void path_update(int i){
    i>>=lsb(i);
    while(i>>=1)update(i);
  }
  template<int com>
  inline void op(T&x,const node&y){
    static_assert(com==0||com==1||com==2);
    if constexpr(com==0)x+=y.sum;
    else if constexpr(com==1)x=std::min(x,y.mn);
    else x=std::max(x,y.mx);
  }
  template<int com>
  inline T e(){
    static_assert(com==0||com==1||com==2);
    if constexpr(com==0)return 0;
    else if constexpr(com==1)return inf;
    else return minf;
  }
  template<int com>
  T prod(int l,int r){
    l+=z,r+=z;
    path_push(l),path_push(r);
    T res=e<com>();
    while(l<r){
      if(l&1)op<com>(res,dat[l++]);
      if(r&1)op<com>(res,dat[--r]);
      l>>=1,r>>=1;
    }
    return res;
  }
  template<int com>
  void apply(int l,int r,T v){
    l+=z,r+=z;
    path_push(l),path_push(r);
    int l2=l,r2=r;
    while(l<r){
      if(l&1)propagate<com>(l++,v);
      if(r&1)propagate<com>(--r,v);
      l>>=1,r>>=1;
    }
    path_update(l2),path_update(r2);
  }
public:
  SegmentTreeBeats(){}
  explicit SegmentTreeBeats(int n):SegmentTreeBeats(std::vector<T>(n)){}
  SegmentTreeBeats(int n,T v):SegmentTreeBeats(std::vector<T>(n,v)){}
  explicit SegmentTreeBeats(const std::vector<T>&init){
    z=ceil_pow2(init.size());
    log2n=msb(z);
    dat.resize(z*2);
    for(int i=0;i<std::ssize(init);i++){
      dat[i+z].sum=dat[i+z].mx=dat[i+z].mn=init[i];
      dat[i+z].mxcnt=dat[i+z].mncnt=1;
    }
    for(int i=z;--i;)update(i);
  }
  inline T sum(int l,int r){return prod<0>(l,r);}
  inline T min(int l,int r){return prod<1>(l,r);}
  inline T max(int l,int r){return prod<2>(l,r);}
  inline void add(int l,int r,T v){apply<0>(l,r,v);}
  inline void chmin(int l,int r,T v){apply<1>(l,r,v);}
  inline void chmax(int l,int r,T v){apply<2>(l,r,v);}
};
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<long long>a(n);
  for(int i=0;i<n;i++)rd(a[i]);
  SegmentTreeBeats<long long>seg(a);
  while(q--){
    int t,l,r;
    rd(t),rd(l),rd(r);
    if(t==0){
      long long b;
      rd(b);
      seg.chmin(l,r,b);
    }
    else if(t==1){
      long long b;
      rd(b);
      seg.chmax(l,r,b);
    }
    else if(t==2){
      long long b;
      rd(b);
      seg.add(l,r,b);
    }
    else{
      wt(seg.sum(l,r)),wt('\n');
    }
  }
}
