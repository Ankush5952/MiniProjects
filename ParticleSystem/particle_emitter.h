#pragma once

#include"particle_manager.h"
#include"preset_manager.h"
namespace ParticleSystem
{
	class ParticleEmitter
	{
	private:
		Particle type;
		Vector2 position;
		Vector2 velRangeMin;
		Vector2 velRangeMax;
		float frequency;

		double lastSpawnTime;
		double secondsPerParticle;

	public:
		std::string name = "default";
		bool enabled = false;
		//getters
		const Particle& getParticleData() const;
		Vector2 getPos() const;
		Vector2 getVelRangeX() const;
		Vector2 getVelRangeY() const;
		float getFrequency() const;

		//setters
		void setParticleType(Particle p);
		void setPos(Vector2 p);
		void setVelRange(Vector2 vx, Vector2 vy);
		void setFrequency(float f);

		//functionality
		void spawnParticle(ParticleManager* manager);
		void update(ParticleManager* manager);

		//constructor
		ParticleEmitter() = default;
		ParticleEmitter(Particle p, Vector2 pos = { 0,0 }, Vector2 velMin = { 0,0 }, Vector2 velMax = { 0,0 }, float freq = 1);
		ParticleEmitter(const Preset& pre, Vector2 pos = {0,0}, float freq = 1);
	};
}
