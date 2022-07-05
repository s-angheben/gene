#ifndef __VEC_PCIM__
#define __VEC_PCIM__

#include <cstddef>
#include <random>
#include <chrono>
#include <iostream>
#include <ostream>
#include <vector>
#include <map>
#include "algorithm_pcim.hpp"

// --------------------------------------tile type----frequency type
class vector_pcim : public algorithm_pcim<vector<int>, vector<int>> {
    std::vector<int> other_probes;
    void (vector_pcim::*f_tile_creation)(int index) = &vector_pcim::vector_tile_creation;

public:
    void vector_init();

    void vector_other_probes_shuffle();

    void vector_tile_creation(int index);
    void vector_tile_creation_random_insert(int index);

    void vector_iter_init();

// CONFIG FUNCTIONS
    void set_tile_creation_lgn_shuffle () { f_tile_creation=&vector_pcim::vector_tile_creation; }
    void set_tile_creation_lgn_insert () { f_tile_creation=&vector_pcim::vector_tile_creation_random_insert; }


    vector_pcim(int _iterations, int _tile_size, int _n_total_probes, std::vector<int> _lgn) :
        algorithm_pcim(_iterations, _tile_size, _n_total_probes, _lgn) {
        }
//    using algorithm_pcim::algorithm_pcim; // use the default constructor

// ALGORITHM
    void init() { vector_init(); }
    void iteration_init() { vector_iter_init(); }
    void tile_creation(int index) { (this->*f_tile_creation)(index); }
    void iteration_end() {}
    void end() {}
};

#endif
