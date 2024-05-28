#include "duck.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <iostream>
#include <fstream>
#include <vector>

duck::duck()
{}

duck::~duck()
{}

void duck::InitGLFromFile(std::string duckFile)
{
    glGenVertexArrays(1, &m_gl_VAO);
    glGenBuffers(1, &m_gl_VBO);
    glGenBuffers(1, &m_gl_EBO);

    std::ifstream fs(duckFile);
    if (!fs.is_open()) {
        std::cout << "Duck GL Init failed because file stream was not opened"
        << "\n\t[file]: " << duckFile << std::endl;
        return;
    }

    unsigned int V;
    fs >> V;
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::uvec2> textures;
    vertices.reserve(V);
    normals.reserve(V);
    textures.reserve(V);

    for (int i = 0; i < V; i++) {
        
    }
}

void duck::DeInitGL()
{
    glDeleteVertexArrays(1, &m_gl_VAO);
    glDeleteBuffers(1, &m_gl_VBO);
    glDeleteBuffers(1, &m_gl_EBO);
}

void duck::Draw()
{

}
