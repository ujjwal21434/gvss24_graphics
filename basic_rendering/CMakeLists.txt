find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

add_library(a1 ../src/hw.cpp)
target_link_libraries(a1 GLEW::GLEW glm::glm OpenGL::GL SDL2)
add_executable(basic_example basic_example.cpp)
target_link_libraries(basic_example a1)

add_executable(algebra algebra.cpp)
target_link_libraries(algebra a1)


add_executable(transformations transformations.cpp)
target_link_libraries(transformations a1)

add_executable(depth_testing1 depth_testing1.cpp)
target_link_libraries(depth_testing1 a1)

add_executable(depth_testing2 depth_testing2.cpp)
target_link_libraries(depth_testing2 a1)

add_executable(shaders shaders.cpp)
target_link_libraries(shaders a1)
