cmake_minimum_required(VERSION 3.20 VERSION 0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(PHY204_proj)
add_executable(phy204_proj main.cpp)
find_package(OpenMP)
target_link_libraries(phy204_proj PUBLIC OpenMP::OpenMP_CXX)

# target_include_directories(main PRIVATE
#     libs/boost_1_82_0
# )