FILE(GLOB sim_splitter_src src/*.cpp)
add_library(sim_splitter ${sim_splitter_src})
target_include_directories(sim_splitter PUBLIC ./include)
target_link_libraries(sim_splitter sim_random sim_component)

add_executable(simfs_spl src/simfs_spl.cpp)
target_link_libraries(simfs_spl sim_splitter sim_component)
