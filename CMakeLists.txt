cmake_minimum_required (VERSION 3.8)

if (NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif ()

project (uuid_v4 VERSION 1.0.0 LANGUAGES CXX)

option(test "Build all tests." OFF)
option(test_use_internal_googletest "Build googletest, do not use it from system." ON)
option(benchmark "Build benchmarks." OFF)
option(benchmark_use_internal_googlebenchmark "Build google benchmark, do not use it from system." ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif ()

if (test)
  if (test_use_internal_googletest)
    # build tests (targets: gtest_main, gtest)
    add_subdirectory(vendor/google/googletest/googletest)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
      target_compile_options (gtest PRIVATE "-Wno-error=misleading-indentation")
    endif()
    
    add_library(GTest::gtest INTERFACE IMPORTED)
    target_link_libraries(GTest::gtest INTERFACE gtest)
    add_library(GTest::gtest_main INTERFACE IMPORTED)
    target_link_libraries(GTest::gtest_main INTERFACE gtest_main)
  else ()
    find_package(GTest REQUIRED)
  endif ()

  enable_testing ()
  add_subdirectory (tests)
endif ()

if (benchmark)
  if (benchmark_use_internal_googlebenchmark)
    # build google benchmark (target: benchmark)
    # do not build tests of benchmarking lib
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)
    add_subdirectory(vendor/google/benchmark)
    add_library(benchmark::benchmark INTERFACE IMPORTED)
    target_link_libraries(benchmark::benchmark INTERFACE benchmark)
  else ()
    find_package(benchmark REQUIRED)
  endif ()

  add_subdirectory (benchmarks)
endif ()

add_executable(example example.cpp)
target_link_libraries (example)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if (NOT CMAKE_VERSION VERSION_LESS 3.8)
    target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
endif()

target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>)

################################################################################
## PACKAGE SUPPORT
################################################################################

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(include_install_dir "include")
set(config_install_dir "lib/cmake/${PROJECT_NAME}")


set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

include(CMakePackageConfigHelpers)

# CMake automatically adds an architecture compatibility check to make sure
# 32 and 64 bit code is not accidentally mixed. For a header-only library this
# is not required. The check can be disabled by temporarily unsetting
# CMAKE_SIZEOF_VOID_P. In CMake 3.14 and later this can be achieved more cleanly
# with write_basic_package_version_file(ARCH_INDEPENDENT).
# TODO: Use this once a newer CMake can be required.
set(TMP_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})
unset(CMAKE_SIZEOF_VOID_P)
write_basic_package_version_file(
    "${version_config}" VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion
)
set(CMAKE_SIZEOF_VOID_P ${TMP_SIZEOF_VOID_P})

configure_file("Config.cmake.in" "${project_config}" @ONLY)

install(TARGETS ${PROJECT_NAME}
        EXPORT "${targets_export_name}"
        INCLUDES DESTINATION "${include_install_dir}")

install(FILES "uuid_v4.h"
        DESTINATION "${include_install_dir}/uuid_v4")
install(FILES "endianness.h"
        DESTINATION "${include_install_dir}/uuid_v4")

install(FILES "${project_config}" "${version_config}"
        DESTINATION "${config_install_dir}")

install(EXPORT "${targets_export_name}"
        NAMESPACE "${namespace}"
        DESTINATION "${config_install_dir}")
