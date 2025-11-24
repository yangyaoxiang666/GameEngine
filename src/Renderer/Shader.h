#pragma once
#include <string>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader() = default;

    void Use() const;

private:
    unsigned int ID;
};
