cmake_minimum_required(VERSION 3.16)
project(oop_lr5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(oop_lr5 STATIC
    src/figures.cpp
)

add_executable(main
    main.cpp
)
target_link_libraries(main oop_lr5)


enable_testing()
add_executable(test_oop_lr5
    tests/test.cpp
)
target_link_libraries(test_oop_lr5 oop_lr5 GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_oop_lr5)
