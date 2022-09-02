#include <chrono>
#include <numeric>
#include <ostream>
#include <random>
#include <iostream>
#include <set>
#include <algorithm>

std::ostream& operator<<(std::ostream& os, const std::set<int>& set) {
    for (auto& elem : set) {
        os << elem << " ";
    }
    return os;
}

void set_pcim(int iter, int tile_size, int n_probes, const std::set<int>& lgn) {
    int lgn_size = lgn.size();

    std::set<int> all_probes;
    for (int i=0; i<n_probes; i++) {
        all_probes.emplace_hint(all_probes.end(), i);
    }
//    std::cout << all_probes << std::endl;
    int subset_size = tile_size - lgn_size;
    std::set<int> other_probes;
    std::set_difference(
                all_probes.cbegin(), all_probes.cend(),
                lgn.cbegin(), lgn.cend(),
                std::inserter(other_probes, other_probes.end())
            );
//    std::cout << other_probes << std::endl;

//    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    unsigned seed = 1;
    std::default_random_engine e(seed);

    for(int i=0; i<iter; i++) {
        std::cout << std::endl;
        std::cout << "ITERATION: " << i;
        std::cout << std::endl;

        std::shuffle(all_probes.begin(), all_probes.end(), e);
    }
}

int main () {
    int iter = 2;
    std::set<int> lgn {22, 33};
    int tile_size = 30;
    int v_size = 100;

    set_pcim(iter, tile_size, v_size, lgn);
}
