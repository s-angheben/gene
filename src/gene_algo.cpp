#include <algorithm>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <ostream>
#include "gene_algo.hpp"

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v) {
    for (auto& elem : v) {
        os << elem << " ";
    }
    return os;
}

std::mt19937 random_generator() {
    unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    return generator;
}

std::mt19937 debug_generator() {
    unsigned seed = 1;
    std::mt19937 generator(seed);
    return generator;
}

void algorithm_pcim::set_generator (std::mt19937 (*f)()) {
    f_generator = f;
}

void algorithm_pcim::vector_init() {
    other_probes.resize(n_total_probes);
    iota(other_probes.begin(), other_probes.end(), 0);

    for (int i=0; i != lgn.size(); i++) {
        std::swap(other_probes[lgn[i]], other_probes[other_probes.size() - i - 1]);
    }
    other_probes.erase(other_probes.end() - lgn.size(), other_probes.end());
}

void algorithm_pcim::vector_other_probes_shuffle() {
    shuffle(other_probes.begin(), other_probes.end(), generator);
    iteration_probes.insert(iteration_probes.begin(),
                            other_probes.begin(),
                            other_probes.end());

    // add to the end other elements if necessary
    int to_add = tile_number * subset_size - other_probes.size();
    if (to_add) {            // if zero skip
        iteration_probes.insert(iteration_probes.end(), other_probes.begin(), other_probes.begin()+to_add);

        for(auto it=other_probes.begin(); it != other_probes.begin()+to_add; ++it) {
            frequency[*it] = 2;
        }
    }
}

void algorithm_pcim::vector_tile_creation(int index) {
    std::copy(lgn.begin(), lgn.end(), tile.begin());
    std::copy(iteration_probes.begin() + (index * subset_size),
              iteration_probes.begin() + (index * subset_size + subset_size),
              tile.begin()+lgn.size());
    shuffle(tile.begin(), tile.end(), generator);
}

void algorithm_pcim::vector_tile_cout(int index) {
    std::cout << "TILE" << index << ": (#" << tile.size() << ") " << tile << std::endl;
}


void algorithm_pcim::map_cout() {
    std::cout << "FREQUENCY: ";
    for (int i=0; i<n_total_probes; i++) {
        if(frequency[i]) {
            std::cout << "[" << i << ":" << frequency[i] << "]";
        } else {
            std::cout << "[" << i << ":" << 1 << "]";
        }
    }
    std::cout << std::endl;
}

void algorithm_pcim::calculate_tile_number() {
    tile_number=(n_total_probes-lgn.size())/subset_size;
    int available = (n_total_probes - lgn.size() - tile_number * subset_size);
    if (available) {            // if zero skip
        tile_number++;
    }
}
/*
int algorithm_pcim::run() {
    std::vector<int> other_probes(n_total_probes);
    setup_other_probes(other_probes, lgn);
    std::cout << other_probes << std::endl;

    std::mt19937 generator = f_generator();

    for (int i=0; i<iterations; i++) {
        std::cout << "ITERATION " << i << std::endl;

        shuffle(other_probes.begin(), other_probes.end(), generator);
        std::cout << other_probes << std::endl;

        std::vector<int> tile(tile_size);

        // elements used twice
        std::vector<int> frequency;

        // calculate tail to do
        int tail_number = tail_to_do(other_probes, subset_size, frequency);

        for (int j=0; j<tail_number; j++) {
            create_tail(tile, j, other_probes, lgn, subset_size, generator);
            std::cout << "TILE" << j << ": (#" << tile.size() << ") " << tile << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
*/
int algorithm_pcim::run() {

    generator = f_generator();

    (this->*f_init)();
    for (int i=0; i<iterations; i++) {
        std::cout << "ITERATION " << i << std::endl;

        (this->*f_iteration_init)();
        for (int j=0; j<tile_number; j++) {
            (this->*f_tile_creation)(j);
            (this->*f_tile_save)(j);
        }
        (this->*f_freq_save)();
    }
    return EXIT_SUCCESS;
}
