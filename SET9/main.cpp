#include "StringGenerator.h"
#include "StringSortTester.h"

#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

std::vector<std::string> takeFirst(const std::vector<std::string> &a, int n) {
    return {a.begin(), a.begin() + n};
}

int main() {
    const int maxSize = 3000;
    const int step = 100;
    const int repeats = 5;

    StringGenerator::Params params;
    params.maxSize = maxSize;
    params.minLen = 10;
    params.maxLen = 200;
    params.seed = 12345;

    StringGenerator generator(params);

    std::vector<std::string> randomData = generator.randomArray(maxSize);
    std::vector<std::string> reversedData = generator.reversedArray(randomData);
    std::vector<std::string> almostSortedData = generator.almostSortedArray(randomData, 90);
    std::vector<std::string> commonPrefixData = generator.commonPrefixArray(maxSize, 80);

    std::vector<std::pair<std::string, std::vector<std::string> > > datasets = {
        {"random", randomData},
        {"reversed", reversedData},
        {"almost_sorted", almostSortedData},
        {"common_prefix", commonPrefixData}
    };

    std::vector<std::string> algorithms = {
        "quick_sort",
        "merge_sort",
        "string_quick_sort",
        "string_merge_sort",
        "msd_radix_sort",
        "msd_radix_sort_with_quick"
    };

    StringSortTester tester;

    std::ofstream out("results.csv");
    out << "dataset,size,algorithm,time_us,char_comparisons\n";

    for (const auto &dataset: datasets) {
        for (int n = step; n <= maxSize; n += step) {
            std::vector<std::string> currentData = takeFirst(dataset.second, n);

            for (const std::string &algorithm: algorithms) {
                TestResult result = tester.measure(algorithm, currentData, repeats);

                out << dataset.first << ','
                        << n << ','
                        << algorithm << ','
                        << result.avgTimeUs << ','
                        << result.avgCharComparisons << '\n';

                std::cout << dataset.first
                        << " n=" << n
                        << " algorithm=" << algorithm
                        << " done\n";
            }
        }
    }
    std::cout << "Results saved\n";
}
