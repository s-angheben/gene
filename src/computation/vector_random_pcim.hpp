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
    void vector_random_init();
    void vector_random_iter_init();
    void vector_random_tile_creation(int index);
    void vector_tile_cout(int index);
/*
    vector_random_pcim(int _iterations, int _tile_size, int _n_total_probes, std::vector<int> _lgn) :
        algorithm_pcim(_iterations, _tile_size, _n_total_probes, _lgn) {
        }
*/
    using algorithm_pcim::algorithm_pcim; // use the default constructor

// ALGORITHM
    void init() { vector_random_init(); }
    void iteration_init() { vector_random_iter_init(); }
    void tile_creation(int index) { vector_random_tile_creation(index); }
    void iteration_end() {}
    void end() {}
};

#endif
