#ifndef __VEC_EXTRACT_PCIM__
#define __VEC_EXTRACT_PCIM__

#include "algorithm_pcim.hpp"
#include <chrono>
#include <cstddef>
#include <iostream>
#include <map>
#include <ostream>
#include <random>
#include <vector>

using namespace std;

class vector_extract_pcim : public algorithm_pcim<vector<int>, vector<int>> {
  vector<int> other_probes;
  vector<int> custom_prob;

public:
  using algorithm_pcim::algorithm_pcim;
  void (vector_extract_pcim::*f_tile_creation)(int index) =
    &vector_extract_pcim::vec_uniform_tile_creation;

  void vec_extract_init();
  void vec_extract_iter_init();

  void vec_uniform_tile_creation(int index);
  void vec_extract_custom_tile_creation(int index);

  void set_custom_probability(vector<int> prob_vec) {
    custom_prob = prob_vec;
    f_tile_creation = &vector_extract_pcim::vec_extract_custom_tile_creation;
  }
  void vec_extract_uniform() {
    f_tile_creation = &vector_extract_pcim::vec_uniform_tile_creation;
  }

  void init() { vec_extract_init(); }
  void iteration_init() { vec_extract_iter_init(); }
  void tile_creation(int index) { (this->*f_tile_creation)(index); }
  void iteration_end() {}
  void end() {}
};

#endif
