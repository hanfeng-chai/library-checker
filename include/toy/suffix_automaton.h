#pragma once
#include <toy/alphabet_trie.h>
namespace toy {
struct SuffixAutomaton {
    struct Node{u32 length,link,end;};AlphabetTrie trie;Buffer<Node>node;u32 last=0;u64 distinct=0;
    explicit SuffixAutomaton(std::string_view s):trie(2*s.size()+1),node(2*s.size()+1){trie.node();node[0]={0,~0u,0};for(u32 i=0;i<s.size();++i)extend(s[i]-'a',i);}
    void extend(u32 c,u32 at){u32 current=trie.node(),p=last;node[current]={node[last].length+1,0,at};
        while(p!=~0u&&!trie.get(p,c)){trie.set(p,c,current);p=node[p].link;}
        if(p!=~0u){u32 q=trie.get(p,c);if(node[p].length+1==node[q].length)node[current].link=q;
            else{u32 copy=trie.clone(q);node[copy]=node[q];node[copy].length=node[p].length+1;while(p!=~0u&&trie.get(p,c)==q){trie.set(p,c,copy);p=node[p].link;}node[q].link=node[current].link=copy;}
        }
        last=current;distinct+=node[current].length-node[node[current].link].length;
    }
    std::array<u32,4> longest_common_substring(std::string_view t)const{u32 v=0,length=0,best=0,a=0,b=0;
        for(u32 i=0;i<t.size();++i){u32 c=t[i]-'a',next=trie.get(v,c);while(v&&!next){v=node[v].link;length=node[v].length;next=trie.get(v,c);}if(next){v=next;++length;}else length=0;if(length>best){best=length;a=node[v].end+1;b=i+1;}}
        return {a-best,a,b-best,b};
    }
};
}
