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
#include <vector_extract_pcim.hpp>
#include <vector_pcim.hpp>
#include <vector_random_pcim.hpp>

////////////////////////
////// SUPPORT FUNCTIONS

template <ranges::common_range T1, ranges::common_range T2>
void run_algo_no_output(algorithm_pcim<T1, T2> &a) {
  a.init();
  for (int i = 0; i < a.iterations; i++) {
    a.iteration_init();
    for (int j = 0; j < a.tile_number; j++) {
      a.tile_creation(j);
      a.iteration_end();
    }
    a.end();
  }
}

vector<int> create_random_vector(int size, int max) {
  vector<int> vec;

  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0,max-1);

  for (int i=0; i<size; i++) {
    int number = distribution(generator);
    vec.push_back(number);
  }
  return vec;
}

////////////////////////
////////////// BENCHMARK


// VFDS
static void BM_ComputationVfds(benchmark::State & state) {
  for (auto _ : state) {
    state.PauseTiming();
    int iteration = 3;
    long unsigned int size = state.range(0);
    long unsigned int tile_size = int(float(size)/100*5);
    vector<int> lgn = create_random_vector(1, size);
    int npc = 10; //not used

    if (tile_size >= size || lgn.size() >= tile_size) {
      state.SkipWithError("invalid parameters!");
      continue;
    }

    vector_pcim a(iteration, tile_size, size, lgn, npc);
    state.ResumeTiming();

    run_algo_no_output(a);
  }
}
BENCHMARK(BM_ComputationVfds)
->DenseRange(100, 150000, 10000);

// VFSI
static void BM_ComputationVfsi(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int iteration = 3;
    long unsigned int size = state.range(0);
    long unsigned int tile_size = int(float(size) / 100 * 5);
    vector<int> lgn = create_random_vector(1, size);
    int npc = 10; // not used

    if (tile_size >= size || lgn.size() >= tile_size) {
      state.SkipWithError("invalid parameters!");
      continue;
    }

    vector_pcim a(iteration, tile_size, size, lgn, npc);
    a.set_tile_creation_lgn_insert();
    state.ResumeTiming();

    run_algo_no_output(a);
  }
}
BENCHMARK(BM_ComputationVfsi)->DenseRange(100, 150000, 10000);

// VRI
static void BM_ComputationVri(benchmark::State & state) {
  for (auto _ : state) {
    state.PauseTiming();
    int iteration = 3;
    long unsigned int size = state.range(0);
    long unsigned int tile_size = int(float(size) / 100 * 5);
    vector<int> lgn = create_random_vector(1, size);
    int npc = 10; // not used

    if (tile_size >= size || lgn.size() >= tile_size) {
      state.SkipWithError("invalid parameters!");
      continue;
    }

    vector_random_pcim a(iteration, tile_size, size, lgn, npc);
    state.ResumeTiming();

    run_algo_no_output(a);
  }
}
BENCHMARK(BM_ComputationVri)->DenseRange(100, 150000, 10000);

// VCI
static void BM_ComputationVci(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int iteration = 3;
    long unsigned int size = state.range(0);
    long unsigned int tile_size = int(float(size) / 100 * 5);
    vector<int> lgn = create_random_vector(1, size);
    int npc = 10; // not used
    vector<int> custom_prob = create_random_vector(size-lgn.size(), 10);

    if (tile_size >= size || lgn.size() >= tile_size) {
      state.SkipWithError("invalid parameters!");
      continue;
    }

    vector_random_pcim a(iteration, tile_size, size, lgn, npc);
    a.set_custom_probability(custom_prob);
    state.ResumeTiming();

    run_algo_no_output(a);
  }
}
BENCHMARK(BM_ComputationVci)
->DenseRange(100, 150000, 10000);

// VEU
static void BM_ComputationVeu(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int iteration = 3;
    long unsigned int size = state.range(0);
    long unsigned int tile_size = int(float(size) / 100 * 5);
    vector<int> lgn = create_random_vector(1, size);
    int npc = 10; // not used

    if (tile_size >= size || lgn.size() >= tile_size) {
      state.SkipWithError("invalid parameters!");
      continue;
    }

    vector_extract_pcim a(iteration, tile_size, size, lgn, npc);
    state.ResumeTiming();

    run_algo_no_output(a);
  }
}
BENCHMARK(BM_ComputationVeu)
->DenseRange(100, 150000, 10000);

// VEC
static void BM_ComputationVec(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int iteration = 3;
    long unsigned int size = state.range(0);
    long unsigned int tile_size = int(float(size) / 100 * 5);
    vector<int> lgn = create_random_vector(1, size);
    int npc = 10; // not used
    vector<int> custom_prob = create_random_vector(size - lgn.size(), 10);

    if (tile_size >= size || lgn.size() >= tile_size) {
      state.SkipWithError("invalid parameters!");
      continue;
    }

    vector_extract_pcim a(iteration, tile_size, size, lgn, npc);
    a.set_custom_probability(custom_prob);
    state.ResumeTiming();

    run_algo_no_output(a);
  }
}
BENCHMARK(BM_ComputationVec)
->DenseRange(100, 150000, 10000);

BENCHMARK_MAIN();
