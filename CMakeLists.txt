# At LEAST 2.8 but newer is better
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(FCS VERSION 1.0 LANGUAGES CXX)

add_compile_options(-Wall -Wextra -pedantic -Werror)
set(CMAKE_CXX_FLAGS "-std=gnu++14 -Wall -O3 -march=native -pthread")

enable_testing()
# RANDOM LIB defined in this library folder CMakeLists.txt
add_subdirectory(${CMAKE_SOURCE_DIR}/src/lib/)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/components/)
add_subdirectory(${CMAKE_SOURCE_DIR}/tests/)
