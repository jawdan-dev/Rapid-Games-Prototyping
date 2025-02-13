#include "mesh.hpp"

Mesh::Mesh() :
	m_modified(false),
	m_vbo(0), m_ebo(0), m_drawCount(0), m_attributeStride(0),
	m_data(), m_indices() {}
Mesh::~Mesh() {
	for (auto it : m_data)
		free(it.second.m_data);
	m_data.clear();
}

const size_t Mesh::getAttributeOffset(const String& attributeName) const {
	size_t offset = 0;
	for (auto it : m_data) {
		const size_t dataSize = glGetTypeByteSize(it.second.m_glType);
		if (it.first == attributeName)
			return offset;
		offset += dataSize;
	}
	return 0;
}

void Mesh::setDataRaw(const String& attributeName, const void* data, const size_t dataCount, const GLenum glType) {
	const size_t dataSize = dataCount * glGetTypeByteSize(glType);

	const MeshStaticData staticData = {
		.m_glType = glType,
		.m_data = malloc(dataSize),
		.m_dataCount = dataCount,
	};
	memcpy(staticData.m_data, data, dataSize);

	auto it = m_data.find(attributeName);
	if (it != m_data.end()) {
		free((*it).second.m_data);
		m_data.erase(it);
	}
	m_data.emplace(attributeName, staticData);

	m_modified = true;
}
void Mesh::setIndices(const std::vector<uint16_t>& indices) {
	m_indices = indices;
	m_modified = true;
}

void Mesh::upload() {
	if (!m_modified)
		return;
	m_modified = false;

	m_drawCount = 0;

	if (m_data.size() > 0) {
		if (!m_vbo)
			glGenBuffers(1, &m_vbo);

		m_attributeStride = 0;
		for (auto it : m_data) {
			m_attributeStride += glGetTypeByteSize(it.second.m_glType);
			m_drawCount = __max(m_drawCount, it.second.m_dataCount);
		}

		void* data = malloc(m_drawCount * m_attributeStride);
		for (auto it : m_data) {
			const size_t dataSize = glGetTypeByteSize(it.second.m_glType);
			const size_t offset = getAttributeOffset(it.first);

			for (size_t i = 0; i < it.second.m_dataCount && i < m_drawCount; i++)
				memcpy(
					(void*)((intptr_t)data + ((i * m_attributeStride) + offset)),
					(void*)((intptr_t)it.second.m_data + (i * dataSize)),
					dataSize);
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_drawCount * m_attributeStride, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	} else if (m_vbo) {
		glDeleteBuffers(1, &m_vbo);
		m_vbo = 0;
	}

	if (m_indices.size() > 0) {
		if (!m_ebo)
			glGenBuffers(1, &m_ebo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint16_t), m_indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_drawCount = m_indices.size();
	} else if (m_ebo) {
		glDeleteBuffers(1, &m_ebo);
		m_ebo = 0;
	}
}