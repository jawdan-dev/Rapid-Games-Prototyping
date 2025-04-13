#pragma once

#include "goblinEngine.rendering.sprite.hpp"

class TextSprite : public Sprite {
public:
	TextSprite(const unsigned int* const data,
			   const int width, const int height,
			   const int tileSizeX, const int tileSizeY);
	TextSprite(const unsigned int* const data,
			   const unsigned short* const palette, const int paletteSize,
			   const int width, const int height,
			   const int tileSizeX, const int tileSizeY);

	void drawText(const char* text, const int x, const int y);
	void drawTextBound(const char* text, const int x, const int y, const int width, const int height, const int centered = true, const int spacing = 1);
};