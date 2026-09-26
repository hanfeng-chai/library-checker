// simplex remake
#include <stdio.h>
#include <string.h>
#include <algorithm>
#define getchar getchar_unlocked
#define putchar putchar_unlocked
typedef __int128_t i128;
i128 rd()
{
    i128 k = 0, f = 1;
    char s = getchar();
    while (s < '0' || s > '9')
    {
        if (s == '-')
            f = 0;
        s = getchar();
    }
    while (s >= '0' && s <= '9')
    {
        k = (k << 1) + (k << 3) + (s ^ '0');
        s = getchar();
    }
    return f ? k : -k;
}
void wr(i128 x)
{
    if (x < 0)
        putchar('-'), x = -x;
    if (x > 9)
        wr(x / 10);
    putchar((x % 10) ^ '0');
}
using cap = i128;
const int N = 110;
const int M = 1510;
// 1-indexed
namespace MCMF
{

    const cap flow_INF = (i128)1145141919810114514ll * 1145141919810114514ll;
    const cap cost_offset = (i128)1145141919810ll;

    int n, m, s, t;

	struct edge
	{
		int from, to, nxt;
		cap flow, cost;
		bool origin;
	} e[M << 1];
	int cnt;
	int h[N];

	int add_edge(int u, int v, cap flow, cap cost, bool directed = true)
	{
		++m;
		e[++cnt].from = u, e[cnt].to = v, e[cnt].nxt = h[u], e[cnt].flow = flow, e[cnt].cost = cost, e[cnt].origin = 1;
		h[u] = cnt;
		e[++cnt].from = v, e[cnt].to = u, e[cnt].nxt = h[v], e[cnt].flow = directed ? 0 : flow, e[cnt].cost = -cost, e[cnt].origin = 0;
		h[v] = cnt;
		return cnt;
	}
	cap get_remain_flow(int no) { return e[no].flow; }
	
	int tme;
	int vis[N], fa[N], fe[N], circle[N], mark[N];
	cap pi[N];

	void dfs(int u, int fi)
	{
		fa[u] = e[fi].from, fe[u] = fi;
		mark[u] = 1;
		for (int i = h[u]; i; i = e[i].nxt)
		{
			int v = e[i].to;
			if (e[i].origin && !mark[v])
				dfs(v, i);
		}
	}

	cap phi(int u)
	{
		if (mark[u] == tme)
			return pi[u];
		mark[u] = tme, pi[u] = phi(fa[u]) + e[fe[u]].cost;
		return pi[u];
	}

	cap pushflow(int eg)
	{
		int rt = e[eg].from, lca = e[eg].to;
		++tme;
		int circle_cnt = 0;
		while (rt)
			mark[rt] = tme, rt = fa[rt];
		while (mark[lca] ^ tme)
			mark[lca] = tme, lca = fa[lca];
		cap minflow = e[eg].flow, p = 2, del_u = 0;
		for (int u = e[eg].from; u ^ lca; u = fa[u])
		{
			circle[++circle_cnt] = fe[u];
			if (e[fe[u]].flow < minflow)
				minflow = e[fe[u]].flow, del_u = u, p = 0;
		}
		for (int u = e[eg].to; u ^ lca; u = fa[u])
		{
			int ne = fe[u] ^ 1;
			circle[++circle_cnt] = ne;
			if (e[ne].flow < minflow)
				minflow = e[ne].flow, del_u = u, p = 1;
		}
		circle[++circle_cnt] = eg;

		cap cost = 0;
		for (int i = 1; i <= circle_cnt; ++i)
		{
			cost += e[circle[i]].cost * minflow;
			e[circle[i]].flow -= minflow, e[circle[i] ^ 1].flow += minflow;
		}
		if (p == 2)
			return cost;

		int u = e[eg].from, v = e[eg].to;
		if (p == 1)
			std::swap(u, v);
		int last_e = eg ^ p, last_u = v;
		while (last_u ^ del_u)
		{
			last_e ^= 1, --mark[u], std::swap(fe[u], last_e);
			int nu = fa[u];
			fa[u] = last_u, last_u = u, u = nu;
		}
		return cost;
	}
	void init_sz(int _n) { n = _n, m = 0, cnt = 1, tme = 1; }
	std::pair<cap, cap> solve(int _s, int _t)
	{
		s = _s, t = _t;
		add_edge(t, s, flow_INF, -cost_offset);
		dfs(t, 0), mark[t] = ++tme;
		fa[t] = 0;
		cap cost = 0, flow = 0;
		bool run = 1;
		while (run)
		{
			run = 0;
			for (int i = 2; i <= cnt; ++i)
				if (e[i].flow && e[i].cost + phi(e[i].from) - phi(e[i].to) < 0)
					cost += pushflow(i), run = 1;
		}
		flow = e[cnt].flow;
		return std::make_pair(flow, cost + flow * cost_offset);
	}
}
namespace bound_MCMF
{
    cap fl[N]; // > 0 : supply, < 0 demand
    cap supply_sum, base_cost, extra_cost;
    cap ori_edge_lo[M];
    int rev_edge_no[M];
    int n, S, T;
    void init(int _n) { n = _n, MCMF::init_sz(n + 2), S = n + 1, T = n + 2; }
    void standard_supply_or_demand(int no, cap val) { fl[no] += val; }
    void add_edge(int no, int u, int v, cap lo, cap hi, cap cost)
    {
        fl[u] -= lo, fl[v] += lo, ori_edge_lo[no] = lo;
        base_cost += lo * cost;
        rev_edge_no[no] = MCMF::add_edge(u, v, hi - lo, cost);
    }
    bool solve()
    {
        for (int i = 1; i <= n; ++i)
        {
            if (fl[i] > 0)
                MCMF::add_edge(S, i, fl[i], 0), supply_sum += fl[i];
            else if (fl[i] < 0)
                MCMF::add_edge(i, T, -fl[i], 0);
        }
        std::pair<cap, cap> ans = MCMF::solve(S, T);
        return extra_cost = ans.second, ans.first == supply_sum;
    }
    cap mincost() { return base_cost + extra_cost; }
    cap get_pi(int u) { return MCMF::cost_offset + MCMF::pi[u]; }
    cap real_flow(int no) { return ori_edge_lo[no] + MCMF::get_remain_flow(rev_edge_no[no]); }
}

int main()
{
    int n = rd(), m = rd();
    bound_MCMF::init(n);
    for (int i = 1; i <= n; ++i)
        bound_MCMF::standard_supply_or_demand(i, rd());
    for (int i = 1; i <= m; ++i)
    {
        int u = rd() + 1, v = rd() + 1;
        cap lo = rd(), hi = rd(), cost = rd();
        bound_MCMF::add_edge(i, u, v, lo, hi, cost);
    }
    if (!bound_MCMF::solve())
        puts("infeasible");
    else
    {
        wr(bound_MCMF::mincost()), putchar('\n');
        for (int i = 1; i <= n; ++i)
            wr(bound_MCMF::get_pi(i)), putchar('\n');
        for (int i = 1; i <= m; ++i)
            wr(bound_MCMF::real_flow(i)), putchar('\n');
    }
}