#pragma once
#include <beingEngine/core/core.hpp>

#include <beingEngine/rendering/instance/instanceData.hpp>

class Instance {
public:
	Instance();
	Instance(const Instance& other) = delete;
	~Instance();

	const std::map<String, InstanceData>& getInstanceData() const { return m_data; };

	void setDataRaw(const String& attributeName, const void* data, const GLenum glType);
	template<typename T>
	void setData(const String& attributeName, const T& data, const GLenum glType) { setDataRaw(attributeName, &data, glType); }

private:
	std::map<String, InstanceData> m_data;
};