#pragma once

#include "goblinEngine.assets.hpp"
#include "goblinEngine.game.object.hpp"

class DebugObject : public GameObject {
public:
	DebugObject() :
		GameObject("Debug"),
		m_touchPosition{.px = 0, .py = 0} {}

	void update() override {
		int heldKeys = keysHeld();
		if (heldKeys & KEY_TOUCH)
			touchRead(&m_touchPosition);
	}

	void drawTop() override {
		spr_map.draw(0, 0);
		spr_test.draw(m_touchPosition.px, m_touchPosition.py);

		glLine(m_touchPosition.px, 0, m_touchPosition.px, m_touchPosition.py, RGB15(31, 31, 31));
		glLine(0, m_touchPosition.py, m_touchPosition.px, m_touchPosition.py, RGB15(31, 31, 31));

		for (int i = 0; i < 9; i++) {
			spr_goblins.draw(i * 16, 100, i);
		}
	}

	void drawBottom() override {
		static const char* testStr = "thislonglong is a test. ig this text needs to be much longer, and thats fine.";
		spr_panel.drawPanel(0, 0, m_touchPosition.px, m_touchPosition.py);
		spr_6x6.drawTextBound(testStr, 0 + spr_panel.getSpriteWidth(), 0 + spr_panel.getSpriteHeight(), m_touchPosition.px - (2 * spr_panel.getSpriteWidth()), m_touchPosition.py - (2 * spr_panel.getSpriteHeight()));
	}

private:
	touchPosition m_touchPosition;
};