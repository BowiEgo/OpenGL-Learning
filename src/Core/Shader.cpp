#include "Core/Shader.h"
#include "Core/Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
    m_RendererID = CreateShader(vertexSrc, fragmentSrc);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniformBool(const std::string &name, bool value)
{
    GLCall(glUniform1i(GetUniformLoaction(name), value ? 1 : 0));
}

void Shader::SetUniform1i(const std::string &name, int value)
{
    GLCall(glUniform1i(GetUniformLoaction(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLoaction(name), value));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
    GLCall(glUniform2f(GetUniformLoaction(name), v0, v1));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(GetUniformLoaction(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLoaction(name), v0, v1, v2, v3));
}

void Shader::SetUniformVec2(const std::string &name, const glm::vec2 &vector)
{
    GLCall(glUniform2f(GetUniformLoaction(name), vector.x, vector.y));
}

void Shader::SetUniformVec3(const std::string &name, const glm::vec3 &vector)
{
    GLCall(glUniform3f(GetUniformLoaction(name), vector.x, vector.y, vector.z));
}

void Shader::SetUniformVec3(const std::string &name, const float vector[3])
{
    GLCall(glUniform3f(GetUniformLoaction(name), vector[0], vector[1], vector[2]));
}

void Shader::SetUniformVec4(const std::string &name, const glm::vec4 &vector)
{
    GLCall(glUniform4f(GetUniformLoaction(name), vector.x, vector.y, vector.z, vector.w));
}

void Shader::SetUniformMat3(const std::string& name, const glm::mat3& matrix)
{
    GLCall(glUniformMatrix3fv(GetUniformLoaction(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLoaction(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLoaction(const std::string& name) const
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // Error handling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" <<
            std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertextShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertextShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}