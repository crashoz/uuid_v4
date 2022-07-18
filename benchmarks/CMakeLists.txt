
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

include_directories (${uuid_v4_SOURCE_DIR})

add_executable (uuid_v4_benchmark uuid_v4_benchmark.cpp)
target_link_libraries (uuid_v4_benchmark benchmark::benchmark)
add_test(NAME uuid_v4_benchmark COMMAND uuid_v4_benchmark)

add_executable (compare_benchmark compare_benchmark.cpp)
target_link_libraries (compare_benchmark benchmark::benchmark uuid)
add_test(NAME compare_benchmark COMMAND compare_benchmark)
