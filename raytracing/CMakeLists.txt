add_library(a1 ../src/dummy.cpp)

add_executable(basic_image basic_image.cpp)
target_link_libraries(basic_image a1)

add_executable(basic_tools basic_tools.cpp)
target_link_libraries(basic_tools a1)
