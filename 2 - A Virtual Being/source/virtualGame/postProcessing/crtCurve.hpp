#pragma once
#include <beingEngine/rendering/processingStack/processingLayer.hpp>
#include <virtualGame/embeds/embeds.hpp>

class CRTCurveProcessing : public ProcessingLayer {
public:
	CRTCurveProcessing(const float radius, const float aspect, const int divisions) :
		m_shader("postProcessing/crtCurve"), m_uniformBuffer(),
		m_mesh(), m_instance(),
		m_radius(radius), m_aspect(aspect), m_divisions(divisions),
		m_zExtent() {}

	void setup() override {
		std::vector<Vector3> vertices;
		std::vector<Vector2> uvs;
		std::vector<uint16_t> indices;

		const float
			targetWidth = 1.0f,
			targetHeight = targetWidth * m_aspect;

		const float startAngle = acosf(targetWidth / m_radius);
		const float endAngle = PI - startAngle;

		const float increment = (endAngle - startAngle) / (float)m_divisions;
		const float zOffset = sin(startAngle) * m_radius;
		m_zExtent = (targetHeight + ((sin((startAngle + endAngle) * 0.5f) * m_radius) - zOffset));

		for (int i = 0; i <= m_divisions; i++) {
			const float angle = startAngle + (increment * (float)i);
			vertices.emplace_back(-cos(angle) * m_radius, targetHeight, (sin(angle) * m_radius) - zOffset);
			vertices.emplace_back(-cos(angle) * m_radius, -targetHeight, (sin(angle) * m_radius) - zOffset);
			uvs.emplace_back((angle - startAngle) / (endAngle - startAngle), 1.0f);
			uvs.emplace_back((angle - startAngle) / (endAngle - startAngle), 0.0f);
		}

		// 1 3 5
		// 2 4 6
		for (int counter = 0; counter < m_divisions * 2; counter += 2) {
			indices.emplace_back(counter);
			indices.emplace_back(counter + 2);
			indices.emplace_back(counter + 1);
			//
			indices.emplace_back(counter + 1);
			indices.emplace_back(counter + 2);
			indices.emplace_back(counter + 3);
		}

		m_mesh.setData("v_pos", vertices, GL_FLOAT_VEC3);
		m_mesh.setData("v_uv", uvs, GL_FLOAT_VEC2);
		m_mesh.setIndices(indices);

		m_uniformBuffer.setData("u_base", getTexture(), GL_SAMPLER_2D);
	}

	void process() override {
		const Matrix4 crtViewProjection =
			Matrix4::perspective(90.0f * degToRad, (float)getWidth() / (float)getHeight(), 0.001f, 100.0f) *
			Matrix4::lookAt(Vector3(0, 0, m_zExtent), Vector3(0, 0, 0));
		m_uniformBuffer.setData("u_crtViewProjection", crtViewProjection, GL_FLOAT_MAT4);

		Vector2 mousePos = Input::s_input->getMousePosition();
		// printf("%f, %f\n", mousePos.x(), mousePos.y());
		const Vector2 inputScalar(getWidth(), getHeight());
		mousePos /= inputScalar;


		const Embeds::Embed inputMap = Embeds::s_InputMap;
		const int xPos = mousePos.x() * (float)Embeds::getWidth(inputMap),
				  yPos = mousePos.y() * (float)Embeds::getHeight(inputMap);
		const Embeds::EmbedColor color = Embeds::readPixel(inputMap, xPos, yPos);

		if (color.a == 0 || color.b > 0) {
			mousePos = Vector2(0.0f, 0.0f);
		} else {
			mousePos = Vector2((float)color.r / 255.0f, (float)color.g / 255.0f) * inputScalar;
		}
		Input::s_input->setWeakMousePosition(mousePos);
	}
	void draw() override {
		getRenderer().draw(m_shader, m_uniformBuffer, m_mesh, m_instance);
	}

private:
	Shader m_shader;
	UniformBuffer m_uniformBuffer;
	Mesh m_mesh;
	Instance m_instance;

	float m_radius, m_aspect;
	int m_divisions;
	float m_zExtent;
};