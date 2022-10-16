#ifndef VAO_H
#define VAO_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class VAO {
public:
	GLuint buffer;

	VAO() {
		glGenVertexArrays(1, &buffer);
	}

	void bind() { glBindVertexArray(buffer); }

	void unBind() { glBindVertexArray(0); }

	~VAO() {
		glDeleteVertexArrays(1, &buffer);
	}
};

#endif