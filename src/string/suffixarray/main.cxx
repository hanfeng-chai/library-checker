#include <toy/io_batch.h>
#include <toy/suffix_array.h>
using namespace toy;
int main(){Reader in;Writer out;auto answer=suffix_array(in.token());write_bulk6(out,std::span(answer.p,answer.n));out.put('\n');}
