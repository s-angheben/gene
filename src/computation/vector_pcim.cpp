#include <algorithm>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <ostream>
#include <fstream>
#include "algorithm_pcim.hpp"
#include "vector_pcim.hpp"

template <class RandomAccessIterator, class URNG>
  RandomAccessIterator random_it (RandomAccessIterator first, RandomAccessIterator last, URNG&& g)
{
    auto i=(last-first)-1;
    std::uniform_int_distribution<decltype(i)> d(0,i);
    std::advance(first, d(g));
    return first;
}

void vector_pcim::vector_init() {
    tile.resize(tile_size);
    frequency.resize(n_total_probes);
    other_probes.resize(n_total_probes);
    iota(other_probes.begin(), other_probes.end(), 0);

    for (int i=0; i != lgn.size(); i++) {
        std::swap(other_probes[lgn[i]], other_probes[other_probes.size() - i - 1]);
    }
    other_probes.erase(other_probes.end() - lgn.size(), other_probes.end());
}

void vector_pcim::vector_iter_init() {
    other_probes.resize(n_total_probes - lgn.size());
    shuffle(other_probes.begin(), other_probes.end(), generator);

    int to_add = tile_number * subset_size - other_probes.size();
    if (to_add) {            // if zero skip
        other_probes.insert(other_probes.end(), other_probes.begin(), other_probes.begin()+to_add);

        for(auto it=other_probes.begin(); it != other_probes.begin()+to_add; ++it) {
            frequency[*it] = 2;
        }
    }
}

void vector_pcim::vector_tile_creation(int index) {
//    std::cout << "vector bucket creation" << std::endl;
    std::copy(lgn.begin(), lgn.end(), tile.begin());
    std::copy(other_probes.begin() + (index * subset_size),
              other_probes.begin() + (index * subset_size + subset_size),
              tile.begin()+lgn.size());
    shuffle(tile.begin(), tile.end(), generator);
}

void vector_pcim::vector_tile_creation_random_insert(int index) {
//    std::cout << "vector random insert" << std::endl;
    tile.resize(subset_size);

    std::copy(other_probes.begin() + (index * subset_size),
              other_probes.begin() + (index * subset_size + subset_size),
              tile.begin());

    for(int i=0; i<lgn.size(); i++) {
        auto it = random_it(tile.begin(), tile.end(), generator);
        tile.insert(it, lgn[i]);
    }
}

