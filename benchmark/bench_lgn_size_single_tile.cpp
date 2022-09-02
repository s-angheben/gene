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
  int j = 0;
  a.init();
  a.iteration_init();
  a.tile_creation(j);
  a.iteration_end();
  a.end();
}

////////////////////////
////////////// BENCHMARK

// VFSI
static void BM_ComputationVfsi(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int iteration = 3;
    long unsigned int size = 80000;
    long unsigned int tile_size = 1000;
    vector<int> lgn(state.range(0));
    std::iota(lgn.begin(), lgn.end(), 0);
    int npc = 10; // not used

    if (tile_size >= size || lgn.size() >= tile_size) {
      state.SkipWithError("invalid parameters!");
      continue;
    }

    vector_pcim a(iteration, tile_size, size, lgn, npc);
    a.set_tile_creation_lgn_insert();

    int j = 0;
    a.init();
    a.iteration_init();
    state.ResumeTiming();

    a.tile_creation(j);
  }
}
BENCHMARK(BM_ComputationVfsi)->DenseRange(1, 999, 25);

// VFDS
static void BM_ComputationVfds(benchmark::State & state) {
  for (auto _ : state) {
    state.PauseTiming();
    int iteration = 3;
    long unsigned int size = 80000;
    long unsigned int tile_size = 1000;
    vector<int> lgn (state.range(0));
    std::iota(lgn.begin(), lgn.end(), 0);
    int npc = 10; //not used

    if (tile_size >= size || lgn.size() >= tile_size) {
      state.SkipWithError("invalid parameters!");
      continue;
    }

    vector_pcim a(iteration, tile_size, size, lgn, npc);
    int j = 0;
    a.init();
    a.iteration_init();
    state.ResumeTiming();

    a.tile_creation(j);
  }
  // for (auto _ : state) {
  //   state.PauseTiming();
  //   long unsigned int size = 80000;
  //   long unsigned int tile_size = 1000;
  //   vector<int> lgn = create_random_vector(state.range(0), size);

  //   vector<int> other_probes;
  //   other_probes.resize(size);
  //   iota(other_probes.begin(), other_probes.end(), 0);

  //   for (long unsigned int i = 0; i != lgn.size(); i++) {
  //     std::swap(other_probes[lgn[i]],
  //               other_probes[other_probes.size() - i - 1]);
  //   }
  //   other_probes.erase(other_probes.end() - lgn.size(), other_probes.end());
  //   int index = 0;
  //   int subset_size = size - lgn.size();
  //   std::default_random_engine generator;

  //   state.ResumeTiming();

  //   vector<int> tile(size);
  //   std::copy(lgn.begin(), lgn.end(), tile.begin());
  //   std::copy(other_probes.begin() + (index * subset_size),
  //             other_probes.begin() + (index * subset_size + subset_size),
  //             tile.begin() + lgn.size());
  //   shuffle(tile.begin(), tile.end(), generator);
  // }
}
BENCHMARK(BM_ComputationVfds)
->DenseRange(1, 999, 25);


BENCHMARK_MAIN();
