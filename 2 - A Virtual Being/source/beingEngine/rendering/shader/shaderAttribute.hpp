#pragma once
#include <beingEngine/core/core.hpp>

struct ShaderAttribute {
	GLint m_location;
	GLenum m_glType;
	size_t m_dataSize, m_dataOffset;
	bool m_isStatic;
};
