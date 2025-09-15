#ifndef VAO_H
#define VAO_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class VAO {
public:
	VAO()
	{
		glGenVertexArrays(1, &buffer);
	}

	void bind() const
	{
		glBindVertexArray(buffer);
	}

	void unBind() const
	{
		glBindVertexArray(0);
	}

	~VAO()
	{
		glDeleteVertexArrays(1, &buffer);
	}

	VAO(const VAO& other) = delete;
	VAO& operator=(const VAO& other) = delete;
	VAO(VAO&& other) = delete;
	VAO& operator=(VAO&& other) = delete;

private:
	GLuint buffer;
};

#endif // VAO_H
