cmake_minimum_required(VERSION 3.29)
project(g3dSubmarin)

set(CMAKE_CXX_STANDARD 26)

add_executable(g3dSubmarin g3dSubmarin.cpp
        includes.h
        model.cpp
        model.h
        mesh.cpp
        mesh.h
        shader.cpp
        shader.h)


find_package(GLUT REQUIRED)

find_package(GLEW REQUIRED)

find_package(OpenGL REQUIRED)

find_package(glfw3 3.3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} )
target_link_libraries(g3dSubmarin ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLEW::GLEW glfw  ${ASSIMP_LIBRARIES} -lassimp)

