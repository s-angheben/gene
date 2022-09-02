add_executable(gene main.cpp)

target_link_directories(gene PUBLIC "${PROJECT_SOURCE_DIR}/src/computation" "${PROJECT_SOURCE_DIR}/src/parsing")

target_include_directories(gene PUBLIC "${PROJECT_SOURCE_DIR}/src/computation" "${PROJECT_SOURCE_DIR}/src/parsing")

target_link_libraries(gene algorithm_pcim vector_pcim vector_random_pcim parser)

install(TARGETS gene DESTINATION "${PROJECT_SOURCE_DIR}/build/bin")
install(TARGETS gene DESTINATION )

add_subdirectory(computation)
add_subdirectory(parsing)

#install(TARGETS gene_algo DESTINATION lib)
#install(TARGETS gene_algo DESTINATION "${PROJECT_SOURCE_DIR}/build/lib/")
#install(FILES gene_algo.hpp DESTINATION include)
