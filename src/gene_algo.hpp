#pragma once

#include <cstddef>
#include <random>
#include <chrono>
#include <iostream>
#include <ostream>
#include <vector>
#include <map>

// cout vector definition
std::ostream& operator<<(std::ostream& os, const std::vector<int>& v);

// initialize random generator
std::mt19937 random_generator();

// debug generator with seed=1
std::mt19937 debug_generator();


// pcim algorithm class
class algorithm_pcim {
    int iterations;
    int tile_size;
    int n_total_probes;
    std::vector<int>& lgn;

    int subset_size;
    int tile_number;
    std::vector<int> tile;
    std::map<int, int> frequency; //use map
    std::mt19937 generator;

    std::mt19937 (*f_generator)() = random_generator;
    void (algorithm_pcim::*f_init)() = &algorithm_pcim::vector_init;
    void (algorithm_pcim::*f_iteration_init)() = &algorithm_pcim::vector_other_probes_shuffle;
    void (algorithm_pcim::*f_tile_creation)(int) = &algorithm_pcim::vector_tile_creation;
    void (algorithm_pcim::*f_tile_save)(int) = &algorithm_pcim::vector_tile_cout;
    void (algorithm_pcim::*f_freq_save)() = &algorithm_pcim::map_cout;

    //vector variable
    std::vector<int> other_probes;
    std::vector<int> iteration_probes;

    void calculate_tile_number();
    void vector_init();
    void vector_other_probes_shuffle();
    void vector_tile_creation(int index);
    void vector_tile_cout(int index);
    void map_cout();
public:
    algorithm_pcim(int _iterations, int _tile_size, int _n_total_probes, std::vector<int>& _lgn):
        lgn(_lgn)
    {
        iterations = _iterations;
        tile_size = _tile_size;
        n_total_probes = _n_total_probes;

        subset_size = tile_size - lgn.size();
        tile.resize(tile_size);
        calculate_tile_number();
    }

    void set_generator(std::mt19937 (*f)());

    int run();
};
