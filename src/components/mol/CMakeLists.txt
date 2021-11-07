###############################################################################
# Diffusion Component
###############################################################################

add_library(sim_dif 
    src/diffusion/component.cpp 
    src/diffusion/capi.cpp
)

target_include_directories(sim_dif PUBLIC ./include)
target_link_libraries(sim_dif sim_util sim_component)

add_executable(simfs_dif src/diffusion/simfs_dif.cpp)
target_link_libraries(simfs_dif sim_dif)

###############################################################################
# Conformation Component
###############################################################################

add_library(sim_cnf src/conformation/component.cpp)
target_include_directories(sim_cnf PUBLIC ./include)
target_link_libraries(sim_cnf sim_graph sim_component)

add_executable(simfs_cnf src/conformation/simfs_cnf.cpp)
target_link_libraries(simfs_cnf sim_cnf)

###############################################################################
# Shifter Component
###############################################################################

add_library(sim_sft src/shift/component.cpp)
target_include_directories(sim_sft PUBLIC ./include)
target_link_libraries(sim_sft sim_component)

add_executable(simfs_sft src/shift/simfs_sft.cpp)
target_link_libraries(simfs_sft sim_sft)
