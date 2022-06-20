add_executable(gene main.cpp gene_algo.hpp gene_algo.cpp)
install(TARGETS gene DESTINATION "${PROJECT_SOURCE_DIR}/bin")

add_library(gene_algo gene_algo.hpp gene_algo.cpp)
install(TARGETS gene_algo DESTINATION lib)
install(FILES gene_algo.hpp DESTINATION include)
