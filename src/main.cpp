#include <cstdlib>
#include <iostream>
#include <algorithm_pcim.hpp>
#include <vector_pcim.hpp>
#include <parse.hpp>

using namespace std;

int main(int argc, char **argv) {

    algorithm_pcim* algo;
    algo = process_command_line(argc, argv);

    if(algo == nullptr) {
        cout << "error creating the algorithm_pcim" << endl;
        return EXIT_FAILURE;
    }

    algo->run();

    delete(algo);

    return EXIT_SUCCESS;
}
