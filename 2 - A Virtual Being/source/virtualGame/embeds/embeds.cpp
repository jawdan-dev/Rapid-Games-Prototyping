#include "embeds.hpp"

const int Embeds::getWidth(const Embed embed) {
	return embed[0];
}
const int Embeds::getHeight(const Embed embed) {
	return embed[1];
}

Embeds::EmbedColor Embeds::readPixel(const Embed embed, const int x, const int y) {
	const int width = getWidth(embed),
			  height = getHeight(embed);

	EmbedColor color;
	color.raw = 0;

	if (x < 0 || x >= width || y < 0 || y >= height)
		return color;

	color.raw = ((uint32_t*)(embed + 2))[x + (y * width)];
	return color;
}
void Embeds::loadIntoTexture(const Embed embed, const GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, getWidth(embed), getHeight(embed), 0, GL_RGBA, GL_UNSIGNED_BYTE, embed + 2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
}

#include "crtDots.ipp"
#include "crtMap.ipp"
#include "inputMap.ipp"
