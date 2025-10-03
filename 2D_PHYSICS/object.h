#ifndef OBJECT_H
#define OBJECT_H

#include"includes.cpp"
enum SHAPE
{
	LINE,
	CIRCLE,
	RECTANGLE
};

class Object
{
	int posx = 0;
	int posy = 0;
	SHAPE shape;
	Color color = WHITE;
public:
	pair<int, int> getPos();
	SHAPE getShape();
	Color getColor();

	void setPos(int x, int y);
	void setPos(pair<int, int> p);
	void setShape(SHAPE s);
	void setColor(Color c);

	void Draw(int a, int b = 1, bool horizontal = true);
};

#endif
