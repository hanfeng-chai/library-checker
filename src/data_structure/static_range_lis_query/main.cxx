#include <toy/io.h>
#include <toy/lis.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),q=in.read<u32,6>();Buffer<u32>a(n);for(auto& x:span(a.p,a.n))x=in.read<u32,6>();Buffer<LisRange> ranges(q);for(auto& range:span(ranges.p,ranges.n)){auto[l,r]=in.read_pair<6>();range={l,r};}auto answer=range_lis(span<const u32>(a),span<const LisRange>(ranges));out.write(span(answer.p,answer.n));}
