FILE(GLOB sim_buffer_src src/buffer/*.cpp)
add_library(sim_buffer ${sim_buffer_src})
target_include_directories(sim_buffer PUBLIC ./include)
target_link_libraries(sim_buffer sim_component)

add_executable(simfs_buf src/buffer/simfs_buf.cpp)
target_link_libraries(simfs_buf sim_buffer)

FILE(GLOB sim_mixer_src src/mixer/*.cpp)
add_library(sim_mixer ${sim_mixer_src})
target_include_directories(sim_mixer PUBLIC ./include)
target_link_libraries(sim_mixer sim_random sim_component)

add_executable(simfs_mix src/mixer/simfs_mix.cpp)
target_link_libraries(simfs_mix sim_mixer sim_component)

FILE(GLOB sim_splitter_src src/splitter/*.cpp)
add_library(sim_splitter ${sim_splitter_src})
target_include_directories(sim_splitter PUBLIC ./include)
target_link_libraries(sim_splitter sim_random sim_component)

add_executable(simfs_spl src/splitter/simfs_spl.cpp)
target_link_libraries(simfs_spl sim_splitter sim_component)
