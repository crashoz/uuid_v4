cmake_minimum_required (VERSION 3.8)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

project (uuid_v4)

option(test "Build all tests." OFF)
option(test_use_internal_googletest "Build googletest, do not use it from system." ON)
option(benchmark "Build benchmarks." OFF)
option(benchmark_use_internal_googlebenchmark "Build google benchmark, do not use it from system." ON)

IF (test_use_internal_googletest)
  # build tests (targets: gtest_main, gtest)
  add_subdirectory(vendor/google/googletest/googletest)
  target_compile_options (gtest PRIVATE "-Wno-error=misleading-indentation")
  add_library(GTest::gtest INTERFACE IMPORTED)
  target_link_libraries(GTest::gtest INTERFACE gtest)
  add_library(GTest::gtest_main INTERFACE IMPORTED)
  target_link_libraries(GTest::gtest_main INTERFACE gtest_main)
ELSE ()
  find_package(GTest REQUIRED)
ENDIF (test_use_internal_googletest)

IF (test)
  enable_testing ()
  add_subdirectory (tests)
ENDIF (test)

IF (benchmark_use_internal_googlebenchmark)
  # build google benchmark (target: benchmark)
  # do not build tests of benchmarking lib
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)
  add_subdirectory(vendor/google/benchmark)
  add_library(benchmark::benchmark INTERFACE IMPORTED)
  target_link_libraries(benchmark::benchmark INTERFACE benchmark)
ELSE ()
  find_package(benchmark REQUIRED)
ENDIF (benchmark_use_internal_googlebenchmark)

IF (benchmark)
  add_subdirectory (benchmarks)
ENDIF (benchmark)

add_executable(example example.cpp)
target_link_libraries (example)
