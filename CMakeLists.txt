cmake_minimum_required(VERSION 3.30)
project(starmans_odyssey)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#### GET OpenGL ####
find_package(OpenGL REQUIRED)

#### GET GLFW ####
find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

#### SET SOURCES ####
set(SOURCES
        src/main.cpp
        libs/src/glad.c
        src/Engine/src/Window.cpp
)

add_executable(starmans_odyssey ${SOURCES})

target_include_directories(starmans_odyssey PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/include>
        ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(starmans_odyssey PRIVATE ${OPENGL_LIBRARIES} glfw)