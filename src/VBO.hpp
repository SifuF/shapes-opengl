#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VBO {
public:
    VBO()
    {
        glGenBuffers(1, &buffer);
    }

    void bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
    }

    void unBind() const
    { 
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
    }

    ~VBO()
    {
        glDeleteBuffers(1, &buffer);
    }

    VBO(const VBO& other) = delete;
    VBO& operator=(const VBO& other) = delete;
    VBO(VBO&& other) = delete;
    VBO& operator=(VBO&& other) = delete;

private:
    GLuint buffer;
};

#endif // VBO_H
