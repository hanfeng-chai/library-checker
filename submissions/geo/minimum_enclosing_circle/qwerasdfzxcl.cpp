#include <bits/stdc++.h>

using namespace std;
using ll = long long;

namespace cover_2d{
  const double EPS = 1e-13;
  using point = complex<double>;
  struct circle{ point p; double r; };
  double dist(point p, point q){ return abs(p-q); }
  double area2(point p, point q){ return (conj(p)*q).imag(); }
  bool in(const circle& c, point p){ // dist <= r*(1+EPS)
    return c.r >= 0 && norm(c.p-p) <= c.r*c.r*(1+2*EPS); }
  bool on(const circle& c, point p){
    return c.r >= 0 && norm(c.p-p) <= c.r*c.r*(1+2*EPS) && norm(c.p-p) >= c.r*c.r*(1-2*EPS);
  }
  const circle INVAL = circle{point(0, 0), -1};
  circle mcc(point a, point b, point c){
    b -= a; c -= a;
    double d = 2*area2(b, c); if(d==0) return INVAL;
    point ans = (c*norm(b) - b*norm(c)) * point(0, -1) / d;
    return circle{a + ans, abs(ans)};
  }
  circle solve(vector<point> p){
    mt19937 gen(0x94949); shuffle(p.begin(), p.end(), gen);
    point o = p.size() ? p[0] : 0; for(auto &q : p) q -= o;
    circle c = INVAL;
    for(int i=0; i<p.size(); ++i) if(!in(c, p[i])){
      c = circle{p[i], 0};
      for(int j=0; j<i; ++j) if(!in(c, p[j])){
        c = circle{(p[i]+p[j])*0.5, dist(p[i], p[j])*0.5};
        for(int k=0; k<j; ++k) if(!in(c, p[k]))
          c = mcc(p[i], p[j], p[k]);
      }
    }
    c.p += o; return c;
  }
};

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;

    vector<complex<double>> a;
    for (int i=1;i<=n;i++){
        int x, y;
        cin >> x >> y;
        a.emplace_back(x, y);
    }

    auto ans = cover_2d::solve(a);

    for (int i=0;i<n;i++){
        if (cover_2d::on(ans, a[i])) printf("1");
        else printf("0");
    }
    printf("\n");
}