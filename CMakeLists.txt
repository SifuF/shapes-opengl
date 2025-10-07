cmake_minimum_required(VERSION 3.22.1)

project(Shapes VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory("${CMAKE_SOURCE_DIR}/submodules/glfw")

set(SOURCES
    src/main.cpp
    thirdparty/glad/glad.c
)

set(HEADERS
    src/EBO.hpp
    src/MatrixStack.hpp
    src/Shader.hpp
    src/ShapeMesh.hpp
    src/Texture.hpp
    src/VAO.hpp
    src/VBO.hpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/submodules/glm"
    "${CMAKE_SOURCE_DIR}/submodules/stb"
    "${CMAKE_SOURCE_DIR}/thirdparty"
)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
    glfw
    opengl32
)

file(COPY ${PROJECT_SOURCE_DIR}/src/shaders/ DESTINATION ${PROJECT_BINARY_DIR}/shaders/)
file(COPY ${PROJECT_SOURCE_DIR}/src/textures/ DESTINATION ${PROJECT_BINARY_DIR}/textures/)
