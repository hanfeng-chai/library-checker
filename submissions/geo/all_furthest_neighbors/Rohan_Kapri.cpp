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
#include<memory>
template<typename T>
struct fast_stack{
private:
  T *st;
  int p;
public:
  fast_stack(int n):p(0){
    st=new T[n];
  }
  fast_stack(){}
  inline void push(const T&x){st[p++]=x;}
  template<typename...Args>
  inline T& emplace(Args&&...args){
    st[p++]=T(std::forward<Args>(args)...);
    return st[p-1];
  }
  inline T& pop(){return st[--p];}
  inline T top()const{return st[p-1];}
  inline T& top(){return st[p-1];}
  inline int size()const{return p;}
  inline bool empty()const{return !p;}
  inline void clear(){p=0;}
  ~fast_stack(){delete[] st;}
};
template<typename Func>
std::vector<int>monotone_minima(int n,int m,const Func&f){
  std::vector<int>res(n);
  fast_stack<std::tuple<int,int,int,int>>st(msb(n)*2+5);
  st.emplace(0,n,0,m);
  while(!st.empty()){
    const auto[lx,rx,ly,ry]=st.pop();
    if(lx==rx)continue;
    if(lx+1==rx){
      res[lx]=ly;
      for(int i=ly+1;i<ry;i++)if(f(lx,res[lx],i))res[lx]=i;
    }
    else{
      int mid=(lx+rx)>>1;
      res[mid]=ly;
      for(int i=ly+1;i<ry;i++)if(f(mid,res[mid],i))res[mid]=i;
      st.emplace(lx,mid,ly,res[mid]+1);
      st.emplace(mid+1,rx,res[mid],ry);
    }
  }
  return res;
}
#include<numeric>
#include<cmath>
#include<iostream>
#include<optional>
template<typename T>constexpr std::enable_if_t<std::is_floating_point_v<T>,T> epsilon(){return 1e-10;}
template<typename T>constexpr std::enable_if_t<!std::is_floating_point_v<T>,T>epsilon(){return 0;}
template<typename T>
struct Point{
  static constexpr T eps=epsilon<T>();
  static constexpr T T_abs(T x){return x<T(0)?-x:x;}
  T x,y;
  constexpr Point():x(0),y(0){}
  constexpr Point(T x_,T y_):x(x_),y(y_){}
  Point &operator+=(const Point&rhs){
    this->x+=rhs.x;
    this->y+=rhs.y;
    return *this;
  }
  Point &operator-=(const Point&rhs){
    this->x-=rhs.x;
    this->y-=rhs.y;
    return *this;
  }
  Point &operator*=(const T&rhs){
    this->x*=rhs;
    this->y*=rhs;
    return *this;
  }
  Point &operator/=(const T&rhs){
    this->x/=rhs;
    this->y/=rhs;
    return *this;
  }
  friend Point operator+(const Point&lhs,const Point&rhs){return Point(lhs)+=rhs;}
  friend Point operator-(const Point&lhs,const Point&rhs){return Point(lhs)-=rhs;}
  friend Point operator*(const Point&lhs,const T&rhs){return Point(lhs)*=rhs;}
  friend Point operator/(const Point&lhs,const T&rhs){return Point(lhs)/=rhs;}
  friend bool operator==(const Point&lhs,const Point&rhs){return T_abs(lhs.x-rhs.x)<=eps&&T_abs(lhs.y-rhs.y)<=eps;}
  friend bool operator!=(const Point&lhs,const Point&rhs){return !(lhs==rhs);}
  friend bool operator<(const Point&lhs,const Point&rhs){
    if(lhs.x!=rhs.x)return lhs.x<rhs.x;
    return lhs.y<rhs.y;
  }
  friend bool operator>(const Point&lhs,const Point&rhs){return rhs<lhs;}
  friend bool operator<=(const Point&lhs,const Point&rhs){return !(rhs<lhs);}
  friend bool operator>=(const Point&lhs,const Point&rhs){return !(lhs<rhs);}
  friend std::istream &operator>>(std::istream&is,Point&p){
    is>>p.x>>p.y;
    return is;
  }
  friend std::ostream &operator<<(std::ostream&os,const Point&p){
    os<<p.x<<' '<<p.y;
    return os;
  }
  T arg()const{
    static_assert(std::is_floating_point_v<T>);
    return atan2l(y,x);
  }
  Point<T>rot(T theta)const{
    static_assert(std::is_floating_point_v<T>);
    T s=sin(theta),c=cos(theta);
    return Point(c*x-s*y,s*x+c*y);
  }
  Point<T>rot90()const{
    return Point(-y,x);
  }
  T norm()const{return x*x+y*y;}
  T abs()const{
    static_assert(std::is_floating_point_v<T>);
    return std::sqrt(norm());
  }
  template<typename T2=double>
  Point<T2>convert()const{
    Point<T2>res(this->x,this->y);
    return res;
  }
};
template<typename T>
T cross(const Point<T>&a,const Point<T>&b){return a.x*b.y-a.y*b.x;}
template<typename T>
T dot(const Point<T>&a,const Point<T>&b){return a.x*b.x+a.y*b.y;}
//交差する点のうち1つ
template<typename T>
std::conditional_t<std::is_floating_point_v<T>,std::optional<Point<T>>,bool>intersect(const Point<T>&a,const Point<T>&b,const Point<T>&c,const Point<T>&d){
  T ac=cross(d-c,a-c),bc=cross(d-c,b-c),cc=cross(b-a,c-a),dc=cross(b-a,d-a);
  T ab=(b-a).norm(),cd=(d-c).norm();
  if(T_abs(ac)<=epsilon<T>()){
    if((c-a).norm()<=cd+epsilon<T>()&&(d-a).norm()<=cd+epsilon<T>()){
      if constexpr(std::is_floating_point_v<T>)return std::make_optional(a);
      else return true;
    }
    else if(T_abs(bc)<=epsilon<T>()){
      if((c-b).norm()<=cd+epsilon<T>()&&(d-b).norm()<=cd+epsilon<T>()){
        if constexpr(std::is_floating_point_v<T>)return std::make_optional(b);
        else return true;
      }
    }
    if(dot(a-c,b-c)<epsilon<T>()){
      if constexpr(std::is_floating_point_v<T>)return std::make_optional(c);
      else return true;
    }
    if constexpr(std::is_floating_point_v<T>)return std::nullopt;
    else return false;
  }
  if(T_abs(bc)<=epsilon<T>()){
    if((c-b).norm()<=cd+epsilon<T>()&&(d-b).norm()<=cd+epsilon<T>()){
      if constexpr(std::is_floating_point_v<T>)return std::make_optional(b);
      else return true;
    }
    if constexpr(std::is_floating_point_v<T>)return std::nullopt;
    else return false;
  }
  if(T_abs(cc)<=epsilon<T>()){
    if((a-c).norm()<=ab+epsilon<T>()&&(b-c).norm()<=ab+epsilon<T>()){
      if constexpr(std::is_floating_point_v<T>)return std::make_optional(c);
      else return true;
    }
    if constexpr(std::is_floating_point_v<T>)return std::nullopt;
    else return false;
  }
  if(T_abs(dc)<=epsilon<T>()){
    if((a-d).norm()<=ab+epsilon<T>()&&(b-d).norm()<=ab+epsilon<T>()){
      if constexpr(std::is_floating_point_v<T>)return std::make_optional(d);
      else return true;
    }
    if constexpr(std::is_floating_point_v<T>)return std::nullopt;
    else return false;
  }
  if((ac>0)!=(bc>0)&&(cc>0)!=(dc>0)){
    if constexpr(std::is_floating_point_v<T>)return c+(d-c)*cross(b-a,b-c)/cross(b-a,d-c);
    else return true;
  }
  else{
    if constexpr(std::is_floating_point_v<T>)return std::nullopt;
    else return false;
  }
}
using namespace std;
int main(){
  int t;
  rd(t);
  while(t--){
    int n;
    rd(n);
    vector<Point<long long>>a(n);
    for(auto&[x,y]:a)rd(x),rd(y);
    auto norm=[&](int i,int j)->long long {
      if(j>=n)j-=n;
      return (a[i]-a[j]).norm();
    };
    int s=0;
    for(int i=1;i<n;i++)if(norm(0,s)<norm(0,i))s=i;
    vector<int>ans(n);
    auto dfs=[&](auto self,int lx,int rx,int ly,int ry)->void {
      if(lx==rx)return;
      int mid=(lx+rx)/2;
      int t=ly;
      for(int i=ly+1;i<ry;i++){
        if(norm(mid,s+t)<norm(mid,s+i))t=i;
      }
      ans[mid]=s+t;
      if(ans[mid]>=n)ans[mid]-=n;
      self(self,lx,mid,ly,t+1);
      self(self,mid+1,rx,t,ry);
    };
    int p=0;
    for(int i=1;i<n;i++)if(norm(s,s+p)<norm(s,s+i))p=i;
    ans[s]=s+p;
    if(ans[s]>=n)ans[s]-=n;
    dfs(dfs,0,s,0,p+1);
    dfs(dfs,s+1,n,p,n+1);
    for(int i=0;i<n;i++)wt(ans[i]),wt(" \n"[i+1==n]);
  }
}
