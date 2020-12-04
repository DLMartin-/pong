cmake_minimum_required(VERSION 3.10)
project(pongchan)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(libs)


add_executable(pongchan src/main.cpp)
target_link_libraries(pongchan z png SDL2_image SDL2)
target_link_libraries(pongchan ecs)

add_custom_command(TARGET pongchan POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:pongchan>/res)
