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

  std::ofstream standardRandomFile("quick_standard_random.txt");
  std::ofstream standardReverseFile("quick_standard_reverse.txt");
  std::ofstream standardAlmostFile("quick_standard_almost.txt");

  std::ofstream hybridRandomFile("quick_hybrid_random.txt");
  std::ofstream hybridReverseFile("quick_hybrid_reverse.txt");
  std::ofstream hybridAlmostFile("quick_hybrid_almost.txt");

  const size_t NUM_RUNS = 5;

  for (size_t size = MIN_SIZE; size <= MAX_SIZE; size += STEP) {
    uint64_t totalStandardRandom = 0, totalStandardReverse = 0,
             totalStandardAlmost = 0;
    uint64_t totalHybridRandom = 0, totalHybridReverse = 0,
             totalHybridAlmost = 0;

    for (int run = 0; run < NUM_RUNS; ++run) {
      std::vector<int> randomi(random.begin(), random.begin() + size);
      totalStandardRandom += SortTester::timeStandard(randomi);

      std::vector<int> randomih(random.begin(), random.begin() + size);
      totalHybridRandom += SortTester::timeHybrid(randomih);

      std::vector<int> reversei(reverse.begin(), reverse.begin() + size);
      totalStandardReverse += SortTester::timeStandard(reversei);

      std::vector<int> reverseih(reverse.begin(), reverse.begin() + size);
      totalHybridReverse += SortTester::timeHybrid(reverseih);

      std::vector<int> almosti(almostSorted.begin(),
                               almostSorted.begin() + size);
      totalStandardAlmost += SortTester::timeStandard(almosti);

      std::vector<int> almostih(almostSorted.begin(),
                                almostSorted.begin() + size);
      totalHybridAlmost += SortTester::timeHybrid(almostih);
    }

    standardRandomFile << size << " " << totalStandardRandom / NUM_RUNS << "\n";
    standardReverseFile << size << " " << totalStandardReverse / NUM_RUNS
                        << "\n";
    standardAlmostFile << size << " " << totalStandardAlmost / NUM_RUNS << "\n";

    hybridRandomFile << size << " " << totalHybridRandom / NUM_RUNS << "\n";
    hybridReverseFile << size << " " << totalHybridReverse / NUM_RUNS << "\n";
    hybridAlmostFile << size << " " << totalHybridAlmost / NUM_RUNS << "\n";
  }

  standardRandomFile.close();
  standardReverseFile.close();
  standardAlmostFile.close();

  hybridRandomFile.close();
  hybridReverseFile.close();
  hybridAlmostFile.close();
}