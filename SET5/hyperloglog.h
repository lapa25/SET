#ifndef SET5_HYPERLOGLOG_H
#define SET5_HYPERLOGLOG_H

#include <vector>
#include <string>
#include "hash_func_gen.h"

class HyperLogLog {
public:
    explicit HyperLogLog(uint8_t p);

    //сбросить регистры
    void reset();

    //добавить элемент по готовому 32-битному хешу
    void add_hash(uint32_t h);

    void add(const std::string& s, const HashFuncGen& hf) {
        add_hash(hf(s));
    }

    //оценка кардинальности N_t
    [[nodiscard]] double estimate() const;

    [[nodiscard]] uint8_t p() const {
        return p_;
    }
    [[nodiscard]] uint32_t m() const {
        return m_;
    }
    [[nodiscard]] const std::vector<uint8_t>& regs() const {
        return M_;
    }

    [[nodiscard]] std::size_t memory_bytes() const {
        return M_.size() * sizeof(uint8_t);
    }

private:
    uint8_t p_;
    uint32_t m_;
    std::vector<uint8_t> M_; // регистры

    //константа alpha(m) из стандартного HLL
    static double alpha_m(uint32_t m);

    //число ведущих нулей в 32-битном числе
    static inline uint32_t leading_zeros32(uint32_t x);
};


#endif