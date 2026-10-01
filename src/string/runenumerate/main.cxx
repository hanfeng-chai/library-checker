#include <toy/io_batch.h>
#include <toy/string_runs.h>
using namespace toy;
int main(){Reader in;Writer out;auto runs=string_runs(in.token());out.write(runs.n);write_bulk6(out,std::span((u32*)runs.p,3*runs.n));out.put('\n');}
