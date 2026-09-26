#include<bits/stdc++.h>
#define int long long
#define pb push_back
#define fast ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#define MOD 1000000007
#define inf 1e18
#define fi first
#define se second
#define FOR(i,a,b) for(int i=a;i<=b;i++)
#define FORD(i,a,b) for(int i=a;i>=b;i--)
#define sz(a) ((int)(a).size())
#define endl '\n'
#define pi 3.14159265359
#define TASKNAME "mdst"
using namespace std;
template<typename T> bool maximize(T &res, const T &val) { if (res < val){ res = val; return true; }; return false; }
template<typename T> bool minimize(T &res, const T &val) { if (res > val){ res = val; return true; }; return false; }
typedef pair<int,int> ii;
typedef pair<int,ii> iii;
typedef vector<int> vi;
const int MAXN = 2e3 + 9;

iii edge[MAXN];
int par[MAXN], n, m;
vector<int> g[MAXN], tree[MAXN];

int best = inf;
vector<int> ansEdge;

int dist[MAXN], trace[MAXN], del[MAXN], dp[MAXN];

void dijkstra(int s){
    memset(dist, 0x3f, sizeof(dist));
    dist[s] = 0;
    trace[s] = -1;

    priority_queue<ii, vector<ii>, greater<ii>> pq;
    pq.push({0, s});

    while(!pq.empty()){
        int du = pq.top().fi;
        int u = pq.top().se;
        pq.pop();

        if (du > dist[u]) continue;

        for(auto id: g[u]){
            int v = edge[id].se.fi + edge[id].se.se - u;
            int w = edge[id].fi;
            if (minimize(dist[v], dist[u] + w)){
                pq.push({dist[v], v});
                trace[v] = id;
            }
        }
    }
}

void update(ii &b, int x){
    if (x > b.fi) tie(b.se, b.fi) = ii(b.fi, x);
    else if (x > b.se) tie(b.se, b.fi) = ii(x, b.fi);
}
void dfs(int u, int p, int &ans){

    ii best = {0, 0};
    for(auto id: tree[u]){
        int v = edge[id].se.se + edge[id].se.fi - u;
        int w = edge[id].fi;
        if (v == p) continue;
        dfs(v, u, ans);
        update(best, dp[v] + w);
    }

    maximize(ans, best.fi + best.se);
    maximize(dp[u], best.fi);
}
void trying(int s){
    dijkstra(s);
    memset(del, false, sizeof(del));
    vector<int> listEdge;

    FOR(i, 0, n - 1){
        int cur = i;
        while(cur != s) {
            int edge_id = trace[cur];
            int nxt = edge[edge_id].se.fi + edge[edge_id].se.se - cur;
            if (del[edge_id]) break;
            del[edge_id] = true;

            listEdge.pb(edge_id);
            tree[cur].pb(edge_id);
            tree[nxt].pb(edge_id);
            cur = nxt;
        }
    }

    int diameter = 0;
    dfs(s, -1, diameter);

    if (minimize(best, diameter)){
        ansEdge = listEdge;
    }

    FOR(i, 0, n - 1){
        dp[i] = 0;
        tree[i].clear();
    }
}
main()
{
    fast;
    if (fopen(TASKNAME".inp","r")){
        freopen(TASKNAME".inp","r",stdin);
        freopen(TASKNAME".out","w",stdout);
    }
    cin >> n >> m;
    FOR(i, 0, m - 1){
        cin >> edge[i].se.fi >> edge[i].se.se >> edge[i].fi;
        g[edge[i].se.fi].pb(i);
        g[edge[i].se.se].pb(i);
    }

    FOR(i, 0, n - 1){
        trying(i);
    }

    cout << best << endl;
    for(auto id: ansEdge){
        cout << id << ' ';
    }
    cout << endl;
}
/**
Warning:
Đọc sai đề???
Cận lmao
Code imple thiếu case nào không.
Limit.
**/
