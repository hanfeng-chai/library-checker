#pragma once
#include <toy/buffer.h>
namespace toy {
// Min-heap Cartesian tree; equal values keep the earlier position above the later.
template<class T,class Compare=std::less<T>> Buffer<u32> cartesian_tree(std::span<const T> values,Compare less={}){
    struct Entry{T value;u32 index;};Buffer<u32> parent(values.size());Buffer<Entry> stack(values.size());u32 count=0;
    for(u32 i=0;i<values.size();++i){u32 last=~0u;while(count&&less(values[i],stack[count-1].value))last=stack[--count].index;
        parent[i]=count?stack[count-1].index:i;if(last!=~0u)parent[last]=i;stack[count++]={values[i],i};}
    return parent;
}
}
