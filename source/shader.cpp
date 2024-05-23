#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(): m_vertexID{0}, m_tessellationControlID{0}, m_tessellationEvaluationID{0}, m_fragmentID{0}, ID{0} { }

std::string Shader::ReadShaderCode(const std::string& codeFilePath)
{
    std::string code;
    std::ifstream shaderFile;
    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        shaderFile.open(codeFilePath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    return code;
}

void Shader::VertexShader(const std::string& vertexShaderPath)
{
    m_vertexID = glCreateShader(GL_VERTEX_SHADER);

    std::string vertexCode = ReadShaderCode(vertexShaderPath);
    const char* vShaderCode = vertexCode.c_str();

    glShaderSource(m_vertexID, 1, &vShaderCode, NULL);
    glCompileShader(m_vertexID);
    checkCompileErrors(m_vertexID, "VERTEX", vertexShaderPath);
}

void Shader::GeometryShader(const std::string& geometryShaderPath)
{
    m_geometryID = glCreateShader(GL_GEOMETRY_SHADER);

    std::string geometryCode = ReadShaderCode(geometryShaderPath);
    const char* gShaderCode = geometryCode.c_str();

    glShaderSource(m_geometryID, 1, &gShaderCode, NULL);
    glCompileShader(m_geometryID);
    checkCompileErrors(m_geometryID, "GEOMETRY", geometryShaderPath);    
}

void Shader::FragmentShader(const std::string& fragmentShaderPath)
{
    m_fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

    std::string fragmentCode = ReadShaderCode(fragmentShaderPath);
    const char* fShaderCode = fragmentCode.c_str();

    glShaderSource(m_fragmentID, 1, &fShaderCode, NULL);
    glCompileShader(m_fragmentID);
    checkCompileErrors(m_fragmentID, "FRAGMENT", fragmentShaderPath);
}

void Shader::LinkProgram()
{
    ID = glCreateProgram();
    glAttachShader(ID, m_vertexID);
    glAttachShader(ID, m_geometryID);
    glAttachShader(ID, m_fragmentID);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM", "null");
    
    glDeleteShader(m_vertexID);
    glDeleteShader(m_fragmentID);
}

void Shader::Use()
{
    glUseProgram(ID);
}

    // *=*=*=*=*=*=*=*=*=*=*=*
    //    UNIFORM SETTERS
    // *=*=*=*=*=*=*=*=*=*=*=*

void Shader::set1b(const char* name, GLboolean value)
{
    glUniform1i(glGetUniformLocation(ID, name), (int)value); 
}

void Shader::set1f(const char* name, GLfloat v0)
{
    glUniform1f(glGetUniformLocation(ID, name), v0);
}

void Shader::set3f(const char* name, GLfloat v0, GLfloat v1, GLfloat v2)
{
    glUniform3f(glGetUniformLocation(ID, name), v0, v1, v2);
}

void Shader::set3fv(const char* name, const glm::vec3& value)
{
    glUniform3fv(glGetUniformLocation(ID, name), 1, (float*)&value);
}

void Shader::setM4fv(const char* name, GLboolean transpose, const glm::mat4& matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, transpose, (float*)&matrix);
}

void Shader::checkCompileErrors(GLuint shader, std::string type, std::string file)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << " - " << file << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << " - " << file << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}