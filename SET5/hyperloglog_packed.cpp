#include "hyperloglog_packed.h"
#include <cmath>
#include <algorithm>

HyperLogLogPacked::HyperLogLogPacked(uint8_t p)
    : p_(p), m_(1u << p) {
    const std::size_t bits = 6ull * static_cast<std::size_t>(m_);
    const std::size_t bytes = (bits + 7ull) / 8ull;
    data_.assign(bytes, 0);
}

void HyperLogLogPacked::reset() {
    std::ranges::fill(data_, 0);
}

inline uint32_t HyperLogLogPacked::leading_zeros32(uint32_t x) {
    if (x == 0) {
        return 32;
    }
    return static_cast<uint32_t>(__builtin_clz(x));
}

uint8_t HyperLogLogPacked::get_reg(const uint32_t ind) const {
    //битовая позиция начала регистра
    const auto bitpos = 6ull * static_cast<uint64_t>(ind);
    const auto bytepos = bitpos / 8ull;
    const auto shift = static_cast<uint32_t>(bitpos % 8ull);

    //считываем окно 16..24 бит, чтобы точно захватить 6 бит регистра
    uint32_t w = data_[bytepos];
    if (bytepos + 1 < data_.size()) {
        w |= static_cast<uint32_t>(data_[bytepos + 1]) << 8;
    }
    if (bytepos + 2 < data_.size()) {
        w |= static_cast<uint32_t>(data_[bytepos + 2]) << 16;
    }

    const uint32_t val = (w >> shift) & 0x3Fu;
    return static_cast<uint8_t>(val);
}

void HyperLogLogPacked::set_reg_max(uint32_t ind, uint8_t val) {
    if (val > 63) {
        val = 63;
    }
    const uint8_t curr = get_reg(ind);
    if (val <= curr) {
        return; // обновлять не нужно
    }

    const uint64_t bitpos = 6ull * static_cast<uint64_t>(ind);
    const uint64_t bytepos = bitpos / 8ull;
    const auto shift = static_cast<uint32_t>(bitpos % 8ull);

    uint32_t w = data_[bytepos];
    if (bytepos + 1 < data_.size()) {
        w |= static_cast<uint32_t>(data_[bytepos + 1]) << 8;
    }
    if (bytepos + 2 < data_.size()) {
        w |= static_cast<uint32_t>(data_[bytepos + 2]) << 16;
    }

    //очищаем 6 бит и записываем новое значение
    w &= ~(0x3Fu << shift);
    w |= (static_cast<uint32_t>(val) & 0x3Fu) << shift;

    //возвращаем обратно в массив байт
    data_[bytepos] = static_cast<uint8_t>(w & 0xFFu);
    if (bytepos + 1 < data_.size()) {
        data_[bytepos + 1] = static_cast<uint8_t>((w >> 8) & 0xFFu);
    }
    if (bytepos + 2 < data_.size()) {
        data_[bytepos + 2] = static_cast<uint8_t>((w >> 16) & 0xFFu);
    }
}

void HyperLogLogPacked::add_hash(const uint32_t h) {
    const uint32_t ind = h >> (32 - p_);
    const uint32_t w = (h << p_) | (1u << (p_ - 1));
    uint32_t rho = leading_zeros32(w) + 1;
    if (rho > 63) {
        rho = 63;
    }
    set_reg_max(ind, static_cast<uint8_t>(rho));
}

double HyperLogLogPacked::alpha_m(const uint32_t m) {
    if (m == 16) {
        return 0.673;
    }
    if (m == 32) {
        return 0.697;
    }
    if (m == 64) {
        return 0.709;
    }
    return 0.7213 / (1.0 + 1.079 / static_cast<double>(m));
}

double HyperLogLogPacked::estimate() const {
    double inv_sum = 0.0;
    uint32_t V = 0;

    for (uint32_t i = 0; i < m_; ++i) {
        const uint8_t r = get_reg(i);
        inv_sum += std::ldexp(1.0, -static_cast<int>(r));
        if (r == 0) {
            ++V;
        }
    }

    const double E = alpha_m(m_) * static_cast<double>(m_) * static_cast<double>(m_) / inv_sum;

    if (E <= 2.5 * static_cast<double>(m_)) {
        if (V != 0) {
            return static_cast<double>(m_) * std::log(static_cast<double>(m_) / static_cast<double>(V));
        }
        return E;
    }

    constexpr double two32 = 4294967296.0;
    if (E > (two32 / 30.0)) {
        double ratio = E / two32;
        if (ratio >= 1.0) {
            ratio = 0.999999999;
        }
        return -two32 * std::log(1.0 - ratio);
    }
    return E;
}
