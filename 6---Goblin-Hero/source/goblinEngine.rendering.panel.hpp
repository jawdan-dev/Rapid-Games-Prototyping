#pragma once

#include "goblinEngine.rendering.sprite.hpp"

class PanelSprite : public Sprite {
public:
	PanelSprite(const unsigned int* const data,
				const int width, const int height,
				const bool transparent = true);
	PanelSprite(const unsigned int* const data,
				const unsigned short* const palette, const int paletteSize,
				const int width, const int height,
				const bool transparent = true);

	void drawPanel(const int x, const int y, const int w, const int h, const int type = 0);
};