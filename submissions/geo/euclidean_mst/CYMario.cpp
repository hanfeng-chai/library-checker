// solution from NachiaVivias
#include <utility>
#include <vector>
#include <tuple>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#define getchar getchar_unlocked
#define putchar putchar_unlocked
typedef long long i64;
i64 rd()
{
    i64 k = 0, f = 1;
    char c = getchar();
    while (c < '0' || c > '9')
    {
        if (c == '-')
            f = 0;
        c = getchar();
    }
    while (c >= '0' && c <= '9')
    {
        k = (k << 1) + (k << 3) + (c ^ 48);
        c = getchar();
    }
    return f ? k : -k;
}
void wr(int x)
{
    if (x < 0)
        putchar('-'), x = -x;
    if (x > 9)
        wr(x / 10);
    putchar((x % 10) ^ '0');
}

template <class Int = i64, class Int2 = i64>
struct VecI2
{
    Int x, y;
    VecI2() : x(0), y(0) {}
    VecI2(std::pair<Int, Int> _p) : x(std::move(_p.first)), y(std::move(_p.second)) {}
    VecI2(Int _x, Int _y) : x(std::move(_x)), y(std::move(_y)) {}
    VecI2 &operator+=(VecI2 r) { return x += r.x, y += r.y, (*this); }
    VecI2 &operator-=(VecI2 r) { x -= r.x, y -= r.y, (*this); }
    VecI2 &operator*=(Int r) { x *= r, y *= r, (*this); }
    VecI2 operator+(VecI2 r) const { return VecI2(x + r.x, y + r.y); }
    VecI2 operator-(VecI2 r) const { return VecI2(x - r.x, y - r.y); }
    VecI2 operator*(Int r) const { return VecI2(x * r, y * r); }
    VecI2 operator-() const { return VecI2(-x, -y); }
    Int2 operator*(VecI2 r) const { return Int2(x) * Int2(r.x) + Int2(y) * Int2(r.y); }
    Int2 operator^(VecI2 r) const { return Int2(x) * Int2(r.y) - Int2(y) * Int2(r.x); }
    bool operator<(VecI2 r) const { return x < r.x || (!(r.x < x) && y < r.y); }
    Int2 norm() const { return Int2(x) * Int2(x) + Int2(y) * Int2(y); }
    static bool compareYX(VecI2 a, VecI2 b) { return a.y < b.y || (!(b.y < a.y) && a.x < b.x); }
    static bool compareXY(VecI2 a, VecI2 b) { return a.x < b.x || (!(b.x < a.x) && a.y < b.y); }
    bool operator==(VecI2 r) const { return x == r.x && y == r.y; }
    bool operator!=(VecI2 r) const { return x != r.x || y != r.y; }
};

template <class Elem>
class CsrArray
{
public:
    struct ListRange
    {
        using iterator = typename std::vector<Elem>::iterator;
        iterator begi, endi;
        iterator begin() const { return begi; }
        iterator end() const { return endi; }
        int size() const { return (int)std::distance(begi, endi); }
        Elem &operator[](int i) const { return begi[i]; }
    };
    struct ConstListRange
    {
        using iterator = typename std::vector<Elem>::const_iterator;
        iterator begi, endi;
        iterator begin() const { return begi; }
        iterator end() const { return endi; }
        int size() const { return (int)std::distance(begi, endi); }
        const Elem &operator[](int i) const { return begi[i]; }
    };

private:
    int m_n;
    std::vector<Elem> m_list;
    std::vector<int> m_pos;

public:
    CsrArray() : m_n(0), m_list(), m_pos() {}
    static CsrArray Construct(int n, std::vector<std::pair<int, Elem>> items)
    {
        CsrArray res;
        res.m_n = n;
        std::vector<int> buf(n + 1, 0);
        for (auto &[u, v] : items)
            ++buf[u];
        for (int i = 1; i <= n; i++)
            buf[i] += buf[i - 1];
        res.m_list.resize(buf[n]);
        for (int i = (int)items.size() - 1; i >= 0; i--)
            res.m_list[--buf[items[i].first]] = std::move(items[i].second);
        res.m_pos = std::move(buf);
        return res;
    }
    static CsrArray FromRaw(std::vector<Elem> list, std::vector<int> pos)
    {
        CsrArray res;
        res.m_n = pos.size() - 1, res.m_list = std::move(list), res.m_pos = std::move(pos);
        return res;
    }
    ListRange operator[](int u) { return ListRange{m_list.begin() + m_pos[u], m_list.begin() + m_pos[u + 1]}; }
    ConstListRange operator[](int u) const { return ConstListRange{m_list.begin() + m_pos[u], m_list.begin() + m_pos[u + 1]}; }
    int size() const { return m_n; }
    int fullSize() const { return (int)m_list.size(); }
};

// Int3 must be able to handle the value range :
//  |x| <= | (any input - any input) ** 4 * 12 |

template <class Int = i64, class Int2 = i64, class Int3 = Int2>
class DelaunayTriangulation
{
public:
    using GPos2 = VecI2<Int, Int2>;

    struct Edge
    {
        int to, ccw, cw, rev;
        bool enabled = false;
    };

private:
    static int isDinOABC(GPos2 a, GPos2 b, GPos2 c, GPos2 d)
    {
        a = a - d, b = b - d, c = c - d;
        auto val = Int3(b ^ c) * Int3(a.norm()) + Int3(c ^ a) * Int3(b.norm()) + Int3(a ^ b) * Int3(c.norm());
        return val > Int3(0) ? 1 : 0;
    }

    int getOpenAddress()
    {
        if (openAddress.empty())
        {
            edges.push_back({});
            return (int)edges.size() - 1;
        }
        int res = openAddress.back();
        openAddress.pop_back();
        return res;
    }

    std::pair<int, int> newEdge(int u, int v)
    {
        int euv = getOpenAddress(), evu = getOpenAddress();
        edges[euv].ccw = edges[euv].cw = euv, edges[evu].ccw = edges[evu].cw = evu;
        edges[euv].to = v, edges[evu].to = u;
        edges[euv].rev = evu, edges[evu].rev = euv;
        edges[euv].enabled = true, edges[evu].enabled = true;
        return {euv, evu};
    }

    void eraseSingleEdge(int e)
    {
        int eccw = edges[e].ccw, ecw = edges[e].cw;
        edges[eccw].cw = ecw, edges[ecw].ccw = eccw;
        edges[e].enabled = false;
    }

    void eraseEdgeBidirectional(int e)
    {
        int ex = edges[e].rev;
        eraseSingleEdge(e), eraseSingleEdge(ex);
        openAddress.push_back(e), openAddress.push_back(ex);
    }

    void insertCcwAfter(int e, int x)
    {
        int xccw = edges[x].ccw;
        edges[e].ccw = xccw, edges[xccw].cw = e;
        edges[e].cw = x, edges[x].ccw = e;
    }

    void insertCwAfter(int e, int x)
    {
        int xcw = edges[x].cw;
        edges[e].cw = xcw, edges[xcw].ccw = e;
        edges[e].ccw = x, edges[x].cw = e;
    }

    // move from ab to ac ... is this ccw?
    int isCcw(int a, int b, int c) const
    {
        auto ab = pos[b] - pos[a], ac = pos[c] - pos[a], cp = ab ^ ac;
        return (cp > 0) ? 1 : (cp < 0) ? -1 : 0;
    }

    std::pair<int, int> goNext(int, int ea)
    {
        int ap = edges[ea].to, eap = edges[edges[ea].rev].ccw;
        return {ap, eap};
    }

    std::pair<int, int> goPrev(int, int ea)
    {
        int ap = edges[edges[ea].cw].to, eap = edges[edges[ea].cw].rev;
        return {ap, eap};
    }

    std::tuple<int, int, int, int> goBottom(int a, int ea, int b, int eb)
    {
        while (true)
        {
            auto [ap, eap] = goPrev(a, ea);
            if (isCcw(b, a, ap) > 0)
            {
                std::tie(a, ea) = {ap, eap};
                continue;
            }
            auto [bp, ebp] = goNext(b, eb);
            if (isCcw(a, b, bp) < 0)
            {
                std::tie(b, eb) = {bp, ebp};
                continue;
            }
            break;
        }
        return {a, ea, b, eb};
    }

    std::pair<int, int> getMaximum(int a, int ea, bool toMin)
    {
        std::pair<int, int> ans = {a, ea};
        int p = a, ep = ea;
        do
        {
            std::tie(p, ep) = goNext(p, ep);
            if (toMin)
                ans = std::min(ans, std::make_pair(p, ep));
            else
                ans = std::max(ans, std::make_pair(p, ep));
        } while (ep != ea);
        return ans;
    }

    bool isDinOABC(int a, int b, int c, int d) { return isDinOABC(pos[a], pos[b], pos[c], pos[d]); }

    std::pair<int, int> dfs(int a, int ea, int b, int eb)
    {
        std::tie(a, ea) = getMaximum(a, ea, false);
        std::tie(b, eb) = getMaximum(b, eb, true);
        auto [al, eal, bl, ebl] = goBottom(a, ea, b, eb);
        auto [bu, ebu, au, eau] = goBottom(b, eb, a, ea);
        ebl = edges[ebl].cw, ebu = edges[ebu].cw;

        auto [abl, bal] = newEdge(al, bl);
        insertCwAfter(abl, eal), insertCcwAfter(bal, ebl);
        if (al == au)
            eau = abl;
        if (bl == bu)
            ebu = bal;

        int ap = al, eap = eal, bp = bl, ebp = ebl;
        while (ap != au || bp != bu)
        {
            int a2 = edges[eap].to, b2 = edges[ebp].to;
            int nxeap = edges[eap].ccw, nxebp = edges[ebp].cw;

            if (eap != eau && nxeap != abl)
            {
                int a1 = edges[nxeap].to;
                if (isDinOABC(ap, bp, a2, a1))
                {
                    eraseEdgeBidirectional(eap);
                    eap = nxeap;
                    continue;
                }
            }

            if (ebp != ebu && nxebp != bal)
            {
                int b1 = edges[nxebp].to;
                if (isDinOABC(b2, ap, bp, b1))
                {
                    eraseEdgeBidirectional(ebp);
                    ebp = nxebp;
                    continue;
                }
            }

            bool chooseA = ebp == ebu;
            if (eap != eau && ebp != ebu)
            {
                if (isCcw(ap, bp, b2) < 0)
                    chooseA = true;
                else if (isCcw(a2, ap, bp) < 0)
                    chooseA = false;
                else
                    chooseA = isDinOABC(ap, bp, b2, a2);
            }

            if (chooseA)
            {
                nxeap = edges[edges[eap].rev].ccw;
                auto [hab, hba] = newEdge(a2, bp);
                insertCwAfter(hab, nxeap), insertCcwAfter(hba, ebp);
                eap = nxeap, ap = a2;
            }
            else
            {
                nxebp = edges[edges[ebp].rev].cw;
                auto [hba, hab] = newEdge(b2, ap);
                insertCcwAfter(hba, nxebp), insertCwAfter(hab, eap);
                ebp = nxebp, bp = b2;
            }
        }
        return {al, abl};
    }

    std::pair<int, int> solveRange(int l, int r)
    {
        if (r - l == 2)
        {
            int u = l, v = l + 1;
            auto [uv, vu] = newEdge(u, v);
            return {u, uv};
        }
        if (r - l == 3)
        {
            int u = l, v = l + 1, w = l + 2;
            auto [uv, vu] = newEdge(u, v);
            auto [vw, wv] = newEdge(v, w);
            int ccw = isCcw(u, v, w);
            if (ccw == 0)
                insertCcwAfter(vu, vw);
            if (ccw > 0)
            {
                auto [uw, wu] = newEdge(u, w);
                insertCwAfter(uv, uw);
                insertCwAfter(vw, vu);
                insertCwAfter(wu, wv);
                return {u, uv};
            }
            if (ccw < 0)
            {
                auto [uw, wu] = newEdge(u, w);
                insertCcwAfter(uv, uw);
                insertCcwAfter(vw, vu);
                insertCcwAfter(wu, wv);
                return {v, vu};
            }
            return {u, uv};
        }
        int m = (l + r) >> 1;

        auto [a, ea] = solveRange(l, m);
        auto [b, eb] = solveRange(m, r);

        return dfs(a, ea, b, eb);
    }

    void solve()
    {
        int sz = (int)pos.size();
        if (sz <= 1)
            return;

        std::vector<int> pi(pos.size());
        for (int i = 0; i < (int)pi.size(); i++)
            pi[i] = i;
        std::stable_sort(
            pi.begin(), pi.end(),
            [&](int l, int r)
            {
                return pos[l].x != pos[r].x ? pos[l].x < pos[r].x : pos[l].y < pos[r].y;
            });
        auto posbuf = pos;
        int posptr = 0;
        mappings.assign(sz, 0);
        for (int i = 0; i < sz; i++)
        {
            int v = pi[i];
            if (i == 0 || !(posbuf[pi[posptr - 1]] == posbuf[v]))
                pi[posptr] = v, pos[posptr++] = posbuf[v], mappings[v] = v;
            else
                mappings[v] = pi[posptr - 1];
        }

        if (posptr >= 2)
            outerOneEdge = solveRange(0, posptr).second;
        std::swap(pos, posbuf);
        for (auto &e : edges)
            e.to = pi[e.to];
    }

    std::vector<int> openAddress;
    std::vector<GPos2> pos;
    std::vector<Edge> edges;
    std::vector<int> mappings;
    int outerOneEdge = -1;

public:
    DelaunayTriangulation()
        : pos() { solve(); }

    DelaunayTriangulation(std::vector<GPos2> x_points)
        : pos(std::move(x_points)) { solve(); }

    std::vector<std::pair<int, int>> getEdges() const
    {
        std::vector<std::pair<int, int>> res;
        for (int e = 0; e < (int)edges.size(); e++)
            if (edges[e].enabled)
            {
                int re = edges[e].rev;
                if (e < re)
                    continue;
                res.push_back({edges[e].to, edges[re].to});
            }
        for (int v = 0; v < int(mappings.size()); v++)
        {
            if (mappings[v] != v)
                res.push_back({v, mappings[v]});
        }
        return res;
    }
};

struct DsuFast
{
private:
    std::vector<int> w;

public:
    DsuFast(int n = 0) : w(n, -1) {}
    int leader(int u)
    {
        if (w[u] < 0)
            return u;
        return w[u] = leader(w[u]);
    }
    int operator[](int u) { return leader(u); }
    int merge(int u, int v)
    {
        u = leader(u);
        v = leader(v);
        if (u == v)
            return u;
        if (-w[u] < -w[v])
            std::swap(u, v);
        w[u] += w[v];
        w[v] = u;
        return u;
    }
    int size(int u) { return -w[leader(u)]; }
    bool same(int u, int v) { return leader(u) == leader(v); }
};

int main()
{
    using namespace std;
    using Point = VecI2<i64, i64>;

    int N = rd();
    vector<Point> A(N);
    for (int i = 0; i < N; i++)
        A[i].x = rd(), A[i].y = rd();

    auto weight = [&](pair<int, int> p)
    { return (A[p.first] - A[p.second]).norm(); };

    // if coord value bounding is like 0 <= x, y <= 10^9
    //    DelaunayTriangulation<i64, i64, __int128_t>
    auto tri = DelaunayTriangulation(A).getEdges();
    stable_sort(tri.begin(), tri.end(),
                [&](pair<int, int> l, pair<int, int> r) -> bool
                { return weight(l) < weight(r); });

    auto dsu = DsuFast(N);
    double mst_length = 0.0;
    // vector<pair<int, int>> ans;
    for (auto a : tri)
    {
        if (dsu.same(a.first, a.second))
            continue;
        dsu.merge(a.first, a.second);
        wr(a.first), putchar(' '), wr(a.second), putchar('\n');
        mst_length += sqrt(weight(a));
    }
    fprintf(stderr, "mst length = %.10f", mst_length);
    return 0;
}