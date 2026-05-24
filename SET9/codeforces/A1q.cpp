#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

std::array<int, 256> buildRank() {
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

int charRank(const std::string &s, int pos) {
    static std::array<int, 256> rank = buildRank();

    if (pos >= s.size()) {
        return 0;
    }

    return rank[s[pos]];
}

void stringQuickSort(std::vector<std::string> &a, int left, int right, int depth) {
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

    stringQuickSort(a, left, lt - 1, depth);

    if (pivot > 0) {
        stringQuickSort(a, lt, gt, depth + 1);
    }

    stringQuickSort(a, gt + 1, right, depth);
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

    if (!a.empty()) {
        stringQuickSort(a, 0, n - 1, 0);
    }

    for (const std::string &s: a) {
        std::cout << s << '\n';
    }
}
