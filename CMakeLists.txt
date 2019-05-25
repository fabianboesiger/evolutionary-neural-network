cmake_minimum_required(VERSION 3.7.2)
project(Test VERSION 1.0.0)

add_library(
    environment
    environment.hpp
    environment.cpp
)

add_executable(cmake main.cpp)

target_link_libraries(cmake PRIVATE network)