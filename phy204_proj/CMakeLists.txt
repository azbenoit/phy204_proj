cmake_minimum_required(VERSION 3.20 VERSION 0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(PHY204_proj)
add_executable(main main.cpp)
target_include_directories(main PRIVATE
    libs/boost_1_82_0
)