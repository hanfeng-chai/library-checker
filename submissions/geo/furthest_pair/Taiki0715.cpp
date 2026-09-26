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
#include<cassert>
#include<vector>
#include<algorithm>
#include<queue>
#include<numeric>
#include<iostream>
template<typename T>constexpr std::enable_if_t<std::is_floating_point_v<T>,T> epsilon(){return 1e-10;}
template<typename T>constexpr std::enable_if_t<std::is_integral_v<T>,T>epsilon(){return 0;}
template<typename T>
struct Point{
  static_assert(std::is_arithmetic_v<T>);
  static constexpr T eps=epsilon<T>();
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
  friend bool operator==(const Point&lhs,const Point&rhs){return std::abs(lhs.x-rhs.x)<=eps&&std::abs(lhs.y-rhs.y)<=eps;}
  friend bool operator!=(const Point&lhs,const Point&rhs){return !(lhs==rhs);}
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
  Point<T>rot90(){
    return Point(-y,x);
  }
  T norm()const{return x*x+y*y;}
};
template<typename T>
T cross(const Point<T>&a,const Point<T>&b){return a.x*b.y-a.y*b.x;}
template<typename T>
T dot(const Point<T>&a,const Point<T>&b){return a.x*b.x+a.y*b.y;}
template<typename T,bool LOWERHULL=true>
struct ConvexHullTrick{
  static constexpr T inf=std::numeric_limits<T>::max();
  static constexpr T eps=epsilon<T>();
  static inline bool is_concave(const Point<T>&l,const Point<T>&m,const Point<T>&r){
    if constexpr(LOWERHULL)return cross<T>(m-l,r-l)>eps;
    else return cross<T>(m-l,r-l)<eps;
  }
  std::deque<Point<T>>que;
  static constexpr Point<T>none{inf,inf};
  ConvexHullTrick():que(){}
  Point<T>add_left(const Point<T>&v){
    if(que.empty()){
      que.push_front(v);
      return none;
    }
    while(que.size()>=2){
      const Point<T>&m=que[0];
      const Point<T>&r=que[1];
      if(is_concave(v,m,r))break;
      que.pop_front();
    }
    Point<T>res=que.front();
    que.push_front(v);
    return res;
  }
  Point<T>add_right(const Point<T>&v){
    if(que.empty()){
      que.push_back(v);
      return none;
    }
    while(que.size()>=2){
      const Point<T>&l=que[que.size()-2];
      const Point<T>&m=que.back();
      if(is_concave(l,m,v))break;
      que.pop_back();
    }
    Point<T>res=que.back();
    que.push_back(v);
    return res;
  }
};
template<typename T>
std::vector<Point<T>>static_convex_hull(std::vector<Point<T>>points){
  std::sort(points.begin(),points.end(),[](const Point<T>&lhs,const Point<T>&rhs){return lhs.x==rhs.x?lhs.y<rhs.y:lhs.x<rhs.x;});
  points.erase(std::unique(points.begin(),points.end()),points.end());
  if(points.size()<=2)return points;
  ConvexHullTrick<T,true>lower;
  ConvexHullTrick<T,false>upper;
  for(const Point<T>&p:points)lower.add_right(p),upper.add_right(p);
  std::vector<Point<T>>res(lower.que.begin(),lower.que.end());
  res.insert(res.end(),upper.que.rbegin()+1,upper.que.rend()-1);
  return res;
}
template<typename T>
std::pair<int,int>furthest_pair_points(const std::vector<Point<T>>&points){
  assert(points.size()>=2);
  std::vector<Point<T>>ch=static_convex_hull(points);
  if(ch.size()==1)return std::make_pair(0,1);
  int n=ch.size();
  ch.insert(ch.end(),ch.begin(),ch.end());
  T dist=0;
  std::pair<Point<T>,Point<T>>best;
  for(int i=0,j=1;i<n;i++){
    while(cross(ch[i+1]-ch[i],ch[j+1]-ch[j])>epsilon<T>())j++;
    T now=(ch[i]-ch[j]).norm();
    if(dist<now)dist=now,best=std::make_pair(ch[i],ch[j]);
  }
  int i,j;
  for(int k=0;k<(int)points.size();k++){
    if(best.first==points[k])i=k;
    if(best.second==points[k])j=k;
  }
  return std::make_pair(i,j);
}
using namespace std;
int main(){
  int t;
  rd(t);
  while(t--){
    int n;
    rd(n);
    std::vector<Point<long long>>a(n);
    for(int i=0;i<n;i++)rd(a[i].x),rd(a[i].y);
    auto [i,j]=furthest_pair_points(a);
    wt(i),wt(' '),wt(j),wt('\n');
  }
}
