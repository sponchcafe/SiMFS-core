###############################################################################
# FCS Component
###############################################################################

add_library(sim_fcs src/focus/component.cpp)
target_include_directories(sim_fcs PUBLIC ./include)
target_link_libraries(sim_fcs sim_focus sim_component)

add_executable(simfs_fcs src/focus/simfs_fcs.cpp)
target_link_libraries(simfs_fcs sim_fcs)

###############################################################################
# Pulse Component
###############################################################################

add_library(sim_pls src/pulse/component.cpp)
target_include_directories(sim_pls PUBLIC ./include)
target_link_libraries(sim_pls sim_component)

add_executable(simfs_pls src/pulse/simfs_pls.cpp)
target_link_libraries(simfs_pls sim_pls)

###############################################################################
# Efield Component
###############################################################################

add_library(sim_efi src/efield/component.cpp)
target_include_directories(sim_efi PUBLIC ./include)
target_link_libraries(sim_efi sim_component sim_focus)

add_executable(simfs_efi src/efield/simfs_efi.cpp)
target_link_libraries(simfs_efi sim_efi)
