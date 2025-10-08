cmake_minimum_required(VERSION 3.22.1)

project(Shapes VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(SHAPES_BUILD_DEMOS "Build demo programs" ON)

add_subdirectory("${CMAKE_SOURCE_DIR}/submodules/glfw")

set(SOURCES
    src/Shader.cpp
    src/ShapeMesh.cpp
    src/Texture.cpp
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

set(INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/submodules/glm"
    "${CMAKE_SOURCE_DIR}/submodules/stb"
    "${CMAKE_SOURCE_DIR}/thirdparty"
)

set(LINK_LIBS
    glfw
    opengl32
)

# build sample
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} src/demos/sample.cpp ${HEADERS})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${LINK_LIBS})

if(SHAPES_BUILD_DEMOS) 
    add_executable(polar_demo ${SOURCES} src/demos/polar.cpp ${HEADERS})

    target_include_directories(polar_demo PUBLIC ${INCLUDE_DIRS})

    target_link_libraries(polar_demo PUBLIC ${LINK_LIBS})
endif()
