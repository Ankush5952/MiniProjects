#include"includes.cpp"
#include"physics_object.h"

int WIDTH = 900;
int HEIGHT = 900;
char TITLE[12] = "2D PHYSICS";

int main()
{
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(60);

	PhysicsObject a;
	a.setPos({ WIDTH / 2.0f, HEIGHT / 2.0f });
	a.setColor(GREEN);
	a.setShape(CIRCLE);
	a.setRadius(50);

	PhysicsObject b;
	b.setPos(200, 200);
	b.setColor(BLUE);
	b.setShape(RECTANGLE);
	b.setWidth(100);
	b.setHeight(200);

	PhysicsObject c;
	c.setPos(100, 400);
	c.setColor(WHITE);
	c.setShape(LINE);
	c.setLength(100);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		a.Draw();
		b.Draw();
		c.Draw();
		EndDrawing();
	}
	CloseWindow();
}