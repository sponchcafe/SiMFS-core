add_executable(diffusion diffusion.cpp)
target_link_libraries(diffusion sim random)

add_executable(exi_alpha exi_alpha.cpp)
target_link_libraries(exi_alpha sim sim_focus)
