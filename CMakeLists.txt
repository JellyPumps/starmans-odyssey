cmake_minimum_required(VERSION 3.30)
project(starmans_odyssey)

set(CMAKE_CXX_STANDARD 23)

#### GET SDL2 ####
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

#### SET SOURCES ####
set(SOURCES
    src/main.cpp
        src/Tools/Window.hpp
        src/Tools/Window.cpp
        src/Tools/Input.hpp
        src/Tools/Input.cpp
)

add_executable(starmans_odyssey ${SOURCES})
target_link_libraries(starmans_odyssey PRIVATE ${SDL2_LIBRARIES})