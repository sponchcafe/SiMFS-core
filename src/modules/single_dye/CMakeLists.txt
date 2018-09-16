FILE(GLOB sim_single_dye_src src/*.cpp)
add_library(sim_single_dye ${sim_single_dye_src})
target_include_directories(sim_single_dye PUBLIC ./include)
target_link_libraries(sim_single_dye
    sim_component 
    sim_buffer 
    sim_diffusion 
    sim_excitation 
    sim_detection
    sim_photophysics
    sim_splitter
    )

add_executable(simfs_single_dye src/simfs_single_dye.cpp)
target_link_libraries(simfs_single_dye sim_single_dye)
