#pragma once

#include "goblinEngine.assets.hpp"

class Card {
public:
	enum class CardType {
		Goblin_King_SummonOnStart,
		Goblin_Normal_ToKing,
		Goblin_Normal_ToGiant,
		Goblin_Normal_ToSnake,
		Goblin_Normal_ToRage,

		Goblin_Summoner_AttackSpeedUp,
		Goblin_Skeleton_ToNormal,

		Goblin_Rage_ToAOAOAO,

		COUNT
	};

public:
	virtual ~Card() = 0;

	static const char* getCardName(const CardType cardType) {
		switch (cardType) {
			case CardType::Goblin_King_SummonOnStart:
				return "King Summoner";
			case CardType::Goblin_Normal_ToKing:
				return "Royal Promotion";
			case CardType::Goblin_Normal_ToGiant:
				return "Gym";
			case CardType::Goblin_Normal_ToSnake:
				return "Medusa's Medicine";
			case CardType::Goblin_Normal_ToRage:
				return "Momentary Madness";

			case CardType::Goblin_Summoner_AttackSpeedUp:
				return "Summoner Atk Spd+";
			case CardType::Goblin_Skeleton_ToNormal:
				return "Reanimate";

			case CardType::Goblin_Rage_ToAOAOAO:
				return "???";
		}
		return "Unnamed";
	}
	static const char* getCardDescription(const CardType cardType) {
		switch (cardType) {
			case CardType::Goblin_King_SummonOnStart:
				return "Allows Goblin Kings to summon 1 goblin at the start of every wave.";
			case CardType::Goblin_Normal_ToKing:
				return "Promotes 1 goblin to king.";
			case CardType::Goblin_Normal_ToGiant:
				return "Converts 2 goblins to giant goblins";
			case CardType::Goblin_Normal_ToSnake:
				return "Converts 4 goblins to serpents.";
			case CardType::Goblin_Normal_ToRage:
				return "Enrages 3 goblins.";

			case CardType::Goblin_Summoner_AttackSpeedUp:
				return "Increase the summon speed of all summoners.";
			case CardType::Goblin_Skeleton_ToNormal:
				return "Reanimates all skeletons to fully-fleshed goblins.";

			case CardType::Goblin_Rage_ToAOAOAO:
				return "Science has y?t to disc?ver th? eff?cts of this.";
		}
		return "Unknown effects.";
	}

	static void drawCard(const int x, const int y, const int w, const int h, const CardType cardType) {
		spr_cards.drawTo(x, y, w, h, getDrawIndex(cardType));
	}
	static const int getDrawIndex(const CardType cardType) {
		switch (cardType) {
			case CardType::Goblin_King_SummonOnStart:
				return 1;
			case CardType::Goblin_Summoner_AttackSpeedUp:
				return 2;
		}
		return 0;
	}

	static void summonFrom(const CharacterObject::CharacterType from, const CharacterObject::CharacterType type, const int n) {
		for (auto it = CharacterObject::s_characters.begin(); it != CharacterObject::s_characters.end(); it++) {
			if ((*it)->getCharacterType() != from)
				continue;

			for (int i = 0; i < n; i++) {
				(*it)->getParent()->addChild(new CharacterObject(type, (*it)->getPos()));
				AttackParticlesObject::spawnParticle((*it)->getPos(), 10.0f, 3);
			}
			break;
		}
	}
	static void convertXToYNTimes(const CharacterObject::CharacterType from, const CharacterObject::CharacterType to, const int n) {
		int remaining = n;
		for (auto it = CharacterObject::s_characters.begin(); it != CharacterObject::s_characters.end(); it++) {
			if ((*it)->getCharacterType() != from)
				continue;

			(*it)->setCharacterType(to);

			remaining--;
			if (n != -1 && remaining <= 0)
				return;
		}
	}

	static void applyCard(const CardType cardType) {
		switch (cardType) {
			case CardType::Goblin_King_SummonOnStart: {
				summonFrom(CharacterObject::CharacterType::Goblin_King, CharacterObject::CharacterType::Goblin_Normal, 1);
			} break;

			case CardType::Goblin_Normal_ToKing: {
				convertXToYNTimes(CharacterObject::CharacterType::Goblin_Normal, CharacterObject::CharacterType::Goblin_King, 1);
			} break;
			case CardType::Goblin_Normal_ToGiant: {
				convertXToYNTimes(CharacterObject::CharacterType::Goblin_Normal, CharacterObject::CharacterType::Goblin_Giant, 2);
			} break;
			case CardType::Goblin_Normal_ToSnake: {
				convertXToYNTimes(CharacterObject::CharacterType::Goblin_Normal, CharacterObject::CharacterType::Goblin_Snake, 4);
			} break;
			case CardType::Goblin_Normal_ToRage: {
				convertXToYNTimes(CharacterObject::CharacterType::Goblin_Normal, CharacterObject::CharacterType::Goblin_Rage, 3);
			} break;

			case CardType::Goblin_Summoner_AttackSpeedUp: {
				CharacterObject::getCharacterStats(CharacterObject::CharacterType::Goblin_Summoner).m_attackCooldownMax *= 0.8;
			} break;
			case CardType::Goblin_Skeleton_ToNormal: {
				convertXToYNTimes(CharacterObject::CharacterType::Goblin_Skeleton, CharacterObject::CharacterType::Goblin_Normal, -1);
			} break;

			case CardType::Goblin_Rage_ToAOAOAO: {
				convertXToYNTimes(CharacterObject::CharacterType::Goblin_Rage, CharacterObject::CharacterType::Goblin_AOAOAO, -1);
			} break;
		}
	}
	static const bool isAppliedOnce(const CardType cardType) {
		switch (cardType) {
			default:
				return true;
			case CardType::Goblin_King_SummonOnStart:
				return false;
		}
		return true;
	}
	static const bool isPermanentCard(const CardType cardType) {
		switch (cardType) {
			default:
				return false;
		}
	}

	static void getAvailableCards(const size_t count, std::vector<Card::CardType>* output) {
		if (m_availableCards.size() <= 0)
			return;

		for (size_t remainingAttempts = count * 10; remainingAttempts > 0 && output->size() < count; remainingAttempts--) {
			const CardType choice = m_availableCards[rand() % m_availableCards.size()];

			bool found = false;
			for (size_t j = 0; j < output->size(); j++) {
				if ((*output)[j] != choice)
					continue;
				found = true;
				break;
			}
			if (found)
				continue;

			output->emplace_back(choice);
		}
	}
	static void activateCard(const CardType cardType) {
		if (!isPermanentCard(cardType)) {
			for (size_t i = 0; i < m_availableCards.size(); i++) {
				if (m_availableCards[i] != cardType)
					continue;
				m_availableCards.erase(m_availableCards.begin() + i);
				break;
			}
		}
		if (isAppliedOnce(cardType)) {
			applyCard(cardType);
		} else {
			m_activeCards.emplace_back(cardType);
		}
	}

	static void applyCards() {
		for (size_t i = 0; i < m_activeCards.size(); i++)
			applyCard(m_activeCards[i]);
	}

public:
	static std::vector<CardType> m_availableCards, m_activeCards;
};

std::vector<Card::CardType> Card::m_availableCards{
	Card::CardType::Goblin_King_SummonOnStart,
	Card::CardType::Goblin_Normal_ToKing,
	Card::CardType::Goblin_Normal_ToGiant,
	Card::CardType::Goblin_Normal_ToSnake,
	Card::CardType::Goblin_Normal_ToRage,

	Card::CardType::Goblin_Summoner_AttackSpeedUp,
	Card::CardType::Goblin_Skeleton_ToNormal,

	Card::CardType::Goblin_Rage_ToAOAOAO,
};
std::vector<Card::CardType> Card::m_activeCards;