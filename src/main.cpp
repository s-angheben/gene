#include <iostream>
#include "algorithm_pcim.hpp"
#include "vector_pcim.hpp"

int main() {
    int iterations = 2;
    std::vector<int> lgn {22, 33};
    int tile_size = 30;
    int v_size = 40;


    vector_pcim algo(iterations, tile_size, v_size, lgn);
    algo.set_generator(debug_generator);
    algo.run();

    return 0;
}
