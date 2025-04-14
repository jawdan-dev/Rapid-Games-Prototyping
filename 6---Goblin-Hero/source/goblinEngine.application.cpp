#include "goblinEngine.application.hpp"

#include "goblinEngine.gui.hpp"

#include "goblinGame.objects.attackParticles.hpp"
#include "goblinGame.objects.background.hpp"
#include "goblinGame.objects.character.hpp"
#include "goblinGame.objects.characterSorter.hpp"
#include "goblinGame.objects.debug.hpp"
#include "goblinGame.objects.gameManager.hpp"

Application::Application() :
	m_sceneRoot("_root", nullptr) {}

void Application::start() {
	m_sceneRoot.addChild(new BackgroundObject);
	//
	CharacterSorterObject* characterSorter = new CharacterSorterObject;
	for (int i = 0; i < 10; i++)
		characterSorter->addChild(new CharacterObject((CharacterObject::CharacterType)(i % (int)CharacterObject::CharacterType::COUNT)));
	characterSorter->addChild(new CharacterObject(CharacterObject::CharacterType::Goblin_Summoner, Vector2()));
	m_sceneRoot.addChild(characterSorter);
	//
	m_sceneRoot.addChild(new GameManager);
	m_sceneRoot.addChild(new AttackParticlesObject);

	int frame = 0;
	while (pmMainLoop()) {
		frame++;
		//
		scanKeys();
		//
		if (keysHeld() & KEY_TOUCH)
			touchRead(&GUI_STATE.m_touchPosition);
		GUI_STATE.m_wasTouching = GUI_STATE.m_isTouching;
		GUI_STATE.m_isTouching = keysHeld() & KEY_TOUCH;

		// Process.
		m_sceneRoot._startAll();
		m_sceneRoot._updateAll();
		CharacterObject::_randomMoveCharacters();
		CharacterObject::_separateMoveCharacters();
		CharacterObject::_handleAttacks();
		m_sceneRoot._freeAll();
		CharacterObject::_assignTargetCharacters();

		// Draw top.
		while (REG_DISPCAPCNT & DCAP_ENABLE)
			;
		lcdMainOnBottom();
		vramSetBankC(VRAM_C_LCD);
		vramSetBankD(VRAM_D_SUB_SPRITE);
		REG_DISPCAPCNT = DCAP_BANK(2) | DCAP_ENABLE | DCAP_SIZE(3);
		glBegin2D();
		m_sceneRoot._drawTopAll();
		glEnd2D();
		// Sync.
		glFlush(0);
		swiWaitForVBlank();

		// Draw bottom.
		while (REG_DISPCAPCNT & DCAP_ENABLE)
			;
		lcdMainOnTop();
		vramSetBankD(VRAM_D_LCD);
		vramSetBankC(VRAM_C_SUB_BG);
		REG_DISPCAPCNT = DCAP_BANK(3) | DCAP_ENABLE | DCAP_SIZE(3);
		glBegin2D();
		m_sceneRoot._drawBottomAll();
		glEnd2D();
		// Sync.
		glFlush(0);
		swiWaitForVBlank();

		// Debug helper.
		if (keysDown() & KEY_START)
			break;
	}
}