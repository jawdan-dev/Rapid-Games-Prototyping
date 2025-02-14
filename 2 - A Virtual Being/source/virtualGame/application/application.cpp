#include "application.hpp"

#include <beingEngine/rendering/renderer/renderer.hpp>

Application::Application() :
	m_window(640, 360) {}

void Application::start() {
	Shader shader(File::readAll("assets/shader.vs"), File::readAll("assets/shader.fs"));

	std::vector<Vector3> vertices = {
		Vector3(0, 0, 0),
		Vector3(0, 1.0f, 0),
		Vector3(1.0f, 0, 0),

		Vector3(0, 0, 0),
		Vector3(1.0f, 0, 0),
		Vector3(0, 1.0f, 0),
	};

	Mesh mesh;
	mesh.setData("v_pos", vertices, GL_FLOAT_VEC3);

	Renderer renderer;

	while (m_window.isOpen()) {
		m_window.process();

		const Input& input = m_window.getInput();
		const Time& time = m_window.getTime();

		if (input.isKeyDown(GLFW_KEY_A))
			printf("down\n");
		if (input.isKeyPressed(GLFW_KEY_A))
			printf("presssed\n");
		if (input.isKeyUp(GLFW_KEY_A))
			printf("up\n");

		static int counter = 0;
		if (counter++ > 200) {
			printf("time: %fms; fps: %f\n", m_window.getTime().getDelta(), 1.0f / m_window.getTime().getDelta());
			counter = 0;
		}

		Instance instance;
		UniformBuffer uniforms;

		instance.setData("test", sinf(time.getNow()), GL_FLOAT);
		instance.setData("anything", Vector3(0.5, 0.1, 0.5), GL_FLOAT_VEC3);
		renderer.draw(shader, uniforms, mesh, instance);

		const float mouseScaled = ((input.getMousePosition().x() / 640.0f) - 0.5f) * 2.0f;
		instance.setData("test", mouseScaled, GL_FLOAT);
		renderer.draw(shader, uniforms, mesh, instance);

		instance.setData("test", -0.1f, GL_FLOAT);
		instance.setData("anything", Vector3(0.0, fabsf(cosf(time.getNow())) - 0.5f, 0.0), GL_FLOAT_VEC3);
		renderer.draw(shader, uniforms, mesh, instance);

		const Matrix4 viewProjection =
			Matrix4::perspective(90.0f / (3.14f / 180.0f), 1.0f, 0.01f, 1000.0f) *
			Matrix4::lookAt(Vector3(0, 0, 10 * sinf(time.getNow() + 0.2f)), Vector3(0, 0, 0));
		renderer.render(viewProjection);

		m_window.draw();
		renderer.clear();
	}
}
