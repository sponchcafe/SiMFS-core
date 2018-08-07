FILE(GLOB sim_component_src src/*.cpp)
add_library(sim_component ${sim_component_src})
target_include_directories(sim_component PUBLIC ./include) 
