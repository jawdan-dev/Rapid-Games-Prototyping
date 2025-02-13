#include "renderer.hpp"

Renderer::Renderer() :
	m_renderInstances() {}
Renderer::~Renderer() {
	// CBA to cleanup data rn.
}

void Renderer::draw(Shader& shader, Mesh& mesh, Instance& instance) {
	const std::pair<Shader*, Mesh*> key = std::make_pair(&shader, &mesh);

	auto it = m_renderInstances.find(key);
	if (it == m_renderInstances.end())
		it = m_renderInstances.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(shader, mesh)).first;

	it->second.addInstance(instance);
}
void Renderer::render() {
	for (auto& it : m_renderInstances) {
		it.second.draw();
	}
	glBindVertexArray(0);
	glUseProgram(0);
}
void Renderer::clear() {
	for (auto& it : m_renderInstances) {
		it.second.clear();
	}
}
