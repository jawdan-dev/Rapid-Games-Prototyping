#pragma once

#include "goblinEngine.assets.hpp"
#include "goblinGame.objects.character.hpp"

class CharacterSorterObject : public GameObject {
public:
	CharacterSorterObject() :
		GameObject("CharacterSorterObject") {}

	void update() override {
		sortChildren([](GameObject* const a, GameObject* const b) {
			return ((CharacterObject*)a)->getPos().m_y < ((CharacterObject*)b)->getPos().m_y;
		}, false);
	}
};