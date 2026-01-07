add_executable(uuid_v4_test uuid_v4_test.cpp)

target_link_libraries(uuid_v4_test
  PRIVATE
    uuid_v4::uuid_v4
    GTest::gtest_main
)

add_test(NAME uuid_v4_test COMMAND uuid_v4_test)
