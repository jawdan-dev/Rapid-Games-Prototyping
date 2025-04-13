#include "goblinEngine.rendering.sprite.hpp"

Sprite::Sprite(const unsigned int* const data,
			   const int width, const int height,
			   const bool transparent) :
	Sprite(data, nullptr, 0, width, height, transparent) {}
Sprite::Sprite(const unsigned int* const data,
			   const unsigned short* const palette, const int paletteSize,
			   const int width, const int height,
			   const bool transparent) :
	Sprite(data, palette, paletteSize, width, height, width, height, transparent) {}
Sprite::Sprite(const unsigned int* const data,
			   const unsigned short* const palette, const int paletteSize,
			   const int width, const int height,
			   const int tileSizeX, const int tileSizeY,
			   const bool transparent) {

	CORE_INITIALIZE();

	if (width > 1024 || height > 1024)
		CORE_ERROR("Max texture size exceeded.");

	int compareWidth = 8;
	GL_TEXTURE_SIZE_ENUM widthEnum = TEXTURE_SIZE_8;
	while (compareWidth < width) {
		compareWidth *= 2;
		widthEnum = (GL_TEXTURE_SIZE_ENUM)(((int)widthEnum) + 1);
	}
	int compareHeight = 8;
	GL_TEXTURE_SIZE_ENUM heightEnum = TEXTURE_SIZE_8;
	while (compareHeight < height) {
		compareHeight *= 2;
		heightEnum = (GL_TEXTURE_SIZE_ENUM)(((int)heightEnum) + 1);
	}

	GL_TEXTURE_TYPE_ENUM textureType = GL_RGB;
	switch (paletteSize) {
		default:
			CORE_ERROR("Palette stuff not supported");
		case 0:
			break;
		case 16:
			textureType = GL_RGB16;
			break;
		case 256:
			textureType = GL_RGB256;
			break;
	}

	m_textureCount = (width / tileSizeX) * (height / tileSizeY);
	m_images = new glImage[m_textureCount];

	int textureParameters = GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF;
	if (transparent)
		textureParameters |= GL_TEXTURE_COLOR0_TRANSPARENT;

	glLoadTileSet(
		m_images,
		tileSizeX, tileSizeY,
		width, height,
		textureType,
		widthEnum, heightEnum,
		textureParameters,
		paletteSize, (u16*)palette,
		(u8*)data);
}
Sprite::~Sprite() {
	for (int i = 0; i < m_textureCount; i++)
		glDeleteTextures(1, &m_images[i].textureID);
	delete[] m_images;
}

const int Sprite::getSpriteWidth() {
	return m_textureCount > 0 ? m_images[0].width : 0;
}
const int Sprite::getSpriteHeight() {
	return m_textureCount > 0 ? m_images[0].height : 0;
}

void Sprite::draw(const int x, const int y, const int index) {
	glSprite(x, y, GL_FLIP_NONE, &m_images[index]);
}
void Sprite::drawExt(const int x, const int y, const int xScale, const int yScale, const int index) {
	if (xScale <= 0 || yScale <= 0)
		return;
	constexpr int scaleFactor = ((1 << 12) / 100);
	glSpriteScaleXY(x, y, xScale * scaleFactor, yScale * scaleFactor, GL_FLIP_NONE, &m_images[index]);
}
void Sprite::drawTo(const int x, const int y, const int width, const int height, const int index) {
	const int
		xScale = (width * (1 << 12)) / getSpriteWidth(),
		yScale = (height * (1 << 12)) / getSpriteHeight();

	glSpriteScaleXY(
		x, y,
		xScale, yScale,
		GL_FLIP_NONE, &m_images[index]);
}