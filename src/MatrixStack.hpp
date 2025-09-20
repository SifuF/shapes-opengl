#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include "glm/glm.hpp"

#include <stack>

class MatrixStack {
public:
    void push(glm::mat4 matrix) {
        m_stack.push(std::move(matrix));
    }

    glm::mat4 pop() {
        const auto matrix = m_stack.top();
        m_stack.pop();
        return matrix;
    }
    
    MatrixStack() = default;
    ~MatrixStack() = default;
    MatrixStack(const MatrixStack& other) = delete;
    MatrixStack& operator=(const MatrixStack& other) = delete;
    MatrixStack(MatrixStack&& other) = delete;
    MatrixStack& operator=(MatrixStack&& other) = delete;

private:
    std::stack<glm::mat4> m_stack;
};

#endif // MATRIXSTACK_H
 