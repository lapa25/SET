#include "StringSortTester.h"

#include <algorithm>
#include <chrono>
#include <iostream>

std::array<int, 256> StringSortTester::buildRank() {
    std::array<int, 256> rank{};
    rank.fill(0);

    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-";

    std::sort(alphabet.begin(), alphabet.end());
    alphabet.erase(std::unique(alphabet.begin(), alphabet.end()), alphabet.end());

    for (int i = 0; i < alphabet.size(); ++i) {
        rank[alphabet[i]] = i + 1;
    }

    return rank;
}

int StringSortTester::charRank(const std::string &s, int pos) {
    static std::array<int, 256> rank = buildRank();

    if (pos >= s.size()) {
        return 0;
    }

    return rank[s[pos]];
}

StringSortTester::CompareResult StringSortTester::lcpCompare(const std::string &a, const std::string &b,
                                                             int start, Metrics &metrics) {
    int i = start;
    int minLen = std::min(a.size(), b.size());

    while (i < minLen) {
        ++metrics.charComparisons;

        if (a[i] < b[i]) {
            return {-1, i};
        }

        if (a[i] > b[i]) {
            return {1, i};
        }
        ++i;
    }

    if (a.size() < b.size()) {
        return {-1, i};
    }

    if (a.size() > b.size()) {
        return {1, i};
    }

    return {0, i};
}

int StringSortTester::compareStrings(const std::string &a, const std::string &b, Metrics &metrics) {
    return lcpCompare(a, b, 0, metrics).cmp;
}

void StringSortTester::standardQuickSortRec(std::vector<std::string> &a, int left, int right, Metrics &metrics) {
    if (left >= right) {
        return;
    }
    int mid = left + (right - left) / 2;
    std::swap(a[left], a[mid]);

    std::string pivot = a[left];

    int lt = left;
    int gt = right;
    int i = left + 1;

    while (i <= gt) {
        int cmp = compareStrings(a[i], pivot, metrics);

        if (cmp < 0) {
            std::swap(a[lt], a[i]);
            ++lt;
            ++i;
        } else if (cmp > 0) {
            std::swap(a[i], a[gt]);
            --gt;
        } else {
            ++i;
        }
    }
    standardQuickSortRec(a, left, lt - 1, metrics);
    standardQuickSortRec(a, gt + 1, right, metrics);
}

void StringSortTester::standardMergeSortRec(std::vector<std::string> &a, int left, int right,
                                            std::vector<std::string> &buffer, Metrics &metrics) {
    if (left >= right) {
        return;
    }
    int mid = left + (right - left) / 2;

    standardMergeSortRec(a, left, mid, buffer, metrics);
    standardMergeSortRec(a, mid + 1, right, buffer, metrics);

    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (compareStrings(a[i], a[j], metrics) <= 0) {
            buffer[k] = a[i];
            ++i;
        } else {
            buffer[k] = a[j];
            ++j;
        }
        ++k;
    }

    while (i <= mid) {
        buffer[k] = a[i];
        ++i;
        ++k;
    }

    while (j <= right) {
        buffer[k] = a[j];
        ++j;
        ++k;
    }

    for (int t = left; t <= right; ++t) {
        a[t] = buffer[t];
    }
}

void StringSortTester::stringQuickSortRec(std::vector<std::string> &a, int left, int right, int depth,
                                          Metrics &metrics) {
    if (left >= right) {
        return;
    }
    int mid = left + (right - left) / 2;
    std::swap(a[left], a[mid]);

    int pivot = charRank(a[left], depth);

    int lt = left;
    int gt = right;
    int i = left + 1;

    while (i <= gt) {
        ++metrics.charComparisons;

        int current = charRank(a[i], depth);

        if (current < pivot) {
            std::swap(a[lt], a[i]);
            ++lt;
            ++i;
        } else if (current > pivot) {
            std::swap(a[i], a[gt]);
            --gt;
        } else {
            ++i;
        }
    }
    stringQuickSortRec(a, left, lt - 1, depth, metrics);

    if (pivot > 0) {
        stringQuickSortRec(a, lt, gt, depth + 1, metrics);
    }

    stringQuickSortRec(a, gt + 1, right, depth, metrics);
}

std::vector<StringSortTester::LcpItem> StringSortTester::mergeLcpVectors(const std::vector<LcpItem> &leftPart,
                                                                         const std::vector<LcpItem> &rightPart,
                                                                         Metrics &metrics) {
    std::vector<LcpItem> result;
    result.reserve(leftPart.size() + rightPart.size());

    int i = 0;
    int j = 0;

    int lcpLeft = 0;
    int lcpRight = 0;

    bool hasPrevious = false;

    while (i < leftPart.size() && j < rightPart.size()) {
        if (lcpLeft > lcpRight) {
            int oldLcpRight = lcpRight;

            result.push_back({leftPart[i].value, hasPrevious ? lcpLeft : 0});
            hasPrevious = true;
            ++i;

            lcpRight = oldLcpRight;

            if (i < leftPart.size()) {
                lcpLeft = leftPart[i].lcpWithPrevious;
            }
        } else if (lcpLeft < lcpRight) {
            int oldLcpLeft = lcpLeft;

            result.push_back({rightPart[j].value, hasPrevious ? lcpRight : 0});
            hasPrevious = true;
            ++j;

            lcpLeft = oldLcpLeft;

            if (j < rightPart.size()) {
                lcpRight = rightPart[j].lcpWithPrevious;
            }
        } else {
            CompareResult cmp = lcpCompare(leftPart[i].value, rightPart[j].value, lcpLeft, metrics);

            if (cmp.cmp <= 0) {
                result.push_back({leftPart[i].value, hasPrevious ? lcpLeft : 0});
                hasPrevious = true;
                ++i;

                lcpRight = cmp.lcp;

                if (i < leftPart.size()) {
                    lcpLeft = leftPart[i].lcpWithPrevious;
                }
            } else {
                result.push_back({rightPart[j].value, hasPrevious ? lcpRight : 0});
                hasPrevious = true;
                ++j;

                lcpLeft = cmp.lcp;

                if (j < rightPart.size()) {
                    lcpRight = rightPart[j].lcpWithPrevious;
                }
            }
        }
    }

    while (i < leftPart.size()) {
        result.push_back({leftPart[i].value, hasPrevious ? lcpLeft : 0});
        hasPrevious = true;
        ++i;

        if (i < leftPart.size()) {
            lcpLeft = leftPart[i].lcpWithPrevious;
        }
    }

    while (j < rightPart.size()) {
        result.push_back({rightPart[j].value, hasPrevious ? lcpRight : 0});
        hasPrevious = true;
        ++j;

        if (j < rightPart.size()) {
            lcpRight = rightPart[j].lcpWithPrevious;
        }
    }

    return result;
}

std::vector<StringSortTester::LcpItem> StringSortTester::stringMergeSortRec(const std::vector<std::string> &a,
                                                                            int left, int right, Metrics &metrics) {
    if (left == right) {
        return {{a[left], 0}};
    }

    int mid = left + (right - left) / 2;

    std::vector<LcpItem> leftPart = stringMergeSortRec(a, left, mid, metrics);
    std::vector<LcpItem> rightPart = stringMergeSortRec(a, mid + 1, right, metrics);

    return mergeLcpVectors(leftPart, rightPart, metrics);
}

void StringSortTester::msdRadixSortRec(std::vector<std::string> &a, int left, int right,
                                       int depth, std::vector<std::string> &buffer, Metrics &metrics,
                                       bool useQuickSwitch) {
    const int alphabetSize = 74;

    if (left >= right) {
        return;
    }

    if (useQuickSwitch && right - left + 1 < alphabetSize) {
        stringQuickSortRec(a, left, right, depth, metrics);
        return;
    }

    std::vector<int> count(alphabetSize + 2, 0);

    for (int i = left; i <= right; ++i) {
        ++metrics.charComparisons;

        int c = charRank(a[i], depth);
        ++count[c + 1];
    }

    for (int r = 0; r <= alphabetSize; ++r) {
        count[r + 1] += count[r];
    }

    std::vector<int> start = count;

    for (int i = left; i <= right; ++i) {
        ++metrics.charComparisons;

        int c = charRank(a[i], depth);
        buffer[count[c]] = a[i];
        ++count[c];
    }

    for (int i = left; i <= right; ++i) {
        a[i] = buffer[i - left];
    }

    for (int r = 1; r <= alphabetSize; ++r) {
        int bucketLeft = left + start[r];
        int bucketRight = left + start[r + 1] - 1;

        msdRadixSortRec(a, bucketLeft, bucketRight, depth + 1, buffer, metrics, useQuickSwitch);
    }
}

void StringSortTester::standardQuickSort(std::vector<std::string> &a, Metrics &metrics) const {
    if (!a.empty()) {
        standardQuickSortRec(a, 0, a.size() - 1, metrics);
    }
}

void StringSortTester::standardMergeSort(std::vector<std::string> &a, Metrics &metrics) const {
    if (a.empty()) {
        return;
    }

    std::vector<std::string> buffer(a.size());
    standardMergeSortRec(a, 0, a.size() - 1, buffer, metrics);
}

void StringSortTester::stringQuickSort(std::vector<std::string> &a, Metrics &metrics) const {
    if (!a.empty()) {
        stringQuickSortRec(a, 0, a.size() - 1, 0, metrics);
    }
}

void StringSortTester::stringMergeSort(std::vector<std::string> &a, Metrics &metrics) const {
    if (a.empty()) {
        return;
    }
    std::vector<LcpItem> sorted = stringMergeSortRec(a, 0, a.size() - 1, metrics);

    for (int i = 0; i < a.size(); ++i) {
        a[i] = sorted[i].value;
    }
}

void StringSortTester::msdRadixSort(std::vector<std::string> &a, Metrics &metrics) const {
    if (a.empty()) {
        return;
    }

    std::vector<std::string> buffer(a.size());
    msdRadixSortRec(a, 0, a.size() - 1, 0, buffer, metrics, false);
}

void StringSortTester::msdRadixSortWithQuickSwitch(std::vector<std::string> &a, Metrics &metrics) const {
    if (a.empty()) {
        return;
    }

    std::vector<std::string> buffer(a.size());
    msdRadixSortRec(a, 0, a.size() - 1, 0, buffer, metrics, true);
}

TestResult StringSortTester::measure(const std::string &algorithm, const std::vector<std::string> &data,
                                     int repeats) const {
    double totalTime = 0.0;
    long long totalComparisons = 0;

    for (int run = 0; run < repeats; ++run) {
        std::vector<std::string> a = data;
        Metrics metrics;

        auto start = std::chrono::steady_clock::now();

        if (algorithm == "quick_sort") {
            standardQuickSort(a, metrics);
        } else if (algorithm == "merge_sort") {
            standardMergeSort(a, metrics);
        } else if (algorithm == "string_quick_sort") {
            stringQuickSort(a, metrics);
        } else if (algorithm == "string_merge_sort") {
            stringMergeSort(a, metrics);
        } else if (algorithm == "msd_radix_sort") {
            msdRadixSort(a, metrics);
        } else if (algorithm == "msd_radix_sort_with_quick") {
            msdRadixSortWithQuickSwitch(a, metrics);
        }

        auto finish = std::chrono::steady_clock::now();

        if (!std::is_sorted(a.begin(), a.end())) {
            std::cerr << "Sort error in " << algorithm << '\n';
        }

        std::chrono::duration<double, std::micro> diff = finish - start;

        totalTime += diff.count();
        totalComparisons += metrics.charComparisons;
    }

    TestResult result;
    result.avgTimeUs = totalTime / repeats;
    result.avgCharComparisons = totalComparisons / repeats;

    return result;
}
