#include <toy/io.h>
#include <toy/aho_corasick.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,7>();AhoCorasick ac(1000001);Buffer<u32>terminal(n);for(u32 i=0;i<n;++i)terminal[i]=ac.add(in.token());ac.build();out.write(ac.size());for(u32 v=1;v<ac.size();++v){out.write(ac.parent[v],' ');out.write(ac.suffix[v]);}out.write(std::span(terminal.p,terminal.n),' ');out.put('\n');}
