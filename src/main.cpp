#include <cstdlib>
#include <iostream>
#include <algorithm_pcim.hpp>
#include <vector_pcim.hpp>
#include <parse.hpp>

using namespace std;

int main(int argc, char **argv) {

    unique_ptr<algo> a;
    unique_ptr<Algo_config> config;
    config = process_command_line(argc, argv);

    if (config == nullptr) {
        cout << "error parsing the configurations" << endl;
        return EXIT_FAILURE;
    }

    a = (move(create_algo(config)));

    if(a == nullptr) {
        cout << "error creating the algorithm_pcim" << endl;
        return EXIT_FAILURE;
    }

    a->run();

    return EXIT_SUCCESS;
}
