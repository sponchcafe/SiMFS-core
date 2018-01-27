add_executable(ph2 ./excitation_action.cpp ./emission_action.cpp ./ph2.cpp)
target_link_libraries(ph2 graph sim getopt json)
