#ifndef STRING_SORT_TESTER_H
#define STRING_SORT_TESTER_H

#include "Metrics.h"

#include <array>
#include <string>
#include <vector>

class StringSortTester {
    struct CompareResult {
        int cmp = 0;
        int lcp = 0;
    };

    struct LcpItem {
        std::string value;
        int lcpWithPrevious = 0;
    };

    static std::array<int, 256> buildRank();

    static int charRank(const std::string &s, int pos);

    static CompareResult lcpCompare(
        const std::string &a,
        const std::string &b,
        int start,
        Metrics &metrics
    );

    static int compareStrings(
        const std::string &a,
        const std::string &b,
        Metrics &metrics
    );

    static void standardQuickSortRec(
        std::vector<std::string> &a,
        int left,
        int right,
        Metrics &metrics
    );

    static void standardMergeSortRec(
        std::vector<std::string> &a,
        int left,
        int right,
        std::vector<std::string> &buffer,
        Metrics &metrics
    );

    static void stringQuickSortRec(
        std::vector<std::string> &a,
        int left,
        int right,
        int depth,
        Metrics &metrics
    );

    static std::vector<LcpItem> stringMergeSortRec(
        const std::vector<std::string> &a,
        int left,
        int right,
        Metrics &metrics
    );

    static std::vector<LcpItem> mergeLcpVectors(
        const std::vector<LcpItem> &leftPart,
        const std::vector<LcpItem> &rightPart,
        Metrics &metrics
    );

    static void msdRadixSortRec(
        std::vector<std::string> &a,
        int left,
        int right,
        int depth,
        std::vector<std::string> &buffer,
        Metrics &metrics,
        bool useQuickSwitch
    );

public:
    void standardQuickSort(std::vector<std::string> &a, Metrics &metrics) const;

    void standardMergeSort(std::vector<std::string> &a, Metrics &metrics) const;

    void stringQuickSort(std::vector<std::string> &a, Metrics &metrics) const;

    void stringMergeSort(std::vector<std::string> &a, Metrics &metrics) const;

    void msdRadixSort(std::vector<std::string> &a, Metrics &metrics) const;

    void msdRadixSortWithQuickSwitch(std::vector<std::string> &a, Metrics &metrics) const;

    TestResult measure(
        const std::string &algorithm,
        const std::vector<std::string> &data,
        int repeats
    ) const;
};

#endif
