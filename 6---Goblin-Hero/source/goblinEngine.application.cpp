#include "goblinEngine.application.hpp"

#include "goblineGame.debug.hpp"

Application::Application() :
	m_sceneRoot("_root", nullptr) {}

void Application::start() {
	m_sceneRoot.setChild(new DebugObject);

	int frame = 0;
	while (pmMainLoop()) {
		scanKeys();
		frame++;

		// Process.
		m_sceneRoot._startAll();
		m_sceneRoot._updateAll();

		// Draw.
		while (REG_DISPCAPCNT & DCAP_ENABLE)
			;
		const bool drawnOnTop = (frame & 0b1) == 0;
		if (drawnOnTop) {
			lcdMainOnBottom();
			vramSetBankC(VRAM_C_LCD);
			vramSetBankD(VRAM_D_SUB_SPRITE);
			REG_DISPCAPCNT = DCAP_BANK(2) | DCAP_ENABLE | DCAP_SIZE(3);
		} else {
			lcdMainOnTop();
			vramSetBankD(VRAM_D_LCD);
			vramSetBankC(VRAM_C_SUB_BG);
			REG_DISPCAPCNT = DCAP_BANK(3) | DCAP_ENABLE | DCAP_SIZE(3);
		}
		glBegin2D();
		if (drawnOnTop) {
			m_sceneRoot._drawTopAll();
		} else {
			m_sceneRoot._drawBottomAll();
		}
		glEnd2D();

		// Sync.
		glFlush(0);
		swiWaitForVBlank();
		// consoleClear();

		// Debug helper.
		if (keysDown() & KEY_START)
			break;
	}
}