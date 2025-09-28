cmake_minimum_required(VERSION 3.28)
project(RayTracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(RayTracer
        src/main.cpp
        src/rt/geom/hittable.cpp
        src/rt/geom/hittable_list.cpp
        src/rt/geom/sphere.cpp
        src/rt/lighting/ambient_light.cpp
        src/rt/lighting/directional_light.cpp
        src/rt/lighting/light_list.cpp
        src/rt/lighting/point_light.cpp
        src/rt/render/render.cpp
)

# Include headers
target_include_directories(RayTracer PUBLIC
        include
)

# Warnings & debug flags (target-scoped)
target_compile_options(RayTracer PRIVATE
        -Wall -Wextra -Wpedantic -Werror -ggdb
)

# Sanitizers (both compile & link)
target_compile_options(RayTracer PRIVATE -fsanitize=address,undefined)
target_link_options(RayTracer PRIVATE -fsanitize=address,undefined)

# Put binaries under build
set_target_properties(RayTracer PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
