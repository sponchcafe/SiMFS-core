###############################################################################
# Focus library
###############################################################################

FILE(GLOB sim_focus_src src/function/*.cpp src/efield/*.cpp)
add_library(sim_focus  ${sim_focus_src})
target_link_libraries(sim_focus PUBLIC sim_util) 
target_include_directories(sim_focus PUBLIC ./include)
