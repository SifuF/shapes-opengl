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

bool throttle = false;
bool throttleR = false;
bool spinLeft = false;
bool spinRight = false;

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

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) { throttle = true; }
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE) { throttle = false; }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) { throttleR = true; }
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) { throttleR = false; }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) { spinLeft = true; }
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) { spinLeft = false; }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) { spinRight = true; }
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) { spinRight = false; }
    
    if (key == GLFW_KEY_W && action == GLFW_PRESS) { camY += 1.0; }
    if (key == GLFW_KEY_S && action == GLFW_PRESS){ camY -= 1.0; }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) { camX -= 1.0; }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) { camX += 1.0; }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS) { camZ -= 1.0; }
    if (key == GLFW_KEY_X && action == GLFW_PRESS) { camZ += 1.0; }
}

void drawPlane(Shader& shader, const glm::mat4& model, std::shared_ptr<CylinderMesh> cylinder, std::shared_ptr<ConeMesh> cone, std::shared_ptr<CuboidMesh> cube) {
    {
        const glm::mat4 bodyT = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        const glm::mat4 bodyS = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 1.0f, 0.3f));
        shader.setModel(bodyT * bodyS);
        cylinder->draw();

        const glm::mat4 noseT = glm::translate(bodyT, glm::vec3(0.0f, 1.2f, 0.0f));
        const glm::mat4 noseS = glm::scale(glm::mat4(1.0f), glm::vec3(0.24f, 0.2f, 0.22f));
        shader.setModel(noseT * noseS);
        cone->draw();

        const glm::mat4 frontWingT = glm::translate(bodyT, glm::vec3(0.0f, 0.3f, 0.0f));
        const glm::mat4 frontWingS = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 0.3f, 0.3f));
        shader.setModel(frontWingT * frontWingS);
        cube->draw();

        const glm::mat4 rearWingT = glm::translate(bodyT, glm::vec3(0.0f, -0.8f, 0.0f));
        const glm::mat4 rearWingS = glm::scale(glm::mat4(1.0f), glm::vec3(1.2f, 0.2f, 0.2f));
        shader.setModel(rearWingT * rearWingS);
        cube->draw();

        const glm::mat4 tailT = glm::translate(rearWingT, glm::vec3(0.0f, 0.0f, 0.3f));
        const glm::mat4 tailS = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.2f, 0.2f));
        shader.setModel(tailT * tailS);
        cube->draw();
    }
}

void drawCar(Shader& shader, const glm::mat4& model, std::shared_ptr<CuboidMesh> cubeoid, std::shared_ptr<TorusMesh> torus) {
    const glm::mat4 bodyT = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    const glm::mat4 bodyS = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.7f, 0.15f));
    shader.setModel(bodyT * bodyS);
    cubeoid->draw();

    const glm::mat4 cabinT = glm::translate(model, glm::vec3(0.0f, -0.05f, 0.13f));
    const glm::mat4 cabinS = glm::scale(bodyS, glm::vec3(0.7f, 0.5f, 0.7f));
    shader.setModel(cabinT * cabinS);
    cubeoid->draw();

    static float angle = 0.0f;
    angle += 3.0f;

    const glm::mat4 leftFrontT = glm::translate(model, glm::vec3(-0.19f, 0.2f, 0.0f));
    const glm::mat4 leftFrontS = glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 0.15f));
    glm::mat4 leftFrontR = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.1f, 0.0f));
    leftFrontR = glm::rotate(leftFrontR, glm::radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f));
    shader.setModel(leftFrontT * leftFrontS * leftFrontR);
    torus->draw();

    const glm::mat4 rightFrontT = glm::translate(model, glm::vec3(0.19f, 0.2f, 0.0f));
    const glm::mat4 rightFrontS = glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 0.15f));
    glm::mat4 rightFrontR = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.1f, 0.0f));
    rightFrontR = glm::rotate(rightFrontR, glm::radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f));
    shader.setModel(rightFrontT * rightFrontS * rightFrontR);
    torus->draw();

    const glm::mat4 leftBackT = glm::translate(model, glm::vec3(-0.19f, -0.2f, 0.0f));
    const glm::mat4 leftBackS = glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 0.15f));
    glm::mat4 leftBackR = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.1f, 0.0f));
    leftBackR = glm::rotate(leftBackR, glm::radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f));
    shader.setModel(leftBackT * leftBackS * leftBackR);
    torus->draw();

    const glm::mat4 rightBackT = glm::translate(model, glm::vec3(0.19f, -0.2f, 0.0f));
    const glm::mat4 rightBackS = glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 0.15f));
    glm::mat4 rightBackR = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.1f, 0.0f));
    rightBackR = glm::rotate(rightBackR, glm::radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f));
    shader.setModel(rightBackT * rightBackS * rightBackR);
    torus->draw();
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

    auto cube = std::make_shared<CuboidMesh>(1.0f, 1.0f, 1.0f);
    auto cone = std::make_shared<ConeMesh>(20);
    auto cylinder = std::make_shared<CylinderMesh>(40);
    auto sphere = std::make_shared<SphereMesh>(20);
    auto torus = std::make_shared<TorusMesh>(40);

    glEnable(GL_DEPTH_TEST);

    float mix = 0.0f;
    bool forward = true;
    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    glm::vec3 planeUp(0.0f, 0.0f, 1.0f);

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

        // Identity - render area is a unit cube
        shader.use();
        shader.setProjection(glm::mat4(1.0f));
        shader.setView(glm::mat4(1.0f));
        shader.setModel(glm::mat4(1.0f));
        //circle->draw();

        //Perspective - global coordiate system
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1080 / float(1080), 0.1f, 100.0f);
        shader.setProjection(projection);

        // Camera - rendered in camera space
        const glm::vec3 position = glm::vec3(camX, camY, camZ);
        const glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        const glm::mat4 view = glm::lookAt(position, glm::vec3(0.0f), up);
        shader.setView(view);

        // Model - per model transforms
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, -2.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, -1.0f, 0.0f));
        shader.setModel(model);

        static bool barrelLeft = false;
        static float barrelRot = 0.0f;
        if (barrelLeft) {
            barrelRot -= 0.005f;
        }
        else {
            barrelRot += 0.005f;
        }
        if (barrelRot > 0.2f) {
            barrelLeft = true;
        }
        else if (barrelRot < -0.2f) {
            barrelLeft = false;
        }
        
        static float planeAngle = 0.0f;
        static float planeYaw = 0.0f;
        if (throttle) {
            planeAngle -= 0.5f;
        }
        else if (throttleR) {
            planeAngle += 0.5f;
        }

        if (spinLeft) {
            planeYaw += 1.0f;
        }
        else if (spinRight) {
            planeYaw -= 1.0f;
        }

        glm::mat4 planeModel = glm::mat4(1.0f);
        planeModel = glm::rotate(planeModel, glm::radians(planeYaw), planeUp);
        planeModel = glm::rotate(planeModel, glm::radians(planeAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        planeModel = glm::translate(planeModel, glm::vec3(0.0f, 0.0f, 1.5f));
        planeUp = glm::vec3(planeModel[2]);
        drawPlane(shader, planeModel, cylinder, cone, cube);

        glm::mat4 carModel = glm::mat4(1.0f);
        carModel = glm::rotate(carModel, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        carModel = glm::rotate(carModel, glm::radians(-rotation * 2.5f), glm::vec3(1.0f, 0.0f, 1.0f));
        carModel = glm::translate(carModel, glm::vec3(0.0f, 0.0f, 1.03f));
        carModel = glm::scale(carModel, glm::vec3(0.5f, 0.5f, 0.5f));
        drawCar(shader, carModel, cube, torus);

        flatShader.use();
        flatShader.setView(view);
        flatShader.setProjection(projection);
        {
            glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, -10.0f));
            glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(rotation * 0.05f), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 20.0f, 20.0f));
            flatShader.use();
            flatShader.setModel(T * R * S);
            torus->draw();
        }

        {
            glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(rotation * 1), glm::vec3(0.0f, 1.0f, 1.0f));
            glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
            flatShader.use();
            flatShader.setModel(T * R);
            sphere->draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
