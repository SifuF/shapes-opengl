#include "ShapeMesh.hpp"

#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "Texture.hpp"

#include <cmath>
#include <limits>
#include <numbers>
#include <vector>

void ShapeMesh::draw() const
{
    bind();
    if (!indices.empty()) {
        glDrawElements(primitive, indices.size(), GL_UNSIGNED_INT, 0);
    }
    else {
        glDrawArrays(primitive, 0, vertices.size());
    }
    unBind();
}

void ShapeMesh::setLayout() const
{
    bind();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attribCount * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, attribCount * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, attribCount * sizeof(float), (void*)(6 * sizeof(float)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, attribCount * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    unBind();
}

void ShapeMesh::bind() const
{
    vao.bind();
    vbo.bind();
    ebo.bind();
}

void ShapeMesh::unBind() const
{
    vbo.unBind();
    vao.unBind();
    ebo.unBind();
    // Unbind EBO AFTER unbinding VAO, as the EBO is linked in the VAO
    // This does not apply to the VBO because the VBO is already linked to the VAO during glVertexAttribPointer
}

CoordinateAxesMesh::CoordinateAxesMesh()
{
    GLfloat r = 10.0f;
    vertices = {
        0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,      0.0f, 1.0f,      0.0f, 0.0f, 1.0f,       //  2
        r, 0.0f, 0.0f,      1.0f, 0.0f, 0.0f,      0.0f, 1.0f,      0.0f, 0.0f, 1.0f,       //  | 3
        0.0f, r, 0.0f,      0.0f, 1.0f, 0.0f,      0.0f, 0.0f,      0.0f, 0.0f, 1.0f,       //  |/
        0.0f, 0.0f, r,      0.0f, 0.0f, 1.0f,      1.0f, 1.0f,      0.0f, 0.0f, 1.0f,       //  0-----1
    };

    primitive = GL_LINES;
    indices = {
        0, 1,
        0, 2,
        0, 3
    };

    setLayout();
}

RectangleMesh::RectangleMesh(GLfloat height, GLfloat width)
{
    vertices = {
        -width / 2.0f, height / 2.0f, 0.0f,      1.0f, 0.0f, 0.0f,      0.0f, 1.0f,      0.0f, 0.0f, 1.0f,
        -width / 2.0f, -height / 2.0f, 0.0f,     0.0f, 1.0f, 0.0f,      0.0f, 0.0f,      0.0f, 0.0f, 1.0f,     //  0-------2
        width / 2.0f, height / 2.0f, 0.0f,       0.0f, 0.0f, 1.0f,      1.0f, 1.0f,      0.0f, 0.0f, 1.0f,     //  |       |
        width / 2.0f, -height / 2.0f, 0.0f,      1.0f, 1.0f, 0.0f,      1.0f, 0.0f,      0.0f, 0.0f, 1.0f      //  1-------3
    };

    indices = {
        0, 1, 2,
        1, 3, 2
    };

    setLayout();
}

CuboidMesh::CuboidMesh( GLfloat width, GLfloat height, GLfloat depth)
{
    vertices = {
        -width / 2.0f, height / 2.0f, depth / 2.0f,     1.0f, 0.0f, 0.0f,       0.0f, 1.0f,      0.0f, 0.0f, 1.0f, //front 0
        -width / 2.0f, -height / 2.0f, depth / 2.0f,    0.0f, 1.0f, 0.0f,       0.0f, 0.0f,      0.0f, 0.0f, 1.0f,
        width / 2.0f, height / 2.0f, depth / 2.0f,      0.0f, 0.0f, 1.0f,       1.0f, 1.0f,      0.0f, 0.0f, 1.0f,
        width / 2.0f, -height / 2.0f, depth / 2.0f,     1.0f, 1.0f, 0.0f,       1.0f, 0.0f,      0.0f, 0.0f, 1.0f,

        -width / 2.0f, height / 2.0f, -depth / 2.0f,    1.0f, 0.0f, 0.0f,       0.0f, 1.0f,      0.0f, 0.0f, -1.0f, //back 4
        -width / 2.0f, -height / 2.0f, -depth / 2.0f,   0.0f, 1.0f, 0.0f,       0.0f, 0.0f,      0.0f, 0.0f, -1.0f,
        width / 2.0f, height / 2.0f, -depth / 2.0f,     0.0f, 0.0f, 1.0f,       1.0f, 1.0f,      0.0f, 0.0f, -1.0f,
        width / 2.0f, -height / 2.0f, -depth / 2.0f,    1.0f, 1.0f, 0.0f,       1.0f, 0.0f,      0.0f, 0.0f, -1.0f,

        -width / 2.0f, height / 2.0f, -depth / 2.0f,    1.0f, 0.0f, 0.0f,       0.0f, 1.0f,      -1.0f, 0.0f, 0.0f, //left 8
        -width / 2.0f, -height / 2.0f, -depth / 2.0f,   0.0f, 1.0f, 0.0f,       0.0f, 0.0f,      -1.0f, 0.0f, 0.0f,
        -width / 2.0f, height / 2.0f, depth / 2.0f,     1.0f, 0.0f, 0.0f,       1.0f, 1.0f,      -1.0f, 0.0f, 0.0f,
        -width / 2.0f, -height / 2.0f, depth / 2.0f,    0.0f, 1.0f, 0.0f,       1.0f, 0.0f,      -1.0f, 0.0f, 0.0f,

        width / 2.0f, height / 2.0f, depth / 2.0f,      0.0f, 0.0f, 1.0f,       0.0f, 1.0f,     1.0f, 0.0f, 0.0f, //right 12
        width / 2.0f, -height / 2.0f, depth / 2.0f,     1.0f, 1.0f, 0.0f,       0.0f, 0.0f,     1.0f, 0.0f, 0.0f,
        width / 2.0f, height / 2.0f, -depth / 2.0f,     0.0f, 0.0f, 1.0f,       1.0f, 1.0f,     1.0f, 0.0f, 0.0f,
        width / 2.0f, -height / 2.0f, -depth / 2.0f,    1.0f, 1.0f, 0.0f,       1.0f, 0.0f,     1.0f, 0.0f, 0.0f,

        -width / 2.0f, height / 2.0f, -depth / 2.0f,    1.0f, 0.0f, 0.0f,       0.0f, 1.0f,     0.0f, 1.0f, 0.0f, //top 16
        -width / 2.0f, height / 2.0f, depth / 2.0f,     1.0f, 0.0f, 0.0f,       0.0f, 0.0f,     0.0f, 1.0f, 0.0f,                    //  16       18
        width / 2.0f, height / 2.0f, -depth / 2.0f,     0.0f, 0.0f, 1.0f,       1.0f, 1.0f,     0.0f, 1.0f, 0.0f,                    //     8 4--------6 14
        width / 2.0f, height / 2.0f, depth / 2.0f,      0.0f, 0.0f, 1.0f,       1.0f, 0.0f,     0.0f, 1.0f, 0.0f,                    //      /|       /|
                                                                                                                                        //  10 0--------2 | 12
        -width / 2.0f, -height / 2.0f, -depth / 2.0f,   0.0f, 1.0f, 0.0f,       0.0f, 1.0f,     0.0f, -1.0f, 0.0f, //bottom 20       //     | |      | |
        -width / 2.0f, -height / 2.0f, depth / 2.0f,    0.0f, 1.0f, 0.0f,       0.0f, 0.0f,     0.0f, -1.0f, 0.0f,                   //    9| 5------|-7 15
        width / 2.0f, -height / 2.0f, -depth / 2.0f,    1.0f, 1.0f, 0.0f,       1.0f, 1.0f,     0.0f, -1.0f, 0.0f,                   //     |/       |/
        width / 2.0f, -height / 2.0f, depth / 2.0f,     1.0f, 1.0f, 0.0f,       1.0f, 0.0f,     0.0f, -1.0f, 0.0f                    //  11 1--------3 13
    };

    indices = {
        0, 1, 2,        1, 3, 2,
        6, 7, 5,        5, 4, 6,
        8, 9, 11,       11, 10, 8,
        12, 13, 15,     15, 14, 12,
        16, 17, 18,     17, 19, 18,
        20, 21, 22,     21, 23, 22
    };

    setLayout();
}

CircleMesh::CircleMesh(int n, float r)
{
    // n steps requires n+1 verts including middle
    vertices = { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.5f, 0.5f,   0.0f, 0.0f, -1.0f }; // middle vertex
    vertices.resize(attribCount * (n + 1));

    for (int i = attribCount, j = 0; i < vertices.size(); i += attribCount, ++j) {
        vertices[i] = r * cos(j * 2 * std::numbers::pi / n);
        vertices[i + 1] = r * sin(j * 2 * std::numbers::pi / n);
        vertices[i + 2] = 0.0f;

        vertices[i + 3] = rand() / (1.0 * RAND_MAX);
        vertices[i + 4] = rand() / (1.0 * RAND_MAX);
        vertices[i + 5] = rand() / (1.0 * RAND_MAX);

        vertices[i + 6] = (vertices[i] / r + 1.0f) * 0.5f;
        vertices[i + 7] = (vertices[i + 1] / r + 1.0f) * 0.5f;

        vertices[i + 8] = 0.0f;
        vertices[i + 9] = 0.0f;
        vertices[i + 10] = -1.0f;
    }

    // need 3n indices
    indices.resize(3 * n);
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    for (int i = 3; i < indices.size(); i += 3) {
        indices[i] = 0;
        indices[i + 1] = indices[i - 1];
        indices[i + 2] = indices[i - 1] + 1;
    }
    indices[indices.size() - 1] = indices[1];

    setLayout();
}

CylinderMesh::CylinderMesh(int n, float r)
{
    // n steps requires n+1 verts including middle
    vertices.resize(attribCount * (n + 1) * 2);
    float h = 1.0f;

    //bottom circle
    vertices[0] = 0.0f;
    vertices[1] = -h;
    vertices[2] = 0.0f;

    vertices[3] = rand() / (1.0 * RAND_MAX);
    vertices[4] = rand() / (1.0 * RAND_MAX);
    vertices[5] = rand() / (1.0 * RAND_MAX);

    vertices[6] = 0.5f;
    vertices[7] = 0.5f;

    vertices[8] = 0.0f;
    vertices[9] = 0.0f;
    vertices[10] = -1.0f;

    int j = 0;
    int i = attribCount;
    for (; i < (vertices.size() - attribCount) / 2; i += attribCount, ++j) {
        vertices[i] = r * cos(j * 2 * std::numbers::pi / n);
        vertices[i + 1] = -h;
        vertices[i + 2] = r * sin(j * 2 * std::numbers::pi / n);

        vertices[i + 3] = rand() / (1.0 * RAND_MAX);
        vertices[i + 4] = rand() / (1.0 * RAND_MAX);
        vertices[i + 5] = rand() / (1.0 * RAND_MAX);

        vertices[i + 6] = (vertices[i] / r + 1.0f) * 0.5f;
        vertices[i + 7] = (vertices[i + 1] / r + 1.0f) * 0.5f;

        vertices[i + 8] = 0.0f;
        vertices[i + 9] = 0.0f;
        vertices[i + 10] = -1.0f;
    }

    //top circle
    for (j = 0; i < vertices.size() - attribCount; i += attribCount, ++j) {
        vertices[i] = r * cos(j * 2 * std::numbers::pi / n);
        vertices[i + 1] = h;
        vertices[i + 2] = r * sin(j * 2 * std::numbers::pi / n);

        vertices[i + 3] = rand() / (1.0 * RAND_MAX);
        vertices[i + 4] = rand() / (1.0 * RAND_MAX);
        vertices[i + 5] = rand() / (1.0 * RAND_MAX);

        vertices[i + 6] = (vertices[i] / r + 1.0f) * 0.5f;
        vertices[i + 7] = (vertices[i + 1] / r + 1.0f) * 0.5f;

        vertices[i + 8] = 0.0f;
        vertices[i + 9] = 0.0f;
        vertices[i + 10] = -1.0f;
    }

    vertices[i] = 0.0f;
    vertices[i + 1] = h;
    vertices[i + 2] = 0.0f;

    vertices[i + 3] = 0.0f;
    vertices[i + 4] = 0.0f;
    vertices[i + 5] = 0.0f;

    vertices[i + 6] = 0.5f;
    vertices[i + 7] = 0.5f;

    vertices[i + 8] = 0.0f;
    vertices[i + 9] = 0.0f;
    vertices[i + 10] = -1.0f;

    // need 3n indices
    indices.resize(3 * n + 6 * n + 3 * n);

    // bottom
    // indices[0] = 0   indices[1] = 1   indices[2] = 2             // n=6 => points = 18
    // indices[3] = 0   indices[4] = 2   indices[5] = 3             //
    // indices[6] = 0   indices[7] = 3   indices[8] = 4             //       3______2
    // indices[9] = 0   indices[10] = 4  indices[11] = 5            //       /      \
    // indices[12] = 0  indices[13] = 5  indices[14] = 6            //     4/   0    \ 1
    // indices[12] = 0  indices[13] = 5  indices[14] = 6            //      \        /
    // indices[15] = 0  indices[16] = 6  indices[17 == 3n-1] = 1    //      5\______/6

    int l = 1;
    for (i = 0; i < 3*(n-1); i += 3, ++l) {
        indices[i] = 0;             // indices[0] to indices[3n-6]
        indices[i + 1] = l;         // indices[1] to indices[3n-5]
        indices[i + 2] = l + 1;     // indices[2] to indices[3n-4]
    }
    indices[i] = 0;         // indices[3n-3]
    indices[i + 1] = l;     // indices[3n-2]
    indices[i + 2] = 1;     // indices[3n-1]

    // hollow cone side
    // indices[18 == 3n] = 7    indices[21] = 2     //       n=6 => points = 18 + 36
    // indices[19] = 1          indices[22] = 8     //
    // indices[20] = 2          indices[23] = 7     //       9______8
                                                    //       /      \
    // indices[24] = 8          indices[27] = 3     //    10/   13   \7
    // indices[25] = 2          indices[28] = 9     //      \        /
    // indices[26] = 3          indices[29] = 8     //     11\______/12

    // indices[30] = 9          indices[33] = 4
    // indices[31] = 3          indices[34] = 10
    // indices[32] = 4          indices[35] = 9

    // indices[36] = 10         indices[39] = 5     //       3______2
    // indices[37] = 4          indices[40] = 11    //       /      \
    // indices[38] = 5          indices[41] = 10    //     4/   0    \1
                                                    //      \        /
    // indices[42] = 11         indices[45] = 6     //      5\______/6
    // indices[43] = 5          indices[46] = 12
    // indices[44] = 6          indices[47] = 11

    // indices[48] = 12         indices[51] = 1
    // indices[49] = 6          indices[52] = 7
    // indices[50] = 1          indices[53 == 18 + 2(18)-1 == 3n + 2(3n)-1] = 12

    for (i = 3 * n, l = 1; i < (3 * n + 2 * 3 * n - 6); i += 6, ++l) {
        indices[i] = n + l;             // indices[3n] to indices[3n + 2(3n)-6]
        indices[i + 1] = l;             // indices[3n+1] to indices[3n + 2(3n)-5]
        indices[i + 2] = l + 1;         // indices[3n+2] to indices[3n + 2(3n)-4]

        indices[i + 3] = l + 1;         // indices[3n+3] to indices[3n + 2(3n)-3]
        indices[i + 4] = n + l + 1;     // indices[3n+4] to indices[3n + 2(3n)-2]
        indices[i + 5] = n + l;         // indices[3n+5] to indices[3n + 2(3n)-1]
    }
    indices[i] = 2*n-(n-1); // redo last triangle
    indices[i + 1] = n;
    indices[i + 2] = 1;

    indices[i + 3] = 2*n; // redo last triangle
    indices[i + 4] = 2 * n - (n - 1);
    indices[i + 5] = n;

    //top
    // n = 6 = > points = 18 + 36 + 18 = 72
    int k = 3 * n + 2 * 3 * n;
    for (i = 0, l = 1; i < 3 * (n - 1); i += 3, ++l) {
        indices[k + i] = (n + 1) * 2 - 1;   // indices[3n + 2(3)n] to indices[3n-5] = last vertex
        indices[k + i + 1] = n + l;         // indices[3n + 2(3)n + 1] to indices[3n-5]
        indices[k + i + 2] = n + l + 1;     // indices[3n + 2(3)n + 2] to indices[3n-4]
    }
    indices[k + i] = (n + 1) * 2 - 1;       // last vertex
    indices[k + i + 1] = n+l;               // indices[3n-2]
    indices[k + i + 2] = n+1;               // indices[3n-1]

    setLayout();
}

PolynomialMesh::PolynomialMesh(float a, float b, float c, float d, float e, float r, float s, float low, float high, int n, bool ySquared)
{
    float dx = (high - low) / float(n-1);
    vertices.resize(attribCount * n);

    float err = 0.00001;
    for (int i = 0; i < vertices.size(); i+= attribCount) {
        float x = low + (i / float(attribCount)) * dx;

        float A = a * x * x * x * x;
        float B = b * x * x * x;
        float C = c * x * x;
        float D = d * x;
        float E = e;
        float R;
        float S;
        if (std::abs(x) > err) {
            R = r / x;
            S = s / (x * x);
        }
        else {
            R = std::numeric_limits<float>::max();
            S = std::numeric_limits<float>::max();
        }

        float y = A + B + C + D + E + R + S;
        if (ySquared) {
            y = std::sqrt(y);
        }

        x *= 0.15f;
        y *= 0.15f;

        vertices[i] = x;
        vertices[i + 1] = y;
        vertices[i + 2] = 0.0f;

        vertices[i + 3] = 1.0f;
        vertices[i + 4] = 0.0f;
        vertices[i + 5] = 0.0f;

        vertices[i + 6] = 0.0f;
        vertices[i + 7] = 1.0f;

        vertices[i + 8] = 0.0f;
        vertices[i + 9] = 0.0f;
        vertices[i + 10] = 0.0f;
    }

    primitive = GL_LINES;
    indices.resize(2 * n);  // points = indices.size() - 1;
    int k = 0;
    for (int i = 0; i < indices.size() - 1; i+=2) {
        indices[i] = k;
        indices[i + 1] = k + 1;
        k++;
    }

    setLayout();
}

ConeMesh::ConeMesh(int n, float r)
{
    //n steps requires n+2 verts including both middles
    vertices.resize(attribCount * (n + 2));
    float h = 1.0f;

    vertices[0] = 0.0f;
    vertices[1] = h;
    vertices[2] = 0.0f;

    vertices[3] = 0.0f;
    vertices[4] = 1.0f;
    vertices[5] = 0.0f;

    vertices[6] = 0.5f;
    vertices[7] = 0.5f;

    vertices[8] = 0.0f;
    vertices[9] = 0.0f;
    vertices[10] = -1.0f;

    for (int i = attribCount, j = 0; i < vertices.size(); i += attribCount, ++j) {
        vertices[i] = r * cos(j * 2 * std::numbers::pi / n);
        vertices[i + 1] = -h;
        vertices[i + 2] = r * sin(j * 2 * std::numbers::pi / n);;

        vertices[i + 3] = rand() / (1.0 * RAND_MAX);
        vertices[i + 4] = rand() / (1.0 * RAND_MAX);
        vertices[i + 5] = rand() / (1.0 * RAND_MAX);

        vertices[i + 6] = (vertices[i] / r + 1.0f) * 0.5f;
        vertices[i + 7] = (vertices[i + 1] / r + 1.0f) * 0.5f;

        vertices[i + 8] = 0.0f;
        vertices[i + 9] = 0.0f;
        vertices[i + 10] = -1.0f;
    }

    //need 3n indices
    indices.resize(3 * n);

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    for (int i = 3; i < indices.size(); i += 3) {
        indices[i] = 0;
        indices[i + 1] = indices[i - 1];
        indices[i + 2] = indices[i - 1] + 1;
    }
    indices[indices.size() - 1] = indices[1];

    setLayout();
}

SphereMesh::SphereMesh(int n, float r)
{
    // n verts for the circle (no middle)
    // times m+1 for the sphere
    int m = n;
    vertices.resize(attribCount * (n * (m + 1)));

    for (int k = 0; k <= m; k++) {
        for (int i = 0, j = 0; i < attribCount * n; i += attribCount, ++j) {
            vertices[k * (attribCount * n) + i] = r * cos(j * 2.0f * std::numbers::pi / n) * sin(k * std::numbers::pi / m);
            vertices[k * (attribCount * n) + i + 1] = r * sin(j * 2.0f * std::numbers::pi / n) * sin(k * std::numbers::pi / m);
            vertices[k * (attribCount * n) + i + 2] = r * cos(k * std::numbers::pi / m);

            vertices[k * (attribCount * n) + i + 3] = rand() / (1.0 * RAND_MAX);
            vertices[k * (attribCount * n) + i + 4] = rand() / (1.0 * RAND_MAX);
            vertices[k * (attribCount * n) + i + 5] = rand() / (1.0 * RAND_MAX);

            vertices[k * (attribCount * n) + i + 6] = (vertices[k * (attribCount * n) + i] / r + 1) * 0.5f;
            vertices[k * (attribCount * n) + i + 7] = (vertices[k * (attribCount * n) + i + 1] / r + 1) * 0.5f;

            vertices[k * (attribCount * n) + i + 8] = 0.0f;
            vertices[k * (attribCount * n) + i + 9] = 0.0f;
            vertices[k * (attribCount * n) + i + 10] = -1.0f;
        }
    }

    indices.resize(6 * (n+1) * m);
    for (int j = 0, k = 0; j < m; j++, k+=6) {
        for (int i = 0; i < n-1; i++, k+=6) {
            indices[k] = i + (j + 1) * n;;
            indices[k+1] = i + j * n;
            indices[k+2] = i + 1 + (j + 1) * n;

            indices[k+3] = i + j * n;
            indices[k+4] = i + 1 + j * n;
            indices[k+5] = i + 1 + (j + 1) * n;
        }
        indices[k] = n-1 + (j + 1) * n;;
        indices[k + 1] = n-1 + j * n;
        indices[k + 2] = (j+1)*n;

        indices[k + 3] = n-1 + j * n;
        indices[k + 4] = j * n;
        indices[k + 5] = (j + 1)* n;
    }

    setLayout();
}

TorusMesh::TorusMesh(int n, float R) {
    int m = n;
    float r = R / 2;
    vertices.resize(n * m * attribCount);

    for (int j = 0, k = 0; k < m; ++j, ++k) {
        for (int i = 0; i < attribCount * n; i += attribCount) {
            int offset = k * attribCount * n;
            GLfloat A = (r * cos(i * 2.0f * std::numbers::pi / (attribCount * n)) + R) * cos(j * 2.0f * std::numbers::pi / (m));
            GLfloat B = r * sin(i * 2.0f * std::numbers::pi / (attribCount * n));
            GLfloat C = (r * cos(i * 2.0f * std::numbers::pi / (attribCount * n)) + R) * sin(j * 2.0f * std::numbers::pi / (m));

            vertices[offset + i] = C;
            vertices[offset + i + 1] = A;
            vertices[offset + i + 2] = B;

            vertices[offset + i + 3] = rand() / (1.0 * RAND_MAX);
            vertices[offset + i + 4] = rand() / (1.0 * RAND_MAX);
            vertices[offset + i + 5] = rand() / (1.0 * RAND_MAX);

            vertices[offset + i + 6] = 0.0;
            vertices[offset + i + 7] = 0.0;

            vertices[offset + i + 8] = 0.0;
            vertices[offset + i + 9] = 0.0;
            vertices[offset + i + 10] = 0.0;
        }
    }

    indices.resize(6 * (n) * (m));
    int k = 0;
    for (int j = 0; j < m-1; ++j, k+=6)
    {
        for (int i = 0; i < n - 1; ++i, k+=6)
        {
            indices[k] = i + (j + 1) * n;
            indices[k + 1] = i + j * n;
            indices[k + 2] = i + 1 + (j + 1) * n;

            indices[k + 3] = i + j * n;
            indices[k + 4] = i + 1 + j * n;
            indices[k + 5] = i + 1 + (j + 1) * n;
        }
        indices[k] = n - 1 + (j + 1) * n;
        indices[k + 1] = n - 1 + j * n;
        indices[k + 2] = (j + 1) * n;

        indices[k + 3] = n - 1 + j * n;
        indices[k + 4] = j * n;
        indices[k + 5] = (j + 1) * n;
    }

    for (int i = 0; i < n - 1; ++i, k+=6)
    {
        indices[k] = m * (n - 1) + i;
        indices[k + 1] = m * (n - 1) + i + 1;
        indices[k + 2] = i;

        indices[k + 3] = m * (n - 1) + i + 1;
        indices[k + 4] = i + 1;
        indices[k + 5] = i;
    }

    indices[k ] = m*(n-1);
    indices[k+1] = n*m - 1;
    indices[k + 2] = 0;

    indices[k + 3] = n * m - 1;
    indices[k + 4] = n-1;
    indices[k + 5] = 0;

    setLayout();
}

StarTorusMesh::StarTorusMesh(int n, float R) {
    int m = n;
    float r = R / 2;
    vertices.resize( n * m * attribCount);

    int j = 0;
    for (int j = 0, k = 0; k < m; ++j, ++k) {
        for (int i = 0; i < attribCount * n; i += attribCount) {
            int offset = k * attribCount * n;

            GLfloat A = (r * cos(i * 2.0f * std::numbers::pi / n) + R) * cos(j * 2.0f * std::numbers::pi / m);
            GLfloat B = r * sin(i * 2.0f * std::numbers::pi / n);
            GLfloat C = (r * cos(i * 2.0f * std::numbers::pi / n) + R) * sin(j * 2.0f * std::numbers::pi / m);

            vertices[offset + i] = C;
            vertices[offset + i + 1] = A;
            vertices[offset + i + 2] = B;

            vertices[offset + i + 3] = rand() / (1.0 * RAND_MAX);
            vertices[offset + i + 4] = rand() / (1.0 * RAND_MAX);
            vertices[offset + i + 5] = rand() / (1.0 * RAND_MAX);

            vertices[offset + i + 6] = 0.0;
            vertices[offset + i + 7] = 0.0;

            vertices[offset + i + 8] = 0.0;
            vertices[offset + i + 9] = 0.0;
            vertices[offset + i + 10] = 0.0;
        }
    }

    indices.resize( 6 * (n + 1) * m );
    for (int j = 0, k = 0; j < m; ++j, k+=6) {
        for (int i = 0; i < n - 1; ++i, k+=6) {
            indices[k] = i + (j + 1) * n;;
            indices[k + 1] = i + j * n;
            indices[k + 2] = i + 1 + (j + 1) * n;

            indices[k + 3] = i + j * n;
            indices[k + 4] = i + 1 + j * n;
            indices[k + 5] = i + 1 + (j + 1) * n;
        }

        indices[k] = n - 1 + (j + 1) * n;;
        indices[k + 1] = n - 1 + j * n;
        indices[k + 2] = (j + 1) * n;

        indices[k + 3] = n - 1 + j * n;
        indices[k + 4] = j * n;
        indices[k + 5] = (j + 1) * n;
    }
    indices[indices.size() - 1] = indices[1];

    setLayout();
}
