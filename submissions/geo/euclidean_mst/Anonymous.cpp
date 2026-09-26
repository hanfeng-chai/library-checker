#include <bits/stdc++.h>

#define rep(i, n) for(ll i = 0; i < ll(n); i++)
#define rep2(i, s, n) for(ll i = ll(s); i < ll(n); i++)
#define rrep(i, n) for(ll i = ll(n) - 1; i >= 0; i--)
#define rrep2(i, n, t) for(ll i = ll(n) - 1; i >= ll(t); i--)
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
bool chmin(auto& a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto& a, auto b) { return a < b ? a = b, 1 : 0; }

using ld = long long;
using Point = complex<ld>;
using std::norm;
using vd = vector<ld>;
using vdd = vector<vd>;
using pdd = pair<ld, ld>;

constexpr ld INF = 1e18;
constexpr ld EPS = 0;

constexpr Point NULL_POINT = (INF, INF);
constexpr Point ORIGIN = (0, 0);

//reversed priority_queue
template<class T>
class prique :public std::priority_queue<T, std::vector<T>, std::greater<T>> {};

inline bool isNullPoint(Point &p){
    return p.real() > INF/2;
}

inline bool equal(const ld &a, const ld &b){
    return abs(a-b) <= EPS;
}

inline bool equal(const Point &a, const Point &b){
    return equal(a.real(), b.real()) && equal(a.imag(), b.imag());
}


Point unitVector(const Point &a){
    return a / abs(a);
}

Point normalVector(const Point &a){
    return a * Point(0,1);
}

ld dot(const Point &a, const Point &b){
    return a.real() * b.real() + a.imag() * b.imag();
}

ld cross(const Point &a, const Point &b){
    return (a.real() * b.imag() - a.imag()*b.real());
}

Point rotate(const Point &p, const ld &theta){
    return p * Point(cos(theta), sin(theta));
}

ld radianToDegree(const ld &radian) { return radian * 180.0 / M_PI; }

ld degreeToRadian(const ld &degree) { return degree * M_PI / 180.0; }

struct Line {
    Point a, b;
    Line() = default;
    Line(Point a, Point b) : a(a), b(b) {}

    // Ax + By = C
    Line(ld A, ld B, ld C){
        if(equal(A, 0)){
            assert(!equal(B, 0));
            a = Point(0, C/B), b = Point(1, C/B);
        }
        else if(equal(B, 0)){
            a = Point(C/A, 0), b = Point(C/A, 1);
        }
        else{
            a = Point(C/A, 0), b = Point(0, C/B);
        }
    }
};

struct Segment : Line{
    Segment() = default;
    Segment(Point a, Point b) : Line(a, b){}
};

struct Circle {
    Point p;
    ld r;

    Circle() = default;

    Circle(Point p, ld r) : p(p), r(r) {}

    bool contain(const Point &q){
        return (abs(q-p) <= r+EPS);
    }
};

Point projection(const Line &l, const Point &p){
    ld t = dot(l.b-l.a, l.b-p) / norm(l.b-l.a);
    return l.a + (l.b-l.a) * t;
}

Point reflection(const Line &l, const Point &p){
    return p + (projection(l, p) - p) * ld(2.0);
}

// a → b → c の位置関係

/**
 * a → b → c の位置関係
 * 
 * @return
 * 1 : 反時計回り
 * -1 : 時計回り
 * 2 : c → a → b
 * -2 : a → b → c
 * 0 : a → c → b
 */
int ccw(const Point &a, Point b, Point c){
    b -= a, c -= a;
    if(cross(b,c) > EPS){
        return 1;
    }
    else if(cross(b,c) < -EPS){
        return -1;
    }
    else if(dot(b,c) < 0){
        return 2;
    }
    else if(norm(b) < norm(c)){
        return -2;
    }
    return 0;
}

// 直行？
bool isOrthogonal(const Line &a, const Line &b){
    return equal(dot(a.b-a.a, b.b-b.a), 0);
}

// 並行？
bool isParallel(const Line &a, const Line &b){
    return equal(cross(a.b-a.a, b.b-b.a), 0);
}

// 交差判定 → 線分 ab に対して c, d が反対側にあるか？
bool isIntersect(const Segment &s, const Segment &t){
    return ccw(s.a, s.b, t.a) * ccw(s.a, s.b, t.b) <= 0
    && ccw(t.a, t.b, s.a) * ccw(t.a, t.b, s.b) <= 0;
}

//交点
Point crossPoint(const Line &s, const Line &t){
    ld d1 = cross(s.b-s.a, t.b-t.a);
    ld d2 = cross(s.b-s.a, s.b-t.a);
    if(equal(fabs(d1), 0)){
        if(equal(fabs(d2), 0)) return t.a;
        else return NULL_POINT;
    }
    return t.a + (t.b-t.a) * d2 / d1;
}

Point crossPoint(const Segment &s, const Segment &t){
    if(!isIntersect(s, t)) return NULL_POINT;
    return crossPoint(Line(s), Line(t));
}

ld distanceBetweenLineAndPoint(const Line &l, const Point &p){
    return fabs(cross(l.b-l.a, p-l.a)) / abs(l.b-l.a);
}

ld distanceBetweenSegmentAndPoint(const Segment &l, const Point &p){
    if(dot(l.b-l.a, p-l.a) < EPS) {
        return abs(p-l.a);
    }
    if(dot(l.a-l.b, p-l.b) < EPS){
        return abs(p-l.b);
    }
    return fabs(cross(l.b-l.a, p-l.a)) / abs(l.b-l.a);
}

ld distanceBetweenSegments(const Segment &s, const Segment &t){
    if(isIntersect(s, t)) return (ld)0;
    return std::min({
        distanceBetweenSegmentAndPoint(s, t.a),
        distanceBetweenSegmentAndPoint(s, t.b),
        distanceBetweenSegmentAndPoint(t, s.a),
        distanceBetweenSegmentAndPoint(t, s.b),
    });
}

// v : 多角形の頂点、反時計回り
struct Polygon{
    vector<Point> v;
    Polygon() = default;
    Polygon(vector<Point> p) : v(p) {}
    
    void add(Point a) {
        v.eb(a);
    }

    void rem(){
        assert(!v.empty());
        v.pop_back();
    }

    ld getArea(){
        ld res = 0;
        int n = v.size();
        assert(n >= 1);
        rep(i,n){
            res += cross(v[i], v[(i+1)%n]);
        }
        return res * ld(0.5);
    }

    /**
     * TODO : 一直線上にある場合の判定について少し考える
     */
    bool isConvex(){
        int n = v.size();
        rep(i,n){
            Point pre = v[i];
            Point now = v[(i+1)%n];
            Point nxt = v[(i+2)%n];
            if(ccw(pre, now, nxt) == -1) return false;
        }
        return true;
    }
    
    /**
     * @return
     * 2 : 含まれる
     * 1 : 辺上
     * 0 : 含まれない
     */
    int contain(Point &p){
        bool in = false;
        int n = v.size();
        rep(i,n){
            Point a = v[i]-p, b = v[(i+1)%n]-p;
            if(imag(a) > imag(b)) {
                std::swap(a, b);
            }
            if(imag(a) <= EPS && EPS < imag(b) && cross(a, b) < -EPS){
                in = !in;
            }
            if(equal(cross(a, b), 0) && dot(a, b) <= EPS){
                return 1;
            }
        }
        return (in ? 2 : 0);
    }
};

// 凸包、反時計回り
// @param border
// true -> border is included
// false -> border is not included
Polygon ConvexHull(Polygon p, bool border){
    ld feps = EPS;
    if(border) feps = -EPS;

    int n = p.v.size();

    if (n <= 1) return p;

    sort(all(p.v), [](const Point &a, const Point &b){
        if(equal(real(a), real(b))) return imag(a) < imag(b);
        return real(a) < real(b);
    });

    vector<Point> ret(2 * n);
    int k = 0;

    // 一直線上の3点を含める -> (< -EPS)
    // 含め無い -> (< EPS)
    rep(i,n){
        while(k >= 2){
            Point pre = ret[k-2], now = ret[k-1], nxt = p.v[i];
            if(cross(now-pre, nxt-now) < feps) k--;
            else break;
        }
        ret[k++] = p.v[i];
    }

    int t = k+1;

    rrep(i,n-1){
        while(k >= t){
            Point pre = ret[k-2], now = ret[k-1], nxt = p.v[i];
            if(cross(now-pre, nxt-now) < feps) k--;
            else break;
        }
        ret[k++] = p.v[i];
    }

    ret.resize(k-1);

    if (!border && ret.size() == 2 && equal(ret[0], ret[1])) ret.pop_back();
    
    return Polygon(ret);
}

/**
 * @return
 * 4 : 2 つの円が離れている
 * 3 : 外接している
 * 2 : 2 点で交わる
 * 1 : 内接している
 * 0 : 内包している
 */
int isIntersect(const Circle &c1, const Circle &c2){
    ld d = fabs(c1.p-c2.p);

    if(d > c1.r + c2.r + EPS) return 4;

    if(equal(d, c1.r + c2.r)) return 3;

    if(equal(d, fabs(c1.r-c2.r))) return 1;

    if(d < fabs(c1.r-c2.r)-EPS) return 0;

    return 2;
}

Circle inCircle(const Point &a, const Point &b, const Point &c){
    ld A = abs(a-c), B = abs(a-c), C = abs(a-b);

    Point p = a * A + b * B + c * C;
    p /= (A+B+C);

    ld r = distanceBetweenLineAndPoint(Line(a,b), p);

    return Circle(p, r);
}

vector<Point> argSort(vector<Point> p) {
    sort(all(p), [](const Point &a, const Point &b){
        auto type = [&](Point x) {
            if (equal(x, ORIGIN)) return 0;
            if (x.imag() < -EPS || (equal(x.imag(), 0) && -EPS < x.real())) return -1;
            return 1;
        };
        int ta = type(a), tb = type(b);
        if (ta != tb) return (ta < tb);
        return cross(a, b) > 0;
    });
    return p;
}

Point input() {
    ld x, y; cin >> x >> y;
    return Point(x, y);
}

pair<Point, Point> closest_pair(vector<Point> &x, int left = 0, int right = -1){
    if (right == -1) {
        right = x.size();
        sort(all(x), [](const Point &a, const Point &b){
            if (equal(a.real(), b.real())) return a.imag() < b.imag();
            return a.real() < b.real();
        });
    }
    if (right-left <= 1) {
        return make_pair(Point(INF, INF), Point(-INF, -INF));
    }

    int mid = (left+right)/2;
    auto p = x[mid];
    pair<Point, Point> res;
    {
        auto l = closest_pair(x, left, mid);
        auto r = closest_pair(x, mid, right);
        if (abs(l.first-l.second) < abs(r.first-r.second)) res = l;
        else res = r;
    }

    auto within = [&](ld a, ld b) {
        return fabs(a-b) < abs(res.first-res.second)-EPS;
    };

    auto chminP = [&](Point a, Point b) {
        if (abs(a-b) < abs(res.first-res.second)) res = make_pair(a, b);
    };

    vector<Point> y, z;
    {
        int l = left, r = mid;
        while(l < mid && r < right) {
            if (x[l].imag() < x[r].imag()) {
                y.eb(x[l++]);
            } else {
                y.eb(x[r++]);
            }
        }
        while (l < mid) y.eb(x[l++]);
        while (r < right) y.eb(x[r++]);

        rep2(i,left,right) {
            x[i] = y[i-left];
            if (within(x[i].real(), p.real())){
                z.eb(x[i]);
            }
        }
    }

    rep(i,z.size()) {
        rep2(j,i+1,z.size()) {
            if (within(z[i].imag(), z[j].imag())) chminP(z[i], z[j]);
            else break;
        }
        rrep2(j,i,0) {
            if (within(z[i].imag(), z[j].imag())) chminP(z[i], z[j]);
            else break;
        }
    }
    return res;
}

pair<Point, Point> furthest_pair(vector<Point> x) {

    vector<Point> p = ConvexHull(Polygon(x), false).v;

    if(p.size() == 1) return make_pair(p[0], p[0]);
    if(p.size() == 2) return make_pair(p[0], p[1]);

    auto res = make_pair(p[0], p[1]);

    auto chmaxP = [&](Point a, Point b) {
        if (abs(a-b) > abs(res.first-res.second)) res = make_pair(a, b);
    };

    int now = 0;
    rep(i,p.size()) {
        while(true){
            chmaxP(p[i], p[now]);
            if(cross(p[(i+1)%p.size()]-p[i], p[(now+1)%p.size()]-p[now]) < -EPS) break;
            now++;
            now %= SZ(p);
        }
    }
    return res;
}

Circle outerCenter(Point a, Point b, Point c){
    ld A = norm(b-c), B = norm(c-a), C = norm(a-b);
    ld S = cross(c-a, b-a);
    Point cent = (A*(B+C-A)*a + B*(C+A-B)*b + C*(A+B-C)*c) / (ld(4)*S*S);
    return Circle(cent, abs(cent-a));
}

// 最小包含円、期待値 O(N)
Circle min_ball(vector<Point> p){
    random_device rnd;
    mt19937 mt(rnd());
    shuffle(all(p), mt);

    ll N = SZ(p);

    Circle c;

    auto upd = [&](int i, int j){
        c = Circle((p[i]+p[j])/ld(2), abs(p[i]-p[j])/ld(2));
    };

    auto upd2 = [&](int i, int j, int k){
        c = outerCenter(p[i], p[j], p[k]);
    };

    if(N == 1) return Circle(p[0], 0);
    upd(0,1);

    rep2(i,2,N){
        if(c.contain(p[i])) continue;
        upd(0,i);
        rep(j,i){
            if(c.contain(p[j])) continue;
            upd(j,i);
            rep(k,j){
                if(c.contain(p[k])) continue;
                upd2(i,j,k);
            }
        }
    }
    return c;
}

// TODO: Doraunay to Voronoi の実装
// adopted from https://github.com/yosupo06/library-checker-problems/pull/1207/files#diff-81b1abd2ec4355ff9c6811fd0087ac7438a261f67db65f1bcba32650b3cc4a4c
struct Delaunay {

    struct Edge {
        int to;
        int ccw;
        int cw;
        int rev;
        bool enabled = false;
    };

    bool isDinOABC(int i, int j, int k, int l){
        auto a = pos[i], b = pos[j], c = pos[k], d = pos[l];
        a = a - d;
        b = b - d;
        c = c - d;
        auto val = norm(a)*cross(b,c) - norm(b)*cross(a,c) + norm(c)*cross(a,b);
        if(val > -EPS) return 1;
        else return 0;
    }

    P newEdge(int u, int v){
        int now = SZ(edges);
        edges.eb(Edge{v, now, now, now+1, true});
        edges.eb(Edge{u, now+1, now+1, now, true});
        return { now, now+1 };
    }

    void eraseSingleEdge(int e){
        int eccw = edges[e].ccw;
        int ecw = edges[e].cw;
        edges[eccw].cw = ecw;
        edges[ecw].ccw = eccw;
        edges[e].enabled = false;
    }

    void eraseEdgeBidirectional(int e){
        int ex = edges[e].rev;
        eraseSingleEdge(e);
        eraseSingleEdge(ex);
    }

    void insertCcwAfter(int e, int x){
        int xccw = edges[x].ccw;
        edges[e].ccw = xccw;
        edges[xccw].cw = e;
        edges[e].cw = x;
        edges[x].ccw = e;
    }

    void insertCwAfter(int e, int x){
        int xcw = edges[x].cw;
        edges[e].cw = xcw;
        edges[xcw].ccw = e;
        edges[e].ccw = x;
        edges[x].cw = e;
    }

    // move from ab to ac ... is this ccw?
    int isCcw(int a, int b, int c) const {
        auto ab = pos[b] - pos[a];
        auto ac = pos[c] - pos[a];
        auto cp = cross(ab, ac);
        if(EPS < cp) return 1;
        if(cp < -EPS) return -1;
        return 0;
    }

    std::pair<int, int> goNext(int , int ea){
        int ap = edges[ea].to;
        int eap = edges[edges[ea].rev].ccw;
        return { ap, eap };
    }

    std::pair<int, int> goPrev(int , int ea){
        int ap = edges[edges[ea].cw].to;
        int eap = edges[edges[ea].cw].rev;
        return { ap, eap };
    }

    std::tuple<int, int, int, int> goBottom(int l, int el, int r, int er){
        while(true){
            auto [lp, elp] = goPrev(l, el);
            if(isCcw(r, l, lp) > -EPS){
                std::tie(l, el) = { lp, elp };
                continue;
            }
            auto [rp, erp] = goNext(r, er);
            if(isCcw(l, r, rp) < EPS){
                std::tie(r, er) = { rp, erp };
                continue;
            }
            break;
        }
        return { l, el, r, er };
    }

    P getMaximum(int a, int ea, bool toMin){
        P ans = { a, ea };
        ll p = a, ep = ea;
        do {
            tie(p, ep) = goNext(p, ep);
            if(toMin) ans = min(ans, make_pair(p, ep));
            else ans = max(ans, make_pair(p, ep));
        } while(ep != ea);
        return ans;
    }

    P dfs(int l, int el, int r, int er){
        tie(l, el) = getMaximum(l, el, false);
        tie(r, er) = getMaximum(r, er, true);
        auto [lb, elb, rb, erb] = goBottom(l, el, r, er);
        auto [ru, eru, lu, elu] = goBottom(r, er, l, el);
        erb = edges[erb].cw;
        eru = edges[eru].cw;

        auto [lr, rl] = newEdge(lb, rb);
        insertCwAfter(lr, elb);
        insertCcwAfter(rl, erb);
        if(lb == lu) elu = lr;
        if(rb == ru) eru = rl;

        int lp = lb, elp = elb;
        int rp = rb, erp = erb;

        while(lp != lu || rp != ru){
            int a2 = edges[elp].to;
            int b2 = edges[erp].to;
            int nxelp = edges[elp].ccw;
            int nxerp = edges[erp].cw;

            if(elp != elu && nxelp != lr){
                int a1 = edges[nxelp].to;
                if(isDinOABC(lp, rp, a2, a1)){
                    eraseEdgeBidirectional(elp);
                    elp = nxelp;
                    continue;
                }
            }

            if(erp != eru && nxerp != rl){
                int b1 = edges[nxerp].to;
                if(isDinOABC(b2, lp, rp, b1)){
                    eraseEdgeBidirectional(erp);
                    erp = nxerp;
                    continue;
                }
            }

            bool chooseA = erp == eru;
            if(elp != elu && erp != eru){
				if(isCcw(lp, rp, b2) < -EPS) chooseA = true;
				else if(isCcw(a2, lp, rp) < -EPS) chooseA = false;
				else chooseA = isDinOABC(lp, rp, b2, a2);
            }

            if(chooseA){
                nxelp = edges[edges[elp].rev].ccw;
                auto [hab, hba] = newEdge(a2, rp);
                insertCwAfter(hab, nxelp);
                insertCcwAfter(hba, erp);
                elp = nxelp; lp = a2;
            }
            else {
                nxerp = edges[edges[erp].rev].cw;
                auto [hba, hab] = newEdge(b2, lp);
                insertCcwAfter(hba, nxerp);
                insertCwAfter(hab, elp);
                erp = nxerp; rp = b2;
            }
        }

        return { lb, lr };
    }

    P solveRange(int left, int right){
        if(right - left == 1) return {-1, -1};
        if(right - left == 2){
            int u = left, v = left+1;
            auto [uv, vu] = newEdge(u, v);
            return { u, uv };
        }
        if(right - left == 3){
            int u = left, v = left+1, w = left+2;
            auto [uv, vu] = newEdge(u, v);
            auto [vw, wv] = newEdge(v, w);
            int ccw = isCcw(u, v, w);
            if(ccw == 0){
                insertCcwAfter(vu, vw);
                return { u, uv };
            }
            else{
                auto [uw, wu] = newEdge(u, w);
                insertCwAfter(uv, uw);
                insertCwAfter(vw, vu);
                insertCwAfter(wu, wv);
                if(ccw > 0) return { u, uv };
                else return { v, vu };
            }
        }
        int mid = (left + right) / 2;

        auto [l, el] = solveRange(left, mid);
        auto [r, er] = solveRange(mid, right);

        return dfs(l, el, r, er);
    }

    vector<Point> pos;
    vector<Edge> edges;
    vl mappings;

    void solve(vector<Point> x){
        int n = x.size();
        if(n <= 1) return;

        vl pi(n);
        rep(i,n) pi[i] = i;
        sort(all(pi),[&](int l, int r){
                if(equal(x[l].real(), x[r].real())) return x[l].imag() < x[r].imag();
                return x[l].real() < x[r].real();
        });
        int idx = 0;
        mappings.resize(n);
        rep(i,n){
            int v = pi[i];
            if(i == 0 || !equal(pos.back(), x[v])){
                pi[idx++] = v;
                pos.eb(x[v]);
                mappings[v] = v;
            } else {
                mappings[v] = pi[idx-1];
            }
        }
        solveRange(0, idx);
        for(auto &e: edges) e.to = pi[e.to];
    }

    Delaunay(vector<Point> x) {
        solve(x);
    }

    vp getEdges() const {
        vp res;
        rep(e, SZ(edges)){
            if(edges[e].enabled){
                int re = edges[e].rev;
                if(e < re) continue;
                res.eb(edges[e].to, edges[re].to);
            }
        }
        rep(i, SZ(mappings)){
            if(mappings[i] != i) res.eb(i, mappings[i]);
        }
        return res;
    }

};


//union_find_tree
struct union_find{
    ll N;
    vl par, siz;
    union_find(int n) : N(n){
        par.resize(N);
        siz.resize(N, 1);
        rep(i,N) par[i] = i;
    }
    ll root(ll X){
        if(par[X] == X) return X;
        return par[X] = root(par[X]);
    }
    bool same(ll X, ll Y){
        return root(X) == root(Y);
    }
    void unite(ll X, ll Y){
        X = root(X);
        Y = root(Y);
        if(X == Y) return;
        if(siz[Y] < siz[X]) std::swap(X, Y);
        par[X] = Y;
        siz[Y] += siz[X];
        siz[X] = 0;
    }
    ll size(ll X){
        return siz[root(X)];
    }
};



int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    cout << std::fixed << std::setprecision(15);
    ll N; cin >> N;
    vector<Point> x(N);
    rep(i,N) x[i] = input();
    Delaunay d(x);
    auto p = d.getEdges();
    sort(all(p), [&](P l, P r){
        auto dist = [&](P i){
            return norm(x[i.first]-x[i.second]);
        };
        return dist(l) < dist(r);
    });
    union_find tree(N);
    for(auto el: p){
        if(!tree.same(el.first, el.second)){
            tree.unite(el.first, el.second);
            cout << el.first << " " << el.second << "\n";
        }
    }
}