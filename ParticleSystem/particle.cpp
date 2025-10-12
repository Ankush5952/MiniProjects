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
    
    if (shape == TRIANGLE) updateTriangleGeometry();
}

void ParticleSystem::Particle::setVelocity(Vector2 v)
{
    velocity = v;
}

void ParticleSystem::Particle::setSide(int r)
{
    side = r;

    if (shape == TRIANGLE) updateTriangleGeometry();
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

    velocity += gravity * dt;

    position += velocity * dt;

    if (boundary)
    {
        float extraSep = 0.01f;
        float halfside = side * 0.5f + 0.01f;
        float l = position.x + halfside;
        float r = position.x - halfside;
        float u = position.y - halfside;
        float d = position.y + halfside;

        if (l >= WIDTH)
        {
            position.x = WIDTH - halfside;
            velocity.x = -velocity.x;
        }
        if (r <= 0)
        {
            position.x = halfside;
            velocity.x = -velocity.x;
        }
        if (u <= 0)
        {
            position.y = halfside;
            velocity.y = -velocity.y;
        }
        if (l >= HEIGHT)
        {
            position.y = HEIGHT + halfside;
            velocity.y = -velocity.y;
        }
    }

    if(shape == TRIANGLE) updateTriangleGeometry();
}

void ParticleSystem::Particle::draw()
{
    float lifePercent = timeSinceLifeBegan / lifetime;
    float fadeVal = (fadeEffect)? 1.0 - lifePercent : color.a;
    switch (shape)
    {
        case CIRCLE:
            DrawCircleV(position, side, Fade(color, fadeVal)); 
            break;
        case SQUARE: 
            DrawRectangle(position.x - side*0.5f, position.y - side*0.5f, side, side, Fade(color,fadeVal));
            break;
        case TRIANGLE:
            DrawTriangle(v2, v1, v3, Fade(color,fadeVal));
            break;
        default:
            DrawCircleV(position, side, Fade(color,fadeVal));
    }
}

void ParticleSystem::Particle::resetParticle()
{
    timeSinceLifeBegan = 0.0f;
}

void ParticleSystem::Particle::updateTriangleGeometry()
{
    v1 = { position.x - side * 0.5f, position.y + float(side * 0.5f * oneOverRoot3) };
    v2 = { position.x, position.y - float(side * oneOverRoot3) };
    v3 = { position.x + side * 0.5f,position.y + float(side * 0.5f * oneOverRoot3) };

    Vector2 side1 = Vector2Subtract(v2, v1);
    Vector2 side2 = Vector2Subtract(v3, v2);
    Vector2 side3 = Vector2Subtract(v1, v3);

    n1 = Vector2Normalize({ -side1.y, side1.x });
    n2 = Vector2Normalize({ -side2.y, side2.x });
    n3 = Vector2Normalize({ -side3.y, side3.x });
}

ParticleSystem::Particle::Particle(ParticleShape s, int r, float t,Color c, Vector2 pos, Vector2 vel, CollissionAlgo response)
{
    shape = s;
    side = r;
    lifetime = t;
    position = pos;
    velocity = vel;
    color = c;
    collissionResponse = response;

    timeSinceLifeBegan = 0;

    if (s == TRIANGLE)
    {
        updateTriangleGeometry();
    }
}
