#include <algorithm>
#include <iterator>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <ostream>
#include "algorithm_pcim.hpp"
#include "vector_random_pcim.hpp"

template <class RandomAccessIterator, class URNG>
RandomAccessIterator random_it (RandomAccessIterator first, RandomAccessIterator last, URNG&& g)
{
  auto i=(last-first)-1;
  std::uniform_int_distribution<decltype(i)> d(0,i);
  std::advance(first, d(g));
  return first;
}

void vector_random_pcim::vector_random_init() {
  frequency.resize(n_total_probes, 0);
  other_probes.resize(n_total_probes);
  iota(other_probes.begin(), other_probes.end(), 0);

  for (int i=0; i != lgn.size(); i++) {
    std::swap(other_probes[lgn[i]], other_probes[other_probes.size() - i - 1]);
  }
  other_probes.erase(other_probes.end() - lgn.size(), other_probes.end());
}

void vector_random_pcim::vector_random_iter_init() {
}

void vector_random_pcim::vector_random_tile_creation(int index) {
  tile_ptr = make_unique<vector<int>>(subset_size);

  std::uniform_int_distribution<int> dist_tile_space(0,tile_size-1);
  std::uniform_int_distribution<int> dist_other_probes_space(0, other_probes.size()-1);

  for(int i=0; i<subset_size; i++) {
    int value = *random_it(other_probes.begin(), other_probes.end(), generator);
    (*tile_ptr)[i] = value;

    frequency[value] += 1;
  }
  for(int i=0; i<lgn.size(); i++) {
    auto it = random_it(tile_ptr->begin(), tile_ptr->end(), generator);
    tile_ptr->insert(it, lgn[i]);
    frequency[lgn[i]] += 1;
  }
}

