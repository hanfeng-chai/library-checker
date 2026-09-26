#include <vector>
#include <array>
#include <iostream>
#include <cassert>
#define range(i, l, r) for(int i=l;i<r;i++)
using namespace std;
void io_init(){
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);
}
template<typename T>
std::ostream &operator<<(std::ostream &dest, const std::vector<T> &v){
  int sz = v.size();
  if(sz==0) return dest;
  for(int i=0;i<sz-1;i++) dest << v[i] << ' ';
  dest << v[sz-1];
  return dest;
}
template<typename T, int BITLEN>
struct binary_basis{
  int r, zero;
  T val[BITLEN];
  binary_basis(): r(0), zero(0){}
  binary_basis(T x): r(0), zero(0){
    if(x) val[r++] = x;
    else zero++;
  }
  // 要素数
  int size(){
    return r + zero;
  }
  // ランク
  int rank(){
    return r;
  }
  // マージ O(BITLEN ^ 2)
  void merge(const binary_basis<T, BITLEN> &v){
    for(int i = 0; i < v.r; i++) push(v.val[i]);
  }
  // xを追加, O(BITLEN)
  void push(T x){
    for(int j = 0; j < r; j++) x = std::min(x, x ^ val[j]);
    if(x){
      val[r++] = x;
      for(int j = r - 1; j && val[j] > val[j - 1]; j--) std::swap(val[j], val[j - 1]);
    }else zero++;
    assert(r <= BITLEN);
  }
  // xを追加, O(BITLEN)
  // msbがiの要素がある -> 他の全ての要素のiのビットが0になる　　ように変形
  void push2(T x){
    for(int j = 0; j < r; j++) x = std::min(x, x ^ val[j]);
    if(x){
      val[r++] = x;
      int j = r - 1;
      for(; j && val[j] > val[j - 1]; j--) std::swap(val[j], val[j - 1]);
      for(int i = j + 1; i < r; i++) val[j] = std::min(val[j], val[j] ^ val[i]);
      for(int i = j - 1; i >= 0; i--) val[i] = std::min(val[i], val[i] ^ val[j]);
    }else zero++;
    assert(r <= BITLEN);
  }
  // 0個以上の要素のxorで表せる最大値, O(BITLEN)
  T max_xor(){
    T ans = 0;
    for(int i = 0; i < r; i++) ans = std::max(ans, ans ^ val[i]);
    return ans;
  }
  // 1個以上の要素のxorで表せる最小値, O(1)
  T min_xor(){
    assert(r);
    return val[r - 1];
  }
  // 0個以上のxorでxを作れるか
  bool equation(T x){
    for(int i = 0; i < r; i++) x = std::min(x, x ^ val[i]);
    return x == 0;
  }
};



#include <unistd.h>
struct IO{
  static constexpr int ibufsize = 1 << 27;
  static constexpr int obufsize = 1 << 27;
  char ibuf[ibufsize], obuf[obufsize];
  char *ip, *op;
  IO(): ip(ibuf), op(obuf){ for(int t = 0, k = 0; (k = read(STDIN_FILENO, ibuf + t, sizeof(ibuf) - t)) > 0; t += k); }
  ~IO(){ for(int t = 0, k = 0; (k = write(STDOUT_FILENO, obuf + t, op - obuf - t)) > 0; t += k);}
  
  long long in(){
    long long x = 0;
    bool neg = false;
    for(; *ip < '+'; ip++) ;
    if(*ip == '-'){ neg = true; ip++;}
    else if(*ip == '+') ip++;
    for(; *ip >= '0'; ip++) x = 10 * x + *ip - '0';
    if(neg) x = -x;
    return x;
  }
  char in_char(){
    for(; *ip < '!'; ip++) ;
    return *ip++;
  }
  void out(long long x, char c = 0){
    static char tmp[20];
    if(!x) *op++ = '0';
    else{
      int i;
      if(x < 0){
        *op++ = '-';
        x = -x;
      }
      for(i = 0; x; i++){
        tmp[i] = x % 10;
        x /= 10;
      }
      for(i--; i >= 0; i--) *op++ = tmp[i]+'0';
    }
    if(c) *op++ = c;
  }
  void out_char(char x, char c = 0){
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


int main(){
  int T = io.in();
  range(t, 0, T){
    binary_basis<long long, 60> A;
    int n = io.in();
    range(i, 0, n){
      long long x = io.in();
      A.push((x << 30) + x);
    }
    int m = io.in();
    range(i, 0, m){
      long long x = io.in();
      A.push(x << 30);
    }
    vector<int> ans;
    range(i, 0, A.r){
      long long x = A.val[i];
      if(x < (1LL << 30)) ans.push_back(x);
    }
    if(ans.empty()){
      io.out(0, '\n');
    }else{
      io.out(ans.size(), ' ');
      range(j, 0, ans.size()) io.out(ans[j], (j + 1 == ans.size() ? '\n' : ' '));
    }
  }
}
