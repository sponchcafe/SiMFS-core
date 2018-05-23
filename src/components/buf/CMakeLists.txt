FILE(GLOB sim_buffer_src src/*.cpp)
add_library(sim_buffer ${sim_buffer_src})
target_include_directories(sim_buffer PUBLIC ./include)
target_link_libraries(sim_buffer sim_component)

add_executable(simfs_buf src/simfs_buf.cpp)
target_link_libraries(simfs_buf sim_buffer sim_component)
