# infrastructure library
FILE(GLOB sim_cmdutils_src src/*.cpp)
add_library(sim_cmdutils ${sim_cmdutils_src})
target_include_directories(sim_cmdutils PUBLIC ./include)

# Tests
add_subdirectory(tests)
