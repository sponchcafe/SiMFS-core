add_library(sim src/options.cpp src/logging.cpp)
target_include_directories(sim PUBLIC ./include)
target_link_libraries(sim json)
target_link_libraries(sim getopt)
