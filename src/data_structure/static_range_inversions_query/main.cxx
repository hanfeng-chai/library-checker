#include <toy/io.h>
#include <toy/inversions.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),q=in.read<u32,6>();Buffer<u32>a(n);for(auto& x:std::span(a.p,a.n))x=in.read<u32,10>();Buffer<InversionRange> ranges(q);for(auto& range:std::span(ranges.p,ranges.n)){auto[l,r]=in.read_pair<6>();range={l,r};}auto answers=range_inversions(std::span<const u32>(a),std::span<const InversionRange>(ranges));out.write(std::span(answers.p,answers.n));}
