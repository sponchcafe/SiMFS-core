FILE(GLOB sim_fcs_src src/gauss.cpp simfs_fcs.cpp)
add_library(sim_fcs ${sim_fcs_src})
target_include_directories(sim_fcs PUBLIC ./include)
target_link_libraries(sim_fcs sim_focus sim_component)

add_executable(simfs_fcs src/simfs_fcs.cpp)
target_link_libraries(simfs_fcs sim_fcs sim_component)
