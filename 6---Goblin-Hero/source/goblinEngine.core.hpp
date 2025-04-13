#pragma once

#ifdef __INTELLISENSE__
#	define __NDS__
#	define ARM9
#	define __MISC_VISIBLE true
#	define iprintf printf
#endif

#include <vector>

#include <gl2d.h>
#include <nds.h>
#include <stdio.h>

void CORE_INITIALIZE();
void CORE_ERROR(const char* msg);

#define MIN(a, b) ((b < a) ? (b) : (a))
#define MAX(a, b) ((b > a) ? (b) : (a))