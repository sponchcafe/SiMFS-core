set(TARGET_NAME test_mix)

add_library(${TARGET_NAME} OBJECT test_mix.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC sim_mix sim_component catch2)

