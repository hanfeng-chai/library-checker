#include <bits/stdc++.h>
using namespace std;

// ---- prelude (subset of src/basic/) ----
#define ALL(v) (v).begin(), (v).end()
#define SZ(v) int((v).size())
#define FOR(i, begin, end) for(int i = (begin), i##_end_ = (end); i < i##_end_; i++)
#define REP(i, n) FOR(i, 0, n)
#define eb emplace_back
#define pb push_back

// ---- src/flow_matching/max_clique.cpp (verbatim) ----
template<int V>
struct max_clique {
	using B = bitset<V>;
	int n = 0;
	vector<B> g, buf;
	struct P {
		int idx, col, deg;
		P(int a, int b, int c) : idx(a), col(b), deg(c) {}
	};
	max_clique(int _n) : n(_n), g(_n), buf(_n){}
	void add_edge(int a, int b) {
		assert(a != b);
		g[a][b] = g[b][a] = 1;
	}
	vector<int> now, clique;
	void dfs(vector<P>& rem){
		if(SZ(clique) < SZ(now)) clique = now;
		sort(ALL(rem), [](P a, P b) { return a.deg > b.deg; });
		int max_c = 1;
		for(auto& p : rem){
			p.col = 0;
			while((g[p.idx] & buf[p.col]).any()) p.col++;
			max_c = max(max_c, p.idx + 1);
			buf[p.col][p.idx] = 1;
		}
		REP(i, max_c) buf[i].reset();
		sort(ALL(rem), [&](P a, P b) { return a.col < b.col; });
		for(;SZ(rem); rem.pop_back()){
			auto& p = rem.back();
			if(SZ(now) + p.col + 1 <= SZ(clique)) break;
			vector<P> nrem;
			B bs;
			for(auto& q : rem){
				if(g[p.idx][q.idx]){
					nrem.eb(q.idx, -1, 0);
					bs[q.idx] = 1;
				}
			}
			for(auto& q : nrem) q.deg = (bs & g[q.idx]).count();
			now.eb(p.idx);
			dfs(nrem);
			now.pop_back();
		}
	}
	vector<int> solve(){
		vector<P> remark;
		REP(i, n) remark.eb(i, -1, SZ(g[i]));
		dfs(remark);
		return clique;
	}
};

// ---- driver: MIS = max clique on complement ----
const int V = 40; // N_MAX

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	vector<bitset<V>> adj(n);
	REP(e, m) {
		int u, v;
		scanf("%d %d", &u, &v);
		adj[u][v] = adj[v][u] = 1;
	}
	max_clique<V> mc(n);
	REP(i, n) FOR(j, i + 1, n) if(!adj[i][j]) mc.add_edge(i, j); // complement
	auto S = mc.solve();
	printf("%d\n", SZ(S));
	REP(i, SZ(S)) printf("%d%c", S[i], " \n"[i + 1 == SZ(S)]);
	if(S.empty()) printf("\n");
	return 0;
}
