FILE(GLOB sim_detection_src src/*.cpp)
add_library(sim_detection ${sim_detection_src})
target_include_directories(sim_detection PUBLIC ./include)
target_link_libraries(sim_detection sim_focus sim_component sim_random)

add_executable(simfs_det src/simfs_det.cpp)
target_link_libraries(simfs_det sim_detection sim_component)
