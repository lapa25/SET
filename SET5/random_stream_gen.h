#ifndef SET_RANDOM_STREAM_GEN_H
#define SET_RANDOM_STREAM_GEN_H

#include <cstddef>
#include <random>
#include <string>
#include <vector>

//генерация словаря уникальных строк, а потом генерация индексов чтобы доставать какие-то слова из словаря
//и получать повторы для более точного тестирования гиперлоглога
class RandomStreamGen {
public:
    struct Params {
        std::size_t stream_len = 200000;      //длина потока
        std::size_t universe_size = 50000;    //размер словаря
        std::size_t min_len = 1;              //мин длина строки
        std::size_t max_len = 30;             //макс длина строки
        uint64_t seed = 1;
    };

    explicit RandomStreamGen(const Params& p);

    //словарь строк
    [[nodiscard]] const std::vector<std::string>& universe() const {
        return universe_;
    }

    //поток как массив индексов
    std::vector<uint32_t> generate_stream_indices();

    //индексы в строки
    [[nodiscard]] std::vector<std::string> materialize_stream(const std::vector<uint32_t>& ind) const;

    //список длин префиксов для моментов времени t
    static std::vector<std::size_t> prefix_lengths(std::size_t stream_len, std::size_t steps);

private:
    Params params_;
    std::mt19937_64 rng_;
    std::vector<std::string> universe_;

    //генерация одной случайной строки
    std::string random_string();
};


#endif