cmake_minimum_required(VERSION 3.30)
project(starmans_odyssey)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#### GET OpenGL ####
find_package(OpenGL REQUIRED)

#### GET GLFW ####
find_package(glfw3 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

#### GET ASSIMP ####
find_package(assimp REQUIRED)

#### SET SOURCES ####
set(SOURCES
        src/main.cpp
        libs/src/glad.c
        src/Engine/Window.cpp
        src/Engine/Shader.cpp
        src/Engine/Mesh.cpp
        src/Engine/Texture.cpp
        src/Engine/Camera.cpp
        src/Engine/Input.cpp
        src/Starman/Player.cpp
)

add_executable(starmans_odyssey ${SOURCES})

target_include_directories(starmans_odyssey PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/libs/include
        ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(starmans_odyssey PRIVATE ${OPENGL_LIBRARIES} glfw assimp::assimp)