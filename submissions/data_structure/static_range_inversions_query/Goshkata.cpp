#include <bits/stdc++.h>
#pragma GCC optimize("O3")
#define endl '\n'

using namespace std;

const int maxn = 1e5 + 3; 
const int bucket = 350;

struct query
{
	int l, r, idx;
	bool operator < (const query &t) const 
	{
		if (l / bucket != t.l / bucket)
		return l / bucket < t.l / bucket; 
		
		else
		return ((l / bucket) & 1) ? r < t.r : r > t.r; 
	}
	
	query() {}
	query(int _l, int _r, int _idx) {l = _l, r = _r, idx = _idx;}
};

struct range_query
{
	int l, r, mult, idx; 
	bool type; 
	
	range_query() {}
	range_query(int _l, int _r, int _mult, int _idx, bool _type)
	{
		l = _l;
		r = _r; 
		mult = _mult;
		idx = _idx;
		type = _type; 
	}
};

int n, q;
int a[maxn];
query queries[maxn];
void read()
{
	cin >> n >> q;
	for (int i = 1; i <= n; i++)
	cin >> a[i];
	
	for (int i = 1; i <= q; i++)
	{
		cin >> queries[i].l >> queries[i].r;
		
		queries[i].l++;
		queries[i].idx = i; 
	}
}

int lazy[maxn], val[maxn];
int get(int idx)
{
	return lazy[(idx - 1) / bucket] + val[idx];
}

int get_bigger(int idx)
{
	return get(idx + 1);
}

int get_smaller(int idx)
{
	return get(1) - get(idx);
}

void update(int idx)
{
	int bucket_idx = (idx - 1) / bucket; 
	for (int i = 0; i < bucket_idx; i++)
	lazy[i]++;
	
	for (int i = bucket_idx * bucket + 1; i <= idx; i++)
	val[i]++;
}

long long ans[maxn];
int val1[maxn][2], val2[maxn][2];
vector <range_query> to_add[maxn];
void process()
{
	for (int i = 1; i <= n; i++)
	{
		update(a[i]);
		val1[i][0] = get_bigger(a[i]);
		val1[i][1] = get_smaller(a[i]);
		val2[i][0] = get_bigger(a[i+1]);
		val2[i][1] = get_bigger(a[i+1]);
		
		for (auto j: to_add[i])
		{
			int add = 0; 
			for (int k = j.l; k <= j.r; k++)
			add += (j.type ? get_bigger(a[k]) : get_smaller(a[k]));
			 
			ans[j.idx] += j.mult * add;
		}
	}	
}

void solve()
{
	vector <int> v;
	for (int i = 1; i <= n; i++)
	v.push_back(a[i]);
	
	sort (v.begin(), v.end());
	for (int i = 1; i <= n; i++)
	a[i] = lower_bound(v.begin(), v.end(), a[i]) - v.begin() + 1; 
	
	sort (queries + 1, queries + q + 1);
	
	int l = 1, r = 1; 
	for (int i = 1; i <= q; i++)
	{
		if (queries[i].r > r)
		{
			to_add[l-1].push_back(range_query(r + 1, queries[i].r, -1, queries[i].idx, true));
			r = queries[i].r;
		}
		
		if (queries[i].l < l)
		{
			to_add[r].push_back(range_query(queries[i].l, l - 1, 1, queries[i].idx, false));
			l = queries[i].l;
		}
		
		if (queries[i].r < r)
		{
			to_add[l-1].push_back(range_query(queries[i].r + 1, r, 1, queries[i].idx, true));
			r = queries[i].r;
		}
		
		if (queries[i].l > l)
		{
			to_add[r].push_back(range_query(l, queries[i].l-1, -1, queries[i].idx, false));
			l = queries[i].l;
		}
	}
	
	process();
	
	l = r = 1;
	for (int i = 1; i <= q; i++)
	{
		if (i != 1)
		ans[queries[i].idx] += ans[queries[i-1].idx];
		
		while (r < queries[i].r)
		{
			ans[queries[i].idx] += val2[r][0];
			r++;
		}
		
		while (l > queries[i].l)
		{
			ans[queries[i].idx] -= val1[l-1][1];
			l--;
		}
		
		while (r > queries[i].r)
		{
			ans[queries[i].idx] -= val2[r-1][0];
			r--;
		}
		
		while (l < queries[i].l)
		{
			ans[queries[i].idx] += val1[l][1];
			l++;
		}
	}
	
	for (int i = 1; i <= q; i++)
	cout << ans[i] << endl; 
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	read();
	solve();
}
