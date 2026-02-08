cmake_minimum_required(VERSION 4.1)
project(SET5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SET5
        random_stream_gen.cpp
        hash_func_gen.cpp
        hyperloglog.cpp
        hyperloglog_packed.cpp
        experiment.cpp
)
target_include_directories(SET5 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
