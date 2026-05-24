#include "StringGenerator.h"

#include <algorithm>

StringGenerator::StringGenerator()
    : StringGenerator(Params()) {
}

StringGenerator::StringGenerator(const Params &params)
    : params_(params), rng_(params.seed), alphabet_("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
          "abcdefghijklmnopqrstuvwxyz"
          "0123456789"
          "!@#%:;^&*()-") {
}

std::string StringGenerator::randomString() {
    std::uniform_int_distribution<int> lenDist(
        static_cast<int>(params_.minLen),
        static_cast<int>(params_.maxLen)
    );

    std::uniform_int_distribution<int> charDist(
        0,
        static_cast<int>(alphabet_.size()) - 1
    );

    int len = lenDist(rng_);

    std::string s;
    s.reserve(len);

    for (int i = 0; i < len; ++i) {
        s.push_back(alphabet_[charDist(rng_)]);
    }

    return s;
}

std::vector<std::string> StringGenerator::randomArray(std::size_t n) {
    std::vector<std::string> result;
    result.reserve(n);

    for (std::size_t i = 0; i < n; ++i) {
        result.push_back(randomString());
    }

    return result;
}

std::vector<std::string> StringGenerator::reversedArray(const std::vector<std::string> &base) {
    std::vector<std::string> result = base;

    std::sort(result.begin(), result.end());
    std::reverse(result.begin(), result.end());

    return result;
}

std::vector<std::string> StringGenerator::almostSortedArray(const std::vector<std::string> &base,
    std::size_t swapsCount) {
    std::vector<std::string> result = base;

    std::sort(result.begin(), result.end());

    if (result.empty()) {
        return result;
    }

    std::uniform_int_distribution<int> posDist(
        0,
        static_cast<int>(result.size()) - 1
    );

    for (std::size_t i = 0; i < swapsCount; ++i) {
        int first = posDist(rng_);
        int second = posDist(rng_);

        std::swap(result[first], result[second]);
    }

    return result;
}

std::vector<std::string> StringGenerator::commonPrefixArray(std::size_t n, std::size_t prefixLen) {
    if (prefixLen + 1 > params_.maxLen) {
        prefixLen = params_.maxLen - 1;
    }

    std::uniform_int_distribution<int> charDist(
        0,
        static_cast<int>(alphabet_.size()) - 1
    );

    std::string prefix;
    prefix.reserve(prefixLen);

    for (std::size_t i = 0; i < prefixLen; ++i) {
        prefix.push_back(alphabet_[charDist(rng_)]);
    }

    std::vector<std::string> result;
    result.reserve(n);

    std::size_t minLen = std::max(params_.minLen, prefixLen + 1);

    std::uniform_int_distribution<int> lenDist(
        static_cast<int>(minLen),
        static_cast<int>(params_.maxLen)
    );

    for (std::size_t i = 0; i < n; ++i) {
        int len = lenDist(rng_);

        std::string s = prefix;

        while (s.size() < len) {
            s.push_back(alphabet_[charDist(rng_)]);
        }

        result.push_back(s);
    }

    return result;
}
