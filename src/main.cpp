#include <iostream>
#include "gene_algo.hpp"

int main() {
    int iterations = 2;
    std::vector<int> lgn {22, 33};
    int tile_size = 30;
    int v_size = 30;

    algorithm_pcim algo(iterations, tile_size, v_size, lgn);

    algo.run();

    return 0;
}
