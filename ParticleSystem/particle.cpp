#include "particle.h"

Vector2 ParticleSystem::Particle::getPos() const
{
    return position;
}

Vector2 ParticleSystem::Particle::getVelocity() const
{
    return velocity;
}

int ParticleSystem::Particle::getSide() const
{
    return side;
}

ParticleSystem::ParticleShape ParticleSystem::Particle::getShape() const
{
    return shape;
}

Color ParticleSystem::Particle::getColor() const
{
    return color;
}

float ParticleSystem::Particle::getLifetime() const
{
    return lifetime;
}

float ParticleSystem::Particle::getTimeSinceLifeBegan() const
{
    return timeSinceLifeBegan;
}

ParticleSystem::CollissionAlgo ParticleSystem::Particle::getCollissionResponse() const
{
    return collissionResponse;
}

Vector2 ParticleSystem::Particle::getTriangleVertex(int num) const
{
    if (shape != TRIANGLE) throw std::exception("INVALID SHAPE FOR VERTEX");
    switch (num)
    {
        case 1:
            return v1;
            break;
        case 2:
            return v2;
            break;
        case 3:
            return v3;
            break;
        default:
            throw std::exception("Invalid Vertex count");
    }
}

Vector2 ParticleSystem::Particle::getTriangleNormal(int num) const
{
    if (shape != TRIANGLE) throw std::exception("INVALID SHAPE FOR NORMAL");
    switch (num)
    {
    case 1:
        return n1;
        break;
    case 2:
        return n2;
        break;
    case 3:
        return n3;
        break;
    default:
        throw std::exception("Invalid Normal count");
    }
}

void ParticleSystem::Particle::setPos(Vector2 p)
{
    position = p;
}

void ParticleSystem::Particle::setVelocity(Vector2 v)
{
    velocity = v;
}

void ParticleSystem::Particle::setSide(int r)
{
    side = r;
}

void ParticleSystem::Particle::setShape(ParticleShape s)
{
    shape = s;
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
    switch (shape)
    {
        case CIRCLE:
            DrawCircleV(position, side, color); 
            break;
        case SQUARE: 
            DrawRectangle(position.x - side, position.y - side, side, side, color); 
            break;
        case TRIANGLE:
            DrawTriangle(v1, v2, v3, color);
            break;
        default:
            DrawCircleV(position, side, color);
    }
}

void ParticleSystem::Particle::resetParticle()
{
    timeSinceLifeBegan = 0.0f;
}

ParticleSystem::Particle::Particle(ParticleShape s, int r, float t,Color c, Vector2 pos, Vector2 vel, CollissionAlgo response)
{

    side = r;
    lifetime = t;
    position = pos;
    velocity = vel;
    color = c;
    collissionResponse = response;

    timeSinceLifeBegan = 0;

    if (shape == TRIANGLE)
    {
        v1 = { position.x - side * 0.5f, position.y + float(side * 0.5f * oneOverRoot3) };
        v2 = { position.x, position.y - float(side * oneOverRoot3) };
        v3 = { position.x + side * 0.5f,position.y + float(side * 0.5f * oneOverRoot3) };

        Vector2 side1 = Vector2Subtract(v2, v1);
        Vector2 side2 = Vector2Subtract(v3, v2);
        Vector2 side3 = Vector2Subtract(v1, v3);

        n1 = Vector2Normalize({ -side1.x, side1.y });
        n2 = Vector2Normalize({ -side2.x, side2.y });
        n3 = Vector2Normalize({ -side3.x, side3.y });
    }
}
