project(ColorAttributes)
cmake_minimum_required(VERSION 3.0)
aux_source_directory(. SRC_LIST)

# Enable C++ 11 support.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


# Find OpenGL
find_package(OpenGL REQUIRED)

# Find glfw
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)


# OpenGL & glfw headers
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIRS})

# glfw library path
link_directories(${GLFW_LIBRARY_DIRS})

# Declare the executable target built from your sources
add_executable(${PROJECT_NAME} ${SRC_LIST} ../myGL.hpp ../Context.hpp Geometry.hpp)

# Link application with libraries
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})


# Copy shaders
configure_file(plane.vert plane.vert COPYONLY)
configure_file(smooth_color.frag smooth_color.frag COPYONLY)