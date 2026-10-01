#pragma once
#include <toy/affine.h>
#include <toy/buffer.h>
#include <toy/hash.h>
#include <toy/montgomery.h>
namespace toy {
template<u32 P=998244353,u32 B=64> struct AffineKDTree {
    static_assert(B>=8&&B%8==0);
    using R=Montgomery<P>;using M=Mod<P>;using Function=Affine<P>;
    struct Point{u32 x,y,id;};struct Position{u32 node,at;};
    struct alignas(64) Node{__m128i box{};u32 child[2]{},parent=0,begin=0,end=0,count=0,sum=0;Function lazy{R::one,0};};
    struct Bounds{__m128i outside,inside;u32 left,down,width,height;};
    Buffer<Node> nodes;Buffer<Position> positions;Buffer<u32> xs,ys,values,active;u32 root=0,used=1;int preferred=-1;
    static Function identity(){return {R::one,0};}static bool empty(Function f){return f.a==R::one&&!f.b;}
    static Function compose(Function a,Function b){if(empty(a))return b;if(empty(b))return a;return {R::multiply(a.a,b.a),R::add(R::multiply(a.b,b.a),b.b)};}
    static u32 mapped(u32 value,u32 count,Function f){if(empty(f))return value;u64 z=u64(value)*f.a+u64(count)*f.b;u32 result=(z+u64(u32(z)*M::inverse)*P)>>32;return std::min(result,result-2*P);}
    static __m128i box(u32 l,u32 d,u32 r,u32 u){return _mm_setr_epi32(l^0x80000000u,d^0x80000000u,~(r^0x80000000u),~(u^0x80000000u));}
    static Bounds bounds(u32 l,u32 d,u32 r,u32 u){return {box(r-1,u-1,l,d),box(l,d,r-1,u-1),l,d,r-l,u-d};}
    static bool outside(const Node& x,const Bounds& q){return _mm_movemask_epi8(_mm_cmpgt_epi32(x.box,q.outside));}
    static bool covered(const Node& x,const Bounds& q){return !_mm_movemask_epi8(_mm_cmpgt_epi32(q.inside,x.box));}
    static u32 cardinality(std::span<const u32> values,u32 limit){HashMap<u64,u32> map(limit+1);u32 count=0;for(u32 value:values){auto& x=map[u64(value)];if(!x){x=1;if(++count>limit)break;}}return count;}
    void pull(u32 i){nodes[i].sum=R::add(nodes[nodes[i].child[0]].sum,nodes[nodes[i].child[1]].sum);nodes[i].count=nodes[nodes[i].child[0]].count+nodes[nodes[i].child[1]].count;}
    u32 build(Buffer<Point>& points,std::span<const u32> initial,u32 initial_count,u32 l,u32 r,u32 parent,bool axis){
        if(l==r)return 0;u32 i=used++,minx=points[l].x,maxx=minx,miny=points[l].y,maxy=miny;
        for(u32 j=l+1;j<r;++j){minx=std::min(minx,points[j].x);maxx=std::max(maxx,points[j].x);miny=std::min(miny,points[j].y);maxy=std::max(maxy,points[j].y);}
        auto& node=nodes[i];node.box=box(minx,miny,maxx,maxy);node.parent=parent;node.begin=l;node.end=r;
        if(r-l<=B){for(u32 j=l;j<r;++j){auto p=points[j];xs[j]=p.x;ys[j]=p.y;positions[p.id]={i,j};active[j]=p.id<initial_count?~0u:0;values[j]=active[j]?initial[p.id]:0;node.count+=bool(active[j]);node.sum=M::add(node.sum,values[j]);}return i;}
        if(preferred==0)axis=minx!=maxx;else if(preferred==1)axis=miny==maxy;else if(minx==maxx)axis=false;else if(miny==maxy)axis=true;
        u32 mid=(l+r)/2;std::nth_element(points.p+l,points.p+mid,points.p+r,[=](Point a,Point b){return axis?a.x<b.x:a.y<b.y;});
        node.child[0]=build(points,initial,initial_count,l,mid,i,!axis);node.child[1]=build(points,initial,initial_count,mid,r,i,!axis);pull(i);return i;
    }
    AffineKDTree(std::span<const u32> x,std::span<const u32> y,std::span<const u32> initial,u32 initial_count):nodes(4*x.size()/B+4),positions(x.size()),xs(x.size()+8),ys(x.size()+8),values(x.size()+8),active(x.size()+8){
        std::fill(nodes.p,nodes.p+nodes.n,Node{});for(usize i=x.size();i<x.size()+8;++i)xs[i]=ys[i]=values[i]=active[i]=0;
        u32 limit=(x.size()+B-1)/B;if(limit>=8){u32 nx=cardinality(x,limit),ny=cardinality(y,limit);if(nx<=limit/8&&nx<=ny/8)preferred=0;else if(ny<=limit/8&&ny<=nx/8)preferred=1;}
        Buffer<Point> points(x.size());for(u32 i=0;i<x.size();++i)points[i]={x[i],y[i],i};root=build(points,initial,initial_count,0,x.size(),0,true);
    }
    static __m256i tail(u32 count){return _mm256_cmpgt_epi32(_mm256_set1_epi32(count),_mm256_setr_epi32(0,1,2,3,4,5,6,7));}
    __m256i select(u32 at,u32 end,const Bounds& q)const{auto sign=_mm256_set1_epi32(0x80000000u);auto x=_mm256_sub_epi32(_mm256_loadu_si256((const __m256i*)(xs.p+at)),_mm256_set1_epi32(q.left));auto y=_mm256_sub_epi32(_mm256_loadu_si256((const __m256i*)(ys.p+at)),_mm256_set1_epi32(q.down));
        x=_mm256_cmpgt_epi32(_mm256_set1_epi32(q.width^0x80000000u),_mm256_xor_si256(x,sign));y=_mm256_cmpgt_epi32(_mm256_set1_epi32(q.height^0x80000000u),_mm256_xor_si256(y,sign));return _mm256_and_si256(_mm256_and_si256(x,y),_mm256_and_si256(_mm256_loadu_si256((const __m256i*)(active.p+at)),tail(end-at)));
    }
    static u64 total(__m256i x){auto y=_mm_add_epi64(_mm256_castsi256_si128(x),_mm256_extracti128_si256(x,1));return _mm_cvtsi128_si64(_mm_add_epi64(y,_mm_srli_si128(y,8)));}
    static void accumulate(__m256i& sum,__m256i x){sum=_mm256_add_epi64(sum,_mm256_add_epi64(_mm256_and_si256(x,_mm256_set1_epi64x(0xffffffffu)),_mm256_srli_epi64(x,32)));}
    void materialize(u32 i){auto& x=nodes[i];auto f=x.lazy;if(empty(f))return;auto a=_mm256_set1_epi32(f.a),b=_mm256_set1_epi32(R::decode(f.b));
        for(u32 j=x.begin;j<x.end;j+=8){auto value=_mm256_loadu_si256((const __m256i*)(values.p+j));value=M::add(M::mont(value,a),b);value=_mm256_and_si256(value,_mm256_loadu_si256((const __m256i*)(active.p+j)));_mm256_maskstore_epi32((int*)(values.p+j),tail(x.end-j),value);}x.lazy=identity();
    }
    void apply_node(u32 i,Function f){auto& x=nodes[i];if(!x.count)return;x.sum=mapped(x.sum,x.count,f);x.lazy=compose(x.lazy,f);}
    void push(u32 i){auto& x=nodes[i];if(empty(x.lazy))return;if(!x.child[0]){materialize(i);return;}auto f=x.lazy;apply_node(x.child[0],f);apply_node(x.child[1],f);x.lazy=identity();}
    void set(u32 id,u32 value){auto p=positions[id];std::array<u32,32> path{};u32 length=0;for(u32 i=p.node;i;i=nodes[i].parent)path[length++]=i;while(length)push(path[--length]);auto& node=nodes[p.node];node.sum=R::add(R::subtract(node.sum,values[p.at]),value);node.count+=!active[p.at];active[p.at]=~0u;values[p.at]=value;for(u32 i=node.parent;i;i=nodes[i].parent)pull(i);}
    void update_leaf(u32 i,const Bounds& q,Function f){auto& node=nodes[i];auto old=node.lazy,next=compose(old,f);auto a=_mm256_set1_epi32(old.a),b=_mm256_set1_epi32(R::decode(old.b)),na=_mm256_set1_epi32(next.a),nb=_mm256_set1_epi32(R::decode(next.b)),sum=_mm256_setzero_si256();
        // Blend the old/new maps before transforming: one pass materializes both regions.
        for(u32 j=node.begin;j<node.end;j+=8){auto mask=select(j,node.end,q),live=_mm256_and_si256(_mm256_loadu_si256((const __m256i*)(active.p+j)),tail(node.end-j));auto v=_mm256_loadu_si256((const __m256i*)(values.p+j));v=M::add(M::mont(v,_mm256_blendv_epi8(a,na,mask)),_mm256_blendv_epi8(b,nb,mask));v=_mm256_and_si256(v,live);_mm256_maskstore_epi32((int*)(values.p+j),tail(node.end-j),v);accumulate(sum,v);}node.sum=total(sum)%P;node.lazy=identity();
    }
    u32 query_leaf(u32 i,const Bounds& q,Function carry)const{const auto& x=nodes[i];auto sum=_mm256_setzero_si256();u32 count=0;
        for(u32 j=x.begin;j<x.end;j+=8){auto mask=select(j,x.end,q);count+=std::popcount(u32(_mm256_movemask_ps(_mm256_castsi256_ps(mask))));accumulate(sum,_mm256_and_si256(_mm256_loadu_si256((const __m256i*)(values.p+j)),mask));}return mapped(total(sum)%P,count,compose(x.lazy,carry));
    }
    void update(u32 i,const Bounds& q,Function f){if(!i||!nodes[i].count||outside(nodes[i],q))return;if(covered(nodes[i],q)){apply_node(i,f);return;}if(!nodes[i].child[0]){update_leaf(i,q,f);return;}push(i);update(nodes[i].child[0],q,f);update(nodes[i].child[1],q,f);pull(i);}
    u32 query(u32 i,const Bounds& q,Function carry)const{if(!i||!nodes[i].count||outside(nodes[i],q))return 0;const auto& x=nodes[i];if(covered(x,q))return mapped(x.sum,x.count,carry);if(!x.child[0])return query_leaf(i,q,carry);auto next=compose(x.lazy,carry);return R::add(query(x.child[0],q,next),query(x.child[1],q,next));}
    void apply(u32 l,u32 d,u32 r,u32 u,Function f){if(l<r&&d<u&&(f.a!=1||f.b))update(root,bounds(l,d,r,u),{R::encode(f.a),R::encode(f.b)});}
    u32 sum(u32 l,u32 d,u32 r,u32 u)const{if(l==r||d==u)return 0;u32 value=query(root,bounds(l,d,r,u),identity());return std::min(value,value-P);}
};
}
