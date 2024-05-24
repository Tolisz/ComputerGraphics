#include "waterGrid.h"

#include <vector>
#include <glm/vec3.hpp>

waterGrid::waterGrid() 
    : waterGrid(256, 2.0f, 1.0f)
{ }

waterGrid::waterGrid(int N, float a, float c)
    : N{N}, a{a}, c{c}
{
    h = a / (N - 1);
    dt = 1.0f / N;
}

waterGrid::~waterGrid()
{
    glDeleteVertexArrays(1, &m_gl_VAO);
    glDeleteBuffers(1, &m_gl_StartHeightBuffer);
    glDeleteBuffers(1, &m_gl_EndHeightBuffer);
    glDeleteBuffers(1, &m_gl_EBO);
}

void waterGrid::InitGL()
{
    glGenVertexArrays(1, &m_gl_VAO);
    glGenBuffers(1, &m_gl_StartHeightBuffer);
    glGenBuffers(1, &m_gl_EndHeightBuffer);
    glGenBuffers(1, &m_gl_EBO);

    PopulateBuffers();
}

void waterGrid::Draw()
{   
    glBindVertexArray(m_gl_VAO);
    glDrawElements(GL_TRIANGLES, m_gl_triangleCount * 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void waterGrid::PopulateBuffers()
{
    // water lies on XZ plane 
    // *~*~*~*~*~*~*~*~*~*~*~*~*

    std::vector<glm::vec3> points;
    points.reserve(N * N);

    float a_half = a / 2.0f;
    float PosX, PoxZ;
    for (int x = 0; x < N; x++) {
        for (int z = 0; z < N; z++) {
            PosX = -a_half + h * x;
            PoxZ = -a_half + h * z;
            points.push_back(glm::vec3(PosX, 0.0f, PoxZ));
        }
    }

    std::vector<glm::uvec3> indices;
    m_gl_triangleCount = (N - 1) * (N - 1) * 2;
    indices.reserve(m_gl_triangleCount);
   
    for (int x = 0; x < N - 1; x++) {
        for (int z = 0; z < N - 1; z++) {
            indices.push_back(glm::uvec3(x * N + z, x * N + (z + 1), (x + 1) * N + (z + 1)));
            indices.push_back(glm::uvec3((x + 1) * N + z, x * N + z, (x + 1) * N + (z + 1)));
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_gl_StartHeightBuffer);   
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_DYNAMIC_COPY);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_gl_EndHeightBuffer);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), nullptr, GL_DYNAMIC_COPY);

    glBindBuffer(GL_ARRAY_BUFFER, m_gl_EBO);
    glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec3), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(m_gl_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gl_StartHeightBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_EBO);
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}