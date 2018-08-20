FILE(GLOB sim_diffusion_src src/*.cpp)
add_library(sim_diffusion ${sim_diffusion_src})
target_include_directories(sim_diffusion PUBLIC ./include)
target_link_libraries(sim_diffusion sim_util sim_component)

add_executable(simfs_dif src/simfs_dif.cpp)
target_link_libraries(simfs_dif sim_diffusion sim_component)
