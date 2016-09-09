cmake_minimum_required(VERSION 3.6)
project(cvTexture)

aux_source_directory(. SRC_LIST)

# Enable C++ 11 support.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


# Find OpenGL
find_package(OpenGL REQUIRED)

# Find glfw
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# Find OpenCV
find_package(OpenCV REQUIRED)


# OpenGL & glfw headers
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIRS})

# Add OpenCV headers location to your include paths
include_directories(${OpenCV_INCLUDE_DIRS})


# glfw library path
link_directories(${GLFW_LIBRARY_DIRS})

# Declare the executable target built from your sources
add_executable(${PROJECT_NAME} ${SRC_LIST} ../myGL.hpp ../Context.hpp)

# Link application with libraries
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})

# Link your application with OpenCV libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})


# Copy shaders
configure_file(color.vert color.vert COPYONLY)
configure_file(color.frag color.frag COPYONLY)
configure_file(texture.vert texture.vert COPYONLY)
configure_file(texture.frag texture.frag COPYONLY)
configure_file(../../opencv/Lenna.png Lenna.png COPYONLY)
