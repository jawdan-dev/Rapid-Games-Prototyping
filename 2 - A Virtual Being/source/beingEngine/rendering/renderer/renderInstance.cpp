#include "renderInstance.hpp"

RenderInstance::RenderInstance(Shader& shader, Mesh& mesh) :
	m_modified(false),
	m_instanceData(nullptr), m_instanceCount(0), m_instanceCapacity(0),
	m_vao(0), m_ivbo(0),
	m_shader(&shader), m_mesh(&mesh) {
	glGenBuffers(1, &m_ivbo);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	mesh.upload();
	for (auto it : m_shader->getAttributes()) {
		const bool isStatic = it.second.m_isStatic;
		glBindBuffer(GL_ARRAY_BUFFER, isStatic ? mesh.getVBO() : m_ivbo);

		glVertexAttribPointer(
			it.second.m_location,
			glGetTypeElementCount(it.second.m_glType),
			glGetTypeBase(it.second.m_glType),
			GL_FALSE,
			isStatic ? mesh.getAttributeStride() : m_shader->getInstanceAttributesTotalSize(),
			(void*)(isStatic ? mesh.getAttributeOffset(it.first) : it.second.m_dataOffset));
		glVertexAttribDivisor(it.second.m_location, isStatic ? 0 : 1);
		glEnableVertexAttribArray(it.second.m_location);
	}
	glBindVertexArray(0);
}
RenderInstance::~RenderInstance() {
	if (m_vao) {
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
	if (m_ivbo) {
		glDeleteBuffers(1, &m_ivbo);
		m_ivbo = 0;
	}
}

void RenderInstance::addInstance(const Instance& instanceData) {
	if (m_shader->getInstanceAttributesTotalSize() <= 0)
		return;
	resize(m_instanceCount + 1);

	auto& attributes = m_shader->getAttributes();
	for (auto it : instanceData.getInstanceData()) {
		auto find = attributes.find(it.first);
		if (find == attributes.end() || find->second.m_isStatic || find->second.m_glType != it.second.m_glType)
			continue;

		const size_t dataSize = glGetTypeByteSize(it.second.m_glType);
		void* const dataTarget = (void*)((intptr_t)m_instanceData + (m_instanceCount * m_shader->getInstanceAttributesTotalSize()) + find->second.m_dataOffset);
		if (memcmp(dataTarget, it.second.m_data, dataSize) == 0)
			continue;

		memcpy(dataTarget, it.second.m_data, dataSize);
		m_modified = true;
	}

	m_instanceCount++;
}
void RenderInstance::draw() {
	if (m_modified) {
		glBindBuffer(GL_ARRAY_BUFFER, m_ivbo);
		glBufferData(GL_ARRAY_BUFFER, m_instanceCapacity * m_shader->getInstanceAttributesTotalSize(), m_instanceData, GL_DYNAMIC_DRAW);
		m_modified = false;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	const GLuint vbo = m_mesh->getVBO();
	if (!vbo)
		return;

	const GLuint ebo = m_mesh->getEBO();
	const size_t drawCount = m_mesh->getDrawCount();
	const bool instanced = m_shader->getInstanceAttributesTotalSize() > 0;

	glBindVertexArray(m_vao);
	glUseProgram(m_shader->getProgram());

	if (ebo) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		if (instanced) {
			glDrawElementsInstanced(GL_TRIANGLES, drawCount, GL_UNSIGNED_SHORT, nullptr, m_instanceCount);
		} else {
			glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_SHORT, nullptr);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	} else if (instanced) {
		glDrawArraysInstanced(GL_TRIANGLES, 0, drawCount, m_instanceCount);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, drawCount);
	}
}
void RenderInstance::clear() {
	m_instanceCount = 0;
}

void RenderInstance::resize(const size_t size) {
	if (m_instanceCapacity >= size)
		return;

	if (m_instanceCapacity <= 0)
		m_instanceCapacity = 4;
	while (m_instanceCapacity < size)
		m_instanceCapacity *= 2;

	void* temp = realloc(m_instanceData, m_instanceCapacity * m_shader->getInstanceAttributesTotalSize());
	if (temp == nullptr)
		BEING_ERROR("Failed to resize render instance.");

	m_instanceData = temp;
	m_modified = true;
}
