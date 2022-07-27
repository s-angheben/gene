add_library(parser parse.hpp)

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)

include_directories( ${Boost_INCLUDE_DIRS} )

target_link_libraries(parser ${Boost_LIBRARIES} algorithm_pcim vector_pcim vector_random_pcim vector_extract_pcim)

target_include_directories(parser PUBLIC "${PROJECT_SOURCE_DIR}/src/computation")


