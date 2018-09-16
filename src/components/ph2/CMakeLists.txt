###############################################################################
# Photophysikcs Component
###############################################################################

add_library(sim_ph2 src/photophysics/component.cpp)
target_include_directories(sim_ph2 PUBLIC ./include)
target_link_libraries(sim_ph2 sim_util sim_graph sim_component)

add_executable(simfs_ph2 src/photophysics/simfs_ph2.cpp)
target_link_libraries(simfs_ph2 sim_ph2)
