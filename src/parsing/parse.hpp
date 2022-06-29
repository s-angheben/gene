#include <boost/program_options.hpp>
#include <boost/spirit/include/qi.hpp>
#include <string>
namespace po = boost::program_options;
namespace qi = boost::spirit::qi;

#include <iostream>
#include <iterator>
#include <vector>
#include <string>

#include <algorithm_pcim.hpp>
#include <vector_pcim.hpp>
#include <vector_random_pcim.hpp>

using namespace std;

algorithm_pcim * process_command_line(int ac, char* av[])
{
    algorithm_pcim* algo = nullptr;
    try {
        int iterations;
        string lgn_string;
        std::vector<int> lgn;
        int tile_size;
        int v_size;

        string set_generator;

        po::options_description desc("Program Usage", 1024, 512);
        desc.add_options()
            ("help,h,h", "produce help message")
            ("generator,g", po::value<string>(&set_generator)->default_value("random"), "set generator [random(seed=clock), debug(seed=1)]")
            ("iterations,i", po::value<int>(&iterations)->default_value(1), "set iterations value, default set to 1")
            ("lgn", po::value<string>(&lgn_string)->required(), "set lgn")
            ("tile_size,t", po::value<int>(&tile_size)->required(), "set tile_size")
            ("size,s", po::value<int>(&v_size)->required(), "set total size")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(ac, av, desc), vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return nullptr;
        }

        po::notify(vm);

        auto f = lgn_string.cbegin(), l = lgn_string.cend();
        qi::phrase_parse(f, l, *(qi::int_), qi::space, lgn);
        if (f!=l) {
                std::cout << "Unparsed input '" << std::string(f,l) << "'\n";
                throw po::validation_error(
                            po::validation_error::invalid_option_value,
                            "lgn", string(f,l));

        }

        // check lgn indexes are < than total size
        bool valid_indexes = true;
        auto it = lgn.begin();
        for (; it!=lgn.end() && valid_indexes; ++it) {
            valid_indexes = *it < v_size && *it >= 0;
        }
        if (!valid_indexes) {
            throw po::error("lgn index not valid: " + to_string(*--it));
        }

        if (tile_size < lgn.size()) {
            throw po::error("tile_size < lgn_size()");
        }

        cout << "SETTINGS: " << endl;
        cout << "total size: " << v_size << endl;
        cout << "tile size: " << tile_size << endl;
        cout << "lgn size: " << lgn.size() << endl;
        cout << "iterations: " << iterations << endl;

//        algo = new vector_pcim(iterations, tile_size, v_size, lgn);

        algo = new vector_random_pcim(iterations, tile_size, v_size, lgn);

        if (vm.count("generator")) {
            if(set_generator == "random") {
                cout << "random generator" << endl;
            } else if (set_generator == "debug") {
                cout << "debug generator" << endl;
                algo->set_generator(debug_generator);
            } else {
                throw po::validation_error(
                            po::validation_error::invalid_option_value,
                            "generator", string(set_generator));
            }
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return nullptr;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
        return nullptr;
    }

    return algo;
}

