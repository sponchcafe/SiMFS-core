add_library(sim_component INTERFACE)
target_link_libraries(sim_component INTERFACE sim_util)
target_include_directories(sim_component INTERFACE ./include) 

add_subdirectory(tests)

