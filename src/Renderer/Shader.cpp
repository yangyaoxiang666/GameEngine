#include <glad/glad.h>
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // 1. 读取文件
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile(vertexPath);
    std::ifstream fShaderFile(fragmentPath);

    if (!vShaderFile.is_open()) {
        std::cerr << "[Shader] Failed to open vertex shader: " << vertexPath << std::endl;
    }
    if (!fShaderFile.is_open()) {
        std::cerr << "[Shader] Failed to open fragment shader: " << fragmentPath << std::endl;
    }

    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    const char* vCode = vertexCode.c_str();
    const char* fCode = fragmentCode.c_str();

    // 2. 编译 Shader
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

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fCode, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "[Shader] Fragment shader compile error:\n" << infoLog << std::endl;
    }

    // 3. 链接 Program
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

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() const {
    glUseProgram(ID);
}
