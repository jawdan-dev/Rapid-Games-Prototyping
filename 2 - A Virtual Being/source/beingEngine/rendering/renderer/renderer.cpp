#include "renderer.hpp"

Renderer::Renderer() :
	m_renderInstances() {}
Renderer::~Renderer() {
	// CBA to cleanup data rn.
}

void Renderer::draw(Shader& shader, const UniformBuffer& uniformBuffer, Mesh& mesh, Instance& instance) {
	const std::tuple<Shader*, Mesh*, UniformBuffer> key = std::make_tuple(&shader, &mesh, uniformBuffer);

	auto it = m_renderInstances.find(key);
	if (it == m_renderInstances.end())
		it = m_renderInstances.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(shader, mesh)).first;

	it->second.addInstance(instance);
}
void Renderer::render(const Matrix4& viewProjection) {
	for (auto& it : m_renderInstances) {
		GLuint sampleCounter = 0;

		// This is scuffed.
		Shader& shader = *std::get<0>(it.first);
		const UniformBuffer& uniformBuffer = std::get<2>(it.first);

		glUseProgram(shader.getProgram());
		uniformBuffer.bindAll(shader, sampleCounter);
		if (shader.hasUniform("u_viewProjection")) {
			glUniformMatrix4fv(
				shader.getUniform("u_viewProjection").m_location,
				1, GL_FALSE, viewProjection.getData());
		}

		it.second.draw();
	}
	glBindVertexArray(0);
	glUseProgram(0);
}
void Renderer::clear() {
	auto it = m_renderInstances.begin();
	while (it != m_renderInstances.end()) {
		if (it->second.getInstanceCount() <= 0) {
			it = m_renderInstances.erase(it);
			continue;
		}

		it->second.clear();
		it++;
	}
}
