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
#include <vector>

constexpr int NUM_CARS = 100;
bool controlPlane = false;
unsigned carSelector = 0;
bool throttle = false;
bool throttleR = false;
bool spinLeft = false;
bool spinRight = false;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400;
float lastY = 300;
float yaw = -90.0f;
float pitch = 0.0f;
float roll = 0.0f;
bool firstMouse = true;
float fov = 45.0f;

class Sphere
{
public:
    Sphere(std::shared_ptr<SphereMesh> mesh) : m_mesh(mesh) {}

    glm::mat4 getModel()
    {
        glm::mat4 model = glm::translate(glm::mat4{1.0f}, m_position);
        model = glm::rotate(model, m_angle, m_rotation);
        model = glm::scale(model, glm::vec3{m_size});
        return model;
    }

    void update()
    {
        m_angle += 0.01f;
        if (m_angle >= 360.0f)
        {
            m_angle = 0.0f;
        }
    }

    void draw()
    {
        m_mesh->draw();
    }

private:
    std::shared_ptr<SphereMesh> m_mesh = nullptr;
    glm::vec3 m_position{0.0f, 0.0f, 0.0f};
    glm::vec3 m_rotation{0.0f, 1.0f, 0.0f};
    float m_size{ 1.0f };
    float m_angle{ 0.0f };
};

class GlobeTransform
{
public:
    GlobeTransform(float radius, float size, bool randomPos = false)
        : m_position(glm::vec3{ 0.0f, 0.0f, radius }), m_size(size)
    {
        if (randomPos)
        {
            auto getRandom = [] {return (rand() - (RAND_MAX/2.0f)) / (1.0f*RAND_MAX); };
            m_position = radius * glm::normalize(glm::vec3{ getRandom(), getRandom(), getRandom() });
        }
    }

    glm::mat4 getModel()
    {
        const glm::vec3 forward = glm::normalize(-m_forward);
        const glm::vec3 right = glm::normalize(glm::cross(m_position - m_origin, forward));
        const glm::vec3 up = glm::cross(forward, right);

        glm::mat4 rotation = glm::mat4(1.0f);
        rotation[0] = glm::vec4(right, 0.0f);
        rotation[1] = glm::vec4(forward, 0.0f);
        rotation[2] = glm::vec4(up, 0.0f);

        const glm::mat4 translation = glm::translate(glm::mat4{1.0f}, m_position);

        const glm::mat4 scale = glm::scale(glm::mat4{1.0f}, glm::vec3{m_size});
        
        return translation * rotation * scale;
    }

    float getWheelAngle()
    {
        return m_wheelAngle;
    }

    void move(bool backwards = false)
    {
        const glm::vec3 rightVector = glm::normalize(glm::cross(m_forward, m_position - m_origin));
        const glm::mat4 rotation = glm::rotate(glm::mat4{1.0f}, backwards ? -0.01f : 0.01f, rightVector);
        m_position = rotation * glm::vec4(m_position, 1.0f);
        m_forward = rotation * glm::vec4(m_forward, 1.0f);
        backwards ? m_wheelAngle -= 3.0f : m_wheelAngle += 3.0f;
    }

    void yaw(bool right = false)
    {
        const float delta = right ? -0.03f : 0.03f;
        const glm::vec3 up = glm::normalize(m_position - m_origin);
        const glm::mat4 yawMat = glm::rotate(glm::mat4{1.0f}, delta, up);
        m_forward = yawMat * glm::vec4(m_forward, 1.0f);
    }

private:
    glm::vec3 m_position;
    glm::vec3 m_forward{0.0f, 1.0f, 0.0f};
    glm::vec3 m_origin{0.0f, 0.0f, 0.0f};
    float m_size{ 0.15f };
    float m_wheelAngle{ 0.0f };
};

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    const float cameraSpeed = 2.5f * deltaTime;

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) { throttle = true; }
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE) { throttle = false; }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) { throttleR = true; }
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) { throttleR = false; }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) { spinLeft = true; }
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) { spinLeft = false; }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) { spinRight = true; }
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) { spinRight = false; }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) { controlPlane = !controlPlane; }
    if (key == GLFW_KEY_C && action == GLFW_PRESS) { 
        carSelector++;
        if (carSelector >= NUM_CARS) { carSelector = 0; }
    }
}

void processInput(GLFWwindow* window)
{
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
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

void drawCar(Shader& shader, const glm::mat4& model, std::shared_ptr<CuboidMesh> cubeoid, std::shared_ptr<TorusMesh> torus, float angle) {
    const glm::mat4 bodyT = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    const glm::mat4 bodyS = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.7f, 0.15f));
    shader.setModel(bodyT * bodyS);
    cubeoid->draw();

    const glm::mat4 cabinT = glm::translate(model, glm::vec3(0.0f, -0.05f, 0.13f));
    const glm::mat4 cabinS = glm::scale(bodyS, glm::vec3(0.7f, 0.5f, 0.7f));
    shader.setModel(cabinT * cabinS);
    cubeoid->draw();

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSwapInterval(1);

    gladLoadGL();

    Shader shader("../src/shaders/default.vert", "../src/shaders/default.frag");
    Shader flatShader("../src/shaders/flat.vert", "../src/shaders/flat.frag");

    auto cube = std::make_shared<CuboidMesh>(1.0f, 1.0f, 1.0f);
    auto cone = std::make_shared<ConeMesh>(20);
    auto cylinder = std::make_shared<CylinderMesh>(40);
    auto sphere = std::make_shared<SphereMesh>(20);
    auto torus = std::make_shared<TorusMesh>(40);

    Sphere sphereObject{ sphere };
    GlobeTransform planeTransform{ 1.2f, 0.25f };

    std::vector<GlobeTransform> carTransforms;
    for (int i = 0; i < NUM_CARS; ++i) {
        carTransforms.emplace_back(1.02f, 0.3f);
    }

    glEnable(GL_DEPTH_TEST);

    float mix = 0.0f;
    bool forward = true;
    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

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

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 projection = glm::perspective(glm::radians(fov), 1080 / float(1080), 0.1f, 100.0f);
        
        const glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        shader.use();
        shader.setProjection(projection);
        shader.setView(view);

        if (throttle && !controlPlane && carSelector == 0) {
            carTransforms[0].move();
        }
        else if (throttleR && !controlPlane && carSelector == 0) {
            carTransforms[0].move(true);
        }
        for (int i = 1; i < NUM_CARS; ++i) {
            carTransforms[i].move();
        }
       
        planeTransform.move();

        if (spinLeft) {
            controlPlane ? planeTransform.yaw() : carTransforms[carSelector].yaw();
        }
        else if (spinRight) {
            controlPlane ? planeTransform.yaw(true) : carTransforms[carSelector].yaw(true);
        }
        auto planeModel = planeTransform.getModel();
        shader.setModel(planeModel);
        drawPlane(shader, planeModel, cylinder, cone, cube);

        for (auto& carTransform : carTransforms) {
            auto carModel = carTransform.getModel();
            shader.setModel(carModel);
            drawCar(shader, carModel, cube, torus, carTransform.getWheelAngle());
        }

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

        auto sphereObjectModel = sphereObject.getModel();
        flatShader.setModel(sphereObjectModel);
        sphereObject.draw();
        sphereObject.update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
