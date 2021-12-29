cmake_minimum_required (VERSION 3.8)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(uuid_v4)

option(test "Build all tests." OFF)
option(benchmark "Build benchmarks." OFF)
option(example "Build example" OFF)

set(uuid_v4_directory ${CMAKE_CURRENT_LIST_DIR}/include/uuid_v4)
set(uuid_v4_include_directory ${CMAKE_CURRENT_LIST_DIR}/include)
set(uuid_v4_source
  ${uuid_v4_directory}/uuid_v4.h
  ${uuid_v4_directory}/endianness.h
  )

add_library(uuid_v4 INTERFACE)
target_sources(uuid_v4 INTERFACE ${uuid_v4_source})
target_include_directories(uuid_v4 INTERFACE ${uuid_v4_include_directory})
#Enable AVX2
if(MSVC)
    target_compile_options(uuid_v4 INTERFACE /arch:AVX2)
else()
    target_compile_options(uuid_v4 INTERFACE -mavx2)
endif()

IF (test)
  # build tests (targets: gtest_main, gtest)
  add_subdirectory(vendor/google/googletest/googletest)
  target_compile_options (gtest PRIVATE "-Wno-error=misleading-indentation")

  enable_testing ()
  add_subdirectory (tests)
ENDIF (test)

IF (benchmark)
  # build google benchmark (target: benchmark)
  # do not build tests of benchmarking lib
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)
  add_subdirectory(vendor/google/benchmark)

  add_subdirectory (benchmarks)
ENDIF (benchmark)

IF (example)
  add_executable(example example.cpp)
  target_link_libraries(example uuid_v4)
ENDIF (example)
