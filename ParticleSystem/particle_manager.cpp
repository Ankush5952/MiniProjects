#include "particle_manager.h"

std::vector<ParticleSystem::Particle*> ParticleSystem::ParticleManager::getParticles()
{
	return particles;
}

void ParticleSystem::ParticleManager::addParticle(ParticleSystem::Particle* p)
{
	particles.push_back(p);
}

void ParticleSystem::ParticleManager::removeParticle(ParticleSystem::Particle* p)
{
	if (particles.size() == 0) return;
	if (std::find(particles.begin(), particles.end(), p) == particles.end()) return;
	particles.erase(std::remove(particles.begin(), particles.end(), p), particles.end());
	delete(p);
}

void ParticleSystem::ParticleManager::createParticle(ParticleSystem::Particle* p)
{
	particles.push_back(p);
}

void ParticleSystem::ParticleManager::createParticle(int radius, float lifetime,Color c, Vector2 pos, Vector2 vel)
{
	ParticleSystem::Particle* temp = new ParticleSystem::Particle(radius, lifetime, c, pos, vel);
	particles.push_back(temp);
}

void ParticleSystem::ParticleManager::update(float dt)
{
	std::vector<ParticleSystem::Particle*> particlesToDelete;
	for (auto& i : particles)
	{
		i->update(dt);
		if (i->getTimeSinceLifeBegan() >= i->getLifetime()) particlesToDelete.push_back(i);
	}
	for (auto& i : particlesToDelete) removeParticle(i);
}

void ParticleSystem::ParticleManager::draw()
{
	for (auto& i : particles)
	{
		i->draw();
	}
}