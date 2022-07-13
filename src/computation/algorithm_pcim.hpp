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

    virtual void set_random_generator() = 0;
    virtual void set_debug_generator() = 0;
    virtual void set_custom_generator(unsigned seed) = 0;

    virtual void set_tile_cout() = 0;
    virtual void set_freq_cout() = 0;
    virtual void set_seed_cout() = 0;

    virtual void set_tile_to_file(string fp) = 0;
    virtual void set_freq_to_file(string fp) = 0;
    virtual void set_seed_to_file(string fp) = 0;

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
        cout << "GROUP:" << current_wus << ", TILE" << index << ":";
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
    void seed_cout() {
        cout << "SEED: " << seed;
    }
    void tile_to_file(int index) {
        tile_out_file << "TILE" << index << ":";
        for(const auto& item : tile) tile_out_file << item << ",";
        tile_out_file << endl;
        tile_out_file << std::flush;
    }
    void freq_to_file() {
        freq_out_file << "FREQUENCY: ";
        for (int i=0; i<n_total_probes; i++) {
            if(frequency[i]) {
                freq_out_file << "[" << i << ":" << frequency[i] << "]";
            } else {
                freq_out_file << "[" << i << ":" << 0 << "]";
            }
        }
        freq_out_file << endl;
    }
    void seed_to_file() {
        seed_out_file << "SEED:" << seed << endl;
    }
public:
// INPUT
    int iterations;
    int tile_size;
    int n_total_probes;
    int npc;
    T1 lgn;

// INTERNAL VARIABLES
    int subset_size;
    int tile_number;

    string tile_name_fp;

    int current_wus;
    int counter;

    unsigned seed;
    std::mt19937 generator;
    ofstream tile_out_file;
    ofstream freq_out_file;
    ofstream seed_out_file;
    void (algorithm_pcim::*f_save_tile)(int index) = &algorithm_pcim::tile_cout;
    void (algorithm_pcim::*f_save_freq)() = &algorithm_pcim::freq_cout;
    void (algorithm_pcim::*f_save_seed)() = &algorithm_pcim::seed_cout;

// OUTPUT
    T1 tile;
    T2 frequency;

// POSSIBLE CONFIG FUNCTIONS
    void set_random_generator() {
        seed = ch::high_resolution_clock::now().time_since_epoch().count();
        generator.seed(seed);
    }
    void set_debug_generator() {
        seed = 1;
        generator.seed(seed);
    }
    void set_custom_generator(unsigned _seed) {
        seed = _seed;
        generator.seed(seed);
    }
    void set_tile_cout() { f_save_tile = &algorithm_pcim::tile_cout; }
    void set_freq_cout() { f_save_freq = &algorithm_pcim::freq_cout; }
    void set_seed_cout() { f_save_seed = &algorithm_pcim::seed_cout; }
    void set_tile_to_file(string fp) {
        tile_name_fp=fp;
        tile_out_file.open (tile_name_fp+to_string(current_wus)+".txt", ios::out | ios::app);
        f_save_tile = &algorithm_pcim::tile_to_file;
    }
    void set_freq_to_file(string fp) { freq_out_file.open (fp, ios::out | ios::app); f_save_freq = &algorithm_pcim::freq_to_file; }
    void set_seed_to_file(string fp) { seed_out_file.open (fp, ios::out | ios::app); f_save_seed = &algorithm_pcim::seed_to_file; }


// VIRTUAL FUNCTIONS
    virtual void init() = 0;
    virtual void iteration_init() = 0;
    virtual void tile_creation(int index) = 0;
    virtual void iteration_end() = 0;
    virtual void end() = 0;

    algorithm_pcim(int _iterations, int _tile_size, int _n_total_probes, std::vector<int> _lgn, int _npc):
        lgn(_lgn)
    {
        iterations = _iterations;
        tile_size = _tile_size;
        n_total_probes = _n_total_probes;
        if (_npc == 0) {
            npc = 2147483647; //MAXINT
        } else {
            npc = _npc;
        }
        current_wus = 1;

        subset_size = tile_size - lgn.size();
        tile.resize(tile_size); //to check
        calculate_tile_number();
    }

    void increase_state () {
        counter++;
        if (counter > npc) {
            counter = 0;
            current_wus++;

            if (tile_out_file.is_open()) {
                // close previus file
                tile_out_file.close();

                //open next file
                tile_out_file.open (tile_name_fp+to_string(current_wus)+".txt", ios::out | ios::app);
            }
        }
    }

    void init_state () {
        counter = 0;
        current_wus = 1;
    }

    int run() {
        init();
        init_state();
        for (int i=0; i<iterations; i++) {
            iteration_init();
            for (int j=0; j<tile_number; j++) {
                tile_creation(j);
                (this->*f_save_tile)(counter+npc*(current_wus-1));
                increase_state();
            }
            (this->*f_save_freq)();
            iteration_end();
        }
        (this->*f_save_seed)();
        end();
        return EXIT_SUCCESS;
    }
    ~algorithm_pcim() {
        if(tile_out_file) tile_out_file.close();
        if(freq_out_file) freq_out_file.close();
        if(seed_out_file) seed_out_file.close();
    }
};

#endif
