add_executable(gene_test gene_algo_test.cc)



target_link_directories(gene_test PUBLIC
                          "${PROJECT_SOURCE_DIR}/src"
                       )

target_include_directories(gene_test PUBLIC
                          "${PROJECT_SOURCE_DIR}/src"
                          )

target_link_libraries(gene_test gtest_main algorithm_pcim vector_pcim)

include(GoogleTest)
gtest_discover_tests(gene_test)

