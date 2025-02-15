#pragma once
#include <beingEngine/rendering/processingStack/processingLayer.hpp>

class TestPostProcessing : public ProcessingLayer {
public:
	TestPostProcessing() :
		m_shader("postProcessing/test"), m_uniformBuffer(),
		m_mesh(), m_instance() {}

	void setup() override {
		std::vector<Vector3> vertices = {
			Vector3(-1, -1, 0),
			Vector3(-1, 1, 0),
			Vector3(1, -1, 0),
			Vector3(1, 1, 0),
			Vector3(1, -1, 0),
			Vector3(-1, 1, 0),
		};
		std::vector<Vector2> uvs = {
			Vector2(0, 0),
			Vector2(0, 1),
			Vector2(1, 0),
			Vector2(1, 1),
			Vector2(1, 0),
			Vector2(0, 1),
		};

		m_mesh.setData("v_pos", vertices, GL_FLOAT_VEC3);
		m_mesh.setData("v_uv", uvs, GL_FLOAT_VEC2);

		m_uniformBuffer.setData("u_base", getTexture(), GL_SAMPLER_2D);
	}

	void draw() override {
		getRenderer().draw(m_shader, m_uniformBuffer, m_mesh, m_instance);
	}

private:
	Shader m_shader;
	UniformBuffer m_uniformBuffer;
	Mesh m_mesh;
	Instance m_instance;
};