#include <toy/io_batch.h>
#include <toy/string_basic.h>
using namespace toy;
int main(){Reader in;Writer out;auto answer=palindrome_lengths(in.token());write_bulk6(out,std::span(answer.p,answer.n));out.put('\n');}
