#ifndef VBO_H
#define VBO_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class VBO {
public:
	GLuint buffer;

	VBO() {
		glGenBuffers(1, &buffer);
	}

	void bind() { glBindBuffer(GL_ARRAY_BUFFER, buffer); }

	void unBind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

	~VBO() {
		glDeleteBuffers(1, &buffer);
	}
};

#endif