#include <benchmark/benchmark.h>

#include <algorithm>
#include <chrono>
#include <fenv.h>
#include <math.h>
#include <random>
#include <ranges>
#include <string>
#include <vector>

#include <algorithm_pcim.hpp>
#include <vector_pcim.hpp>
#include <vector_random_pcim.hpp>

static void BM_Writer(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int iteration = 2000;
    long unsigned int size = 100000;
    long unsigned int tile_size = 1000;
    vector<int> lgn {56};
    int npc = 600;

    vector_random_pcim a(iteration, tile_size, size, lgn, npc);
    a.set_tile_to_file("/tmp/bench_out");
    a.set_freq_to_file("/tmp/bench_freq.txt");
    a.set_seed_to_file("/tmp/bench_seed.txt");
    state.ResumeTiming();

    a.run();
  }
}
BENCHMARK(BM_Writer);

static void BM_Writer_async(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int iteration = 2000;
    long unsigned int size = 100000;
    long unsigned int tile_size = 1000;
    vector<int> lgn{56};
    int npc = 600;

    vector_random_pcim a(iteration, tile_size, size, lgn, npc);
    a.set_tile_to_file_async("/tmp/bench_out_async");
    a.set_freq_to_file("/tmp/bench_freq_async.txt");
    a.set_seed_to_file("/tmp/bench_seed_async.txt");
    state.ResumeTiming();

    a.run();
  }
}
BENCHMARK(BM_Writer_async);

BENCHMARK_MAIN();
