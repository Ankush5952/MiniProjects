#include"includes.cpp"
#include"physics_world.h"

int WIDTH = 900;
int HEIGHT = 900;
char TITLE[12] = "2D PHYSICS";

Vector2 gravity = { 0,980 };

int errors = 0;

void LogCollissionErrors()
{
	char text[4];
	sprintf_s(text, "%i", errors);
	DrawText(text, WIDTH / 2, 50, 30, RED);

	EndDrawing();
}

int main()
{
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(60);

	PhysicsWorld world;
	world.setGravity(gravity);
	world.setCollissionAlgo(MINIMUM);

	world.createPhysicsObject(RECTANGLE, WIDTH, 1.0f,  { WIDTH / 2.0f, HEIGHT - 1.0f }, 50, DARKGREEN, true,  1);//floor
	world.createPhysicsObject(RECTANGLE, 1.0f, HEIGHT, { 1.0f, HEIGHT / 2.0f },         50, DARKGREEN, true,  1);//rwall
	world.createPhysicsObject(RECTANGLE, 1.0f, HEIGHT, { WIDTH - 1.0f, HEIGHT / 2.0f }, 50, DARKGREEN, true,  1);//lwall
	world.createPhysicsObject(RECTANGLE, WIDTH, 1.0f,  { WIDTH / 2.0f, 2.0f },          50, DARKGREEN, true,  1);//ceil

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();

		//Update Physics
		world.update(dt);

		//User Input
		if (IsMouseButtonPressed(0))
		{
			Vector2 mousePos = GetMousePosition();
			world.createPhysicsObject(
				CIRCLE,
				20,
				mousePos,
				10, 
				{ 
					(unsigned char)(rand() % 255),//r
					(unsigned char)(rand() % 255),//b
					(unsigned char)(rand() % 255),//g
					255                           //a
				},
				false,
				1.0f
			);
		}

		if (IsMouseButtonPressed(1))
		{
			Vector2 mousePos = GetMousePosition();
			world.createPhysicsObject(
				RECTANGLE,
				20.0f,
				20.0f,
				mousePos,
				20.0f,
				{
					(unsigned char)(rand() % 255),
					(unsigned char)(rand() % 255),
					(unsigned char)(rand() % 255),
					255
				},
				false,
				1
			);
		}
#pragma region MemOp
		//Memory optimization : Delete objects no longer on screen
		auto objects = world.getObjects();
		for (auto i : objects)
		{
			if (!i->getIsStatic())
			{
				float upper = 0.0f;
				switch (i->getShape())
				{
				case LINE:
					upper = (i->isHorizontal()) ? i->getPos().y : i->getPos().y + i->getLength() / 2;
					break;
				case CIRCLE:
					upper = i->getPos().y + i->getRadius();
					break;
				case RECTANGLE:
					upper = i->getPos().y + i->getHeight() / 2;
					break;
				}

				if (upper > HEIGHT * 2) world.removeObject(i), errors++;
			}
		}
#pragma endregion

		BeginDrawing();
		ClearBackground(BLACK);

//Render
		world.draw();
	//Debug
		//LogCollissionErrors();
	}
	CloseWindow();
}