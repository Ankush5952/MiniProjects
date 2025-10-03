#include "object.h"

pair<int, int> Object::getPos()
{
	return { posx, posy };
}

SHAPE Object::getShape()
{
	return shape;
}

Color Object::getColor()
{
	return color;
}

void Object::setPos(int x, int y)
{
	posx = x;
	posy = y;
}

void Object::setPos(pair<int, int> p)
{
	posx = p.first;
	posy = p.second;
}

void Object::setShape(SHAPE s)
{
	shape = s;
}

void Object::setColor(Color c)
{
	color = c;
}

//LINE : a = length, b = none, horizontal = line direction
//CIRCLE : a = radius, b = none, horizontal = none
//RECTANGLE : a = length, b = breadth, horizontal = none
void Object::Draw(int a, int b, bool horizontal)
{
	switch (shape)
	{
		case LINE:
			DrawLine(
				(horizontal) ? posx - a / 2 : posx,
				(horizontal) ? posy : posy - a / 2,
				(horizontal) ? posx + a / 2 : posx,
				(horizontal) ? posy : posy + a / 2,
				color);
			break;
		case CIRCLE:
			DrawCircle(posx, posy, a, color);
			break;
		case RECTANGLE:
			DrawRectangle(
				posx - a / 2,
				posy + b / 2,
				a, b, color);
			break;
		default:
			break;
	}
}
