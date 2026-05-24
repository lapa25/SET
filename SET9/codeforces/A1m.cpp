#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct CompareResult {
    int cmp = 0;
    int lcp = 0;
};

struct LcpItem {
    std::string value;
    int lcpWithPrevious = 0;
};

CompareResult lcpCompare(const std::string &a, const std::string &b, int start) {
    int i = start;
    int minLen = std::min(a.size(), b.size());

    while (i < minLen) {
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

std::vector<LcpItem> mergeParts(const std::vector<LcpItem> &leftPart, const std::vector<LcpItem> &rightPart) {
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
            CompareResult cmp = lcpCompare(leftPart[i].value, rightPart[j].value, lcpLeft);

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

std::vector<LcpItem> stringMergeSort(const std::vector<std::string> &a, int left, int right) {
    if (left == right) {
        return {{a[left], 0}};
    }

    int mid = left + (right - left) / 2;

    std::vector<LcpItem> leftPart = stringMergeSort(a, left, mid);
    std::vector<LcpItem> rightPart = stringMergeSort(a, mid + 1, right);

    return mergeParts(leftPart, rightPart);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<std::string> a(n);

    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    if (n > 0) {
        std::vector<LcpItem> sorted = stringMergeSort(a, 0, n - 1);

        for (const LcpItem &item: sorted) {
            std::cout << item.value << '\n';
        }
    }
}
