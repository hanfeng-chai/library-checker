#include <toy/io.h>
#include <toy/rectangle_add_sum.h>
using namespace toy;
struct Query{u32 left,down,right,up;};
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),q=in.read<u32,6>();Buffer<WeightedRectangle> rectangles(n);Buffer<Query> queries(q);
    for(auto& r:std::span(rectangles.p,rectangles.n))r={in.read<u32,10>(),in.read<u32,10>(),in.read<u32,10>(),in.read<u32,10>(),in.read<u32,16>()};
    for(auto& r:std::span(queries.p,queries.n))r={in.read<u32,10>(),in.read<u32,10>(),in.read<u32,10>(),in.read<u32,10>()};
    auto answer=rectangle_add_sum(std::span<const WeightedRectangle>(rectangles),std::span<const Query>(queries));out.write(std::span(answer.p,answer.n));}
