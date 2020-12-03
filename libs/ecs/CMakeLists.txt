cmake_minimum_required(VERSION 3.10)
project(ecs CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ecs inc/ecs/object_pool.h inc/ecs/entity.h inc/ecs/component_cache.h inc/ecs/entity_factory.h)
target_include_directories(ecs PUBLIC inc PRIVATE inc/ecs)
set_target_properties(ecs PROPERTIES LINKER_LANGUAGE CXX)
