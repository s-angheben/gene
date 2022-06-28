#include <cstdlib>
#include <iostream>
#include <algorithm_pcim.hpp>
#include <vector_pcim.hpp>
#include <parse.hpp>

int main(int argc, char **argv) {

    algorithm_pcim* algo;
    algo = process_command_line(argc, argv);

    if(algo == nullptr) {
        return EXIT_FAILURE;
    }

    algo->run();

    delete(algo);

    return EXIT_SUCCESS;
}
