#pragma once

#include "goblinEngine.core.hpp"
#include "goblinEngine.game.object.hpp"

#include "goblinEngine.assets.hpp"

class Application {
public:
	Application();

	void start();

private:
	GameObject m_sceneRoot;
};