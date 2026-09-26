#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;
const ll INF = 1ll << 60;
#define REP(i,n) for(ll i=0; i<ll(n); i++)
template <class T> using V = vector<T>;
template <class A, class B> void chmax(A& l, const B& r){ if(l < r) l = r; }
template <class A, class B> void chmin(A& l, const B& r){ if(r < l) l = r; }

#include <utility>
#include <numeric>

namespace nachia{

template<class Int = long long, class Int2 = long long>
struct VecI2 {
    Int x, y;
    VecI2() : x(0), y(0) {}
    VecI2(std::pair<Int, Int> _p) : x(std::move(_p.first)), y(std::move(_p.second)) {}
    VecI2(Int _x, Int _y) : x(std::move(_x)), y(std::move(_y)) {}
    VecI2& operator+=(VecI2 r){ x+=r.x; y+=r.y; return *this; }
    VecI2& operator-=(VecI2 r){ x-=r.x; y-=r.y; return *this; }
    VecI2& operator*=(Int r){ x*=r; y*=r; return *this; }
    VecI2 operator+(VecI2 r) const { return VecI2(x+r.x, y+r.y); }
    VecI2 operator-(VecI2 r) const { return VecI2(x-r.x, y-r.y); }
    VecI2 operator*(Int r) const { return VecI2(x*r, y*r); }
    VecI2 operator-() const { return VecI2(-x, -y); }
    Int2 operator*(VecI2 r) const { return Int2(x) * Int2(r.x) + Int2(y) * Int2(r.y); }
    Int2 operator^(VecI2 r) const { return Int2(x) * Int2(r.y) - Int2(y) * Int2(r.x); }
    bool operator<(VecI2 r) const { return x < r.x || (!(r.x < x) && y < r.y); }
    Int2 norm() const { return Int2(x) * Int2(x) + Int2(y) * Int2(y); }
    Int2 manhattan() const { return std::abs(x) + std::abs(y); }
    static bool compareYX(VecI2 a, VecI2 b){ return a.y < b.y || (!(b.y < a.y) && a.x < b.x); }
    static bool compareXY(VecI2 a, VecI2 b){ return a.x < b.x || (!(b.x < a.x) && a.y < b.y); }
    bool operator==(VecI2 r) const { return x == r.x && y == r.y; }
    bool operator!=(VecI2 r) const { return x != r.x || y != r.y; }
    Int gcd() const { return std::gcd(std::abs(x), std::abs(y)); }
    friend std::istream& operator>>(std::istream& istr, VecI2& d){ return istr >> d.x >> d.y; }
    friend std::ostream& operator<<(std::ostream& ostr, const VecI2& d){ return ostr << "(" << d.x << " " << d.y << ")"; }
};

} // namespace nachia
using Vec2 = nachia::VecI2<ll>;

void testcase(){
  ll N; cin >> N;
  V<Vec2> A(N*2);
  REP(i,N){ cin >> A[i]; A[N+i] = A[i]; }
  V<ll> arg(N+2), val(N+2, -INF);
  arg[N+1] = N-1;
  ll qlim = 1; while(qlim * 2 <= N) qlim *= 2;
  for(ll q=qlim; q>0; q/=2) for(ll m=q; m<=N; m+=q*2){
    ll l = m - q, r = min(m + q, N + 1);
    ll li = max<ll>(0, arg[l]-q), ri = min<ll>(arg[r]+q, N-1);
    for(ll p=li; p<=ri; p++){
      ll d = (A[m-1] - A[(m-1+p)]).norm();
      if(val[m] < d){ arg[m] = p; val[m] = d; }
    }
  }
  REP(i,N){
    if(i) cout << " ";
    cout << ((i + arg[i+1]) % N);
  } cout << "\n";
}

int main(){
  cin.tie(0)->sync_with_stdio(0);
  ll T; cin >> T; REP(t,T)
  testcase();
  return 0;
}
