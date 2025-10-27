#include "particle.h"

void ParticleSystem::Particle::loadShader()
{
    if (!isParticleShaderLoaded)
    {
        particleShader = LoadShader("particle_vert.glsl", "particle_frag.glsl");
        isParticleShaderLoaded = true;
    }

    fadeLoc = GetShaderLocation(particleShader, "fadeEnabled");
    glowLoc = GetShaderLocation(particleShader, "glowEnabled");
    glowIntensityLoc = GetShaderLocation(particleShader, "glowIntensity");
    mvpLoc = GetShaderLocation(particleShader, "mvpMat");

    std::cout <<"fadeloc : " << fadeLoc << ", glowloc : " << glowLoc << ", GIloc : " << glowIntensityLoc<<", mvpLoc : "<<mvpLoc<<'\n';
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

std::deque<Vector2>& ParticleSystem::Particle::getTrail()
{
    return trail;
}

float ParticleSystem::Particle::getSide() const
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

void ParticleSystem::Particle::drawTrail()
{
    float lifePercent = timeSinceLifeBegan / lifetime;
    float fadeVal = (fadeEffect) ? 1.0 - lifePercent : 1.0f;

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
}

void ParticleSystem::Particle::drawParticle()
{
    //BeginShaderMode(particleShader);
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

    //EndShaderMode();
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

Mesh ParticleSystem::Particle::generateCircleMesh(int segments)
{
    int vCount = segments + 1; //vertex count
    int tCount = segments; //triangle count

    Mesh circle = { 0 };
    circle.vertexCount = vCount;
    circle.triangleCount = tCount;

    //allocate memory for vertex data
    circle.vertices = (float*)MemAlloc(vCount * 3 * sizeof(float)); //x,y,z per vertex
    circle.indices = (unsigned short*)MemAlloc(tCount * 3 * sizeof(unsigned short)); //3 indices per triangle
    circle.texcoords = (float*)MemAlloc(vCount * 2 * sizeof(float)); //u,v per vertex
    circle.colors = (unsigned char*)MemAlloc(vCount * 4 * sizeof(unsigned char)); //r,g,b,a per vertex

    //default vertex center values
    circle.vertices[0] = 0.0; //x
    circle.vertices[1] = 0.0; //y
    circle.vertices[2] = 0.0; //z
    circle.texcoords[0] = 0.5; //u
    circle.texcoords[1] = 0.5; //v

    //Generate vertices in a circle
    for (int i = 0; i < segments; i++)
    {
        float angle = (float)i / segments * PI * 2;
        int vi = (i + 1) * 3; //vertex index
        int ti = (i + 1) * 2; //texture index

        //Positions (r = 1)
        circle.vertices[vi + 0] = cosf(angle); //x
        circle.vertices[vi + 1] = sinf(angle); //y
        circle.vertices[vi + 2] = 0.0; //z

        //UV coords
        circle.texcoords[ti + 0] = 0.5f + 0.5f * cosf(angle); //u
        circle.texcoords[ti + 1] = 0.5f + 0.5f * sinf(angle); //v

        //Triangle indices
        circle.indices[i * 3 + 0] = 0; //center vertex
        circle.indices[i * 3 + 1] = i+1; //current edge vertex
        circle.indices[i * 3 + 2] = (i+1)%segments + 1; //next edge vertex(wrapping)
    }

    UploadMesh(&circle, false);
    return circle;
}

Mesh ParticleSystem::Particle::generateSquareMesh()
{
    Mesh square = { 0 };
    square.vertexCount = 4;
    square.triangleCount = 2;

    //Allocate memory for vertex data
    square.vertices = (float*)MemAlloc(12 * sizeof(float));
    square.texcoords = (float*)MemAlloc(8 * sizeof(float));
    square.colors = (unsigned char*)MemAlloc(16 * sizeof(unsigned char));
    square.indices = (unsigned short*)MemAlloc(6 * sizeof(unsigned short));

    //Vertex Positions
    square.vertices[0] = -0.5; square.vertices[1] =  -0.5; square.vertices[2] =  0.0;
    square.vertices[3] = -0.5; square.vertices[4] =   0.5; square.vertices[5] =  0.0;
    square.vertices[6] =  0.5; square.vertices[7] =   0.5; square.vertices[8] =  0.0;
    square.vertices[9] =  0.5; square.vertices[10] = -0.5; square.vertices[11] = 0.0;

    //UV coords
    square.texcoords[0] = 0.0; square.texcoords[1] = 0.0;
    square.texcoords[2] = 0.0; square.texcoords[3] = 1.0;
    square.texcoords[4] = 1.0; square.texcoords[5] = 1.0;
    square.texcoords[6] = 1.0; square.texcoords[7] = 0.0;

    //indices
    square.indices[0] = 0; square.indices[1] = 1; square.indices[2] = 3;
    square.indices[3] = 1; square.indices[4] = 2; square.indices[5] = 3;

    UploadMesh(&square, false);
    return square;
}

Mesh ParticleSystem::Particle::generateTriangleMesh()
{
    Mesh triangle = { 0 };
    triangle.vertexCount = 3;
    triangle.triangleCount = 1;
    
    //vertex data memory allocation
    triangle.vertices = (float*)MemAlloc(9 * sizeof(float));
    triangle.texcoords = (float*)MemAlloc(6 * sizeof(float));
    triangle.colors = (unsigned char*)MemAlloc(12 * sizeof(unsigned char));
    triangle.indices = (unsigned short*)MemAlloc(3 * sizeof(unsigned short));

    //x,y,z
    triangle.vertices[0] = -0.5; triangle.vertices[1] = -0.5  * oneOverRoot3; triangle.vertices[2] = 0.0;
    triangle.vertices[3] =  0.0; triangle.vertices[4] =  1.25 * oneOverRoot3; triangle.vertices[5] = 0.0;
    triangle.vertices[6] =  0.5; triangle.vertices[7] = -0.5  * oneOverRoot3; triangle.vertices[8] = 0.0;

    //u,v
    triangle.texcoords[0] = 0.0; triangle.texcoords[1] = 0.0;
    triangle.texcoords[2] = 0.5; triangle.texcoords[3] = 0.5 * sqrt(3);
    triangle.texcoords[4] = 0.0; triangle.texcoords[5] = 1.0;

    //indices
    triangle.indices[0] = 0; triangle.indices[1] = 1; triangle.indices[2] = 2;

    UploadMesh(&triangle, false);
    return triangle;
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
