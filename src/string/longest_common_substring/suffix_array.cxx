#include <toy/io.h>
#include <toy/suffix_array.h>
using namespace toy;
int main(){Reader in;Writer out;auto s=in.token(),t=in.token();u32 n=s.size(),m=t.size();Buffer<char>text(n+m+1);memcpy(text.p,s.data(),n);text[n]=0;memcpy(text.p+n+1,t.data(),m);auto sa=suffix_array(std::string_view(text.p,text.n));auto lcp=lcp_array(std::string_view(text.p,text.n),sa);u32 best=0,a=0,b=0;for(u32 i=0;i<lcp.n;++i){u32 x=sa[i],y=sa[i+1];if(x==n||y==n||(x<n)==(y<n))continue;if(lcp[i]>best){best=lcp[i];if(x>n)std::swap(x,y);a=x;b=y-n-1;}}out.write(a,' ');out.write(a+best,' ');out.write(b,' ');out.write(b+best);}
