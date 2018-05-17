FILE(GLOB sim_excitation_src src/*.cpp)
add_library(sim_excitation ${sim_excitation_src})
target_include_directories(sim_excitation PUBLIC ./include)
target_link_libraries(sim_excitation sim_focus sim_component)

add_executable(simfs_exi src/simfs_exi.cpp)
target_link_libraries(simfs_exi sim_excitation sim_component)
