#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Texture.hpp"
#include "ShapeMesh.hpp"
#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

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
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        camY += 1.0;
    }
    
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        camY -= 1.0;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        camX -= 1.0;
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        camX += 1.0;
    }

    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        camZ -= 1.0;
    }

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        camZ += 1.0;
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        torusUp = true;
    }

    if (key == GLFW_KEY_P && action == GLFW_RELEASE) 
    {
        torusUp = false;
    }

    if (key == GLFW_KEY_O && action == GLFW_PRESS) 
    {
        torusDown = true;
    }

    if (key == GLFW_KEY_O && action == GLFW_RELEASE) 
    {
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

    auto rectangle = std::make_shared<RectangleMesh>(2.0f, 2.0f);
    auto cube = std::make_shared<CuboidMesh>(1.0f, 1.0f, 1.0f);
    auto circle = std::make_shared<CircleMesh>(30);
    auto cone = std::make_shared<ConeMesh>(20);
    auto cylinder = std::make_shared<CylinderMesh>(40);
    auto sphere = std::make_shared<SphereMesh>(20);
    auto torus = std::make_shared<TorusMesh>(40);
    auto starTorus = std::make_shared<StarTorusMesh>(20);
    auto axes = std::make_shared<CoordinateAxesMesh>();
    //PolynomialVolume quadratic(1.0f, 6.0f, -5.0f, -8.0f, 2.0f, 20);
    auto poly = std::make_shared<PolynomialMesh>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -10.0f, 10.0f, 100, false);
    //Polynomial quadratic(0.0f, 1.0f, 0.0f, 0.0f, 7.0f, 0.0f, 0.0f, -2.0f, 5.0f, 100, true);

    Texture texture1("textures/photo1.jpg", GL_TEXTURE0);
    Texture texture2("textures/photo2.jpg", GL_TEXTURE1);

    shader.use();
    shader.setTexture(0);
    shader.setTexture(1);

    glEnable(GL_DEPTH_TEST);

    float mix = 0.0f;
    bool forward = true;
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
        shader.setMixer(mix); // debug
        
        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1 / 60) {
            rotation += 0.5f;
            prevTime = crntTime;
        }

        //////////////////Identity - render area is a unit cube////////////////////////
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        shader.use();
        shader.setModel(model);
        shader.setView(view);
        shader.setProjection(projection);

        //circle->draw();
        //rectangle->draw();
        //cube->draw();
        //cone->draw();
        //cylinder->draw();
        //sphere->draw();
        //starTorus->draw();
        //torus->draw();
        //axes->draw();
        //poly->draw();

        ///////////////////Camera - rendered in camera space/////////////////////////
        glm::vec3 position = glm::vec3(camX, camY, camZ);
        glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        //view = glm::lookAt(position, position + orientation, up);
        view = glm::lookAt(position, glm::vec3(0.0f), up);
        shader.setView(view);

        //circle->draw();
        //rectangle->draw();

        ////////////////////Perspective - global coordiate system///////////////////
        projection = glm::perspective(glm::radians(45.0f), 1080 / float(1080), 0.1f, 100.0f);
        shader.setProjection(projection);

        //circle->draw();
        //axes->draw();
        //poly->draw();
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //sphere->draw();
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        ////////////////////Model - per model transforms////////////////////////////
        glm::mat4 model2 = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.0f));
        model2 = glm::rotate(model2, glm::radians(rotation), glm::vec3(0.0f, -1.0f, 0.0f));
        shader.setModel(model2);
        texture1.bind();
        texture2.bind();
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //sphere->draw();
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        texture1.unBind();
        texture2.unBind();

        model2 = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 0.0f, 0.0f));
        model2 = glm::translate(model2, glm::vec3(-1.0f, 0.0f, 0.0f));
        shader.setModel(model2);
        //starTorus->draw();

        model2 = glm::rotate(model, glm::radians(rotation*2), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::translate(model2, glm::vec3(3.0f, 2.0f, 0.0f));
        shader.setModel(model2);
        //cube->draw();

        glm::mat4 model5 = glm::translate(model, glm::vec3(-2.0f, 1.5f, 1.0f));
        model5 = glm::rotate(model5, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.setModel(model5);
        //rectangle->draw();

        //////////////////// Flat shader
        flatShader.use();
        flatShader.setView(view);
        flatShader.setProjection(projection);

        glm::mat4 model3 = glm::mat4(1.0f);
        model3 = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        model3 = glm::rotate(model3, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        flatShader.setModel(model3);
        //sphere->draw();
        //cone->draw();
        //circle->draw();
        //cylinder->draw();

        glm::mat4 model4 = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
        model4 = glm::rotate(model4, glm::radians(rotation*5), glm::vec3(0.0f, 0.0f, 1.0f));
        model4 = glm::rotate(model4, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        flatShader.setModel(model4);
        //circle->draw();
        torus->draw();
        //starTorus->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (mix > 1.0)
        {
            forward = false;
        }

        if (mix < 0.0)
        {
            forward = true;
        }

        if (forward)
        {
            mix += 0.01f;
        }
        else
        {
            mix -= 0.01f;
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}