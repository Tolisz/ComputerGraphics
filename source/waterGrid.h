#pragma once

#include <glad/glad.h>

#include "shader.h"

class waterGrid
{
private: 

    int m_N;
    float m_a;
    float m_h;
    float m_c;
    float m_dt;

private: 

    GLuint m_gl_VAO;
    GLuint m_gl_StartHeightBuffer;
    GLuint m_gl_EndHeightBuffer;
    GLuint m_gl_EBO;

    GLsizei m_gl_triangleCount;

    shader m_sh_waterSimulation;

public:

    // *=*=*=*=*=*=*=*=*=*=
    //   Object Creation
    // *=*=*=*=*=*=*=*=*=*=

    waterGrid();
    waterGrid(int N, float a, float c);
    ~waterGrid();

    waterGrid(const waterGrid&) = delete;
    waterGrid(waterGrid&&) = delete;
    waterGrid& operator=(const waterGrid&) = delete;
    waterGrid& operator=(waterGrid&&) = delete;

    // *=*=*=*=*=*=*=*=*=*=
    //      Methods
    // *=*=*=*=*=*=*=*=*=*=

    void InitGL();
    
    void SimulateWater();
    void Draw();

private:

    void PopulateBuffers();
};

