#include <algorithm>
#include <cassert>
#include <cstdint>
#include <deque>
#include <iostream>
#include <utility>
#include <vector>

/// @complexity Time: Expected O(n^3 log p) with quadratic polynomial
/// arithmetic. Space: O(n^2) across the factor queue and temporaries.

namespace noya {

/// @brief One monic irreducible factor over F_p and its multiplicity.
struct finite_field_polynomial_factor {
  std::vector<std::uint64_t> polynomial;
  int multiplicity = 0;
};

namespace finite_field_polynomial_factorization_internal {

using polynomial = std::vector<std::uint64_t>;

inline void trim(polynomial &value) {
  while (!value.empty() && value.back() == 0) {
    value.pop_back();
  }
}

inline int degree(polynomial value) {
  trim(value);
  return int(value.size()) - 1;
}

inline std::uint64_t power(std::uint64_t base, std::uint64_t exponent,
                           std::uint64_t modulus) {
  std::uint64_t result = 1 % modulus;
  while (exponent > 0) {
    if (exponent & 1) {
      result = result * base % modulus;
    }
    base = base * base % modulus;
    exponent >>= 1;
  }
  return result;
}

inline polynomial monic(polynomial value, std::uint64_t modulus) {
  trim(value);
  if (value.empty()) {
    return value;
  }
  std::uint64_t scale = power(value.back(), modulus - 2, modulus);
  for (std::uint64_t &coefficient : value) {
    coefficient = coefficient * scale % modulus;
  }
  return value;
}

inline polynomial add(polynomial left, const polynomial &right,
                      std::uint64_t modulus) {
  left.resize(std::max(left.size(), right.size()));
  for (int i = 0; i < int(right.size()); i++) {
    left[i] += right[i];
    if (left[i] >= modulus) {
      left[i] -= modulus;
    }
  }
  trim(left);
  return left;
}

inline polynomial subtract(polynomial left, const polynomial &right,
                           std::uint64_t modulus) {
  left.resize(std::max(left.size(), right.size()));
  for (int i = 0; i < int(right.size()); i++) {
    if (left[i] >= right[i]) {
      left[i] -= right[i];
    } else {
      left[i] += modulus - right[i];
    }
  }
  trim(left);
  return left;
}

inline polynomial multiply(const polynomial &left, const polynomial &right,
                           std::uint64_t modulus) {
  if (left.empty() || right.empty()) {
    return {};
  }
  polynomial result(left.size() + right.size() - 1);
  for (int i = 0; i < int(left.size()); i++) {
    for (int j = 0; j < int(right.size()); j++) {
      result[i + j] =
          (result[i + j] + left[i] * right[j]) % modulus;
    }
  }
  trim(result);
  return result;
}

inline std::pair<polynomial, polynomial>
divmod(polynomial dividend, polynomial divisor, std::uint64_t modulus) {
  trim(dividend);
  trim(divisor);
  assert(!divisor.empty());
  if (dividend.size() < divisor.size()) {
    return {{}, dividend};
  }
  polynomial quotient(dividend.size() - divisor.size() + 1);
  std::uint64_t inverse_leading =
      power(divisor.back(), modulus - 2, modulus);
  for (int position = int(dividend.size() - divisor.size()); position >= 0;
       position--) {
    std::uint64_t coefficient =
        dividend[position + divisor.size() - 1] * inverse_leading % modulus;
    quotient[position] = coefficient;
    for (int index = 0; index < int(divisor.size()); index++) {
      std::uint64_t removed = coefficient * divisor[index] % modulus;
      std::uint64_t &target = dividend[position + index];
      target = target >= removed ? target - removed : target + modulus - removed;
    }
  }
  trim(quotient);
  trim(dividend);
  return {quotient, dividend};
}

inline polynomial remainder(const polynomial &value, const polynomial &modulus,
                            std::uint64_t prime) {
  return divmod(value, modulus, prime).second;
}

inline polynomial gcd(polynomial first, polynomial second,
                      std::uint64_t modulus) {
  trim(first);
  trim(second);
  while (!second.empty()) {
    polynomial next = remainder(first, second, modulus);
    first = std::move(second);
    second = std::move(next);
  }
  return monic(std::move(first), modulus);
}

inline polynomial multiply_mod(const polynomial &left, const polynomial &right,
                               const polynomial &polynomial_modulus,
                               std::uint64_t prime) {
  return remainder(multiply(left, right, prime), polynomial_modulus, prime);
}

inline polynomial power_mod(polynomial base, std::uint64_t exponent,
                            const polynomial &polynomial_modulus,
                            std::uint64_t prime) {
  polynomial result = remainder({1}, polynomial_modulus, prime);
  base = remainder(base, polynomial_modulus, prime);
  while (exponent > 0) {
    if (exponent & 1) {
      result = multiply_mod(result, base, polynomial_modulus, prime);
    }
    exponent >>= 1;
    if (exponent > 0) {
      base = multiply_mod(base, base, polynomial_modulus, prime);
    }
  }
  return result;
}

inline std::uint64_t splitmix64(std::uint64_t &state) {
  std::uint64_t value = (state += 0x9e3779b97f4a7c15ULL);
  value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
  value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;
  return value ^ (value >> 31);
}

inline polynomial random_polynomial(int coefficient_count,
                                    std::uint64_t modulus,
                                    std::uint64_t &state) {
  polynomial result(coefficient_count);
  for (std::uint64_t &coefficient : result) {
    coefficient = splitmix64(state) % modulus;
  }
  trim(result);
  return result;
}

inline polynomial odd_character(const polynomial &value, int factor_degree,
                                const polynomial &polynomial_modulus,
                                std::uint64_t prime) {
  polynomial conjugate =
      power_mod(value, (prime - 1) / 2, polynomial_modulus, prime);
  polynomial result = remainder({1}, polynomial_modulus, prime);
  for (int index = 0; index < factor_degree; index++) {
    result = multiply_mod(result, conjugate, polynomial_modulus, prime);
    if (index + 1 < factor_degree) {
      conjugate =
          power_mod(conjugate, prime, polynomial_modulus, prime);
    }
  }
  return result;
}

inline polynomial binary_trace(const polynomial &value, int factor_degree,
                               const polynomial &polynomial_modulus) {
  polynomial conjugate = remainder(value, polynomial_modulus, 2);
  polynomial result;
  for (int index = 0; index < factor_degree; index++) {
    result = add(std::move(result), conjugate, 2);
    if (index + 1 < factor_degree) {
      conjugate = multiply_mod(conjugate, conjugate, polynomial_modulus, 2);
    }
  }
  return result;
}

inline std::vector<polynomial>
equal_degree_factorization(polynomial value, int factor_degree,
                           std::uint64_t prime, std::uint64_t &state) {
  value = monic(std::move(value), prime);
  std::deque<polynomial> pending = {value};
  std::vector<polynomial> result;
  while (!pending.empty()) {
    polynomial current = monic(std::move(pending.front()), prime);
    pending.pop_front();
    int current_degree = degree(current);
    if (current_degree == factor_degree) {
      result.push_back(std::move(current));
      continue;
    }
    while (true) {
      polynomial random = random_polynomial(current_degree, prime, state);
      polynomial separator =
          prime == 2
              ? binary_trace(random, factor_degree, current)
              : subtract(odd_character(random, factor_degree, current, prime),
                         {1}, prime);
      polynomial left = gcd(current, separator, prime);
      int left_degree = degree(left);
      if (left_degree <= 0 || left_degree == current_degree) {
        continue;
      }
      polynomial right = divmod(current, left, prime).first;
      pending.push_back(std::move(left));
      pending.push_back(std::move(right));
      break;
    }
  }
  return result;
}

} // namespace finite_field_polynomial_factorization_internal

/// @brief Factor a monic polynomial over F_p. Distinct-degree factorization
/// uses gcd(f,x^(p^d)-x); Cantor-Zassenhaus character or trace tests split each
/// equal-degree part, and exact repeated division recovers multiplicities.
inline std::vector<finite_field_polynomial_factor>
factor_finite_field_polynomial(
    std::vector<std::uint64_t> polynomial, std::uint64_t prime,
    std::uint64_t seed = 0x13198a2e03707344ULL) {
  using namespace finite_field_polynomial_factorization_internal;
  assert(prime >= 2);
  for (std::uint64_t &coefficient : polynomial) {
    coefficient %= prime;
  }
  polynomial = monic(std::move(polynomial), prime);
  std::vector<finite_field_polynomial_factor> result;
  if (degree(polynomial) <= 0) {
    return result;
  }

  const finite_field_polynomial_factorization_internal::polynomial x = {0, 1};
  auto frobenius = x;
  for (int factor_degree = 1;
       degree(polynomial) > 0 && 2 * factor_degree <= degree(polynomial);
       factor_degree++) {
    frobenius = power_mod(frobenius, prime, polynomial, prime);
    auto same_degree_part = gcd(polynomial,
                                subtract(frobenius, x, prime), prime);
    if (degree(same_degree_part) <= 0) {
      continue;
    }
    auto factors = equal_degree_factorization(
        std::move(same_degree_part), factor_degree, prime, seed);
    for (auto &factor : factors) {
      int multiplicity = 0;
      while (degree(polynomial) >= degree(factor)) {
        auto [quotient, remainder_value] = divmod(polynomial, factor, prime);
        if (!remainder_value.empty()) {
          break;
        }
        polynomial = std::move(quotient);
        multiplicity++;
      }
      result.push_back({std::move(factor), multiplicity});
    }
  }
  if (degree(polynomial) > 0) {
    result.push_back({monic(std::move(polynomial), prime), 1});
  }
  return result;
}

} // namespace noya

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int degree;
  std::uint64_t prime;
  std::cin >> degree >> prime;
  std::vector<std::uint64_t> polynomial(degree + 1);
  for (auto &coefficient : polynomial) {
    std::cin >> coefficient;
  }
  auto factors = noya::factor_finite_field_polynomial(polynomial, prime);
  std::cout << factors.size() << '\n';
  for (const auto &factor : factors) {
    std::cout << factor.multiplicity << ' ' << factor.polynomial.size() - 1;
    for (auto coefficient : factor.polynomial) {
      std::cout << ' ' << coefficient;
    }
    std::cout << '\n';
  }
}
