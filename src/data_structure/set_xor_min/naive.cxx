#include <toy/io.h>
#include <toy/patricia.h>
using namespace toy;
int main() {
    Reader in; Writer out; u32 q = in.read<u32, 6>(); PatriciaSet set(q);
    while(q--) { u32 type = in.read<u32,1>(), x = in.read<u32,10>(); if (!type) set.insert(x); else if(type==1)set.erase(x);else out.write(set.min_xor(x)); }
}
