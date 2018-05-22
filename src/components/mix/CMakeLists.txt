FILE(GLOB sim_mixer_src src/*.cpp)
add_library(sim_mixer ${sim_mixer_src})
target_include_directories(sim_mixer PUBLIC ./include)
target_link_libraries(sim_mixer sim_random sim_component)

add_executable(simfs_mix src/simfs_mix.cpp)
target_link_libraries(simfs_mix sim_mixer sim_component)
