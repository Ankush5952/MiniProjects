#pragma once
#ifndef PARTICLEMANAGER_H

#include"particle.h"
namespace ParticleSystem
{
	class ParticleManager
	{
	private:
		std::vector<ParticleSystem::Particle*> particles;

	public:
	//getters
		std::vector<ParticleSystem::Particle*> getParticles();

	//setters
		void addParticle(ParticleSystem::Particle* p);
		void removeParticle(ParticleSystem::Particle* p);

	//misc
		void createParticle(ParticleSystem::Particle* p);
		void createParticle(int radius = 1.0f, float lifetime = 1.0f, Color c = WHITE, Vector2 pos = { 0,0 }, Vector2 vel = { 0,0 });
		void update(float dt);
		void draw();
	};
}
#endif // !PARTICLEMANAGER_H



