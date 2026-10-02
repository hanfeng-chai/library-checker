#include <toy/io_batch.h>
#include <toy/frobenius.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,3>();Buffer<u32>a(n*n);read_bulk9(in,std::span(a.p,a.n));auto p=characteristic_polynomial(n,std::span(a.p,a.n));out.write(std::span(p.p,p.n),' ');out.put('\n');}
