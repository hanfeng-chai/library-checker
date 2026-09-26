#include<set>
#include<map>
#include<queue>
#include<vector>
#include<algorithm>
#include<bits/stdc++.h>
#define pr pair
#define f first
#define s second
#define ll long long
#define mp make_pair
#define pll pr<ll,ll>
#define pii pr<int,int>
#define piii pr<int,pii>
using namespace std;
int b[1<<19],ans[1<<19];
void sol(int n,int a[])
{
//	cout<<"S "<<n<<endl;
//	for(int i=0;i<n;i++) cout<<a[i]+1000000000<<' ';
//	cout<<endl;
//	for(int i=0;i<n;i++) cout<<b[i]+1000000000<<' ';
//	cout<<endl;
	vector<piii> cb;
	for(int i=0;i<n;i++)
	{
		if(i==0) cb.push_back(mp(i,mp(0,n-1)));
		else
		{
			int tr=i-1;
			while(cb.size())
			{
				piii g=cb.back();
				if(b[g.f]+a[g.s.s-g.f]>=b[i]+a[g.s.s-i])
				{
					tr=g.s.s;
					cb.pop_back();
					continue;
				}
				int m,r=g.s.s;
				while(tr<r-1)
				{
					m=tr+r>>1;
					if(b[g.f]+a[m-g.f]>=b[i]+a[m-i]) tr=m;
					else r=m;
				}
				cb.back().s.f=r;
				break;
			}
//			cout<<"Fb "<<i<<' '<<tr<<endl;
			if(tr>=i) cb.push_back(mp(i,mp(i,tr)));
		}
//		cout<<"E "<<i<<' ';
//		for(piii j:cb) cout<<j.f<<' '<<j.s.f<<'-'<<j.s.s<<"  ";
//		cout<<endl;
		piii&g=cb.back();
		ans[i]=b[g.f]+a[i-g.f];
		g.s.f++;
		if(g.s.f>g.s.s) cb.pop_back();
	}
//	for(int i=0;i<n;i++) cout<<ans[i]+2000000000<<' ';
//	cout<<endl;
}
int a[1<<19|1],ra[1<<19|1];
int lb[1<<20|1];
int op[3<<19|1];
int main()
{
	ios_base::sync_with_stdio(0);
	int n,m;
	cin>>n>>m;
	for(int i=0;i<n;i++) cin>>a[i],a[i]-=1e9,ra[n-i-1]=a[i];
	for(int i=0;i<m;i++) cin>>lb[i],lb[i]-=1e9;
	for(int i=m;i<1<<20;i++) lb[i]=1e9;
	for(int i=0;i<n+m-1;i++) op[i]=2e9;
	int ci;
	for(ci=0;ci<m;ci+=n+1)
	{
		for(int j=0;j<n;j++) b[j]=lb[ci+j];
		sol(n,a);
		for(int j=0;j<n;j++) op[ci+j]=min(op[ci+j],ans[j]);
		for(int j=0;j<n;j++) b[j]=lb[ci+n-j];
		sol(n,ra);
		for(int j=0;j<n;j++) op[ci+n*2-j-1]=min(op[ci+n*2-j-1],ans[j]);
	}
	for(int i=0;i<n+m-1;i++) op[i]+=2e9,cout<<op[i]<<' ';
	cout<<endl;
	return 0;
}