#include <toy/deque.h>
#include <toy/io.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 q = in.read<u32, 6>();
    Deque<u32> queue(q);
    while (q--) {
        u32 type = in.read<u32, 1>();
        if (type < 2) {
            u32 x = in.read<u32, 10>();
            if (!type)
                queue.push_front(x);
            else
                queue.push_back(x);
        } else if (type == 2)
            queue.pop_front();
        else if (type == 3)
            queue.pop_back();
        else
            out.write(queue[in.read<u32, 6>()]);
    }
}
