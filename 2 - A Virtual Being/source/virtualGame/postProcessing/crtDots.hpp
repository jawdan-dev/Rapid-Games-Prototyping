#pragma once
#include <beingEngine/rendering/processingStack/processingLayer.hpp>

#include <virtualGame/embeds/embeds.hpp>

class CRTDotsPostProcessing : public ProcessingLayer {
public:
	CRTDotsPostProcessing() :
		m_shader("postProcessing/crtDots"), m_uniformBuffer(),
		m_mesh(), m_instance(),
		m_dotsTexture(0), m_dotsMapTexture(0) {
		glGenTextures(1, &m_dotsTexture);
		glGenTextures(1, &m_dotsMapTexture);
		Embeds::loadIntoTexture(Embeds::s_CRTDots, m_dotsTexture);
		Embeds::loadIntoTexture(Embeds::s_CRTMap, m_dotsMapTexture);
	}
	~CRTDotsPostProcessing() {
		if (m_dotsTexture) {
			glDeleteTextures(1, &m_dotsTexture);
			m_dotsTexture = 0;
		}
		if (m_dotsMapTexture) {
			glDeleteTextures(1, &m_dotsMapTexture);
			m_dotsMapTexture = 0;
		}
	}

	void setup() override {
		std::vector<Vector3> vertices = {
			Vector3(-1, -1, 0),
			Vector3(-1, 1, 0),
			Vector3(1, -1, 0),
			Vector3(1, 1, 0),
		};
		std::vector<Vector2> uvs = {
			Vector2(0, 0),
			Vector2(0, 1),
			Vector2(1, 0),
			Vector2(1, 1),
		};
		std::vector<uint16_t> indices = {
			0, 1, 2, 2, 1, 3};

		m_mesh.setData("v_pos", vertices, GL_FLOAT_VEC3);
		m_mesh.setData("v_uv", uvs, GL_FLOAT_VEC2);
		m_mesh.setIndices(indices);

		m_uniformBuffer.setData("u_base", getTexture(), GL_SAMPLER_2D);
		m_uniformBuffer.setData("u_dots", m_dotsTexture, GL_SAMPLER_2D);
		m_uniformBuffer.setData("u_dotMap", m_dotsMapTexture, GL_SAMPLER_2D);
		m_uniformBuffer.setData("u_dotsAspect", 1.0f, GL_FLOAT); //(float)Embeds::getHeight(Embeds::s_CRT) / (float)Embeds::getWidth(Embeds::s_CRT), GL_FLOAT);
		m_uniformBuffer.setData("u_dotsScale", 128.0f, GL_FLOAT);
	}

	void draw() override {
		getRenderer().draw(m_shader, m_uniformBuffer, m_mesh, m_instance);
	}

private:
	Shader m_shader;
	UniformBuffer m_uniformBuffer;
	Mesh m_mesh;
	Instance m_instance;

	GLuint m_dotsTexture, m_dotsMapTexture;
};