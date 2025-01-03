cmake_minimum_required(VERSION 3.28)
project(ds VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)

add_executable(ds ${SRC_DIR}/main.cpp)

target_include_directories(ds PRIVATE
    ${SRC_DIR}/vector
    ${SRC_DIR}/string
    ${SRC_DIR}/stack
    ${SRC_DIR}/queue
    ${SRC_DIR}/linked_list
    ${SRC_DIR}/tree
)

target_compile_options(ds PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Werror>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)

include(CTest)
enable_testing()

add_executable(ds_tests
    ${TEST_DIR}/vec_test.cpp
    ${TEST_DIR}/string_test.cpp
    ${TEST_DIR}/stack_test.cpp
    ${TEST_DIR}/monostack_test.cpp
    ${TEST_DIR}/queue_test.cpp
    ${TEST_DIR}/deque_test.cpp
    ${TEST_DIR}/cqueue_test.cpp
    ${TEST_DIR}/sll_test.cpp
    ${TEST_DIR}/dll_test.cpp
    ${TEST_DIR}/cll_test.cpp
)

target_link_libraries(ds_tests PRIVATE
    gtest
    gtest_main
)

target_include_directories(ds_tests PRIVATE
    ${SRC_DIR}/vector
    ${SRC_DIR}/string
    ${SRC_DIR}/stack
    ${SRC_DIR}/queue
    ${SRC_DIR}/linked_list
    ${SRC_DIR}/tree
)

include(GoogleTest)
gtest_discover_tests(ds_tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
