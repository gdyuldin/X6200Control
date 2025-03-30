include_guard(GLOBAL)

include(CMakePackageConfigHelpers)

# Aether X6200 Control library
add_library(aether_radio::x6200_control ALIAS aether_x6200_control)
target_include_directories(aether_x6200_control #
                           PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
install(TARGETS aether_x6200_control)
install(TARGETS aether_x6200_control FILE_SET HEADERS)
