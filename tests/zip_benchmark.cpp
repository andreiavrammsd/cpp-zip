#include <benchmark/benchmark.h>

#include "msd/zip.hpp"

/**
    Results on release build...
 */

static void BM_Zip(benchmark::State&) {}

BENCHMARK(BM_Zip);

BENCHMARK_MAIN();
