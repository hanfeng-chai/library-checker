#include <bits/stdc++.h>

std::int64_t a_mag[2000000];
std::int64_t b_mag[2000000];

namespace aal::bigint::detail {

template <typename TDigit>
class resource_base
{
public:
  using size_type = std::uint32_t;
  using digit_type = TDigit;

protected:
  size_type size_;
  bool is_negative_;
  digit_type* mag_ = nullptr;

  constexpr resource_base(std::string_view number, size_type initial_capacity, digit_type* mag)
    : size_ {initial_capacity - 1}, is_negative_ {number[0] == '-'}, mag_ {mag}
  {
  }

  constexpr void remove_lz_() noexcept
  {
    while (size_ > 1 && mag_[size_ - 1] == 0)
      --size_;
  }
};

} // namespace aal::bigint::detail

namespace aal::bigint::detail::digit_storage {

class base : public resource_base<std::int64_t>
{
public:
  static constexpr digit_type radix = 1e18;
  static constexpr digit_type digits_per_chunk = 18;

private:
  [[nodiscard]] static constexpr auto get_chunk_cnt_(std::string_view number)
  {
    return static_cast<size_type>((number.size() - (number[0] == '-') + digits_per_chunk - 1) / digits_per_chunk) + 1;
  }

protected:
  constexpr base(std::string_view number, digit_type* mag) : resource_base(number, get_chunk_cnt_(number), mag)
  {
    digit_type mag_begin = is_negative_, chunk_end = static_cast<digit_type>(number.size());
    for (size_type i = 0; i < size_; ++i) {
      auto chunk_begin = std::max(chunk_end - digits_per_chunk, mag_begin);
      [[maybe_unused]] auto conv_result = std::from_chars(
        number.data() + chunk_begin, number.data() + chunk_end, mag_[i]
      );
      chunk_end = chunk_begin;
    }
  }
};

} // namespace aal::bigint::detail::digit_storage

namespace aal::bigint {

class simple_t : public detail::digit_storage::base
{
  constexpr void carry_() noexcept
  {
    auto initial_size = size_;
    remove_lz_();
    int coeff;
    if (mag_[size_ - 1] < 0) {
      coeff = -1;
      is_negative_ = true;
    }
    else {
      coeff = 1;
      is_negative_ = false;
    }
    size_ = initial_size;

    digit_type carr = 0, r;
    for (size_type i = 0; i < size_; ++i) {
      mag_[i] = mag_[i] * coeff + carr;
      r = mag_[i] % radix;
      carr = mag_[i] / radix - (r < 0);
      mag_[i] = r + radix * (r < 0);
    }
  }

public:
  explicit constexpr simple_t(std::string_view number, digit_type* mag) : base(number, mag)
  {
  }

  constexpr auto& operator +=(const simple_t& y)
  {
    digit_type x_coeff = is_negative_ ? -1 : 1, y_coeff = y.is_negative_ ? -1 : 1;

    auto initial_size = size_;
    size_ = std::max(size_, y.size_) + 1;
    std::fill(mag_ + initial_size, mag_ + size_, 0);

    for (size_type i = 0; i < size_; ++i)
      mag_[i] = mag_[i] * x_coeff + (i < y.size_ ? y.mag_[i] : 0) * y_coeff;

    carry_();
    remove_lz_();
    return *this;
  }

  template <typename TChar>
  friend auto& operator <<(std::basic_ostream<TChar>& ostr, const simple_t& n)
  {
    if (n.is_negative_)
      ostr << '-';

    ostr << n.mag_[n.size_ - 1];
    ostr << std::setfill('0');
    for (auto i = n.size_ - 2, end = static_cast<size_type>(-1); i != end; --i)
      ostr << std::setw(n.digits_per_chunk) << n.mag_[i];
    ostr << std::setfill(' ');
    return ostr;
  }
};

} // namespace aal::bigint

constexpr int size = 64 * 1024 * 1024;
char buffer[size];

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  using bint = aal::bigint::simple_t;

  int cnt = std::fread(buffer, sizeof(char), size, stdin);
  buffer[cnt] = '\n';

  char* l = buffer;
  char* r;

  r = static_cast<char*>(std::memchr(l, '\n', cnt));
  int t;
  std::from_chars(l, r, t);
  cnt -= r - l + 1;
  l = r + 1;

  while (t--) {
    r = static_cast<char*>(std::memchr(l, ' ', cnt));
    bint a(std::string_view(l, r - l), a_mag);
    cnt -= r - l + 1;
    l = r + 1;

    r = static_cast<char*>(std::memchr(l, '\n', cnt));
    bint b(std::string_view(l, r - l), b_mag);
    cnt -= r - l + 1;
    l = r + 1;

    std::cout << (a += b) << "\n";
  }
}
