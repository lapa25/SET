#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

const int ALPHABET_SIZE = 74;

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

void msdRadixSort(std::vector<std::string> &a, int left, int right, int depth, std::vector<std::string> &buffer) {
    if (left >= right) {
        return;
    }
    std::vector<int> count(ALPHABET_SIZE + 2, 0);

    for (int i = left; i <= right; ++i) {
        int c = charRank(a[i], depth);
        ++count[c + 1];
    }

    for (int r = 0; r <= ALPHABET_SIZE; ++r) {
        count[r + 1] += count[r];
    }

    std::vector<int> start = count;

    for (int i = left; i <= right; ++i) {
        int c = charRank(a[i], depth);
        buffer[count[c]] = a[i];
        ++count[c];
    }

    for (int i = left; i <= right; ++i) {
        a[i] = buffer[i - left];
    }

    for (int r = 1; r <= ALPHABET_SIZE; ++r) {
        int bucketLeft = left + start[r];
        int bucketRight = left + start[r + 1] - 1;

        msdRadixSort(a, bucketLeft, bucketRight, depth + 1, buffer);
    }
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
        std::vector<std::string> buffer(a.size());
        msdRadixSort(a, 0, n - 1, 0, buffer);
    }

    for (const std::string &s: a) {
        std::cout << s << '\n';
    }

    return 0;
}
