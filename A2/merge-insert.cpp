#include <iostream>
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

void mergeHybrSort(std::vector<int> &arr, size_t left, size_t right, size_t threshold) {
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

int main() {
  std::ios::sync_with_stdio(false);
  std ::cin.tie(nullptr);

  size_t n = 0;
  std::cin >> n;
  std::vector<int> arr(n);
  for (size_t i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }
  mergeHybrSort(arr, 0, arr.size() - 1, 15);

  for (size_t i = 0; i < n; ++i) {
    std::cout << arr[i] << " ";
  }
}
