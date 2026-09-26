#include <bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace std;

#define optimizar_io ios_base::sync_with_stdio(0);cin.tie(0);
typedef pair<int, int>             pii;
typedef pair<long long, int>       pli;
typedef pair<pii, int>              piii;
typedef pair<long long, long long> pll;
typedef pair<pll, long long>        plll;
typedef vector<int>                vi;
typedef vector<bool>                vb;
typedef vector<vector<bool>>        vvb;
typedef vector<char>                vc;
typedef vector<vector<int>>        vvi;
typedef vector<long long>          vl;
typedef vector<vector<char>>       vvc;
typedef vector<vector<long long>>  vvl;
typedef vector<pii>                vpii;
typedef vector<piii>               vpiii;
typedef vector<pli>                vpli;
typedef vector<pll>                vpll;
typedef vector<plll>               vplll;
typedef vector<vector<plll>>       vvplll;
typedef vector<vector<pll>>        vvpll;
typedef vector<vector<piii>>       vvpiii;
typedef vector<vector<pii>>        vvpii;
typedef vector<vector<pli>>        vvpli;
typedef long long                  ll;

using namespace __gnu_pbds;
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

#define fi first
#define se second
#define pb push_back
#define mp make_pair
#define data data_
#define endl "\n"
#define isOn(S, j) ((S) & (1ll << (j)))
#define setBit(S, j) ((S) |= (1ll << (j)))
#define clearBit(S, j) ((S) &= ~(1ll << (j)))
#define toggleBit(S, j) (S ^= (1ll << (j)))
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define all(x) x.begin(),x.end()
#define sz(x) x.size()
#define dbg(v) cout << "Line(" << __LINE__ << ") -> " << #v << " = " << (v) << endl

long long MOD = 998244353;
long long sol = 0;
int touched[100];
long long v_vals[100];

void generate_cliques(int u_to_insert, int num_ins, vvi &g, long long cVal){
	sol += cVal;
	for (int v : g[u_to_insert]){
		touched[v]++;
	}
	for (int v : g[u_to_insert]){
		if (touched[v] == num_ins){
			long long newVal = (cVal * v_vals[v]) % MOD;
			generate_cliques(v, num_ins+1, g, newVal);
		}
	}
	for (int v : g[u_to_insert]){
		touched[v]--;
	}
}

int main(){
	optimizar_io;
	int n, m; cin >> n >> m;
	for (int i = 0; i < n; i++){
		cin >> v_vals[i];
	}
	vvi g(n);
	for (int i = 0; i < m; i++){
		int u, v; cin >> u >> v;
		g[min(u,v)].pb(max(u,v)); 
	}
	for (int i = 0; i < n; i++){
		generate_cliques(i, 1, g, v_vals[i]);
	}
	cout << sol % MOD << endl;
	return 0;
}
