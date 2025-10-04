#include"includes.cpp"
#include"physics_world.h"

int WIDTH = 900;
int HEIGHT = 900;
char TITLE[12] = "2D PHYSICS";

Vector2 gravity = { 0,980 };

int main()
{
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(60);

	PhysicsWorld world;
	world.setGravity(gravity);

	PhysicsObject a(LINE,      WIDTH / 2, { 200,200 },                     10, BLUE);
	PhysicsObject b(CIRCLE,    50,        { WIDTH / 2.0f, HEIGHT / 2.0f }, 20, GREEN);
	PhysicsObject c(RECTANGLE, 100, 150,  { 600,200 },                     15, RED);

	world.addObject(&a);
	world.addObject(&b);
	world.addObject(&c);

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();

		//Update Physics
		world.update(dt);

		//User Input
		if (IsMouseButtonPressed(0))
		{
			Vector2 mousePos = GetMousePosition();
			PhysicsObject* ball = new PhysicsObject(
				CIRCLE, 20, mousePos, 10, 
				{ (unsigned char)(rand() % 255),
				(unsigned char)(rand() % 255),
				(unsigned char)(rand() % 255),
				255}
			);
			world.addObject(ball);
		}

//Memory optimization : Delete objects no longer on screen
		auto objects = world.getObjects();
		for (auto i : objects)
		{
			if (!i->getIsStatic())
			{
				float upper;
				switch (i->getShape())
				{
				case LINE:
					upper = (i->isHorizontal()) ? i->getPos().y : i->getPos().y + i->getLength() / 2;
				case CIRCLE:
					upper = i->getPos().y + i->getRadius();
				case RECTANGLE:
					upper = i->getPos().y + i->getHeight() / 2;
				}
				
				if (upper > HEIGHT) world.removeObject(i);
			}
		}

		BeginDrawing();
		ClearBackground(BLACK);

		//Render
		world.draw();

		EndDrawing();
	}
	CloseWindow();
}