#ifndef STRING_GENERATOR_H
#define STRING_GENERATOR_H

#include <cstddef>
#include <random>
#include <string>
#include <vector>

class StringGenerator {
public:
    struct Params {
        std::size_t maxSize = 3000;
        std::size_t minLen = 10;
        std::size_t maxLen = 200;
        unsigned int seed = 12345;
    };

    StringGenerator();
    explicit StringGenerator(const Params& params);

    std::string randomString();

    std::vector<std::string> randomArray(std::size_t n);
    std::vector<std::string> reversedArray(const std::vector<std::string>& base);
    std::vector<std::string> almostSortedArray(const std::vector<std::string>& base, std::size_t swapsCount);
    std::vector<std::string> commonPrefixArray(std::size_t n, std::size_t prefixLen);

private:
    Params params_;
    std::mt19937 rng_;
    std::string alphabet_;
};

#endif