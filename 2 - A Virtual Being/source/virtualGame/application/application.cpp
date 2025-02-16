#include "application.hpp"

#include <beingEngine/rendering/processingStack/processingStack.hpp>
#include <beingEngine/rendering/renderer/renderer.hpp>
#include <virtualGame/postProcessing/crtCurve.hpp>
#include <virtualGame/postProcessing/crtDots.hpp>
#include <virtualGame/postProcessing/test.hpp>
#include <virtualGame/story/interpreter/interpreter.hpp>
#include <virtualGame/story/textRenderer/textRenderer.hpp>

Application::Application() :
	m_window(640 * 2, 360 * 2) {}

void Application::start() {
	ProcessingStack processingStack(m_window.getWidth(), m_window.getHeight());
	// processingStack.emplaceLayer<CRTDotsPostProcessing>();
	processingStack.emplaceLayer<CRTCurveProcessing>(5.0f, screenRatio, 10);

	TextRenderer textRenderer;
	StoryInterpreter storyInterpreter;

	Shader shader("shader");

	const float meshDepth = 0;
	std::vector<Vector3> vertices = {
		Vector3(0, 0, meshDepth),
		Vector3(0, 1.0f, meshDepth),
		Vector3(1.0f, 0, meshDepth),
		Vector3(0, 0, meshDepth),
		Vector3(1.0f, 0, meshDepth),
		Vector3(0, 1.0f, meshDepth),
	};

	Mesh mesh;
	mesh.setData("v_pos", vertices, GL_FLOAT_VEC3);

	const float cursorScale = 0.05f;
	std::vector<Vector3> cursorVertices = {
		Vector3(cursorScale, cursorScale, 0),
		Vector3(-cursorScale, cursorScale, 0),
		Vector3(cursorScale, -cursorScale, 0),
		Vector3(cursorScale, -cursorScale, 0),
		Vector3(-cursorScale, cursorScale, 0),
		Vector3(-cursorScale, -cursorScale, 0),
	};
	Mesh cursor;
	cursor.setData("v_pos", cursorVertices, GL_FLOAT_VEC3);

	Renderer renderer;

	while (m_window.isOpen()) {
		processingStack.setSize(m_window.getWidth(), m_window.getHeight());
		m_window.process();

		Time::s_time = &m_window.getTime();
		Input::s_input = &m_window.getInput();
		{
			Vector2 inputFlip = Input::s_input->getMousePosition();
			inputFlip.y() = m_window.getHeight() - inputFlip.y();
			Input::s_input->setWeakMousePosition(inputFlip);
		}
		processingStack.process();

		static int counter = 0;
		if (counter++ > 200) {
			printf("time: %fms; fps: %f\n", m_window.getTime().getDelta(), 1.0f / m_window.getTime().getDelta());
			counter = 0;
		}

		Instance instance;
		UniformBuffer uniforms;

		const Vector2 mouseScaled = ((Input::s_input->getMousePosition() / Vector2(m_window.getWidth(), m_window.getHeight())) - 0.5f) * 2.0f;
		instance.setData("test", mouseScaled.x(), GL_FLOAT);
		instance.setData("test2", mouseScaled.y() * screenRatio, GL_FLOAT);
		renderer.draw(shader, uniforms, cursor, instance);

		// const float scale = 0.04f;
		// const float lineSpacing = -1.2f;
		// textRenderer.drawText(renderer, Vector2(-0.8f, 0 * lineSpacing * scale), scale, "[SYSTEM STATUS]");
		// textRenderer.drawText(renderer, Vector2(-0.8f, 1 * lineSpacing * scale), scale, "> OPERATING SYSTEM VERSION 8.72b");
		// textRenderer.drawText(renderer, Vector2(-0.8f, 3 * lineSpacing * scale), scale, "> Status.......... ONLINE");

		storyInterpreter.process();
		storyInterpreter.draw(renderer, textRenderer);

		const Matrix4 viewProjection =
			// Matrix4::perspective(90.0f * degToRad, (float)m_window.getWidth() / (float)m_window.getHeight(), 0.01f, 1000.0f) *
			// Matrix4::orthographic(-1, 1, screenRatio, -screenRatio, 0.1, 1000.0f) *
			Matrix4::orthographic(0, m_window.getWidth(), 0, m_window.getHeight(), -100.0f, 100.0f) *
			Matrix4::lookAt(Vector3(0, 0, 2), Vector3(0, 0, 0));

		processingStack.render(renderer, viewProjection);
		m_window.render();

		renderer.clear();
		processingStack.clear();
	}
}
