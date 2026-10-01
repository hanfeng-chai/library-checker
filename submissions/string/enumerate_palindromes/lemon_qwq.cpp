#pragma GCC optimize("Ofast", "unroll-loops")
#ifndef _FASTIO_H_
#define _FASTIO_H_
#define _GNU_SOURCE //for fwrite_unlocked
#include<time.h>
#include<ctype.h>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#pragma region IO output
#include<string.h>
#ifndef IO_buffer_size
#define IO_buffer_size (1u<<20)
#endif
static char _IO_buf[IO_buffer_size+100];
static unsigned _IO_optr=0;
#define IO_flush() (fwrite_unlocked(_IO_buf,1,_IO_optr,stdout),_IO_optr=0)
#define _IO_chk() ((void)(_IO_optr<IO_buffer_size||IO_flush()))
#define _IO_pc_nochk(x) (_IO_buf[_IO_optr++]=(x))
static inline __attribute__((always_inline))void IO_pc(char x){_IO_pc_nochk(x),_IO_chk();}
#define A(x) x|48<<24,x|49<<24,x|50<<24,x|51<<24,x|52<<24,x|53<<24,x|54<<24,x|55<<24,x|56<<24,x|57<<24,
#define B(x) A(x|48<<16)A(x|49<<16)A(x|50<<16)A(x|51<<16)A(x|52<<16)A(x|53<<16)A(x|54<<16)A(x|55<<16)A(x|56<<16)A(x|57<<16)
#define C(x) B(x|48<<8)B(x|49<<8)B(x|50<<8)B(x|51<<8)B(x|52<<8)B(x|53<<8)B(x|54<<8)B(x|55<<8)B(x|56<<8)B(x|57<<8)
static const unsigned _IO_omp[10000]={C(48)C(49)C(50)C(51)C(52)C(53)C(54)C(55)C(56)C(57)};
#undef A
#undef B
#undef C
#define S _IO_buf+_IO_optr
static inline __attribute__((always_inline,hot))void IO_pui(unsigned y){
	unsigned b=y/10000u;
	if(b){
		unsigned c=y%10000u;
		if(b>=1000u)memcpy(S,_IO_omp+b,4),_IO_optr+=4;
		else if(b>=100u)memcpy(S,(char*)(_IO_omp+b)+1,3),_IO_optr+=3;
		else if(b>=10u)memcpy(S,(char*)(_IO_omp+b)+2,2),_IO_optr+=2;
		else _IO_pc_nochk(b|48);
		memcpy(S,_IO_omp+c,4);
		_IO_optr+=4;
	}else{
		if(y>=1000u)memcpy(S,_IO_omp+y,4),_IO_optr+=4;
		else if(y>=100u)memcpy(S,(char*)(_IO_omp+y)+1,3),_IO_optr+=3;
		else if(y>=10u)memcpy(S,(char*)(_IO_omp+y)+2,2),_IO_optr+=2;
		else _IO_pc_nochk(y|48);
	}_IO_chk();
}
static inline __attribute__((always_inline,hot))void IO_pi(int x){
	unsigned y;
	if(__builtin_expect(x<0,0))_IO_pc_nochk('-'),y=-(unsigned)x;
	else y=x;
	IO_pui(y);
}
static inline __attribute__((always_inline,hot))void IO_pull(unsigned long long y){
	unsigned a=y/0x2386F26FC10000ull,b=y%0x2386F26FC10000ull/0xE8D4A51000ull,c=y%0xE8D4A51000ull/0x5F5E100,d=y%0x5F5E100/10000,e=y%10000;
	if(a){
		if(a>=1000u)memcpy(S,_IO_omp+a,4),_IO_optr+=4;
		else if(a>=100u)memcpy(S,(char*)(_IO_omp+a)+1,3),_IO_optr+=3;
		else if(a>=10u)memcpy(S,(char*)(_IO_omp+a)+2,2),_IO_optr+=2;
		else _IO_pc_nochk(a|48);
		memcpy(S,_IO_omp+b,4),memcpy(S+4,_IO_omp+c,4),memcpy(S+8,_IO_omp+d,4),memcpy(S+12,_IO_omp+e,4);
		_IO_optr+=16;
	}else if(b){
		if(b>=1000u)memcpy(S,_IO_omp+b,4),_IO_optr+=4;
		else if(b>=100u)memcpy(S,(char*)(_IO_omp+b)+1,3),_IO_optr+=3;
		else if(b>=10u)memcpy(S,(char*)(_IO_omp+b)+2,2),_IO_optr+=2;
		else _IO_pc_nochk(b|48);
		memcpy(S,_IO_omp+c,4),memcpy(S+4,_IO_omp+d,4),memcpy(S+8,_IO_omp+e,4);
		_IO_optr+=12;
	}else if(c){
		if(c>=1000u)memcpy(S,_IO_omp+c,4),_IO_optr+=4;
		else if(c>=100u)memcpy(S,(char*)(_IO_omp+c)+1,3),_IO_optr+=3;
		else if(c>=10u)memcpy(S,(char*)(_IO_omp+c)+2,2),_IO_optr+=2;
		else _IO_pc_nochk(c|48);
		memcpy(S,_IO_omp+d,4),memcpy(S+4,_IO_omp+e,4);
		_IO_optr+=8;
	}else if(d){
		if(d>=1000u)memcpy(S,_IO_omp+d,4),_IO_optr+=4;
		else if(d>=100u)memcpy(S,(char*)(_IO_omp+d)+1,3),_IO_optr+=3;
		else if(d>=10u)memcpy(S,(char*)(_IO_omp+d)+2,2),_IO_optr+=2;
		else _IO_pc_nochk(d|48);
		memcpy(S,_IO_omp+e,4);
		_IO_optr+=4;
	}else{
		if(e>=1000u)memcpy(S,_IO_omp+e,4),_IO_optr+=4;
		else if(e>=100u)memcpy(S,(char*)(_IO_omp+e)+1,3),_IO_optr+=3;
		else if(e>=10u)memcpy(S,(char*)(_IO_omp+e)+2,2),_IO_optr+=2;
		else _IO_pc_nochk(e|48);
	}_IO_chk();
}
static inline __attribute__((always_inline,hot))void IO_pll(long long x){
	unsigned long long y;
	if(__builtin_expect(x<0,0))_IO_pc_nochk('-'),y=-(unsigned long long)x;
	else y=x;
	IO_pull(y);
}
#undef S
#undef _IO_chk
// #undef _IO_pc_nochk
static inline __attribute__((always_inline,destructor))void _IO_flusher(){IO_flush();}
#pragma endregion
#endif

#include<stdio.h>
#include<string.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
char s[500005];
int p[500005],q[1000005],n;
char*ptr;
int sz=-1;
static inline int min(int x,int y){
	return x<y?x:y;
}
static inline int max(int x,int y){
	return x>y?x:y;
}
int main(){
	struct stat st;
	fstat(0,&st);
	sz=st.st_size;
	ptr=(char*)mmap(0,sz,PROT_READ,MAP_PRIVATE,0,0);
	while(ptr[sz-1]<'a')sz--;
	n=sz;
	s[0]='~';
	memcpy(s+1,ptr,n);
	for(int i=1,l=1,r=0;i<=n;i++){
		p[i]=1;
		if(i<=r)p[i]=min(p[l+r-i],r-i+1);
		int a=i-p[i],b=i+p[i];
		while(s[a]==s[b])a--,b++;
		p[i]=b-i;
		if(i+p[i]>=r)l=i-p[i]+1,r=i+p[i]-1;
		q[(i<<1)-1]=(p[i]<<1)-1;
	}
	for(int i=1,l=1,r=0;i<=n;i++){
		p[i]=0;
		if(i<=r)p[i]=min(p[l+r-i+1],r-i+1);
		int a=i-p[i]-1,b=i+p[i];
		while(s[a]==s[b])a--,b++;
		p[i]=b-i;
		if(i+p[i]>=r)l=i-p[i],r=i+p[i]-1;
		q[(i-1<<1)]=p[i]<<1;
	}
	for(int i=1;i<2*n;i++){
		IO_pui(q[i]);
		_IO_pc_nochk(' ');
	}
	return 0;
}