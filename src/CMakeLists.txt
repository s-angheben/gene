add_executable(gene main.cpp gene_algo.hpp gene_algo.cpp)
install(TARGETS gene DESTINATION "${PROJECT_SOURCE_DIR}/build/bin")

add_library(gene_algo gene_algo.hpp gene_algo.cpp)
#install(TARGETS gene_algo DESTINATION lib)
#install(TARGETS gene_algo DESTINATION "${PROJECT_SOURCE_DIR}/build/lib/")
#install(FILES gene_algo.hpp DESTINATION include)
