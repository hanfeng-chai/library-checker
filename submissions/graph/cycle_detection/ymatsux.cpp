#include <algorithm>
#include <functional>
#include <optional>

#pragma once

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ranges>
#include <vector>

using int64 = int64_t;
using uint64 = uint64_t;
using int32 = int32_t;
using uint32 = uint32_t;
using int16 = int16_t;
using uint16 = uint16_t;
using int8 = int8_t;
using uint8 = uint8_t;
using std::array;
using std::optional;
using std::vector;
using std::views::iota;

template <std::integral T>
constexpr auto closed_iota(T first, T last) {
  return std::views::iota(first, last + 1);
}

class FastStandardInputReader {
  private:
   static constexpr int kBufferSize = 1 << 17;
   array<char, kBufferSize> buffer_ {};
   FILE* stream_;
   char* buffer_begin_;
   char* buffer_end_;
   char* buffer_pointer_;
 
   template<int N = 0> void Read() {
     if (const auto k = buffer_end_ - buffer_pointer_; k <= N) {
       fread(std::copy_n(buffer_pointer_, k, buffer_begin_), 1, kBufferSize - k,
             stream_);
       buffer_pointer_ = buffer_begin_;
     }
   }
 
   void SkipSpaces() {
     if (buffer_pointer_ == buffer_end_) {
       Read<kBufferSize>();
     }
     while (*buffer_pointer_ <= ' ') {
       buffer_pointer_++;
       if (buffer_pointer_ == buffer_end_) {
         Read<kBufferSize>();
       }
     }
   }
 
  public:
   FastStandardInputReader()
       : stream_(stdin),
         buffer_begin_(buffer_.data()),
         buffer_end_(buffer_begin_ + kBufferSize),
         buffer_pointer_(buffer_end_) {
     Read<kBufferSize>();
   }
   ~FastStandardInputReader() {}
 
   int64 NextInt64() {
     SkipSpaces();
 
     Read<32>();
 
     bool is_negative = false;
     if (*buffer_pointer_ == '-') {
       is_negative = true;
       buffer_pointer_++;
     }
 
     uint64 result = 0;
     uint64 vs[2];
     memcpy(vs, buffer_pointer_, 16);
 
     uint64 v0 = vs[0];
     uint64 v1 = vs[1];
 
     constexpr auto is_all_digits = [](uint64 x) {
       return ((x ^ 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0) == 0;
     };
 
     if (is_all_digits(v0)) {
       v0 ^= 0x3030303030303030;
       v0 = (v0 * 10 + (v0 >> 8)) & 0x00ff00ff00ff00ff;
       v0 = (v0 * 100 + (v0 >> 16)) & 0x0000ffff0000ffff;
       v0 = (v0 * 10000 + (v0 >> 32)) & 0x00000000ffffffff;
       result = v0;
       buffer_pointer_ += 8;
       if (is_all_digits(v1)) {
         v1 ^= 0x3030303030303030;
         v1 = (v1 * 10 + (v1 >> 8)) & 0x00ff00ff00ff00ff;
         v1 = (v1 * 100 + (v1 >> 16)) & 0x0000ffff0000ffff;
         v1 = (v1 * 10000 + (v1 >> 32)) & 0x00000000ffffffff;
         result = result * 100000000 + v1;
         buffer_pointer_ += 8;
       }
     }
 
     while ('0' <= *buffer_pointer_) {
       result *= 10;
       result += *buffer_pointer_ - '0';
       ++buffer_pointer_;
     }
 
     // Skip a space character after the digits for efficiency.
     ++buffer_pointer_;
 
     if (is_negative) {
       result = -result;
     }
     return result;
   }
 
   int NextInt() {
     return static_cast<int>(NextInt64());
   }
 };
 
 class FastStandardOutputWriter {
  private:
   static constexpr int kBufferSize = 1 << 16;
   array<char, kBufferSize> buffer_ {};
   char* buffer_begin_;
   char* buffer_end_;
 
   // Strings of 0..999 with leading zeros.
   char table_[1000][4];
 
  public:
   FastStandardOutputWriter()
       : buffer_begin_(buffer_.data()),
         buffer_end_(buffer_begin_) {
     // Initialize lz_table_ and nlz_table_
     for (int i = 0; i < 1000; i++) {
       std::snprintf(table_[i], 4, "%03d", i);
     }
   }
   ~FastStandardOutputWriter() { Flush(); }
 
   void WriteChar(char c) {
     if (buffer_end_ - buffer_begin_ == kBufferSize) {
       fwrite(buffer_begin_, 1, kBufferSize, stdout);
       buffer_end_ = buffer_begin_;
     }
     *buffer_end_ = c;
     buffer_end_++;
   }
 
   void WriteInt64(const int64 x) {
     // Make sure that the buffer has 32 bytes of space to write the number.
     if (buffer_begin_ + kBufferSize - buffer_end_ < 32) {
       Flush();
     }
 
     if (x == 0) {
       WriteChar('0');
       return;
     }
 
     int64 v = x;
     if (v < 0) {
       WriteChar('-');
       // Note: This doesn't work correctly when value == INT64_MIN.
       v *= -1;
     }
 
     bool need_leading_zeros = false;
     static array<int64, 7> kBases = {
       static_cast<int64>(1E18), static_cast<int64>(1E15),
       static_cast<int64>(1E12), static_cast<int64>(1E9),
       static_cast<int64>(1E6),  static_cast<int64>(1E3),
       static_cast<int64>(1)};
     for (const int64 base : kBases) {
       if (need_leading_zeros) {
         const int64 c = v / base;
         memcpy(buffer_end_, table_[c], 3);
         buffer_end_ += 3;
         v -= c * base;
       } else {
         if (v < base) continue;
         const int64 c = v / base;
         if (c >= 100) {
           memcpy(buffer_end_, table_[c], 3);
           buffer_end_ += 3;
           need_leading_zeros = true;
         } else if (c >= 10) {
           memcpy(buffer_end_, table_[c] + 1, 2);
           buffer_end_ += 2;
           need_leading_zeros = true;
         } else {
           memcpy(buffer_end_, table_[c] + 2, 1);
           buffer_end_ += 1;
           need_leading_zeros = true;
         }
         v -= c * base;
       }
     }
   }

   void WriteInt32(const int32 x) {
     // Make sure that the buffer has 32 bytes of space to write the number.
     if (buffer_begin_ + kBufferSize - buffer_end_ < 32) {
       Flush();
     }

     if (x == 0) {
       WriteChar('0');
       return;
     }

     int64 v = x;
     if (v < 0) {
       WriteChar('-');
       // Note: This doesn't work correctly when value == INT32_MIN.
       v *= -1;
     }

     bool need_leading_zeros = false;
     static array<int64, 4> kBases = {
         static_cast<int64>(1E9), static_cast<int64>(1E6),
         static_cast<int64>(1E3), static_cast<int64>(1)};
     for (const int64 base : kBases) {
       if (need_leading_zeros) {
         const int64 c = v / base;
         memcpy(buffer_end_, table_[c], 3);
         buffer_end_ += 3;
         v -= c * base;
       } else {
         if (v < base) continue;
         const int64 c = v / base;
         if (c >= 100) {
           memcpy(buffer_end_, table_[c], 3);
           buffer_end_ += 3;
           need_leading_zeros = true;
         } else if (c >= 10) {
           memcpy(buffer_end_, table_[c] + 1, 2);
           buffer_end_ += 2;
           need_leading_zeros = true;
         } else {
           memcpy(buffer_end_, table_[c] + 2, 1);
           buffer_end_ += 1;
           need_leading_zeros = true;
         }
         v -= c * base;
       }
     }
   }

   void Flush() {
     fwrite(buffer_begin_, 1, buffer_end_ - buffer_begin_, stdout);
     buffer_end_ = buffer_begin_;
   }
 };
 
 FastStandardInputReader g_in;
 FastStandardOutputWriter g_out;


// https://judge.yosupo.jp/problem/cycle_detection
// 163ms

struct Edge {
  int s;
  int t;
  int id;
};

// Returns one directed cycle as a sequence of edge IDs, or std::nullopt if no cycle exists.
std::optional<std::vector<int>> DetectCycle(
    const std::vector<std::vector<Edge>>& graph) {
  const int n = static_cast<int>(graph.size());
  std::vector<int> color(n, 0);  // 0: unvisited, 1: in-stack, 2: finished
  std::vector<int> parent(n, -1);
  std::vector<int> parent_edge_id(n, -1);
  int back_edge_from = -1;
  int back_edge_to = -1;
  int back_edge_id = -1;

  std::function<bool(int)> dfs = [&](int v) -> bool {
    color[v] = 1;
    for (const Edge& e : graph[v]) {
      if (color[e.t] == 0) {
        parent[e.t] = v;
        parent_edge_id[e.t] = e.id;
        if (dfs(e.t)) return true;
      } else if (color[e.t] == 1) {
        back_edge_from = v;
        back_edge_to = e.t;
        back_edge_id = e.id;
        return true;
      }
    }
    color[v] = 2;
    return false;
  };

  for (int s = 0; s < n; s++) {
    if (color[s] == 0 && dfs(s)) break;
  }

  if (back_edge_to == -1) return std::nullopt;

  // Reconstruct vertex cycle [back_edge_to ... back_edge_from], then convert to edge ids.
  std::vector<int> verts;
  int x = back_edge_from;
  verts.push_back(x);
  while (x != back_edge_to) {
    x = parent[x];
    verts.push_back(x);
  }
  std::reverse(verts.begin(), verts.end());

  std::vector<int> path_edge_ids;
  for (size_t i = 0; i + 1 < verts.size(); ++i) {
    path_edge_ids.push_back(parent_edge_id[verts[i + 1]]);
  }
  // Close the cycle with the back edge.
  path_edge_ids.push_back(back_edge_id);
  return path_edge_ids;
}

template<typename T, bool directed, int MAXN, int MAXM>
struct cycle_detection {
    int n, m,cycle_len=0,ecnt=0;
    array<int, MAXN> head, parent_e, cycle_e; 
    array<int, directed?MAXM:2*MAXM> to, nxt, eid;
    array<uint8_t, MAXN>  vis;        
    array<T, MAXN> parent_v,cycle_v;   
    cycle_detection(int _n, int _m) : n(_n), m(_m) {
        head.fill(-1);
        vis .fill(0);
    }
    void add_edge(int u, int v, int id) {
        to [ecnt] = v;
        eid[ecnt] = id;
        nxt[ecnt] = head[u];
        head[u]   = ecnt++;
        if constexpr (!directed) {
            to [ecnt] = u;
            eid[ecnt] = id;
            nxt[ecnt] = head[v];
            head[v]   = ecnt++;
        }
    }
    bool find_cycle() {
        struct Frame { T v; int ei; };
        array<Frame, MAXN> stk;
        int top = 0;
        for (T start = 0; start < n; ++start) {
            if (vis[start]) continue;
            parent_v[start] = -1;
            parent_e[start] = -1;
            stk[top++] = {start, head[start]};
            while (top) {
                auto &fr = stk[top-1];
                T v = fr.v;
                if (vis[v] == 0) vis[v] = 1;
                int &ei = fr.ei;
                if (ei == -1) {
                    vis[v] = 2;
                    --top;
                    continue;
                }
                int ecur = ei; 
                ei = nxt[ei];
                T w = to[ecur];
                int id = eid[ecur];
                if constexpr (!directed) {
                    if (id == parent_e[v])  continue;  
                }
                if (vis[w] == 0) {
                    parent_v[w] = v;
                    parent_e[w] = id;
                    stk[top++]  = {w, head[w]};
                }
                else if (vis[w] == 1) {
                    int idx = 0;
                    cycle_v[idx] = w;
                    cycle_e[idx] = id;
                    ++idx;
                    T cur = v;
                    while (cur != w) {
                        cycle_v[idx] = cur;
                        cycle_e[idx] = parent_e[cur];
                        cur = parent_v[cur];
                        ++idx;
                    }
                    cycle_len = idx;
                    return true;
                }
            }
        }
        return false;
    }
    int length() const { return cycle_len; }
    const auto& vertices() const { return cycle_v; }
    const auto& edges()    const { return cycle_e; }
};
const int MAXN=500000+5;
const int MAXM=500000+5;

int main() {
  int n = g_in.NextInt();
  int m = g_in.NextInt();

  cycle_detection<int, true, MAXN, MAXM> cd(n, m);
  for (const int edge_id : iota(0, m)) {
    int s = g_in.NextInt();
    int t = g_in.NextInt();
    cd.add_edge(s, t, edge_id);
  }
  if (!cd.find_cycle()) {
    g_out.WriteInt64(-1);
    g_out.WriteChar('\n');
    return 0;
  }
  g_out.WriteInt64(cd.length());
  g_out.WriteChar('\n');
  const auto& es = cd.edges();
  for (int i = cd.length() - 1; i >= 0; i--) {
    g_out.WriteInt64(es[i]);
    g_out.WriteChar('\n');
  }

  // vector<vector<Edge>> graph(n);
  // for (const int edge_id : iota(0, m)) {
  //   int s = g_in.NextInt();
  //   int t = g_in.NextInt();
  //   graph[s].push_back({s, t, edge_id});
  // }

  // const optional<vector<int>> cycle = DetectCycle(graph);
  // if (cycle) {
  //   g_out.WriteInt64(cycle->size());
  //   g_out.WriteChar('\n');
  //   for (const int v : *cycle) {
  //     g_out.WriteInt64(v);
  //     g_out.WriteChar('\n');
  //   }
  // } else {
  //   g_out.WriteInt64(-1);
  //   g_out.WriteChar('\n');
  // }
  // g_out.Flush();
}