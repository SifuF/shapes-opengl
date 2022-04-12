#ifndef TEXTURE_H
#define TEXTURE_H


class Texture {
public:
	int width, height, numColCh;
	unsigned char* bytes;
	GLuint texture;
	int unit;

	Texture(const char* filename, int unit) {
		this->unit = unit;
		
		stbi_set_flip_vertically_on_load(true);
		bytes = stbi_load(filename, &width, &height, &numColCh, 0);

		glGenTextures(1, &texture);
		glActiveTexture(this->unit);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (bytes)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(bytes);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void bind() {
		glActiveTexture(unit);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void unBind() {
		glActiveTexture(unit);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	~Texture() {
		glDeleteTextures(1, &texture);
	}
};

#endif
