#include <toy/io.h>
#include <toy/hash.h>
#include <toy/offline_frequency.h>
using namespace toy;
int main(){
    Reader in;Writer out;u32 n=in.read<u32,6>(),q=in.read<u32,6>(),value_count=0,answers=0;HashMap<u32,u32> ids(n+q);
    auto id=[&](u32 x){auto& entry=ids[x];if(!entry)entry=++value_count;return entry-1;};
    Buffer<u32> initial(n),current(n);for(u32 i=0;i<n;++i)initial[i]=current[i]=id(in.read<u32,10>());if(!n&&q)while(*in.p<=' ')++in.p;
    Buffer<FrequencyEvent> events(0,2*q);
    while(q--){u32 type=in.read<u32,1>(),l=in.read<u32,6>();if(!type){u32 value=id(in.read<u32,10>());events[events.n++]=FrequencyEvent::change(current[l],l,false);current[l]=value;events[events.n++]=FrequencyEvent::change(value,l,true);}
        else{u32 r=in.read<u32,6>(),value=ids.get(in.read<u32,10>());if(value)events[events.n++]=FrequencyEvent::query(value-1,l,r,answers);++answers;}}
    auto result=point_value_frequencies(value_count,std::span<const u32>(initial),std::span<const u32>(current),std::move(events),answers);out.write(std::span(result.p,result.n));
}
