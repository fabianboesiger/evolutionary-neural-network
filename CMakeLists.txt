cmake_minimum_required(VERSION 3.7.2)
project(evolutionaryneuralnetwork VERSION 1.0.0)

add_library(
    data
    data.hpp
    data.cpp
)

add_library(
    world
    world.hpp
    world.cpp
)

add_library(
    network
    network.hpp
    network.cpp
)

add_executable(cmake main.cpp)

target_link_libraries(cmake PRIVATE data)
target_link_libraries(cmake PRIVATE world)
target_link_libraries(cmake PRIVATE network)