#pragma once

#include"particle.h"
namespace ParticleSystem
{
	class ParticleManager
	{
	private:
		std::vector<ParticleSystem::Particle*> particles;
		std::vector<ParticleSystem::Particle*> particlesToDelete;
		float bounceFactor = 0.8f;

	public:
	//getters
		std::vector<ParticleSystem::Particle*> getParticles() const;
		bool getIsMeantForDeletion(Particle* p);

	//setters
		void addParticle(ParticleSystem::Particle* p);
		void removeParticle(ParticleSystem::Particle* p);

	//misc
		void createParticle(ParticleSystem::Particle* p);
		void createParticle(int radius = 1, float lifetime = 1.0f, Color c = WHITE, Vector2 pos = { 0,0 }, Vector2 vel = { 0,0 }, CollissionAlgo response = BOUNCE);
		void update(float dt);
		void draw();

	//collission
		void convertParticle(Particle* from, Particle* to);
		void absorbParticle(Particle* absorber, Particle* absorbed);
		void BounceParticles(Particle* a, Particle* b, Vector2 normal, float bounce_factor);
		void stickParticles(Particle* a, Particle* b);
		void repelParticles(Particle* a, Particle* b, Vector2 normal);
		bool checkParticleCollission(ParticleSystem::Particle* a, ParticleSystem::Particle* b);
		void resolveParticleCollission(ParticleSystem::Particle* a, ParticleSystem::Particle* b);

	};
}



