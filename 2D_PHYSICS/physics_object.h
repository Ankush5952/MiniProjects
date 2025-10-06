#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include"includes.cpp"
enum SHAPE
{
	LINE,
	CIRCLE,
	RECTANGLE
};

class PhysicsObject
{
private:
	//physics

	Vector2 position = { 0,0 };
	Vector2 velocity = { 0,0 };
	Vector2 acceleration = { 0,0 };
	float mass = 1.0f;
	float restitution = 0.8f;
	bool isStatic = false;

	float invM = 0.0f;

	//visual

	SHAPE shape;
	Color color = WHITE;

	//shape

#if shape == CIRCLE
	float radius = 0;
#endif

#if shape == RECTANGLE
	float width = 0, height = 0;
#endif

#if shape == LINE
	float length = 0;
	bool horizontal = false;
#endif

public:
//getters
	//physics

	Vector2 getPos();
	Vector2 getVelocity();
	float getMass();
	float getRestitution();
	float getInvMass();
	bool getIsStatic();

	//visual

	SHAPE getShape();
	Color getColor();

	//shape
#if shape == CIRCLE
	float getRadius();
#endif // shape == CIRCLE

#if shape == RECTANGLE
	float getWidth();
	float getHeight();
#endif // shape == RECTANGLE

#if shape == LINE
	float getLength();
	bool isHorizontal();
#endif // shape == LINE


//setters
	//physics

	void setPos(int x, int y);
	void setPos(Vector2 p);
	void setVelocity(Vector2 v);
	void setVelocity(float vx, float vy);
	void setMass(float m);
	void setRestitution(float r);
	void setStatic(bool b);

	//visuals

	void setShape(SHAPE s);
	void setColor(Color c);

	//shape

#if shape == CIRCLE
	void setRadius(float r);
#endif // shape == CIRCLE
#if shape == RECTANGLE
	void setWidth(float w);
	void setHeight(float h);
#endif // shape == RECTANGLE
#if shape == LINE
	void setLength(float l);
	void setHorizontal(bool b);
#endif // shape == LINE

//PHYSICS

	void Draw();
	void ApplyForce(Vector2 f);
	void update(float dt);

//Constructor
	PhysicsObject(SHAPE s = LINE, float dimension = 1.0f, Vector2 pos = { 0,0 }, float m = 1.0f, Color c = WHITE, bool is_static = false, float res = 0.8f);
	PhysicsObject(SHAPE s = RECTANGLE, float dimension1 = 1.0f, float dimension2 = 1.0f, Vector2 pos = { 0,0 }, float m = 1.0f, Color c = WHITE, bool is_static = false, float res = 0.8f);
};

#endif
