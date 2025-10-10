#include "particle_manager.h"

std::vector<ParticleSystem::Particle*> ParticleSystem::ParticleManager::getParticles() const
{
	return particles;
}

bool ParticleSystem::ParticleManager::getIsMeantForDeletion(Particle* p)
{
	return std::find(particlesToDelete.begin(), particlesToDelete.end(), p) != particlesToDelete.end();
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

bool ParticleSystem::ParticleManager::checkParticleCollission(ParticleSystem::Particle* a, ParticleSystem::Particle* b)
{
	bool result = false;

	ParticleShape s1 = a->getShape();
	ParticleShape s2 = b->getShape();

	if (s1 == CIRCLE && s2 == CIRCLE)
	{
		result = checkCollissionCC(a, b);
	}
	else if ((s1 == CIRCLE && s2 == TRIANGLE) || (s1 == TRIANGLE && s2 == CIRCLE))
	{
		Particle* circle = (s1 == CIRCLE) ? a : b;
		Particle* triangle = (s1 == CIRCLE) ? b : a;
		result = checkCollissionCT(circle, triangle);
	}
	else if ((s1 == CIRCLE && s2 == SQUARE) || (s1 == SQUARE && s2 == CIRCLE))
	{
		Particle* circle = (s1 == CIRCLE) ? a : b;
		Particle* square = (s1 == CIRCLE) ? b : a;
		result = checkCollissionCS(circle, square);
	}
	else if (s1 == TRIANGLE && s2 == TRIANGLE)
	{
		result = checkCollissionTT(a, b);
	}
	else if ((s1 == TRIANGLE && s2 == SQUARE) || (s1 == SQUARE && s2 == TRIANGLE))
	{
		Particle* triangle = (s1 == TRIANGLE) ? a : b;
		Particle* square = (s1 == TRIANGLE) ? b : a;
		result = checkCollissionTS(triangle, square);
	}
	else if (s1 == SQUARE && s2 == SQUARE)
	{
		result = checkCollissionCC(a, b);
	}

	return result;
}

void ParticleSystem::ParticleManager::resolveParticleCollission(ParticleSystem::Particle* a, ParticleSystem::Particle* b)
{
	CollissionAlgo c1 = a->getCollissionResponse();
	CollissionAlgo c2 = b->getCollissionResponse();
	if (c1 == DESTROY || c2 == DESTROY) 
	{
		if (std::find(particlesToDelete.begin(), particlesToDelete.end(), a) == particlesToDelete.end())
			particlesToDelete.push_back(a);
		if (std::find(particlesToDelete.begin(), particlesToDelete.end(), b) == particlesToDelete.end()) 
			particlesToDelete.push_back(b);
		return;
	}
	if (c1 == FLOW || c2 == FLOW) return;

	int r1 = a->getSide();
	int r2 = b->getSide();
	Vector2 p1 = a->getPos();
	Vector2 p2 = b->getPos();
	Vector2 v1 = a->getVelocity();
	Vector2 v2 = b->getVelocity();

	float dist = Vector2Distance(p1, p2);
	Vector2 normal = Vector2Normalize(p2 - p1);
	float overlap = (r1 + r2) - dist;

	//Update Pos
	a->setPos(Vector2Subtract(p1, Vector2Scale(normal, overlap / 2)));
	b->setPos(Vector2Add(p2, Vector2Scale(normal, overlap / 2)));

	//Relative velocity
	Vector2 newV1;
	Vector2 newV2;
	
	CollissionAlgo resolver = (c1 > c2) ? c2 : c1;
	Particle* dominant = (c1 > c2) ? b : a;
	switch (resolver)
	{
		case BOUNCE:
			BounceParticles(a, b, normal, bounceFactor);
			break;
		case CONVERT:
			convertParticle((dominant == a) ? b : a, dominant);
			break;
		case ABSORB:
			absorbParticle(dominant, (dominant == a) ? b : a);
			break;
		case REPEL:
			repelParticles(a, b, normal);
			break;
		case STICK:
			stickParticles(a, b);
			break;
		default:
			BounceParticles(a, b, normal, bounceFactor);

	}
}

void ParticleSystem::ParticleManager::createParticle(ParticleSystem::Particle* p)
{
	particles.push_back(p);
}

void ParticleSystem::ParticleManager::createParticle(ParticleShape s, int side, float lifetime,Color c, Vector2 pos, Vector2 vel, CollissionAlgo response)
{
	ParticleSystem::Particle* temp = new ParticleSystem::Particle(s, side, lifetime, c, pos, vel, response);
	particles.push_back(temp);
}

void ParticleSystem::ParticleManager::update(float dt)
{
	//Particle updation and deletion
	for (auto& i : particles)
	{
		i->update(dt);
		if (i->getTimeSinceLifeBegan() >= i->getLifetime()) particlesToDelete.push_back(i);
	}
	for (auto& i : particlesToDelete) removeParticle(i);
	particlesToDelete.clear();

	//Particle Collission
	int numPars = particles.size();
	for (int i = 0; i < numPars; i++)
	{
		for (int j = i + 1; j < numPars; j++)
		{
			if (getIsMeantForDeletion(particles[j]) || getIsMeantForDeletion(particles[i])) continue;

			if (checkParticleCollission(particles[i], particles[j]))
			{
				resolveParticleCollission(particles[i], particles[j]);
			}
		}
	}
	for (auto& i : particlesToDelete) removeParticle(i);
	particlesToDelete.clear();
}

void ParticleSystem::ParticleManager::draw()
{
	for (auto& i : particles)
	{
		i->draw();
	}
}

void ParticleSystem::ParticleManager::convertParticle(Particle* from, Particle* to)
{
	from->setCollissionResponse(to->getCollissionResponse());
	from->setColor(to->getColor());
	from->setLifetime(to->getLifetime());
	from->setSide(to->getSide());
	from->setShape(to->getShape());

	from->resetParticle();
}

void ParticleSystem::ParticleManager::absorbParticle(Particle* absorber, Particle* absorbed)
{
	int r1 = absorber->getSide();
	int r2 = absorbed->getSide();
	absorber->setSide((int)sqrt(r1 * r1 + r2 * r2));

	Vector2 v1 = absorber->getVelocity();
	Vector2 v2 = absorbed->getVelocity();
	absorber->setVelocity(Vector2Scale(Vector2Add(v1, v2), 0.5f));

	float t1 = absorber->getLifetime();
	float t2 = absorbed->getLifetime();
	absorber->setLifetime(t1 + t2 * 0.5f);

	if (!getIsMeantForDeletion(absorbed)) particlesToDelete.push_back(absorbed);
}

void ParticleSystem::ParticleManager::BounceParticles(Particle* a, Particle* b,Vector2 normal, float bounce_factor)
{
	Vector2 v1 = a->getVelocity();
	Vector2 v2 = b->getVelocity();
	Vector2 newV1, newV2;
	newV1 = Vector2Subtract(v1, Vector2Scale(normal, 2 * Vector2DotProduct(normal, v1)));
	newV2 = Vector2Subtract(v2, Vector2Scale(normal, 2 * Vector2DotProduct(normal, v2)));
	newV1 = Vector2Scale(newV1, bounceFactor);
	newV2 = Vector2Scale(newV2, bounceFactor);
	a->setVelocity(newV1);
	b->setVelocity(newV2);
}

void ParticleSystem::ParticleManager::stickParticles(Particle* a, Particle* b)
{
	Vector2 Vavg = Vector2Scale(Vector2Add(a->getVelocity(), b->getVelocity()), 0.5f);
	a->setVelocity(Vavg);
	b->setVelocity(Vavg);
}

void ParticleSystem::ParticleManager::repelParticles(Particle* a, Particle* b, Vector2 normal)
{
	Vector2 v1 = a->getVelocity();
	Vector2 v2 = b->getVelocity();

	float v1AlongNormal = Vector2DotProduct(v1, normal);
	float v2AlongNormal = Vector2DotProduct(v2, normal);

	// If moving toward each other, cancel that component
	if (v1AlongNormal < 0) {
		a->setVelocity(Vector2Subtract(v1, Vector2Scale(normal, v1AlongNormal)));
	}
	if (v2AlongNormal > 0) {
		b->setVelocity(Vector2Subtract(v2, Vector2Scale(normal, v2AlongNormal)));
	}

	float separationForce = 15.0f;
	a->setVelocity(Vector2Subtract(a->getVelocity(), Vector2Scale(normal, separationForce)));
	b->setVelocity(Vector2Add(b->getVelocity(), Vector2Scale(normal, separationForce)));
}

bool ParticleSystem::ParticleManager::checkCollissionCC(ParticleSystem::Particle* c1, ParticleSystem::Particle* c2)
{
	int r1 = c1->getSide();
	int r2 = c2->getSide();
	Vector2 p1 = c1->getPos();
	Vector2 p2 = c2->getPos();
	double dist = Vector2Distance(p1, p2);

	return dist <= r1 + r2;
}

bool ParticleSystem::ParticleManager::checkCollissionCS(ParticleSystem::Particle* c, ParticleSystem::Particle* s)
{
	int r = c->getSide();
	float halfa = s->getSide() / 2.0f;
	Vector2 p1 = c->getPos();
	Vector2 p2 = s->getPos();

	//closest point on square to circle's center
	float closestX = Clamp(p1.x, p2.x - halfa, p2.x + halfa);
	float closestY = Clamp(p1.y, p2.y - halfa, p2.y + halfa);

	//distance from circle's center
	float dx = p1.x - closestX;
	float dy = p1.y - closestY;

	return ((dx * dx + dy * dy) <= (r * r));
}

bool ParticleSystem::ParticleManager::checkCollissionCT(ParticleSystem::Particle* c, ParticleSystem::Particle* t)
{
	//TODO
	return false;
}

bool ParticleSystem::ParticleManager::checkCollissionTT(ParticleSystem::Particle* t1, ParticleSystem::Particle* t2)
{
	//Loop over both triangles->each 3 normals -> project each 3 vertices -> check overlap
	for(int k = 0; k < 2; k++)
	{
		//Select a triangle
		Particle* p = (k == 0) ? t1 : t2;
		for (int i = 1; i < 4; i++)
		{
			//choose an axis
			Vector2 axis = p->getTriangleNormal(i);
			float triangle1[3];
			float triangle2[3];

			for (int j = 1; j < 4; j++)
			{ 
				triangle1[j - 1] = Vector2DotProduct(t1->getTriangleVertex(j), axis);
				triangle2[j - 1] = Vector2DotProduct(t2->getTriangleVertex(j), axis);
			}

			float minT1 = fmin(triangle1[0], fmin(triangle1[1], triangle1[2]));
			float maxT1 = fmax(triangle1[0], fmax(triangle1[1], triangle1[2]));
			float minT2 = fmin(triangle2[0], fmin(triangle2[1], triangle2[2]));
			float maxT2 = fmax(triangle2[0], fmax(triangle2[1], triangle2[2]));

			//if any side doesn't overlap -> no collission
			if ((maxT2 < minT1) || (maxT1 < minT2)) return false;
		}
	}

	return true;
}

bool ParticleSystem::ParticleManager::checkCollissionTS(ParticleSystem::Particle* t, ParticleSystem::Particle* s)
{
	//TODO
	return false;
}

bool ParticleSystem::ParticleManager::checkCollissionSS(ParticleSystem::Particle* s1, ParticleSystem::Particle* s2)
{
	int a = s1->getSide();
	int b = s2->getSide();
	Vector2 dir = Vector2Subtract(s2->getPos(), s1->getPos());
	float halfsum = (a + b) * 0.5f;
	
	return (fabs(dir.x) <= halfsum || fabs(dir.y) <= halfsum);
}
