# pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>

class Shader
{
public: 

    GLuint m_vertexID;
    GLuint m_tessellationControlID;
    GLuint m_tessellationEvaluationID;
    GLuint m_geometryID;
    GLuint m_fragmentID;

public:

    GLuint ID;

public:

    Shader();

    void VertexShader(const std::string& vertexShaderPath);
    void GeometryShader(const std::string& geometryShaderPath);
    void FragmentShader(const std::string& fragmentShaderPath);

    void LinkProgram();
    void Use();

    // *=*=*=*=*=*=*=*=*=*=*=*
    //    UNIFORM SETTERS
    // *=*=*=*=*=*=*=*=*=*=*=*

    void set1b(const char* name, GLboolean value);
    void set1f(const char* name, GLfloat v0);
    void set3f(const char* name, GLfloat v0, GLfloat v1, GLfloat v2);
    void set3fv(const char* name, const glm::vec3& value);
    void setM4fv(const char* name, GLboolean transpose, const glm::mat4& matrix);

private:

    std::string ReadShaderCode(const std::string& codeFilePath);

    void checkCompileErrors(GLuint shader, std::string type, std::string file);
};