add_executable(uuid_v4_benchmark uuid_v4_benchmark.cpp)
target_link_libraries(uuid_v4_benchmark
  PRIVATE
    uuid_v4::uuid_v4
    benchmark::benchmark
)
add_test(NAME uuid_v4_benchmark COMMAND uuid_v4_benchmark)

add_executable(compare_benchmark compare_benchmark.cpp)
target_link_libraries(compare_benchmark
  PRIVATE
    uuid_v4::uuid_v4
    benchmark::benchmark
    uuid
)
add_test(NAME compare_benchmark COMMAND compare_benchmark)
