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

class vector_pcim : public algorithm_pcim {
    std::vector<int> other_probes;

public:
    void vector_init();
    void vector_other_probes_shuffle();
    void vector_tile_creation(int index);
    void vector_tile_cout(int index);
    void vector_iter_init();

    void vector_tile_creation_random_insert(int index);

    vector_pcim(int _iterations, int _tile_size, int _n_total_probes, std::vector<int> _lgn) :
        algorithm_pcim(_iterations, _tile_size, _n_total_probes, _lgn) {
        }

//    using algorithm_pcim::algorithm_pcim; // use the default constructor

    void init() { vector_init(); }
    void iteration_init() { vector_iter_init(); }
    void tile_creation(int index) { vector_tile_creation_random_insert(index); }
    void tile_save(int index) { vector_tile_cout(index); }
    void freq_save() { freq_cout(); }
    void iteration_end() {}
    void end() {}
//    void tile_save(int index) { }
//    void freq_save() {  }
};

#endif
