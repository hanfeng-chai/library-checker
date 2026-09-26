// ===== Required external/system includes =====
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <concepts>
#include <numeric>
#include <span>
#include <string>
#include <vector>
#include <cstdint>
#include <limits>

// ===== PS HEADER: debug.hpp =====
namespace akrbt {
namespace internal {
inline void Assert(
    const char* msg,
    const char* expr,
    const char* file,
    int line,
    const char* func) {
  std::fprintf(
      stderr,
      "assertion failed:\n"
      "  message   : %s\n"
      "  expression: %s\n"
      "  location  : %s:%d\n"
      "  function  : %s\n",
      msg ? msg : "(null)",
      expr ? expr : "(unknown)",
      file ? file : "(unknown)",
      line,
      func ? func : "(unknown)");

  std::abort();
}
}
#define AKRBT_FILE_NAME __FILE_NAME__

#define AKRBT_FUNC_NAME __PRETTY_FUNCTION__

#define AKRBT_ASSERT(EXPR, MSG)                                        \
  (void)((!!(EXPR)) ||                                                 \
         (::akrbt::internal::Assert(                                   \
              MSG, #EXPR, AKRBT_FILE_NAME, __LINE__, AKRBT_FUNC_NAME), \
          0))


// ===== PS HEADER: types.hpp =====
using i8 = std::int8_t;
using i32 = std::int32_t;
using u64 = std::uint64_t;

// ===== PS HEADER: strings.hpp =====
class SuffixArray {
 public:
  template <typename T>
    requires std::is_integral_v<T>
  void Init(const std::vector<T>& input) {
    Init(std::span<const T>(input.data(), input.size()));
  }

  template <typename CharT, typename Traits, typename Alloc>
    requires std::is_integral_v<CharT>
  void Init(const std::basic_string<CharT, Traits, Alloc>& input) {
    Init(std::span<const CharT>(input.data(), input.size()));
  }

  template <typename T>
    requires std::is_integral_v<T>
  void Init(std::span<const T> input) {
    AKRBT_ASSERT(!input.empty(),
                 "input must be non-empty");
    AKRBT_ASSERT(input.size() <= (u64)(1u << 24),
                 "input size exceeds the maximum allowed range");

    n_ = (i32)input.size();
    sa_.resize(n_ + 1);

    T minn = input[0], maxx = input[0];
    for (i32 i = 1; i < n_; i++) {
      minn = std::min<T>(minn, input[i]);
      maxx = std::max<T>(maxx, input[i]);
    }

    arr_.resize(n_ + 1);
    arr_[n_] = 0;

    i32 ub = 0;
    u64 dt = (u64)maxx - (u64)minn, bnd = std::max<u64>(n_, 256);
    if (dt < bnd) {
      for (i32 i = 0; i < n_; i++) {
        arr_[i] = (i32)(input[i] - minn) + 1;
      }
      ub = (i32)(dt + 1);
    } else {
      bkt_.resize(n_);
      std::iota(bkt_.begin(), bkt_.end(), 0);
      std::sort(bkt_.begin(), bkt_.end(), [&](i32 lhs, i32 rhs) { return input[lhs] < input[rhs]; });

      i32 idx = 0;
      while (idx < n_) {
        i32 nxt = idx, ord = ++ub;
        T val = input[bkt_[idx]];
        while (nxt < n_ && val == input[bkt_[nxt]]) arr_[bkt_[nxt++]] = ord;
        idx = nxt;
      }
    }

    SAIS(std::span<const i32>(arr_.data(), arr_.size()), ub);
  }

  i32 operator[](i32 i) const {
    AKRBT_ASSERT(n_ > 0,
                 "`Init()` must be called first");
    AKRBT_ASSERT(0 <= i && i < n_,
                 "index must be within input range");

    return sa_[i + 1];
  }

 private:
  void SAIS(std::span<const i32> s, i32 ub) {
    i32 n = (i32)s.size();

    std::fill(sa_.begin(), sa_.begin() + n, -1);

    std::vector<i8> ltypes(n);
    for (i32 i = n - 2; i >= 0; i--) {
      ltypes[i] = (s[i] > s[i + 1] || (s[i] == s[i + 1] && ltypes[i + 1]));
    }

    std::vector<i32> psum(ub + 1);
    for (auto e : s) ++psum[e];
    for (i32 i = 0; i < ub; i++) psum[i + 1] += psum[i];

    Induce(s, ltypes, psum);

    i32 cnt = 0;
    for (i32 i = 0; i < n; i++) {
      i32 pos = sa_[i];
      if (pos > 0 && ltypes[pos - 1] > ltypes[pos]) sa_[cnt++] = pos;
    }

    i32 half = n >> 1;
    auto sub = std::span<i32>(sa_.data() + half, n - half);
    std::fill(sub.begin(), sub.end(), -1);

    i32 rank = 0;
    sub[(sa_[0] - 1) >> 1] = 0;
    for (i32 i = 1; i < cnt; i++) {
      i32 a = sa_[i - 1], b = sa_[i];
      for (;; a++, b++) {
        if (s[a] != s[b] || ltypes[a] != ltypes[b]) {
          ++rank;
          break;
        }
        if (ltypes[a] > ltypes[a + 1] && ltypes[b] > ltypes[b + 1]) {
          rank += (s[a + 1] != s[b + 1]);
          break;
        }
      }

      sub[(sa_[i] - 1) >> 1] = rank;
    }

    auto it = std::remove(sub.begin(), sub.end(), -1);
    sub = std::span<i32>(sub.begin(), it);

    i32 sub_len = (i32)sub.size();
    if (rank + 1 != sub_len) {
      SAIS(sub, rank);
      for (i32 i = 0; i < sub_len; i++) sub[sa_[i]] = i;
    }

    for (i32 i = 1, j = 0; i < n; i++) {
      if (ltypes[i - 1] > ltypes[i]) sa_[sub[j++]] = -i;
    }
    std::fill(sa_.begin() + sub_len, sa_.begin() + n, 0);

    Induce(s, ltypes, psum, sub_len);
  }

  void Induce(std::span<const i32> s, const std::vector<i8>& ltypes, const std::vector<i32>& psum, i32 lms_len = -1) {
    i32 n = (i32)s.size();
    bkt_.resize(psum.size());

    std::copy(psum.begin(), psum.end(), bkt_.begin());
    if (lms_len < 0) {
      for (i32 i = 1; i < n; i++) {
        if (ltypes[i - 1] > ltypes[i]) sa_[--bkt_[s[i]]] = i;
      }
    } else {
      for (i32 i = lms_len - 1; i >= 0; i--) {
        i32 pos = -sa_[i];
        sa_[--bkt_[s[pos]]] = pos;
      }
    }

    bkt_[0] = 0;
    std::copy(psum.begin(), psum.end() - 1, bkt_.begin() + 1);
    for (i32 i = 0; i < n; i++) {
      i32 pos = sa_[i] - 1;
      if (pos >= 0 && ltypes[pos]) sa_[bkt_[s[pos]]++] = pos;
    }

    std::copy(psum.begin(), psum.end(), bkt_.begin());
    for (i32 i = n - 1; i > 0; i--) {
      i32 pos = sa_[i] - 1;
      if (pos >= 0 && !ltypes[pos]) sa_[--bkt_[s[pos]]] = pos;
    }
  }

  i32 n_ = 0;
  std::vector<i32> sa_, bkt_, arr_;
};
class LCPArray {
 public:
  template <typename T>
    requires std::is_integral_v<T>
  void Init(const std::vector<T>& input, const SuffixArray& sa) {
    Init(std::span<const T>(input.data(), input.size()), sa);
  }

  template <typename CharT, typename Traits, typename Alloc>
    requires std::is_integral_v<CharT>
  void Init(const std::basic_string<CharT, Traits, Alloc>& input, const SuffixArray& sa) {
    Init(std::span<const CharT>(input.data(), input.size()), sa);
  }

  template <typename T>
    requires std::is_integral_v<T>
  void Init(std::span<const T> input, const SuffixArray& sa) {
    AKRBT_ASSERT(!input.empty(),
                 "input must be non-empty");
    AKRBT_ASSERT(input.size() <= (u64)(1u << 24),
                 "input size exceeds the maximum allowed range");

    n_ = (i32)input.size();
    lcp_.resize(n_);
    rank_.resize(n_);

    for (i32 i = 0; i < n_; i++) rank_[sa[i]] = i + 1;

    for (i32 i = 0, k = 0; i < n_; i++, k && --k) {
      if (rank_[i] == n_) {
        k = 0;
      } else {
        i32 j = sa[rank_[i]], l = std::max<i32>(i, j);
        while (l + k < n_ && input[i + k] == input[j + k]) k++;
        lcp_[rank_[i]] = k;
      }
    }
  }

  i32 operator[](i32 i) const {
    AKRBT_ASSERT(n_ > 0,
                 "`Init()` must be called first");
    AKRBT_ASSERT(0 <= i && i < n_,
                 "index must be within input range");

    return lcp_[i];
  }

 private:
  i32 n_ = 0;
  std::vector<i32> lcp_, rank_;
};

}
// ===== User source =====
#include <bits/stdc++.h>


using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string s;
  cin >> s;

  akrbt::SuffixArray sa;
  sa.Init(s);

  akrbt::LCPArray lcp;
  lcp.Init(s, sa);

  int64_t n = s.size();
  int64_t ans = n * (n + 1) / 2;
  for (int i = 0; i < n; i++) ans -= lcp[i];
  cout << ans;

  return 0;
}