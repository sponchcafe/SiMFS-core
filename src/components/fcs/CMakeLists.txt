###############################################################################
# Excitation Component
###############################################################################

add_library(sim_exi 
    src/excitation/component.cpp 
    src/focus/component.cpp
    )
target_include_directories(sim_exi PUBLIC ./include)
target_link_libraries(sim_exi sim_shape sim_component)

add_executable(simfs_exi src/excitation/simfs_exi.cpp)
target_link_libraries(simfs_exi sim_exi)

###############################################################################
# Pulse Component
###############################################################################

add_library(sim_pls src/pulse/component.cpp)
target_include_directories(sim_pls PUBLIC ./include)
target_link_libraries(sim_pls sim_component)

add_executable(simfs_pls src/pulse/simfs_pls.cpp)
target_link_libraries(simfs_pls sim_pls)
