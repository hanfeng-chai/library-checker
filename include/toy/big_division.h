#pragma once
#include <toy/big_integer.h>
namespace toy {
template <bool Hex = false>
struct BigDivision {
    using Big = BigInteger<Hex>;
    using Limb = typename Big::Limb;
    using Wide = std::conditional_t<Hex, u128, u64>;
    static constexpr Wide radix = [] {
        if constexpr (Hex)
            return u128(1) << 64;
        else
            return u64(Big::base);
    }();
    IntegerFFT fft;
    Buffer<Limb> u, v;
    static Limb low(Wide x) {
        if constexpr (Hex)
            return x;
        else
            return x % Big::base;
    }
    static Wide high(Wide x) {
        if constexpr (Hex)
            return x >> 64;
        else
            return x / Big::base;
    }
    static Limb quotient(Wide value, Limb divisor) {
        if constexpr (Hex) {
            // Baseline x86-64 DIV; high(value)<divisor guarantees no overflow.
            u64 q, r;
            asm("divq %4"
                : "=a"(q), "=d"(r)
                : "a"(u64(value)), "d"(u64(value >> 64)), "r"(divisor)
                : "cc");
            return q;
        } else
            return value / divisor;
    }
    static Big slice(const Big &a, usize skip = 0) {
        Big b;
        if (skip >= a.digits.n) return b;
        b.digits = Buffer<Limb>(a.digits.n - skip);
        memcpy(b.digits.p, a.digits.p + skip, b.digits.n * sizeof(Limb));
        b.trim();
        return b;
    }
    static void increment(Big &a) {
        usize i = 0;
        Limb last = Limb(radix - 1);
        while (i < a.digits.n && a.digits[i] == last) a.digits[i++] = 0;
        if (i == a.digits.n) {
            a.digits.reserve(i + 1);
            a.digits[a.digits.n++] = 1;
        } else
            ++a.digits[i];
    }
    static void decrement(Big &a) {
        usize i = 0;
        while (!a.digits[i]) a.digits[i++] = Limb(radix - 1);
        --a.digits[i];
        a.trim();
    }
    static Limb word_division(std::span<const Limb> a, Limb divisor, Big &q) {
        q.digits.reserve(a.size());
        Wide rem = 0;
        for (usize i = a.size(); i--;) {
            Wide value = rem * radix + a[i];
            q.digits[i] = quotient(value, divisor);
            rem = value - Wide(q.digits[i]) * divisor;
        }
        q.digits.n = a.size();
        q.negative = false;
        q.trim();
        return rem;
    }
    // Knuth D; normalization makes the quotient estimate at most two too high.
    void school(const Big &a, const Big &b, Big &q, Big &r) {
        usize n = a.digits.n, m = b.digits.n;
        if (m == 1) {
            Limb rem = word_division(std::span<const Limb>(a.digits), b.digits[0], q);
            r.digits.reserve(1);
            r.digits[0] = rem;
            r.digits.n = rem != 0;
            r.negative = false;
            return;
        }
        u.reserve(n + 1);
        v.reserve(m);
        u.n = n + 1;
        v.n = m;
        unsigned shift = 0;
        Limb factor = 1;
        if constexpr (Hex) {
            shift = std::countl_zero(b.digits[m - 1]);
            auto normalize = [&](const Big &input, Limb *output) {
                Limb carry = 0;
                for (usize i = 0; i < input.digits.n; ++i) {
                    Limb x = input.digits[i];
                    output[i] = (x << shift) | carry;
                    carry = shift ? x >> (64 - shift) : 0;
                }
                return carry;
            };
            normalize(b, v.p);
            u[n] = normalize(a, u.p);
        } else {
            factor = Big::base / (u64(b.digits[m - 1]) + 1);
            auto normalize = [&](const Big &input, Limb *output) {
                Wide carry = 0;
                for (usize i = 0; i < input.digits.n; ++i) {
                    Wide x = Wide(input.digits[i]) * factor + carry;
                    output[i] = low(x);
                    carry = high(x);
                }
                return Limb(carry);
            };
            normalize(b, v.p);
            u[n] = normalize(a, u.p);
        }
        q.digits.reserve(n - m + 1);
        q.digits.n = n - m + 1;
        for (usize j = n - m + 1; j--;) {
            Wide numerator = Wide(u[j + m]) * radix + u[j + m - 1];
            Wide estimate = u[j + m] == v[m - 1] ? radix - 1 : quotient(numerator, v[m - 1]);
            Wide rem = numerator - estimate * v[m - 1];
            while (rem < radix && estimate * v[m - 2] > rem * radix + u[j + m - 2]) {
                --estimate;
                rem += v[m - 1];
            }
            Wide carry = 0;
            for (usize i = 0; i < m; ++i) {
                Wide product = estimate * v[i] + carry;
                Limb digit = low(product);
                carry = high(product);
                bool borrow = u[j + i] < digit;
                if constexpr (Hex)
                    u[j + i] -= digit;
                else
                    u[j + i] = u[j + i] - digit + borrow * Big::base;
                carry += borrow;
            }
            bool negative = Wide(u[j + m]) < carry;
            u[j + m] = low(Wide(u[j + m]) + radix - carry);
            if (negative) {
                --estimate;
                Wide carry = 0;
                for (usize i = 0; i < m; ++i) {
                    Wide sum = Wide(u[j + i]) + v[i] + carry;
                    u[j + i] = low(sum);
                    carry = high(sum);
                }
                u[j + m] = low(Wide(u[j + m]) + carry);
            }
            q.digits[j] = estimate;
        }
        r.digits.reserve(m);
        r.digits.n = m;
        if constexpr (Hex) {
            for (usize i = 0; i < m; ++i)
                r.digits[i] = (u[i] >> shift) | (shift && i + 1 < m ? u[i + 1] << (64 - shift) : 0);
        } else {
            Wide carry = 0;
            for (usize i = m; i--;) {
                Wide value = carry * radix + u[i];
                r.digits[i] = value / factor;
                carry = value % factor;
            }
        }
        q.negative = r.negative = false;
        q.trim();
        r.trim();
    }
    Big inverse(const Big &divisor, usize precision) {
        usize length = divisor.digits.n;
        if (length <= 32 || precision - length <= 32) {
            Big numerator, quotient, remainder;
            numerator.digits = Buffer<Limb>(precision + 1);
            std::fill(numerator.digits.p, numerator.digits.p + precision + 1, Limb(0));
            numerator.digits[precision] = 1;
            school(numerator, divisor, quotient, remainder);
            return quotient;
        }
        usize half = (precision - length + 5) / 2, drop = length > half ? length - half : 0;
        auto top = slice(divisor, drop);
        usize previous = half + top.digits.n;
        auto r = inverse(top, previous);
        Big product, error, result;
        usize exponent = previous + drop, shift = 2 * exponent - precision;
        // The residual is O(beta^(length+2)). Recover its signed value from
        // a shorter cyclic product, then discard a tail worth <1 in the correction.
        usize omit = shift > r.digits.n + 2 ? shift - r.digits.n - 2 : 0;
        usize needed = std::max(length + 5, length + 3 - std::min(omit, length + 3) + r.digits.n);
        usize size = std::max<usize>(64, std::bit_ceil(Hex ? (needed * 64 + 13) / 14 : 2 * needed));
        usize period = Hex ? size * 14 / 64 : size / 2;
        auto fixed_r = fft.fixed(r.chunks(), size, true);
        auto coefficients = fft(divisor.chunks(), fixed_r);
        product.digits.reserve(period + 4);
        Big::carry_product(coefficients, product);
        if (product.digits.n > period) {
            auto tail = slice(product, period);
            product.digits.n = period;
            product.trim();
            Big::add(product, tail, product);
        }
        Big modulus;
        modulus.digits = Buffer<Limb>(period);
        std::fill(modulus.digits.p, modulus.digits.p + period, Limb(radix - 1));
        if (Big::compare_magnitude(product, modulus) >= 0)
            Big::template magnitude<true>(product, modulus, product);
        Big unit;
        unit.digits.resize(exponent % period + 1);
        unit.digits[exponent % period] = 1;
        product.negative = true;
        Big::add(unit, product, error);
        if (error.digits.n == period && error.digits[period - 1] >= radix / 2) {
            bool sign = !error.negative;
            Big::template magnitude<true>(modulus, error, error);
            error.negative = sign;
        }
        bool over = error.negative;
        error = slice(error, omit);
        if (over && omit) increment(error); // Round the negative correction away from zero.
        coefficients = fft(error.chunks(), fixed_r);
        product.digits.reserve(period + 4);
        Big::carry_product(coefficients, product);
        auto correction = slice(product, shift - omit);
        usize left_shift = precision - exponent;
        result.digits = Buffer<Limb>(r.digits.n + left_shift);
        std::fill(result.digits.p, result.digits.p + left_shift, Limb(0));
        memcpy(result.digits.p + left_shift, r.digits.p, r.digits.n * sizeof(Limb));
        if (over) {
            Big::template magnitude<true>(result, correction, result);
            decrement(result);
        } else
            Big::add(result, correction, result);
        return result;
    }
    void blocked(const Big &a, const Big &b, Big &q, Big &r) {
        usize n = a.digits.n, m = b.digits.n;
        usize needed = Hex ? ((m + 1) * 64 + 13) / 14 : 2 * (m + 1);
        usize cyclic_size = std::max<usize>(64, std::bit_ceil(needed));
        usize period = Hex ? cyclic_size * 14 / 64 : cyclic_size / 2;
        usize block = 0;
        u64 best_cost = -1;
        // Model the two variable transforms per block plus reciprocal setup.
        // Try FFT boundaries rather than arbitrary limb counts.
        for (usize scale = 1; scale <= 8; scale *= 2) {
            usize limit = period / scale > 48 ? period / scale - 32 : 16;
            usize pieces = (n - m + limit) / limit, width = (n - m + pieces) / pieces;
            usize chunks = Hex ? ((2 * width + 8) * 64 + 13) / 14 : 2 * (2 * width + 8);
            usize size = std::max<usize>(64, std::bit_ceil(chunks));
            u64 cost = (2 * pieces + 6) * size * std::countr_zero(size) +
                       (2 * pieces + 1) * cyclic_size * std::countr_zero(cyclic_size);
            if (cost < best_cost) best_cost = cost, block = width;
        }
        usize drop = m > block + 5 ? m - block - 5 : 0;
        auto denominator = slice(b, drop);
        if (drop) increment(denominator);
        usize precision = denominator.digits.n + block + 2;
        auto reciprocal = inverse(denominator, precision);
        precision += drop;
        auto rc = reciprocal.chunks();
        usize upper_chunks = Hex ? ((block + 2) * 64 + 13) / 14 : 2 * (block + 2);
        usize inverse_size = std::max<usize>(64, std::bit_ceil(rc.n + upper_chunks - 1));
        auto inverse_spectrum = fft.fixed(std::move(rc), inverse_size);
        auto divisor_spectrum = fft.fixed(b.chunks(), cyclic_size, true);
        q.digits.resize(n - m + 1);
        std::fill(q.digits.p, q.digits.p + q.digits.n, Limb(0));
        r.digits.n = 0;
        Big window, estimate, product;
        for (usize start = (n - 1) / block * block;; start -= block) {
            usize take = std::min(block, n - start), length = r.digits.n + take;
            window.digits.reserve(length);
            memcpy(window.digits.p, a.digits.p + start, take * sizeof(Limb));
            if (r.digits.n) memcpy(window.digits.p + take, r.digits.p, r.digits.n * sizeof(Limb));
            window.digits.n = length;
            window.trim();
            if (Big::compare_magnitude(window, b) < 0)
                r = slice(window);
            else {
                auto upper = slice(window, m - 1);
                auto input = upper.chunks();
                auto coefficients = fft(input, inverse_spectrum);
                product.digits.reserve(upper.digits.n + reciprocal.digits.n + 1);
                Big::carry_product(coefficients, product);
                estimate = slice(product, precision - (m - 1));
                if (!estimate.digits.n)
                    r = slice(window);
                else {
                    auto e_chunks = estimate.chunks();
                    coefficients = fft(e_chunks, divisor_spectrum);
                    // Compute A-qB modulo beta^period-1 directly in native limbs.
                    // q is a lower estimate and the residual is <3B<beta^period.
                    r.digits.reserve(period + 1);
                    r.digits.n = period;
                    if constexpr (Hex) {
                        u128 accumulator = 0;
                        int bits = 0;
                        usize j = 0;
                        i128 carry = 0;
                        for (usize i = 0; i < period; ++i) {
                            while (bits < 64) {
                                accumulator += u128(coefficients[j++]) << bits;
                                bits += 14;
                            }
                            i128 value =
                                (i < window.digits.n ? i128(window.digits[i]) : 0) +
                                (i + period < window.digits.n ? i128(window.digits[i + period])
                                                              : 0) -
                                i128(u64(accumulator)) + carry;
                            r.digits[i] = value;
                            carry = value >> 64;
                            accumulator >>= 64;
                            bits -= 64;
                        }
                        carry -= i128(accumulator);
                        for (usize i = 0; carry; ++i) {
                            assert(i < period);
                            i128 value = i128(r.digits[i]) + carry;
                            r.digits[i] = value;
                            carry = value >> 64;
                        }
                    } else {
                        auto split = [](i64 value, i64 &carry) -> u32 {
                            carry = value / Big::base;
                            i64 digit = value - carry * Big::base;
                            if (digit < 0) digit += Big::base, --carry;
                            return digit;
                        };
                        i64 carry = 0;
                        for (usize i = 0; i < period; ++i) {
                            i64 value =
                                (i < window.digits.n ? i64(window.digits[i]) : 0) +
                                (i + period < window.digits.n ? i64(window.digits[i + period])
                                                              : 0) -
                                i64(coefficients[2 * i] + coefficients[2 * i + 1] * 10000) + carry;
                            r.digits[i] = split(value, carry);
                        }
                        for (usize i = 0; carry; ++i) {
                            assert(i < period);
                            r.digits[i] = split(i64(r.digits[i]) + carry, carry);
                        }
                    }
                    r.negative = false;
                    r.trim();
                }
                while (Big::compare_magnitude(r, b) >= 0) {
                    increment(estimate);
                    Big::template magnitude<true>(r, b, r);
                }
                assert(start + estimate.digits.n <= q.digits.n);
                if (estimate.digits.n)
                    memcpy(q.digits.p + start, estimate.digits.p, estimate.digits.n * sizeof(Limb));
            }
            if (!start) break;
        }
        q.trim();
        r.trim();
    }

    // Outputs are distinct and do not alias inputs. Quotient truncates toward zero.
    void divmod(const Big &a, const Big &b, Big &q, Big &r) {
        bool qsign = a.negative != b.negative, rsign = a.negative;
        if (Big::compare_magnitude(a, b) < 0) {
            q.digits.n = 0;
            q.negative = false;
            r = slice(a);
            r.negative = r.digits.n && rsign;
            return;
        }
        usize n = a.digits.n, m = b.digits.n, zeros = 0;
        while (zeros + 1 < m && !b.digits[zeros]) ++zeros;
        if (zeros + 1 == m) {
            Limb rem = word_division(std::span<const Limb>(a.digits.p + zeros, n - zeros),
                                     b.digits[m - 1], q);
            r.digits.reserve(zeros + 1);
            if (zeros) memcpy(r.digits.p, a.digits.p, zeros * sizeof(Limb));
            r.digits[zeros] = rem;
            r.digits.n = zeros + 1;
            r.trim();
        } else if (m <= 32 || n - m < 8)
            school(a, b, q, r);
        else if (n + 1 >= 2 * m)
            blocked(a, b, q, r);
        else {
            usize digits = n - m + 5, drop = m > digits ? m - digits : 0;
            auto denominator = slice(b, drop);
            if (drop) increment(denominator);
            usize precision = digits + denominator.digits.n, total = precision + drop;
            auto reciprocal = inverse(denominator, precision);
            // reciprocal/beta^total <= 1/b, so omitting a low tail <beta^(m-1)
            // reduces the quotient estimate by less than one, even with rounded-up b.
            usize omit = m - 1;
            auto upper = slice(a, omit);
            Big product;
            Big::multiply(upper, reciprocal, product, &fft);
            q = slice(product, total - omit);
            Big::multiply(q, b, product, &fft);
            while (Big::compare_magnitude(product, a) > 0) {
                decrement(q);
                Big::template magnitude<true>(product, b, product);
            }
            r = slice(a);
            Big::template magnitude<true>(r, product, r);
            while (Big::compare_magnitude(r, b) >= 0) {
                increment(q);
                Big::template magnitude<true>(r, b, r);
            }
        }
        q.negative = q.digits.n && qsign;
        r.negative = r.digits.n && rsign;
    }
};
} // namespace toy
