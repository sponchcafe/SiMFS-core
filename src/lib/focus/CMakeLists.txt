add_library(sim_focus INTERFACE)
target_include_directories(sim_focus INTERFACE ./include)

add_executable(test_gaussian tests/test_gaussian.cpp)
target_link_libraries(test_gaussian sim)
target_link_libraries(test_gaussian sim_focus)
target_link_libraries(test_gaussian gtest_main)
add_test(test_gaussian test_gaussian)

add_executable(test_alphaGauss tests/test_alphaGauss.cpp)
target_link_libraries(test_alphaGauss sim)
target_link_libraries(test_alphaGauss sim_focus)
target_link_libraries(test_alphaGauss gtest_main)
add_test(test_alphaGauss test_alphaGauss)
