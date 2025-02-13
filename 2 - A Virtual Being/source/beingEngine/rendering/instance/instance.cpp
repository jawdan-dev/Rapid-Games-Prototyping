#include "instance.hpp"

Instance::Instance() :
	m_data() {}
Instance::~Instance() {
	for (auto it : m_data)
		free(it.second.m_data);
	m_data.clear();
}

void Instance::setDataRaw(const String& attributeName, const void* data, const GLenum glType) {
	const size_t dataSize = glGetTypeByteSize(glType);

	const InstanceData instanceData = {
		.m_glType = glType,
		.m_data = malloc(dataSize),
	};
	memcpy(instanceData.m_data, data, dataSize);

	auto it = m_data.find(attributeName);
	if (it != m_data.end()) {
		free((*it).second.m_data);
		m_data.erase(it);
	}
	m_data.emplace(attributeName, instanceData);
}
