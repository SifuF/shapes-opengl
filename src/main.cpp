#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"

#include"Texture.hpp"
#include"Shape.hpp"
#include"Shader.hpp"

#include<iostream>
#include<fstream>
#include<ctime>
#include<cmath>

bool torusDown = false;
bool torusUp = false;

float camX = 0.0f;
float camY = 0.0f;
float camZ = 5.0f;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		camY += 1.0;

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		camY -= 1.0;

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		camX -= 1.0;

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		camX += 1.0;

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		camZ -= 1.0;

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
		camZ += 1.0;

	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		torusUp = true;
	}
	if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
		torusUp = false;
	}

	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		torusDown = true;
	}
	if (key == GLFW_KEY_O && action == GLFW_RELEASE) {
		torusDown = false;
	}
}

int main()
{
	srand(time(NULL));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	constexpr int windowWidth = 1080;
	constexpr int windowHeight = 1080;

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Shapes by SifuF", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSwapInterval(1);

	gladLoadGL();

	Shader shader("shaders/default.vert", "shaders/default.frag");
	Shader flatShader("shaders/flat.vert", "shaders/flat.frag");

	Rectangle rectangle(2.0f, 2.0f);
	Cuboid cube(1.0f, 1.0f, 1.0f);
	Circle circle(30);
	Cone cone(20);
	Cylinder cylinder(40);
	Sphere sphere(20);
	Torus torus(40);
	StarTorus starTorus(20);
	CoordinateAxes axes;
	//PolynomialVolume quadratic(1.0f, 6.0f, -5.0f, -8.0f, 2.0f, 20);
	Polynomial poly(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -10.0f, 10.0f, 100, false);
	//Polynomial quadratic(0.0f, 1.0f, 0.0f, 0.0f, 7.0f, 0.0f, 0.0f, -2.0f, 5.0f, 100, true);

	Texture texture1("textures/photo1.jpg", GL_TEXTURE0);
	Texture texture2("textures/photo2.jpg", GL_TEXTURE1);

	glUseProgram(shader.getProgram());
	GLuint tex0Uni = glGetUniformLocation(shader.getProgram(), "tex0");
	glUniform1i(tex0Uni, 0);

	GLuint tex1Uni = glGetUniformLocation(shader.getProgram(), "tex1");
	glUniform1i(tex1Uni, 1);

	GLuint uniID = glGetUniformLocation(shader.getProgram(), "scale");
	GLuint uniMixer = glGetUniformLocation(shader.getProgram(), "mixer");

	glEnable(GL_DEPTH_TEST);

	float mix = 0.0f;
	bool forward = true;
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		
		//if (torusUp) { torus.points+=30; }

		//if (torusDown) { torus.points-=30; }

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader.getProgram());
		glUniform1f(uniID, 0.5f);
		glUniform1f(uniMixer, mix);
		glPointSize(5);

		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60) {
			rotation += 0.5f;
			prevTime = crntTime;
		}

		//////////////////Identity////////////////////////////////////////////////////
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		GLuint modelID = glGetUniformLocation(shader.getProgram(), "model");
		GLuint viewID = glGetUniformLocation(shader.getProgram(), "view");
		GLuint projID = glGetUniformLocation(shader.getProgram(), "proj");
		glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projID, 1, GL_FALSE, glm::value_ptr(proj));

			////render area is a unit cube////

		//circle.draw();
		//rectangle.draw();
		//cube.draw();
		//cone.draw();
		//cylinder.draw();
		//sphere.draw();
		//starTorus.draw();
		//torus.draw();
		//axes.draw();
		//poly.draw();

		///////////////////Camera/////////////////////////////////////////////////////
		glm::vec3 position = glm::vec3(camX, camY, camZ);
		glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		//view = glm::lookAt(position, position + orientation, up);
		view = glm::lookAt(position, glm::vec3(0.0f), up);
		glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(view));

			////rendered in camera space////

		//circle.draw();
		//rectangle.draw();

		////////////////////Perspective///////////////////////////////////////////////
		proj = glm::perspective(glm::radians(45.0f), 1080 / float(1080), 0.1f, 100.0f);
		glUniformMatrix4fv(projID, 1, GL_FALSE, glm::value_ptr(proj));

			////global (static) coordiate system////

		//circle.draw();
		//axes.draw();
		//poly.draw();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//sphere.draw();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		////////////////////Model/////////////////////////////////////////////////////

			////per model transforms////

		glm::mat4 model2 = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.0f));
		model2 = glm::rotate(model2, glm::radians(rotation), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model2));
		texture1.bind();
		texture2.bind();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//sphere.draw();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		texture1.unBind();
		texture2.unBind();;

		model2 = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 0.0f, 0.0f));
		model2 = glm::translate(model2, glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model2));
		//starTorus.draw();

		model2 = glm::rotate(model, glm::radians(rotation*2), glm::vec3(0.0f, 1.0f, 0.0f));
		model2 = glm::translate(model2, glm::vec3(3.0f, 2.0f, 0.0f));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model2));
		//cube.draw();

		glm::mat4 model5 = glm::translate(model, glm::vec3(-2.0f, 1.5f, 1.0f));
		model5 = glm::rotate(model5, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model5));
		//rectangle.draw();

		//////////////////// Flat shader
		glUseProgram(flatShader.getProgram());
		GLuint modelID2 = glGetUniformLocation(flatShader.getProgram(), "model");
		GLuint viewID2 = glGetUniformLocation(flatShader.getProgram(), "view");
		GLuint projID2 = glGetUniformLocation(flatShader.getProgram(), "proj");
		glUniformMatrix4fv(modelID2, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewID2, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projID2, 1, GL_FALSE, glm::value_ptr(proj));

		glm::mat4 model3 = glm::mat4(1.0f);
		model3 = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model3 = glm::rotate(model3, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelID2, 1, GL_FALSE, glm::value_ptr(model3));
		//sphere.draw();
		//cone.draw();
		//circle.draw();
		//cylinder.draw();

		glm::mat4 model4 = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		model4 = glm::rotate(model4, glm::radians(rotation*5), glm::vec3(0.0f, 0.0f, 1.0f));
		model4 = glm::rotate(model4, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelID2, 1, GL_FALSE, glm::value_ptr(model4));
		//circle.draw();
		torus.draw();
		//starTorus.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (mix > 1.0) forward = false;
		if (mix < 0.0) forward = true;

		if (forward) {
			mix += 0.01f;
		}
		else {
			mix -= 0.01f;
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}