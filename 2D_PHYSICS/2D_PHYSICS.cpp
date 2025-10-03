#include"includes.cpp"
#include"object.h"

int WIDTH = 900;
int HEIGHT = 900;
char TITLE[12] = "2D PHYSICS";

int main()
{
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(60);

	Object a;
	a.setPos({ WIDTH / 2, HEIGHT / 2 });
	a.setShape(CIRCLE);
	a.setColor(GREEN);

	Object b;
	b.setPos({ 200,200 });
	b.setShape(RECTANGLE);
	b.setColor(YELLOW);

	Object c;
	c.setPos(400, 100);
	c.setShape(LINE);
	c.setColor(BLUE);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		a.Draw(50);
		b.Draw(50, 100);
		c.Draw(100,0,false);
		EndDrawing();
	}
	CloseWindow();
}