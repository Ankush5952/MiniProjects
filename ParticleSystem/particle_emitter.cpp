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
	return velRangeMin;
}

Vector2 ParticleSystem::ParticleEmitter::getVelRangeY() const
{
	return velRangeMax;
}

float ParticleSystem::ParticleEmitter::getFrequency() const
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

void ParticleSystem::ParticleEmitter::setVelRange(Vector2 vx, Vector2 vy)
{
	velRangeMin = vx;
	velRangeMax = vy;
}

void ParticleSystem::ParticleEmitter::setFrequency(float f)
{
	frequency = f;
}

void ParticleSystem::ParticleEmitter::spawnParticle(ParticleManager* manager)
{
	Vector2 vel = {
		velRangeMin.x + (float(rand()) / RAND_MAX) * (velRangeMax.x - velRangeMin.x),
		velRangeMin.y + (float(rand()) / RAND_MAX) * (velRangeMax.y - velRangeMin.y)
	};
	type.setPos(position);
	type.setVelocity(vel);

	manager->createParticle(type);
}

void ParticleSystem::ParticleEmitter::update(ParticleManager* manager)
{
	double timeSinceLastSpawn = GetTime() - lastSpawnTime;
	if(timeSinceLastSpawn >= secondsPerParticle)
	{
		lastSpawnTime = GetTime();
		spawnParticle(manager);
	}
}

ParticleSystem::ParticleEmitter::ParticleEmitter(Particle p, Vector2 pos, Vector2 velMin, Vector2 velMax, float freq)
{
	type = p;
	position = pos;
	velRangeMin = velMin;
	velRangeMax = velMax;
	frequency = freq;
	lastSpawnTime = GetTime();
	name = "default";
	enabled = false;

	type.setPos(pos);
	frequency = (frequency <= 0) ? 1 : frequency;
	secondsPerParticle = 1 / frequency;
}

ParticleSystem::ParticleEmitter::ParticleEmitter(const Preset& pre, Vector2 pos, float freq)
{
	type = {
		pre.shape,
		pre.size,
		pre.lifetime,
		pre.color,
		pos,
		{0,0},
		pre.algo
	};

	position = pos;
	
	velRangeMin = { -pre.velocity.x, -pre.velocity.y };
	velRangeMax = { pre.velocity.x, pre.velocity.y };
	frequency = freq;

	name = pre.name;
	enabled = false;

	lastSpawnTime = GetTime();

	frequency = (frequency == 0) ? 1 : frequency;
	secondsPerParticle = 1 / frequency;
}
