#pragma GCC optimize("O2","O3","Ofast")
#pragma GCC target("sse","sse2","avx","avx2")
#include<cstring>
#include<cstdio>
using UI=unsigned;
const UI SIZI=334*2e5+1e3;
char bufi[SIZI];	char *Si=bufi;
inline UI read(){
	UI x=0;	char ch=*Si++;
	for(;(ch<'0')||(ch>'9');ch=*Si++);
	for(;(ch>='0')&&(ch<='9');ch=*Si++)
		x=x*10+ch-'0';
	return x;
}
const UI SIZO=334*1e5+1e3;
char bufo[SIZO];	int bufoidx;
#define pc(c) (bufo[bufoidx++]=c)
inline void write(UI x){
	static UI stk[10];	UI top=0;
	do{
		stk[top++]=x%10;	x/=10;
	}
	while(x);
	for(;top;pc('0'+stk[--top]));
	pc(' ');	return;
}
const UI N=30;
#define highbit(x) (31-__builtin_clz(x))
struct LB{
	UI a[N];	UI cnt;
	UI &operator[](UI x){
		return a[x];
	}
	UI operator[](const UI &x)const{
		return a[x];
	}
	void init(){
		memset(a,0,N<<2);	cnt=0;	return;
	}
	void insert(UI x){
		if((cnt==N)||(!x))
			return;
		for(UI i;x;x^=a[i])
			if(!a[i=highbit(x)]){
				a[i]=x;	cnt++;	break;
			}
		return;
	}
	LB operator&(const LB &b)const{
		if(cnt==N)
			return b;
		if(b.cnt==N)
			return *this;
		UI bp[N];	memset(bp,0,N<<2);
		LB res,c=*this;	res.init();
		for(UI i=N-1,x,bx;~i;i--){
			if(!(bx=(x=b[i])))
				continue;
			for(UI j;x;x^=c[j],bx^=bp[j])
				if(!c[j=highbit(x)]){
					c[j]=x;	c.cnt++;
					bp[j]=bx;	bx=0;
					break;
				}
			res.insert(bx);
		}
		return res;
	}
	void print(){
		write(cnt);
		for(UI i=N-1;~i;i--)
			a[i]&&(write(a[i]),true);
		pc('\n');	return;
	}
};
int main(){
	fread(Si=bufi,1,SIZI,stdin);
	for(UI _=read();_--;){
		LB a,b;	a.init();	b.init();
		for(UI n=read();n--;a.insert(read()));
		for(UI n=read();n--;b.insert(read()));
		(a&b).print();
	}
	fwrite(bufo,1,bufoidx,stdout);	return 0;
}