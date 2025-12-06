cmake_minimum_required(VERSION 3.10)
project(BalagurFate3)

set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cpp src/NPC.cpp)
target_include_directories(main PRIVATE include)

add_executable(tests test/test.cpp src/NPC.cpp)
target_include_directories(tests PRIVATE include)