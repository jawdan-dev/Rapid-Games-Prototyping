#pragma once

#include "goblinEngine.assets.hpp"
#include "goblinEngine.game.object.hpp"

class AttackParticlesObject : public GameObject {
public:
	AttackParticlesObject() :
		GameObject("AttackParticlesObject") { s_instance = this; }

	void update() override {
		for (auto it = m_particles.begin(); it != m_particles.end();) {
			(*it)->m_remainingLife -= (3.0f / 30.0f);
			if ((*it)->m_remainingLife > 0.0f) {
				it++;
				continue;
			}
			delete (*it);
			it = m_particles.erase(it);
		}
	}

	void drawTop() override {
		for (auto it = m_particles.begin(); it != m_particles.end(); it++) {
			const int size = (int)((*it)->m_remainingLife * (*it)->m_size);
			spr_attack.drawTo((*it)->m_pos.m_x - size, (*it)->m_pos.m_y - size, size * 2, size * 2, (*it)->m_particleType);
		}
	}

	void spawnParticleDirect(const Vector2& pos, const float size, const int type) {
		m_particles.emplace(new ParticleLife{.m_pos = pos + Vector2(8, 8), .m_size = size, .m_particleType = type });
	}

	static void spawnParticle(const Vector2& pos, const float size, const int type) {
		if (s_instance == nullptr) return;
		s_instance->spawnParticleDirect(pos, size, type);
	}

public:
	static AttackParticlesObject* s_instance;

private:
	struct ParticleLife {
		float m_remainingLife = 1.0f;
		Vector2 m_pos;
		float m_size = 10.0f;
		int m_particleType = 0;
	};
	std::set<ParticleLife*> m_particles;
};
AttackParticlesObject* AttackParticlesObject::s_instance = nullptr;