#include "Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

// ------------------------------------------------------------
// 构造函数：读取文件、编译 shader、链接 program
// ------------------------------------------------------------
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 读取 shader 文件
    std::ifstream vFile(vertexPath);
    std::ifstream fFile(fragmentPath);

    if (!vFile.is_open()) {
        std::cerr << "[Shader] Failed to open vertex shader: " << vertexPath << std::endl;
    }
    if (!fFile.is_open()) {
        std::cerr << "[Shader] Failed to open fragment shader: " << fragmentPath << std::endl;
    }

    std::stringstream vStream, fStream;
    vStream << vFile.rdbuf();
    fStream << fFile.rdbuf();

    std::string vertexCode = vStream.str();
    std::string fragmentCode = fStream.str();

    const char* vCode = vertexCode.c_str();
    const char* fCode = fragmentCode.c_str();


    // ------------------------------------------------------------
    // 编译顶点着色器
    // ------------------------------------------------------------
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vCode, nullptr);
    glCompileShader(vertex);

    int success;
    char infoLog[512];

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cerr << "[Shader] Vertex shader compile error:\n" << infoLog << std::endl;
    }


    // ------------------------------------------------------------
    // 编译片元着色器
    // ------------------------------------------------------------
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fCode, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "[Shader] Fragment shader compile error:\n" << infoLog << std::endl;
    }


    // ------------------------------------------------------------
    // 链接 Shader Program
    // ------------------------------------------------------------
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "[Shader] Program link error:\n" << infoLog << std::endl;
    } else {
        std::cout << "[Shader] Shader program created successfully." << std::endl;
    }

    // 链接后删除 shader 对象
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


// ------------------------------------------------------------
// 启用此 shader program
// ------------------------------------------------------------
void Shader::Use() const
{
    glUseProgram(ID);
}


// ------------------------------------------------------------
// 传 float uniform
// ------------------------------------------------------------
void Shader::SetFloat(const std::string& name, float value) const
{
    int location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "[Shader] Warning: uniform '" << name << "' not found. (location = -1)\n";
    }
    glUniform1f(location, value);
}


// ------------------------------------------------------------
// 传 int uniform（sampler2D 必须用 int）
// ------------------------------------------------------------
void Shader::SetInt(const std::string& name, int value) const
{
    int location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "[Shader] Warning: uniform '" << name << "' not found." << std::endl;
    }
    glUniform1i(location, value);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& matrix) const
{
    int location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "[Shader] Warning: uniform '" << name << "' not found.\n";
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

