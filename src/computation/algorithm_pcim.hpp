#ifndef __ALGO_PCIM__
#define __ALGO_PCIM__

#include <ranges>
#include <fstream>
#include <random>
#include <chrono>
#include <iostream>

namespace ch = std::chrono;

using namespace std;

// wrapper function to be able to have a pointer to all possible implementation of the algorithm
class algo {
public:
    virtual int run() = 0;
};

template<ranges::common_range T1, ranges::common_range T2>
class algorithm_pcim : public algo {
    void calculate_tile_number() {
        tile_number=(n_total_probes-lgn.size())/subset_size;
        int available = (n_total_probes - lgn.size() - tile_number * subset_size);
        if (available) {            // if zero skip
            tile_number++;
        }
    }
    void tile_cout(int index) {
        cout << "TILE" << index << ":";
        for(const auto& item : tile) cout << item << ",";
        cout << endl;
    }
    void freq_cout() {
        cout << "FREQUENCY: ";
        for (int i=0; i<n_total_probes; i++) {
            if(frequency[i]) {
                cout << "[" << i << ":" << frequency[i] << "]";
            } else {
                cout << "[" << i << ":" << 0 << "]";
            }
        }
        cout << endl;
    }
    void tile_to_file(int index) {
        out_file << "TILE" << index << ":";
        for(const auto& item : tile) out_file << item << ",";
        out_file << endl;
    }
    void freq_to_file() {
        out_file << "FREQUENCY: ";
        for (int i=0; i<n_total_probes; i++) {
            if(frequency[i]) {
                out_file << "[" << i << ":" << frequency[i] << "]";
            } else {
                out_file << "[" << i << ":" << 0 << "]";
            }
        }
        out_file << endl;
    }
public:
// INPUT
    int iterations;
    int tile_size;
    int n_total_probes;
    T1 lgn;

// INTERNAL VARIABLES
    int subset_size;
    int tile_number;
    std::mt19937 generator;
    ofstream out_file;
    void (algorithm_pcim::*f_save_tile)(int index) = &algorithm_pcim::tile_cout;
    void (algorithm_pcim::*f_save_freq)() = &algorithm_pcim::freq_cout;

// OUTPUT
    T1 tile;
    T2 frequency;

// POSSIBLE CONFIG FUNCTIONS
    void set_random_generator() {
        unsigned seed = ch::high_resolution_clock::now().time_since_epoch().count();
        generator.seed(seed);
    }
    void set_debug_generator() {
        unsigned seed = 1;
        generator.seed(seed);
    }
    void set_custom_generator(seed_seq seed) {
        generator.seed(seed);
    }
    void set_tile_cout() { f_save_tile = &algorithm_pcim::tile_cout; }
    void set_freq_cout() { f_save_freq = &algorithm_pcim::freq_cout; }
    void set_tile_to_file(string fp) { out_file.open (fp, ios::out | ios::app); f_save_tile = &algorithm_pcim::tile_to_file; }
    void set_freq_to_file(string fp) { out_file.open (fp, ios::out | ios::app); f_save_freq = &algorithm_pcim::tile_to_file; }


// VIRTUAL FUNCTIONS
    virtual void init() = 0;
    virtual void iteration_init() = 0;
    virtual void tile_creation(int index) = 0;
    virtual void iteration_end() = 0;
    virtual void end() = 0;

    algorithm_pcim(int _iterations, int _tile_size, int _n_total_probes, std::vector<int> _lgn):
        lgn(_lgn)
    {
        iterations = _iterations;
        tile_size = _tile_size;
        n_total_probes = _n_total_probes;

        subset_size = tile_size - lgn.size();
        tile.resize(tile_size); //to check
        calculate_tile_number();
    }

    int run() {
        init();
        for (int i=0; i<iterations; i++) {
            iteration_init();
            for (int j=0; j<tile_number; j++) {
                tile_creation(j);
                (this->*f_save_tile)(j);
            }
            (this->*f_save_freq)();
            iteration_end();
        }
        end();
        return EXIT_SUCCESS;
    }
    ~algorithm_pcim() {
        if(out_file) out_file.close();
    }
};

#endif
