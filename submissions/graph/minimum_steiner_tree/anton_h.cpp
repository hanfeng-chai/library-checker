#include <bits/stdc++.h>                            
using namespace std;
using ll = long long;
const ll oo = numeric_limits<ll>::max()/4;
using vl = vector<ll>;
#define all(c) begin(c),end(c)
#define FOR(i,a,b) for (ll i=(a); i<(b); i++)
#define FORD(i,a,b) for (ll i=ll(b)-1; i>=(a); i--)
// Graphs                    // ============== random numbers =================
using vvl = vector<vl>;      // mt19937 rng($semirandom)
#define pb push_back         // ============== primes =========================
#define sz(c) ll((c).size()) // seq $from $to | factor | grep -v ' .* '
// pll                       // 2^30: +3, +7, +9, +385, -407
using pll = pair<ll,ll>;     // 2^60: +33, +91, +933, -753
using vpll = vector<pll>;  // =============== stress test script ==============
#define xx first           // for((i=1;;++i)); do
#define yy second          //   echo $i
// double                  //   ./gen > in
using dd = double;         //   diff -w <(./A < in) <(./brute < in) || break
const dd eps = 1e-9;       // done

#define TR(X) ({ if(1) cerr << __LINE__ << ": " #X " = " << (X) << endl; })

vl dijkstra(vl &start, vvl &adj) {
	ll n = sz(start);
	assert(sz(adj) == n);
	vl res = start;
	vl done(n);
	FOR(rd, 0, n) {
		ll next = -1;
		FOR(nd, 0, n) {
			if (!done[nd] && (next == -1 || res[nd] < res[next]))
				next = nd;
		}
		done[next] = true;
		FOR(i, 0, n) res[i] = min(res[next] + adj[next][i], res[i]);
	}
	return res;
}

int main() { 
	cin.sync_with_stdio(0); cin.tie(0);
	TR(oo);

	ll n, m; cin >> n >> m;
	vvl adj(n, vl(n, oo));
	map<pll, ll> e_index;
	FOR(i, 0, m) {
		ll u, v, w; cin >> u >> v >> w;
		if (w < adj[u][v]) {
			adj[u][v] = w;
			adj[v][u] = w;
			e_index[{u, v}] = i;
			e_index[{v, u}] = i;
		}
	}
	ll k; cin >> k;
	vl stei_set(k);
	for (ll &x : stei_set) cin >> x;

	if (k == 1) {
		cout << "0 0\n";
		return 0;
	}

	vvl dp(1 << (k - 1), vl(n, oo));
	FOR(i, 0, k - 1) dp[1 << i][stei_set[i]] = 0;
	FOR(ss, 1, 1 << (k - 1)) {
		FOR(i, 0, n) {
			for (ll p1 = ss, p2 = 0; p1 > p2; p1 = (p1-1) & ss, p2 = ss ^ p1) {
				dp[ss][i] = min(dp[ss][i], dp[p1][i] + dp[p2][i]);
			}
		}
		dp[ss] = dijkstra(dp[ss], adj);
	}
	ll ms = (1 << (k - 1)) - 1;
	ll i = stei_set.back();

	vpll edg;
	auto find_tree = [&](const auto &self, ll ss, ll i) {
		if (dp[ss][i] == 0) return;
		FOR(j, 0, n) {
			if (dp[ss][i] == adj[j][i] + dp[ss][j]) {
				self(self, ss, j);
				edg.pb({i, j});
				return;
			}
		}
		for (ll p1 = ss; p1; p1 = (p1-1) & ss) {
			ll p2 = ss ^ p1;
			if (dp[ss][i] == dp[p1][i] + dp[p2][i]) {
				self(self, p1, i), self(self, p2, i);
				return;
			}
		}
	};
	find_tree(find_tree, ms, i);
	cout << dp[ms][i] << " " << sz(edg) << "\n";
	for (auto [x, y] : edg) cout << e_index[{x, y}] << "\n";
}
