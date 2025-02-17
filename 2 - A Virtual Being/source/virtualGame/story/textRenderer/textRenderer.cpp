#include "textRenderer.hpp"

#include <virtualGame/embeds/embeds.hpp>

TextRenderer::TextRenderer() :
	m_shader("shaders/text"), m_uniforms(),
	m_mesh(), m_instance(),
	m_font(0) {
	glGenTextures(1, &m_font);
	Embeds::loadIntoTexture(Embeds::s_Font, m_font);

	const float textSize = 0.5f;
	const float textDepth = 0.1f;
	std::vector<Vector3> textVertices = {
		Vector3(-textSize, -textSize, textDepth),
		Vector3(textSize, -textSize, textDepth),
		Vector3(-textSize, textSize, textDepth),
		Vector3(textSize, textSize, textDepth),
	};
	const float uvOffset = 0.125f;
	std::vector<Vector2> textUVs = {
		Vector2(0 + uvOffset, 0 + uvOffset),
		Vector2(1 - uvOffset, 0 + uvOffset),
		Vector2(0 + uvOffset, 1 - uvOffset),
		Vector2(1 - uvOffset, 1 - uvOffset),
	};
	std::vector<uint16_t> textIndices = {
		0,
		1,
		2,
		2,
		1,
		3,
	};
	m_mesh.setData("v_pos", textVertices, GL_FLOAT_VEC3);
	m_mesh.setData("v_uv", textUVs, GL_FLOAT_VEC2);
	m_mesh.setIndices(textIndices);

	m_uniforms.setData("u_font", m_font, GL_SAMPLER_2D);
	m_uniforms.setData("u_characterCount", 96.0f, GL_FLOAT);
}
TextRenderer::~TextRenderer() {
	if (m_font) {
		glDeleteTextures(1, &m_font);
		m_font = 0;
	}
}

void TextRenderer::drawCharacter(Renderer& renderer, const Vector3 pos, const float scale, const char character, const Vector3& color) {
	m_instance.setData("i_index", (float)(character - ' '), GL_FLOAT);
	m_instance.setData("i_pos", pos, GL_FLOAT_VEC3);
	m_instance.setData("i_scale", scale, GL_FLOAT);
	m_instance.setData("i_color", color, GL_FLOAT_VEC3);
	renderer.draw(m_shader, m_uniforms, m_mesh, m_instance);
}
void TextRenderer::drawText(Renderer& renderer, const Vector3 pos, const float scale, const String& string, const Vector3& color) {
	Vector3 spacing(1.2f * scale, 0, 0);
	for (int i = 0; i < string.size(); i++) {
		drawCharacter(renderer, pos + (spacing * i), scale, string[i], color);
	}
}