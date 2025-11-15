#include <chrono>
#include <vector>

void insertionSort(std::vector<int> &arr, size_t l, size_t r) {
  for (size_t i = l + 1; i <= r; ++i) {
    int curr = arr[i];
    int j = static_cast<int>(i) - 1;
    while (j >= static_cast<int>(l) && arr[j] > curr) {
      arr[j + 1] = arr[j];
      --j;
    }
    arr[j + 1] = curr;
  }
}
void merge(std::vector<int> &arr, size_t l, size_t mid, size_t r) {
  std::vector<int> larr(arr.begin() + static_cast<int>(l),
                        arr.begin() + static_cast<int>(mid) + 1);
  std::vector<int> rarr(arr.begin() + static_cast<int>(mid) + 1,
                        arr.begin() + static_cast<int>(r) + 1);

  size_t i = 0;
  size_t j = 0;
  size_t k = l;
  while (i < larr.size() && j < rarr.size()) {
    if (larr[i] <= rarr[j]) {
      arr[k++] = larr[i++];
    } else {
      arr[k++] = rarr[j++];
    }
  }

  while (i < larr.size()) {
    arr[k++] = larr[i++];
  }
  while (j < rarr.size()) {
    arr[k++] = rarr[j++];
  }
}

void mergeSort(std::vector<int> &arr, size_t left, size_t right) {
  if (left < right) {
    size_t mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }
}

void mergeHybrSort(std::vector<int> &arr, size_t left, size_t right,
                   size_t threshold) {
  if ((right - left + 1) <= threshold) {
    insertionSort(arr, left, right);
    return;
  }
  if (left < right) {
    size_t mid = left + (right - left) / 2;
    mergeHybrSort(arr, left, mid, threshold);
    mergeHybrSort(arr, mid + 1, right, threshold);
    merge(arr, left, mid, right);
  }
}

class SortTester {
public:
  static uint64_t timeStandard(std::vector<int> &arr) {
    auto start = std::chrono::high_resolution_clock::now();
    mergeSort(arr, 0, arr.size() - 1);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    uint64_t time1 =
        std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    return time1;
  }

  static uint64_t timeHybrid(std::vector<int> &arr, size_t threshold) {
    auto start = std::chrono::high_resolution_clock::now();
    mergeHybrSort(arr, 0, arr.size() - 1, threshold);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    uint64_t time1 =
        std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    return time1;
  }
};