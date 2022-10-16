#ifndef SHADER_H
#define SHADER_H

class Shader {
public:
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;


	std::string get_file_contents(const char* filename)
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
		throw(errno);
	}


	Shader(const char* vertexFile, const char* fragmentFile) {
		std::string vertStr = get_file_contents(vertexFile);
		std::string fragStr = get_file_contents(fragmentFile);

		const char* vertexSource = vertStr.c_str();
		const char* fragmentSource = fragStr.c_str();

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);

		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	~Shader() {
		glDeleteProgram(program);
	}

};
#endif