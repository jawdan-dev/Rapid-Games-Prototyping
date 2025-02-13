#pragma once
#include <beingEngine/core/core.hpp>

#include <beingEngine/rendering/instance/instance.hpp>
#include <beingEngine/rendering/mesh/mesh.hpp>
#include <beingEngine/rendering/shader/shader.hpp>

#include <beingEngine/rendering/renderer/renderInstance.hpp>

class Renderer {
public:
	Renderer();
	Renderer(const Renderer& other) = delete;
	~Renderer();

	void draw(Shader& shader, Mesh& mesh, Instance& instance);
	void render();
	void clear();

private:
	std::map<std::pair<Shader*, Mesh*>, RenderInstance> m_renderInstances;
};