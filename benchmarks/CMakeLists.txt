include_directories (${uuid_v4_SOURCE_DIR})

add_executable (uuid_v4_benchmark uuid_v4_benchmark.cpp)
target_link_libraries (uuid_v4_benchmark benchmark)
add_test(NAME uuid_v4_benchmark COMMAND uuid_v4_benchmark)
