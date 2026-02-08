#include "hash_func_gen.h"

#include <random>

HashFuncGen::HashFuncGen(const uint64_t seed) {
    //выбираем a и b случайно
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<uint64_t> dist(0, P - 1);

    a_ = dist(rng);
    b_ = dist(rng);
    if (a_ == 0) { //а не 0, т к тогда константа
        a_ = 1;
    }
}

uint32_t HashFuncGen::operator()(const std::string_view s) const {
    const uint64_t x = PolyHashModP(s);
    const uint64_t y = (MulMod(a_, x) + b_) % P;
    return static_cast<uint32_t>(y);
}

uint64_t HashFuncGen::PolyHashModP(const std::string_view s) {
    uint64_t x = 0;
    for (const unsigned char ch : s) {
        x = (MulMod(x, B) + static_cast<uint64_t>(ch)) % P;
    }
    return x;
}

uint64_t HashFuncGen::MulMod(const uint64_t a, const uint64_t b) {
    const __uint128_t prod = static_cast<__uint128_t>(a) * static_cast<__uint128_t>(b);
    return static_cast<uint64_t>(prod % P);
}

