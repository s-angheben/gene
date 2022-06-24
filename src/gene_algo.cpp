#include <algorithm>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <ostream>

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v) {
    for (auto& elem : v) {
        os << elem << " ";
    }
    return os;
}

std::mt19937 setup_generator() {
    // unsigned seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    unsigned seed = 1;
    std::mt19937 generator(seed);
    return generator;
}

void setup_other_probes(std::vector<int>& other_probes, const std::vector<int>& lgn) {
    iota(other_probes.begin(), other_probes.end(), 0);

    for (int i=0; i != lgn.size(); i++) {
        std::swap(other_probes[lgn[i]], other_probes[other_probes.size() - i - 1]);
    }
    other_probes.erase(other_probes.end() - lgn.size(), other_probes.end());
}

int tail_to_do(std::vector<int>& other_probes, int subset_size, std::vector<int>& frequency){
    int tail_number=other_probes.size()/subset_size;

    // add to the end other elements if necessary
    int available = (other_probes.size() - tail_number * subset_size);
    if (available) {            // if zero skip
        int need = subset_size - available;
        other_probes.insert(other_probes.end(), other_probes.begin(), other_probes.begin()+need);

        // save frequency
        frequency.insert(frequency.begin(),
                        other_probes.begin(),
                        other_probes.begin()+need);
        tail_number++;
    }

    return tail_number;
}

void create_tail(std::vector<int>& tile, int index, const std::vector<int>& other_probes, const std::vector<int>& lgn, int subset_size, std::mt19937 generator) {
    std::copy(lgn.begin(), lgn.end(), tile.begin());
    std::copy(other_probes.begin() + (index * subset_size),
              other_probes.begin() + (index * subset_size + subset_size),
              tile.begin()+lgn.size());
    shuffle(tile.begin(), tile.end(), generator);
}

void pcim(int iterations, int tile_size, int n_total_probes, const std::vector<int>& lgn) {
    auto lgn_size = lgn.size();
    auto subset_size = tile_size - lgn_size;

    std::vector<int> other_probes(n_total_probes);
    setup_other_probes(other_probes, lgn);
    std::cout << other_probes << std::endl;

    std::mt19937 generator = setup_generator();

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

}

