#line 1 "b.cpp"
#include <vector>
#include <iostream>
#include <cassert>
#include <tuple>

template<typename monoid, typename Val>
struct dynamic_sequence_merge_split{
private:
  struct node{
    node *l, *r;
    int sz;
    bool flip;
    Val val, sum;
    node(Val _val = monoid::template id<Val>()): l(nullptr), r(nullptr), sz(1),
    flip(false), val(_val), sum(_val){}
  };
  node *root;
  int size(node *v){
    return !v ? 0 : v->sz;
  }
  void update(node *v){
    v->sz = 1;
    v->sum = v->val;
    if(v->l){
      v->sz += v->l->sz;
      v->sum = monoid::template merge<Val>(v->l->sum, v->sum);
    }
    if(v->r){
      v->sz += v->r->sz;
      v->sum = monoid::template merge<Val>(v->sum, v->r->sum);
    }
  }
  void push_down(node *v){
    if(!v) return;
    if(v->flip){
      if(v->l) flip(v->l);
      if(v->r) flip(v->r);
      v->flip = false;
    }
  }
  void flip(node *v){
    std::swap(v->l, v->r);
    v->sum = monoid::template flip<Val>(v->sum);
    v->flip ^= 1;
  }
  // vの左の子をvの位置に持ってくる
  node *rotate_right(node *v){
    node *l = v->l;
    v->l = l->r;
    l->r = v;
    update(v);
    update(l);
    return l;
  }
  // vの右の子をvの位置に持ってくる
  node *rotate_left(node *v){
    node *r = v->r;
    v->r = r->l;
    r->l = v;
    update(v);
    update(r);
    return r;
  }
  // k番目(0 <= k < size)のノードを根にする
  node *splay_top_down(node *v, int k){
    push_down(v);
    int szl = v->l ? v->l->sz : 0;
    if(k == szl) return v;
    if(k < szl){
      v->l = splay_top_down(v->l, k);
      v = rotate_right(v);
    }else{
      v->r = splay_top_down(v->r, k - szl - 1);
      v = rotate_left(v);
    }
    update(v);
    return v;
  }
  node *merge(node *l, node *r){
    if(!l || !r) return !l ? r : l;
    r = splay_top_down(r, 0);
    r->l = l;
    update(r);
    return r;
  }
  // 左がkノードになるように分割
  std::pair<node*, node*> split(node *v, int k){
    int n = size(v);
    if(k >= n) return {v, nullptr};
    v = splay_top_down(v, k);
    node *l = v->l;
    v->l = nullptr;
    update(v);
    return {l, v};
  }
  std::tuple<node*, node*, node*> split3(node *v, int l, int r){
    if(l == 0){
      auto [b, c] = split(v, r);
      return {nullptr, b, c};
    }
    v = splay_top_down(v, l - 1); //    (l - 1個)  /  v  / (残り)
    auto [b, c] = split(v->r, r - l); // cがnullptrまたはcの左が空
    v->r = nullptr; // vの右が空
    update(v);
    return {v, b, c};
  }
  // split3によって分割された組でないと壊れる
  node *merge3(node *a, node *b, node *c){
    node *v = merge(b, c); // O(1)
    if(!a) return v;
    a->r = v; // O(1)
    update(a);
    return a;
  }
  node *set_inner(node *v, int k, Val x){
    v = splay_top_down(v, k);
    v->val = x;
    update(v);
    return v;
  }
  node *get_inner(node *v, int k, Val &x){
    v = splay_top_down(v, k);
    x = v->val;
    return v;
  }
  node *query_inner(node *v, int l, int r, Val &res){
    if(r == l) return v;
    auto [a, b, c] = split3(v, l, r);
    res = b->sum;
    return merge3(a, b, c);
  }
  node *flip_inner(node *v, int l, int r){
    if(r == l) return v;
    auto [a, b, c] = split3(v, l, r);
    if(b) flip(b);
    return merge3(a, b, c);
  }
  node *insert_inner(node *v, int k, node *u){
    if(k == size(v)){
      u->l = v;
      update(u);
      return u;
    }
    if(k == 0){
      u->r = v;
      update(u);
      return u;
    }
    v = splay_top_down(v, k);
    u->l = v->l;
    v->l = u;
    update(u);
    update(v);
    return v;
  }
  node *erase_inner(node *v, int k){
    v = splay_top_down(v, k);
    return merge(v->l, v->r);
  }
  node *build(const std::vector<Val> &v, int l, int r){
    int m = (l + r) >> 1;
    node *u = new node(v[m]);
    if(m > l) u->l = build(v, l, m);
    if(r > m + 1) u->r = build(v, m + 1, r);
    update(u);
    return u;
  }
  dynamic_sequence_merge_split(node *r): root(r){}
public:
  dynamic_sequence_merge_split(): root(nullptr){}
  dynamic_sequence_merge_split(const std::vector<Val> &v): root(nullptr){
    if(!v.empty()) root = build(v, 0, v.size());
  }
  int size(){return size(root);}
  void set(int k, Val x){
    assert(0 <= k && k < size());
    root = set_inner(root, k, x);
  }
  Val get(int k){
    assert(0 <= k && k < size());
    Val res = monoid::template id<Val>();
    root = get_inner(root, k, res);
    return res;
  }
  /*
  void update(int l, int r, Lazy x){
    assert(0 <= l && r <= size());
    root = update_inner(root, l, r, x);
  }
  */
  Val query(int l, int r){
    assert(0 <= l && r <= size());
    Val res = monoid::template id<Val>();
    root = query_inner(root, l, r, res);
    return res;
  }
  void flip(int l, int r){
    assert(0 <= l && r <= size());
    root = flip_inner(root, l, r);
  }
  void insert(int k, Val x){
    assert(0 <= k && k <= size());
    root = insert_inner(root, k, new node(x));
  }
  void erase(int k){
    assert(0 <= k && k < size());
    root = erase_inner(root, k);
  }
};

template<typename monoid, typename Val, typename Lazy>
struct lazy_dynamic_sequence_merge_split{
private:
  struct node{
    node *l, *r;
    int sz;
    bool flip;
    Val val, sum;
    Lazy lazy;
    node(Val _val = monoid::template id1<Val>()): l(nullptr), r(nullptr), sz(1),
    flip(false), val(_val), sum(_val), lazy(monoid::template id2<Lazy>()){}
  };
  node *root;
  int size(node *v){
    return !v ? 0 : v->sz;
  }
  void update(node *v){
    v->sz = 1;
    v->sum = v->val;
    if(v->l){
      v->sz += v->l->sz;
      v->sum = monoid::template merge<Val>(v->l->sum, v->sum);
    }
    if(v->r){
      v->sz += v->r->sz;
      v->sum = monoid::template merge<Val>(v->sum, v->r->sum);
    }
  }
  void push_down(node *v){
    if(!v) return;
    if(v->lazy != monoid::template id2<Lazy>()){
      if(v->l) propagate(v->l, v->lazy);
      if(v->r) propagate(v->r, v->lazy);
      v->lazy = monoid::template id2<Lazy>();
    }
    if(v->flip){
      if(v->l) flip(v->l);
      if(v->r) flip(v->r);
      v->flip = false;
    }
  }
  void propagate(node *v, Lazy x){
    v->lazy = monoid::template propagate<Lazy>(v->lazy, x);
    v->val = monoid::template apply<Val, Lazy>(v->val, x, 0, 1);
    v->sum = monoid::template apply<Val, Lazy>(v->sum, x, 0, v->sz);
  }
  void flip(node *v){
    std::swap(v->l, v->r);
    v->sum = monoid::template flip<Val>(v->sum);
    v->flip ^= 1;
  }
  // vの左の子をvの位置に持ってくる
  node *rotate_right(node *v){
    node *l = v->l;
    v->l = l->r;
    l->r = v;
    update(v);
    update(l);
    return l;
  }
  // vの右の子をvの位置に持ってくる
  node *rotate_left(node *v){
    node *r = v->r;
    v->r = r->l;
    r->l = v;
    update(v);
    update(r);
    return r;
  }
  // k番目(0 <= k < size)のノードを根にする
  node *splay_top_down(node *v, int k){
    push_down(v);
    int szl = v->l ? v->l->sz : 0;
    if(k == szl) return v;
    if(k < szl){
      v->l = splay_top_down(v->l, k);
      v = rotate_right(v);
    }else{
      v->r = splay_top_down(v->r, k - szl - 1);
      v = rotate_left(v);
    }
    update(v);
    return v;
  }
  node *merge(node *l, node *r){
    if(!l || !r) return !l ? r : l;
    r = splay_top_down(r, 0);
    r->l = l;
    update(r);
    return r;
  }
  // 左がkノードになるように分割
  std::pair<node*, node*> split(node *v, int k){
    int n = size(v);
    if(k >= n) return {v, nullptr};
    v = splay_top_down(v, k);
    node *l = v->l;
    v->l = nullptr;
    update(v);
    return {l, v};
  }
  std::tuple<node*, node*, node*> split3(node *v, int l, int r){
    if(l == 0){
      auto [b, c] = split(v, r);
      return {nullptr, b, c};
    }
    v = splay_top_down(v, l - 1); //    (l - 1個)  /  v  / (残り)
    auto [b, c] = split(v->r, r - l); // cがnullptrまたはcの左が空
    v->r = nullptr; // vの右が空
    update(v);
    return {v, b, c};
  }
  // split3によって分割された組でないと壊れる
  node *merge3(node *a, node *b, node *c){
    node *v = merge(b, c); // O(1)
    if(!a) return v;
    a->r = v; // O(1)
    update(a);
    return a;
  }
  node *set_inner(node *v, int k, Val x){
    v = splay_top_down(v, k);
    v->val = x;
    update(v);
    return v;
  }
  node *get_inner(node *v, int k, Val &x){
    v = splay_top_down(v, k);
    x = v->val;
    return v;
  }
  node *update_inner(node *v, int l, int r, Lazy x){
    if(r == l) return v;
    auto [a, b, c] = split3(v, l, r);
    propagate(b, x);
    return merge3(a, b, c);
  }
  node *query_inner(node *v, int l, int r, Val &res){
    if(r == l) return v;
    auto [a, b, c] = split3(v, l, r);
    res = b->sum;
    return merge3(a, b, c);
  }
  node *flip_inner(node *v, int l, int r){
    if(r == l) return v;
    auto [a, b, c] = split3(v, l, r);
    if(b) flip(b);
    return merge3(a, b, c);
  }
  node *insert_inner(node *v, int k, node *u){
    if(k == size(v)){
      u->l = v;
      update(u);
      return u;
    }
    if(k == 0){
      u->r = v;
      update(u);
      return u;
    }
    v = splay_top_down(v, k);
    u->l = v->l;
    v->l = u;
    update(u);
    update(v);
    return v;
  }
  node *erase_inner(node *v, int k){
    v = splay_top_down(v, k);
    return merge(v->l, v->r);
  }
  node *build(const std::vector<Val> &v, int l, int r){
    int m = (l + r) >> 1;
    node *u = new node(v[m]);
    if(m > l) u->l = build(v, l, m);
    if(r > m + 1) u->r = build(v, m + 1, r);
    update(u);
    return u;
  }
  lazy_dynamic_sequence_merge_split(node *r): root(r){}
public:
  lazy_dynamic_sequence_merge_split(): root(nullptr){}
  lazy_dynamic_sequence_merge_split(const std::vector<Val> &v): root(nullptr){
    if(!v.empty()) root = build(v, 0, v.size());
  }
  int size(){return size(root);}
  void set(int k, Val x){
    assert(0 <= k && k < size());
    root = set_inner(root, k, x);
  }
  Val get(int k){
    assert(0 <= k && k < size());
    Val res = monoid::template id1<Val>();
    root = get_inner(root, k, res);
    return res;
  }
  void update(int l, int r, Lazy x){
    assert(0 <= l && r <= size());
    root = update_inner(root, l, r, x);
  }
  Val query(int l, int r){
    assert(0 <= l && r <= size());
    Val res = monoid::template id1<Val>();
    root = query_inner(root, l, r, res);
    return res;
  }
  void flip(int l, int r){
    assert(0 <= l && r <= size());
    root = flip_inner(root, l, r);
  }
  void insert(int k, Val x){
    assert(0 <= k && k <= size());
    root = insert_inner(root, k, new node(x));
  }
  void erase(int k){
    assert(0 <= k && k < size());
    root = erase_inner(root, k);
  }
};

struct range_add_range_sum{
  template<typename T>
  static T id1(){
    return T(0);
  }
  template<typename E>
  static E id2(){
    return E(0);
  }
  template<typename T>
  static T merge(T a, T b){
    return a + b;
  }
  template<typename T, typename E>
  static T apply(T a, E b, int l, int r){
    return a + b * (r - l);
  }
  template<typename E>
  static E propagate(E a, E b){
    return a + b;
  }
  template<typename T>
  static T flip(T a){
    return a;
  }
};
struct point_add_range_sum{
  template<typename T>
  static T id(){
    return 0;
  }
  template<typename T>
  static T update(T a, T b){
    return a + b;
  }
  template<typename T>
  static T merge(T a, T b){
    return a + b;
  }
  template<typename T>
  static T flip(T a){
    return a;
  }
};

#line 2 ".lib/fast_io.hpp"
#include <unistd.h>

struct IO {
  static const int bufsize=1<<25;
  char ibuf[bufsize], obuf[bufsize];
  char *ip, *op;
  IO(): ip(ibuf), op(obuf) { for(int t = 0, k = 0; (k = read(STDIN_FILENO, ibuf+t, sizeof(ibuf)-t)) > 0; t+=k); }
  ~IO(){ for(int t = 0, k = 0; (k = write(STDOUT_FILENO, obuf+t, op-obuf-t)) > 0; t+=k); }

  long long in(){
    long long x=0;
    bool neg=false;
    for(;*ip<'+';ip++) ;
    if(*ip=='-'){ neg=true; ip++;}
    else if(*ip=='+') ip++;
    for(;*ip>='0';ip++) x = 10*x+*ip-'0';
    if(neg) x = -x;
    return x;
  }
  char in_char(){
    for(; *ip < '!'; ip++) ;
    return *ip++;
  }
  void out(long long x, char c=0){
    static char tmp[20];
    if(x==0) *op++ = '0';
    else {
      int i;
      if(x<0){
        *op++ = '-';
        x = -x;
      }
      for(i=0; x; i++){
        tmp[i] = x % 10;
        x /= 10;
      }
      for(i--; i>=0; i--) *op++ = tmp[i]+'0';
    }
    if(c) *op++ = c;
  }
  void out_char(char x, char c=0){
    *op++ = x;
    if(c) *op++ = c;
  }
} io;

template<typename T>
void read(T &x){
  x = io.in();
}
char get_char(){
  return io.in_char();
}
void print(long long x, char c = '\0'){
  io.out(x, c);
}
void print_char(char x, char c = '\0'){
  io.out_char(x, c);
}
#line 477 "b.cpp"

int main(){
  int n = io.in();
  int q = io.in();
  std::vector<long long> v(n);
  for(int i = 0; i < n; i++) v[i] = io.in();
  dynamic_sequence_merge_split<point_add_range_sum, long long> t(v);

  for(int i = 0; i < q; i ++){
    int a, b, c;
    a = io.in();
    b = io.in();
    c = io.in();
    if(a == 0){
      t.flip(b, c);
    }else{
      io.out(t.query(b, c), '\n');
    }
  }
}
