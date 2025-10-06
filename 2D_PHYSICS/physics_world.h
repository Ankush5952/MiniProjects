#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include"includes.cpp"
#include"physics_object.h"

enum COLLISSION_ALGO
{
	MINIMUM,
	AVERAGE,
	GEOMETRIC_MEAN
};

class PhysicsWorld
{
private:
	vector<PhysicsObject*> objects;
	Vector2 gravity = { 0, 980 };
	COLLISSION_ALGO algo = MINIMUM;
	int substeps = 4;

public:
//getters

	vector<PhysicsObject*>& getObjects();
	Vector2 getGravity();
	COLLISSION_ALGO getCollissionAlgo();
	int getSubsteps() { return substeps; }

//setters
	void addObject(PhysicsObject* obj);
	void removeObject(PhysicsObject* obj);
	void setGravity(Vector2 g);
	void setGravity(float x, float y);
	void setCollissionAlgo(COLLISSION_ALGO c = MINIMUM);
	void setSubsteps(int s) { substeps = s; }

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
	void createPhysicsObject(SHAPE s = RECTANGLE, float d1 = 1.0f, float d2 = 0.0f, Vector2 pos = { 0,0 }, float m = 1, Color c = WHITE, bool st = 0, float res = 0.8);
	void createPhysicsObject(SHAPE s = LINE, float d1 = 0.0f, Vector2 pos = { 0,0 }, float m = 1, Color c = WHITE, bool st = 0, float res = 0.8);
	void draw();
};
#endif

