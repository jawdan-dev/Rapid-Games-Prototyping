#pragma once

#include "goblinEngine.assets.hpp"
#include "goblinEngine.gui.hpp"

#include "goblinGame.objects.background.hpp"
#include "goblinGame.objects.character.hpp"
#include "goblinGame.objects.characterSorter.hpp"

#include "goblinGame.cards.hpp"

class GameManager : public GameObject {
public:
	GameManager() :
		GameObject("GameManager"),
		m_cardChoice(-1) {}

	void update() override {
		if (CharacterObject::s_playerCharacters.size() <= 0) {
			// Player loss state.
			BackgroundObject::s_enabled = true;
		} else if (CharacterObject::s_enemyCharacters.size() <= 0) {
			// Fighting done state.
			BackgroundObject::s_enabled = true;
		} else {
			BackgroundObject::s_enabled = false;
		}
	}

	void drawTop() override {
		if (!BackgroundObject::s_enabled)
			return;
		if (m_cardChoice <= -1 || m_availableCards.size() <= (size_t)m_cardChoice)
			return;

		const int verticalSpacing = 40, horizontalSpacing = 40;
		GUI_LabelPanel(Card::getCardDescription(m_availableCards[(size_t)m_cardChoice]), horizontalSpacing, verticalSpacing, SCREEN_WIDTH - (horizontalSpacing * 2), SCREEN_HEIGHT - (verticalSpacing * 2));
	}
	void drawBottom() override {
		if (CharacterObject::s_playerCharacters.size() <= 0) {
			// Player loss state.
			drawDeathScreen(BackgroundObject::s_enabled);
			return;
		}
		// Fighting done state.
		drawSelectScreen(BackgroundObject::s_enabled);
	}

	void drawDeathScreen(const bool enabled) {
		GUI_Label("End state reached!", 10, 10, SCREEN_WIDTH - 20, 30, enabled);
		GUI_Label("Thanks for playing!!", 10, 40, SCREEN_WIDTH - 20, 30, enabled);
		if (GUI_Button("Exit Game", 48, 100, SCREEN_WIDTH - 96, SCREEN_HEIGHT - 150, enabled)) {
			exit(1);
		}
	}
	void drawSelectScreen(const bool enabled) {
		// Present 3 cards.

		const int spacing = 10,
				  promptLabelHeight = 6,
				  confirmButtonHeight = 22,
				  confirmButtonWidthBorder = 80,
				  cardCount = 3;

		if (enabled && m_availableCards.size() == 0) {
			Card::getAvailableCards(cardCount, &m_availableCards);
			if (m_availableCards.size() <= 0) {
				drawDeathScreen(enabled);
				return;
			}
		}

		const int cardHeight = SCREEN_HEIGHT - (spacing + promptLabelHeight + spacing + spacing + confirmButtonHeight + spacing),
				  cardWidth = (SCREEN_WIDTH - (spacing * (cardCount + 1))) / cardCount,
				  cardSpacing = 5,
				  cardLabelHeight = 6 * 3,
				  cardImageSize = cardWidth - (cardSpacing * 2);

		GUI_Label("Choose a Card.", spacing, spacing, SCREEN_WIDTH - (spacing * 2), promptLabelHeight, enabled);

		int cardXOffset = spacing, cardYOffset = spacing + promptLabelHeight + spacing;
		for (size_t i = 0; i < cardCount; i++) {
			const bool yeahEnabled = enabled && i < m_availableCards.size();
			if (GUI_Button("", cardXOffset, cardYOffset, cardWidth, cardHeight, yeahEnabled, (size_t)m_cardChoice == i)) {
				m_cardChoice = i;
			}
			if (yeahEnabled) {
				// Draw card stuff.
				GUI_Label(Card::getCardName(m_availableCards[i]), cardXOffset + cardSpacing, cardYOffset + cardSpacing, cardImageSize, cardLabelHeight);
				Card::drawCard(cardXOffset + cardSpacing, cardYOffset + cardSpacing + cardLabelHeight + cardSpacing, cardImageSize, cardImageSize, m_availableCards[i]);
			}
			cardXOffset += cardWidth + spacing;
		}

		if (GUI_Button("Confirm", spacing + confirmButtonWidthBorder, spacing + promptLabelHeight + spacing + cardHeight + spacing, SCREEN_WIDTH - ((spacing + confirmButtonWidthBorder) * 2), confirmButtonHeight, enabled && m_cardChoice != -1)) {
			Card::activateCard(m_availableCards[m_cardChoice]);
			m_cardChoice = -1;
			m_availableCards.clear();
			startNextWave();
		}
	}

	void startNextWave() {
		Card::applyCards();

		// Get wave values.
		static float waveDifficulty = 2;
		struct {
			float m_waveValue;
			int m_factor;
			CharacterObject::CharacterType m_type;
		} characterDifficulties[]{
			{
				.m_waveValue = 1.0f,
				.m_factor = 100,
				.m_type = CharacterObject::CharacterType::Goblin_OfficeWorker,
			},
			{
				.m_waveValue = 2.0f,
				.m_factor = 30,
				.m_type = CharacterObject::CharacterType::Goblin_AOAOAO,
			}};

		CharacterSorterObject* const sorter = (CharacterSorterObject*)(getObjectInScene("CharacterSorterObject"));
		if (sorter != nullptr) {
			Vector2 spawnPos(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);

			// Spawn les bois.
			float remainingWaveDifficulty = waveDifficulty;
			while (remainingWaveDifficulty > 0) {
				int totalAvailableFactor = 0;
				for (size_t i = 0; i < sizeof(characterDifficulties) / sizeof(*characterDifficulties); i++) {
					if (characterDifficulties[i].m_waveValue > remainingWaveDifficulty)
						continue;
					totalAvailableFactor += characterDifficulties[i].m_factor;
				}
				if (totalAvailableFactor <= 0)
					break;

				int choice = rand() % totalAvailableFactor;
				for (size_t i = 0; i < sizeof(characterDifficulties) / sizeof(*characterDifficulties); i++) {
					if (characterDifficulties[i].m_waveValue > remainingWaveDifficulty)
						continue;
					choice -= characterDifficulties[i].m_factor;
					if (choice > 0)
						continue;

					// Spawn.
					remainingWaveDifficulty -= characterDifficulties[i].m_waveValue;
					sorter->addChild(new CharacterObject(characterDifficulties[i].m_type, spawnPos));

					break;
				}
			}
		}

		// Make more difficult.
		waveDifficulty *= 1.5;
	}

private:
	int m_cardChoice;
	std::vector<Card::CardType> m_availableCards;
};
