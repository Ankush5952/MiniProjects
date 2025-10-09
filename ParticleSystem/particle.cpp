#include "particle.h"

Vector2 ParticleSystem::Particle::getPos()
{
    return position;
}

Vector2 ParticleSystem::Particle::getVelocity()
{
    return velocity;
}

int ParticleSystem::Particle::getRadius()
{
    return radius;
}

Color ParticleSystem::Particle::getColor()
{
    return color;
}

float ParticleSystem::Particle::getLifetime()
{
    return lifetime;
}

float ParticleSystem::Particle::getTimeSinceLifeBegan()
{
    return timeSinceLifeBegan;
}

ParticleSystem::CollissionAlgo ParticleSystem::Particle::getCollissionResponse()
{
    return collissionResponse;
}

void ParticleSystem::Particle::setPos(Vector2 p)
{
    position = p;
}

void ParticleSystem::Particle::setVelocity(Vector2 v)
{
    velocity = v;
}

void ParticleSystem::Particle::setRadius(int r)
{
    radius = r;
}

void ParticleSystem::Particle::setColor(Color c)
{
    color = c;
}

void ParticleSystem::Particle::setLifetime(int t)
{
    lifetime = t;
}

void ParticleSystem::Particle::setCollissionResponse(CollissionAlgo c)
{
    collissionResponse = c;
}

void ParticleSystem::Particle::update(float dt)
{
    timeSinceLifeBegan += dt;

    position += velocity * dt;
}

void ParticleSystem::Particle::draw()
{
    DrawCircleV(position, radius, color);
}

void ParticleSystem::Particle::resetParticle()
{
    timeSinceLifeBegan = 0.0f;
}

ParticleSystem::Particle::Particle(int r, float t,Color c, Vector2 pos, Vector2 vel, CollissionAlgo response)
{
    radius = r;
    lifetime = t;
    position = pos;
    velocity = vel;
    color = c;
    collissionResponse = response;

    timeSinceLifeBegan = 0;
}
