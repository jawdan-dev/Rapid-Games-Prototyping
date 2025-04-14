#pragma once

#include "goblinEngine.assets.hpp"
#include "goblinEngine.game.object.hpp"
#include "goblinGame.objects.attackParticles.hpp"

class CharacterObject : public GameObject {
public:
	struct CharacterStats {
		float
			// Health.
			m_maxHealth = 100.0f,
			// Attacks.
			m_attackDamage = 10.0f,
			m_attackDistance = 10.0f,
			m_attackDamageRange = 4.0f,
			m_attackCooldownMax = 2.0f,
			// Movement.
			m_moveSpeed = 1.0f,
			m_separationDistance = 4.0f;
	};

	enum class CharacterType {
		Goblin_King,
		Goblin_Normal,
		Goblin_Giant,
		//
		Goblin_Summoner,
		Goblin_Skeleton,
		Goblin_Ghost,
		//
		Goblin_AOAOAO,
		//
		Goblin_Rage,
		//
		Goblin_Snake,
		//
		Goblin_OfficeWorker,
		//

		//
		Enemy_Slime,
		//

		PLAYER_START = Goblin_King,
		PLAYER_END = Goblin_Ghost,

		//
		COUNT = Enemy_Slime + 1,
	};

public:
	static CharacterStats& getCharacterStats(const CharacterType characterType) {
		static CharacterStats s_stats[(int)CharacterType::COUNT];

		static bool initialized = false;
		if (initialized)
			return s_stats[(int)characterType];
		initialized = true;

		for (int i = 0; i < (int)CharacterType::COUNT; i++)
			new (&s_stats[i]) CharacterStats();

		s_stats[(int)CharacterType::Goblin_King].m_maxHealth = 200.0f;
		s_stats[(int)CharacterType::Goblin_Giant].m_maxHealth = 300.0f;
		s_stats[(int)CharacterType::Goblin_Giant].m_attackDamage = 35.0f;
		s_stats[(int)CharacterType::Goblin_Giant].m_attackDamageRange = 20.0f;
		s_stats[(int)CharacterType::Goblin_Giant].m_attackCooldownMax = 8.0f;
		s_stats[(int)CharacterType::Goblin_Summoner].m_maxHealth = 50.0f;
		s_stats[(int)CharacterType::Goblin_Summoner].m_moveSpeed = 0.5f;
		s_stats[(int)CharacterType::Goblin_Summoner].m_attackDamage = 0.0f;
		s_stats[(int)CharacterType::Goblin_Summoner].m_attackDistance = 70.0f;
		s_stats[(int)CharacterType::Goblin_Summoner].m_attackCooldownMax = 4.0f;
		s_stats[(int)CharacterType::Goblin_Summoner].m_separationDistance = 60.0f;
		s_stats[(int)CharacterType::Goblin_Skeleton].m_maxHealth = 30.0f;
		s_stats[(int)CharacterType::Goblin_Skeleton].m_attackDamage = 5.0f;
		s_stats[(int)CharacterType::Goblin_Skeleton].m_moveSpeed = 0.8f;
		s_stats[(int)CharacterType::Goblin_Ghost].m_maxHealth = 1.0f;
		s_stats[(int)CharacterType::Goblin_Ghost].m_attackDamage = 1.0f;
		s_stats[(int)CharacterType::Goblin_Ghost].m_moveSpeed = 0.1f;
		s_stats[(int)CharacterType::Goblin_Rage].m_maxHealth = 80.0f;
		s_stats[(int)CharacterType::Goblin_Rage].m_moveSpeed = 1.1f;
		s_stats[(int)CharacterType::Goblin_Rage].m_attackCooldownMax = 0.6f;
		s_stats[(int)CharacterType::Goblin_Snake].m_maxHealth = 110.0f;
		s_stats[(int)CharacterType::Goblin_Snake].m_moveSpeed = 1.5f;

		return s_stats[(int)characterType];
	}

public:
	CharacterObject(const CharacterType characterType, const Vector2& pos = Vector2(100, 100)) :
		GameObject("CharacterObject"),
		m_characterType(CharacterType::COUNT),
		m_pos(pos), m_lastPosX(pos.m_x), m_flipped(false),
		m_health(0),
		m_target(nullptr), m_attackCooldown(0) {
		s_characters.emplace(this);
		setCharacterType(characterType);
	}
	virtual ~CharacterObject() {
		auto it = s_characters.find(this);
		if (it != s_characters.end())
			s_characters.erase(it);
		//
		std::set<CharacterObject*>& from = isPlayer() ? s_playerCharacters : s_enemyCharacters;
		it = from.find(this);
		if (it != from.end())
			from.erase(it);
		//
		for (auto it = s_characters.begin(); it != s_characters.end(); it++) {
			if ((*it)->m_target != this)
				continue;
			(*it)->m_target = nullptr;
		}
	}
	const Vector2& getPos() const { return m_pos; }
	const CharacterType getCharacterType() const { return m_characterType; }

	void start() override {
		switch (m_characterType) {
			default:
				break;
			case CharacterType::Goblin_King:
				for (int i = 0; i < 5; i++)
					getParent()->addChild(new CharacterObject(CharacterType::Goblin_Normal, m_pos));
				break;
		}
	}
	void update() override {
		const CharacterStats& characterStats = getCharacterStats(m_characterType);

		if (m_attackCooldown > 0.0f)
			m_attackCooldown -= (1.0f / 30.0f);

		if (m_target != nullptr) {
			// Try to move towards target.
			const Vector2 move = m_target->m_pos - m_pos;
			const float targetDist = move.sqrLength();
			if (targetDist < characterStats.m_separationDistance * characterStats.m_separationDistance) {
				m_pos = m_pos + (move.normal() * -1.0f * characterStats.m_moveSpeed);
			} else if (targetDist > characterStats.m_attackDistance * characterStats.m_attackDistance) {
				m_pos = m_pos + (move.normal() * 1.0f * characterStats.m_moveSpeed);
			}
		}

		m_flipped = (m_pos.m_x == m_lastPosX) ? m_flipped : ((m_pos.m_x - m_lastPosX) < 0);
		m_lastPosX = m_pos.m_x;
	}
	void drawTop() override {
		spr_goblins.draw(m_pos.m_x, m_pos.m_y, (int)m_characterType, m_flipped ? GL_FLIP_H : GL_FLIP_NONE);
		// if (m_target != nullptr)
		// 	glLine(
		// 		m_pos.m_x + (spr_goblins.getSpriteWidth() / 2),
		// 		m_pos.m_y,
		// 		m_target->m_pos.m_x + (spr_goblins.getSpriteWidth() / 2),
		// 		m_target->m_pos.m_y + spr_goblins.getSpriteHeight(),
		// 		isPlayer() ? RGB15(31, 31, 31) : RGB15(31, 0, 10));
	}

	void handleAttack() {
		if (m_target == nullptr)
			return;

		const CharacterStats& characterStats = getCharacterStats(m_characterType);

		if (m_attackCooldown > 0.0f)
			return;

		const Vector2 move = m_target->m_pos - m_pos;
		if (move.sqrLength() > (characterStats.m_attackDistance * characterStats.m_attackDistance))
			return;

		// Try to attack.
		m_attackCooldown = characterStats.m_attackCooldownMax;
		dealDamage(m_target->m_pos);
		if (m_target->m_target != this)
			m_target->m_target = this;
		onAttack();
	}

	void takeDamage(const int damage) {
		if (damage <= 0)
			return;
		m_health -= damage;
		if (m_health > 0)
			return;

		onDeath();
		if (m_health > 0)
			return;

		queueFree();
	}
	void dealDamage(const Vector2 target) {
		const CharacterStats& characterStats = getCharacterStats(m_characterType);

		if (characterStats.m_attackDamage <= 0)
			return;

		s_searchTargets.clear();
		AttackParticlesObject::spawnParticle(target, characterStats.m_attackDamageRange, isPlayer() ? 2 : 1);
		getAllCharactersInRange(target, characterStats.m_attackDamageRange, &s_searchTargets, 5, true);
		for (size_t i = 0; i < s_searchTargets.size(); i++) {
			s_searchTargets[i]->takeDamage(characterStats.m_attackDamage);
		}
	}
	void onAttack() {
		switch (m_characterType) {
			default:
				break;
			case CharacterType::Goblin_Summoner:
				for (int i = 0; i < 1; i++) {
					CharacterObject* skeleton = new CharacterObject(CharacterType::Goblin_Skeleton, m_pos);
					skeleton->m_target = m_target;
					getParent()->addChild(skeleton);
					AttackParticlesObject::spawnParticle(m_pos, 10.0f, 3);
				}
				break;
		}
	}
	void onDeath() {
		switch (m_characterType) {
			default:
				AttackParticlesObject::spawnParticle(m_pos, 7.0f, 0);
				break;
			case CharacterType::Goblin_Skeleton:
				setCharacterType(CharacterType::Goblin_Ghost);
				AttackParticlesObject::spawnParticle(m_pos, 10.0f, 3);
				break;
		}
	}

	const bool isPlayer() const {
		return isPlayerType(m_characterType);
	}
	static const bool isPlayerType(const CharacterType characterType) {
		return CharacterType::PLAYER_START <= characterType && characterType <= CharacterType::PLAYER_END;
	}

	void moveRandom() {
		Vector2 moveAmount =
			Vector2(
				(rand() % 3) - 1,
				(rand() % 3) - 1)
				.normal() *
			0.4f * getCharacterStats(m_characterType).m_moveSpeed;

		m_pos = m_pos + moveAmount;
		if (m_pos.m_x < 0)
			m_pos.m_x = 0;
		else if (m_pos.m_x >= SCREEN_WIDTH - spr_goblins.getSpriteWidth())
			m_pos.m_x = SCREEN_WIDTH - spr_goblins.getSpriteWidth();
		if (m_pos.m_y < 0)
			m_pos.m_y = 0;
		else if (m_pos.m_y >= SCREEN_HEIGHT - spr_goblins.getSpriteHeight())
			m_pos.m_y = SCREEN_HEIGHT - spr_goblins.getSpriteHeight();

		m_lastPosX = m_pos.m_x;
	}
	void moveSeparate() {
		s_searchTargets.clear();
		getAllCharactersInRange(8.0f, &s_searchTargets, 3);
		if (s_searchTargets.size() <= 0)
			return;

		const Vector2 center = s_searchTargets[rand() % s_searchTargets.size()]->m_pos;
		m_pos = m_pos + ((m_pos - center).normal() * 2.5f * getCharacterStats(m_characterType).m_moveSpeed);
	}

	CharacterObject* const getNewTarget() const {
		if (isPlayer()) {
			if (s_enemyCharacters.size() <= 0)
				return nullptr;
			auto it = s_enemyCharacters.begin();
			int index = (size_t)rand() % s_enemyCharacters.size();
			while (index > 0) {
				index--;
				it++;
			}
			return *it;
		}

		if (s_playerCharacters.size() <= 0)
			return nullptr;
		auto it = s_playerCharacters.begin();
		int index = (size_t)rand() % s_playerCharacters.size();
		while (index > 0) {
			index--;
			it++;
		}
		return *it;
	}

	void getAllCharactersInRange(const float range, std::vector<CharacterObject*>* output, const size_t maxCount, const bool onlyOpponent = false) {
		return getAllCharactersInRange(m_pos, range, output, maxCount, onlyOpponent);
	}
	void getAllCharactersInRange(const Vector2& pos, const float range, std::vector<CharacterObject*>* output, const size_t maxCount, const bool onlyOpponent = false) {
		// TODO: Spatial optimization??? no time ig...
		const float sqrRange = range * range;
		const bool isPlayerCharacter = isPlayer();
		for (auto it = s_characters.begin(); it != s_characters.end(); it++) {
			if ((*it) == this ||
				(pos - (*it)->m_pos).sqrLength() > sqrRange ||
				(onlyOpponent && (*it)->isPlayer() == isPlayerCharacter))
				continue;
			output->emplace_back((*it));
			if (output->size() >= maxCount)
				return;
		}
	}
	static void _separateMoveCharacters(const size_t count = 3) {
		static size_t indexOffset = 0;
		const size_t end = MIN(indexOffset + count, s_characters.size());

		auto it = s_characters.begin();
		for (size_t i = 0; i < indexOffset; i++)
			it++;
		for (; indexOffset < end; indexOffset++, it++)
			(*it)->moveSeparate();

		if (indexOffset >= s_characters.size())
			indexOffset = 0;
	}
	static void _randomMoveCharacters(const size_t count = 10) {
		static size_t indexOffset = 0;
		const size_t end = MIN(indexOffset + count, s_characters.size());

		auto it = s_characters.begin();
		for (size_t i = 0; i < indexOffset; i++)
			it++;
		for (; indexOffset < end; indexOffset++, it++)
			(*it)->moveRandom();

		if (indexOffset >= s_characters.size())
			indexOffset = 0;
	}
	static void _assignTargetCharacters(const size_t count = 10) {
		static size_t indexOffset = 0;
		const size_t end = MIN(indexOffset + count, s_characters.size());

		auto it = s_characters.begin();
		for (size_t i = 0; i < indexOffset; i++)
			it++;
		for (; indexOffset < end; indexOffset++, it++)
			if ((*it)->m_target == nullptr)
				(*it)->m_target = (*it)->getNewTarget();

		if (indexOffset >= s_characters.size())
			indexOffset = 0;
	}
	static void _handleAttacks(const size_t count = 3) {
		static size_t indexOffset = 0;
		const size_t end = MIN(indexOffset + count, s_characters.size());

		auto it = s_characters.begin();
		for (size_t i = 0; i < indexOffset; i++)
			it++;
		for (; indexOffset < end; indexOffset++, it++)
			(*it)->handleAttack();

		if (indexOffset >= s_characters.size())
			indexOffset = 0;
	}

	void setCharacterType(const CharacterType characterType) {
		if (m_characterType == characterType || characterType >= CharacterType::COUNT)
			return;

		std::set<CharacterObject*>& from = isPlayer() ? s_playerCharacters : s_enemyCharacters;
		std::set<CharacterObject*>& to = isPlayerType(characterType) ? s_playerCharacters : s_enemyCharacters;

		auto it = from.find(this);
		if (it != from.end())
			from.erase(it);
		to.emplace(this);

		m_characterType = characterType;

		const CharacterStats& characterStats = getCharacterStats(m_characterType);
		m_health = characterStats.m_maxHealth;
		start();
	}

public:
	static std::set<CharacterObject*>
		s_characters,
		s_playerCharacters, s_enemyCharacters;

private:
	static std::vector<CharacterObject*> s_searchTargets;

private:
	CharacterType m_characterType;
	//
	Vector2 m_pos;
	float m_lastPosX;
	bool m_flipped;
	//
	float m_health;
	//
	CharacterObject* m_target;
	float m_attackCooldown;
};

std::set<CharacterObject*> CharacterObject::s_characters;
std::set<CharacterObject*> CharacterObject::s_playerCharacters;
std::set<CharacterObject*> CharacterObject::s_enemyCharacters;
std::vector<CharacterObject*> CharacterObject::s_searchTargets;