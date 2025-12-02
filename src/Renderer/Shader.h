#pragma once
#include <string>
#include <glm/mat4x4.hpp>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader() = default;

    void Use() const;

    void SetFloat(const std::string& name, float value) const;
    void SetInt(const std::string& name, int value) const;

    void SetMat4(const std::string& name, const glm::mat4& matrix) const;

private:
    unsigned int ID;
};
