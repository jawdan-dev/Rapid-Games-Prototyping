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

#include "inputMap.ipp"
