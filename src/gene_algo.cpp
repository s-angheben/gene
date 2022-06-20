#include <algorithm>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <ostream>

void vshuffle(std::vector<int> & v, std::vector<int>::size_type n, std::mt19937 & generator) {
  int max = v.size();
  for(int i = 0; i < n; i++) {
    std::uniform_int_distribution<> dis(i, max - 1);
    int random = dis(generator);
    std::swap(v[i], v[random]);
  }
}

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v) {
    for (auto& elem : v) {
        os << elem << " ";
    }
    return os;
}


void pcim(int iterations, int tile_size, int n_total_probes, const std::vector<int>& lgn) {
    auto lgn_size = lgn.size();
    auto subset_size = tile_size - lgn_size;

    std::vector<int> other_probes(n_total_probes);
    iota(other_probes.begin(), other_probes.end(), 0);
    std::cout << other_probes << std::endl;

    for (int i=0; i != lgn_size; i++) {
        std::swap(other_probes[lgn[i]], other_probes[n_total_probes - i - 1]);
    }
    other_probes.erase(other_probes.end() - lgn_size, other_probes.end());
    std::cout << other_probes << std::endl;

    // unsigned seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    unsigned seed = 1;
    std::mt19937 generator(seed);

    for (int i=0; i<iterations; i++) {
        std::cout << "ITERATION " << i << std::endl;

        shuffle(other_probes.begin(), other_probes.end(), generator);
        std::cout << other_probes << std::endl;

        int j=0;
        std::vector<int> tile (tile_size);
        for (j=0; j<other_probes.size()/subset_size; j++) {
            std::copy(lgn.begin(), lgn.end(), tile.begin());
            std::copy(other_probes.begin() + (j * subset_size),
                      other_probes.begin() + (j * subset_size + subset_size),
                      tile.begin()+lgn_size);
            vshuffle(tile, tile_size, generator);
            std::cout << "TILE" << j << ": " << tile << std::endl;
        }
        if (int available = (other_probes.size() - j * subset_size)) {             // if zero skip
            int need = subset_size - available;
            std::cout << "missing " << need << std::endl;
            std::cout << "available " << available << std::endl;
            std::copy(lgn.begin(), lgn.end(), tile.begin());
            std::copy(other_probes.begin() + (j * subset_size),
                      other_probes.end(),
                      tile.begin()+lgn_size);

            std::copy(other_probes.begin(),                                   //always the beginnig, maybe not random enough
                      other_probes.begin()+need,
                      tile.begin()+lgn_size+available);

            vshuffle(tile, tile_size, generator);

            std::cout << "TILE" << ++j << ": " << tile << std::endl;

            //save the frequency
            //save the idexes used twice (last copy)
            std::vector<int> used_twice(need);
            std::copy(other_probes.begin(),
                      other_probes.begin()+need,
                      used_twice.begin());
            std::cout << "used twice: " << used_twice << std::endl;
        }
    }

}

