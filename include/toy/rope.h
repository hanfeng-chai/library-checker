#pragma once
#include <toy/affine.h>
#include <toy/buffer.h>
#include <toy/montgomery.h>
namespace toy {
template<u32 P=0,u32 B=512> struct Rope {
    static constexpr u32 minimum=B/8,tolerance=4,path_capacity=161;
    static_assert(B>=16&&B%8==0);
    using Sum=std::conditional_t<P==0,u64,u32>;using R=Montgomery<P>;
    struct Empty{};using Tag=std::conditional_t<P==0,Empty,Affine<P>>;
    struct Node{u32 left=0,right=0,size=0,block=0;Sum sum=0;[[no_unique_address]]Tag lazy{};u16 front=0,back=0;u8 height=0,rev=0;};
    Buffer<Node> nodes;Buffer<u32> storage;u32 root=0,free_node=0,free_block=0,blocks=1;
    static Sum plus(Sum a,Sum b){if constexpr(P)return R::add(a,b);else return a+b;}
    static Sum minus(Sum a,Sum b){if constexpr(P)return R::subtract(a,b);else return a-b;}
    static Sum norm(u64 x){if constexpr(P)return x%P;else return x;}
    static Tag identity(){if constexpr(P)return {R::one,0};else return {};}
    static bool empty(Tag f){if constexpr(P)return f.a==R::one&&!f.b;else return true;}
    static Tag compose(Tag f,Tag g){if constexpr(P)return {R::multiply(f.a,g.a),R::add(R::multiply(f.b,g.a),g.b)};else return {};}
    static Sum mapped(Sum value,u32 count,Tag f){if constexpr(P){u64 z=u64(value)*f.a+u64(count)*f.b;u32 x=(z+u64(u32(z)*Mod<P>::inverse)*P)>>32;return std::min(x,x-2*P);}else return value;}
    // Widen u32 lanes before adding; two independent accumulators overlap loads.
    static u64 raw_sum(const u32* p,u32 n){auto a=_mm256_setzero_si256(),b=a,z=a;u32 i=0;
        for(;i+8<=n;i+=8){auto x=_mm256_loadu_si256((const __m256i*)(p+i));a=_mm256_add_epi64(a,_mm256_unpacklo_epi32(x,z));b=_mm256_add_epi64(b,_mm256_unpackhi_epi32(x,z));}
        a=_mm256_add_epi64(a,b);auto v=_mm_add_epi64(_mm256_castsi256_si128(a),_mm256_extracti128_si256(a,1));u64 s=_mm_cvtsi128_si64(_mm_add_epi64(v,_mm_srli_si128(v,8)));for(;i<n;++i)s+=p[i];return s;}
    u32* data(u32 i){return storage.p+usize(nodes[i].block)*B;}const u32* data(u32 i)const{return storage.p+usize(nodes[i].block)*B;}
    u32 allocate(){u32 i;if(free_node){i=free_node;free_node=nodes[i].left;}else{if(nodes.n==nodes.capacity)nodes.reserve(std::max<usize>(4,2*nodes.capacity));i=nodes.n++;}nodes[i]={};nodes[i].lazy=identity();return i;}
    void release(u32 i){nodes[i].left=free_node;free_node=i;}
    u32 leaf(){u32 i=allocate(),b;if(free_block){b=free_block;free_block=storage[usize(b)*B];}else{b=blocks++;usize needed=usize(blocks)*B;if(needed>storage.capacity)storage.reserve(std::max(needed,2*storage.capacity));storage.n=needed;}nodes[i].block=b;nodes[i].height=1;return i;}
    void release_leaf(u32 i){u32 b=nodes[i].block;storage[usize(b)*B]=free_block;free_block=b;release(i);}
    void repair_leaf(u32 i,Sum sum){auto& x=nodes[i];x.sum=sum;x.front=x.back=std::min(x.size,minimum);}
    void pull(u32 i){auto& x=nodes[i];const auto& a=nodes[x.left];const auto& b=nodes[x.right];x.size=a.size+b.size;x.sum=plus(a.sum,b.sum);x.front=a.front;x.back=b.back;x.height=1+std::max(a.height,b.height);}
    u32 branch(u32 a,u32 b,u32 spare=0){u32 i=spare?spare:allocate();nodes[i].left=a;nodes[i].right=b;nodes[i].block=nodes[i].rev=0;nodes[i].lazy=identity();pull(i);return i;}
    void flip(u32 i){if(i){auto& x=nodes[i];if(!x.block)std::swap(x.left,x.right);std::swap(x.front,x.back);x.rev^=1;}}
    void transform(u32 i,Tag f){if constexpr(P){auto& x=nodes[i];x.sum=mapped(x.sum,x.size,f);x.lazy=compose(x.lazy,f);}}
    void push(u32 i){auto& x=nodes[i];if(x.rev){flip(x.left);flip(x.right);x.rev=0;}if constexpr(P)if(!empty(x.lazy)){transform(x.left,x.lazy);transform(x.right,x.lazy);x.lazy=identity();}}
    // Values are ordinary residues; encoded multipliers keep the result ordinary.
    static void map_values(u32* p,u32 n,Tag f) requires(P!=0){auto a=_mm256_set1_epi32(f.a),b=_mm256_set1_epi32(R::multiply(f.b,1));u32 i=0;
        for(;i+8<=n;i+=8){auto* q=(__m256i*)(p+i);_mm256_storeu_si256(q,R::add(R::multiply(_mm256_loadu_si256(q),a),b));}for(;i<n;++i)p[i]=mapped(p[i],1,f);}
    void materialize_values(u32 i){auto& x=nodes[i];if constexpr(P)if(!empty(x.lazy)){map_values(data(i),x.size,x.lazy);x.lazy=identity();}}
    void materialize(u32 i){materialize_values(i);auto& x=nodes[i];if(x.rev){std::reverse(data(i),data(i)+x.size);x.rev=0;}}
    u32 rotate_right(u32 i){u32 a=nodes[i].left;push(a);nodes[i].left=nodes[a].right;nodes[a].right=i;pull(i);pull(a);return a;}
    u32 rotate_left(u32 i){u32 b=nodes[i].right;push(b);nodes[i].right=nodes[b].left;nodes[b].left=i;pull(i);pull(b);return b;}
    template<bool Arbitrary=false> u32 balance(u32 i){u32 a=nodes[i].left,b=nodes[i].right;
        if constexpr(Arbitrary)if(nodes[a].height>nodes[b].height+tolerance+1||nodes[b].height>nodes[a].height+tolerance+1)return join(a,b,i);
        if(nodes[a].height>nodes[b].height+tolerance){push(a);if(nodes[nodes[a].left].height<nodes[nodes[a].right].height)nodes[i].left=rotate_left(a);return rotate_right(i);}
        if(nodes[b].height>nodes[a].height+tolerance){push(b);if(nodes[nodes[b].right].height<nodes[nodes[b].left].height)nodes[i].right=rotate_right(b);return rotate_left(i);}pull(i);return i;}
    [[gnu::noinline]] u32 join_higher(u32 a,u32 b,u32 spare){if(nodes[a].height>nodes[b].height+tolerance){push(a);u32 x=join(nodes[a].right,b,spare);nodes[a].right=x;return balance(a);}push(b);u32 x=join(a,nodes[b].left,spare);nodes[b].left=x;return balance(b);}
    [[gnu::always_inline]] u32 join(u32 a,u32 b,u32 spare=0){if(!a||!b){if(spare)release(spare);return a|b;}if(nodes[a].height>nodes[b].height+tolerance||nodes[b].height>nodes[a].height+tolerance)return join_higher(a,b,spare);return branch(a,b,spare);}
    std::pair<u32,u32> split_leaf(u32 i,u32 at){if(!at)return {0,i};if(at==nodes[i].size)return {i,0};materialize(i);u32 old=nodes[i].size,j=leaf();Sum first=norm(raw_sum(data(i),at)),total=nodes[i].sum;
        memcpy(data(j),data(i)+at,4*(old-at));nodes[i].size=at;nodes[j].size=old-at;repair_leaf(i,first);repair_leaf(j,minus(total,first));return {i,j};}
    struct Split{u32 first,second,spare;};
    Split from_leaf(u32 leaf,u32 first,u32 second,std::span<const u32> path){u32 spare=0,child=leaf;
        for(usize k=path.size();k--;){u32 parent=path[k],a=nodes[parent].left,b=nodes[parent].right;
            if(child==a){if(!first){pull(parent);second=parent;}else if(!second){second=b;spare=parent;}else second=join(second,b,parent);}
            else{if(!second){pull(parent);first=parent;}else if(!first){first=a;spare=parent;}else first=join(a,first,parent);}child=parent;}return {first,second,spare};}
    u32 concat(u32 a,u32 b){if(!a||!b)return a|b;if(nodes[a].back>=minimum&&nodes[b].front>=minimum)return join(a,b);
        std::array<u32,path_capacity> ap,bp;u32 an=0,bn=0,x=a,y=b;
        while(!nodes[x].block){push(x);ap[an++]=x;x=nodes[x].right;}while(!nodes[y].block){push(y);bp[bn++]=y;y=nodes[y].left;}
        materialize(x);materialize(y);u32 nx=nodes[x].size,ny=nodes[y].size,total=nx+ny;Sum sum=plus(nodes[x].sum,nodes[y].sum);
        if(total<=B){memcpy(data(x)+nx,data(y),4*ny);nodes[x].size=total;repair_leaf(x,sum);release_leaf(y);
            if(!bn)b=0;else{u32 parent=bp[--bn];b=nodes[parent].right;release(parent);while(bn){u32 p=bp[--bn];nodes[p].left=b;b=balance(p);}}
            while(an)pull(ap[--an]);return concat(a,b);}
        u32 target=total/2;
        if(nx<target){u32 moved=target-nx;memcpy(data(x)+nx,data(y),4*moved);memmove(data(y),data(y)+moved,4*(ny-moved));}
        else if(nx>target){u32 moved=nx-target;memmove(data(y)+moved,data(y),4*ny);memcpy(data(y),data(x)+target,4*moved);}
        nodes[x].size=target;nodes[y].size=total-target;Sum first=norm(raw_sum(data(x),target));repair_leaf(x,first);repair_leaf(y,minus(sum,first));while(an)pull(ap[--an]);while(bn)pull(bp[--bn]);return join(a,b);
    }
    u32 build(std::span<const u32> values,u32 count){if(count==1){u32 i=leaf();nodes[i].size=values.size();memcpy(data(i),values.data(),values.size_bytes());repair_leaf(i,norm(raw_sum(data(i),values.size())));return i;}
        u32 half=count/2,cut=u64(values.size())*half/count;u32 a=build(values.first(cut),half),b=build(values.subspan(cut),count-half);return branch(a,b);}
    explicit Rope(std::span<const u32> values,usize extra=0):nodes(1,std::max<usize>(4,2*(values.size()+extra)/minimum+8)),storage(B,std::max<usize>(4,(values.size()+extra)/minimum+4)*B){nodes[0]={};if(!values.empty())root=build(values,(values.size()+B-1)/B);}
    u32 size()const{return nodes[root].size;}
    u32 insert_at(u32 i,u32 at,u32 value){if(!i){i=leaf();nodes[i].size=1;data(i)[0]=value;repair_leaf(i,value);return i;}
        if(nodes[i].block){materialize_values(i);if(nodes[i].size==B){auto[a,b]=split_leaf(i,B/2);if(at<=B/2)a=insert_at(a,at,value);else b=insert_at(b,at-B/2,value);return branch(a,b);}
            if(nodes[i].rev)at=nodes[i].size-at;u32* p=data(i);memmove(p+at+1,p+at,4*(nodes[i].size-at));p[at]=value;++nodes[i].size;repair_leaf(i,plus(nodes[i].sum,value));return i;}
        push(i);u32 a=nodes[i].left,left=nodes[a].size;if(at<left){u32 x=insert_at(a,at,value);nodes[i].left=x;}else{u32 x=insert_at(nodes[i].right,at-left,value);nodes[i].right=x;}return balance(i);}
    void insert(u32 at,u32 value){root=insert_at(root,at,value);}
    u32 erase_at(u32 i,u32 at){if(nodes[i].block){if(nodes[i].rev)at=nodes[i].size-1-at;u32* p=data(i);Sum sum=minus(nodes[i].sum,mapped(p[at],1,nodes[i].lazy));memmove(p+at,p+at+1,4*(nodes[i].size-at-1));if(!--nodes[i].size){release_leaf(i);return 0;}repair_leaf(i,sum);return i;}
        push(i);u32 left=nodes[nodes[i].left].size;if(at<left)nodes[i].left=erase_at(nodes[i].left,at);else nodes[i].right=erase_at(nodes[i].right,at-left);
        u32 a=nodes[i].left,b=nodes[i].right;if(!a||!b){release(i);return a|b;}if(nodes[a].back<minimum||nodes[b].front<minimum){release(i);return concat(a,b);}return balance(i);}
    void erase(u32 at){root=erase_at(root,at);}
    [[gnu::noinline]] u32 reverse_blocks(u32 x,u32 l,u32 r){u32 a=nodes[x].left,b=nodes[x].right,left=nodes[a].size,u=a,v=b,i=l,j=r-left-1,an=0,bn=0;std::array<u32,path_capacity> ap,bp;
        auto descend=[&](u32& at,u32& position,auto& path,u32& count){push(at);path[count++]=at;u32 left=nodes[at].left,right=nodes[at].right,n=nodes[left].size;bool go=position>=n;at=go?right:left;position-=go?n:0;};
        while(!nodes[u].block&&!nodes[v].block){descend(u,i,ap,an);descend(v,j,bp,bn);}while(!nodes[u].block)descend(u,i,ap,an);while(!nodes[v].block)descend(v,j,bp,bn);++j;
        u32 nu=nodes[u].size,nv=nodes[v].size,newu=nv-j+nu-i,newv=j+i;
        if(newu<minimum||newv<minimum||newu>B||newv>B){auto[ua,ub]=split_leaf(u,i);auto[va,vb]=split_leaf(v,j);auto first=from_leaf(u,ua,ub,std::span(ap.data(),an)),second=from_leaf(v,va,vb,std::span(bp.data(),bn));if(first.spare)release(first.spare);if(second.spare)release(second.spare);flip(first.second);flip(second.first);u32 middle=join(second.first,first.second,x);return concat(concat(first.first,middle),second.second);}
        // Swap the OUTSIDE pieces in reverse order, then flip whole blocks. This
        // preserves the interval without creating two extra boundary leaves.
        if(i||j!=nv){materialize_values(u);materialize_values(v);alignas(32)u32 saved[B];bool ur=nodes[u].rev,vr=nodes[v].rev;
            auto prefix=[&](u32 id,u32 cut){u32 n=nodes[id].size;bool rev=nodes[id].rev;if(cut<=n-cut)return norm(raw_sum(data(id)+(rev?n-cut:0),cut));return minus(nodes[id].sum,norm(raw_sum(data(id)+(rev?0:cut),n-cut)));};
            Sum a0=prefix(u,i),b1=minus(nodes[v].sum,prefix(v,j)),su=plus(minus(nodes[u].sum,a0),b1),sv=plus(minus(nodes[v].sum,b1),a0);
            // Equal physical orientations require reversing the transferred fragment.
            auto copy=[&](const u32* from,u32 count,u32* to){if(ur==vr)std::reverse_copy(from,from+count,to);else memcpy(to,from,4*count);};
            u32 inside=nu-i,tail=nv-j;copy(data(u)+(ur?inside:0),i,saved);
            if(!ur&&tail!=i)memmove(data(u)+tail,data(u)+i,4*inside);
            copy(data(v)+(vr?0:j),tail,data(u)+(ur?inside:0));
            if(vr&&tail!=i)memmove(data(v)+i,data(v)+tail,4*j);
            memcpy(data(v)+(vr?0:j),saved,4*i);
            nodes[u].size=newu;nodes[v].size=newv;repair_leaf(u,su);repair_leaf(v,sv);}
        auto first=from_leaf(u,0,u,std::span(ap.data(),an)),second=from_leaf(v,v,0,std::span(bp.data(),bn));flip(first.second);flip(second.first);u32 middle=join(second.first,first.second,x);return join(join(first.first,middle,first.spare),second.second,second.spare);
    }
    u32 reverse_at(u32 i,u32 l,u32 r){if(!l&&r==nodes[i].size){flip(i);return i;}if(nodes[i].block){if(nodes[i].rev){u32 first=nodes[i].size-r;r=nodes[i].size-l;l=first;}std::reverse(data(i)+l,data(i)+r);return i;}push(i);u32 left=nodes[nodes[i].left].size;
        if(r>left&&l<left)return reverse_blocks(i,l,r);
        if(r<=left){u32 x=reverse_at(nodes[i].left,l,r);nodes[i].left=x;}else{u32 x=reverse_at(nodes[i].right,l-left,r-left);nodes[i].right=x;}
        u32 a=nodes[i].left,b=nodes[i].right;if(nodes[a].back<minimum||nodes[b].front<minimum){release(i);return concat(a,b);}return balance<true>(i);}
    void reverse(u32 l,u32 r){if(r-l>1)root=reverse_at(root,l,r);}
    void update(u32 i,u32 l,u32 r,Tag f) requires(P!=0){if(!l&&r==nodes[i].size){transform(i,f);return;}
        if(nodes[i].block){materialize(i);u32 old=norm(raw_sum(data(i)+l,r-l)),next=mapped(old,r-l,f);map_values(data(i)+l,r-l,f);nodes[i].sum=plus(minus(nodes[i].sum,old),next);return;}
        push(i);u32 left=nodes[nodes[i].left].size;if(l<left)update(nodes[i].left,l,std::min(r,left),f);if(r>left)update(nodes[i].right,l>left?l-left:0,r-left,f);pull(i);}
    void apply(u32 l,u32 r,Affine<P> f) requires(P!=0){if(l<r)update(root,l,r,{R::encode(f.a),R::encode(f.b)});}
    Sum fold(u32 i,u32 l,u32 r,Tag outer,bool reversed)const{const auto& x=nodes[i];if(!l&&r==x.size)return mapped(x.sum,x.size,outer);outer=compose(x.lazy,outer);
        if(x.block){if(reversed!=bool(x.rev)){u32 first=x.size-r;r=x.size-l;l=first;}return mapped(norm(raw_sum(data(i)+l,r-l)),r-l,outer);}
        u32 a=reversed?x.right:x.left,b=reversed?x.left:x.right,left=nodes[a].size;reversed^=bool(x.rev);
        if(r<=left)return fold(a,l,r,outer,reversed);if(l>=left)return fold(b,l-left,r-left,outer,reversed);return plus(fold(a,l,left,outer,reversed),fold(b,0,r-left,outer,reversed));}
    Sum sum(u32 l,u32 r)const{if(l==r)return 0;Sum x=fold(root,l,r,identity(),false);if constexpr(P)return std::min(x,x-P);else return x;}
};
}
