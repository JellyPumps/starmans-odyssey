cmake_minimum_required(VERSION 3.30)
project(starmans_odyssey)

set(CMAKE_CXX_STANDARD 23)

#### GET SDL2 ####
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

#### SET SOURCES ####
set(SOURCES
    src/main.cpp
        includes/Window.hpp
        src/Tools/Window.cpp
        includes/Input.hpp
        src/Tools/Input.cpp
        includes/Math/Vectors.hpp
        src/Rendering/Camera.cpp
        includes/Rendering/Camera.hpp
        includes/Math/Matrix4x4.hpp
        src/Tools/Math/Matrix4x4.cpp
)

add_executable(starmans_odyssey ${SOURCES})
target_link_libraries(starmans_odyssey PRIVATE ${SDL2_LIBRARIES})

#### SET INCLUDE DIRECTORIES ####
target_include_directories(starmans_odyssey PRIVATE includes)