#include <toy/io_batch.h>
#include <toy/hafnian_cycles.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,3>();Buffer<u32>a(n*n);read_bulk9(in,std::span(a.p,a.n));out.write(hafnian_cycles(n,std::span(a.p,a.n)));}
