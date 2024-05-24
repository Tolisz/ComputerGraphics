#include "waterGrid.h"

#include <vector>
#include <glm/vec3.hpp>

waterGrid::waterGrid()
    : waterGrid(256, 2.0f, 1.0f)
{ }

waterGrid::waterGrid(int N, float a, float c)
    : m_N{N}, m_a{a}, m_c{c}
{
    m_h = a / (N - 1);
    m_dt = 1.0f / N;
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

    m_sh_waterSimulation.Init();
    m_sh_waterSimulation.AttachShader("shaders/waterSimulation.comp", GL_COMPUTE_SHADER);
    m_sh_waterSimulation.Link();

}

void waterGrid::Draw()
{
    glBindVertexArray(m_gl_VAO);
    glDrawElements(GL_TRIANGLES, m_gl_triangleCount * 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void waterGrid::SimulateWater()
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_gl_StartHeightBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_gl_EndHeightBuffer);

    m_sh_waterSimulation.Use();
    m_sh_waterSimulation.set1ui("N", m_N);
    glDispatchCompute(m_N, m_N, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void waterGrid::PopulateBuffers()
{
    // water lies on XZ plane
    // *~*~*~*~*~*~*~*~*~*~*~*~*

    std::vector<glm::vec4> points;
    points.reserve(m_N * m_N);

    float a_half = m_a / 2.0f;
    float PosX, PoxZ;
    for (int x = 0; x < m_N; x++) {
        for (int z = 0; z < m_N; z++) {
            PosX = -a_half + m_h * x;
            PoxZ = -a_half + m_h * z;
            points.push_back(glm::vec4(PosX, 0.0f, PoxZ, 1.0f));
        }
    }

    std::vector<glm::uvec3> indices;
    m_gl_triangleCount = (m_N - 1) * (m_N - 1) * 2;
    indices.reserve(m_gl_triangleCount);

    for (int x = 0; x < m_N - 1; x++) {
        for (int z = 0; z < m_N - 1; z++) {
            indices.push_back(glm::uvec3(x * m_N + z, x * m_N + (z + 1), (x + 1) * m_N + (z + 1)));
            indices.push_back(glm::uvec3((x + 1) * m_N + z, x * m_N + z, (x + 1) * m_N + (z + 1)));
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_gl_StartHeightBuffer);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec4), points.data(), GL_DYNAMIC_COPY);

    glBindBuffer(GL_ARRAY_BUFFER, m_gl_EndHeightBuffer);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec4), nullptr, GL_DYNAMIC_COPY);

    glBindBuffer(GL_ARRAY_BUFFER, m_gl_EBO);
    glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec3), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(m_gl_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gl_StartHeightBuffer);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_EBO);
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
