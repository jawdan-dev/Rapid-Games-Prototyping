#pragma once
#include <beingEngine/core/core.hpp>

#include <beingEngine/rendering/instance/instance.hpp>
#include <beingEngine/rendering/mesh/mesh.hpp>
#include <beingEngine/rendering/shader/shader.hpp>
#include <beingEngine/rendering/uniformBuffer/uniformBuffer.hpp>

#include <beingEngine/rendering/renderer/renderInstance.hpp>

class Renderer {
public:
	Renderer();
	Renderer(const Renderer& other) = delete;
	~Renderer();

	void draw(Shader& shader, const UniformBuffer& uniformBuffer, Mesh& mesh, Instance& instance);
	void render(const Matrix4& viewProjection);
	void clear();

private:
	std::map<std::tuple<Shader*, Mesh*, UniformBuffer>, RenderInstance> m_renderInstances;
};