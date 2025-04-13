#pragma once

#include "goblinEngine.core.hpp"

class Sprite {
public:
	Sprite(const unsigned int* const data,
		   const int width, const int height,
		   const bool transparent = true);
	Sprite(const unsigned int* const data,
		   const unsigned short* const palette, const int paletteSize,
		   const int width, const int height,
		   const bool transparent = true);
	Sprite(const unsigned int* const data,
		   const unsigned short* const palette, const int paletteSize,
		   const int width, const int height,
		   const int tileSizeX, const int tileSizeY,
		   const bool transparent = true);
	~Sprite();

	const int getSpriteWidth();
	const int getSpriteHeight();

	void draw(const int x, const int y, const int index = 0);
	void drawExt(const int x, const int y, const int xScale, const int yScale, const int index = 0);
	void drawTo(const int x, const int y, const int width, const int height, const int index = 0);

private:
	glImage* m_images;
	int m_textureCount;
};