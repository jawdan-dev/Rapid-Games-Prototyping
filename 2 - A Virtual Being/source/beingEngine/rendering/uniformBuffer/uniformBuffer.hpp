#pragma once

#include <beingEngine/rendering/instance/instance.hpp>
#include <beingEngine/rendering/shader/shader.hpp>

class UniformBuffer : public Instance {
public:
	using Instance::Instance;

	void bindAll(Shader& shader, GLuint& samplerCounter) const;
};
