#pragma once
#include <toy/alphabet_trie.h>
namespace toy {
struct Eertree {
    struct Node{i32 length;u32 parent,suffix,series;};
    AlphabetTrie trie;Buffer<Node>node;u32 last=1;
    explicit Eertree(u32 capacity):trie(capacity+2),node(capacity+2){trie.node();trie.node();node[0]={-1,0,0,0};node[1]={0,0,0,0};}
    u32 extend(std::string_view s,u32 at){u32 c=s[at]-'a';auto find=[&](u32 v){while(i32(at)-1-node[v].length<0||s[at-1-node[v].length]!=s[at])v=node[v].suffix;return v;};
        u32 parent=find(last),next=trie.get(parent,c);if(!next){next=trie.node();u32 suffix=parent?trie.get(find(node[parent].suffix),c):1;node[next]={node[parent].length+2,parent,suffix,suffix};
            if(node[next].length-node[suffix].length==node[suffix].length-node[node[suffix].suffix].length)node[next].series=node[suffix].series;trie.set(parent,c,next);
        }return last=next;
    }
    u32 size()const{return trie.row.n-2;}
};
}
