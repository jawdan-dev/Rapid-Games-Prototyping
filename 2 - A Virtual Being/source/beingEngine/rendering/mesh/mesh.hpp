#pragma once
#include <beingEngine/core/core.hpp>

#include <beingEngine/rendering/mesh/meshStaticData.hpp>

class Mesh {
public:
	Mesh();
	Mesh(const Mesh& other) = delete;
	~Mesh();

	inline const GLuint getVBO() const { return m_vbo; }
	inline const GLuint getEBO() const { return m_ebo; }
	inline const GLuint getDrawCount() const { return m_drawCount; }
	inline const std::map<String, MeshStaticData>& getData() const { return m_data; }
	const size_t getAttributeOffset(const String& attributeName) const;
	inline const size_t getAttributeStride() const { return m_attributeStride; };

	void setDataRaw(const String& attributeName, const void* data, const size_t dataCount, const GLenum glType);
	template<typename T>
	void setData(const String& attributeName, const std::vector<T>& data, const GLenum glType) { setDataRaw(attributeName, data.data(), data.size(), glType); }
	void setIndices(const std::vector<uint16_t>& indices);

	void upload();

private:
	bool m_modified;
	GLuint m_vbo, m_ebo;
	size_t m_drawCount, m_attributeStride;

	std::map<String, MeshStaticData> m_data;
	std::vector<uint16_t> m_indices;
};