# CMakeLists.txt
cmake_minimum_required(VERSION 3.12)
project(GraphicsEngine)

find_package(glfw3 REQUIRED)

add_executable(GraphicsEngine main.cpp)

target_link_libraries(GraphicsEngine PRIVATE glfw)
