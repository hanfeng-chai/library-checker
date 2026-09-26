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
#include<climits>
template<int mx=CHAR_MAX>
struct eertree{
  struct node{
    node* child[mx];
    node *link,*par;
    int idx,l,r;
    node(node *link_,int idx_,int l_,int r_):link(link_),par(nullptr),idx(idx_),l(l_),r(r_){for(int i=0;i<mx;i++)child[i]=nullptr;}
  };
  std::vector<node>nds;
  std::vector<node*>belong;
  eertree(const std::string&s):belong(s.size()+1){
    nds.reserve(s.size()+2);
    nds.emplace_back(nullptr,0,0,-1);
    nds.emplace_back(&nds[0],1,0,0);
    nds[1].par=&nds[0];
    node *p=&nds[0];
    for(int i=0;i<(int)s.size();i++){
      belong[i]=p;
      while(p!=&nds[0]){
        int j=i-1-p->r+p->l;
        if(j>=0&&s[i]==s[j])break;
        p=p->link;
      }
      if(p->child[s[i]]){
        p=p->child[s[i]];
        continue;
      }
      int to=nds.size();
      int l=i-1-p->r+p->l;
      int r=i+1;
      node*nxt=&nds.emplace_back(nullptr,nds.size(),i-1-p->r+p->l,i+1);
      nxt->par=p;
      p->child[s[i]]=nxt;
      if(p==&nds[0])nxt->link=&nds[1];
      else{
        while(1){
          p=p->link;
          int j=i-1-p->r+p->l;
          if(j>=0&&s[i]==s[j])break;
        }
        nxt->link=p->child[s[i]];
      }
      p=nxt;
    }
    belong[s.size()]=p;
  }
  node* operator[](int i){return belong[i];}
  int distinct_palindrome()const{return nds.size()-2;}
};
int main(){
  std::string s;
  rd(s);
  for(char&c:s)c-='a';
  eertree<26>st(s);
  wt(st.distinct_palindrome()),wt('\n');
  for(const auto&nd:st.nds)if(nd.idx>=2)wt(nd.par->idx-1),wt(' '),wt(nd.link->idx-1),wt('\n');
  for(int i=0;i<(int)s.size();i++)wt(st[i+1]->idx-1),wt(" \n"[i+1==(int)s.size()]);
}
