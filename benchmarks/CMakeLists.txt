include_directories (${uuid_v4_SOURCE_DIR})

add_executable (uuid_v4_benchmark uuid_v4_benchmark.cpp)
target_link_libraries (uuid_v4_benchmark benchmark::benchmark)
add_test(NAME uuid_v4_benchmark COMMAND uuid_v4_benchmark)

add_executable (compare_benchmark compare_benchmark.cpp)
target_link_libraries (compare_benchmark benchmark::benchmark uuid)
add_test(NAME compare_benchmark COMMAND compare_benchmark)
