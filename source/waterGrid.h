#pragma once

#include <glad/glad.h>

class waterGrid
{
private: 

    int N;
    float a;
    float h;
    float c;
    float dt;

private: 

    GLuint m_gl_VAO;
    GLuint m_gl_StartHeightBuffer;
    GLuint m_gl_EndHeightBuffer;
    GLuint m_gl_EBO;

    GLsizei m_gl_triangleCount;

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
    void Draw();

private:

    void PopulateBuffers();

};

