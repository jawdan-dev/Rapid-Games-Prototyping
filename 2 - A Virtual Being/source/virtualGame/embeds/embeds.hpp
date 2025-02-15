#pragma once
#include <beingEngine/core/core.hpp>

/**
 * Forgive me God for what I'm about to commit.
 * I would like forgiveness for the atrocities I will bring into this code base.
 * Amen.
 */

class Embeds {
public:
	struct EmbedColor {
		union {
			uint32_t raw;
			struct {
				uint8_t b, g, r, a;
			};
		};
	};

	typedef const uint64_t* Embed;

	static const int getWidth(const Embed embed);
	static const int getHeight(const Embed embed);
	static EmbedColor readPixel(const Embed embed, const int x, const int y);
	static void loadIntoTexture(const Embed embed, const GLuint texture);

	static const uint64_t s_InputMap[];
	static const uint64_t s_CRTDots[];
static const uint64_t s_CRTMap[];
};