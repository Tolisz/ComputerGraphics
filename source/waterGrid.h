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
    GLuint m_gl_PreviousPosBuffer;
    GLuint m_gl_CurrentPosBuffer;
    GLuint m_gl_EBO;
    GLuint m_gl_DampingBuffer;
    GLuint m_gl_normalTex;

    GLsizei m_gl_triangleCount;

    shader m_sh_waterSimulation;
    shader m_sh_computeNormals;

private:

    bool m_bShouldDisturb;
    int m_iDisturb;
    int m_jDisturb;
    float m_disturbHeight;

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
    void DeInitGL();
    
    void SimulateWater();
    void DisturbWaterAt(glm::vec2 coords, float newHeight);
    void Draw();

    GLuint GetNormalTex();
    float GetA();
    
private:

    void PopulateBuffers();
    void PrepareShaders();
    void PrepareTextures();
};

