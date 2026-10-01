#pragma once
#include <toy/buffer.h>
namespace toy {
// Uninitialized storage; large blocks use aligned transparent huge pages.
template<class T>Buffer<T> page_buffer(usize n){
    if(n*sizeof(T)<(1<<20))return Buffer<T>(n);
    usize bytes=(n*sizeof(T)+(1<<21)-1)&-usize(1<<21);Buffer<T>a;
    a.p=(T*)aligned_alloc(1<<21,bytes);a.n=a.capacity=n;madvise(a.p,bytes,MADV_HUGEPAGE);return a;
}
}
