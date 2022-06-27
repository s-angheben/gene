add_executable(parse_test parse.cpp)

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
include_directories( ${Boost_INCLUDE_DIRS} )
target_link_libraries(parse_test ${Boost_LIBRARIES})

#install(TARGETS gene_algo DESTINATION lib)
#install(TARGETS gene_algo DESTINATION "${PROJECT_SOURCE_DIR}/build/lib/")
#install(FILES gene_algo.hpp DESTINATION include)
