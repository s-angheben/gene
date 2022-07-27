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

vector<int> create_lgn_vector(int size) {
  vector<int> lgn;

  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0,9);

  for (int i=0; i<size; i++) {
    int number = distribution(generator);
    lgn.push_back(number);
  }
  return lgn;
}

////////////////////////
//////BENCHMARK ARGUMENT

static void CustomArguments(benchmark::internal::Benchmark * b) {
  vector<double> tile_perc {1, 5, 40};
  vector<double> lgn_perc {1, 5, 50};
  for (double size = 8; size <= 400000; size *= 6) {
    for (auto &x : tile_perc) {
      double tile_size = size / 100 * x;
      int size_int = nearbyint(size);
      int tile_size_int = nearbyint(tile_size);
      b->Args({size_int, tile_size_int, 1});
      for (auto &y : lgn_perc) {
        double lgn_size = tile_size / 100 * y;
        int lgn_size_int = nearbyint(lgn_size);
        b->Args({size_int, tile_size_int, lgn_size_int});
      }
    }
  }
}

// static void argSizeAndTile_size(benchmark::internal::Benchmark *b) {
//   vector<double> tile_perc {1, 5, 40};
//   for (double size = 8; size <= 400000; size *= 6) {
//     for (auto& x : tile_perc) {
//       double tile_size = size / 100 * x;
//       int size_int = nearbyint(size);
//       int tile_size_int = nearbyint(tile_size);
//       b->Args({size_int, tile_size_int});
//     }
//   }
// }
// static void argSizeAndTile_size(benchmark::internal::Benchmark *b) {
//   for (int i = 2; i <= 10; ++i)
//     for (int j = 32; j <= 1024*1024; j *= 8)
//       b->Args({i, j});
// }

////////////////////////
////////////// BENCHMARK

// tile_size is important?
static void BM_ComputationVfds(benchmark::State & state) {
  for (auto _ : state) {
    state.PauseTiming();
    int iteration = 3;
    long unsigned int size = state.range(0);
    long unsigned int tile_size = state.range(1);
    vector<int> lgn = create_lgn_vector(state.range(2));
    int npc = 10;

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
->Apply(CustomArguments);

static void BM_ComputationVfsi(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int iteration = 3;
    long unsigned int size = state.range(0);
    long unsigned int tile_size = state.range(1);
    vector<int> lgn = create_lgn_vector(state.range(2));
    int npc = 10;

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
BENCHMARK(BM_ComputationVfsi)
->Apply(CustomArguments);

static void BM_ComputationVri(benchmark::State & state) {
  for (auto _ : state) {
    state.PauseTiming();
    int iteration = 3;
    long unsigned int size = state.range(0);
    long unsigned int tile_size = state.range(1);
    vector<int> lgn = create_lgn_vector(state.range(2));
    int npc = 10;

    if (tile_size >= size || lgn.size() >= tile_size) {
      state.SkipWithError("invalid parameters!");
      continue;
    }

    vector_random_pcim a(iteration, tile_size, size, lgn, npc);
    state.ResumeTiming();

    run_algo_no_output(a);
  }
}
BENCHMARK(BM_ComputationVri)
->Apply(CustomArguments);


BENCHMARK_MAIN();
