add_executable(gene_bench bench.cpp)

target_link_libraries(gene_bench benchmark::benchmark)
target_link_directories(gene_test PUBLIC "${PROJECT_SOURCE_DIR}/src/computation")
target_include_directories(gene_test PUBLIC "${PROJECT_SOURCE_DIR}/src/computation")
