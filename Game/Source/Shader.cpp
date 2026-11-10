#include "Shader.h"
#include "App.h"
#include "OpenGLDebug.h"
#include <utility>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    Set(vertexPath, fragmentPath);
}

Shader::~Shader()
{
    if (ID != 0)
        glDeleteProgram(ID);
}


Shader::Shader(Shader&& other) noexcept
    : ID(std::exchange(other.ID, 0)),
    valid(std::exchange(other.valid, false)),
    uniformLocations(std::move(other.uniformLocations))
{
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this == &other)
        return *this;

    if (ID != 0)
        glDeleteProgram(ID);

    ID = std::exchange(other.ID, 0);
    valid = std::exchange(other.valid, false);
    uniformLocations = std::move(other.uniformLocations);

    return *this;
}

void Shader::Set(const char* vertexPath, const char* fragmentPath)
{
    if (ID != 0)
    {
        glDeleteProgram(ID);
        ID = 0;
    }

    valid = false;
    uniformLocations.clear();

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        //std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        DEBUG_LOG("FILE_NOT_SUCCESFULLY_READ");
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    //2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckOpenGLError("Compiling vertex shader");

    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        //std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        DEBUG_LOG("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
        DEBUG_LOG("%s", infoLog);
    };

    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckOpenGLError("Compiling fragment shader");

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        DEBUG_LOG("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
        DEBUG_LOG("%s", infoLog);
    }

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    CheckOpenGLError("linking shaders to program");

    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        DEBUG_LOG("ERROR::SHADER::PROGRAM::LINKING_FAILED");
        DEBUG_LOG("%s", infoLog);
    }

    valid = success;

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use()
{
    glUseProgram(ID);
}
void Shader::Unuse()
{
    glUseProgram(0);
}

void Shader::SetInt(const std::string& name, int value) const
{
    const GLint location = GetUniformLocation(name);

    if (location != -1)
    {
        glUniform1i(location, value);

    }
    else DEBUG_LOG("Uniform not found or inactive: %s", name.c_str());

}

void Shader::SetBool(const std::string& name, bool value) const
{
    SetInt(name, value);
}



void Shader::SetFloat(const std::string& name, float value) const
{
    const GLint location = GetUniformLocation(name);

    if (location != -1)
    {
        glUniform1f(location, value);
    }
    else DEBUG_LOG("Uniform not found or inactive: %s", name.c_str());

}

void Shader::SetMat4x4(const std::string& name, const mat4x4& value) const
{
    const GLint location = GetUniformLocation(name);

    if (location != -1)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, &value);
    }
    else DEBUG_LOG("Uniform not found or inactive: %s", name.c_str());

}

void Shader::SetVec4(const std::string& name, const vec4& value) const
{
    const GLint location = GetUniformLocation(name);

    if (location != -1)
    {
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }
    else DEBUG_LOG("Uniform not found or inactive: %s", name.c_str());

}

void Shader::SetTexture(const std::string& name, int textureUnit) const
{
    SetInt(name, textureUnit);
}

bool Shader::IsValid() const
{
    return valid;
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
    const auto iterator = uniformLocations.find(name);

    if (iterator != uniformLocations.end())
    {
        return iterator->second;
    }

    const GLint location = glGetUniformLocation(ID, name.c_str());

    uniformLocations.emplace(name, location);

    return location;
}
