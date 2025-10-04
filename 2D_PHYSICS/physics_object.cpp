#include "includes.cpp"
#include "physics_world.h"

Vector2 PhysicsObject::getPos()
{
	return position;
}

Vector2 PhysicsObject::getVelocity()
{
	return velocity;
}

float PhysicsObject::getMass()
{
	return mass;
}

bool PhysicsObject::getIsStatic()
{
	return isStatic;
}

SHAPE PhysicsObject::getShape()
{
	return shape;
}

Color PhysicsObject::getColor()
{
	return color;
}

float PhysicsObject::getRadius()
{
	return radius;
}

float PhysicsObject::getWidth()
{
	return width;
}

float PhysicsObject::getHeight()
{
	return height;
}

float PhysicsObject::getLength()
{
	return length;
}

bool PhysicsObject::isHorizontal()
{
	return horizontal;
}

void PhysicsObject::setPos(int x, int y)
{
	position.x = x;
	position.y = y;
}

void PhysicsObject::setPos(Vector2 p)
{
	position.x = p.x;
	position.y = p.y;
}

void PhysicsObject::setVelocity(Vector2 v)
{
	velocity = v;
}

void PhysicsObject::setVelocity(float vx, float vy)
{
	velocity = { vx,vy };
}

void PhysicsObject::setMass(float m)
{
	mass = m;
}

void PhysicsObject::setStatic(bool b)
{
	isStatic = b;
}

void PhysicsObject::setShape(SHAPE s)
{
	shape = s;
}

void PhysicsObject::setColor(Color c)
{
	color = c;
}

void PhysicsObject::setRadius(float r)
{
	radius = r;
}

void PhysicsObject::setWidth(float w)
{
	width = w;
}

void PhysicsObject::setHeight(float h)
{
	height = h;
}

void PhysicsObject::setLength(float l)
{
	length = l;
}

void PhysicsObject::setHorizontal(bool b)
{
	horizontal = b;
}

void PhysicsObject::Draw()
{
	switch (shape)
	{
		case LINE:
			if (!horizontal)
			{
				DrawLine(
					position.x - length / 2,
					position.y,
					position.x + length / 2,
					position.y,
					color
				);
			}
			else
			{
				DrawLine(
					position.x,
					position.y - length / 2,
					position.x,
					position.y + length / 2,
					color
				);
			}
			break;
		case CIRCLE:
			DrawCircle(position.x, position.y, radius, color);
			break;
		case RECTANGLE:
			DrawRectangle(
				position.x - width / 2,
				position.y - height / 2,
				width, height, color);
			break;
		default:
			break;
	}
}

void PhysicsObject::ApplyForce(Vector2 f)
{
	if (isStatic) return;

	Vector2 a = Vector2Scale(f, 1.0f / mass);
	acceleration = Vector2Add(acceleration, a);
}

void PhysicsObject::update(float dt)
{
	if (isStatic) return;
	//v = u + at
	velocity = Vector2Add(velocity, Vector2Scale(acceleration, dt));

	//sf = si + vt : {s = ut + 1/2 at^2}
	position = Vector2Add(position, Vector2Scale(velocity, dt));

	//reset the acceleration to be updated each frame
	acceleration = { 0,0 };
}

PhysicsObject::PhysicsObject(SHAPE s, float dimension, Vector2 pos, float m, Color c, bool is_static)
{
	shape = s;
	position = pos;
	mass = m;
	color = c;
	isStatic = is_static;
	if (s == LINE) length = dimension;
	else if (s == CIRCLE) radius = dimension;
	else width = dimension, height = 1.0f;
}
PhysicsObject::PhysicsObject(SHAPE s, float dimension1, float dimension2, Vector2 pos, float m, Color c, bool is_static)
{
	shape = s;
	position = pos;
	mass = m;
	color = c;
	isStatic = is_static;
	if (shape == RECTANGLE) width = dimension1, height = dimension2;
	else if (shape == LINE) length = dimension1;
	else radius = dimension1;
}
