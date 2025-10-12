#include"includes.h"
#include"particle_ui.h"
#include"preset_manager.h"

static void onWindowResize(int pw, int ph, ParticleSystem::ParticleManager& manager)
{
	GRIDW = WIDTH / CELLSIZE;
	GRIDH = HEIGHT / CELLSIZE;

	center = { WIDTH / 2.0F, HEIGHT / 2.0f };

	const std::vector<ParticleSystem::Particle*>& pars = manager.getParticles();
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

static void applyPreset(const ParticleSystem::Preset& preset)
{
	particleLifetime = preset.lifetime;
	particleSize = preset.size;
	shapeIndex = preset.shape;
	collissionIndex = preset.algo;
	Color drawParticleColor = preset.color;
	velocityRangeX = { -preset.velocity.x, preset.velocity.x };
	velocityRangeY = { -preset.velocity.y, preset.velocity.y };
	colorIndex = 0;
	for (auto& i : colorPresets)
	{
		if (i.r == drawParticleColor.r && i.b == drawParticleColor.b && i.g == drawParticleColor.g)
		{
			break;
		}
		colorIndex++;
	}
}

int main()
{
//WINDOW
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(WIDTH, HEIGHT, TITLE);
	//SetTargetFPS(60);
	InitializeRuntimeVariables();

//VARIABLES
	ParticleSystem::ParticleManager manager;
	ParticleSystem::ParticleUI ui;
	ParticleSystem::PresetManager prema;
	currentPresetName = prema.getCurrentPreset().name;
	
//MAIN LOOP
	while (!WindowShouldClose())
	{
	//DELTA TIME
		float dt = GetFrameTime();
		dt = fmin(dt, 0.016f);

	//EVENT HANDLING
		//toggles
		if (IsKeyPressed(KEY_G)) isGridEnabled = !isGridEnabled;
		if (IsKeyPressed(KEY_TAB)) showControls = !showControls;
		if (IsKeyPressed(KEY_F)) fadeEffect = !fadeEffect;
		if (IsKeyPressed(KEY_B)) boundary = !boundary;
		
		//particle properties
		if (IsKeyPressed(KEY_A))
		{
			shapeIndex++;
			shapeIndex %= 3;
		}
		if (IsKeyPressed(KEY_D))
		{
			collissionIndex++;
			collissionIndex %= 7;
		}
		if (IsKeyPressed(KEY_UP))
		{
			particleSize++;
		}
		if (IsKeyPressed(KEY_DOWN))
		{
			particleSize = fmax(1, particleSize - 1);
		}
		if (IsKeyPressed(KEY_RIGHT))
		{
			colorIndex = fmin(9, colorIndex + 1);
		}
		if (IsKeyPressed(KEY_LEFT))
		{
			colorIndex = fmax(0, colorIndex - 1);
		}
		if (IsKeyPressed(KEY_C))
		{
			manager.clean();
		}
		if (IsKeyPressed(KEY_KP_ADD))
		{
			particleLifetime++;
		}
		if (IsKeyPressed(KEY_KP_SUBTRACT))
		{
			particleLifetime = fmax(-1, particleLifetime - 1);
		}
		if (IsKeyPressed(KEY_P))
		{
			prema.nextPreset();
			currentPresetName = prema.getCurrentPreset().name;
		}
		if (IsKeyPressed(KEY_O))
		{
			prema.prevPreset();
			currentPresetName = prema.getCurrentPreset().name;
		}
		if (IsKeyPressed(KEY_ENTER))
		{
			applyPreset(prema.getCurrentPreset());
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

		HandleResize(manager);

	//PHYSICS AND UPDATES
		manager.update(dt);

	//RENDER
		BeginDrawing();
		ClearBackground(BLUE);

		if (isGridEnabled) DrawGrid();

		manager.draw();
		if(showControls) ui.drawUI(&manager);

		EndDrawing();
	}
}