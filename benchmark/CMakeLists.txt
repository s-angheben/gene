add_executable(bench_algo bench.cpp)

target_link_libraries(bench_algo benchmark::benchmark)
target_link_libraries(bench_algo algorithm_pcim vector_pcim vector_random_pcim)

target_link_directories(bench_algo PUBLIC "${PROJECT_SOURCE_DIR}/src/computation")
target_include_directories(bench_algo PUBLIC "${PROJECT_SOURCE_DIR}/src/computation")


add_executable(bench_writer bench_writer.cpp)

target_link_libraries(bench_writer benchmark::benchmark)
target_link_libraries(bench_writer algorithm_pcim vector_pcim vector_random_pcim)

target_link_directories(bench_writer PUBLIC "${PROJECT_SOURCE_DIR}/src/computation")
target_include_directories(bench_writer PUBLIC "${PROJECT_SOURCE_DIR}/src/computation")
