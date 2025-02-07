# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.10)

find_package(doctest REQUIRED)

# Set the project name
project(Planets)

# Add an executable target
add_executable(Planets main.c)

# Make test executable
add_executable(tests test/doctest_main.cpp test/test.cpp)
target_compile_features(tests PRIVATE cxx_std_17)
target_link_libraries(tests PRIVATE doctest::doctest)