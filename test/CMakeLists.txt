add_executable(hello_test hello_test.cc)



target_link_directories(hello_test PUBLIC
                          "${PROJECT_SOURCE_DIR}/src"
                       )

target_include_directories(hello_test PUBLIC
                          "${PROJECT_SOURCE_DIR}/src"
                          )

target_link_libraries(hello_test gtest_main gene_algo)

include(GoogleTest)
gtest_discover_tests(hello_test)

