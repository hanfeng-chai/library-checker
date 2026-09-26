#include <bits/stdc++.h>
using namespace std;
#if __has_include(<atcoder/all>)
#include <atcoder/all>
#endif
using ll=long long;
using ull=unsigned long long;
using P=pair<ll,ll>;
template<typename T>using minque=priority_queue<T,vector<T>,greater<T>>;
template<typename T>bool chmax(T &a,const T &b){return (a<b?(a=b,true):false);}
template<typename T>bool chmin(T &a,const T &b){return (a>b?(a=b,true):false);}
template<typename T1,typename T2>istream &operator>>(istream &is,pair<T1,T2>&p){is>>p.first>>p.second;return is;}
template<typename T>istream &operator>>(istream &is,vector<T> &a){for(auto &i:a)is>>i;return is;}
template<typename T1,typename T2>void operator++(pair<T1,T2>&a,int n){a.first++,a.second++;}
template<typename T1,typename T2>void operator--(pair<T1,T2>&a,int n){a.first--,a.second--;}
template<typename T>void operator++(vector<T>&a,int n){for(auto &i:a)i++;}
template<typename T>void operator--(vector<T>&a,int n){for(auto &i:a)i--;}
#define reps(i,a,n) for(int i=(a);i<int(n);i++)
#define rep(i,n) reps(i,0,n)
#define all(x) x.begin(),x.end()
#define pcnt(x) __builtin_popcountll(x)
#define fin(x) return cout<<x<<'\n',static_cast<void>(0)
ll myceil(ll a,ll b){return (a+b-1)/b;}
template<typename T,size_t n,size_t id=0>
auto vec(const int (&d)[n],const T &init=T()){
  if constexpr (id<n)return vector(d[id],vec<T,n,id+1>(d,init));
  else return init;
}
#ifdef LOCAL
#include<debug.h>
#else
#define debug(...) static_cast<void>(0)
#define debugg(...) static_cast<void>(0)
template<typename T1,typename T2>ostream &operator<<(ostream &os,const pair<T1,T2>&p){os<<p.first<<' '<<p.second;return os;}
#endif
void SOLVE();
int main(){
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout<<fixed<<setprecision(16);
  #ifdef LOCAL
  clock_t start=clock();
  #endif
  int testcase=1;
  //cin>>testcase;
  for(int i=0;i<testcase;i++){
    SOLVE();
  }
  #ifdef LOCAL
  cerr<<"time:";
  cerr<<(clock()-start)/1000;
  cerr<<"ms\n";
  #endif
}
template<typename T>
struct ReversibleSequence{
private:
  struct node{
    static node nil;
    node *left,*right;
    int sz;
    T v,sum;
    bool rev;
    node():left(this),right(this),sz(0),v(0),sum(0),rev(false){}
    node(const T&x):left(&nil),right(&nil),sz(1),v(x),sum(x),rev(false){}
    inline void push(){
      if(rev){
        left->reverse();
        right->reverse();
        rev=false;
      }
    }
    inline void reverse(){
      if(this==&nil)return;
      rev^=1;
      swap(left,right);
    }
  };
  void splay(node*&nd,int k){
    #define upd(x) x->sum=x->left->sum+x->right->sum+x->v;x->sz=1+x->left->sz+x->right->sz;
    #define only_left(x)x->sum=x->left->sum+x->v;x->sz=1+x->left->sz;
    #define only_right(x)x->sum=x->right->sum+x->v;x->sz=1+x->right->sz;
    #define r_upd rsize+=rnd->sz;rsum+=rnd->sum;
    #define l_upd lsize+=lnd->sz;lsum+=lnd->sum;
    node *lnd=&node::nil,*rnd=&node::nil;
    int lsize=0,rsize=0;
    T lsum=0,rsum=0;
    while(k!=nd->left->sz){
      nd->push();
      if(k<nd->left->sz){
        node *ch=nd->left;
        ch->push();
        if(k==ch->left->sz){
          rnd->left=nd;
          nd=ch;
          rnd=rnd->left;
          only_right(rnd)
          r_upd
        }
        else if(k<ch->left->sz){
          node *lele=ch->left;
          nd->left=ch->right;
          ch->right=nd;
          upd(nd)
          only_right(ch)
          rnd->left=ch;
          rnd=ch;
          nd=lele;
          r_upd
        }
        else{
          k-=1+ch->left->sz;
          node *leri=ch->right;
          lnd->right=ch;
          lnd=lnd->right;
          rnd->left=nd;
          rnd=rnd->left;
          nd=leri;
          only_left(lnd)
          only_right(rnd)
          l_upd
          r_upd
        }
      }
      else{
        k-=1+nd->left->sz;
        node *ch=nd->right;
        ch->push();
        if(k==ch->left->sz){
          lnd->right=nd;
          nd=ch;
          lnd=lnd->right;
          only_left(lnd)
          l_upd
        }
        else if(k<ch->left->sz){
          node *rile=ch->left;
          lnd->right=nd;
          lnd=lnd->right;
          rnd->left=ch;
          rnd=rnd->left;
          nd=rile;
          only_left(lnd)
          only_right(rnd)
          l_upd
          r_upd
        }
        else{
          k-=1+ch->left->sz;
          node *riri=ch->right;
          nd->right=ch->left;
          ch->left=nd;
          upd(nd)
          only_left(ch)
          lnd->right=ch;
          lnd=ch;
          nd=riri;
          l_upd
        }
      }
    }
    debug(lsize,rsize,lsum,rsum);
    nd->push();
    lsize+=nd->left->sz,rsize+=nd->right->sz;
    lsum+=nd->left->sum,rsum+=nd->right->sum;
    nd->sum=nd->v+lsum+rsum;
    nd->sz=1+lsize+rsize;
    debug(lsize,lsum);
    debug(rsize,rsum);
    node *ptr;
    if(lnd!=&node::nil){
      lnd->right=nd->left;
      for(ptr=node::nil.right;ptr!=&node::nil;ptr=ptr->right){
        lsize-=ptr->sz;
        lsum-=ptr->sum;
        ptr->sz+=lsize;
        ptr->sum+=lsum;
        if(ptr==lnd)break;
      }
      nd->left=node::nil.right;
    }
    if(rnd!=&node::nil){
      rnd->left=nd->right;
      for(ptr=node::nil.left;ptr!=&node::nil;ptr=ptr->left){
        rsize-=ptr->sz;
        rsum-=ptr->sum;
        ptr->sz+=rsize;
        ptr->sum+=rsum;
        if(ptr==rnd)break;
      }
      nd->right=node::nil.left;
    }
    #undef upd
    #undef only_left
    #undef only_right
    #undef l_upd
    #undef r_upd
  }
  node* merge(node *l,node *r){
    if(l==&node::nil)return r;
    if(r==&node::nil)return l;
    splay(r,0);
    r->left=l;
    r->update();
    return r;
  }
  node *root;
public:
  ReversibleSequence():root(nullptr){}
  ReversibleSequence(const vector<T>&init){
    if(init.empty())root=nullptr;
    else{
      auto dfs=[&](auto self,int l,int r)->node* {
        if(l==r)return &node::nil;
        if(l+1==r)return new node(init[l]);
        int mid=(l+r)>>1;
        node *nd=new node(init[mid]);
        nd->left=self(self,l,mid);
        nd->right=self(self,mid+1,r);
        nd->sum=nd->left->sum+nd->right->sum+init[mid];
        nd->sz=1+nd->left->sz+nd->right->sz;
        return nd;
      };
      root=dfs(dfs,0,init.size());
    }
  }
  void insert(int i,const T&x){
    if(!root)root=new node(x);
    else if(root->sz==i){
      node *nd=new node(x);
      nd->left=root;
      root=nd;
      root->sz=1+root->left->sz;
      root->sum=root->left->sum+x;
    }
    else{
      splay(root,i);
      node *nd=new node(x);
      nd->left=root->left;
      nd->right=root;
      root->left=&node::nil;
      root->sz=1+root->right->sz;
      root->sum=root->v+root->right->sum;
      root=nd;
      root->sz=1+root->left->sz+root->right->sz;
      root->sum=root->v+root->left->sum+root->right->sum;
    }
  }
  void erase(int i){
    splay(root,i);
    node *le=root->left,*ri=root->right;
    delete(root);
    root=merge(le,ri);
  }
  void set(int i,const T&x){
    splay(root,i);
    root->v=x;
    root->sum=root->left->sum+x+root->right->sum;
  }
  T get(int i){
    splay(root,i);
    return root->v;
  }
  void reverse(int l,int r){
    if(r-l<=1)return;
    if(l==0&&r==root->sz)root->reverse();
    else if(l==0){
      splay(root,r);
      root->left->reverse();
    }
    else if(r==root->sz){
      splay(root,l-1);
      root->right->reverse();
    }
    else{
      splay(root,l-1);
      splay(root->right,r-l);
      root->right->left->reverse();
    }
  }
  T sum(int l,int r){
    if(l==r)return 0;
    if(l==0&&r==root->sz)return root->sum;
    else if(l==0){
      splay(root,r);
      return root->left->sum;
    }
    else if(r==root->sz){
      splay(root,l-1);
      return root->right->sum;
    }
    else{
      splay(root,l-1);
      splay(root->right,r-l);
      return root->right->left->sum;
    }
  }
  void print(){
    map<node*,int>mp;
    int idx=0;
    auto add=[&](auto self,node *nd)->void {
      if(nd==&node::nil)return;
      mp[nd]=idx++;
      self(self,nd->left);
      self(self,nd->right);
    };
    add(add,root);
    mp[&node::nil]=-1;
    auto dfs=[&](auto self,node *nd)->void {
      if(nd==&node::nil)return;
      debug(mp[nd],mp[nd->left],mp[nd->right],nd->sz,nd->v,nd->sum,nd->rev);
      self(self,nd->left);
      self(self,nd->right);
    };
    dfs(dfs,root);
  }
};
template<typename T>
typename ReversibleSequence<T>::node ReversibleSequence<T>::node::nil;
void SOLVE(){
  int n,q;
  cin>>n>>q;
  vector<ll>a(n);
  cin>>a;
  ReversibleSequence<ll>s(a);
  while(q--){
    int t,l,r;
    cin>>t>>l>>r;
    if(t==0)s.reverse(l,r);
    else cout<<s.sum(l,r)<<'\n';
  }
}
