cmake_minimum_required(VERSION 3.10)
project(ecs CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ecs inc/ecs/types.h inc/ecs/containers/array.h)
target_include_directories(ecs PUBLIC inc PRIVATE inc/ecs)
set_target_properties(ecs PROPERTIES LINKER_LANGUAGE CXX)
