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

#include <beingEngine/math/vector2.hpp>
#include <beingEngine/math/vector3.hpp>
#include <beingEngine/systems/file.hpp>
