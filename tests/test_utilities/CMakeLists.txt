set(TARGET_NAME test_utilities)

add_library(${TARGET_NAME} OBJECT test_env.cpp test_random.cpp test_baseIO.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC sim_util sim_baseIO catch2)
