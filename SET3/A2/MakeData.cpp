#include "ArrayGenerator.cpp"
#include "SortTester.cpp"
#include <fstream>

int main() {
  ArrayGenerator generator;
  const size_t MAX_SIZE = 100000;
  const size_t MIN_SIZE = 500;
  const size_t STEP = 100;
  const size_t RANDOM_MIN = 0;
  const size_t RANDOM_MAX = 6000;
  const size_t SWAP_COUNT = 100;

  std::vector<int> random = generator.random(MAX_SIZE, RANDOM_MIN, RANDOM_MAX);
  std::vector<int> reverse = generator.reverse(MAX_SIZE);
  std::vector<int> almostSorted = generator.almostSorted(MAX_SIZE, SWAP_COUNT);

  std::vector<int> thresholds = {200, 100, 50, 25, 10, 5};

  std::ofstream standardRandomFile("standard_random.txt");
  std::ofstream standardReverseFile("standard_reverse.txt");
  std::ofstream standardAlmostFile("standard_almost.txt");

  std::vector<std::ofstream> hybridRandomFiles;
  std::vector<std::ofstream> hybridReverseFiles;
  std::vector<std::ofstream> hybridAlmostFiles;

  for (int threshold : thresholds) {
    hybridRandomFiles.emplace_back("hybrid_random_" +
                                   std::to_string(threshold) + ".txt");
    hybridReverseFiles.emplace_back("hybrid_reverse_" +
                                    std::to_string(threshold) + ".txt");
    hybridAlmostFiles.emplace_back("hybrid_almost_" +
                                   std::to_string(threshold) + ".txt");
  }

  for (size_t size = MIN_SIZE; size <= MAX_SIZE; size += STEP) {
    int s = static_cast<int>(size);
    std::vector<int> randomi(random.begin(), random.begin() + s);
    uint64_t timeRandomStandard = SortTester::timeStandard(randomi);
    standardRandomFile << size << " " << timeRandomStandard << "\n";

    std::vector<int> reversei(reverse.begin(), reverse.begin() + s);
    uint64_t timeReverseStandard = SortTester::timeStandard(reversei);
    standardReverseFile << size << " " << timeReverseStandard << "\n";

    std::vector<int> almosti(almostSorted.begin(), almostSorted.begin() + s);
    uint64_t timeAlmostStandard = SortTester::timeStandard(almosti);
    standardAlmostFile << size << " " << timeAlmostStandard << "\n";

    for (size_t i = 0; i < thresholds.size(); ++i) {
      std::vector<int> randomih(random.begin(), random.begin() + s);
      uint64_t timeRandomHybrid =
          SortTester::timeHybrid(randomih, thresholds[i]);
      hybridRandomFiles[i] << size << " " << timeRandomHybrid << "\n";

      std::vector<int> reverseih(reverse.begin(), reverse.begin() + s);
      uint64_t timeReverseHybrid =
          SortTester::timeHybrid(reverseih, thresholds[i]);
      hybridReverseFiles[i] << size << " " << timeReverseHybrid << "\n";

      std::vector<int> almostih(almostSorted.begin(), almostSorted.begin() + s);
      uint64_t timeAlmostHybrid =
          SortTester::timeHybrid(almostih, thresholds[i]);
      hybridAlmostFiles[i] << size << " " << timeAlmostHybrid << "\n";
    }
  }

  standardRandomFile.close();
  standardReverseFile.close();
  standardAlmostFile.close();

  for (auto &file : hybridRandomFiles) {
    file.close();
  }
  for (auto &file : hybridReverseFiles) {
    file.close();
  }
  for (auto &file : hybridAlmostFiles) {
    file.close();
  }
}