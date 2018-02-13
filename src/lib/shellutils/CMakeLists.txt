# infrastructure library
add_library(sim_shellutils INTERFACE)
target_include_directories(sim_shellutils INTERFACE ./include)

# Tests
add_subdirectory(tests)
