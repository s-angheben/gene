add_executable(gene main.cpp)
target_link_directories(gene PUBLIC "${PROJECT_SOURCE_DIR}/src/computation")

target_include_directories(gene PUBLIC "${PROJECT_SOURCE_DIR}/src/computation")
target_link_libraries(gene algorithm_pcim vector_pcim)
install(TARGETS gene DESTINATION "${PROJECT_SOURCE_DIR}/build/bin")

add_subdirectory(computation)
add_subdirectory(parsing)

#install(TARGETS gene_algo DESTINATION lib)
#install(TARGETS gene_algo DESTINATION "${PROJECT_SOURCE_DIR}/build/lib/")
#install(FILES gene_algo.hpp DESTINATION include)
