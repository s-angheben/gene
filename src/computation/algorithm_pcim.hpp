#ifndef __ALGO_PCIM__
#define __ALGO_PCIM__

#include <cstddef>
#include <random>
#include <chrono>
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <map>

// cout vector definition
std::ostream& operator<<(std::ostream& os, const std::vector<int>& v);

// initialize random generator
std::mt19937 random_generator();

// debug generator with seed=1
std::mt19937 debug_generator();

// pcim algorithm abstract class
class algorithm_pcim {
public:
    int iterations;
    int tile_size;
    int n_total_probes;
    std::vector<int> lgn;
    std::ofstream out_file;

    int subset_size;
    int tile_number;
    std::vector<int> tile;
    std::map<int, int> frequency;
    std::mt19937 generator;

    std::mt19937 (*f_generator)() = &random_generator;

    void calculate_tile_number();

    void freq_cout();

    virtual void init() = 0;
    virtual void iteration_init() = 0;
    virtual void tile_creation(int index) = 0;
    virtual void tile_save(int index) = 0;
    virtual void freq_save() = 0;
    virtual void iteration_end() = 0;
    virtual void end() = 0;

    algorithm_pcim(int _iterations, int _tile_size, int _n_total_probes, std::vector<int> _lgn):
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
    virtual ~algorithm_pcim() = 0;
};
#endif
