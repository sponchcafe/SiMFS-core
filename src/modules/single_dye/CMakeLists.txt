add_executable(simfs_single_dye src/simfs_single_dye.cpp)
target_link_libraries(simfs_single_dye 
    sim_component 
    sim_buffer 
    sim_diffusion 
    sim_excitation 
    sim_detection
    sim_photophysics
    sim_splitter
    )
