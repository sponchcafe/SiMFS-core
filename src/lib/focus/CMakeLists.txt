FILE(GLOB sim_focus_src src/*.cpp)
add_library(sim_focus ${sim_focus_src})
target_link_libraries(sim_focus PUBLIC sim_util) 
target_include_directories(sim_focus PUBLIC ./include)

# add_executable(gaussian_test tests/gaussian_test.cpp)
# target_link_libraries(gaussian_test sim_focus)
# target_link_libraries(gaussian_test gtest_main)
# add_test(gaussian_test gaussian_test)

# add_executable(alpha_gauss_test tests/alpha_gauss_test.cpp)
# target_link_libraries(alpha_gauss_test sim_focus)
# target_link_libraries(alpha_gauss_test gtest_main)
# add_test(alpha_gauss_test alpha_gauss_test)

# add_executable(efield_test tests/efield_test.cpp)
# target_link_libraries(efield_test sim_focus)
# target_link_libraries(efield_test gtest_main)
# add_test(efield_test efield_test)
