#include "instance.hpp"

Instance::Instance() :
	m_data() {}
Instance::Instance(const Instance& other) :
	m_data() {
	for (auto it : other.m_data) {
		const size_t dataSize = glGetTypeByteSize(it.second.m_glType);
		const InstanceData instanceData = {
			.m_glType = it.second.m_glType,
			.m_data = malloc(dataSize),
		};
		memcpy(instanceData.m_data, it.second.m_data, dataSize);
		m_data.emplace(it.first, instanceData);
	}
}
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

const bool Instance::operator<(const Instance& other) const {
	auto leftIt = m_data.begin();
	auto rightIt = other.m_data.begin();

	while (leftIt != m_data.end() && rightIt != other.m_data.end()) {
		if (leftIt->first != rightIt->first)
			return leftIt->first < rightIt->first;

		if (leftIt->second.m_glType != rightIt->second.m_glType)
			return leftIt->second.m_glType < rightIt->second.m_glType;

		const size_t dataSize = glGetTypeByteSize(leftIt->second.m_glType);
		const int comparison = memcmp(leftIt->second.m_data, rightIt->second.m_data, dataSize);
		if (comparison != 0)
			return comparison < 0;

		leftIt++;
		rightIt++;
	}

	if (leftIt == m_data.end())
		return rightIt != other.m_data.end();

	return false;
}
