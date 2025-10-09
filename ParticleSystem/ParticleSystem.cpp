#include"includes.h"
#include"particle_manager.h"

static void onWindowResize(int pw, int ph, ParticleSystem::ParticleManager& manager)
{
	GRIDW = WIDTH / CELLSIZE;
	GRIDH = HEIGHT / CELLSIZE;

	center = { WIDTH / 2.0F, HEIGHT / 2.0f };

	std::vector<ParticleSystem::Particle*> pars = manager.getParticles();
	double widthFactor = double(WIDTH) / double(pw);
	double heightFactor = double(HEIGHT) / double(ph);
	for (auto& i : pars)
	{
		Vector2 currPos = i->getPos();
		Vector2 newPos = {
			currPos.x * widthFactor,
			currPos.y * heightFactor
		};
		i->setPos(newPos);
	}
}

static void HandleFullScreenToggle()
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
		WIDTH = maxW;
		HEIGHT = maxH;

		widthBeforeFS = prevW;
		heightBeforeFS = prevH;
	}

	SetWindowSize(WIDTH, HEIGHT);

	isFullscreen = !isFullscreen;
}

static void HandleResize(ParticleSystem::ParticleManager& manager)
{
	WIDTH = GetScreenWidth();
	HEIGHT = GetScreenHeight();
	if (prevW != WIDTH || prevH != HEIGHT)
	{
		onWindowResize(prevW, prevH, manager);
		prevW = WIDTH;
		prevH = HEIGHT;
	}
}

int main()
{
//WINDOW
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(60);
	InitializeRuntimeVariables();

//VARIABLES
	ParticleSystem::ParticleManager manager;
	
//MAIN LOOP
	while (!WindowShouldClose())
	{
	//DELTA TIME
		float dt = GetFrameTime();
		dt = fmin(dt, 0.016f);

	//EVENT HANDLING
		if (IsKeyPressed(KEY_G)) isGridEnabled = !isGridEnabled;

		if (IsKeyDown(KEY_SPACE))
		{
			manager.createParticle(
				10,
				5,
				{
					(unsigned char)(rand() % 256),
					(unsigned char)(rand() % 256),
					(unsigned char)(rand()%256),
					255
				},
				{
					float(rand() % (WIDTH)),
					float(rand() % (HEIGHT))
				},
				{
					50 - float(rand()%100),
					50 - float(rand()%100)
				}
			);
		}

	//RESIZE HANDLING
		if (IsKeyPressed(KEY_F11)) HandleFullScreenToggle();

		HandleResize(manager);

	//PHYSICS AND UPDATES
		manager.update(dt);

	//RENDER
		BeginDrawing();
		ClearBackground(BLUE);

		if (isGridEnabled) DrawGrid();

		manager.draw();

		EndDrawing();
	}
}