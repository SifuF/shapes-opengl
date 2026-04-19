#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>

class Shader
{
public:
    Shader(const char* vertexFile, const char* fragmentFile);
    ~Shader();

    void use() const;

    void setModel(const glm::mat4& model) const;

    void setView(const glm::mat4& view) const;

    void setProjection(const glm::mat4& projection) const;

    void setTexture(GLuint textureUnit);

    void setScale(GLfloat val) const;

    void setAmbient(GLfloat val) const;

    void setVec3(const char* name, glm::vec3 value) const;

    std::string get_file_contents(const char* filename) const;

    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;
    Shader(Shader&& other) = delete;
    Shader& operator=(Shader&& other) = delete;

private:
    GLuint program;
};

#endif //SHADER_H
