#include <toy/io.h>
#include <toy/persistent_affine.h>
using namespace toy;
int main(){
    Reader in;Writer out;u32 n=in.read<u32,6>(),q=in.read<u32,6>();Buffer<u32>a(n),roots(q+1);for(auto& x:span(a.p,a.n))x=in.read<u32,16>();PersistentAffineArray tree{span<const u32>(a),2*n+64*usize(q)+1};roots[0]=tree.root;
    for(u32 i=0;i<q;++i){u32 type=in.read<u32,1>(),base=in.read<i32,6>()+1;if(type==0){auto[l,r]=in.read_pair<6>();u32 a=in.read<u32,16>(),b=in.read<u32,16>();roots[i+1]=tree.apply(roots[base],l,r,{a,b});}
        else if(type==1){u32 from=in.read<i32,6>()+1;auto[l,r]=in.read_pair<6>();roots[i+1]=tree.copy(roots[base],roots[from],l,r);}
        else{auto[l,r]=in.read_pair<6>();roots[i+1]=roots[base];out.write(tree.sum(roots[base],l,r));}}
}
