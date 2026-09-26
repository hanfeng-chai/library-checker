#include<bits/stdc++.h>
#define FAST_IO
#ifndef INTEGER_H
#define INTEGER_H 20250904L
#define __AVX2__
#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <random>

#ifdef ENABLE_VALIDITY_CHECK
#define VALIDITY_CHECK(condition, errorType, message) \
    if (!(condition)) throw errorType(message);
#else
#define VALIDITY_CHECK(condition, errorType, message)
#endif

#if defined(__AVX2__)
#include <immintrin.h>
#elif defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#ifndef __CONSTEXPR
#ifdef _GLIBCXX14_CONSTEXPR
#define __CONSTEXPR _GLIBCXX14_CONSTEXPR
#else
#define __CONSTEXPR constexpr
#endif
#endif

#if __cplusplus >= 202002L
#define __CXX20_CONSTEXPR constexpr
#else
#define __CXX20_CONSTEXPR
#endif

namespace detail {
    constexpr std::uint32_t log2(std::uint32_t n) {
        VALIDITY_CHECK(n, std::invalid_argument, "log2 error: the provided integer is zero.");
#if defined(__GNUC__) && !defined(__clang__)
        return std::__lg(n);
#else
        return std::log2(n);
#endif
    }

    struct InputHelper {
        std::uint32_t table[0x10000];

        __CONSTEXPR InputHelper() : table() {
            for (std::uint32_t i = 48; i != 58; ++i)
                for (std::uint32_t j = 48; j != 58; ++j)
                    table[i << 8 | j] = (j & 15) * 10 + (i & 15);
        }

        std::uint32_t operator()(const char* value) const {
            const std::uint32_t high = static_cast<std::uint8_t>(value[1]);
            const std::uint32_t low = static_cast<std::uint8_t>(value[0]);
            return table[(high << 8) | low];
        }
    };

    struct OutputHelper {
        std::uint32_t table[10000];

        __CONSTEXPR OutputHelper() : table() {
            for (std::uint32_t *i = table, a = 48; a != 58; ++a)
                for (std::uint32_t b = 48; b != 58; ++b)
                    for (std::uint32_t c = 48; c != 58; ++c)
                        for (std::uint32_t d = 48; d != 58; ++d)
                            *i++ = a | (b << 8) | (c << 16) | (d << 24);
        }

        const char* operator()(std::uint32_t value) const {
            return reinterpret_cast<const char*>(table + value);
        }
    };

#if defined(__AVX2__)
    // AVX2 FFT omitted comment logic for brevity...
    struct TransformHelper {
        __m128d* twiddleFactors;
        std::uint32_t length;

        TransformHelper() : twiddleFactors(new __m128d[1]()), length(1) { *twiddleFactors = _mm_set_pd(0.0, 1.0); }
        ~TransformHelper() noexcept { delete[] twiddleFactors; }

        static inline __m128d complexMultiply(__m128d first, __m128d second) { return _mm_fmaddsub_pd(_mm_unpacklo_pd(first, first), second, _mm_unpackhi_pd(first, first) * _mm_permute_pd(second, 1)); }
        static inline __m128d complexMultiplyConjugate(__m128d first, __m128d second) { return _mm_fmsubadd_pd(_mm_unpacklo_pd(second, second), first, _mm_unpackhi_pd(second, second) * _mm_permute_pd(first, 1)); }
        static inline __m128d complexMultiplySpecial(__m128d first, __m128d second) { return _mm_fmadd_pd(_mm_unpacklo_pd(first, first), second, _mm_unpackhi_pd(first, first) * _mm_permute_pd(second, 1)); }
        static inline __m128d complexScalarMultiply(__m128d complex, double scalar) { return complex * _mm_set1_pd(scalar); }

        void resize(std::uint32_t transformLength) {
            if (transformLength > length << 1) {
                std::uint32_t halfLog = detail::log2(transformLength) >> 1, halfSize = 1 << halfLog;
                __m128d* baseFactors = new __m128d[halfSize << 1]();
                const double angleStep = std::acos(-1.0) / halfSize, fineAngleStep = angleStep / halfSize;
                for (std::uint32_t i = 0, j = (halfSize * 3) >> 1, phaseAccumulator = 0; i != halfSize; phaseAccumulator -= halfSize - (j >> __builtin_ctz(++i))) {
                    std::complex<double> first = std::polar(1.0, phaseAccumulator * angleStep), second = std::polar(1.0, phaseAccumulator * fineAngleStep);
                    baseFactors[i] = _mm_set_pd(first.imag(), first.real()), baseFactors[i | halfSize] = _mm_set_pd(second.imag(), second.real());
                }
                __m128d* newFactors = reinterpret_cast<__m128d*>(std::memcpy(new __m128d[transformLength >> 1], twiddleFactors, length << 4));
                delete[] twiddleFactors, twiddleFactors = newFactors;
                for (std::uint32_t i = length; i != transformLength >> 1; ++i)
                    twiddleFactors[i] = complexMultiply(baseFactors[i & (halfSize - 1)], baseFactors[halfSize | (i >> halfLog)]);
                delete[] baseFactors, length = transformLength >> 1;
            }
        }
        void decimationInFrequency(__m128d* dataArray, std::uint32_t transformSize) {
            for (std::uint32_t blockSize = transformSize >> 1, stepSize = transformSize; blockSize; stepSize = blockSize, blockSize >>= 1) {
                for (__m128d* currentElement = dataArray; currentElement != dataArray + blockSize; ++currentElement) {
                    const __m128d evenElement = *currentElement, oddElement = currentElement[blockSize];
                    *currentElement = evenElement + oddElement, currentElement[blockSize] = evenElement - oddElement;
                }
                for (__m128d *blockStart = dataArray + stepSize, *twiddlePointer = twiddleFactors + 1; blockStart != dataArray + transformSize; blockStart += stepSize, ++twiddlePointer) {
                    for (__m128d* currentElement = blockStart; currentElement != blockStart + blockSize; ++currentElement) {
                        const __m128d evenElement = *currentElement, oddElement = complexMultiply(currentElement[blockSize], *twiddlePointer);
                        *currentElement = evenElement + oddElement, currentElement[blockSize] = evenElement - oddElement;
                    }
                }
            }
        }
        void decimationInTime(__m128d* dataArray, std::uint32_t transformSize) {
            for (std::uint32_t blockSize = 1, stepSize = 2; blockSize != transformSize; blockSize = stepSize, stepSize <<= 1) {
                for (__m128d* currentElement = dataArray; currentElement != dataArray + blockSize; ++currentElement) {
                    const __m128d evenElement = *currentElement, oddElement = currentElement[blockSize];
                    *currentElement = evenElement + oddElement, currentElement[blockSize] = evenElement - oddElement;
                }
                for (__m128d *blockStart = dataArray + stepSize, *twiddlePointer = twiddleFactors + 1; blockStart != dataArray + transformSize; blockStart += stepSize, ++twiddlePointer) {
                    for (__m128d* currentElement = blockStart; currentElement != blockStart + blockSize; ++currentElement) {
                        const __m128d evenElement = *currentElement, oddElement = currentElement[blockSize];
                        *currentElement = evenElement + oddElement, currentElement[blockSize] = complexMultiplyConjugate(evenElement - oddElement, *twiddlePointer);
                    }
                }
            }
        }
        void frequencyDomainPointwiseMultiply(__m128d* firstArray, __m128d* secondArray, std::uint32_t transformSize) {
            const double normalizationFactor = 1.0 / transformSize, scalingFactor = normalizationFactor * 0.25;
            firstArray[0] = complexScalarMultiply(complexMultiplySpecial(firstArray[0], secondArray[0]), normalizationFactor);
            firstArray[1] = complexScalarMultiply(complexMultiply(firstArray[1], secondArray[1]), normalizationFactor);
            const __m128d conjugateMask = _mm_castsi128_pd(_mm_set_epi64x(std::int64_t(1ull << 63), 0));
            const __m128d negateMask = _mm_castsi128_pd(_mm_set_epi64x(std::int64_t(1ull << 63), std::int64_t(1ull << 63)));
            for (std::uint32_t blockStart = 2, blockEnd = 3; blockStart != transformSize; blockStart <<= 1, blockEnd <<= 1) {
                for (std::uint32_t forwardIndex = blockStart, backwardIndex = forwardIndex + blockStart - 1; forwardIndex != blockEnd; ++forwardIndex, --backwardIndex) {
                    const __m128d firstEven = _mm_add_pd(firstArray[forwardIndex], _mm_xor_pd(firstArray[backwardIndex], conjugateMask)), firstOdd = _mm_sub_pd(firstArray[forwardIndex], _mm_xor_pd(firstArray[backwardIndex], conjugateMask));
                    const __m128d secondEven = _mm_add_pd(secondArray[forwardIndex], _mm_xor_pd(secondArray[backwardIndex], conjugateMask)), secondOdd = _mm_sub_pd(secondArray[forwardIndex], _mm_xor_pd(secondArray[backwardIndex], conjugateMask));
                    const __m128d productA = _mm_sub_pd(complexMultiply(firstEven, secondEven), complexMultiply(complexMultiply(firstOdd, secondOdd), (forwardIndex & 1 ? _mm_xor_pd(twiddleFactors[forwardIndex >> 1], negateMask) : twiddleFactors[forwardIndex >> 1]))), productB = _mm_add_pd(complexMultiply(secondEven, firstOdd), complexMultiply(firstEven, secondOdd));
                    firstArray[forwardIndex] = complexScalarMultiply(_mm_add_pd(productA, productB), scalingFactor);
                    firstArray[backwardIndex] = _mm_xor_pd(complexScalarMultiply(_mm_sub_pd(productA, productB), scalingFactor), conjugateMask);
                }
            }
        }
    };
#elif defined(__ARM_NEON__)
    // NEON omitted comment logic for brevity...
#else
    struct TransformHelper {
        std::complex<double>* twiddleFactors;
        std::uint32_t length;

        TransformHelper() : twiddleFactors(new std::complex<double>[1]()), length(1) { *twiddleFactors = {1.0, 0.0}; }
        ~TransformHelper() noexcept { delete[] twiddleFactors; }

        static inline std::complex<double> complexMultiply(std::complex<double> first, std::complex<double> second) { return first * second; }
        static inline std::complex<double> complexMultiplyConjugate(std::complex<double> first, std::complex<double> second) { return first * std::conj(second); }
        std::complex<double> complexMultiplySpecial(const std::complex<double>& first, const std::complex<double>& second) {
            return std::complex<double>(first.real() * second.real() + first.imag() * second.imag(), first.real() * second.imag() + first.imag() * second.real());
        }
        static inline std::complex<double> complexScalarMultiply(std::complex<double> complex, double scalar) { return complex * scalar; }

        void resize(std::uint32_t transformLength) {
            if (transformLength > length << 1) {
                std::uint32_t halfLog = detail::log2(transformLength) >> 1, halfSize = 1 << halfLog;
                std::complex<double>* baseFactors = new std::complex<double>[halfSize << 1]();
                const double angleStep = std::acos(-1.0) / halfSize, fineAngleStep = angleStep / halfSize;
                for (std::uint32_t i = 0, j = (halfSize * 3) >> 1, phaseAccumulator = 0; i != halfSize; phaseAccumulator -= halfSize - (j >> detail::log2(i ^ (i - 1)))) {
                    baseFactors[i] = std::polar(1.0, phaseAccumulator * angleStep);
                    baseFactors[i | halfSize] = std::polar(1.0, phaseAccumulator * fineAngleStep);
                    ++i;
                }
                std::complex<double>* newFactors = new std::complex<double>[transformLength >> 1];
                std::memcpy(newFactors, twiddleFactors, length * sizeof(std::complex<double>));
                delete[] twiddleFactors; twiddleFactors = newFactors;
                for (std::uint32_t i = length; i != transformLength >> 1; ++i) twiddleFactors[i] = baseFactors[i & (halfSize - 1)] * baseFactors[halfSize | (i >> halfLog)];
                delete[] baseFactors; length = transformLength >> 1;
            }
        }

        void decimationInFrequency(std::complex<double>* dataArray, std::uint32_t transformSize) {
            for (std::uint32_t blockSize = transformSize >> 1, stepSize = transformSize; blockSize; stepSize = blockSize, blockSize >>= 1) {
                for (std::complex<double>* currentElement = dataArray; currentElement != dataArray + blockSize; ++currentElement) {
                    const std::complex<double> evenElement = *currentElement, oddElement = currentElement[blockSize];
                    *currentElement = evenElement + oddElement; currentElement[blockSize] = evenElement - oddElement;
                }
                for (std::complex<double>*blockStart = dataArray + stepSize, *twiddlePointer = twiddleFactors + 1; blockStart != dataArray + transformSize; blockStart += stepSize, ++twiddlePointer) {
                    for (std::complex<double>* currentElement = blockStart; currentElement != blockStart + blockSize; ++currentElement) {
                        const std::complex<double> evenElement = *currentElement, oddElement = currentElement[blockSize] * *twiddlePointer;
                        *currentElement = evenElement + oddElement; currentElement[blockSize] = evenElement - oddElement;
                    }
                }
            }
        }

        void decimationInTime(std::complex<double>* dataArray, std::uint32_t transformSize) {
            for (std::uint32_t blockSize = 1, stepSize = 2; blockSize != transformSize; blockSize = stepSize, stepSize <<= 1) {
                for (std::complex<double>* currentElement = dataArray; currentElement != dataArray + blockSize; ++currentElement) {
                    const std::complex<double> evenElement = *currentElement, oddElement = currentElement[blockSize];
                    *currentElement = evenElement + oddElement; currentElement[blockSize] = evenElement - oddElement;
                }
                for (std::complex<double>*blockStart = dataArray + stepSize, *twiddlePointer = twiddleFactors + 1; blockStart != dataArray + transformSize; blockStart += stepSize, ++twiddlePointer) {
                    for (std::complex<double>* currentElement = blockStart; currentElement != blockStart + blockSize; ++currentElement) {
                        const std::complex<double> evenElement = *currentElement, oddElement = currentElement[blockSize];
                        *currentElement = evenElement + oddElement; currentElement[blockSize] = (evenElement - oddElement) * std::conj(*twiddlePointer);
                    }
                }
            }
        }

        void frequencyDomainPointwiseMultiply(std::complex<double>* firstArray, std::complex<double>* secondArray, std::uint32_t transformSize) {
            const double normalizationFactor = 1.0 / transformSize, scalingFactor = normalizationFactor * 0.25;
            firstArray[0] = complexScalarMultiply(complexMultiplySpecial(firstArray[0], secondArray[0]), normalizationFactor);
            firstArray[1] = complexScalarMultiply(complexMultiply(firstArray[1], secondArray[1]), normalizationFactor);
            for (std::uint32_t blockStart = 2, blockEnd = 3; blockStart != transformSize; blockStart <<= 1, blockEnd <<= 1) {
                for (std::uint32_t forwardIndex = blockStart, backwardIndex = forwardIndex + blockStart - 1; forwardIndex != blockEnd; ++forwardIndex, --backwardIndex) {
                    const std::complex<double> firstEven = firstArray[forwardIndex] + std::conj(firstArray[backwardIndex]), firstOdd = firstArray[forwardIndex] - std::conj(firstArray[backwardIndex]);
                    const std::complex<double> secondEven = secondArray[forwardIndex] + std::conj(secondArray[backwardIndex]), secondOdd = secondArray[forwardIndex] - std::conj(secondArray[backwardIndex]);
                    const std::complex<double> twiddle = (forwardIndex & 1 ? -twiddleFactors[forwardIndex >> 1] : twiddleFactors[forwardIndex >> 1]);
                    const std::complex<double> productA = firstEven * secondEven - firstOdd * secondOdd * twiddle, productB = secondEven * firstOdd + firstEven * secondOdd;
                    firstArray[forwardIndex] = (productA + productB) * scalingFactor;
                    firstArray[backwardIndex] = std::conj((productA - productB) * scalingFactor);
                }
            }
        }
    };
#endif

    static __CONSTEXPR InputHelper I = {};
    static __CONSTEXPR OutputHelper O = {};
    static thread_local TransformHelper T = {};
} // namespace detail

class UnsignedInteger;
class SignedInteger;

class UnsignedInteger {
    static constexpr std::uint32_t Base = 100000000;
    static constexpr std::uint32_t TransformLimit = 4194304;
    static constexpr std::uint32_t BruteforceThreshold = 64;

    std::uint32_t *digits, length, capacity;

  protected:
    __CXX20_CONSTEXPR UnsignedInteger(std::uint32_t initialLength, std::uint32_t initialCapacity) : digits(new std::uint32_t[initialCapacity]()), length(initialLength), capacity(initialCapacity) {}

    __CXX20_CONSTEXPR void resize(std::uint32_t newLength) {
        if (newLength > capacity) {
            std::uint32_t* newMemory = new std::uint32_t[capacity = newLength]();
            std::memcpy(newMemory, digits, length << 2);
            delete[] digits; digits = newMemory;
        }
        length = newLength;
    }

    __CXX20_CONSTEXPR void construct(const char* value, std::uint32_t stringLength) {
        std::uint32_t* currentDigit = digits + length - 1;
        switch (stringLength & 7) {
            case 0: ++currentDigit; break;
            case 1: *currentDigit = *value & 15; break;
            case 2: *currentDigit = detail::I(value); break;
            case 3: *currentDigit = (*value & 15) * 100 + detail::I(value + 1); break;
            case 4: *currentDigit = detail::I(value) * 100 + detail::I(value + 2); break;
            case 5: *currentDigit = (*value & 15) * 10000 + detail::I(value + 1) * 100 + detail::I(value + 3); break;
            case 6: *currentDigit = detail::I(value) * 10000 + detail::I(value + 2) * 100 + detail::I(value + 4); break;
            case 7: *currentDigit = (*value & 15) * 1000000 + detail::I(value + 1) * 10000 + detail::I(value + 3) * 100 + detail::I(value + 5); break;
        }
        for (const char* position = value + (stringLength & 7); currentDigit != digits; *--currentDigit = detail::I(position) * 1000000 + detail::I(position + 2) * 10000 + detail::I(position + 4) * 100 + detail::I(position + 6), position += 8);
        for (; length > 1 && !digits[length - 1]; --length);
    }

    __CXX20_CONSTEXPR std::int32_t reverseCompare(const UnsignedInteger& other) const {
        constexpr std::uint32_t BlockSize = 64;
        const std::uint32_t remaining = length % BlockSize, *firstBlockPointer = digits + length - remaining, *secondBlockPointer = other.digits + length - remaining;
        if (std::memcmp(firstBlockPointer, secondBlockPointer, remaining << 2))
            for (const std::uint32_t *firstElementPointer = firstBlockPointer + remaining, *secondElementPointer = secondBlockPointer + remaining; firstElementPointer != firstBlockPointer;)
                if (*--firstElementPointer != *--secondElementPointer) return std::int32_t(*firstElementPointer - *secondElementPointer);
        while (firstBlockPointer != digits)
            if (std::memcmp(firstBlockPointer -= BlockSize, secondBlockPointer -= BlockSize, BlockSize << 2))
                for (const std::uint32_t *firstElementPointer = firstBlockPointer + BlockSize, *secondElementPointer = secondBlockPointer + BlockSize; firstElementPointer != firstBlockPointer;)
                    if (*--firstElementPointer != *--secondElementPointer) return std::int32_t(*firstElementPointer - *secondElementPointer);
        return 0;
    }

    __CXX20_CONSTEXPR std::pair<UnsignedInteger, UnsignedInteger> bruteforceDivisionAndModulus(const UnsignedInteger& divisor) const {
        if (*this < divisor) return std::make_pair(UnsignedInteger(0), *this);
        UnsignedInteger quotient(length - divisor.length + 1, length - divisor.length + 1), remainder = *this;
        quotient.length = length - divisor.length + 1;
        auto getEstimatedValue = [](const std::uint32_t* digitArray, std::uint32_t highIndex, std::uint32_t arrayLength) -> std::uint64_t {
            return std::uint64_t(10) * Base * ((highIndex + 1) < arrayLength ? digitArray[highIndex + 1] : 0) + std::uint64_t(10) * digitArray[highIndex] + (highIndex ? digitArray[highIndex - 1] : 0) / (Base / 10);
        };
        for (std::uint32_t currentPosition = length - divisor.length; ~currentPosition; --currentPosition) {
            std::uint32_t partialQuotient = 0;
            auto performSubtraction = [&]() {
                std::int64_t carry = 0;
                for (std::uint32_t digitIndex = 0; digitIndex != divisor.length; ++digitIndex) {
                    carry = carry - std::int64_t(partialQuotient) * divisor.digits[digitIndex] + remainder.digits[currentPosition + digitIndex];
                    remainder.digits[currentPosition + digitIndex] = std::uint32_t(carry % Base), carry /= Base;
                    if (remainder.digits[currentPosition + digitIndex] >= Base) remainder.digits[currentPosition + digitIndex] += Base, --carry;
                }
                if (carry) remainder.digits[currentPosition + divisor.length] += std::uint32_t(carry);
                quotient.digits[currentPosition] += partialQuotient;
            };
            for (quotient.digits[currentPosition] = 0; (partialQuotient = std::uint32_t(getEstimatedValue(remainder.digits, currentPosition + divisor.length - 1, length) / (getEstimatedValue(divisor.digits, divisor.length - 1, divisor.length) + 1))); performSubtraction());
            partialQuotient = 1;
            for (std::uint32_t digitIndex = divisor.length; digitIndex--;)
                if (remainder.digits[digitIndex + currentPosition] != divisor.digits[digitIndex] && (partialQuotient = divisor.digits[digitIndex] < remainder.digits[digitIndex + currentPosition], true)) break;
            if (partialQuotient) performSubtraction();
        }
        for (; quotient.length > 1 && !quotient.digits[quotient.length - 1]; --quotient.length);
        for (; remainder.length > 1 && !remainder.digits[remainder.length - 1]; --remainder.length);
        return std::make_pair(std::move(quotient), std::move(remainder));
    }

    __CXX20_CONSTEXPR UnsignedInteger rightShift(std::uint32_t shiftAmount) const {
        if (shiftAmount >= length) return UnsignedInteger(0);
        UnsignedInteger result(length - shiftAmount, length - shiftAmount);
        std::memcpy(result.digits, digits + shiftAmount, result.length << 2);
        for (; result.length > 1 && !result.digits[result.length - 1]; --result.length);
        return result;
    }

    __CXX20_CONSTEXPR UnsignedInteger leftShift(std::uint32_t shiftAmount) const {
        if (length == 0 || shiftAmount == 0 || (length == 1 && digits[0] == 0)) return *this;
        UnsignedInteger result(length + shiftAmount, length + shiftAmount);
        std::memset(result.digits, 0, shiftAmount << 2);
        std::memcpy(result.digits + shiftAmount, digits, length << 2);
        return result;
    }

    UnsignedInteger computeInverse(std::uint32_t precisionBits) const {
        if (length < BruteforceThreshold || precisionBits < length + BruteforceThreshold) {
            UnsignedInteger numerator(precisionBits + 1, precisionBits + 1);
            std::memset(numerator.digits, 0, precisionBits << 2), numerator.digits[precisionBits] = 1;
            return numerator.bruteforceDivisionAndModulus(*this).first;
        }
        const std::uint32_t halfPrecision = (precisionBits - length + 5) >> 1, shiftBack = halfPrecision > length ? 0 : length - halfPrecision;
        UnsignedInteger truncated = rightShift(shiftBack);
        const std::uint32_t newPrecision = halfPrecision + truncated.length;
        UnsignedInteger approximateInverse = truncated.computeInverse(newPrecision);
        UnsignedInteger result = (approximateInverse + approximateInverse).leftShift(precisionBits - newPrecision - shiftBack) - (*this * approximateInverse * approximateInverse).rightShift(2 * (newPrecision + shiftBack) - precisionBits);
        return --result;
    }

    std::pair<UnsignedInteger, UnsignedInteger> divisionAndModulus(const UnsignedInteger& other) const {
        if (*this < other) return std::make_pair(UnsignedInteger(0), *this);
        if (length < BruteforceThreshold || other.length < BruteforceThreshold) return bruteforceDivisionAndModulus(other);
        const std::uint32_t precisionBits = length - other.length + 5, shiftBack = precisionBits > other.length ? 0 : other.length - precisionBits;
        UnsignedInteger adjustedDivisor = other.rightShift(shiftBack);
        if (shiftBack) ++adjustedDivisor;
        const std::uint32_t inversePrecision = precisionBits + adjustedDivisor.length;
        UnsignedInteger quotient = (*this * adjustedDivisor.computeInverse(inversePrecision)).rightShift(inversePrecision + shiftBack);
        while (quotient * other > *this) --quotient;
        UnsignedInteger remainder = *this - quotient * other;
        for (; remainder >= other; ++quotient, remainder -= other);
        return std::make_pair(std::move(quotient), std::move(remainder));
    }

  public:

    // 提供给快读进行零拷贝解析
    __CXX20_CONSTEXPR void assign(const char* value, std::uint32_t stringLength) {
        if (!stringLength) { *this = UnsignedInteger(0); return; }
        if (capacity < (length = (stringLength + 7) >> 3)) {
            delete[] digits; digits = new std::uint32_t[capacity = length];
        }
        construct(value, stringLength);
    }

    __CXX20_CONSTEXPR std::vector<std::uint32_t> to_base2_32() const {
        std::vector<std::uint32_t> res;
        UnsignedInteger n = *this;
        UnsignedInteger divisor(4294967296ull);
        while (n) {
            auto dm = n.divisionAndModulus(divisor);
            res.push_back((std::uint32_t)dm.second);
            n = std::move(dm.first);
        }
        return res;
    }

    __CXX20_CONSTEXPR static UnsignedInteger from_base2_32(const std::vector<std::uint32_t>& v) {
        UnsignedInteger res = UnsignedInteger(0), p = UnsignedInteger(1), multiplier(4294967296ull);
        std::uint32_t i = 0;
        // 【强制必须使用】循环展开优化
        for (; i + 4 <= v.size(); i += 4) {
            res += p * UnsignedInteger(v[i]); p *= multiplier;
            res += p * UnsignedInteger(v[i+1]); p *= multiplier;
            res += p * UnsignedInteger(v[i+2]); p *= multiplier;
            res += p * UnsignedInteger(v[i+3]); p *= multiplier;
        }
        for (; i < v.size(); ++i) { res += p * UnsignedInteger(v[i]); p *= multiplier; }
        return res;
    }

    __CXX20_CONSTEXPR UnsignedInteger() : digits(new std::uint32_t[1]()), length(1), capacity(1) {}
    __CXX20_CONSTEXPR UnsignedInteger(const UnsignedInteger& other) : digits(reinterpret_cast<std::uint32_t*>(std::memcpy(new std::uint32_t[other.length], other.digits, other.length << 2))), length(other.length), capacity(other.length) {}
    __CXX20_CONSTEXPR UnsignedInteger(UnsignedInteger&& other) noexcept : digits(other.digits), length(other.length), capacity(other.capacity) {
        other.digits = new std::uint32_t[other.length = other.capacity = 1]();
    }
    __CXX20_CONSTEXPR UnsignedInteger(const SignedInteger& other);

    template <typename unsignedIntegral, typename std::enable_if<std::is_unsigned<unsignedIntegral>::value>::type* = nullptr>
    __CXX20_CONSTEXPR UnsignedInteger(unsignedIntegral value) : UnsignedInteger(0, sizeof(unsignedIntegral) * 3) {
        while (digits[length++] = std::uint32_t(value % Base), value /= unsignedIntegral(Base));
        if (!length) length = 1;
    }

    template <typename signedIntegral, typename std::enable_if<std::is_signed<signedIntegral>::value && !std::is_floating_point<signedIntegral>::value>::type* = nullptr>
    __CXX20_CONSTEXPR UnsignedInteger(signedIntegral value) : UnsignedInteger(0, sizeof(signedIntegral) * 3) {
        VALIDITY_CHECK(value >= 0, std::invalid_argument, "UnsignedInteger Error: Negative signed integer.")
        while (digits[length++] = std::uint32_t(value % Base), value /= signedIntegral(Base));
        if (!length) length = 1;
    }

#ifdef __SIZEOF_INT128__
    __CXX20_CONSTEXPR UnsignedInteger(unsigned __int128 value) : UnsignedInteger(0, 5) {
        while (digits[length++] = std::uint32_t(value % Base), value /= Base);
        if (!length) length = 1;
    }
    __CXX20_CONSTEXPR UnsignedInteger(__int128 value) : UnsignedInteger(0, 5) {
        VALIDITY_CHECK(value >= 0, std::invalid_argument, "UnsignedInteger Error: Negative __int128");
        while (digits[length++] = std::uint32_t(value % Base), value /= Base);
        if (!length) length = 1;
    }
#endif

    template <typename floatingPoint, typename std::enable_if<std::is_floating_point<floatingPoint>::value>::type* = nullptr>
    UnsignedInteger(floatingPoint value) : UnsignedInteger(0, (std::numeric_limits<floatingPoint>::max_exponent10 + 7) >> 3) {
        VALIDITY_CHECK(value >= 0 && std::isfinite(value), std::invalid_argument, "UnsignedInteger Error: invalid float.")
        while (digits[length++] = std::uint32_t(std::fmod(value, Base)), (value = std::floor(value / Base)));
        if (!length) length = 1;
    }

    __CXX20_CONSTEXPR UnsignedInteger(const char* value) {
        const std::uint32_t stringLength = std::uint32_t(std::strlen(value));
        digits = new std::uint32_t[length = capacity = (stringLength + 7) >> 3];
        construct(value, stringLength);
    }

    __CXX20_CONSTEXPR UnsignedInteger(const std::string& value) : UnsignedInteger(std::uint32_t(value.size() + 7) >> 3, std::uint32_t(value.size() + 7) >> 3) {
        construct(value.data(), std::uint32_t(value.size()));
    }

    __CXX20_CONSTEXPR ~UnsignedInteger() noexcept { delete[] digits; }

    __CXX20_CONSTEXPR UnsignedInteger& operator=(const UnsignedInteger& other) {
        if (this != &other) {
            if (capacity < other.length) { delete[] digits; digits = new std::uint32_t[capacity = other.length]; }
            std::memcpy(digits, other.digits, (length = other.length) << 2);
        }
        return *this;
    }

    __CXX20_CONSTEXPR UnsignedInteger& operator=(UnsignedInteger&& other) noexcept {
        if (this != &other) { delete[] digits; digits = other.digits; length = other.length; capacity = other.capacity; other.digits = new std::uint32_t[other.length = other.capacity = 1](); }
        return *this;
    }

    __CXX20_CONSTEXPR UnsignedInteger& operator=(const SignedInteger& other);

    template <typename unsignedIntegral, typename std::enable_if<std::is_unsigned<unsignedIntegral>::value>::type* = nullptr>
    __CXX20_CONSTEXPR UnsignedInteger& operator=(unsignedIntegral value) {
        if (length = 0, capacity < (std::numeric_limits<unsignedIntegral>::digits10 + 7) >> 3) delete[] digits, digits = new std::uint32_t[capacity = (std::numeric_limits<unsignedIntegral>::digits10 + 7) >> 3];
        while (digits[length++] = std::uint32_t(value % Base), value /= unsignedIntegral(Base));
        if (!length) length = 1; return *this;
    }

    template <typename signedIntegral, typename std::enable_if<std::is_signed<signedIntegral>::value && !std::is_floating_point<signedIntegral>::value>::type* = nullptr>
    __CXX20_CONSTEXPR UnsignedInteger& operator=(signedIntegral value) {
        if (length = 0, capacity < (std::numeric_limits<signedIntegral>::digits10 + 7) >> 3) delete[] digits, digits = new std::uint32_t[capacity = (std::numeric_limits<signedIntegral>::digits10 + 7) >> 3];
        while (digits[length++] = std::uint32_t(value % Base), value /= signedIntegral(Base));
        if (!length) length = 1; return *this;
    }

    template <typename floatingPoint, typename std::enable_if<std::is_floating_point<floatingPoint>::value>::type* = nullptr>
    UnsignedInteger& operator=(floatingPoint value) {
        if (length = 0, capacity < (std::numeric_limits<floatingPoint>::max_exponent10 + 7) >> 3) delete[] digits, digits = new std::uint32_t[capacity = (std::numeric_limits<floatingPoint>::max_exponent10 + 7) >> 3];
        while (digits[length++] = std::uint32_t(std::fmod(value, Base)), (value = std::floor(value / Base)));
        if (!length) length = 1; return *this;
    }

    __CXX20_CONSTEXPR UnsignedInteger& operator=(const char* value) {
        const std::uint32_t stringLength = std::uint32_t(std::strlen(value));
        if (capacity < (length = (stringLength + 7) >> 3)) delete[] digits, digits = new std::uint32_t[capacity = length];
        return construct(value, stringLength), *this;
    }

    __CXX20_CONSTEXPR UnsignedInteger& operator=(const std::string& value) {
        if (capacity < (length = std::uint32_t(value.size() + 7) >> 3)) delete[] digits, digits = new std::uint32_t[capacity = length];
        return construct(value.data(), std::uint32_t(value.size())), *this;
    }

    friend std::istream& operator>>(std::istream& stream, UnsignedInteger& destination) { std::string buffer; return stream >> buffer, destination = buffer, stream; }
    friend std::ostream& operator<<(std::ostream& stream, const UnsignedInteger& source) { return stream << static_cast<const char*>(source); }

    template <typename unsignedIntegral, typename std::enable_if<std::is_unsigned<unsignedIntegral>::value>::type* = nullptr>
    __CXX20_CONSTEXPR operator unsignedIntegral() const {
        unsignedIntegral result = 0; for (std::uint32_t* i = digits + length; i != digits; result = result * unsignedIntegral(Base) + unsignedIntegral(*--i));
        return result;
    }

    template <typename signedIntegral, typename std::enable_if<std::is_signed<signedIntegral>::value && !std::is_floating_point<signedIntegral>::value>::type* = nullptr>
    __CXX20_CONSTEXPR operator signedIntegral() const {
        using UnsignedT = typename std::make_unsigned<signedIntegral>::type;
        UnsignedT result = 0; for (std::uint32_t* i = digits + length; i != digits; result = result * UnsignedT(Base) + UnsignedT(*--i));
        return static_cast<signedIntegral>(result);
    }

    template <typename floatingPoint, typename std::enable_if<std::is_floating_point<floatingPoint>::value>::type* = nullptr>
    operator floatingPoint() const {
        floatingPoint result = 0; for (std::uint32_t* i = digits + length; i != digits; result = result * floatingPoint(Base) + floatingPoint(*--i));
        return result;
    }

    explicit operator const char*() const {
        thread_local std::string buffers[8]; thread_local int cycle = 0; cycle = (cycle + 1) & 7;
        buffers[cycle] = this->operator std::string(); return buffers[cycle].c_str();
    }

    operator std::string() const {
        if (!length || (length == 1 && !digits[0])) return "0";
        std::uint32_t* i = digits + length; std::string result = std::to_string(*--i);
        for (result.reserve(length << 3); i-- != digits; result.append(detail::O(*i / 10000), 4), result.append(detail::O(*i % 10000), 4));
        return result;
    }

    __CXX20_CONSTEXPR operator bool() const noexcept { return length != 1 || *digits; }

#if __cplusplus >= 202002L
    __CXX20_CONSTEXPR std::strong_ordering operator<=>(const UnsignedInteger& other) const { return length == other.length ? reverseCompare(other) <=> 0 : length <=> other.length; }
#endif

    __CXX20_CONSTEXPR bool operator==(const UnsignedInteger& other) const {
        if (length != other.length) return false;
        std::uint32_t i = 0;
        for (; i + 4 <= length; i += 4) if (digits[i] != other.digits[i] || digits[i+1] != other.digits[i+1] || digits[i+2] != other.digits[i+2] || digits[i+3] != other.digits[i+3]) return false;
        for (; i < length; ++i) if (digits[i] != other.digits[i]) return false;
        return true;
    }
    __CXX20_CONSTEXPR bool operator!=(const UnsignedInteger& other) const { return !(*this == other); }
    __CXX20_CONSTEXPR bool operator<(const UnsignedInteger& other) const { return length < other.length || (length == other.length && reverseCompare(other) < 0); }
    __CXX20_CONSTEXPR bool operator>(const UnsignedInteger& other) const { return length > other.length || (length == other.length && reverseCompare(other) > 0); }
    __CXX20_CONSTEXPR bool operator<=(const UnsignedInteger& other) const { return length < other.length || (length == other.length && reverseCompare(other) <= 0); }
    __CXX20_CONSTEXPR bool operator>=(const UnsignedInteger& other) const { return length > other.length || (length == other.length && reverseCompare(other) >= 0); }

    UnsignedInteger& operator+=(const UnsignedInteger& other) {
        if (length <= other.length) {
            const std::uint32_t oldLength = length;
            resize(other.length + 1); std::memset(digits + oldLength, 0, (length - oldLength) << 2);
        }
        std::uint32_t *thisDigit = digits, *thisEnd = digits + length - 1;
        for (std::uint32_t *otherDigit = other.digits, *otherEnd = other.digits + other.length; otherDigit != otherEnd; ++thisDigit, ++otherDigit) {
#if defined(__GNUC__) && defined(__x86_64__)
            __asm__ volatile ( "add %2, %0\n\t" "cmp $100000000, %0\n\t" "jb 1f\n\t" "sub $100000000, %0\n\t" "addl $1, %1\n\t" "1:" : "+r" (*thisDigit), "+rm" (*(thisDigit + 1)) : "r" (*otherDigit) : "cc" );
#else
            if ((*thisDigit += *otherDigit) >= Base) *thisDigit -= Base, ++*(thisDigit + 1);
#endif
        }
        for (; thisDigit != thisEnd && *thisDigit >= Base; *thisDigit -= Base, ++*++thisDigit);
        for (; length > 1 && !digits[length - 1]; --length);
        return *this;
    }
    UnsignedInteger operator+(const UnsignedInteger& other) const { return UnsignedInteger(*this) += other; }

    UnsignedInteger& operator++() {
        std::uint32_t *thisDigit = digits, *thisEnd = digits + length - 1;
        for (++*thisDigit; thisDigit != thisEnd && *thisDigit >= Base; *thisDigit -= Base, ++*++thisDigit);
        if (thisDigit == thisEnd && *thisDigit >= Base) resize(length + 1), digits[length - 2] -= Base, digits[length - 1] = 1;
        return *this;
    }
    UnsignedInteger operator++(int) { UnsignedInteger result = *this; return ++*this, result; }

    UnsignedInteger& operator-=(const UnsignedInteger& other) {
        std::uint32_t *thisDigit = digits, *thisEnd = digits + length;
        for (std::uint32_t *otherDigit = other.digits, *otherEnd = other.digits + other.length; otherDigit != otherEnd; ++thisDigit, ++otherDigit)
            if ((*thisDigit -= *otherDigit) >= Base) *thisDigit += Base, --*(thisDigit + 1);
        for (; thisDigit != thisEnd && *thisDigit >= Base; *thisDigit += Base, --*++thisDigit);
        for (; length > 1 && !digits[length - 1]; --length);
        return *this;
    }
    UnsignedInteger operator-(const UnsignedInteger& other) const { return UnsignedInteger(*this) -= other; }

    UnsignedInteger& operator--() {
        std::uint32_t *thisDigit = digits, *thisEnd = digits + length - 1;
        for (--*thisDigit; thisDigit != thisEnd && *thisDigit >= Base; *thisDigit += Base, --*++thisDigit);
        for (; length > 1 && !digits[length - 1]; --length);
        return *this;
    }
    UnsignedInteger operator--(int) { UnsignedInteger result = *this; return --*this, result; }

    UnsignedInteger& operator*=(const UnsignedInteger& other) {
        if (length < BruteforceThreshold || other.length < BruteforceThreshold) {
            UnsignedInteger result(length + other.length - 1, length + other.length);
            std::uint64_t carry = 0;
            for (std::uint32_t i = 0; i != result.length; result.digits[i++] = std::uint32_t(carry % Base), carry /= Base)
                for (std::uint32_t j = (i >= length ? i - length + 1 : 0); j <= i && j < other.length; ++j) carry += std::uint64_t(digits[i - j]) * other.digits[j];
            if (carry) result.digits[result.length] = std::uint32_t(carry), ++result.length;
            for (; result.length > 1 && !result.digits[result.length - 1]; --result.length);
            return *this = std::move(result);
        }
#if defined(__AVX2__) || defined(__ARM_NEON__)
        const std::uint32_t resultLength = length + other.length, transformLength = 2u << detail::log2(resultLength - 1);
#if defined(__AVX2__)
        thread_local __m128d *firstArray = nullptr, *secondArray = nullptr; thread_local std::uint32_t allocatedSize = 0;
        if (allocatedSize < transformLength) delete[] firstArray, delete[] secondArray, firstArray = new __m128d[transformLength](), secondArray = new __m128d[transformLength](), allocatedSize = transformLength;
        std::memset(firstArray, 0, transformLength * sizeof(__m128d)), std::memset(secondArray, 0, transformLength * sizeof(__m128d));
        for (std::uint32_t i = 0; i != length; ++i) firstArray[i] = _mm_set_pd(std::floor(static_cast<double>(digits[i]) / 10000.0), static_cast<double>(digits[i] % 10000u));
        for (std::uint32_t i = 0; i != other.length; ++i) secondArray[i] = _mm_set_pd(std::floor(static_cast<double>(other.digits[i]) / 10000.0), static_cast<double>(other.digits[i] % 10000u));
        detail::T.resize(transformLength), detail::T.decimationInFrequency(firstArray, transformLength), detail::T.decimationInFrequency(secondArray, transformLength);
        detail::T.frequencyDomainPointwiseMultiply(firstArray, secondArray, transformLength), detail::T.decimationInTime(firstArray, transformLength);
#else
        thread_local float64x2_t *firstArray = nullptr, *secondArray = nullptr; thread_local std::uint32_t allocatedSize = 0;
        if (allocatedSize < transformLength) delete[] firstArray, delete[] secondArray, firstArray = new float64x2_t[transformLength](), secondArray = new float64x2_t[transformLength](), allocatedSize = transformLength;
        std::memset(firstArray, 0, transformLength * sizeof(float64x2_t)), std::memset(secondArray, 0, transformLength * sizeof(float64x2_t));
        for (std::uint32_t i = 0; i != length; ++i) { float64x2_t v = vdupq_n_f64(0.0); v = vsetq_lane_f64(static_cast<double>(digits[i] % 10000u), v, 0); v = vsetq_lane_f64(std::floor(static_cast<double>(digits[i]) / 10000.0), v, 1); firstArray[i] = v; }
        for (std::uint32_t i = 0; i != other.length; ++i) { float64x2_t v = vdupq_n_f64(0.0); v = vsetq_lane_f64(static_cast<double>(other.digits[i] % 10000u), v, 0); v = vsetq_lane_f64(std::floor(static_cast<double>(other.digits[i]) / 10000.0), v, 1); secondArray[i] = v; }
        detail::T.resize(transformLength), detail::T.decimationInFrequency(firstArray, transformLength), detail::T.decimationInFrequency(secondArray, transformLength);
        detail::T.frequencyDomainPointwiseMultiply(firstArray, secondArray, transformLength), detail::T.decimationInTime(firstArray, transformLength);
#endif
        UnsignedInteger result(resultLength, resultLength); std::uint64_t carry = 0;
#if defined(__AVX2__)
        for (std::uint32_t i = 0; i != resultLength; ++i) { __m128d v = firstArray[i]; double realPart = _mm_cvtsd_f64(v), imagPart = _mm_cvtsd_f64(_mm_unpackhi_pd(v, v)); carry += std::uint64_t(std::int64_t(realPart + 0.5) + std::int64_t(imagPart + 0.5) * 10000); result.digits[i] = std::uint32_t(carry % Base), carry /= Base; }
#else
        for (std::uint32_t i = 0; i != resultLength; ++i) { float64x2_t v = firstArray[i]; double realPart = vgetq_lane_f64(v, 0), imagPart = vgetq_lane_f64(v, 1); carry += std::uint64_t(std::int64_t(realPart + 0.5) + std::int64_t(imagPart + 0.5) * 10000); result.digits[i] = std::uint32_t(carry % Base), carry /= Base; }
#endif
#else
        thread_local std::complex<double>*firstArray = nullptr, *secondArray = nullptr; thread_local std::uint32_t allocatedSize = 0; const std::uint32_t resultLength = length + other.length, transformLength = 2u << detail::log2(resultLength - 1);
        if (allocatedSize < transformLength) delete[] firstArray, delete[] secondArray, firstArray = new std::complex<double>[transformLength](), secondArray = new std::complex<double>[transformLength](), allocatedSize = transformLength;
        std::memset(firstArray, 0, transformLength * sizeof(std::complex<double>)), std::memset(secondArray, 0, transformLength * sizeof(std::complex<double>));
        for (std::uint32_t i = 0; i != length; ++i) firstArray[i] = {static_cast<double>(digits[i] % 10000u), std::floor(static_cast<double>(digits[i]) / 10000.0)};
        for (std::uint32_t i = 0; i != other.length; ++i) secondArray[i] = {static_cast<double>(other.digits[i] % 10000u), std::floor(static_cast<double>(other.digits[i]) / 10000.0)};
        detail::T.resize(transformLength), detail::T.decimationInFrequency(firstArray, transformLength), detail::T.decimationInFrequency(secondArray, transformLength);
        detail::T.frequencyDomainPointwiseMultiply(firstArray, secondArray, transformLength), detail::T.decimationInTime(firstArray, transformLength);
        UnsignedInteger result(resultLength, resultLength); std::uint64_t carry = 0;
        for (std::uint32_t i = 0; i != resultLength; ++i) { carry += std::uint64_t(std::int64_t(firstArray[i].real() + 0.5) + std::int64_t(firstArray[i].imag() + 0.5) * 10000); result.digits[i] = std::uint32_t(carry % Base), carry /= Base; }
#endif
        for (; carry && result.length != result.capacity; result.digits[result.length++] = std::uint32_t(carry % Base), carry /= Base) if (result.length == result.capacity) result.resize(result.capacity << 1);
        for (; result.length > 1 && !result.digits[result.length - 1]; --result.length);
        return *this = std::move(result);
    }
    UnsignedInteger operator*(const UnsignedInteger& other) const { return UnsignedInteger(*this) *= other; }
    UnsignedInteger& operator/=(const UnsignedInteger& other) { return *this = std::move(divisionAndModulus(other).first); }
    UnsignedInteger operator/(const UnsignedInteger& other) const { return UnsignedInteger(*this) /= other; }
    UnsignedInteger& operator%=(const UnsignedInteger& other) { return *this = std::move(divisionAndModulus(other).second); }
    UnsignedInteger operator%(const UnsignedInteger& other) const { return UnsignedInteger(*this) %= other; }

    UnsignedInteger operator&(const UnsignedInteger& other) const {
        auto v1 = to_base2_32(), v2 = other.to_base2_32(); std::vector<std::uint32_t> v3(std::max(v1.size(), v2.size()), 0);
        for (size_t i = 0; i < v3.size(); ++i) v3[i] = (i < v1.size() ? v1[i] : 0) & (i < v2.size() ? v2[i] : 0);
        return from_base2_32(v3);
    }
    UnsignedInteger operator|(const UnsignedInteger& other) const {
        auto v1 = to_base2_32(), v2 = other.to_base2_32(); std::vector<std::uint32_t> v3(std::max(v1.size(), v2.size()), 0);
        for (size_t i = 0; i < v3.size(); ++i) v3[i] = (i < v1.size() ? v1[i] : 0) | (i < v2.size() ? v2[i] : 0);
        return from_base2_32(v3);
    }
    UnsignedInteger operator^(const UnsignedInteger& other) const {
        auto v1 = to_base2_32(), v2 = other.to_base2_32(); std::vector<std::uint32_t> v3(std::max(v1.size(), v2.size()), 0);
        for (size_t i = 0; i < v3.size(); ++i) v3[i] = (i < v1.size() ? v1[i] : 0) ^ (i < v2.size() ? v2[i] : 0);
        return from_base2_32(v3);
    }
    UnsignedInteger operator~() const {
        auto v = to_base2_32(); if (v.empty()) return UnsignedInteger(1);
        for (size_t i = 0; i < v.size() - 1; ++i) v[i] = ~v[i];
        std::uint32_t msb = v.back(), mask = 1; while (mask <= msb && mask != 0) mask <<= 1;
        v.back() = (~msb) & (mask - 1); return from_base2_32(v);
    }
    UnsignedInteger operator<<(std::uint32_t shift) const {
        auto v = to_base2_32(); if (v.empty() || (v.size() == 1 && v[0] == 0)) return UnsignedInteger(0);
        std::uint32_t block_shift = shift / 32, bit_shift = shift % 32, carry = 0; std::vector<std::uint32_t> res(v.size() + block_shift + 1, 0);
        for (size_t i = 0; i < v.size(); ++i) {
            std::uint64_t val = v[i];
            if (bit_shift) { res[i + block_shift] = ((val << bit_shift) | carry) & 0xFFFFFFFFull; carry = (std::uint32_t)(val >> (32 - bit_shift)); } 
            else { res[i + block_shift] = (std::uint32_t)val; carry = 0; }
        }
        if (carry) res.back() = carry; else res.pop_back();
        return from_base2_32(res);
    }
    UnsignedInteger operator>>(std::uint32_t shift) const {
        auto v = to_base2_32(); std::uint32_t block_shift = shift / 32; if (block_shift >= v.size()) return UnsignedInteger(0);
        std::uint32_t bit_shift = shift % 32, carry = 0; std::vector<std::uint32_t> res(v.size() - block_shift, 0);
        for (size_t i = v.size(); i-- > block_shift;) {
            std::uint32_t val = v[i];
            if (bit_shift) { res[i - block_shift] = (val >> bit_shift) | carry; carry = val << (32 - bit_shift); } 
            else { res[i - block_shift] = val; carry = 0; }
        }
        return from_base2_32(res);
    }

    UnsignedInteger isqrt() const {
        if (!*this) return UnsignedInteger(0); if (length == 1 && digits[0] == 1) return UnsignedInteger(1);
        UnsignedInteger x; x.resize((length + 1) / 2); x.digits[x.length - 1] = std::sqrt(digits[length - 1]) + 1;
        for (std::uint32_t i = 0; i < x.length - 1; ++i) x.digits[i] = 0;
        UnsignedInteger y = (x + *this / x) / UnsignedInteger(2);
        while (y < x) { x = y; y = (x + *this / x) / UnsignedInteger(2); }
        return x;
    }
    friend UnsignedInteger gcd(UnsignedInteger a, UnsignedInteger b) { while (b) { UnsignedInteger temp = b; b = a % b; a = temp; } return a; }
    friend UnsignedInteger lcm(const UnsignedInteger& a, const UnsignedInteger& b) { if (!a || !b) return UnsignedInteger(0); return (a / gcd(a, b)) * b; }
    static UnsignedInteger factorial(std::uint32_t n) {
        UnsignedInteger res = UnsignedInteger(1); std::uint64_t accum = 1;
        for (std::uint32_t i = 2; i <= n; ++i) { if (accum > 0xFFFFFFFFFFFFFFFFULL / i) { res *= UnsignedInteger(accum); accum = i; } else accum *= i; }
        if (accum > 1) res *= UnsignedInteger(accum);
        return res;
    }
    UnsignedInteger pow_mod(UnsignedInteger exp, const UnsignedInteger& mod) const {
        UnsignedInteger res = UnsignedInteger(1), base = *this % mod;
        while (exp) { if (exp.digits[0] & 1) res = (res * base) % mod; base = (base * base) % mod; exp /= UnsignedInteger(2); }
        return res;
    }
    bool is_prime(int k = 10) const {
        if (*this < UnsignedInteger(2)) return false; if (*this == UnsignedInteger(2) || *this == UnsignedInteger(3)) return true;
        if ((digits[0] & 1) == 0) return false;
        UnsignedInteger d = *this - UnsignedInteger(1); std::uint32_t s = 0;
        while ((d.digits[0] & 1) == 0) { d /= UnsignedInteger(2); s++; }
        thread_local std::mt19937 rng(std::random_device{}()); std::uniform_int_distribution<std::uint32_t> dist(0, 100000000);
        for (int i = 0; i < k; ++i) {
            UnsignedInteger a = UnsignedInteger(2) + UnsignedInteger(dist(rng)) % (*this - UnsignedInteger(3));
            UnsignedInteger x = a.pow_mod(d, *this);
            if (x == UnsignedInteger(1) || x == *this - UnsignedInteger(1)) continue;
            bool composite = true;
            for (std::uint32_t j = 1; j < s; ++j) { x = (x * x) % *this; if (x == *this - UnsignedInteger(1)) { composite = false; break; } }
            if (composite) return false;
        }
        return true;
    }
};

class SignedInteger {
    UnsignedInteger absolute;
    bool sign;

    std::vector<std::uint32_t> get_twos_complement(std::uint32_t target_size) const {
        auto v = absolute.to_base2_32(); while (v.size() < target_size) v.push_back(0);
        if (sign) { std::uint64_t carry = 1; for (size_t i = 0; i < v.size(); ++i) { std::uint64_t val = (~v[i] & 0xFFFFFFFFull) + carry; v[i] = val & 0xFFFFFFFFull; carry = val >> 32; } }
        return v;
    }

    SignedInteger bitwise_op(const SignedInteger& other, char op) const {
        auto v1 = absolute.to_base2_32(), v2 = other.absolute.to_base2_32(); size_t sz = std::max(v1.size(), v2.size()) + 1;
        auto t1 = get_twos_complement(sz), t2 = other.get_twos_complement(sz); std::vector<std::uint32_t> res(sz);
        for (size_t i = 0; i < sz; ++i) { if (op == '&') res[i] = t1[i] & t2[i]; else if (op == '|') res[i] = t1[i] | t2[i]; else if (op == '^') res[i] = t1[i] ^ t2[i]; }
        bool res_sign = false;
        if (op == '&') res_sign = sign & other.sign; else if (op == '|') res_sign = sign | other.sign; else if (op == '^') res_sign = sign ^ other.sign;
        if (res_sign) { std::uint64_t carry = 1; for (size_t i = 0; i < sz; ++i) { std::uint64_t val = (~res[i] & 0xFFFFFFFFull) + carry; res[i] = val & 0xFFFFFFFFull; carry = val >> 32; } }
        UnsignedInteger abs_val = UnsignedInteger::from_base2_32(res); if (!abs_val) res_sign = false;
        return SignedInteger(abs_val, res_sign);
    }

  protected:
    __CXX20_CONSTEXPR SignedInteger(const UnsignedInteger& initialAbsolute, bool initialSign) : absolute(initialAbsolute), sign(initialSign) {}

  public:
    friend class UnsignedInteger;

    // 提供给快读进行零拷贝解析
    __CXX20_CONSTEXPR void assign(const char* value, std::uint32_t stringLength) {
        if (!stringLength) { *this = SignedInteger(0); return; }
        sign = (value[0] == '-'); std::uint32_t offset = sign || (value[0] == '+');
        absolute.assign(value + offset, stringLength - offset);
        if (!absolute) sign = false;
    }

    __CXX20_CONSTEXPR SignedInteger() : absolute(), sign(false) {}
    __CXX20_CONSTEXPR SignedInteger(const SignedInteger& other) = default;
    __CXX20_CONSTEXPR SignedInteger(SignedInteger&& other) noexcept = default;
    __CXX20_CONSTEXPR SignedInteger(const UnsignedInteger& other) : absolute(other), sign(false) {}

    template <typename unsignedIntegral, typename std::enable_if<std::is_unsigned<unsignedIntegral>::value>::type* = nullptr>
    __CXX20_CONSTEXPR SignedInteger(unsignedIntegral value) : absolute(value), sign(false) {}
    template <typename signedIntegral, typename std::enable_if<std::is_signed<signedIntegral>::value && !std::is_floating_point<signedIntegral>::value>::type* = nullptr>
    __CXX20_CONSTEXPR SignedInteger(signedIntegral value) : absolute(std::abs(value)), sign(value < 0) {}
    template <typename floatingPoint, typename std::enable_if<std::is_floating_point<floatingPoint>::value>::type* = nullptr>
    SignedInteger(floatingPoint value) : absolute(std::abs(value)), sign(value < 0) {}

    __CXX20_CONSTEXPR SignedInteger(const char* value) { absolute = value + (sign = *value == '-'), sign = sign && bool(absolute); }
    __CXX20_CONSTEXPR SignedInteger(const std::string& value) { absolute = value.data() + (sign = value.front() == '-'), sign = sign && bool(absolute); }
    __CXX20_CONSTEXPR ~SignedInteger() = default;

    __CXX20_CONSTEXPR SignedInteger& operator=(const SignedInteger& other) { return absolute = other.absolute, sign = other.sign, *this; }
    __CXX20_CONSTEXPR SignedInteger& operator=(SignedInteger&& other) noexcept { return absolute = std::move(other.absolute), sign = other.sign, *this; }
    __CXX20_CONSTEXPR SignedInteger& operator=(const UnsignedInteger& other) { return absolute = other, sign = false, *this; }

    template <typename unsignedIntegral, typename std::enable_if<std::is_unsigned<unsignedIntegral>::value>::type* = nullptr>
    __CXX20_CONSTEXPR SignedInteger& operator=(unsignedIntegral value) { return absolute = value, sign = false, *this; }
    template <typename signedIntegral, typename std::enable_if<std::is_signed<signedIntegral>::value && !std::is_floating_point<signedIntegral>::value>::type* = nullptr>
    __CXX20_CONSTEXPR SignedInteger& operator=(signedIntegral value) { return absolute = std::abs(value), sign = value < 0, *this; }
    template <typename floatingPoint, typename std::enable_if<std::is_floating_point<floatingPoint>::value>::type* = nullptr>
    SignedInteger& operator=(floatingPoint value) { return absolute = std::abs(value), sign = value < 0, *this; }
    __CXX20_CONSTEXPR SignedInteger& operator=(const char* value) { return absolute = value + (sign = *value == '-'), sign = sign && bool(absolute), *this; }
    __CXX20_CONSTEXPR SignedInteger& operator=(const std::string& value) { return absolute = value.data() + (sign = value.front() == '-'), sign = sign && bool(absolute), *this; }

    friend std::istream& operator>>(std::istream& stream, SignedInteger& destination) { std::string buffer; return stream >> buffer, destination = buffer, stream; }
    friend std::ostream& operator<<(std::ostream& stream, const SignedInteger& source) { return stream << static_cast<const char*>(source); }

    template <typename unsignedIntegral, typename std::enable_if<std::is_unsigned<unsignedIntegral>::value>::type* = nullptr>
    __CXX20_CONSTEXPR operator unsignedIntegral() const { return unsignedIntegral(absolute); }
    template <typename signedIntegral, typename std::enable_if<std::is_signed<signedIntegral>::value && !std::is_floating_point<signedIntegral>::value>::type* = nullptr>
    __CXX20_CONSTEXPR operator signedIntegral() const {
        using UnsignedT = typename std::make_unsigned<signedIntegral>::type;
        UnsignedT magnitude = static_cast<UnsignedT>(absolute);
        return sign ? static_cast<signedIntegral>(UnsignedT(0) - magnitude) : static_cast<signedIntegral>(magnitude);
    }
    template <typename floatingPoint, typename std::enable_if<std::is_floating_point<floatingPoint>::value>::type* = nullptr>
    operator floatingPoint() const { return sign ? -floatingPoint(absolute) : floatingPoint(absolute); }

    explicit operator const char*() const {
        thread_local std::string buffers[8]; thread_local int cycle = 0; cycle = (cycle + 1) & 7;
        buffers[cycle] = this->operator std::string(); return buffers[cycle].c_str();
    }
    operator std::string() const { return sign && bool(absolute) ? "-" + absolute.operator std::string() : absolute.operator std::string(); }

    __CXX20_CONSTEXPR operator bool() const noexcept { return bool(absolute); }

#if __cplusplus >= 202002L
    __CXX20_CONSTEXPR std::strong_ordering operator<=>(const SignedInteger& other) const { return sign != other.sign ? other.sign <=> sign : (sign ? other.absolute <=> absolute : absolute <=> other.absolute); }
#endif

    __CXX20_CONSTEXPR bool operator==(const SignedInteger& other) const { return sign == other.sign && absolute == other.absolute; }
    __CXX20_CONSTEXPR bool operator!=(const SignedInteger& other) const { return sign != other.sign || absolute != other.absolute; }
    __CXX20_CONSTEXPR bool operator<(const SignedInteger& other) const { return sign ? !other.sign || absolute > other.absolute : !other.sign && absolute < other.absolute; }
    __CXX20_CONSTEXPR bool operator>(const SignedInteger& other) const { return sign ? other.sign && absolute < other.absolute : other.sign || absolute > other.absolute; }
    __CXX20_CONSTEXPR bool operator<=(const SignedInteger& other) const { return sign ? !other.sign || absolute >= other.absolute : !other.sign && absolute <= other.absolute; }
    __CXX20_CONSTEXPR bool operator>=(const SignedInteger& other) const { return sign ? other.sign && absolute <= other.absolute : other.sign || absolute >= other.absolute; }

    SignedInteger& operator+=(const SignedInteger& other) { sign == other.sign ? absolute += other.absolute : (absolute < other.absolute ? sign = !sign, absolute = other.absolute - absolute : absolute -= other.absolute), sign = sign && bool(absolute); return *this; }
    SignedInteger operator+(const SignedInteger& other) const { return SignedInteger(*this) += other; }
    SignedInteger& operator-=(const SignedInteger& other) { sign != other.sign ? absolute += other.absolute : (absolute < other.absolute ? sign = !sign, absolute = other.absolute - absolute : absolute -= other.absolute), sign = sign && bool(absolute); return *this; }
    SignedInteger operator-(const SignedInteger& other) const { return SignedInteger(*this) -= other; }
    SignedInteger& operator*=(const SignedInteger& other) { absolute *= other.absolute, sign = (sign ^ other.sign) && bool(absolute); return *this; }
    SignedInteger operator*(const SignedInteger& other) const { return SignedInteger(*this) *= other; }
    SignedInteger& operator/=(const SignedInteger& other) { absolute /= other.absolute, sign ^= other.sign, sign = sign && bool(absolute); return *this; }
    SignedInteger operator/(const SignedInteger& other) const { return SignedInteger(*this) /= other; }
    SignedInteger& operator%=(const SignedInteger& other) { *this -= (*this / other) * other; return *this; }
    SignedInteger operator%(const SignedInteger& other) const { return SignedInteger(*this) %= other; }

    SignedInteger operator-() const { return SignedInteger(absolute, bool(absolute) && !sign); }
    SignedInteger operator~() const { return -(*this) - SignedInteger(1); }
    SignedInteger operator<<(std::uint32_t shift) const { return SignedInteger(absolute << shift, sign); }
    SignedInteger operator>>(std::uint32_t shift) const { return SignedInteger(absolute >> shift, sign); }
    SignedInteger operator&(const SignedInteger& other) const { return bitwise_op(other, '&'); }
    SignedInteger operator|(const SignedInteger& other) const { return bitwise_op(other, '|'); }
    SignedInteger operator^(const SignedInteger& other) const { return bitwise_op(other, '^'); }

    SignedInteger isqrt() const { return SignedInteger(absolute.isqrt(), false); }
    friend SignedInteger gcd(const SignedInteger& a, const SignedInteger& b) { return SignedInteger(gcd(a.absolute, b.absolute), false); }
    friend SignedInteger lcm(const SignedInteger& a, const SignedInteger& b) { return SignedInteger(lcm(a.absolute, b.absolute), false); }
    static SignedInteger factorial(std::uint32_t n) { return SignedInteger(UnsignedInteger::factorial(n), false); }
    bool is_prime(int k = 10) const { return !sign && absolute.is_prime(k); }
};

inline __CXX20_CONSTEXPR UnsignedInteger::UnsignedInteger(const SignedInteger& other) : UnsignedInteger(other.absolute) {}
inline __CXX20_CONSTEXPR UnsignedInteger& UnsignedInteger::operator=(const SignedInteger& other) { return *this = other.absolute; }

inline UnsignedInteger operator""_UI(const char* literal, std::size_t) { return UnsignedInteger(literal); }
inline SignedInteger operator""_SI(const char* literal, std::size_t) { return SignedInteger(literal); }

#define DEFINE_ARITHMETIC_OPERATORS_UI(Type) \
    inline UnsignedInteger operator+(const UnsignedInteger& a, Type b) { return a + UnsignedInteger(b); } \
    inline UnsignedInteger operator+(Type a, const UnsignedInteger& b) { return UnsignedInteger(a) + b; } \
    inline UnsignedInteger operator-(const UnsignedInteger& a, Type b) { return a - UnsignedInteger(b); } \
    inline UnsignedInteger operator-(Type a, const UnsignedInteger& b) { return UnsignedInteger(a) - b; } \
    inline UnsignedInteger operator*(const UnsignedInteger& a, Type b) { return a * UnsignedInteger(b); } \
    inline UnsignedInteger operator*(Type a, const UnsignedInteger& b) { return UnsignedInteger(a) * b; } \
    inline UnsignedInteger operator/(const UnsignedInteger& a, Type b) { return a / UnsignedInteger(b); } \
    inline UnsignedInteger operator/(Type a, const UnsignedInteger& b) { return UnsignedInteger(a) / b; } \
    inline UnsignedInteger operator%(const UnsignedInteger& a, Type b) { return a % UnsignedInteger(b); } \
    inline UnsignedInteger operator%(Type a, const UnsignedInteger& b) { return UnsignedInteger(a) % b; } \
    inline bool operator==(const UnsignedInteger& a, Type b) { return a == UnsignedInteger(b); } \
    inline bool operator==(Type a, const UnsignedInteger& b) { return UnsignedInteger(a) == b; } \
    inline bool operator!=(const UnsignedInteger& a, Type b) { return a != UnsignedInteger(b); } \
    inline bool operator!=(Type a, const UnsignedInteger& b) { return UnsignedInteger(a) != b; } \
    inline bool operator<(const UnsignedInteger& a, Type b) { return a < UnsignedInteger(b); } \
    inline bool operator<(Type a, const UnsignedInteger& b) { return UnsignedInteger(a) < b; } \
    inline bool operator<=(const UnsignedInteger& a, Type b) { return a <= UnsignedInteger(b); } \
    inline bool operator<=(Type a, const UnsignedInteger& b) { return UnsignedInteger(a) <= b; } \
    inline bool operator>(const UnsignedInteger& a, Type b) { return a > UnsignedInteger(b); } \
    inline bool operator>(Type a, const UnsignedInteger& b) { return UnsignedInteger(a) > b; } \
    inline bool operator>=(const UnsignedInteger& a, Type b) { return a >= UnsignedInteger(b); } \
    inline bool operator>=(Type a, const UnsignedInteger& b) { return UnsignedInteger(a) >= b; }

#define DEFINE_ARITHMETIC_OPERATORS_SI(Type) \
    inline SignedInteger operator+(const SignedInteger& a, Type b) { return a + SignedInteger(b); } \
    inline SignedInteger operator+(Type a, const SignedInteger& b) { return SignedInteger(a) + b; } \
    inline SignedInteger operator-(const SignedInteger& a, Type b) { return a - SignedInteger(b); } \
    inline SignedInteger operator-(Type a, const SignedInteger& b) { return SignedInteger(a) - b; } \
    inline SignedInteger operator*(const SignedInteger& a, Type b) { return a * SignedInteger(b); } \
    inline SignedInteger operator*(Type a, const SignedInteger& b) { return SignedInteger(a) * b; } \
    inline SignedInteger operator/(const SignedInteger& a, Type b) { return a / SignedInteger(b); } \
    inline SignedInteger operator/(Type a, const SignedInteger& b) { return SignedInteger(a) / b; } \
    inline SignedInteger operator%(const SignedInteger& a, Type b) { return a % SignedInteger(b); } \
    inline SignedInteger operator%(Type a, const SignedInteger& b) { return SignedInteger(a) % b; } \
    inline bool operator==(const SignedInteger& a, Type b) { return a == SignedInteger(b); } \
    inline bool operator==(Type a, const SignedInteger& b) { return SignedInteger(a) == b; } \
    inline bool operator!=(const SignedInteger& a, Type b) { return a != SignedInteger(b); } \
    inline bool operator!=(Type a, const SignedInteger& b) { return SignedInteger(a) != b; } \
    inline bool operator<(const SignedInteger& a, Type b) { return a < SignedInteger(b); } \
    inline bool operator<(Type a, const SignedInteger& b) { return SignedInteger(a) < b; } \
    inline bool operator<=(const SignedInteger& a, Type b) { return a <= SignedInteger(b); } \
    inline bool operator<=(Type a, const SignedInteger& b) { return SignedInteger(a) <= b; } \
    inline bool operator>(const SignedInteger& a, Type b) { return a > SignedInteger(b); } \
    inline bool operator>(Type a, const SignedInteger& b) { return SignedInteger(a) > b; } \
    inline bool operator>=(const SignedInteger& a, Type b) { return a >= SignedInteger(b); } \
    inline bool operator>=(Type a, const SignedInteger& b) { return SignedInteger(a) >= b; }

DEFINE_ARITHMETIC_OPERATORS_UI(int)
DEFINE_ARITHMETIC_OPERATORS_UI(unsigned int)
DEFINE_ARITHMETIC_OPERATORS_UI(long long)
DEFINE_ARITHMETIC_OPERATORS_UI(unsigned long long)
#ifdef __SIZEOF_INT128__
DEFINE_ARITHMETIC_OPERATORS_UI(__int128)
DEFINE_ARITHMETIC_OPERATORS_UI(unsigned __int128)
#endif

DEFINE_ARITHMETIC_OPERATORS_SI(int)
DEFINE_ARITHMETIC_OPERATORS_SI(unsigned int)
DEFINE_ARITHMETIC_OPERATORS_SI(long long)
DEFINE_ARITHMETIC_OPERATORS_SI(unsigned long long)
#ifdef __SIZEOF_INT128__
DEFINE_ARITHMETIC_OPERATORS_SI(__int128)
DEFINE_ARITHMETIC_OPERATORS_SI(unsigned __int128)
#endif

typedef SignedInteger bint;
typedef UnsignedInteger ubint;

// ============================================================================
// ========================== FAST I/O IMPLEMENTATION =========================
// ============================================================================
#ifdef FAST_IO
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace fast_io {
    struct Scanner {
        char* p;
        char* end;
        std::vector<char> fallback_buf;

        Scanner() {
            struct stat s;
            if (fstat(0, &s) == 0 && S_ISREG(s.st_mode) && s.st_size > 0) {
                p = (char*)mmap(nullptr, s.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
                if (p != MAP_FAILED) {
                    end = p + s.st_size;
                    return;
                }
            }
            // fallback if stdin is not a file (e.g. pipe or terminal) or mmap fails
            char tmp[65536];
            int bytes;
            while ((bytes = ::read(0, tmp, sizeof(tmp))) > 0) {
                fallback_buf.insert(fallback_buf.end(), tmp, tmp + bytes);
            }
            if (!fallback_buf.empty()) {
                p = fallback_buf.data();
                end = p + fallback_buf.size();
            } else {
                p = end = nullptr;
            }
        }

        bool has_next() {
            if (!p) return false;
            while (p < end && *p <= ' ') ++p;
            return p < end;
        }

        template <typename T, typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type* = nullptr>
        void read(T& x) {
            if (!has_next()) return;
            bool sgn = false;
            if (*p == '-') { sgn = true; ++p; }
            else if (*p == '+') ++p;
            x = 0;
            while (p < end && *p >= '0' && *p <= '9') {
                x = x * 10 + (*p - '0');
                ++p;
            }
            if (sgn) x = -x;
        }

        template <typename T, typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value>::type* = nullptr>
        void read(T& x) {
            if (!has_next()) return;
            if (*p == '+') ++p;
            x = 0;
            while (p < end && *p >= '0' && *p <= '9') {
                x = x * 10 + (*p - '0');
                ++p;
            }
        }

        void read(UnsignedInteger& x) {
            if (!has_next()) return;
            char* start = p;
            while (p < end && *p >= '0' && *p <= '9') ++p;
            x.assign(start, p - start);
        }

        void read(SignedInteger& x) {
            if (!has_next()) return;
            char* start = p;
            if (*p == '-' || *p == '+') ++p;
            while (p < end && *p >= '0' && *p <= '9') ++p;
            x.assign(start, p - start);
        }

        void read(char* s) {
            if (!has_next()) return;
            while (p < end && *p > ' ') *s++ = *p++;
            *s = '\0';
        }

        void read(std::string& s) {
            if (!has_next()) return;
            s.clear();
            while (p < end && *p > ' ') s += *p++;
        }
    };

    struct Printer {
        char buf[1 << 20];
        int pos;

        Printer() : pos(0) {}
        ~Printer() { flush(); }

        void flush() {
            if (pos) {
                [[maybe_unused]] auto res = ::write(1, buf, pos);
                pos = 0;
            }
        }

        void print(char c) {
            if (pos == sizeof(buf)) flush();
            buf[pos++] = c;
        }

        void print(const char* s) {
            size_t len = std::strlen(s);
            if (pos + len >= sizeof(buf)) {
                flush();
                if (len >= sizeof(buf)) {
                    [[maybe_unused]] auto res = ::write(1, s, len);
                    return;
                }
            }
            std::memcpy(buf + pos, s, len);
            pos += len;
        }

        void print(const std::string& s) { print(s.c_str()); }

        template <typename T>
        void print_unsigned(T x) {
            if (x == 0) { print('0'); return; }
            char temp[32];
            int idx = 0;
            while (x) {
                temp[idx++] = (x % 10) + '0';
                x /= 10;
            }
            while (idx--) print(temp[idx]);
        }

        template <typename T, typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type* = nullptr>
        void print(T x) {
            if (x < 0) { print('-'); x = -x; }
            print_unsigned(typename std::make_unsigned<T>::type(x));
        }

        template <typename T, typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value>::type* = nullptr>
        void print(T x) {
            print_unsigned(x);
        }

        void print(const UnsignedInteger& x) { print(static_cast<const char*>(x)); }
        void print(const SignedInteger& x) { print(static_cast<const char*>(x)); }
    };

    inline Scanner& get_scanner() { static Scanner s; return s; }
    inline Printer& get_printer() { static Printer p; return p; }
} // namespace fast_io

template <typename T>
inline void read(T& x) { fast_io::get_scanner().read(x); }
template <typename T, typename... Args>
inline void read(T& x, Args&... args) { fast_io::get_scanner().read(x); read(args...); }

template <typename T>
inline void print(const T& x) { fast_io::get_printer().print(x); }
template <typename T, typename... Args>
inline void print(const T& x, const Args&... args) { fast_io::get_printer().print(x); print(args...); }

inline void println() { fast_io::get_printer().print('\n'); }
template <typename... Args>
inline void println(const Args&... args) { print(args...); fast_io::get_printer().print('\n'); }

#endif // FAST_IO

#undef VALIDITY_CHECK
#undef __CONSTEXPR
#undef __CXX20_CONSTEXPR
#endif
using namespace std;
int t;
bint a,b;
int main(){
    read(t);
    while(t--){
        read(a,b);
        println(a*b);
    }
    return 0;
}