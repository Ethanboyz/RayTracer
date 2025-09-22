cmake_minimum_required(VERSION 3.28)
project(RayTracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(RayTracer
        src/main.cpp)
set_target_properties(RayTracer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -Iinclude -ggdb -fsanitize=address -fsanitize=undefined")