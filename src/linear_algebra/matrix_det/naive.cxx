#include <toy/io.h>
#include <toy/determinant.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,3>();Buffer<u32>a(n*n);for(u32&x:std::span(a.p,a.n))x=in.read<u32,9>();out.write(determinant<998244353,false>(n,std::span(a.p,a.n)));}
