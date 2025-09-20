cmake_minimum_required(VERSION 4.0)
project(RayTracer)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(RayTracer
        include/ray.h
        include/vec3.h
        include/camera.h
        include/color.h
        src/main.cpp)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -Iinclude -ggdb -fsanitize=address -fsanitize=undefined")