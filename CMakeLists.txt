cmake_minimum_required(VERSION 3.0)

project(a1)
set(CMAKE_CXX_STANDARD 11)

find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

add_library(a1 src/hw.cpp src/helpers.cpp)
target_link_libraries(a1 GLEW::GLEW glm::glm OpenGL::GL SDL2)

add_executable(basic_example examples/basic_example.cpp)
target_link_libraries(basic_example a1)

add_executable(algebra examples/algebra.cpp)
target_link_libraries(algebra a1)


add_executable(transformations examples/transformations.cpp)
target_link_libraries(transformations a1)

add_executable(depth_testing1 examples/depth_testing1.cpp)
target_link_libraries(depth_testing1 a1)

add_executable(depth_testing2 examples/depth_testing2.cpp)
target_link_libraries(depth_testing2 a1)

add_executable(shaders examples/shaders.cpp)
target_link_libraries(shaders a1)

add_executable(basic_image raytracing/basic_image.cpp)
target_link_libraries(basic_image a1)

add_executable(raytracing raytracing/raytracing.cpp)
target_link_libraries(raytracing a1)

add_executable(sphere raytracing/sphere.cpp)
target_link_libraries(sphere a1)

add_executable(sphere_anti_aliasing raytracing/sphere_anti_aliasing.cpp)
target_link_libraries(sphere_anti_aliasing a1)
