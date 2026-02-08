#ifndef SET5_HYPERLOGLOG_PACKED_H
#define SET5_HYPERLOGLOG_PACKED_H

#include <vector>
#include <string>
#include "hash_func_gen.h"

//оптимизация памяти - регистры упакованы по 6 бит, потому что rho в нашей реализации ограничен 63, что
//помещается в 6 бит, а так алгоритм тот же самый
class HyperLogLogPacked {
public:
    explicit HyperLogLogPacked(uint8_t p);

    void reset();
    void add_hash(uint32_t h);
    void add(const std::string& s, const HashFuncGen& hf) { add_hash(hf(s)); }

    [[nodiscard]] double estimate() const;

    [[nodiscard]] uint8_t p() const {
        return p_;
    }
    [[nodiscard]] uint32_t m() const {
        return m_;
    }

    //память под packed-данные (в байтах)
    [[nodiscard]] std::size_t memory_bytes() const {
        return data_.size();
    }

private:
    uint8_t p_;
    uint32_t m_;

    //хранилище packed-значений (битовый массив)
    std::vector<uint8_t> data_;

    //получить/обновить 6-битный регистр
    [[nodiscard]] uint8_t get_reg(uint32_t ind) const;
    void set_reg_max(uint32_t idx, uint8_t val);

    static double alpha_m(uint32_t m);
    static inline uint32_t leading_zeros32(uint32_t x);
};


#endif