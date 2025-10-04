#include"includes.cpp"
#include"physics_object.h"

int WIDTH = 900;
int HEIGHT = 900;
char TITLE[12] = "2D PHYSICS";

Vector2 gravity = { 0,980 };

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
		float dt = GetFrameTime();

		BeginDrawing();
		ClearBackground(BLACK);
//temporary physics area begin
		a.ApplyForce(gravity);
		b.ApplyForce(gravity);
		c.ApplyForce(gravity);

		a.update(dt);
		b.update(dt);
		c.update(dt);
//temporary physics area end

		a.Draw();
		b.Draw();
		c.Draw();
		EndDrawing();
	}
	CloseWindow();
}