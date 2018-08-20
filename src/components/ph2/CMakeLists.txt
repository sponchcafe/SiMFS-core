FILE(GLOB sim_photophysics_src src/*.cpp)
add_library(sim_photophysics ${sim_photophysics_src})
target_include_directories(sim_photophysics PUBLIC ./include)
target_link_libraries(sim_photophysics sim_graph sim_component)

add_executable(simfs_ph2 src/simfs_ph2.cpp)
target_link_libraries(simfs_ph2 sim_photophysics sim_component)
