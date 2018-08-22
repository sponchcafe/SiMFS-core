###############################################################################
# Diffusion Component
###############################################################################

add_library(sim_dif src/dif.cpp)
target_include_directories(sim_dif PUBLIC ./include)
target_link_libraries(sim_dif sim_util sim_component)

add_executable(simfs_dif src/simfs_dif.cpp)
target_link_libraries(simfs_dif sim_dif)

###############################################################################
# Conformation Component
###############################################################################

add_library(sim_cnf src/cnf.cpp)
target_include_directories(sim_cnf PUBLIC ./include)
target_link_libraries(sim_cnf sim_graph sim_component)

add_executable(simfs_cnf src/simfs_cnf.cpp)
target_link_libraries(simfs_cnf sim_cnf)

###############################################################################
# Shifter Component
###############################################################################

add_library(sim_sft src/sft.cpp)
target_include_directories(sim_sft PUBLIC ./include)
target_link_libraries(sim_sft sim_component)

add_executable(simfs_sft src/simfs_sft.cpp)
target_link_libraries(simfs_sft sim_sft)
