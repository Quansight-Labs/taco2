cmake_minimum_required(VERSION 3.8)
project(taco2 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/mason.cmake)

# configure optimization
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OPTIMIZATION_FLAGS "-O0 -DDEBUG")
    message("-- Configuring debug build")
else()
    set(OPTIMIZATION_FLAGS "-O3 -DNDEBUG")
    message("-- Configuring release build")
endif()

# Enable extra warnings to adhere to https://github.com/mapbox/cpp/issues/37
set(DESIRED_WARNINGS "-Wall -Wextra -Wconversion -Wunreachable-code -Wuninitialized -pedantic-errors -Wold-style-cast -Wno-error=unused-variable -Wshadow -Wfloat-equal -Weffc++  -Werror")
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(DESIRED_WARNINGS "${DESIRED_WARNINGS} -Wmost")
endif()

# Note: -D_GLIBCXX_USE_CXX11_ABI=0 is needed to support mason packages that are precompiled libs
# Currently we only depend on a header only library, but this will help avoid issues when more libs are added via mason
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPTIMIZATION_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0 ${DESIRED_WARNINGS}")

# mason_use is a mason function within the mason.cmake file and provides ready-to-go vars, like "STATIC_LIBS" and "INCLUDE_DIRS"
mason_use(catch VERSION 2.4.0 HEADER_ONLY)
include_directories(SYSTEM ${MASON_PACKAGE_catch_INCLUDE_DIRS})

mason_use(benchmark VERSION 1.4.1)
include_directories(SYSTEM ${MASON_PACKAGE_benchmark_INCLUDE_DIRS})

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB TEST_SOURCES test/*.cpp)
add_executable(unit-tests ${TEST_SOURCES})

# libbenchmark.a supports threads and therefore needs pthread support
find_package(Threads REQUIRED)
file(GLOB BENCH_SOURCES bench/*.cpp)
add_executable(bench-tests ${BENCH_SOURCES} include/taco2/level_capabilities/locate.hpp include/taco2/level_capabilities/assembly.hpp)

# link benchmark static library to the bench-tests binary so the bench tests know where to find the benchmark impl code
target_link_libraries(bench-tests ${MASON_PACKAGE_benchmark_STATIC_LIBS} ${CMAKE_THREAD_LIBS_INIT})
