cmake_minimum_required(VERSION 3.10)
project(LabsOOP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
  TLS_VERIFY false
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(include)
add_library(lab_lib src/gcd.cpp)

add_executable(main main.cpp)
target_link_libraries(main lab_lib)

enable_testing()
add_executable(start_tests tests/tests.cpp)
target_link_libraries(start_tests lab_lib gtest_main)
add_test(NAME start_tests COMMAND start_tests)