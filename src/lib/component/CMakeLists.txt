# infrastructure library

add_library(sim_component INTERFACE)
target_include_directories(sim_component INTERFACE ./include)

add_subdirectory(tests)
