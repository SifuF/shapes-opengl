#ifndef SHAPEMESH_H
#define SHAPEMESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "Texture.hpp"

#include <cmath>
#include <limits>
#include <numbers>
#include <vector>

class ShapeMesh
{
public:
    void draw() const;
    void setLayout() const;
    void bind() const;
    void unBind() const;

public:
    ShapeMesh() = default;

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    VAO vao;
    VBO vbo;
    EBO ebo;
    int primitive{ GL_TRIANGLES };
    static constexpr int attribCount = 11; // 11 == 3pos + 3col + 2tex + 3 norm
};

class CoordinateAxesMesh : public ShapeMesh
{
public:
    CoordinateAxesMesh();
};

class RectangleMesh : public ShapeMesh
{
public:
    RectangleMesh(GLfloat height, GLfloat width);
};

class CuboidMesh : public ShapeMesh
{
public:
    CuboidMesh(GLfloat width, GLfloat height, GLfloat depth);
};

class CircleMesh : public ShapeMesh
{
public:
    CircleMesh(int n, float r = 1.0f);
};

class CylinderMesh : public ShapeMesh
{
public:
    CylinderMesh(int n, float r = 0.8f);
};

class PolynomialMesh : public ShapeMesh
{
public:
    // ax^4 + bx^3 + cx^2 + dx + e + r/x + s/x^2    // low < x < high    // n points    // given in y^2
    PolynomialMesh(float a, float b, float c, float d, float e, float r, float s, float low, float high, int n, bool ySquared);
};

class ConeMesh : public ShapeMesh
{
public:
    ConeMesh(int n, float r = 1.0f);
};

class SphereMesh : public ShapeMesh
{
public:
    SphereMesh(int n, float r = 1.0f);
};

class TorusMesh : public ShapeMesh
{
public:
    TorusMesh(int n, float R = 0.5f);
};

class StarTorusMesh : public ShapeMesh
{
public:
    StarTorusMesh(int n, float R = 0.5f);
};

#endif // SHAPEMESH_H

