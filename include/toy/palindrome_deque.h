#pragma once
#include <toy/alphabet_trie.h>
namespace toy {
// Surface representation of a deque eertree. Nodes are retained for reuse;
// occurrence and active suffix-child counts determine which are currently live.
struct PalindromeDeque {
    struct Node{i32 length;u32 parent,suffix,quick;i32 count=0,children=0;bool active=false;};
    struct Cell{u32 character,prefix,suffix;};
    AlphabetTrie trie;Buffer<Node>node;Buffer<Cell>text;u32 left,right,distinct=0;
    explicit PalindromeDeque(u32 q):trie(q+2),node(q+2),text(2*q+3),left(q+1),right(q+1){trie.node();trie.node();node[0]={-1,0,0,0,0,0,true};node[1]={0,0,0,0,0,0,true};}
    template<bool Back>u32 find(u32 v,u32 c){u32 size=right-left;
        auto fits=[&](u32 x){i32 length=node[x].length;return length<0||(u32(length)<size&&text[Back?right-length-1:left+length].character==c);};
        while(!fits(v)){u32 next=node[v].suffix;if(fits(next))return next;v=node[v].quick;}return v;
    }
    template<bool Back>void push(u32 c){u32 parent=left==right?0:find<Back>(Back?text[right-1].suffix:text[left].prefix,c),v=trie.get(parent,c);bool created=!v;
        if(created){v=trie.node();u32 suffix=parent?trie.get(find<Back>(node[parent].suffix,c),c):1;node[v]={node[parent].length+2,parent,suffix,0};trie.set(parent,c,v);}
        if constexpr(Back)text[right++]={c,1,1};else text[--left]={c,1,1};u32 w=node[v].suffix;
        if(created){u32 link=node[w].suffix;if(link){u32 a=Back?right-node[w].length-1:left+node[w].length,b=Back?right-node[link].length-1:left+node[link].length;node[v].quick=text[a].character==text[b].character?node[w].quick:link;}}
        if(!node[v].active){node[v].active=true;++distinct;++node[w].children;}++node[v].count;
        if constexpr(Back){text[right-1].suffix=v;text[right-node[v].length].prefix=v;if(node[w].length>0&&text[right-node[v].length+node[w].length-1].suffix==w)text[right-node[v].length+node[w].length-1].suffix=1;}
        else{text[left].prefix=v;text[left+node[v].length-1].suffix=v;if(node[w].length>0&&text[left+node[v].length-node[w].length].prefix==w)text[left+node[v].length-node[w].length].prefix=1;}
    }
    template<bool Back>void pop(){u32 v=Back?text[right-1].suffix:text[left].prefix,w=node[v].suffix;
        if constexpr(Back){if(node[v].length>=2&&node[text[right-node[v].length+node[w].length-1].suffix].length<node[w].length){text[right-node[v].length+node[w].length-1].suffix=w;text[right-node[v].length].prefix=w;}else text[right-node[v].length].prefix=1;--right;}
        else{if(node[v].length>=2&&node[text[left+node[v].length-node[w].length].prefix].length<node[w].length){text[left+node[v].length-node[w].length].prefix=w;text[left+node[v].length-1].suffix=w;}else text[left+node[v].length-1].suffix=1;++left;}
        if(!--node[v].count&&!node[v].children){node[v].active=false;--distinct;--node[w].children;}
    }
    u32 prefix()const{return left==right?0:node[text[left].prefix].length;}
    u32 suffix()const{return left==right?0:node[text[right-1].suffix].length;}
};
}
