#include <cstdlib>
#include <iostream>
#include <algorithm_pcim.hpp>
#include <vector_pcim.hpp>
#include <parse.hpp>

using namespace std;

int main(int argc, char **argv) {

    algo* a;
    a = process_command_line(argc, argv);

    if(a== nullptr) {
        cout << "error creating the algorithm_pcim" << endl;
        return EXIT_FAILURE;
    }

    a->run();

    delete(a);

    return EXIT_SUCCESS;
}
