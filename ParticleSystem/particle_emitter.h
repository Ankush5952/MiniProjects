#pragma once

#include"particle_manager.h"
namespace ParticleSystem
{
	class ParticleEmitter
	{
	private:
		Particle type;
		Vector2 position;
		Vector2 velRange;
		int frequency;

		double lastSpawnTime;

	public:
		//getters
		const Particle& getParticleData() const;
		Vector2 getPos() const;
		Vector2 getVelRangeX() const;
		int getFrequency() const;

		//setters
		void setParticleType(Particle p);
		void setPos(Vector2 p);
		void setVelRangeX(Vector2 v);
		void setFrequency(int f);

		//functionality
		void spawnParticles(ParticleManager* manager);
		void update(ParticleManager* manager);

		//constructor
		ParticleEmitter(Particle p, Vector2 pos = {0,0}, Vector2 vel = {0,0}, int freq = 1);
	};
}
