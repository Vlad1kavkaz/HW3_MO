cmake_minimum_required(VERSION 3.26)
project(HW3_MO)

set(CMAKE_CXX_STANDARD 14)

add_executable(HW3_MO main.cpp
        Task.h
        GraphNode.h
        Graph.h
)

# If you are using C++14, you can set the standard directly with target_compile_features
target_compile_features(HW3_MO PRIVATE cxx_std_14)
