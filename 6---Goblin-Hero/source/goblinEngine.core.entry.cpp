#include "goblinEngine.application.hpp"
#include "goblinEngine.core.hpp"

#include <time.h>

/**
 * PROBABLE BUG(?) FOUND.
 *
 * Functions 'glSpriteScaleXY' & 'glSpriteOnQuad' do not
 * draw the final row of pixels.
 *
 */

int main(void) {
	srand(time(nullptr));
	Application app;
	app.start();
}