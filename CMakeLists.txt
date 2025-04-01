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
        includes/Window.hpp
        src/Tools/Window.cpp
        includes/Input.hpp
        src/Tools/Input.cpp
        src/Rendering/Camera.cpp
        includes/Rendering/Camera.hpp
        includes/glm/common.hpp
        includes/glm/exponential.hpp
        includes/glm/ext.hpp
        includes/glm/fwd.hpp
        includes/glm/geometric.hpp
        includes/glm/glm.cppm
        includes/glm/integer.hpp
        includes/glm/mat2x2.hpp
        includes/glm/mat2x3.hpp
        includes/glm/mat2x4.hpp
        includes/glm/mat3x2.hpp
        includes/glm/mat3x3.hpp
        includes/glm/mat3x4.hpp
        includes/glm/mat4x2.hpp
        includes/glm/mat4x3.hpp
        includes/glm/mat4x4.hpp
        includes/glm/matrix.hpp
        includes/glm/packing.hpp
        includes/glm/trigonometric.hpp
        includes/glm/vec2.hpp
        includes/glm/vec3.hpp
        includes/glm/vec4.hpp
        includes/glm/vector_relational.hpp
        src/Rendering/Renderer.cpp
        includes/Rendering/Renderer.hpp
        includes/Rendering/Shader.hpp
        includes/Rendering/Mesh.hpp
        src/Rendering/Shader.cpp
        src/Rendering/Mesh.cpp
)

add_executable(starmans_odyssey ${SOURCES})
target_link_libraries(starmans_odyssey PRIVATE
        ${SDL2_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
)

#### SET INCLUDE DIRECTORIES ####
target_include_directories(starmans_odyssey PRIVATE includes)
target_include_directories(starmans_odyssey PRIVATE assets)