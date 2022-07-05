#ifndef __SET_PCIM__
#define __SET_PCIM__

#include <cstddef>
#include <iostream>
#include <ostream>
#include <unordered_set>
#include <map>
#include "algorithm_pcim.hpp"

class set_pcim : public algorithm_pcim {
    std::unordered_set<int> other_probes;
    std::unordered_set<int> iteration_probes;

public:
    void unordered_set_init();
    void unordered_set_other_probes_shuffle();
    void unordered_set_tile_creation(int index);
    void unordered_set_tile_cout(int index);

    set_pcim(int _iterations, int _tile_size, int _n_total_probes, std::vector<int> _lgn) :
        algorithm_pcim(_iterations, _tile_size, _n_total_probes, _lgn) {
        }

//    using algorithm_pcim::algorithm_pcim; // use the default constructor

    void init() { unordered_set_init(); }
    void iteration_init() { unordered_set_other_probes_shuffle(); }
    void tile_creation(int index) { unordered_set_tile_creation(index); }
    void tile_save(int index) { unordered_set_tile_cout(index); }
    void freq_save() { freq_cout(); }
};

#endif
