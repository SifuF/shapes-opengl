cmake_minimum_required(VERSION 3.22.1)

project(Shapes VERSION 1.0)

#set(MAKE_BUILD_TYPE Debug ... FORCE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(shapes_config.hpp.in shapes_config.hpp)

add_executable(Shapes 
    main.cpp
    glad.c
    
    EBO.hpp
    Shader.hpp
    Shape.hpp
    Texture.hpp
    VAO.hpp
    VBO.hpp
)

if(WIN32)
    #set_target_properties(Shapes PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
endif() 

target_include_directories(Shapes PUBLIC
    "${PROJECT_BINARY_DIR}"
)

if(WIN32)
    target_include_directories(Shapes PUBLIC
        "${PROJECT_SOURCE_DIR}/Libraries/include"
    )

    target_link_directories(Shapes PUBLIC
        "${PROJECT_SOURCE_DIR}/Libraries/lib"
    )
endif() 

target_link_libraries(Shapes PUBLIC
    glfw3    
    opengl32
)

file(COPY ${PROJECT_SOURCE_DIR}/shaders/ DESTINATION ${PROJECT_BINARY_DIR}/shaders/)
