###############################################################################
# FCS Component
###############################################################################

add_library(sim_fcs src/fcs.cpp)
target_include_directories(sim_fcs PUBLIC ./include)
target_link_libraries(sim_fcs sim_focus sim_component)

add_executable(simfs_fcs src/simfs_fcs.cpp)
target_link_libraries(simfs_fcs sim_fcs)

###############################################################################
# Pulse Component
###############################################################################

add_library(sim_pls src/pulse.cpp)
target_include_directories(sim_pls PUBLIC ./include)
target_link_libraries(sim_pls sim_component)

add_executable(simfs_pls src/simfs_pls.cpp)
target_link_libraries(simfs_pls sim_pls)

###############################################################################
# Precalc Component
###############################################################################

add_library(sim_pre src/precalc.cpp)
target_include_directories(sim_pre PUBLIC ./include)
target_link_libraries(sim_pre sim_component sim_focus)

add_executable(simfs_pre src/simfs_pre.cpp)
target_link_libraries(simfs_pre sim_pre)
