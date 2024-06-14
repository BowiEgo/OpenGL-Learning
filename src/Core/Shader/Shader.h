#pragma once

#include "pch.h"

#include "glm/glm.hpp"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

using UniformValue = std::variant<
    bool,
    int,
    unsigned int,
    float,
    std::pair<float, float>,
    std::tuple<float, float, float>,
    std::tuple<float, float, float, float>,
    glm::vec2,
    glm::vec3,
    glm::vec4,
    glm::mat3,
    glm::mat4
>;

class Shader
{
public:
    Shader(const std::string& filepath);
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set uniforms
    void SetUniformBool(const std::string& name, bool value);
    void SetUniform1i(const std::string& name, int value);
    // void SetUniform1i(const std::string& name, unsigned int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform2f(const std::string& name, float v0, float v1);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformVec2(const std::string& name, const glm::vec2& vector);
    void SetUniformVec3(const std::string& name, const glm::vec3& vector);
    void SetUniformVec3(const std::string& name, const float vector[3]);
    void SetUniformVec4(const std::string& name, const glm::vec4& vector);
    void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
    void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
    void SetUniform(const std::string& name, const UniformValue& value);
private:
    int GetUniformLoaction(const std::string& name) const;
    ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertextShader, const std::string& fragmentShader);
private:
    std::string m_FilePath;
    std::string m_Name;
    unsigned int m_RendererID;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};
