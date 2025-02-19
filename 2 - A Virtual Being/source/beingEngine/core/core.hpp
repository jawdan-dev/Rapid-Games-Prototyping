#pragma once

#include <cstring>
#include <memory>
#include <cmath>
#include <cstdint>

#include <exception>
#include <stdexcept>

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <beingEngine/core/glfw.hpp>

typedef std::string String;

#ifndef __FILE_NAME__
#	define __FILE_NAME__ __FILE__
#endif

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)
#define BEING_ERROR(msg) throw std::runtime_error(__FILE_NAME__ ":" STRINGIFY(__LINE__) ": " msg);

#ifndef __max
#	define __max(a, b) (a < b ? b : a)
#endif
#ifndef __min
#	define __min(a, b) (a < b ? a : b)
#endif


constexpr float PI = 3.14159265358979323846;
constexpr float degToRad = PI / 180.0f;
constexpr float screenRatio = 1080.0f / 1920.0f;

#include <beingEngine/math/vector2.hpp>
#include <beingEngine/math/vector3.hpp>
#include <beingEngine/math/matrix4.hpp>
#include <beingEngine/systems/file.hpp>
