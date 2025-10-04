#include "physics_world.h"

vector<PhysicsObject*> PhysicsWorld::getObjects()
{
	return objects;
}

Vector2 PhysicsWorld::getGravity()
{
	return gravity;
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

void PhysicsWorld::update(float dt)
{
	for (auto i : objects)
	{
		if (!i->getIsStatic())
		{
			Vector2 force = Vector2Scale(gravity, i->getMass());
			i->ApplyForce(force);
		}
	}

	for (auto i : objects) 
	{
		i->update(dt);
	}
}

void PhysicsWorld::draw()
{
	for (auto i : objects)
	{
		i->Draw();
	}
}
