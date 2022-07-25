#ifndef __ALGO_PCIM__
#define __ALGO_PCIM__

#include <ranges>
#include <fstream>
#include <random>
#include <chrono>
#include <iostream>
#include <future>

#include "writer.hpp"

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

  virtual ~algo() = 0;
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
  unique_ptr<bulk_writer<T1>> bulk_tile;

  string tile_name_fp;

  unsigned seed;
  std::mt19937 generator;
  string out_filename;

  ofstream freq_out_file;
  ofstream seed_out_file;


  void (algorithm_pcim::*f_save_freq)() = &algorithm_pcim::freq_cout;
  void (algorithm_pcim::*f_save_seed)() = &algorithm_pcim::seed_cout;

  // OUTPUT
  unique_ptr<T1> tile_ptr;
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
  void set_tile_cout() { bulk_tile = make_unique<bulk_cout<T1>>(npc); }
  void set_freq_cout() { f_save_freq = &algorithm_pcim::freq_cout; }
  void set_seed_cout() { f_save_seed = &algorithm_pcim::seed_cout; }

  void set_tile_to_file(string fp) {
    bulk_tile = make_unique<bulk_to_file_async<T1>>(npc, fp);
  }

  void set_freq_to_file(string fp) { freq_out_file.open (fp, ios::out | ios::app); f_save_freq = &algorithm_pcim::freq_to_file; }
  void set_seed_to_file(string fp) { seed_out_file.open (fp, ios::out | ios::app); f_save_seed = &algorithm_pcim::seed_to_file; }


  // VIRTUAL FUNCTIONS
  virtual void init() = 0;
  virtual void iteration_init() = 0;
  virtual void tile_creation(int index) = 0;
  virtual void iteration_end() = 0;
  virtual void end() = 0;

  algorithm_pcim(int _iterations, int _tile_size, int _n_total_probes, std::vector<int> _lgn, int _npc) :
    lgn(_lgn), bulk_tile(new bulk_cout<T1>(_npc))
  {
    iterations = _iterations;
    tile_size = _tile_size;
    n_total_probes = _n_total_probes;
    npc = _npc;
    //    bulk_tile = make_unique<bulk_to_file_async> (_npc, _tile_file_prefix);

    subset_size = tile_size - lgn.size();
    calculate_tile_number();
  }


  int run() {
    init();
    for (int i=0; i<iterations; i++) {
      iteration_init();
      for (int j=0; j<tile_number; j++) {
        tile_creation(j);
        bulk_tile->insert(move(tile_ptr));
      }
      iteration_end();
    }
    (this->*f_save_freq)();
    (this->*f_save_seed)();
    end();
    return EXIT_SUCCESS;
  }
  ~algorithm_pcim() {
    if(freq_out_file.is_open()) freq_out_file.close();
    if(seed_out_file.is_open()) seed_out_file.close();
  }
};

#endif
