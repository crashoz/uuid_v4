cmake_minimum_required(VERSION 3.16...3.30)

# Set default build type before project()
if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

project(uuid_v4
  VERSION 1.0.0
  LANGUAGES CXX
  DESCRIPTION "Fast and header-only UUID version 4 generator"
)

# Options
option(UUID_V4_BUILD_TESTS "Build all tests" OFF)
option(UUID_V4_USE_INTERNAL_GTEST "Build googletest instead of using system version" ON)
option(UUID_V4_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(UUID_V4_USE_INTERNAL_GBENCH "Build google benchmark instead of using system version" ON)

# Main library target (header-only interface library)
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Enable AVX2 for all configurations (required by uuid_v4.h)
  target_compile_options(${PROJECT_NAME} INTERFACE
    -march=native
  )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  # Enable AVX2 for all configurations
  target_compile_options(${PROJECT_NAME} INTERFACE
    /arch:AVX2
    $<$<CONFIG:Release>:/MT>
    $<$<CONFIG:Debug>:/MTd>
  )
endif()

# Testing support
if(UUID_V4_BUILD_TESTS)
  if(UUID_V4_USE_INTERNAL_GTEST)
    # Build GoogleTest from source
    set(GOOGLETEST_VERSION 1.15.2)
    add_subdirectory(vendor/google/googletest/googletest EXCLUDE_FROM_ALL)

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
      target_compile_options(gtest PRIVATE -Wno-error=misleading-indentation)
    endif()

    # Create aliases if they don't exist
    if(NOT TARGET GTest::gtest)
      add_library(GTest::gtest ALIAS gtest)
    endif()
    if(NOT TARGET GTest::gtest_main)
      add_library(GTest::gtest_main ALIAS gtest_main)
    endif()
  else()
    find_package(GTest REQUIRED)
  endif()

  enable_testing()
  add_subdirectory(tests)
endif()

# Benchmarking support
if(UUID_V4_BUILD_BENCHMARKS)
  if(UUID_V4_USE_INTERNAL_GBENCH)
    # Build Google Benchmark from source
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark's own tests" FORCE)
    add_subdirectory(vendor/google/benchmark EXCLUDE_FROM_ALL)

    # Create alias if it doesn't exist
    if(NOT TARGET benchmark::benchmark)
      add_library(benchmark::benchmark ALIAS benchmark)
    endif()
  else()
    find_package(benchmark REQUIRED)
  endif()

  add_subdirectory(benchmarks)
endif()

# Example executable
add_executable(example example.cpp)
target_link_libraries(example PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})

################################################################################
## Installation and Package Support
################################################################################

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Install the header-only library
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(FILES uuid_v4.h endianness.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

# Generate and install package configuration files
set(config_install_dir ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
  ARCH_INDEPENDENT  # Header-only library is architecture-independent
)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  @ONLY
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION ${config_install_dir}
)

install(EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${config_install_dir}
)
