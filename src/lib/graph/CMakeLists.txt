# graph library
FILE(GLOB sim_graph_src src/*.cpp)
add_library(sim_graph ${sim_graph_src})
target_include_directories(sim_graph PUBLIC ./include)
target_link_libraries(sim_graph sim_util sim_component)

# Tests
add_subdirectory(tests)
