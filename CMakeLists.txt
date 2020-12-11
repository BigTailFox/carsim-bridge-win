CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(carsim-bridge)

SET(include_dirs common lcm msg/carsim ${CMAKE_SOURCE_DIR})
SET(link_dirs common lcm/lib)

include_directories(${include_dirs})
link_directories(${link_dirs})

add_subdirectory(common)
add_subdirectory(msg/carsim)

add_executable(${PROJECT_NAME} "carsim-bridge.c" "external.c")
target_link_libraries(${PROJECT_NAME} vs_get_api lcm msg)