#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <stdexcept>
#include <string>

class Shader
{
public:
    Shader(const char* vertexFile, const char* fragmentFile)
    {
        const std::string vertStr = get_file_contents(vertexFile);
        const std::string fragStr = get_file_contents(fragmentFile);

        const char* vertexSource = vertStr.c_str();
        const char* fragmentSource = fragStr.c_str();

        const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

        const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ~Shader()
    {
        glDeleteProgram(program);
    }

    GLuint getProgram() const { return program; }

    std::string get_file_contents(const char* filename) const
    {
        std::ifstream in(filename, std::ios::binary);
        if (in)
        {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return(contents);
        }
        throw std::runtime_error("Failed to load shader");
    }

    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;
    Shader(Shader&& other) = delete;
    Shader& operator=(Shader&& other) = delete;

private:
    GLuint program;
};

#endif //SHADER_H
