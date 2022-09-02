#ifndef __VEC_RANDOM_PCIM__
#define __VEC_RANDOM_PCIM__

#include <cstddef>
#include <random>
#include <chrono>
#include <iostream>
#include <ostream>
#include <vector>
#include <map>
#include "algorithm_pcim.hpp"

// --------------------------------------tile type----frequency type
class vector_random_pcim : public algorithm_pcim<vector<int>, vector<int>> {
  std::vector<int> other_probes;

public:
  unique_ptr<std::uniform_int_distribution<int>> dist_other_probes_space_ptr;
  unique_ptr<std::discrete_distribution<int>> dist_other_probes_space_custom_ptr;

  void (vector_random_pcim::*f_tile_creation)(int index) = &vector_random_pcim::vector_random_tile_creation;

  void vector_random_init();
  void vector_random_iter_init();

  void vector_random_tile_creation(int index);
  void vector_custom_random_tile_creation(int index);

  using algorithm_pcim::algorithm_pcim; // use the default constructor

  void set_custom_probability(std::vector<int> prob_vec) {
    dist_other_probes_space_custom_ptr =
      make_unique<std::discrete_distribution<int>> (prob_vec.begin(), prob_vec.end());
    f_tile_creation=&vector_random_pcim::vector_custom_random_tile_creation;
  }
  void set_random_uniform() { f_tile_creation=&vector_random_pcim::vector_random_tile_creation; }

  // ALGORITHM
  void init() { vector_random_init(); }
  void iteration_init() { vector_random_iter_init(); }
  void tile_creation(int index) { (this->*f_tile_creation)(index); }
  void iteration_end() {}
  void end() {}
};

#endif
