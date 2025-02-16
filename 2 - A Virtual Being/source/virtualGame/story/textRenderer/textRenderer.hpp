#pragma once
#include <beingEngine/core/core.hpp>

#include <beingEngine/rendering/renderer/renderer.hpp>

class TextRenderer {
public:
	TextRenderer();
	TextRenderer(const TextRenderer& other) = delete;
	~TextRenderer();

	void drawCharacter(Renderer& renderer, const Vector3 pos, const float scale, const char character, const Vector3& color = Vector3(1, 1, 1));
	void drawText(Renderer& renderer, const Vector3 pos, const float scale, const String& string, const Vector3& color = Vector3(1, 1, 1));

private:
	Shader m_shader;
	Mesh m_mesh;
	Instance m_instance;
	UniformBuffer m_uniforms;
	GLuint m_font;
};