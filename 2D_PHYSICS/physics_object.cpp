#include "physics_object.h"

Vector2 PhysicsObject::getPos()
{
	return { position.x, position.y };
}

Vector2 PhysicsObject::getVelocity()
{
	return Vector2();
}

float PhysicsObject::getMass()
{
	return 0.0f;
}

bool PhysicsObject::getIsStatic()
{
	return false;
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
	return 0.0f;
}

float PhysicsObject::getWidth()
{
	return 0.0f;
}

float PhysicsObject::getHeight()
{
	return 0.0f;
}

float PhysicsObject::getLength()
{
	return 0.0f;
}

bool PhysicsObject::isHorizontal()
{
	return false;
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
