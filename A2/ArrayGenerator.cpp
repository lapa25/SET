#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

class ArrayGenerator {
  std::mt19937 gen;

public:
  ArrayGenerator() : gen(std::random_device{}()) {}

  std::vector<int> random(size_t size, int minVal, int maxVal) {
    std::uniform_int_distribution<int> dist(minVal, maxVal);
    std::vector<int> arr(size);
    for (size_t i = 0; i < size; ++i) {
      arr[i] = dist(gen);
    }
    return arr;
  }

  std::vector<int> reverse(size_t size) {
    std::vector<int> arr(size);
    for (size_t i = 0; i < size; ++i) {
      arr[i] = size - i;
    }
    return arr;
  }

  std::vector<int> almostSorted(size_t size, size_t swaps) {
    std::vector<int> arr(size);
    for (size_t i = 0; i < size; ++i) {
      arr[i] = i + 1;
    }
    std::uniform_int_distribution<int> dist(0, size - 1);
    for (size_t i = 0; i < swaps; ++i) {
      int ind1 = dist(gen);
      int ind2 = dist(gen);
      std::swap(arr[ind1], arr[ind2]);
    }
    return arr;
  }
};