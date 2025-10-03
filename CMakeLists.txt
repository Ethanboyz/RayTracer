cmake_minimum_required(VERSION 3.28)
project(RayTracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_ASAN "Debug build with AddressSanitizer, UBSanitizer" OFF)
option(ENABLE_TSAN "Debug build with ThreadSanitizer, UBSanitizer" OFF)

if (ENABLE_ASAN AND ENABLE_TSAN)
    message(FATAL_ERROR "ENABLE_ASAN and ENABLE_TSAN are mutually exclusive.")
endif()

# Source files
set(SOURCES
        src/main.cpp
        src/rt/utilities.cpp
        src/rt/geom/hittable.cpp
        src/rt/geom/hittable_list.cpp
        src/rt/geom/sphere.cpp
        src/rt/math/vec3.cpp
        src/rt/render/render.cpp
        src/rt/scene/material.cpp
)
add_executable(RayTracer ${SOURCES})
target_include_directories(RayTracer PUBLIC include)

# Warnings & debug flags (ggdb for debug presets, gcc/clang)
target_compile_options(RayTracer PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -Werror>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)
target_compile_options(RayTracer PRIVATE
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU,Clang,AppleClang>>:-ggdb>
)

# Sanitizers (both compile & link)
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    if (ENABLE_ASAN)
        target_compile_options(RayTracer PRIVATE -fsanitize=address,undefined)
        target_link_options(RayTracer PRIVATE -fsanitize=address,undefined)
    elseif(ENABLE_TSAN)
        target_compile_options(RayTracer PRIVATE -fsanitize=thread,undefined)
        target_link_options(RayTracer PRIVATE -fsanitize=thread,undefined)
    endif()
endif()

# Put binaries under build
set_target_properties(RayTracer PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
