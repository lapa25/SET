#ifndef METRICS_H
#define METRICS_H

#include <cstdint>

struct Metrics {
    int64_t charComparisons = 0;
};

struct TestResult {
    double avgTimeUs = 0.0;
    int64_t avgCharComparisons = 0;
};

#endif
