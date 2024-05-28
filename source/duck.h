#pragma once 

#include <glad/glad.h>
#include <string>

class duck 
{
private:
    GLuint m_gl_VAO;
    GLuint m_gl_VBO;
    GLuint m_gl_EBO;

    

public:
    // *=*=*=*=*=*=*=*=*=*=
    //   Object Creation
    // *=*=*=*=*=*=*=*=*=*=

    duck();
    ~duck();

    duck(const duck&) = delete;
    duck(duck&&) = delete;
    duck& operator=(const duck&) = delete;
    duck& operator=(duck&&) = delete;

    // *=*=*=*=*=*=*=*=*=*=
    //      Methods
    // *=*=*=*=*=*=*=*=*=*=

    void InitGLFromFile(std::string duckFile);
    void DeInitGL();

    void Draw();

};