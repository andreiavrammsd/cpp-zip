#include <benchmark/benchmark.h>

#include "msd/zip.hpp"

/**
    Results on release build...
 */

static void BM_Zip(benchmark::State& state) { (void)state; }

BENCHMARK(BM_Zip);

BENCHMARK_MAIN();
