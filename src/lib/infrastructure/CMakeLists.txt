# infrastructure library
FILE(GLOB infra_src src/*.cpp)
add_library(sim_infra ${infra_src})
target_include_directories(sim_infra PUBLIC ./include)
target_link_libraries(sim_infra json)
target_link_libraries(sim_infra getopt)

# Tests
add_subdirectory(tests)
