#include "algorithm_pcim.hpp"
#include "vector_extract_pcim.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <ostream>
#include <random>
#include <vector>

using namespace std;

template <class RandomAccessIterator, class URNG>
RandomAccessIterator random_it(RandomAccessIterator first,
                               RandomAccessIterator last, URNG &&g) {
  auto i = (last - first) - 1;
  std::uniform_int_distribution<decltype(i)> d(0, i);
  std::advance(first, d(g));
  return first;
}

void vector_extract_pcim::vec_extract_init() {
  frequency.resize(n_total_probes, 0);

  other_probes.resize(n_total_probes);
  iota(other_probes.begin(), other_probes.end(), 0);

  sort(lgn.rbegin(), lgn.rend());
  for(auto& elem : lgn) {
    other_probes.erase(other_probes.begin() + elem);
  }
}

void vector_extract_pcim::vec_extract_iter_init() {}

void vector_extract_pcim::vec_uniform_tile_creation(int index) {
  tile_ptr = make_unique<vector<int>> (subset_size);
  vector<int> current_other_probes (other_probes);

  for(int i=0; i<subset_size; i++) {
    auto it = random_it(current_other_probes.begin(), current_other_probes.end(), generator);
    (*tile_ptr)[i] = *it;
    frequency[*it] += 1;

    current_other_probes.erase(it);

  }

  for(long unsigned int i=0; i<lgn.size(); i++) {
    auto it = random_it(tile_ptr->begin(), tile_ptr->end(), generator);
    tile_ptr->insert(it, lgn[i]);
    frequency[lgn[i]] += 1;
  }

  //  shuffle(tile_ptr->begin(), tile_ptr->end(), generator);
}

void vector_extract_pcim::vec_extract_custom_tile_creation(int index) {
  tile_ptr = make_unique<vector<int>> (subset_size);
  vector<int> current_other_probes(other_probes);
  vector<int> current_custom_prob (custom_prob);

  for(int i=0; i<subset_size; i++) {
    auto it_probes = current_other_probes.begin();
    auto it_probab = current_custom_prob.begin();

    discrete_distribution<int> d(current_custom_prob.begin(), current_custom_prob.end());
    int step = d(generator);
    advance(it_probes, step);
    advance(it_probab, step);

    (*tile_ptr)[i] = *it_probes;
    frequency[*it_probes] += 1;

    current_other_probes.erase(it_probes);
    current_custom_prob.erase(it_probab);
  }

  for (long unsigned int i = 0; i < lgn.size(); i++) {
    auto it = random_it(tile_ptr->begin(), tile_ptr->end(), generator);
    tile_ptr->insert(it, lgn[i]);
    frequency[lgn[i]] += 1;
  }
}
