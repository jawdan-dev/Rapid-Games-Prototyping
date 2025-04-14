#include "goblinEngine.rendering.panel.hpp"

PanelSprite::PanelSprite(const unsigned int* const data,
						 const int width, const int height,
						 const bool transparent) :
	PanelSprite(data, nullptr, 0, width, height, transparent) {}
PanelSprite::PanelSprite(const unsigned int* const data,
						 const unsigned short* const palette, const int paletteSize,
						 const int width, const int height, const bool transparent) :
	Sprite(data, palette, paletteSize, width, height, width / 3, width / 3, transparent) {}

void PanelSprite::drawPanel(const int x, const int y, const int w, const int h, const int type) {
	const int
		imageOffset = type * 9,
		tileWidth = getSpriteWidth(),
		tileHeight = getSpriteHeight(),
		drawWidth = MAX(w, tileWidth * 2),
		drawHeight = MAX(h, tileHeight * 2),
		innerDrawWidth = MAX(drawWidth - (tileWidth * 2), 0),
		innerDrawHeight = MAX(drawHeight - (tileHeight * 2), 0);

	drawTo(x + tileWidth, y + tileHeight, innerDrawWidth, innerDrawHeight, 4 + imageOffset);

	drawTo(x + tileWidth, y, innerDrawWidth, tileHeight, 1 + imageOffset);
	drawTo(x, y + tileHeight, tileWidth, innerDrawHeight, 3 + imageOffset);
	drawTo(x + tileWidth, y + (drawHeight - tileHeight), innerDrawWidth, tileHeight, 7 + imageOffset);
	drawTo(x + (drawWidth - tileWidth), y + tileHeight, tileWidth, innerDrawHeight, 5 + imageOffset);

	draw(x, y, 0 + imageOffset);
	draw(x + (drawWidth - tileWidth), y, 2 + imageOffset);
	draw(x, y + (drawHeight - tileHeight), 6 + imageOffset);
	draw(x + (drawWidth - tileWidth), y + (drawHeight - tileHeight), 8 + imageOffset);
}