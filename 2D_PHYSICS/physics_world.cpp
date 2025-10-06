#include "physics_world.h"

vector<PhysicsObject*>& PhysicsWorld::getObjects()
{
	return objects;
}

Vector2 PhysicsWorld::getGravity()
{
	return gravity;
}

COLLISSION_ALGO PhysicsWorld::getCollissionAlgo()
{
	return algo;
}

void PhysicsWorld::addObject(PhysicsObject* obj)
{
	objects.push_back(obj);
}

void PhysicsWorld::removeObject(PhysicsObject* obj)
{
	objects.erase(remove(objects.begin(), objects.end(), obj), objects.end());
}

void PhysicsWorld::setGravity(Vector2 g)
{
	gravity = g;
}

void PhysicsWorld::setGravity(float x, float y)
{
	gravity = { x,y };
}

void PhysicsWorld::setCollissionAlgo(COLLISSION_ALGO c)
{
	algo = c;
}

bool PhysicsWorld::checkCircleCircle(PhysicsObject* circle1, PhysicsObject* circle2)
{
	return CheckCollisionCircles(circle1->getPos(), circle1->getRadius(), circle2->getPos(), circle2->getRadius());
}

bool PhysicsWorld::checkCircleRect(PhysicsObject* circle, PhysicsObject* rectangle)
{
	Rectangle r{
		rectangle->getPos().x - rectangle->getWidth() / 2,
		rectangle->getPos().y - rectangle->getHeight() / 2,
		rectangle->getWidth(),
		rectangle->getHeight()
	};

	return CheckCollisionCircleRec(circle->getPos(), circle->getRadius(), r);
}

bool PhysicsWorld::checkRectRect(PhysicsObject* rectangle1, PhysicsObject* rectangle2)
{
	Vector2 pos1 = rectangle1->getPos();
	Vector2 pos2 = rectangle2->getPos();

	Rectangle a{
		pos1.x - rectangle1->getWidth() / 2,
		pos1.y - rectangle1->getHeight() / 2,
		rectangle1->getWidth(),
		rectangle1->getHeight()
	};
	Rectangle b{
		pos2.x - rectangle2->getWidth() / 2,
		pos2.y - rectangle2->getHeight() / 2,
		rectangle2->getWidth(),
		rectangle2->getHeight()
	};

	return CheckCollisionRecs(a, b);
}

void PhysicsWorld::resolveCircleCircle(PhysicsObject* circle1, PhysicsObject* circle2)
{
	Vector2 pos1 = circle1->getPos();
	Vector2 pos2 = circle2->getPos();

	float dist = Vector2Distance(circle1->getPos(), circle2->getPos());
	if (dist == 0) return; //on top of each other

	Vector2 normal = Vector2Normalize(Vector2Subtract(pos2, pos1));

	float overlap = (circle1->getRadius() + circle2->getRadius()) - dist;

	if (!circle1->getIsStatic() && !circle2->getIsStatic())
	{
		//move both
		Vector2 sep = Vector2Scale(normal, overlap / 2);
		circle1->setPos(Vector2Subtract(pos1, sep));
		circle2->setPos(Vector2Add(pos2, sep));
	}
	else if (!circle1->getIsStatic())
	{
		//move circle 1
		Vector2 sep = Vector2Scale(normal, overlap);
		circle1->setPos(Vector2Subtract(pos1, sep));
	}
	else if (!circle2->getIsStatic())
	{
		//move circle2
		Vector2 sep = Vector2Scale(normal, overlap);
		circle2->setPos(Vector2Add(pos2, sep));
	}

	//relative vel
	Vector2 vr = Vector2Subtract(circle2->getVelocity(), circle1->getVelocity());
	float vn = Vector2DotProduct(vr, normal); //velocity along normal

	if (vn > 0) return; //moving away from each other

	float restitution = 1;
	switch (algo)
	{
		case MINIMUM:
			restitution = fmin(circle1->getRestitution(), circle2->getRestitution());
			break;
		case AVERAGE:
			restitution = (circle1->getRestitution() + circle2->getRestitution()) / 2;
			break;
		case GEOMETRIC_MEAN:
			restitution = sqrt(circle1->getRestitution() * circle2->getRestitution());
			break;
	}
	restitution = fminf(fmaxf(restitution, 0.0f), 1.0f);

	//impulse scalar
	float invM1 = (circle1->getIsStatic()) ? 0.0f : circle1->getInvMass();
	float invM2 = (circle2->getIsStatic()) ? 0.0f : circle2->getInvMass();
	float invSum = invM1 + invM2;
	if (invSum == 0)return;
	float J = -(1 + restitution) * vn;
	J /= invSum;

	//vector impulse
	Vector2 impulse = Vector2Scale(normal, J);

	if (!circle1->getIsStatic())
	{
		Vector2 newVel = Vector2Subtract(circle1->getVelocity(), Vector2Scale(impulse, invM1));
		circle1->setVelocity(newVel);
	}
	if (!circle2->getIsStatic())
	{
		Vector2 newVel = Vector2Add(circle2->getVelocity(), Vector2Scale(impulse, invM2));
		circle2->setVelocity(newVel);
	}
}

void PhysicsWorld::resolveCircleRect(PhysicsObject* circle, PhysicsObject* rectangle)
{
	Vector2 cpos = circle->getPos();
	Vector2 rpos = rectangle->getPos();

	float hw = rectangle->getWidth() / 2; //Half width
	float hh = rectangle->getHeight() / 2; // Half height

	float closestX = fmax(rpos.x - hw, fmin(rpos.x + hw, cpos.x));
	float closestY = fmax(rpos.y - hh, fmin(rpos.y + hh, cpos.y));

	Vector2 closest = { closestX, closestY };

	Vector2 delta = Vector2Subtract(cpos, closest);
	float dist = Vector2Length(delta);

	if (dist == 0)
	{
		//circle centre in rectangle
		float dx = cpos.x - rpos.x;
		float dy = cpos.y - rpos.y;

		if (fabs(dx) > fabs(dy))
		{
			delta = { dx > 0 ? 1.0f : -1.0f, 0 };
		}
		else
		{
			delta = { 0, dy > 0 ? 1.0f : -1.0f };
		}
		dist = 0.1f;
	}

	Vector2 normal = Vector2Normalize(delta);
	float overlap = circle->getRadius() - dist;

	//move the objects
	if (!circle->getIsStatic())
	{
		circle->setPos(Vector2Add(cpos, Vector2Scale(normal, overlap)));
	}
	if (!rectangle->getIsStatic())
	{
		rectangle->setPos(Vector2Subtract(rpos, Vector2Scale(normal, overlap)));
	}

	//relative vel
	Vector2 vr = Vector2Subtract(circle->getVelocity() , rectangle->getVelocity());
	float vn = Vector2DotProduct(vr, normal); //vel along normal

	if (vn > 0) return;

	float restitution = 1;
	switch (algo)
	{
		case MINIMUM:
			restitution = fmin(circle->getRestitution(), rectangle->getRestitution());
			break;
		case AVERAGE:
			restitution = (circle->getRestitution() + rectangle->getRestitution()) / 2;
			break;
		case GEOMETRIC_MEAN:
			restitution = sqrt(circle->getRestitution() * rectangle->getRestitution());
			break;
	}
	restitution = fminf(fmaxf(restitution, 0.0f), 1.0f);

	//impulse
	float invM1 = (circle->getIsStatic()) ? 0.0f : circle->getInvMass();
	float invM2 = (rectangle->getIsStatic()) ? 0.0f : rectangle->getInvMass();
	float invSum = invM1 + invM2;
	if (invSum == 0)return;
	float J = -(1 + restitution) * vn;
	J /= invSum;

	//vector impulse
	Vector2 impulse = Vector2Scale(normal, J);

	//resolve velocities
	if (!circle->getIsStatic())
	{
		Vector2 newVel = Vector2Add(circle->getVelocity(), Vector2Scale(impulse, invM1));
		circle->setVelocity(newVel);
	}
	if (!rectangle->getIsStatic())
	{
		Vector2 newVel = Vector2Subtract(rectangle->getVelocity(), Vector2Scale(impulse, invM2));
		rectangle->setVelocity(newVel);
	}
}

void PhysicsWorld::resolveRectRect(PhysicsObject* rectangle1, PhysicsObject* rectangle2)
{
	Vector2 pos1 = rectangle1->getPos();
	Vector2 pos2 = rectangle2->getPos();
	float dx = (pos2.x > pos1.x) ? 1.0 : -1.0;
	float dy = (pos2.y > pos1.y) ? 1.0f : -1.0f;

	float overlapX = (rectangle1->getWidth() + rectangle2->getWidth()) / 2 - fabs(pos2.x - pos1.x);
	float overlapY = (rectangle1->getHeight() + rectangle2->getHeight()) / 2 - fabs(pos2.y - pos1.y);

	//push along the axis of least overlap
	if (overlapX < overlapY)
	{
		//move along x-axis

		if (!rectangle1->getIsStatic() && !rectangle2->getIsStatic())
		{
			//move both
			rectangle1->setPos({ pos1.x - dx * overlapX / 2, pos1.y });
			rectangle2->setPos({ pos2.x + dx * overlapX / 2, pos2.y });
		}
		else if (!rectangle1->getIsStatic())
		{
			//only move r1
			rectangle1->setPos({ pos1.x - dx * overlapX,pos1.y });
		}
		else if (!rectangle2->getIsStatic())
		{
			//only move r2
			rectangle2->setPos({ pos2.x + dx * overlapX, pos2.y });
		}

		//Bounce
		//if (!rectangle1->getIsStatic()) rectangle1->setVelocity({ -rectangle1->getVelocity().x * restitution, rectangle1->getVelocity().y });
		//if (!rectangle2->getIsStatic()) rectangle2->setVelocity({ -rectangle2->getVelocity().x * restitution, rectangle2->getVelocity().y });
	}
	else
	{
		//move along y-axis

		if (!rectangle1->getIsStatic() && !rectangle2->getIsStatic())
		{
			//move both
			rectangle1->setPos({ pos1.x, pos1.y - dy * overlapY / 2 });
			rectangle2->setPos({ pos2.x, pos2.y + dy * overlapY / 2 });
		}
		else if (!rectangle1->getIsStatic())
		{
			//only move r1
			rectangle1->setPos({ pos1.x, pos1.y - dy * overlapY });
		}
		else if (!rectangle2->getIsStatic())
		{
			//only move r2
			rectangle2->setPos({ pos2.x, pos2.y + dy * overlapY });
		}

		//Bounce
		//if (!rectangle1->getIsStatic()) rectangle1->setVelocity({ rectangle1->getVelocity().x, -rectangle1->getVelocity().y * restitution });
		//if (!rectangle2->getIsStatic()) rectangle2->setVelocity({ rectangle2->getVelocity().x, -rectangle2->getVelocity().y * restitution });
	}

	//relative vel
	Vector2 normal;
	if (overlapX < overlapY)
	{
		normal = { dx, 0 };
	}
	else
	{
		normal = { 0, dy };
	}
	Vector2 vr = Vector2Subtract(rectangle2->getVelocity(), rectangle1->getVelocity());
	float vn = Vector2DotProduct(vr, normal);
	if (vn > 0) return;

	//restitutions
	float resA = rectangle1->getRestitution();
	float resB = rectangle2->getRestitution();
	float restitution = 1;
	switch (algo)
	{
		case MINIMUM:
			restitution = fmin(resA, resB);
			break;
		case AVERAGE:
			restitution = (resA + resB) / 2;
			break;
		case GEOMETRIC_MEAN:
			restitution = sqrt(resA * resB);
			break;
	}
	restitution = fminf(fmaxf(restitution, 0.0f), 1.0f);

	//impulse scalar
	float invM1 = (rectangle1->getIsStatic()) ? 0.0f : rectangle1->getInvMass();
	float invM2 = (rectangle2->getIsStatic()) ? 0.0f : rectangle2->getInvMass();
	float invSum = invM1 + invM2;
	if (invSum == 0)return;
	float J = -(1 + restitution) * vn;
	J /= invSum;

	//vector impulse
	Vector2 impulse = Vector2Scale(normal, J);

	//Bounce
	if (!rectangle1->getIsStatic())
	{
		Vector2 newVel = Vector2Subtract(rectangle1->getVelocity(), Vector2Scale(impulse, invM1));
		rectangle1->setVelocity(newVel);
	}
	if (!rectangle2->getIsStatic())
	{
		Vector2 newVel = Vector2Add(rectangle2->getVelocity(), Vector2Scale(impulse, invM2));
		rectangle2->setVelocity(newVel);
	}
}

void PhysicsWorld::update(float dt)
{
	//run update <substeps> times per frame
	substeps = Clamp((int)(dt / 0.004f), 1, 8);
	dt = dt / substeps;
	for(int i = 0; i < substeps; i++)
	{
		//Apply Forces
		for (auto i : objects)
		{
			if (!i->getIsStatic())
			{
				Vector2 force = Vector2Scale(gravity, i->getMass());
				i->ApplyForce(force);
			}
		}

		//Update objects
		for (auto i : objects)
		{
			i->update(dt);
		}

		//Detect and resolve collisions
		for (size_t i = 0; i < objects.size(); i++)
		{
			for (size_t j = i + 1; j < objects.size(); j++)
			{
				PhysicsObject* a = objects[i];
				PhysicsObject* b = objects[j];

				if (a->getIsStatic() && b->getIsStatic()) continue;

				if (a->getShape() == CIRCLE && b->getShape() == CIRCLE)
				{
					if (checkCircleCircle(a, b))
					{
						resolveCircleCircle(a, b);
					}
				}
				else if ((a->getShape() == CIRCLE && b->getShape() == RECTANGLE) || (a->getShape() == RECTANGLE && b->getShape() == CIRCLE))
				{
					PhysicsObject* c = (a->getShape() == CIRCLE) ? a : b;
					PhysicsObject* r = (a->getShape() == CIRCLE) ? b : a;

					if (checkCircleRect(c, r))
					{
						resolveCircleRect(c, r);
					}
				}
				else if (a->getShape() == RECTANGLE && b->getShape() == RECTANGLE)
				{
					if (checkRectRect(a, b))
					{
						resolveRectRect(a, b);
					}
				}
			}
		}
	}
}

void PhysicsWorld::createPhysicsObject(SHAPE s, float d1, float d2, Vector2 pos, float m, Color c, bool st, float res)
{
	PhysicsObject* temp = new PhysicsObject(s, d1, d2, pos, m, c, st, res);
	addObject(temp);
}

void PhysicsWorld::createPhysicsObject(SHAPE s, float d1, Vector2 pos, float m, Color c, bool st, float res)
{
	PhysicsObject* temp = new PhysicsObject(s, d1, pos, m, c, st, res);
	addObject(temp);
}

void PhysicsWorld::draw()
{
	for (auto i : objects)
	{
		i->Draw();
	}
}
