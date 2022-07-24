cmake_minimum_required(VERSION 3.10)

project(gene)

set(CMAKE_CXX_STANDARD 20)
#thread
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)


add_subdirectory(src)

# boost library for parsing
#find_package( Boost REQUIRED COMPONENTS program_options regex aaa)
include_directories( ${Boost_INCLUDE_DIRS} )

option(ENABLE_TEST "Turn on to enable tests" ON)
if (ENABLE_TEST)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_subdirectory(test)
endif()

option(ENABLE_BENCH "Turn on to enable tests" ON)
if (ENABLE_BENCH)
   find_package(benchmark REQUIRED)
   add_subdirectory(benchmark)
endif()
