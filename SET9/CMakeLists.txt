cmake_minimum_required(VERSION 4.1)
project(SET9)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SET9
        main.cpp
        StringGenerator.cpp
        StringSortTester.cpp
)
add_executable(A1m codeforces/A1m.cpp)
add_executable(A1q codeforces/A1q.cpp)
add_executable(A1r codeforces/A1r.cpp)
add_executable(A1rq codeforces/A1rq.cpp)
target_include_directories(SET9 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})