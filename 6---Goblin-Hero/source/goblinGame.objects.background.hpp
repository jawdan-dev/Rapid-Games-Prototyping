#pragma once

#include "goblinEngine.assets.hpp"
#include "goblinEngine.game.object.hpp"

class BackgroundObject : public GameObject {
public:
	BackgroundObject() :
		GameObject("BackgroundObject") {}

	void drawTop() override {
		spr_map.draw(0, 0);
	}

	void drawBottom() override {
		spr_background.draw(0, 0, s_enabled ? 0 : 1);
	}

public:
	static bool s_enabled;
};

bool BackgroundObject::s_enabled = false;