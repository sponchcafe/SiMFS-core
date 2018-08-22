FILE(GLOB sim_molecule_src src/*/*.cpp)
add_library(sim_molecule ${sim_molecule_src})
target_include_directories(sim_molecule PUBLIC ./include)
target_link_libraries(sim_molecule sim_graph sim_util sim_component)

add_executable(simfs_cnf src/conformation/simfs_cnf.cpp)
target_link_libraries(simfs_cnf sim_molecule sim_component)

add_executable(simfs_dif src/diffusion/simfs_dif.cpp)
target_link_libraries(simfs_dif sim_molecule sim_component)
