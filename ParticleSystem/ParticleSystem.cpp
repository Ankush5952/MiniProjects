#define RAYGUI_IMPLEMENTATION
#include"includes/includes.h"
#include"particle_ui.h"
#include"preset_manager.h"
#include"particle_emitter_manager.h"

static void onWindowResize(ParticleSystem::ParticleManager& manager, ParticleSystem::ParticleEmitterManager& emitterManager)
{
	GRIDW = WIDTH / CELLSIZE;
	GRIDH = HEIGHT / CELLSIZE;

	center = { WIDTH / 2.0F, HEIGHT / 2.0f };

	const std::vector<ParticleSystem::Particle*>& pars = manager.getParticles();
	double widthFactor = double(WIDTH) / double(prevW);
	double heightFactor = double(HEIGHT) / double(prevH);
	for (auto& i : pars)
	{
		std::deque<Vector2>& trail = i->getTrail();
		for (Vector2& j : trail)
		{
			j = {
				float(j.x * widthFactor),
				float(j.y * heightFactor)
			};
		}

		Vector2 currPos = i->getPos();
		Vector2 newPos = {
			currPos.x * widthFactor,
			currPos.y * heightFactor
		};
		i->setPos(newPos);
	}

	const std::vector<ParticleSystem::ParticleEmitter*>& ems = emitterManager.getEmitters();
	for (auto& i : ems)
	{
		Vector2 currPos = i->getPos();
		Vector2 newPos = {
			currPos.x * widthFactor,
			currPos.y * heightFactor
		};
		i->setPos(newPos);
	}

	Matrix proj = MatrixOrtho(0.0, WIDTH, HEIGHT, 0.0, -1.0, 1.0);
	mvpMat = proj;
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

static void HandleResize(ParticleSystem::ParticleManager& manager, ParticleSystem::ParticleEmitterManager& emitterManager)
{
	WIDTH = GetScreenWidth();
	HEIGHT = GetScreenHeight();
	if (prevW != WIDTH || prevH != HEIGHT)
	{
		onWindowResize(manager, emitterManager);
		prevW = WIDTH;
		prevH = HEIGHT;
	}
}

int main()
{
//WINDOW
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(WIDTH, HEIGHT, TITLE);
	if (!gladLoadGL())
	{
		std::cerr << "Failed to init GL Loader.\n";
		CloseWindow();
		return -1;
	}
	InitializeRuntimeVariables();

//VARIABLES
	ParticleSystem::ParticleManager manager;
	manager.generateResources();
	ParticleSystem::ParticleUI ui;
	ParticleSystem::PresetManager& prema = ParticleSystem::PresetManager::get();
	currentPresetName = prema.getCurrentPreset().name;
	ParticleSystem::ParticleEmitterManager& emitterManager = ParticleSystem::ParticleEmitterManager::get();
	const auto& emitters = emitterManager.getEmitters();

//SHADER
	ParticleSystem::Particle::loadShader();
	
//MAIN LOOP
	while (!WindowShouldClose())
	{
	//DELTA TIME
		float dt = GetFrameTime();
		dt = fmin(dt, 0.016f);

	//INPUT HANDLING
		//toggles
		if (IsKeyPressed(KEY_G)) isGridEnabled = !isGridEnabled;
		if (IsKeyPressed(KEY_TAB)) showControls = !showControls;
		if (IsKeyPressed(KEY_F)) fadeEffect = !fadeEffect;
		if (IsKeyPressed(KEY_T)) trailEffect = !trailEffect;
		if (IsKeyPressed(KEY_B)) boundary = !boundary;
		if (IsKeyPressed(KEY_H)) glowEffect = !glowEffect;
		
		//particle properties
		if (IsKeyPressed(KEY_ENTER))
		{
			if (currentEmitterIndex != 0)
			{
				emitterManager.getEmitters()[currentEmitterIndex - 1]->setPos(GetMousePosition());
			}
		}
		if (IsKeyPressed(KEY_C))
		{
			manager.clean();
		}

		//external forces
		if (IsKeyPressed(KEY_Q))
		{
			gravityEnabled = !gravityEnabled;
			gravity.y = gravityEnabled ? 200 : 0;
		}

		//spawning
		if (IsKeyDown(KEY_SPACE))
		{
			manager.createParticle(
				ParticleSystem::ParticleShape(shapeIndex),
				particleSize,
				particleLifetime,
				colorPresets[colorIndex],
				GetMousePosition(),
				{
					velocityRangeX.y - rand()%int(velocityRangeX.y * 2),
					velocityRangeY.y - rand() % int(velocityRangeY.y * 2)
				},
				ParticleSystem::CollissionAlgo(collissionIndex)
			);
		}

	//RESIZE HANDLING
		if (IsKeyPressed(KEY_F11)) HandleFullScreenToggle();

		HandleResize(manager, emitterManager);

	//PHYSICS AND UPDATES
		emitterManager.update(&manager);
		manager.update(dt);

	//RENDER
		BeginDrawing();
		ClearBackground(BLUE);

		if (isGridEnabled) DrawGrid();

		manager.draw();
		if(showControls) ui.drawUI(&manager);

		EndDrawing();
	}

	ParticleSystem::Particle::unloadShader();
	CloseWindow();
}