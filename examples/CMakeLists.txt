cmake_minimum_required(VERSION 3.23)

project(
  AetherX6200ControlTest
  VERSION 0.1
  LANGUAGES C)

add_executable(x6200_atu atu.c)
add_executable(x6200_flow flow.c)
add_executable(x6200_ptt ptt.c)
add_executable(x6200_vfo vfo.c)

target_link_libraries(x6200_atu PRIVATE aether_x6200_control)
target_link_libraries(x6200_flow PRIVATE aether_x6200_control)
target_link_libraries(x6200_flow PRIVATE liquid)
target_link_libraries(x6200_ptt PRIVATE aether_x6200_control)
target_link_libraries(x6200_vfo PRIVATE aether_x6200_control)
