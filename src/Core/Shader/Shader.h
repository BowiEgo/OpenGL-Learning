#pragma once

#include "pch.h"

#include "glm/glm.hpp"

#include "UniformBuffer.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

enum class ShaderType
{
    VertexShader = 0, GeometryShader, FragmentShader, 
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
    glm::mat4,
    std::vector<glm::mat4>
>;

class Shader
{
public:
    Shader(const std::string& filepath);
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    Shader(const std::string& vertexSrc, const std::string& geometrySrc, const std::string& fragmentSrc);
    ~Shader();

    void Setup() const;

    void Bind() const;
    void Unbind() const;

    static void UpdateMatricesProj(const glm::mat4& projection);
    static void UpdateMatricesView(const glm::mat4& view);

    inline uint32_t GetID() const { return m_RendererID; };
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
    void SetUniformMat4V(const std::string& name, const std::vector<glm::mat4>& array);
    void SetUniform(const std::string& name, const UniformValue& value);
private:
    static void InitializeUniformBuffer();
    int GetUniformLoaction(const std::string& name) const;
    ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CompileShader(const ShaderType& type, const std::string& source);
    unsigned int CreateShader(const std::string& vertextShader, const std::string& fragmentShader);
    unsigned int CreateShader(const std::string& vertextShader,
        const std::string& geometryShader,
        const std::string& fragmentShader);
private:
    static Ref<UniformBuffer> s_UBO_Matrices;
private:
    std::string m_FilePath;
    std::string m_Name;
    uint32_t m_RendererID;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};
