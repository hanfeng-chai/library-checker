#pragma once
#include <toy/buffer.h>
#include <toy/line.h>
namespace toy {
// Known sorted query coordinates; line evaluations must fit i64 and be < infinity.
struct LiChaoTree {
    u32 n, capacity, height;
    Buffer<i32> coordinates;
    Buffer<Line> lines;
    Buffer<i64> leaves;
    explicit LiChaoTree(span<const i32> xs) : n(xs.size()), capacity(bit_ceil(max(n,1u))), height(countr_zero(capacity)), coordinates(capacity+1), lines(capacity), leaves(capacity) {
        fill(coordinates.p,coordinates.p+coordinates.n,xs.empty()?0:xs.back());
        if(n)memcpy(coordinates.p,xs.data(),xs.size_bytes());
        fill(lines.p,lines.p+lines.n,Line{});fill(leaves.p,leaves.p+leaves.n,Line::infinity);
    }
    void add(int node, int level, Line line) {
        int left = (node << level) ^ capacity;
        int right = left + (1 << level);
        i64 line_left = line(coordinates[left]);
        i64 line_right = line(coordinates[right]);

        while (true) {
            if (left + 1 == right) {
                leaves[left] =
                    std::min(leaves[left], line_left);
                return;
            }
            Line& current = lines[node];
            i64 current_left = current(coordinates[left]);
            i64 current_right = current(coordinates[right]);
            if (line_left < current_left) {
                if (line_right < current_right) {
                    std::swap(line, current);
                    return;
                }
                int middle = (left + right) / 2;
                i64 line_middle = line(coordinates[middle]);
                i64 current_middle = current(coordinates[middle]);
                if (line_middle < current_middle) {
                    std::swap(line, current);
                    node = node * 2 + 1;
                    left = middle;
                    line_left = current_middle;
                    line_right = current_right;
                } else {
                    node *= 2;
                    right = middle;
                    line_right = line_middle;
                }
            } else {
                if (line_right < current_right) {
                    int middle = (left + right) / 2;
                    i64 line_middle = line(coordinates[middle]);
                    i64 current_middle = current(coordinates[middle]);
                    if (line_middle < current_middle) {
                        std::swap(line, current);
                        node *= 2;
                        right = middle;
                        line_left = current_left;
                        line_right = current_middle;
                    } else {
                        node = node * 2 + 1;
                        left = middle;
                        line_left = line_middle;
                    }
                } else {
                    return;
                }
            }
        }
    }
    void add(Line line){if(n)add(1,height,line);}
    void add_segment(u32 l,u32 r,Line line){
        if(l==r)return;u32 a=l+capacity-1,b=r+capacity,width=bit_width(a^b)-1,mask=(1u<<width)-1;
        for(u32 bits=~a&mask;bits;bits&=bits-1){unsigned k=countr_zero(bits);add((a>>k)^1,k,line);}
        for(u32 bits=b&mask;bits;bits&=bits-1){unsigned k=countr_zero(bits);add((b>>k)^1,k,line);}
    }
    i64 minimum(u32 index)const{
        i64 result=leaves[index];i32 x=coordinates[index];
        for(u32 node=capacity+index;node>>=1;)result=min(result,lines[node](x));return result;
    }
};
}
