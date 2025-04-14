#pragma once

#ifdef __INTELLISENSE__
#	define __NDS__
#	define ARM9
#	define __MISC_VISIBLE true
#	define iprintf printf
#	define viprintf vprintf
#endif

#include <gl2d.h>
#include <nds.h>
#include <stdio.h>

#include <set>
#include <vector>
#include <queue>

#include "goblinEngine.math.vector2.hpp"

void CORE_INITIALIZE();
void CORE_ERROR(const char* msg...);

#define MIN(a, b) ((b < a) ? (b) : (a))
#define MAX(a, b) ((b > a) ? (b) : (a))