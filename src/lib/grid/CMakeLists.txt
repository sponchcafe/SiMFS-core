###############################################################################
# Grid jlibrary
###############################################################################

FILE(GLOB sim_grid_src src/grid/*.cpp)
add_library(sim_grid ${sim_grid_src})
target_link_libraries(sim_grid PUBLIC sim_util)
target_include_directories(sim_grid PUBLIC ./include)
