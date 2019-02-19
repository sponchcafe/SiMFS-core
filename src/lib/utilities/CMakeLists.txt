###############################################################################
# Utility library
###############################################################################

add_library(sim_util src/random.cpp src/env.cpp)
target_include_directories(sim_util PUBLIC ./include)

add_library(sim_baseIO src/baseIO.cpp)
target_include_directories(sim_baseIO PUBLIC ./include)
