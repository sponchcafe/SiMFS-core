# infrastructure library
FILE(GLOB cmdutils_src src/*.cpp)
add_library(cmdutils ${cmdutils_src})
target_include_directories(cmdutils PUBLIC ./include)

# Tests
add_subdirectory(tests)
