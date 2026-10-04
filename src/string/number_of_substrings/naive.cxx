#include <toy/io.h>
#include <toy/suffix_automaton.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    SuffixAutomaton index(in.token());
    out.write(index.distinct);
}
