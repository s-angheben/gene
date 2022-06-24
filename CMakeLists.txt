cmake_minimum_required(VERSION 3.10)

project(gene)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

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