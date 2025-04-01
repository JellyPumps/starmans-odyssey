cmake_minimum_required(VERSION 3.30)
project(starmans_odyssey)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#### GET SDL2 ####
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
)

#### SET SOURCES ####
set(SOURCES
        src/main.cpp
)

add_executable(starmans_odyssey ${SOURCES})

target_include_directories(starmans_odyssey PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include
        ${SDL2_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
)

target_link_libraries(starmans_odyssey PRIVATE
        ${SDL2_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
)