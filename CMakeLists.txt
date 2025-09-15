cmake_minimum_required(VERSION 3.22.1)

project(Shapes VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/main.cpp
    thirdparty/src/glad.c
)

set(HEADERS
    src/EBO.hpp
    src/Shader.hpp
    src/Shape.hpp
    src/Texture.hpp
    src/VAO.hpp
    src/VBO.hpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/thirdparty/include"
)

target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/thirdparty/lib"
)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
    glfw3    
    opengl32
)

file(COPY ${PROJECT_SOURCE_DIR}/src/shaders/ DESTINATION ${PROJECT_BINARY_DIR}/shaders/)
file(COPY ${PROJECT_SOURCE_DIR}/src/textures/ DESTINATION ${PROJECT_BINARY_DIR}/textures/)
