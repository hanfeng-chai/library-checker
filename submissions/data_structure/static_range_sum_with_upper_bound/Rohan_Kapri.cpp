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
#include<iostream>
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
template<typename M>
struct BinaryIndexedTree{
private:
  using S=typename M::S;
  std::vector<S>dat;
  int z;
public:
  BinaryIndexedTree(){}
  explicit BinaryIndexedTree(int n):z(ceil_pow2(n)),dat(n,M::e()){}
  explicit BinaryIndexedTree(const std::vector<S>&init):z(ceil_pow2((int)init.size())),dat(init){
    for(int i=0;i<(int)dat.size();i++){
      int j=i+((i+1)&-(i+1));
      if(j<(int)dat.size())dat[j]=M::op(dat[i],dat[j]);
    }
  }
  inline void add(int i,S x){
    while(i<(int)dat.size()){
      dat[i]=M::op(dat[i],x);
      i+=(i+1)&-(i+1);
    }
  }
  inline S sum(int i)const{
    S res=M::e();
    while(i>0){
      res+=dat[i-1];
      i-=i&-i;
    }
    return res;
  }
  inline S sum(int l,int r)const{
    S lp=M::e(),rp=M::e();
    while(l<r){
      rp=M::op(dat[r-1],rp);
      r-=r&-r;
    }
    while(r<l){
      lp=M::op(dat[l-1],lp);
      l-=l&-l;
    }
    return M::op(M::inverse(lp),rp);
  }
  int lower_bound(S k)const{
    int res=0;
    for(int i=z;i>=1;i>>=1){
      if(res+i<=(int)dat.size()&&dat[res+i-1]<k){
        k-=dat[res+i-1];
        res+=i;
      }
    }
    return res;
  }
  friend std::ostream &operator<<(std::ostream &os,const BinaryIndexedTree&bit){
    os<<"{";
    for(int i=0;i<(int)bit.dat.size();i++)os<<bit.sum(i,i+1)<<",}"[i+1==(int)bit.dat.size()];
    if(bit.dat.empty())os<<"}";
    return os;
  }
};
struct BinaryIndexedTree01{
  int n,n64;
  std::vector<int>dat;
  std::vector<u_int64_t>a;
  explicit BinaryIndexedTree01(int n_):n(n_),n64((n_+63)>>6){
    dat.resize(n64,0);
    a.resize(n64,u_int64_t(0));
  }
  void set(int k,int x){
    if((a[k>>6]>>(k&63)&1)==x)return;
    a[k>>6]^=u_int64_t(1)<<(k&63);
    if(!x)x=-1;
    k>>=6;
    k++;
    while(k<=n64){
      dat[k-1]+=x;
      k+=k&-k;
    }
  }
  int sum(int l,int r)const{
    int ret=__builtin_popcountll(a[r>>6]&((u_int64_t(1)<<(r&63))-1))-__builtin_popcountll(a[l>>6]&((u_int64_t(1)<<(l&63))-1));
    l>>=6,r>>=6;
    while(l<r){
      ret+=dat[r-1];
      r-=r&-r;
    }
    while(r<l){
      ret-=dat[l-1];
      l-=l&-l;
    }
    return ret;
  }
};
template<typename T=int>
struct MonoidAdd{
  using S=T;
  using F=std::nullptr_t;
  static inline S op(S x,S y){return x+y;}
  static inline S e(){return T();}
  static inline S mapping(F,const S&x,long long){return x;}
  static inline F composition(F,F){return nullptr;}
  static inline F id(){return nullptr;}
  static inline S inverse(S x){return -x;}
  static inline void revS(S&x){}
  static inline S pow(S x,long long p){return x*p;}
};
#include<algorithm>
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<int>a(n);
  std::vector<std::pair<int,int>>ord(n+q);
  for(int i=0;i<n;i++)rd(a[i]),ord[i]=std::make_pair(a[i],~i);
  std::vector<std::pair<int,int>>query(q);
  for(int i=0;i<q;i++){
    rd(query[i].first),rd(query[i].second);
    int x;
    rd(x);
    ord[i+n]=std::make_pair(x,i);
  }
  std::sort(ord.begin(),ord.end());
  BinaryIndexedTree01 cnt(n);
  BinaryIndexedTree<MonoidAdd<long long>>sum(n);
  std::vector<std::pair<int,long long>>ans(q);
  for(auto [x,i]:ord){
    if(i<0){
      i=~i;
      cnt.set(i,1);
      sum.add(i,x);
    }
    else{
      ans[i].first=cnt.sum(query[i].first,query[i].second);
      ans[i].second=sum.sum(query[i].first,query[i].second);
    }
  }
  for(auto [c,s]:ans)wt(c),wt(' '),wt(s),wt('\n');
}
