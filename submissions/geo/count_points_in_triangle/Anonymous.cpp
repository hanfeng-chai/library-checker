// 5 penalty
#include <bits/stdc++.h>
#define rep2(i, s, n) for(ll i = ll(s); i < ll(n); i++)
#define rep(i, n) rep2(i, 0, n)
#define rrep2(i, n, t) for(ll i = ll(n) - 1; i >= ll(t); i--)
#define rrep(i, n) rrep2(i, n, 0)
#define all(a) a.begin(), a.end()
#define SZ(a) ll(a.size())
#define eb emplace_back
using namespace std;
using ll = long long;
using P = pair<ll, ll>;
using vl = vector<ll>;
using vvl = vector<vl>;
using vp = vector<P>;
using vvp = vector<vp>;
const ll inf = LLONG_MAX / 4;
template<class T>
bool chmin(T& a, T b) { return a > b ? a = b, 1 : 0; }
template<class T>
bool chmax(T& a, T b) { return a < b ? a = b, 1 : 0; }

#define ln '\n'
#define r(a) real(a)
#define i(a) imag(a)

using Pt=complex<ll>;
using vt=vector<Pt>;

ll dot(Pt a,Pt b){return r(a)*r(b)+i(a)*i(b);}
ll cross(Pt a,Pt b){return r(a)*i(b)-i(a)*r(b);}
ll cross(Pt a,Pt b,Pt c){return cross(b-a,c-a);}

Pt input(){
    ll x,y;cin>>x>>y; return Pt(x,y);
}

struct BIT {
    vl a;
    BIT(ll n) : a(n + 1) {}
    void add(ll i, ll x) {  // A[i] += x
       i++;
       while(i < SZ(a)) {
          a[i] += x;
          i += i & -i;
       }
    }
    ll sum(ll r) {
       ll s = 0;
       while(r) {
          s += a[r];
          r -= r & -r;
       }
       return s;
    }
    ll sum(ll l, ll r) {  // sum of A[l, r)
       return sum(r) - sum(l);
    }
 };

bool arg(Pt a,Pt b){
    auto type=[&](Pt p){
        if(i(p)<0 || (i(p)==0 && r(p)<0)) return 0;
        else return 1;
    };
    ll ta=type(a),tb=type(b);
    if(ta==tb) return cross(a,b)>0;
    return ta<tb;
}

// Adopted from https://judge.yosupo.jp/submission/185382
// query(i,j,k)：三角形 AiAjAk の内部 (境界除く) にある Bl について、C_l の総和を返す
// 前計算 O(NMlogM)、クエリ O(1)
struct Count_Points_In_Triangles {
    const int LIM = 1'000'000'000 + 10;
    vt A, B;
    vl val, idx, point;
    vvl seg, tri;
    Count_Points_In_Triangles(vt A, vt B, vl val) : A(A), B(B), val(val) { build();}
  
    ll query(ll i, ll j, ll k) {
        i = idx[i], j = idx[j], k = idx[k];
        if (i > j) swap(i, j);
        if (j > k) swap(j, k);
        if (i > j) swap(i, j);
        ll d = cross(A[i],A[j],A[k]);
        if (d == 0) return 0;
        if (d > 0) { return tri[i][j] + tri[j][k] - tri[i][k] - seg[i][k]; }
        ll x = tri[i][k] - tri[i][j] - tri[j][k];
        return x - seg[i][j] - seg[j][k] - point[j];
    }

    Pt take_origin() {
        random_device rnd;
        mt19937 mt(rnd());
        // OAiAj, OAiBj が同一直線上にならないようにする
        // fail prob: at most N(N+M)/LIM
        return Pt(-LIM, mt()%(LIM*2+1)-LIM);
    }

    template <typename T>
    vector<T> rearrange(vector<T> A, vl I) {
        vector<T> B(SZ(I));
        rep(i, SZ(I)) B[i] = A[I[i]];
        return B;
    }
  
    void build() {
        Pt O = take_origin();
        for (auto& p: A) p -= O;
        for (auto& p: B) p -= O;
        int N = SZ(A), M = SZ(B);
        vl I(N); iota(all(I), 0);
        sort(all(I), [&](ll i, ll j){return arg(A[i],A[j]);});
        A = rearrange(A, I);
        idx.resize(N);
        rep(i,N) idx[I[i]] = i;
        iota(all(I), 0);
        sort(all(I), [&](ll i, ll j){return arg(B[i],B[j]);});
        B = rearrange(B, I);
        val = rearrange(val, I);
        point.assign(N, 0);
        seg.assign(N, vl(N));
        tri.assign(N, vl(N));

        rep(i, N) rep(j, M) if (A[i] == B[j]) point[i] += val[j];

        auto comp = [&](Pt a, Pt b){return cross(a,b) > 0;};

        int m = 0;
        rep(j, N) {
            while (m < M && cross(A[j],B[m]) < 0) ++m;
            vt C(m);
            rep(k, m) C[k] = B[k] - A[j];
            vl I(m);
            rep(i, m) I[i] = i;
            sort(all(I), [&](ll a, ll b){return comp(C[a], C[b]);});
            C = rearrange(C, I);
            vl rk(m);
            rep(k, m) rk[I[k]] = k;

            BIT bit(m);
            ll k = m;
            rrep(i, j) {
                while (k > 0 && cross(A[i], B[k-1]) > 0){
                    k--;
                    bit.add(rk[k], val[k]);
                }
                Pt p = A[i] - A[j];
                ll lb = lower_bound(all(C), p, comp) - C.begin();
                ll ub = upper_bound(all(C), p, comp) - C.begin();
                seg[i][j] += bit.sum(lb, ub), tri[i][j] += bit.sum(lb);
            }
        }
    }
};
  
int main(){
    cin.tie(0)->sync_with_stdio(0);
    ll n;cin>>n;
    vt a(n);
    rep(i,n) a[i]=input();
    ll m;cin>>m;
    vt b(m);
    rep(i,m) b[i]=input();
    ll q;cin>>q;
    Count_Points_In_Triangles count(a,b,vl(m,1));
    while(q--){
        ll a,b,c;cin>>a>>b>>c;
        cout<<count.query(a,b,c)<<ln;
    }
}