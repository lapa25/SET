#include "random_stream_gen.h"
#include <algorithm>
#include <cmath>

constexpr char alphabet[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "-";

RandomStreamGen::RandomStreamGen(const Params& p) : params_(p), rng_(p.seed) {
    //генерирация словаря строк из universe_size случайных строк
    universe_.reserve(params_.universe_size);
    for (std::size_t i = 0; i < params_.universe_size; ++i) {
        universe_.push_back(random_string());
    }
}
//генерация случайной строки
std::string RandomStreamGen::random_string() {
    //случайная длина в диапазонем[min_len, max_len]
    std::uniform_int_distribution<int> len_dist(static_cast<int>(params_.min_len),
        static_cast<int>(params_.max_len));
    //случайный символ из алфавита
    std::uniform_int_distribution<int> ch_dist(0, sizeof(alphabet) - 2);

    const int l = len_dist(rng_);
    std::string s;
    s.reserve(l);
    for (int i = 0; i < l; ++i) {
        s.push_back(alphabet[ch_dist(rng_)]);
    }
    return s;
}

std::vector<uint32_t> RandomStreamGen::generate_stream_indices() {
    //генерируем поток как набор индексов из словаря
    std::vector<uint32_t> ind;
    ind.reserve(params_.stream_len);

    std::uniform_int_distribution<uint32_t> u(0, static_cast<uint32_t>(params_.universe_size) - 1);
    for (std::size_t i = 0; i < params_.stream_len; ++i) {
        ind.push_back(u(rng_));
    }
    return ind;
}

//поток в строковом представлении
std::vector<std::string> RandomStreamGen::materialize_stream(const std::vector<uint32_t>& ind) const {
    std::vector<std::string> s;
    s.reserve(ind.size());
    for (const auto v : ind) {
        s.push_back(universe_[v]);
    }
    return s;
}

std::vector<std::size_t> RandomStreamGen::prefix_lengths(const std::size_t stream_len, const std::size_t steps) {
    //набор длин префикса для t: t1 < t2 < ... <= stream_len.
    std::vector<std::size_t> lens;
    if (steps == 0) {
        return lens;
    }
    lens.reserve(steps);
    for (std::size_t i = 1; i <= steps; ++i) {
        const double frac = static_cast<double>(i) / static_cast<double>(steps);
        auto len = static_cast<std::size_t>(lround(frac *  static_cast<double>(stream_len) + 0.5));
        len = std::min(len, stream_len);

        // гарантируем строгий рост
        if (!lens.empty() && len <= lens.back()) {
            continue;
        }
        lens.push_back(len);
    }

    if (lens.empty() || lens.back() != stream_len) {
        lens.push_back(stream_len);
    }
    return lens;
}
