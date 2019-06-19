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

add_executable(main main.cpp)

target_link_libraries(main PRIVATE data)
target_link_libraries(main PRIVATE world)
target_link_libraries(main PRIVATE network)