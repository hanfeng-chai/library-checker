#pragma once
#include <toy/buffer.h>
namespace toy {
template<class T,bool Minimum=false> struct BinaryHeap {
    Buffer<T> values;
    static bool better(T a,T b){if constexpr(Minimum)return a<b;else return a>b;}
    bool empty()const{return !values.n;}
    T top()const{return values[0];}
    void clear(){values.n=0;}
    void down(usize i){T x=values[i];for(usize child; (child=2*i+1)<values.n;){if(child+1<values.n&&better(values[child+1],values[child]))++child;if(!better(values[child],x))break;values[i]=values[child];i=child;}values[i]=x;}
    void heapify(){for(usize i=values.n/2;i--;)down(i);}
    void push(T x){if(values.n==values.capacity)values.reserve(max<usize>(4,2*values.capacity));usize i=values.n++;while(i){usize parent=(i-1)/2;if(!better(x,values[parent]))break;values[i]=values[parent];i=parent;}values[i]=x;}
    T pop(){T result=values[0],last=values[--values.n];if(values.n){values[0]=last;down(0);}return result;}
    T replace_top(T x){T result=values[0];values[0]=x;down(0);return result;}
    T push_pop(T x){return better(values[0],x)?replace_top(x):x;}
};
}
