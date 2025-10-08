#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "MatrixStack.hpp"
#include "Texture.hpp"
#include "ShapeMesh.hpp"
#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

float camX = 0.0f;
float camY = 0.0f;
float camZ = 5.0f;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) { camY += 1.0; }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) { camY -= 1.0; }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) { camX -= 1.0; }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) { camX += 1.0; }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS) { camZ -= 1.0; }
    if (key == GLFW_KEY_X && action == GLFW_PRESS) { camZ += 1.0; }
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

    Shader shader("../src/shaders/default.vert", "../src/shaders/default.frag");
    Shader flatShader("../src/shaders/flat.vert", "../src/shaders/flat.frag");

    auto rectangle = std::make_shared<RectangleMesh>(2.0f, 2.0f);
    auto cube = std::make_shared<CuboidMesh>(1.0f, 1.0f, 1.0f);
    auto circle = std::make_shared<CircleMesh>(30);
    auto cone = std::make_shared<ConeMesh>(20);
    auto cylinder = std::make_shared<CylinderMesh>(40);
    auto sphere = std::make_shared<SphereMesh>(20);
    auto torus = std::make_shared<TorusMesh>(15);
    auto starTorus = std::make_shared<StarTorusMesh>(20);
    auto axes = std::make_shared<CoordinateAxesMesh>();
    auto poly = std::make_shared<PolynomialMesh>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -10.0f, 10.0f, 100, false);
    auto quadratic = std::make_shared<PolynomialMesh>(0.0f, 1.0f, 0.0f, 0.0f, 7.0f, 0.0f, 0.0f, -2.0f, 5.0f, 100, true);

    MatrixStack matrix;

    Texture texture1("../src/textures/photo1.jpg", GL_TEXTURE0);
    Texture texture2("../src/textures/photo2.jpg", GL_TEXTURE1);

    shader.use();
    shader.setTexture(0);
    shader.setTexture(1);

    glEnable(GL_DEPTH_TEST);

    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glPointSize(5);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.setScale(0.5f); // debug
        shader.setMixer(0.0f); // debug
        
        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1 / 60) {
            rotation += 0.5f;
            prevTime = crntTime;
        }

        // Identity - render area is a unit cube
        shader.use();
        shader.setModel(glm::mat4(1.0f));
        shader.setView(glm::mat4(1.0f));
        shader.setProjection(glm::mat4(1.0f));
        //circle->draw();

        //Perspective - global coordiate system
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1080 / float(1080), 0.1f, 100.0f);
        shader.setProjection(projection);
        //circle->draw();

        // Camera - rendered in camera space
        glm::vec3 position = glm::vec3(camX, camY, camZ);
        glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 view = glm::lookAt(position, glm::vec3(0.0f), up);
        shader.setView(view);
        //circle->draw();

        // Model - per model transforms
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, -2.5f, -5.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, -1.0f, 0.0f));
        shader.setModel(model);
        texture1.bind();
        texture2.bind();
        shader.setMixer(1.0f);
        sphere->draw();
        shader.setMixer(0.0f);
        texture1.unBind();
        texture2.unBind();

        model = glm::rotate(glm::mat4(1.0f), glm::radians(rotation*2), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setModel(model);
        cube->draw();

        flatShader.use();
        flatShader.setView(view);
        flatShader.setProjection(projection);
        glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, -4.0f));
        glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(rotation * 5), glm::vec3(0.0f, 0.0f, 1.0f));
        flatShader.setModel(T * R);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        torus->draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glm::mat4 T2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat4 R2 = glm::rotate(glm::mat4(1.0f), glm::radians(-rotation * 3), glm::vec3(0.0f, 1.0f, 1.0f));
        flatShader.setModel(T2 * R2);
        sphere->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}