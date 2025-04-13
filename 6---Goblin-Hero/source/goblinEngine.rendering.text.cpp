#include "goblinEngine.rendering.text.hpp"

TextSprite::TextSprite(const unsigned int* const data,
					   const int width, const int height,
					   const int tileSizeX, const int tileSizeY) :
	TextSprite(data, nullptr, 0, width, height, tileSizeX, tileSizeY) {}
TextSprite::TextSprite(const unsigned int* const data,
					   const unsigned short* const palette, const int paletteSize,
					   const int width, const int height,
					   const int tileSizeX, const int tileSizeY) :
	Sprite(data, palette, paletteSize, width, height, tileSizeX, tileSizeY, true) {}

void TextSprite::drawText(const char* text, const int x, const int y) {
	const int
		len = strlen(text),
		spriteWidth = getSpriteWidth();
	for (int i = 0; i < len; i++)
		draw(x + (i * spriteWidth), y, text[i] - ' ');
}
void TextSprite::drawTextBound(const char* text, const int x, const int y, const int width, const int height, const int centered, const int spacing) {
	const int
		len = strlen(text),
		charSize = getSpriteWidth(),
		charDrawSize = charSize + spacing,
		maxWidth = (width + spacing) / charDrawSize,
		maxHeight = (height + spacing) / charDrawSize;

	if (maxWidth <= 0)
		return;

	int textIndex = 0, yOffset = 0;
	for (int n = 0; n < maxHeight; n++) {
		int lastSpace = -1;
		if (textIndex + maxWidth >= len) {
			lastSpace = len - textIndex;
		} else if (textIndex + maxWidth < len && text[textIndex + maxWidth] == ' ') {
			lastSpace = maxWidth;
		} else {
			for (int j = 0; j < maxWidth && textIndex + j < len; j++) {
				if (text[textIndex + j] != ' ')
					continue;
				lastSpace = j;
			}
		}

		const bool found = lastSpace != -1;
		if (!found)
			lastSpace = maxWidth;

		const int offsetX = (width - ((lastSpace * charDrawSize) - spacing)) / 2;

		if (!found)
			lastSpace--;

		for (int i = 0; i < lastSpace; i++)
			draw(offsetX + x + (i * charDrawSize), y + (yOffset * charDrawSize), text[textIndex + i] - ' ');

		textIndex += lastSpace;
		if (found) {
			textIndex++;
		} else {
			draw(offsetX + x + ((maxWidth - 1) * charDrawSize), y + (yOffset * charDrawSize), '-' - ' ');
		}

		if (textIndex >= len || ++yOffset > maxHeight)
			break;
	}
}