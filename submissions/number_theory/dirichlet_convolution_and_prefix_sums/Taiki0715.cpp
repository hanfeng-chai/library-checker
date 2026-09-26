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
#include<iostream>
#include<cassert>
#include<optional>
#include<numeric>
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
#include<vector>
#include<cmath>
struct Quotients{
  std::vector<long long>dat;
  long long n,s;
  Quotients(){}
  explicit Quotients(long long n_):n(n_),s(std::sqrt(n_)){
    dat.resize(s*2-(s==n/s));
    std::iota(dat.begin(),dat.begin()+s,1);
    for(int i=s-(s==n/s);i>=1;i--)dat[dat.size()-i]=n/i;
  }
  const long long &operator[](int i)const{return dat[i];}
  int get_key(long long k)const{
    if(k<=s)return k-1;
    else return dat.size()-n/k;
  }
  int size()const{return dat.size();}
};
using mint=mint998;
int main(){
  int t;
  rd(t);
  while(t--){
    long long n;
    rd(n);
    Quotients quo(n);
    std::vector<mint>f(quo.size()),g(quo.size());
    for(int i=0;i<quo.size();i++){
      int x;
      rd(x);
      f[i]=mint::raw(x);
    }
    for(int i=0;i<quo.size();i++){
      int x;
      rd(x);
      g[i]=mint::raw(x);
    }
    std::vector<mint>fsmall(f.begin(),f.begin()+quo.s),gsmall(g.begin(),g.begin()+quo.s);
    for(int i=quo.s-1;i>=1;i--){
      fsmall[i]-=fsmall[i-1];
      gsmall[i]-=gsmall[i-1];
    }
    std::vector<mint>h(quo.size()+1);
    for(long long x=1;x*x*x<=n;x++){
      for(long long y=x;x*y*y<=n;y++){
        mint val=fsmall[x-1]*gsmall[y-1];
        h[quo.get_key(x*y)]+=val;
        h[quo.size()-y+1]-=val;
      }
    }
    for(long long y=1;y*y*y<=n;y++){
      for(long long x=y+1;y*x*x<=n;x++){
        mint val=fsmall[x-1]*gsmall[y-1];
        h[quo.get_key(x*y)]+=val;
        h[quo.size()-x+1]-=val;
      }
    }
    for(int i=1;i<quo.size();i++)h[i]+=h[i-1];
    for(long long x=1;x*x*x<=n;x++){
      for(long long z=x;x*z*z<=n;z++){
        h[quo.size()-z]+=fsmall[x-1]*(g[quo.get_key(n/(x*z))]-g[z-1]);
      }
    }
    for(long long z=1;z*z*z<=n;z++){
      for(long long x=z+1;z*x*x<=n;x++){
        h[quo.size()-z]+=fsmall[x-1]*(g[quo.get_key(n/(x*z))]-g[x-2]);
      }
    }
    for(long long y=1;y*y*y<=n;y++){
      for(long long z=y;y*z*z<=n;z++){
        h[quo.size()-z]+=gsmall[y-1]*(f[quo.get_key(n/(y*z))]-f[z-1]);
      }
    }
    for(long long z=1;z*z*z<=n;z++){
      for(long long y=z+1;z*y*y<=n;y++){
        h[quo.size()-z]+=gsmall[y-1]*(f[quo.get_key(n/(y*z))]-f[y-1]);
      }
    }
    for(int i=0;i<quo.size();i++)wt(h[i].val()),wt(" \n"[i+1==quo.size()]);
  }
}
