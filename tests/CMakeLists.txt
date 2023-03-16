set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif ()

include_directories (${uuid_v4_SOURCE_DIR})

add_executable (uuid_v4_test uuid_v4_test.cpp)
target_link_libraries (uuid_v4_test GTest::gtest_main)
add_test(NAME uuid_v4_test COMMAND uuid_v4_test)
