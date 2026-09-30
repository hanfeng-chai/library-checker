#pragma once
#include <toy/array.h>
#include <toy/fenwick.h>
#include <toy/radix_sort.h>
namespace toy {
template<class T=i64> struct RectangleAddPointGet {
    struct Rectangle{u32 left,down,right,up;T weight;};
    struct Point{u32 x,y,time;};struct Key{u32 coordinate,id;};
    Buffer<Rectangle> rectangles;Buffer<Point> points;
    template<class U> static void append(Buffer<U>& values,U value){if(values.n==values.capacity)values.reserve(max<usize>(4,2*values.capacity));values[values.n++]=value;}
    RectangleAddPointGet(usize updates=0,usize queries=0):rectangles(0,updates),points(0,queries){}
    void add(u32 l,u32 d,u32 r,u32 u,T value){if(l<r&&d<u)append(rectangles,Rectangle{l,d,r,u,value});}
    u32 query(u32 x,u32 y){u32 id=points.n;append(points,Point{x,y,u32(rectangles.n)});return id;}
    Buffer<T> solve()const{
        Buffer<T> answers(points.n);fill(answers.p,answers.p+answers.n,T{});u32 n=points.n?points[points.n-1].time:0;if(!n)return answers;
        Buffer<Key> x(2*n),y(2*n),temporary(2*n);for(u32 i=0;i<n;++i){auto r=rectangles[i];x[2*i]={r.left,2*i};x[2*i+1]={r.right,2*i+1};y[2*i]={r.down,2*i};y[2*i+1]={r.up,2*i+1};}
        Array<Buffer<Key>> qx(n),qy(n);
        for(u32 i=0;i<points.n;++i)if(points[i].time){u32 end=bit_floor(points[i].time);append(qx[end-1],Key{points[i].x,i});append(qy[end-1],Key{points[i].y,i});}
        auto merge=[&](Buffer<Key>& a,u32 first,u32 middle,u32 last){u32 i=first,j=middle,k=first;while(i<middle&&j<last)temporary[k++]=a[i].coordinate<=a[j].coordinate?a[i++]:a[j++];while(i<middle)temporary[k++]=a[i++];while(j<last)temporary[k++]=a[j++];memcpy(a.p+first,temporary.p+first,usize(last-first)*sizeof(Key));};
        Buffer<u32> yrank(2*n),point_end(points.n);
        u32 start=1;for(auto p:span(points.p,points.n))if(p.time){start=bit_floor(p.time);break;}
        // Before the first query, only the complete initial time block is needed.
        radix_sort(span(x.p,2*start),[](Key x){return x.coordinate;});
        radix_sort(span(y.p,2*start),[](Key x){return x.coordinate;});
        for(u32 end=start;end<=n;++end){u32 width=end&-end;
            for(u32 half=1;end!=start&&half<width;half<<=1){merge(x,2*(end-2*half),2*(end-half),2*end);merge(y,2*(end-2*half),2*(end-half),2*end);}
            auto& a=qx[end-1];auto& b=qy[end-1];if(!a.n)continue;
            if(end==width){radix_sort(span(a.p,a.n),[](Key x){return x.coordinate;});radix_sort(span(b.p,b.n),[](Key x){return x.coordinate;});}
            u32 first=2*(end-width),last=2*end,count=0;
            for(u32 i=first;i<last;++i){if(i==first||y[i].coordinate!=y[i-1].coordinate)++count;yrank[y[i].id]=count-1;}
            u32 at=first,passed=0;for(auto q:span(b.p,b.n)){while(at<last&&y[at].coordinate<=q.coordinate){u32 value=y[at].coordinate;++passed;do{++at;}while(at<last&&y[at].coordinate==value);}point_end[q.id]=passed;}
            Fenwick<T> tree(count);at=first;
            for(auto q:span(a.p,a.n)){while(at<last&&x[at].coordinate<=q.coordinate){u32 id=x[at++].id;T value=rectangles[id/2].weight;if(id&1)value=-value;u32 l=yrank[id&-2u],r=yrank[(id&-2u)+1];
                // The two opposite updates cancel at their first common Fenwick ancestor.
                u32 stop=min(count+1,(l|((u32(1)<<bit_width(l^r))-1))+1);
                for(++l;l<stop;l+=l&-l)tree.tree[l]+=value;
                for(++r;r<stop;r+=r&-r)tree.tree[r]-=value;}answers[q.id]+=tree.prefix(point_end[q.id]);}
            // Each child time block has one parent, so appended query orders stay sorted.
            for(auto q:span(a.p,a.n)){u32 rest=points[q.id].time-end;if(rest)append(qx[end+bit_floor(rest)-1],q);}
            for(auto q:span(b.p,b.n)){u32 rest=points[q.id].time-end;if(rest)append(qy[end+bit_floor(rest)-1],q);}a={};b={};
        }
        return answers;
    }
};
}
