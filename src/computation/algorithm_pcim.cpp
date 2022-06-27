#include <algorithm>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <ostream>
#include "algorithm_pcim.hpp"

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

algorithm_pcim::~algorithm_pcim(){}

void algorithm_pcim::freq_cout() {
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

int algorithm_pcim::run() {
    generator = f_generator();
    init();
    for (int i=0; i<iterations; i++) {
        iteration_init();
        for (int j=0; j<tile_number; j++) {
            tile_creation(j);
            tile_save(j);
        }
        freq_save();
    }
    return EXIT_SUCCESS;
}
