#include"includes.cpp"
#include"physics_world.h"

static PhysicsWorld world;

//Update Object positions and wall lengths based on window size change
static void updateObjects(float pw, float ph)
{
	updateWallParameters();

	vector<PhysicsObject*> objs = world.getObjects();
	objs[0]->setPos(floorpos);
	objs[0]->setWidth(floord1);
	objs[1]->setPos(rwallpos);
	objs[1]->setHeight(walld2);
	objs[2]->setPos(lwallpos);
	objs[2]->setHeight(walld2);
	objs[3]->setPos(ceilpos);
	objs[3]->setWidth(floord1);

	float w = WIDTH, h = HEIGHT;
	if (isFullscreen)
	{
		int monitor = GetCurrentMonitor();
		w = GetMonitorWidth(monitor);
		h = GetMonitorHeight(monitor);
	}

	for (int i = 4; i < objs.size(); i++)
	{
		Vector2 currPos = objs[i]->getPos();
		float newPosx = (currPos.x * w) / pw;
		float newPosy = (currPos.y * h) / ph;
		objs[i]->setPos(newPosx, newPosy);
	}
}

//On Window Resize Event Function
void onWindowResize(float pw, float ph)
{
	updateObjects(pw, ph);
}

int main()
{
//WINDOW
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(60);	
	createWindowVariables();

//WORLD
	world.setGravity(gravity);
	world.setCollissionAlgo(MINIMUM);

//OBJECTS
	world.createPhysicsObject(RECTANGLE, floord1, floord2,   floorpos, 50, DARKGREEN, true,  1);//floor
	world.createPhysicsObject(RECTANGLE, walld1,  walld2, rwallpos, 50, DARKGREEN, true,  1);//rwall
	world.createPhysicsObject(RECTANGLE, walld1,  walld2, lwallpos, 50, DARKGREEN, true,  1);//lwall
	world.createPhysicsObject(RECTANGLE, floord1, floord2,   ceilpos,  50, DARKGREEN, true,  1);//ceil

//MAIN LOOP
	while (!WindowShouldClose())
	{
	//Delta time
		float dt = GetFrameTime();
		dt = fmin(dt, 0.016f);

	//Update Physics
		world.update(dt);

	//User Input
		//fullscreen
		if (IsKeyPressed(KEY_F11))
		{
			prevW = GetScreenWidth();
			prevH = GetScreenHeight();

			ToggleFullscreen();

			if (isFullscreen)
			{
				WIDTH = widthBeforeFS;
				HEIGHT = heightBeforeFS;
			}
			else 
			{
				widthBeforeFS = prevW;
				heightBeforeFS = prevH;

				WIDTH = maxW;
				HEIGHT = maxH;
			}

			SetWindowSize(WIDTH, HEIGHT);

			isFullscreen = !isFullscreen;
		}
		//spawn balls
		if (IsMouseButtonPressed(0) || IsKeyPressed(KEY_ENTER) || IsKeyDown(KEY_ENTER))
		{
			Vector2 mousePos = GetMousePosition();
			world.createPhysicsObject(
				CIRCLE,
				20,
				mousePos,
				10, 
				{ 
					(unsigned char)(rand() % 255),//r
					(unsigned char)(rand() % 255),//g
					(unsigned char)(rand() % 255),//b
					255                           //a
				},
				false,
				1
			);
		}
		//spawn boxes
		if (IsMouseButtonPressed(1) || IsKeyDown(KEY_SPACE) || IsKeyPressed(KEY_SPACE))
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

	//Render World
		BeginDrawing();
		ClearBackground(BLACK);

		world.draw();

		EndDrawing();
	
	//Fix values
		if (!isFullscreen) WIDTH = GetScreenWidth(), HEIGHT = GetScreenHeight();
		if (prevW != WIDTH || prevH != HEIGHT)
		{
			onWindowResize(prevW, prevH);
			prevW = WIDTH;
			prevH = HEIGHT;
		}
	}
	CloseWindow();
}