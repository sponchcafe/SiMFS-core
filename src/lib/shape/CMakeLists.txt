###############################################################################
# Focus library
###############################################################################

FILE(GLOB sim_shape_src src/shape/*.cpp)
add_library(sim_shape ${sim_shape_src})
target_link_libraries(sim_shape PUBLIC sim_util) 
target_include_directories(sim_shape PUBLIC ./include)
