# At LEAST 2.8 but newer is better
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(SiMFS-Tk VERSION 1.0 LANGUAGES CXX)

if(MSVC)
    add_compile_options(
		"/W4" 
		"/arch:AVX2"
		"$<$<CONFIG:RELEASE>:/O2>"
	)
else()
    add_compile_options(
		"-Wall" "-Wextra" "-Werror" "-pedantic"
		"$<$<CONFIG:RELEASE>:-O3>"
		"-march=native"
		"-pthread"
	)
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
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