// Benchmarks with memory tracking for Figure Finder algorithms
#include "finders/figure_finder_factory.hpp"
#include "matrix/matrix.hpp"
#include <benchmark/benchmark.h>
#include <fstream>
#include <sstream>
#include <sys/resource.h>

using namespace figures;

// Memory Tracking Utilities
struct MemoryUsage {
  size_t rss_kb;      // Resident Set Size (actual RAM used)
  size_t vm_size_kb;  // Virtual Memory Size
  size_t peak_rss_kb; // Peak RSS
};

MemoryUsage getCurrentMemoryUsage() {
  MemoryUsage usage{0, 0, 0};

  std::ifstream status("/proc/self/status");
  std::string line;

  while (std::getline(status, line)) {
    std::istringstream iss(line);
    std::string key;
    size_t value;
    std::string unit;

    if (iss >> key >> value >> unit) {
      if (key == "VmRSS:") {
        usage.rss_kb = value;
      } else if (key == "VmSize:") {
        usage.vm_size_kb = value;
      } else if (key == "VmHWM:") { // High Water Mark (peak)
        usage.peak_rss_kb = value;
      }
    }
  }

  return usage;
}

static void BM_CPU(benchmark::State& state, figures::FigureFinderType type) {
  using namespace figures;
  BoolMatrix matrix(1000, 1000);
  for (size_t i = 0; i < 1000; ++i) {
    for (size_t j = 0; j < 1000; ++j) {
      matrix(i, j) = (i + j) % 2;
    }
  }
  auto finder = createFigureFinder(type);
  for (auto _ : state) {
    benchmark::DoNotOptimize(finder->countFigures(matrix));
  }
  state.SetItemsProcessed(state.iterations() * 1000 * 1000);
}

static void RegisterAllCPUBenchmarks() {
  using namespace figures;
  for (auto type : getAllFigureFinderTypes()) {
    benchmark::RegisterBenchmark(("BM_" + toString(type)).c_str(),
                                 [type](benchmark::State& state) { BM_CPU(state, type); })
        ->Unit(benchmark::kMillisecond)
        ->Iterations(10);
  }
}

static void BM_Memory(benchmark::State& state, figures::FigureFinderType type) {
  using namespace figures;
  const size_t rows = state.range(0);
  const size_t cols = state.range(0);
  BoolMatrix matrix(rows, cols, 1);
  auto finder = createFigureFinder(type);
  MemoryUsage before = getCurrentMemoryUsage();
  size_t max_rss_delta = 0;
  for (auto _ : state) {
    state.PauseTiming();
    MemoryUsage start = getCurrentMemoryUsage();
    state.ResumeTiming();
    benchmark::DoNotOptimize(finder->countFigures(matrix));
    state.PauseTiming();
    MemoryUsage end = getCurrentMemoryUsage();
    long long delta = static_cast<long long>(end.rss_kb) - static_cast<long long>(start.rss_kb);
    if (delta > 0) {
      max_rss_delta = std::max(max_rss_delta, static_cast<size_t>(delta));
    }
    state.ResumeTiming();
  }
  MemoryUsage after = getCurrentMemoryUsage();
  long long total_rss_delta =
      static_cast<long long>(after.rss_kb) - static_cast<long long>(before.rss_kb);
  state.counters["RSS_KB"] = benchmark::Counter(total_rss_delta > 0 ? total_rss_delta : 0);
  state.counters["Peak_RSS_KB"] = benchmark::Counter(max_rss_delta);
  state.counters["Cells"] = benchmark::Counter(rows * cols);
  state.counters["Bytes_Per_Cell"] =
      benchmark::Counter((double)(max_rss_delta * 1024) / (rows * cols));
}

static void RegisterAllMemoryBenchmarks() {
  using namespace figures;
  for (auto type : getAllFigureFinderTypes()) {
    benchmark::RegisterBenchmark(("BM_Memory_" + toString(type)).c_str(),
                                 [type](benchmark::State& state) { BM_Memory(state, type); })
        ->Arg(100)
        ->Arg(500)
        ->Arg(1000)
        ->Arg(5000)
        ->Arg(10000)
        ->Unit(benchmark::kMillisecond)
        ->ArgName("Size");
  }
}

static void BM_Scaling_Matrix_Size(benchmark::State& state, figures::FigureFinderType type) {
  using namespace figures;
  const size_t size = state.range(0);
  BoolMatrix matrix(size, size);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      matrix(i, j) = (i + j) % 2;
    }
  }
  auto finder = createFigureFinder(type);
  MemoryUsage before = getCurrentMemoryUsage();
  for (auto _ : state) {
    benchmark::DoNotOptimize(finder->countFigures(matrix));
  }
  MemoryUsage after = getCurrentMemoryUsage();
  long long rss_delta =
      static_cast<long long>(after.rss_kb) - static_cast<long long>(before.rss_kb);
  double rss_mb = (rss_delta > 0 ? rss_delta : 0) / 1024.0;
  // Report metrics
  state.counters["Size"] = benchmark::Counter(size);
  state.counters["Cells"] = benchmark::Counter(size * size);
  state.counters["RSS_MB"] = benchmark::Counter(rss_mb);
  state.SetItemsProcessed(state.iterations() * size * size);
  state.SetBytesProcessed(state.iterations() * size * size);
}

static void RegisterAllScalingBenchmarks() {
  using namespace figures;
  for (auto type : getAllFigureFinderTypes()) {
    benchmark::RegisterBenchmark(
        ("BM_Scaling_Matrix_Size_" + toString(type)).c_str(),
        [type](benchmark::State& state) { BM_Scaling_Matrix_Size(state, type); })
        ->RangeMultiplier(2)
        ->Range(64, 8192)
        ->Unit(benchmark::kMillisecond)
        ->ArgName("Size");
  }
}

static void BM_Cache_Locality(benchmark::State& state, figures::FigureFinderType type) {
  using namespace figures;
  const size_t size = 10000;
  BoolMatrix matrix(size, size, 1);
  auto finder = createFigureFinder(type);
  MemoryUsage before = getCurrentMemoryUsage();
  for (auto _ : state) {
    benchmark::DoNotOptimize(finder->countFigures(matrix));
  }
  MemoryUsage after = getCurrentMemoryUsage();
  long long rss_delta =
      static_cast<long long>(after.rss_kb) - static_cast<long long>(before.rss_kb);
  double rss_mb = (rss_delta > 0 ? rss_delta : 0) / 1024.0;
  state.counters["Matrix_MB"] = benchmark::Counter((size * size) / (1024.0 * 1024.0));
  state.counters["RSS_MB"] = benchmark::Counter(rss_mb);
}

static void RegisterAllCacheBenchmarks() {
  using namespace figures;
  for (auto type : getAllFigureFinderTypes()) {
    benchmark::RegisterBenchmark(
        ("BM_Cache_Locality_" + toString(type)).c_str(),
        [type](benchmark::State& state) { BM_Cache_Locality(state, type); });
  }
}

// Register all parameterized benchmarks
int main(int argc, char** argv) {
  ::benchmark::Initialize(&argc, argv);
  RegisterAllCPUBenchmarks();
  RegisterAllMemoryBenchmarks();
  RegisterAllScalingBenchmarks();
  RegisterAllCacheBenchmarks();
  ::benchmark::RunSpecifiedBenchmarks();
  return 0;
}
