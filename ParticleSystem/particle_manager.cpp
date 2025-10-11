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
		result = checkCollissionSS(a, b);
	}

	return result;
}

void ParticleSystem::ParticleManager::resolveParticlePositions(ParticleSystem::Particle* a, ParticleSystem::Particle* b)
{
	ParticleShape s1 = a->getShape();
	ParticleShape s2 = b->getShape();

	if (s1 == CIRCLE && s2 == CIRCLE)
	{
		resolveCC(a, b);
	}
	else if ((s1 == CIRCLE && s2 == TRIANGLE) || (s1 == TRIANGLE && s2 == CIRCLE))
	{
		Particle* circle = (s1 == CIRCLE) ? a : b;
		Particle* triangle = (s1 == CIRCLE) ? b : a;
		resolveCT(circle, triangle);
	}
	else if ((s1 == CIRCLE && s2 == SQUARE) || (s1 == SQUARE && s2 == CIRCLE))
	{
		Particle* circle = (s1 == CIRCLE) ? a : b;
		Particle* square = (s1 == CIRCLE) ? b : a;
		resolveCS(circle, square);
	}
	else if (s1 == TRIANGLE && s2 == TRIANGLE)
	{
		resolveTT(a, b);
	}
	else if ((s1 == TRIANGLE && s2 == SQUARE) || (s1 == SQUARE && s2 == TRIANGLE))
	{
		Particle* triangle = (s1 == TRIANGLE) ? a : b;
		Particle* square = (s1 == TRIANGLE) ? b : a;
		resolveTS(triangle, square);
	}
	else if (s1 == SQUARE && s2 == SQUARE)
	{
		resolveSS(a, b);
	}
}

void ParticleSystem::ParticleManager::resolveParticleCollission(ParticleSystem::Particle* a, ParticleSystem::Particle* b)
{
	//TODO : IMPLEMENT SHAPE-BASED SIMPLE COLLISSIONS
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

	//TODO - fix normal based on shape-specific calculations
	Vector2 normal = Vector2Normalize(Vector2Subtract(b->getPos(), a->getPos()));

	//Position Correction
	resolveParticlePositions(a, b);

	//Velocity correction
	Vector2 newV1;
	Vector2 newV2;

	CollissionAlgo resolver = (c1 > c2) ? c2 : c1;
	Particle* dominant = (c1 > c2) ? b : a;
	switch (resolver)
	{
		case BOUNCE:
			BounceParticles(a, b, normal);
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
			BounceParticles(a, b, normal);

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

void ParticleSystem::ParticleManager::BounceParticles(Particle* a, Particle* b,Vector2 normal)
{
	ParticleShape s1 = a->getShape();
	ParticleShape s2 = b->getShape();

	if (s1 == CIRCLE && s2 == CIRCLE)
	{
		BounceCC(a, b, normal);
	}
	else if ((s1 == CIRCLE && s2 == TRIANGLE) || (s1 == TRIANGLE && s2 == CIRCLE))
	{
		Particle* circle = (s1 == CIRCLE) ? a : b;
		Particle* triangle = (s1 == CIRCLE) ? b : a;
		BounceCT(circle, triangle, normal);
	}
	else if ((s1 == CIRCLE && s2 == SQUARE) || (s1 == SQUARE && s2 == CIRCLE))
	{
		Particle* circle = (s1 == CIRCLE) ? a : b;
		Particle* square = (s1 == CIRCLE) ? b : a;
		BounceCS(circle, square, normal);
	}
	else if (s1 == TRIANGLE && s2 == TRIANGLE)
	{
		BounceTT(a, b, normal);
	}
	else if ((s1 == TRIANGLE && s2 == SQUARE) || (s1 == SQUARE && s2 == TRIANGLE))
	{
		Particle* triangle = (s1 == TRIANGLE) ? a : b;
		Particle* square = (s1 == TRIANGLE) ? b : a;
		BounceTS(triangle, square, normal);
	}
	else if (s1 == SQUARE && s2 == SQUARE)
	{
		BounceSS(a, b, normal);
	}
}

void ParticleSystem::ParticleManager::BounceCC(Particle* c1, Particle* c2, Vector2 normal)
{
	Vector2 v1 = c1->getVelocity();
	Vector2 v2 = c2->getVelocity();
	Vector2 newV1, newV2;
	newV1 = Vector2Subtract(v1, Vector2Scale(normal, 2 * Vector2DotProduct(normal, v1)));
	newV2 = Vector2Subtract(v2, Vector2Scale(normal, 2 * Vector2DotProduct(normal, v2)));
	newV1 = Vector2Scale(newV1, bounceFactor);
	newV2 = Vector2Scale(newV2, bounceFactor);
	c1->setVelocity(newV1);
	c2->setVelocity(newV2);
}

void ParticleSystem::ParticleManager::BounceSS(Particle* s1, Particle* s2, Vector2 normal)
{
	//TODO
	BounceCC(s1, s2, normal);
}

void ParticleSystem::ParticleManager::BounceTT(Particle* t1, Particle* t2, Vector2 normal)
{
	//TODO
	BounceCC(t1, t2, normal);
}

void ParticleSystem::ParticleManager::BounceCT(Particle* c, Particle* t, Vector2 normal)
{
	//TODO
	BounceCC(c, t, normal);
}

void ParticleSystem::ParticleManager::BounceCS(Particle* c, Particle* s, Vector2 normal)
{
	//TODO
	BounceCC(c, s, normal);
}

void ParticleSystem::ParticleManager::BounceTS(Particle* t, Particle* s, Vector2 normal)
{
	//TODO
	BounceCC(t, s, normal);
}

void ParticleSystem::ParticleManager::stickParticles(Particle* a, Particle* b)
{
	ParticleShape s1 = a->getShape();
	ParticleShape s2 = b->getShape();

	if (s1 == CIRCLE && s2 == CIRCLE)
	{
		StickCC(a, b);
	}
	else if ((s1 == CIRCLE && s2 == TRIANGLE) || (s1 == TRIANGLE && s2 == CIRCLE))
	{
		Particle* circle = (s1 == CIRCLE) ? a : b;
		Particle* triangle = (s1 == CIRCLE) ? b : a;
		StickCT(circle, triangle);
	}
	else if ((s1 == CIRCLE && s2 == SQUARE) || (s1 == SQUARE && s2 == CIRCLE))
	{
		Particle* circle = (s1 == CIRCLE) ? a : b;
		Particle* square = (s1 == CIRCLE) ? b : a;
		StickCS(circle, square);
	}
	else if (s1 == TRIANGLE && s2 == TRIANGLE)
	{
		StickTT(a, b);
	}
	else if ((s1 == TRIANGLE && s2 == SQUARE) || (s1 == SQUARE && s2 == TRIANGLE))
	{
		Particle* triangle = (s1 == TRIANGLE) ? a : b;
		Particle* square = (s1 == TRIANGLE) ? b : a;
		StickTS(triangle, square);
	}
	else if (s1 == SQUARE && s2 == SQUARE)
	{
		StickSS(a, b);
	}
}

void ParticleSystem::ParticleManager::StickCC(Particle* c1, Particle* c2)
{
	Vector2 Vavg = Vector2Scale(Vector2Add(c1->getVelocity(), c2->getVelocity()), 0.5f);
	c1->setVelocity(Vavg);
	c2->setVelocity(Vavg);
}

void ParticleSystem::ParticleManager::StickSS(Particle* s1, Particle* s2)
{
	//TODO
	StickCC(s1, s2);
}

void ParticleSystem::ParticleManager::StickTT(Particle* t1, Particle* t2)
{
	//TODO
	StickCC(t1, t2);
}

void ParticleSystem::ParticleManager::StickCT(Particle* c, Particle* t)
{
	//TODO
	StickCC(c, t);
}

void ParticleSystem::ParticleManager::StickCS(Particle* c, Particle* s)
{
	//TODO
	StickCC(c, s);
}

void ParticleSystem::ParticleManager::StickTS(Particle* t, Particle* s)
{
	//TODO
	StickCC(t, s);
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

void ParticleSystem::ParticleManager::resolveCC(ParticleSystem::Particle* c1, ParticleSystem::Particle* c2)
{
	int r1 = c1->getSide();
	int r2 = c2->getSide();
	Vector2 p1 = c1->getPos();
	Vector2 p2 = c2->getPos();
	Vector2 v1 = c1->getVelocity();
	Vector2 v2 = c2->getVelocity();

	float dist = Vector2Distance(p1, p2);
	Vector2 normal = Vector2Normalize(Vector2Subtract(p2,p1));
	float overlap = (r1 + r2) - dist;
	float extraSep = 1.00f;
	overlap += extraSep;

	//relative velocity
	Vector2 Vr = Vector2Subtract(v2, v1);
	float Vn = Vector2DotProduct(Vr, normal);

	if(overlap > 0)
	{
		//Update Pos
		c1->setPos(Vector2Subtract(p1, Vector2Scale(normal, overlap / 2)));
		c2->setPos(Vector2Add(p2, Vector2Scale(normal, overlap / 2)));

		//Add a little push force
		if(Vn < 0)
		{
			float pushF = 5.0f;
			c1->setVelocity(Vector2Subtract(v1, Vector2Scale(normal, pushF)));
			c2->setVelocity(Vector2Add(v2, Vector2Scale(normal, pushF)));
		}
	}
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

void ParticleSystem::ParticleManager::resolveCS(ParticleSystem::Particle* c, ParticleSystem::Particle* s)
{
	//TODO
	resolveCC(c, s);
}

bool ParticleSystem::ParticleManager::checkCollissionCT(ParticleSystem::Particle* c, ParticleSystem::Particle* t)
{
	//TODO
	return checkCollissionCC(c, t);
}

void ParticleSystem::ParticleManager::resolveCT(ParticleSystem::Particle* c, ParticleSystem::Particle* t)
{
	//TODO
	resolveCC(c, t);
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

void ParticleSystem::ParticleManager::resolveTT(ParticleSystem::Particle* t1, ParticleSystem::Particle* t2)
{
	//TODO
	resolveCC(t1, t2);
}

bool ParticleSystem::ParticleManager::checkCollissionTS(ParticleSystem::Particle* t, ParticleSystem::Particle* s)
{
	//TODO
	return checkCollissionCC(s, t);
}

void ParticleSystem::ParticleManager::resolveTS(ParticleSystem::Particle* t, ParticleSystem::Particle* s)
{
	//TODO
	resolveCC(t, s);
}

bool ParticleSystem::ParticleManager::checkCollissionSS(ParticleSystem::Particle* s1, ParticleSystem::Particle* s2)
{
	int a = s1->getSide();
	int b = s2->getSide();
	Vector2 dir = Vector2Subtract(s2->getPos(), s1->getPos());
	float halfsum = (a + b) * 0.5f;
	
	return (fabs(dir.x) <= halfsum && fabs(dir.y) <= halfsum);
}

void ParticleSystem::ParticleManager::resolveSS(ParticleSystem::Particle* s1, ParticleSystem::Particle* s2)
{
	//TODO
	resolveCC(s1, s2);
}
