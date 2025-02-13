#pragma once
#include <beingEngine/core/core.hpp>

#include <beingEngine/rendering/instance/instance.hpp>
#include <beingEngine/rendering/mesh/mesh.hpp>
#include <beingEngine/rendering/shader/shader.hpp>

class RenderInstance {
public:
	RenderInstance(Shader& shader, Mesh& mesh);
	RenderInstance(const RenderInstance& other) = delete;
	~RenderInstance();

	const size_t getInstanceCount() const { return m_instanceCount; }

	void addInstance(const Instance& instanceData);
	void draw();
	void clear();

private:
	void resize(const size_t size);

private:
	bool m_modified;

	void* m_instanceData;
	size_t m_instanceCount, m_instanceCapacity;
	GLuint m_vao, m_ivbo;

	Shader* m_shader;
	Mesh* m_mesh;
};