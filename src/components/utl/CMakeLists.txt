###############################################################################
# Buffer Component
###############################################################################

add_library(sim_buf src/buffer/component.cpp)
target_include_directories(sim_buf PUBLIC ./include)
target_link_libraries(sim_buf sim_component)

add_executable(simfs_buf src/buffer/simfs_buf.cpp)
target_link_libraries(simfs_buf sim_buf)

###############################################################################
# Mixer Component
###############################################################################

add_library(sim_mix src/mixer/component.cpp)
target_include_directories(sim_mix PUBLIC ./include)
target_link_libraries(sim_mix sim_component)

add_executable(simfs_mix src/mixer/simfs_mix.cpp)
target_link_libraries(simfs_mix sim_mix)

###############################################################################
# Splitter Component
###############################################################################

add_library(sim_spl src/splitter/component.cpp)
target_include_directories(sim_spl PUBLIC ./include)
target_link_libraries(sim_spl sim_util sim_component)

add_executable(simfs_spl src/splitter/simfs_spl.cpp)
target_link_libraries(simfs_spl sim_spl)

###############################################################################
# Imaging Component
###############################################################################

add_library(sim_img src/imager/component.cpp)
target_include_directories(sim_img PUBLIC ./include)
target_link_libraries(sim_img sim_component)

add_executable(simfs_img src/imager/simfs_img.cpp)
target_link_libraries(simfs_img sim_img)

###############################################################################
# t3r writer Component
###############################################################################

add_library(sim_t3r src/t3r_writer/component.cpp)
target_include_directories(sim_t3r PUBLIC ./include)
target_link_libraries(sim_t3r sim_component)

add_executable(simfs_t3r src/t3r_writer/simfs_t3r.cpp)
target_link_libraries(simfs_t3r sim_t3r)
