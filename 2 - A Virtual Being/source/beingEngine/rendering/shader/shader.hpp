#pragma once
#include <beingEngine/core/core.hpp>

#include <beingEngine/rendering/shader/shaderAttribute.hpp>
#include <beingEngine/rendering/shader/shaderUniform.hpp>

class Shader {
public:
    Shader(const String& shaderPath);
    Shader(const String& vertexShaderSource, const String& fragmentShaderSource);
    Shader(const Shader& other) = delete;
    ~Shader();

    inline const size_t getInstanceAttributesTotalSize() const { return m_instanceAttributesTotalSize; }
    inline const size_t getStaticAttributesTotalSize() const { return m_staticAttributesTotalSize; }
    inline const GLuint getProgram() const { return m_program; }

    const bool hasAttribute(const String& attributeName) const { return m_attributes.find(attributeName) != m_attributes.end(); }
    const ShaderAttribute& getAttribute(const String& attributeName) const { return m_attributes.find(attributeName)->second; }
    const std::map<String, ShaderAttribute>& getAttributes() const { return m_attributes; }
    const bool hasUniform(const String& uniformName) const { return m_uniforms.find(uniformName) != m_uniforms.end(); }
    const ShaderUniform& getUniform(const String& uniformName) const { return m_uniforms.find(uniformName)->second; }
    const std::map<String, ShaderUniform>& getUniforms() const { return m_uniforms; }

private:
    const GLuint compileShader(const String& source, const GLenum shaderType);
    const GLuint compileProgram(const String& vertexSource, const String& fragmentSource);

    void loadAttributes();
    void loadUniforms();

private:
    GLuint m_program;
    std::map<String, ShaderAttribute> m_attributes;
    std::map<String, ShaderUniform> m_uniforms;
    size_t m_instanceAttributesTotalSize, m_staticAttributesTotalSize;
};