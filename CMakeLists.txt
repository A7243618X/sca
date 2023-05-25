cmake_minimum_required(VERSION 3.22)
project(sca)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(sca main.cpp FileImpl.cpp)

add_executable(Google_Tests_run FileImplTests.cpp FileImpl.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)
