#ifndef EBO_H
#define EBO_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class EBO {
public:
	GLuint buffer;

	EBO() {
		glGenBuffers(1, &buffer);
	}

	void bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer); }

	void unBind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

	~EBO() {
		glDeleteBuffers(1, &buffer);
	}
};

#endif