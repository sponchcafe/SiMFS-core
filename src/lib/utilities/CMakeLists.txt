###############################################################################
# Utility library
###############################################################################

add_library(sim_util src/random.cpp src/env.cpp)
target_include_directories(sim_util PUBLIC ./include)
