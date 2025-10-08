#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture
{
public:
    Texture(const char* filename, int texunit);

    void bind() const;

    void unBind() const;

    ~Texture();

    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;
    Texture(Texture&& other) = delete;
    Texture& operator=(Texture&& other) = delete;

private:
    int width, height, numColCh;
    GLuint texture;
    int unit;
};

#endif // TEXTURE_H
