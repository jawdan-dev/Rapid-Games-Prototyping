#pragma once

#include "goblinEngine.assets.hpp"

static struct {
	touchPosition m_touchPosition;
	bool m_isTouching, m_wasTouching;
} GUI_STATE;

static void GUI_Label(const char* text, const int x, const int y, const int w, const int h, const bool enabled = true) {
	if (!enabled)
		return;
	spr_6x6.drawTextBound(text, x, y, w, h);
}
static void GUI_Panel(const int x, const int y, const int w, const int h, const bool enabled = true, const int panelType = 0) {
	spr_panel.drawPanel(x, y, w, h, enabled ? panelType : 3);
}
static void GUI_LabelPanel(const char* text, const int x, const int y, const int w, const int h, const bool enabled = true, const int panelType = 0) {
	constexpr int b = 8;
	GUI_Panel(x, y, w, h, enabled, panelType);
	GUI_Label(text, x + b, y + b, w - (b * 2), h - (b * 2), enabled);
}
static const bool GUI_Button(const char* text, const int x, const int y, const int w, const int h, const bool enabled = true, const bool selected = false) {
	const bool hovered =
		(GUI_STATE.m_touchPosition.px >= x) &&
		(GUI_STATE.m_touchPosition.px < (x + w)) &&
		(GUI_STATE.m_touchPosition.py >= y) &&
		(GUI_STATE.m_touchPosition.py < (y + h));
	const bool active = hovered && GUI_STATE.m_isTouching;

	int drawIndex = 0;
	if (selected) {
		drawIndex = 2;
	} else if (active) {
		drawIndex = 1;
	}

	GUI_LabelPanel(text, x, y, w, h, enabled, drawIndex);

	return enabled && hovered && !GUI_STATE.m_isTouching && GUI_STATE.m_wasTouching;
}