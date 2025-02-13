#include "application.hpp"

#include <beingEngine/rendering/renderer/renderer.hpp>

Application::Application() :
	m_window(640, 360) {}

void Application::start() {
	FILE* vsFile = fopen("./shader.vs", "rb");
	FILE* fsFile = fopen("./shader.fs", "rb");

	fseek(vsFile, 0, SEEK_END);
	const long vsFileSize = ftell(vsFile);
	fseek(vsFile, 0, SEEK_SET);
	char* vsSource = new char[vsFileSize + 1];
	fread(vsSource, sizeof(char), vsFileSize, vsFile);
	vsSource[vsFileSize] = '\0';

	fseek(fsFile, 0, SEEK_END);
	const long fsFileSize = ftell(fsFile);
	fseek(fsFile, 0, SEEK_SET);
	char* fsSource = new char[fsFileSize + 1];
	fread(fsSource, sizeof(char), fsFileSize, fsFile);
	fsSource[fsFileSize] = '\0';

	fclose(vsFile);
	fclose(fsFile);

	Shader shader(vsSource, fsSource);

	delete[] vsSource;
	delete[] fsSource;

	//

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

	//
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	Vector3 vert[] = {
		Vector3(0, 0, 0),
		Vector3(0, 1.0f, 0),
		Vector3(1.0f, 0, 0),

		Vector3(0, 0, 0),
		Vector3(1.0f, 0, 0),
		Vector3(0, 1.0f, 0),
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glUseProgram(shader.getProgram());

	Renderer renderer;

	glViewport(0, 0, 640, 360);

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
		instance.setData("test", sinf(time.getNow()), GL_FLOAT);
		instance.setData("anything", Vector3(0.5, 0.1, 0.5), GL_FLOAT_VEC3);
		renderer.draw(shader, mesh, instance);

		instance.setData("test", -0.1f, GL_FLOAT);
		instance.setData("anything", Vector3(0.0, fabsf(cosf(time.getNow())) - 0.5f, 0.0), GL_FLOAT_VEC3);
		renderer.draw(shader, mesh, instance);

		renderer.render();
		m_window.draw();
		renderer.clear();
	}
}
