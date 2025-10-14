#include "particle_emitter.h"

const ParticleSystem::Particle& ParticleSystem::ParticleEmitter::getParticleData() const
{
	return type;
}

Vector2 ParticleSystem::ParticleEmitter::getPos() const
{
	return position;
}

Vector2 ParticleSystem::ParticleEmitter::getVelRangeX() const
{
	return velRange;
}

int ParticleSystem::ParticleEmitter::getFrequency() const
{
	return frequency;
}

void ParticleSystem::ParticleEmitter::setParticleType(Particle p)
{
	type = p;
}

void ParticleSystem::ParticleEmitter::setPos(Vector2 p)
{
	position = p;
}

void ParticleSystem::ParticleEmitter::setVelRangeX(Vector2 v)
{
	velRange = v;
}

void ParticleSystem::ParticleEmitter::setFrequency(int f)
{
	frequency = f;
}

void ParticleSystem::ParticleEmitter::spawnParticles(ParticleManager* manager)
{
	for (int i = 0; i < frequency; i++)
	{
		Vector2 vel = {
			velRange.x - rand() % (int)velRange.x,
			velRange.y - rand() % (int)velRange.y
		};
		type.setVelocity(vel);

		manager->createParticle(type);
	}
}

void ParticleSystem::ParticleEmitter::update(ParticleManager* manager)
{
	double timeSinceLastSpawn = GetTime() - lastSpawnTime;
	if(timeSinceLastSpawn > 1.0)
	{
		lastSpawnTime = GetTime();
		spawnParticles(manager);
	}
}

ParticleSystem::ParticleEmitter::ParticleEmitter(Particle p, Vector2 pos, Vector2 vel, int freq)
{
	type = p;
	position = pos;
	velRange = vel;
	frequency = freq;
	lastSpawnTime = GetTime();

	type.setPos(pos);
}
