add_executable(gene main.cpp algorithm_pcim.hpp algorithm_pcim.cpp vector_pcim.hpp vector_pcim.cpp)
install(TARGETS gene DESTINATION "${PROJECT_SOURCE_DIR}/build/bin")

add_library(algorithm_pcim algorithm_pcim.hpp algorithm_pcim.cpp)
add_library(vector_pcim vector_pcim.hpp vector_pcim.cpp)

#install(TARGETS gene_algo DESTINATION lib)
#install(TARGETS gene_algo DESTINATION "${PROJECT_SOURCE_DIR}/build/lib/")
#install(FILES gene_algo.hpp DESTINATION include)
