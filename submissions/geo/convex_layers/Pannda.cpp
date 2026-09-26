#include <bits/stdc++.h>
using namespace std;

template <typename T, typename T2>
struct DecrementalUpperHull {
 private:
  using Point = pair<T, T>;

  static constexpr T2 cross(const Point &a, const Point &b) {
    return T2(a.first) * b.second - T2(a.second) * b.first;
  }

  static constexpr int ccw(const Point &a, const Point &b) {
    T2 x = cross(a, b);
    return (x > 0) - (x < 0);
  }

  static constexpr Point sub(const Point &a, const Point &b) {
    return {a.first - b.first, a.second - b.second};
  }

  static constexpr int ccw(Point const &a, Point const &b, Point const &c) {
    return ccw(sub(b, a), sub(c, a));
  }

  struct Link {
    Point p;
    Link *prev, *next;
    int id;
  };
  using LP = Link *;

  struct Node {
    LP chain, chain_back, tangent;
    int lch, rch;
  };

  size_t n, sz;
  vector<bool> alive;
  vector<Node> seg;
  vector<Link> links;

  pair<LP, LP> find_bridge(LP l, LP r) const {
    while (l->next or r->next) {
      if (not r->next or (l->next and ccw(sub(l->next->p, l->p),
                                          sub(r->next->p, r->p)) <= 0)) {
        if (ccw(l->p, l->next->p, r->p) <= 0) {
          l = l->next;
        } else {
          break;
        }
      } else {
        if (ccw(l->p, r->p, r->next->p) > 0) {
          r = r->next;
        } else {
          break;
        }
      }
    }
    return {l, r};
  }

  pair<LP, LP> find_bridge_rev(LP l, LP r) const {
    while (r->prev or l->prev) {
      if (not l->prev or (r->prev and ccw(sub(r->prev->p, r->p),
                                          sub(l->prev->p, l->p)) >= 0)) {
        if (ccw(r->p, r->prev->p, l->p) >= 0) {
          r = r->prev;
        } else {
          break;
        }
      } else {
        if (ccw(r->p, l->p, l->prev->p) < 0) {
          l = l->prev;
        } else {
          break;
        }
      }
    }
    return {r, l};
  }

  template <bool rev>
  void fix_chain(int u, LP l, LP r) {
    if (rev)
      tie(r, l) = find_bridge_rev(l, r);
    else
      tie(l, r) = find_bridge(l, r);

    Node &l_node = seg[seg[u].lch];
    Node &r_node = seg[seg[u].rch];

    seg[u].tangent = l;
    seg[u].chain = l_node.chain;
    seg[u].chain_back = r_node.chain_back;
    l_node.chain = l->next;
    r_node.chain_back = r->prev;

    if (l->next)
      l->next->prev = nullptr;
    else
      l_node.chain_back = nullptr;
    if (r->prev)
      r->prev->next = nullptr;
    else
      r_node.chain = nullptr;

    l->next = r;
    r->prev = l;
  }

  void rob(int u, int v) {
    seg[u].chain = seg[v].chain;
    seg[v].chain = nullptr;
    seg[u].chain_back = seg[v].chain_back;
    seg[v].chain_back = nullptr;
  }

  void erase(int u, int a, int b, int i) {
    if (i < a or i >= b or u == -1) return;
    int m = (a + b) / 2;
    int v = i < m ? seg[u].lch : seg[u].rch;
    if (!seg[u].tangent) {
      seg[v].chain = seg[u].chain;
      seg[v].chain_back = seg[u].chain_back;
      if (i < m)
        erase(v, a, m, i);
      else
        erase(v, m, b, i);
      rob(u, v);
      return;
    }

    auto l = seg[u].tangent, r = l->next;
    Node &l_node = seg[seg[u].lch];
    Node &r_node = seg[seg[u].rch];

    l->next = l_node.chain;
    if (l_node.chain)
      l_node.chain->prev = l;
    else
      l_node.chain_back = l;
    l_node.chain = seg[u].chain;

    r->prev = r_node.chain_back;
    if (r_node.chain_back)
      r_node.chain_back->next = r;
    else
      r_node.chain = r;
    r_node.chain_back = seg[u].chain_back;

    if (seg[v].chain == seg[v].chain_back and seg[v].chain->id == i) {
      seg[v].chain = seg[v].chain_back = nullptr;
      rob(u, i < m ? seg[u].rch : seg[u].lch);
      seg[u].tangent = nullptr;
    } else if (i < m) {
      if (l->id == i) l = l->next;
      erase(v, a, m, i);
      if (not l) l = l_node.chain_back;
      fix_chain<true>(u, l, r);
    } else {
      if (r->id == i) r = r->prev;
      erase(v, m, b, i);
      if (not r) r = r_node.chain;
      fix_chain<false>(u, l, r);
    }
  }

  size_t build(size_t &k, int l, int r) {
    if (r - l == 1) return l + n;
    int m = (l + r) / 2;
    size_t res = k++;
    seg[res].lch = build(k, l, m);
    seg[res].rch = build(k, m, r);
    fix_chain<false>(res, seg[seg[res].lch].chain, seg[seg[res].rch].chain);
    return res;
  }

 public:
  explicit DecrementalUpperHull(const vector<Point> &ps)
      : n(ps.size()), seg(2 * n), sz(n), alive(n) {
    assert(is_sorted(ps.begin(), ps.end()));
    links.reserve(n);
    for (int k = 0; k < n; ++k) {
      links.emplace_back(Link{ps[k], nullptr, nullptr, k});
    }
    for (int k = 0; k < n; k++) {
      seg[k + n] = {&links[k], &links[k], nullptr, -1, -1};
    }
    if (ps.size() == 1) seg[0] = seg[1];
    size_t u = 0;
    build(u, 0, n);
  }

  size_t size() const { return sz; }

  bool empty() const { return sz == 0; }

  bool erase(int k) {
    assert(0 <= k and k < n);
    if (alive[k]) return false;
    alive[k] = true;
    --sz;
    if (seg[0].chain == seg[0].chain_back) {
      seg[0].chain = seg[0].chain_back = nullptr;
    } else {
      erase(0, 0, n, k);
    }
    return true;
  }

  vector<int> get_hull() const {
    vector<int> ret;
    for (LP u = seg[0].chain; u; u = u->next) {
      ret.push_back(u->id);
    }
    return ret;
  }
};

template <typename T, typename T2>
vector<int> convex_layers(const vector<pair<T, T> >& ps) {
  int n = (int)ps.size();
  vector<int> ord(n);
  iota(ord.begin(), ord.end(), 0);
  sort(ord.begin(), ord.end(), [&](int a, int b) { return ps[a] < ps[b]; });
  vector<pair<T, T> > us(n);
  for (int i = 0; i < n; i++) {
    us[i] = ps[ord[i]];
  }
  DecrementalUpperHull<T, T2> upper_hull(us);
  vector<pair<T, T> > ds(n);
  for (int i = 0; i < n; i++) {
    ds[i] = {-us[n - i - 1].first, -us[n - i - 1].second};
  }
  DecrementalUpperHull<T, T2> lower_hull(ds);
  vector<int> convex_hull, res(n, -1);
  convex_hull.reserve(n);
  for (int layer = 1; not lower_hull.empty(); layer++) {
    for (auto& p : upper_hull.get_hull()) {
      res[ord[p]] = layer;
      convex_hull.emplace_back(p);
    }
    for (auto& p : lower_hull.get_hull()) {
      p = n - p - 1;
      if (res[ord[p]] == -1) {
        res[ord[p]] = layer;
        convex_hull.emplace_back(p);
      }
    }
    for (auto& p : convex_hull) {
      upper_hull.erase(p);
      lower_hull.erase(n - p - 1);
    }
    convex_hull.clear();
  }
  return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<pair<int, int>> ps(n);
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        ps[i] = {x, y};
    }

    vector<int> res = convex_layers<int, long long>(ps);
    for (int x : res) {
        cout << x << '\n';
    }
}
