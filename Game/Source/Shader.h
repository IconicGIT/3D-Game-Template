#ifndef SHADER_H
#define SHADER_H

#include <gl/glew.h>
#include "glmath.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <string>

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(){}
    Shader(const char* vertexPath, const char* fragmentPath);
    void Set(const char* vertexPath, const char* fragmentPath);

    // use/activate the shader
    void Use();
    void Unuse();

    // utility uniform functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;

    void SetMat4x4(const std::string& name, const mat4x4& value) const;
    void SetVec4(const std::string& name, const vec4& value) const;

    void SetTexture(const std::string& name, int);
    bool IsValid() const;


private:

    bool valid = false;
    mutable std::unordered_map<std::string, GLint> uniformLocations;

    GLint GetUniformLocation(const std::string& name) const;

};

#endif