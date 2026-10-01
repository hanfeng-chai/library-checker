#pragma once
#include <toy/bilinear.h>
#include <toy/fenwick.h>
#include <toy/radix_sort.h>
namespace toy {
struct WeightedRectangle{u32 left,down,right,up,weight;};
// Query supplies left/down/right/up fields; all rectangles are half-open.
template<u32 P=998244353,class Query>
Buffer<u32> rectangle_add_sum(std::span<const WeightedRectangle> rectangles,std::span<const Query> queries){
    using M=Mod<P>;using Coefficient=Bilinear<P>;Buffer<u32> answers(queries.size());std::fill(answers.p,answers.p+answers.n,0u);if(rectangles.empty()||queries.empty())return answers;
    struct Key{u32 value,slot;};usize base=2*rectangles.size();Buffer<Key> coordinates(base+2*queries.size());
    for(u32 i=0;i<rectangles.size();++i){coordinates[2*i]={rectangles[i].down,2*i};coordinates[2*i+1]={rectangles[i].up,2*i+1};}
    for(u32 i=0;i<queries.size();++i){coordinates[base+2*i]={queries[i].down,u32(base+2*i)};coordinates[base+2*i+1]={queries[i].up,u32(base+2*i+1)};}
    radix_sort(std::span(coordinates.p,coordinates.n),[](Key x){return x.value;});Buffer<u32> rank(coordinates.n);u32 count=0;
    for(u32 i=0;i<coordinates.n;++i){if(!i||coordinates[i].value!=coordinates[i-1].value)++count;rank[coordinates[i].slot]=count-1;}coordinates={};
    struct Event{u32 x,tag;};Buffer<Event> updates(base),events(2*queries.size());
    for(u32 i=0;i<rectangles.size();++i){updates[2*i]={rectangles[i].left,2*i};updates[2*i+1]={rectangles[i].right,2*i+1};}
    for(u32 i=0;i<queries.size();++i){events[2*i]={queries[i].left,2*i};events[2*i+1]={queries[i].right,2*i+1};}
    radix_sort(std::span(updates.p,updates.n),[](Event e){return e.x;});radix_sort(std::span(events.p,events.n),[](Event e){return e.x;});Fenwick<Coefficient> tree(count);usize used=0;
    for(auto event:std::span(events.p,events.n)){
        while(used<updates.n&&updates[used].x<event.x){
            if(used+8<updates.n){u32 tag=updates[used+8].tag&-2u;__builtin_prefetch(rectangles.data()+tag/2,0,3);__builtin_prefetch(tree.tree.p+rank[tag]+1,1,3);__builtin_prefetch(tree.tree.p+rank[tag+1]+1,1,3);}
            auto e=updates[used++];auto r=rectangles[e.tag/2];u32 w=e.tag&1?M::sub(0,r.weight):r.weight,wx=M::mul(w,e.x);
            Coefficient lower{{w,M::sub(0,M::mul(w,r.down)),M::sub(0,wx),M::mul(wx,r.down)}};
            Coefficient upper{{M::sub(0,w),M::mul(w,r.up),wx,M::sub(0,M::mul(wx,r.up))}};
            tree.add(rank[e.tag&-2u],lower);tree.add(rank[(e.tag&-2u)+1],upper);
        }
        u32 id=event.tag/2;auto q=queries[id];auto below=tree.prefix(rank[base+2*id]),above=tree.prefix(rank[base+2*id+1]);u32 value=M::sub(above(event.x,q.up),below(event.x,q.down));
        answers[id]=event.tag&1?M::add(answers[id],value):M::sub(answers[id],value);
    }
    return answers;
}
}
