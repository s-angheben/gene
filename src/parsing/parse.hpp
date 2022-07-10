#include <boost/program_options.hpp>
#include <boost/spirit/include/qi.hpp>
#include <string>
namespace po = boost::program_options;
namespace qi = boost::spirit::qi;

#include <iostream>
#include <iterator>
#include <vector>
#include <unordered_map>
#include <string>

#include <algorithm_pcim.hpp>
#include <vector_pcim.hpp>
#include <vector_random_pcim.hpp>

using namespace std;


enum Algo_type {vfds, vfsi, vri};
unordered_map<string,Algo_type> alto_type_map = {
    {"vfds", vfds},
    {"vfsi", vfsi},
    {"vri", vri},
};

enum Gener_type {gdebug, grandom, gcustom};
unordered_map<string,Gener_type> gener_type_map = {
    {"debug", gdebug},
    {"random", grandom},
    {"custom", gcustom},
};


typedef struct Algo_config {
    int iterations;
    std::vector<int> lgn;
    int tile_size;
    int v_size;
    Algo_type algo_type;
    Gener_type gener_type;
    string output_file;
} Algo_config;

algo * create_algo(Algo_config* config);

Algo_config * process_command_line(int ac, char* av[])
{
    Algo_config* config = new Algo_config;
    try {
        string lgn_string;
        string algo_string;
        string gener_string;


        po::options_description desc("Program Usage", 1024, 512);
        desc.add_options()
            ("help,h", "produce help message")
            ("generator,g", po::value<string>(&gener_string)->default_value("random"), "set generator [random(seed=clock), debug(seed=1)]")
            ("iterations,i", po::value<int>(&config->iterations)->default_value(1), "set iterations value, default set to 1")
            ("algorithm,a", po::value<string>(&algo_string)->default_value("vfds"), "set to vector with double shuffle. [vfds vfsi vri]")
            ("lgn", po::value<string>(&lgn_string)->required(), "set lgn")
            ("tile_size,t", po::value<int>(&config->tile_size)->required(), "set tile_size")
            ("size,s", po::value<int>(&config->v_size)->required(), "set total size")
            ("output_file,o", po::value<string>(&config->output_file)->default_value(""), "set output_file")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(ac, av, desc), vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return nullptr;
        }

        po::notify(vm);

        // set algorithm
        if (alto_type_map.find(algo_string) == alto_type_map.end()) {
            throw po::error("invalid algorithm");
        } else {
            config->algo_type = alto_type_map.at(algo_string);
        }

        // set generator
        if (gener_type_map.find(gener_string) == gener_type_map.end()) {
            throw po::error("invalid generator");
        } else {
            config->gener_type = gener_type_map.at(gener_string);
        }

        // set lgn
        auto f = lgn_string.cbegin(), l = lgn_string.cend();
        qi::phrase_parse(f, l, *(qi::int_), qi::space, config->lgn);
        if (f!=l) {
                std::cout << "Unparsed input '" << std::string(f,l) << "'\n";
                throw po::validation_error(
                            po::validation_error::invalid_option_value,
                            "lgn", string(f,l));

        }

        // check lgn indexes are < than total size
        bool valid_indexes = true;
        auto it = config->lgn.begin();
        for (; it!=config->lgn.end() && valid_indexes; ++it) {
            valid_indexes = *it < config->v_size && *it >= 0;
        }
        if (!valid_indexes) {
            throw po::error("lgn index not valid: " + to_string(*--it));
        }

        // check sizes
        if (config->tile_size < config->lgn.size()) {
            throw po::error("tile_size < lgn_size()");
        }

        cout << "PARSED CONFIG: " << endl;
        cout << "algorithm: " << config->algo_type << " -> " << algo_string << endl;
        cout << "generator: " << config->gener_type << " -> " << gener_string << endl;
        cout << "total size: " << config->v_size << endl;
        cout << "tile size: " << config->tile_size << endl;
        cout << "lgn size: " << config->lgn.size() << endl;
        cout << "iterations: " << config->iterations << endl << endl;
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return nullptr;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
        return nullptr;
    }

    return config;
}

algo * create_algo(Algo_config* config) {
    algo* algo;

    // set algorithm
    auto algo_type = config->algo_type;
    if(algo_type == vfds) {
        algo = new vector_pcim(config->iterations, config->tile_size, config->v_size, config->lgn);
    } else if (algo_type == vfsi) {
        auto algo_tmp = new vector_pcim(config->iterations, config->tile_size, config->v_size, config->lgn);
        algo_tmp->set_tile_creation_lgn_insert();
        algo = algo_tmp;
    } else if (algo_type == vri) {
        algo = new vector_random_pcim(config->iterations, config->tile_size, config->v_size, config->lgn);
    }

    // set generator
    switch (config->gener_type)
    {
    case grandom:
        algo->set_random_generator();
        break;
    case gdebug:
        algo->set_debug_generator();
        break;
    case gcustom:
    //    algo->set_custom_generator(4);
        break;
    default:
        break;
    }

    if (!config->output_file.empty()) {
        algo->set_fp(config->output_file);
        algo->set_tile_to_file();
        algo->set_freq_to_file();
        algo->set_seed_to_file();
    }

    return algo;
}
