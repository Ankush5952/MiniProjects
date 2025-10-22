#include "particle.h"

void ParticleSystem::Particle::loadShader()
{
    if (!isParticleShaderLoaded)
    {
        particleShader = LoadShader("particle_vert.glsl", "particle_frag.glsl");
        isParticleShaderLoaded = true;
    }

    lifetimeLoc = GetShaderLocation(particleShader, "lifetime");
    timeLoc = GetShaderLocation(particleShader, "timeSinceLifeBegan");
    fadeLoc = GetShaderLocation(particleShader, "fadeEnabled");
    glowLoc = GetShaderLocation(particleShader, "glowEnabled");
    glowIntensityLoc = GetShaderLocation(particleShader, "glowIntensity");

    std::cout << "lifetimeloc : " << lifetimeLoc << ", timeloc : " << timeLoc << ", fadeloc : " << fadeLoc << ", glowloc : " << glowLoc << ", GIloc : " << glowIntensityLoc;
}

void ParticleSystem::Particle::unloadShader()
{
    if (isParticleShaderLoaded)
    {
        UnloadShader(particleShader);
        isParticleShaderLoaded = false;
    }
}

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

void ParticleSystem::Particle::updateTrail()
{
    if (trail.empty())
    {
        trail.push_front(position);
    }
    else
    {
        float dx = position.x - trail.front().x;
        float dy = position.y - trail.front().y;
        float dist = dx * dx + dy * dy;

        if(dist > MIN_DIST_TRAIL)
        {
            trail.push_front(position);

            if (trail.size() > maxTrail)
            {
                trail.pop_back();
            }
        }
    }
}

void ParticleSystem::Particle::update(float dt)
{

    timeSinceLifeBegan += dt;

    velocity += gravity * dt;

    if (trailEffect)
    {
        updateTrail();
    }
    else
    {
        if(!trail.empty()) trail.clear();
    }

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
        if (d >= HEIGHT)
        {
            position.y = HEIGHT - halfside;
            velocity.y = -velocity.y;
        }
    }

    if(shape == TRIANGLE) updateTriangleGeometry();
}

void ParticleSystem::Particle::draw()
{
    float lifePercent = timeSinceLifeBegan / lifetime;
    float fadeVal = (fadeEffect)? 1.0 - lifePercent : 1.0f;

    float thick = 1.0f;
    if (shape == CIRCLE) thick = side * 1.5f;
    if (shape == SQUARE) thick = side * .5f;
    if (shape == TRIANGLE) thick = side * oneOverRoot3;

    //draw trails
    if (trailEffect && !trail.empty())
    {
        for (int i = 1; i < trail.size(); i++)
        {
            float t = float(i) / (trail.size());
            thick = thick * (1 - t * 0.5f);
            float trailAlpha = fadeVal * (1.0f - t);
            DrawLineEx(trail[i - 1], trail[i], thick, Fade(color, trailAlpha));
        }
    }

    float r = side;
    if (shape == SQUARE || shape == TRIANGLE) r = thick;
    if (shape == TRIANGLE) r = thick;
    int fadeInt = fadeEffect;
    int glowInt = glowEffect;
    SetShaderValue(particleShader, lifetimeLoc, &lifetime, SHADER_UNIFORM_FLOAT);
    SetShaderValue(particleShader, timeLoc, &timeSinceLifeBegan, SHADER_UNIFORM_FLOAT);
    SetShaderValue(particleShader, fadeLoc, &fadeInt, SHADER_UNIFORM_INT);
    SetShaderValue(particleShader, glowLoc, &glowInt, SHADER_UNIFORM_INT);
    SetShaderValue(particleShader, glowIntensityLoc, &glowIntensity, SHADER_UNIFORM_FLOAT);

    BeginShaderMode(particleShader);
    //draw particle
    switch (shape)
    {
    case CIRCLE:
        DrawCircleV(position, side, color);
        break;
    case SQUARE:
        DrawRectangle(position.x - side * 0.5f, position.y - side * 0.5f, side, side, color);
        break;
    case TRIANGLE:
        DrawTriangle(v2, v1, v3, color);
        break;
    default:
        DrawCircleV(position, side, color);
    }

    EndShaderMode();
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
    //frameCount = 0;

    trail = {};

    if (s == TRIANGLE)
    {
        updateTriangleGeometry();
    }
}
