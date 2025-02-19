#pragma once

#ifdef __EMSCRIPTEN__
#	include <emscripten.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <beingEngine/core/core.hpp>

const size_t glGetTypeBase(const GLenum glType);
const size_t glGetTypeBaseByteSize(const GLenum glType);
const size_t glGetTypeElementCount(const GLenum glType);
const size_t glGetTypeVertexSplitCount(const GLenum glType);
const size_t glGetTypeByteSize(const GLenum glType);