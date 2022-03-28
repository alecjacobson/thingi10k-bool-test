cmake_minimum_required(VERSION 3.16)
project(main)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Libigl
option(LIBIGL_COPYLEFT_CGAL "Build target igl_copyleft::cgal" ON)
option(LIBIGL_XML "Build target igl::xml" ON)
include(libigl)

# Add your project files
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC igl_copyleft::cgal igl::xml)

add_executable(compare compare.cpp)
target_link_libraries(compare PUBLIC igl_copyleft::cgal igl::xml)
