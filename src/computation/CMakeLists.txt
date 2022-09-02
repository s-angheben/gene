#add_library(algorithm_pcim algorithm_pcim.hpp algorithm_pcim.cpp)
add_library(algorithm_pcim algorithm_pcim.hpp algorithm_pcim.cpp)
set_target_properties(algorithm_pcim PROPERTIES LINKER_LANGUAGE CXX)

add_library(vector_pcim vector_pcim.hpp vector_pcim.cpp)
add_library(vector_random_pcim vector_random_pcim.hpp vector_random_pcim.cpp)
add_library(vector_extract_pcim vector_extract_pcim.hpp vector_extract_pcim.cpp)
add_library(bulk_writer writer.hpp writer.cpp)
