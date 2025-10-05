#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include"includes.cpp"
#include"physics_object.h"

class PhysicsWorld
{
private:
	vector<PhysicsObject*> objects;
	Vector2 gravity = { 0, 980 };

public:
//getters

	vector<PhysicsObject*> getObjects();
	Vector2 getGravity();

//setters
	void addObject(PhysicsObject* obj);
	void removeObject(PhysicsObject* obj);
	void setGravity(Vector2 g);
	void setGravity(float x, float y);

//Collision Detection
	bool checkCircleCircle(PhysicsObject* circle1, PhysicsObject* circle2);
	bool checkCircleRect(PhysicsObject* circle, PhysicsObject* rectangle);
	bool checkRectRect(PhysicsObject* rectangle1, PhysicsObject* rectangle2);

//Collision Resolver
	void resolveCircleCircle(PhysicsObject* circle1, PhysicsObject* circle2);
	void resolveCircleRect(PhysicsObject* circle, PhysicsObject* rectangle);
	void resolveRectRect(PhysicsObject* rectangle1, PhysicsObject* rectangle2);
	
//physics
	void update(float dt);

//misc
	void draw();
};
#endif

