# At LEAST 2.8 but newer is better
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(SiMFS-Tk VERSION 1.0 LANGUAGES CXX)

if(MSVC)
    set(CMAKE_CXX_FLAGS "/W4 /arch:AVX2 /O2")
else()
    set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -Werror -pedantic -O3 -march=native -pthread")
	if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
		add_compile_options("-stdlib=libc++")
	else()
        # nothing special for gcc at the moment
    endif()
endif()


enable_testing()
# RANDOM LIB defined in this library folder CMakeLists.txt
add_subdirectory(${CMAKE_SOURCE_DIR}/src/lib/)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/components/)
add_subdirectory(${CMAKE_SOURCE_DIR}/tests/)
