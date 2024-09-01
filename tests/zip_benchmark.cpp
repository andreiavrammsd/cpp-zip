#include <benchmark/benchmark.h>

#include "msd/zip.hpp"

/**
    Results on release build...
 */

static void BM_Zip(benchmark::State&) {}  // NOLINT(google-runtime-references)

BENCHMARK(BM_Zip);  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

// NOLINTNEXTLINE(modernize-avoid-c-arrays,hicpp-no-array-decay,cppcoreguidelines-pro-bounds-array-to-pointer-decay,hicpp-avoid-c-arrays)
BENCHMARK_MAIN();  // NOLINT(cppcoreguidelines-avoid-c-arrays)
