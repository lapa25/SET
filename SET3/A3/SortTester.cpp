#include <chrono>
#include <random>
#include <vector>

void insertionSort(std::vector<int> &arr, int l, int r) {
  for (int i = l + 1; i <= r; ++i) {
    int curr = arr[i];
    int j = i - 1;
    while (j >= l && arr[j] > curr) {
      arr[j + 1] = arr[j];
      --j;
    }
    arr[j + 1] = curr;
  }
}

void heapify(std::vector<int> &heap, int start, int end, int i) {
  int n = end - start + 1;
  int ii = i - start;

  while (2 * ii + 1 < n) {
    int j = 2 * ii + 1;
    if ((j + 1 < n) && (heap[start + j] < heap[start + j + 1])) {
      ++j;
    }
    if (heap[start + ii] >= heap[start + j]) {
      break;
    }
    std::swap(heap[start + ii], heap[start + j]);
    ii = j;
  }
}

void buildMaxHeap(std::vector<int> &heap, int start, int end) {
  int n = end - start + 1;
  for (int k = n / 2 - 1; k >= 0; --k) {
    heapify(heap, start, end, start + k);
  }
}

void heapSort(std::vector<int> &a, int l, int r) {
  if (l >= r) {
    return;
  }

  buildMaxHeap(a, l, r);

  for (int i = r; i > l; --i) {
    std::swap(a[l], a[i]);
    heapify(a, l, i - 1, l);
  }
}

int partition(std::vector<int> &arr, int l, int r) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(l, r);

  int ind = dist(gen);
  std::swap(arr[ind], arr[r]);

  int pivot = arr[r];
  int i = l - 1;

  for (int j = l; j < r; ++j) {
    if (arr[j] <= pivot) {
      ++i;
      std::swap(arr[i], arr[j]);
    }
  }
  std::swap(arr[i + 1], arr[r]);
  return i + 1;
}

void introSort(std::vector<int> &arr, int l, int r, int limit) {
  if (l >= r) {
    return;
  }

  if (r - l + 1 <= 16) {
    insertionSort(arr, l, r);
    return;
  }

  if (limit == 0) {
    heapSort(arr, l, r);
    return;
  }

  int pivot = partition(arr, l, r);
  introSort(arr, l, pivot - 1, limit - 1);
  introSort(arr, pivot + 1, r, limit - 1);
}

void quickSortStandard(std::vector<int> &arr, int l, int r) {
  if (l < r) {
    int pivot = partition(arr, l, r);
    quickSortStandard(arr, l, pivot - 1);
    quickSortStandard(arr, pivot + 1, r);
  }
}

class SortTester {
public:
  static uint64_t timeStandard(std::vector<int> &arr) {
    auto start = std::chrono::high_resolution_clock::now();
    quickSortStandard(arr, 0, arr.size() - 1);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    uint64_t time1 =
        std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    return time1;
  }

  static uint64_t timeHybrid(std::vector<int> &arr) {
    auto start = std::chrono::high_resolution_clock::now();
    int limit = 2 * static_cast<int>(log2(arr.size()));
    introSort(arr, 0, arr.size() - 1, limit);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    uint64_t time1 =
        std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    return time1;
  }
};