#include "hyperloglog.h"
#include <cmath>
#include <algorithm>

HyperLogLog::HyperLogLog(const uint8_t p) : p_(p), m_(1u << p), M_(m_, 0) {}

void HyperLogLog::reset() {
    std::ranges::fill(M_, 0);
}

inline uint32_t HyperLogLog::leading_zeros32(uint32_t x) {
    if (x == 0) {
        return 32;
    }
    return static_cast<uint32_t>(__builtin_clz(x));
}

void HyperLogLog::add_hash(const uint32_t h) {
    //индекс регистра по старшим p битам
    const uint32_t ind = h >> (32 - p_);

    //rho = позиция первой единицы (1 + число ведущих нулей).
    const uint32_t w = (h << p_) | (1u << (p_ - 1));
    uint32_t rho = leading_zeros32(w) + 1;

    if (rho > 63) {
        rho = 63;
    }

    //в регистре храним максимум наблюдённых rho
    if (rho > M_[ind]) {
        M_[ind] = static_cast<uint8_t>(rho);
    }
}

double HyperLogLog::alpha_m(const uint32_t m) {
    //стандартные константы HyperLogLog
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

double HyperLogLog::estimate() const {
    //E = alpha(m) * m^2 / sum(2^{-M[i]})
    double inv_sum = 0.0;
    uint32_t V = 0; // число нулевых регистров

    for (uint32_t i = 0; i < m_; ++i) {
        inv_sum += std::ldexp(1.0, -static_cast<int>(M_[i]));
        if (M_[i] == 0) {
            ++V;
        }
    }

    const double E = alpha_m(m_) * static_cast<double>(m_) * static_cast<double>(m_) / inv_sum;

    //если E <= 2.5m и есть нулевые регистры, используем m*ln(m/V) - linear counting
    if (E <= 2.5 * static_cast<double>(m_)) {
        if (V != 0) {
            const double H = static_cast<double>(m_) * std::log(static_cast<double>(m_) /
                static_cast<double>(V));
            return H;
        }
        return E;
    }

    //если E > 2^32/30, используем -2^32 * ln(1 - E/2^32)
    if (constexpr double two32 = 4294967296.0; E > (two32 / 30.0)) {
        double ratio = E / two32;
        if (ratio >= 1.0) {
            ratio = 0.999999999;
        }
        return -two32 * std::log(1.0 - ratio);
    }

    return E;
}
