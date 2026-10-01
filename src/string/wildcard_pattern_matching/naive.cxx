#include <toy/io.h>
#include <toy/wildcard_match.h>
using namespace toy;
int main(){Reader in;Writer out;auto s=in.token(),t=in.token();auto answer=wildcard_match<false>(s,t);out.append({answer.p,answer.n});out.put('\n');}
