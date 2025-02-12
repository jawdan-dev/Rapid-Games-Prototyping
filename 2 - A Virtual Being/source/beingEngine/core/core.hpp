#pragma once

#include <exception>
#include <stdexcept>

#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

#ifndef __FILE_NAME__
#define __FILE_NAME__ __FILE__
#endif

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)
#define BEING_ERROR(msg) throw std::runtime_error(__FILE_NAME__ ":" STRINGIFY(__LINE__) ": " msg);

