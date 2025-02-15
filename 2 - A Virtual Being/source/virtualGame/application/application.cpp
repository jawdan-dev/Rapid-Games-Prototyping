#include "application.hpp"

#include <beingEngine/rendering/processingStack/processingStack.hpp>
#include <beingEngine/rendering/renderer/renderer.hpp>
#include <virtualGame/postProcessing/crtCurve.hpp>
#include <virtualGame/postProcessing/test.hpp>

Application::Application() :
	m_window(640 * 2, 360 * 2) {}

void Application::start() {
	ProcessingStack processingStack(m_window.getWidth(), m_window.getHeight());
	processingStack.emplaceLayer<CRTCurveProcessing>(5.0f, 1080.0f / 1920.0f, 10);
	// processingStack.emplaceLayer<TestPostProcessing>();

	Shader shader("shader");

	const float meshDepth = -0.1f;
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

	Renderer renderer;

	while (m_window.isOpen()) {
		processingStack.setSize(m_window.getWidth(), m_window.getHeight());
		m_window.process();

		Time::s_time = &m_window.getTime();
		Input::s_input = &m_window.getInput();

		processingStack.process();

		if (Input::s_input->isKeyDown(GLFW_KEY_A))
			printf("down\n");
		if (Input::s_input->isKeyPressed(GLFW_KEY_A))
			printf("presssed\n");
		if (Input::s_input->isKeyUp(GLFW_KEY_A))
			printf("up\n");

		static int counter = 0;
		if (counter++ > 200) {
			printf("time: %fms; fps: %f\n", m_window.getTime().getDelta(), 1.0f / m_window.getTime().getDelta());
			counter = 0;
		}

		Instance instance;
		UniformBuffer uniforms;

		instance.setData("test", sinf(Time::s_time->getNow()) * 2.0f, GL_FLOAT);
		instance.setData("anything", Vector3(0.5, 0.1, 0.5), GL_FLOAT_VEC3);
		renderer.draw(shader, uniforms, mesh, instance);

		const float mouseScaled = ((Input::s_input->getMousePosition().x() / m_window.getWidth()) - 0.5f) * 2.0f;
		instance.setData("test", mouseScaled, GL_FLOAT);
		renderer.draw(shader, uniforms, mesh, instance);

		instance.setData("test", -0.1f, GL_FLOAT);
		instance.setData("anything", Vector3(0.0, fabsf(cosf(Time::s_time->getNow())) - 0.5f, 0.0), GL_FLOAT_VEC3);
		renderer.draw(shader, uniforms, mesh, instance);

		const Matrix4 viewProjection =
			// Matrix4::perspective(90.0f * degToRad, (float)m_window.getWidth() / (float)m_window.getHeight(), 0.01f, 1000.0f) *
			Matrix4::orthographic(-1, 1, 1, 0, 0.1, 1000.0f) *
			Matrix4::lookAt(Vector3(0, 0, 1), Vector3(0, 0, 0));

		processingStack.render(renderer, viewProjection);
		m_window.render();

		renderer.clear();
		processingStack.clear();
	}
}
