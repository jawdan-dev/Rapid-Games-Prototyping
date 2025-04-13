#include "goblinEngine.application.hpp"
#include "goblinEngine.core.hpp"

/**
 * PROBABLE BUG(?) FOUND.
 *
 * Functions 'glSpriteScaleXY' & 'glSpriteOnQuad' do not
 * draw the final row of pixels.
 *
 */

int main(void) {
	Application app;
	app.start();
}