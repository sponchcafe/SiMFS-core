###############################################################################
# Component 
###############################################################################

add_library(sim_component src/io/buffer.cpp)
target_link_libraries(sim_component PUBLIC sim_util)
target_include_directories(sim_component PUBLIC ./include) 


