#include<bits/stdc++.h>
using namespace std;
using ll=long long;

bitset<4100>M[4100];

int main(){
        ios::sync_with_stdio(false);
        cin.tie(nullptr);
        cout.tie(nullptr);
        int n,m;
        cin>>n>>m;
        for(int i=0;i<n;++i){
                string s;
                cin>>s;
                for(int j=0;j<m;++j)
                        if(s[j]-48)
                                M[i].set(j);
        }
        string s;
        cin>>s;
        for(int i=0;i<n;++i)
                if(s[i]-48)M[i].set(m);
        int r=0;
        vector<int>pv(m,-1);
        for(int i=0;r<n and i<m;++i){
                int p=-1;
                for(int j=r;j<n;++j)
                        if(M[j][i]){
                                p=j;
                                break;
                        }
                if(p==-1)continue;
                swap(M[r],M[p]);
                pv[i]=r;
                for(int j=0;j<n;++j)
                        if(j^r and M[j][i])
                                M[j]^=M[r];
                ++r;
        }
        for(int i=r;i<n;++i)
                if(M[i][m]){
                        cout<<-1<<'\n';
                        return 0;
                }
        string c(m,'0');
        vector<string>d;
        for(int i=0;i<m;++i){
                if(~pv[i])c[i]=(char)(M[pv[i]][m]+48);
                else{
                        string b(m,'0');
                        b[i]='1';
                        for(int j=0;j<m;++j)
                                if(~pv[j])b[j]=(char)(M[pv[j]][i]+48);
                        d.push_back(b);
                }
        }
        cout<<d.size()<<'\n';
        cout<<c<<'\n';
        for(const string&i:d)
                cout<<i<<'\n';
}