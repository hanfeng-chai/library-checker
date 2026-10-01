#include <toy/io.h>
#include <toy/eertree.h>
using namespace toy;
int main(){Reader in;Writer out;auto s=in.token();Eertree tree(s.size());Buffer<u32>suffix(s.size());for(u32 i=0;i<s.size();++i)suffix[i]=tree.extend(s,i)-1;out.write(tree.size());for(u32 v=2;v<tree.trie.row.n;++v){out.write(i32(tree.node[v].parent)-1,' ');out.write(tree.node[v].suffix-1);}out.write(std::span(suffix.p,suffix.n),' ');out.put('\n');}
