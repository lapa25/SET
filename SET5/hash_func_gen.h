#ifndef SET5_HASH_FUNC_GEN_H
#define SET5_HASH_FUNC_GEN_H

#include <string_view>

// 1.переводим строку в число x по модулю простого P
//    с помощью полиномиального  хеша:
//       x = (((c0)*B + c1)*B + c2 ... ) mod P
//2. применяем универсальное линейное хеширование:
//       y = (a*x + b) mod P
//    где a и b выбираются случайно при создании HashFuncGen.
//3. возвращаем 32-битное значение: uint32_t(y)
class HashFuncGen {
public:
    explicit HashFuncGen(uint64_t seed = 0x52ULL);

    //посчитать 32-битный хеш строки
    uint32_t operator()(std::string_view s) const;

    static constexpr uint64_t PrimeModulus() {
        return P;
    }
    static constexpr uint64_t Base() {
        return B;
    }

private:
    //простое число чуть больше 2^32
    static constexpr uint64_t P = 4294967311ULL;

    //основание полиномиального хеша
    static constexpr uint64_t B = 911382323ULL;

    uint64_t a_ = 1; //коэффициент при х
    uint64_t b_ = 0; //свободный член

    // Полиномиальный хеш строки по модулю P
    static uint64_t PolyHashModP(std::string_view s);

    // (a*b) mod P
    static uint64_t MulMod(uint64_t a, uint64_t b);
};



#endif