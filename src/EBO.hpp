#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class EBO {
public:
    EBO()
    {
        glGenBuffers(1, &buffer);
    }

    void bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    }

    void unBind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ~EBO()
    {
        glDeleteBuffers(1, &buffer);
    }

    EBO(const EBO& other) = delete;
    EBO& operator=(const EBO& other) = delete;
    EBO(EBO&& other) = delete;
    EBO& operator=(EBO&& other) = delete;

private:
    GLuint buffer;
};

#endif // EBO_H
