FILE(GLOB sim_conformation_src src/*.cpp)
add_library(sim_conformation ${sim_conformation_src})
target_include_directories(sim_conformation PUBLIC ./include)
target_link_libraries(sim_conformation sim_graph sim_component)

add_executable(simfs_cnf src/simfs_cnf.cpp)
target_link_libraries(simfs_cnf sim_conformation sim_component)
