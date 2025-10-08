#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdexcept>

Texture::Texture(const char* filename, int texunit) : unit(texunit)
{
    stbi_set_flip_vertically_on_load(true);
    auto* const bytes = stbi_load(filename, &width, &height, &numColCh, 0);
    if (!bytes) {
        throw std::runtime_error("STB failed to allocate image");
    }

    glGenTextures(1, &texture);
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(bytes);
}

void Texture::bind() const
{
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::unBind() const
{
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &texture);
}
