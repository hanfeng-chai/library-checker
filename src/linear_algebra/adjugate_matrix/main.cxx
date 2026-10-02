#include <toy/io_batch.h>
#include <toy/matrix_inverse.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,3>();Buffer<u32>a(n*n);read_bulk9(in,std::span(a.p,a.n));SquareReduction reduction(n,std::span(a.p,a.n));auto result=reduction.result(true);write_bulk9(out,std::span<const u32>(result.p,result.n));out.put('\n');}
