#include"includes.h"
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
	InitializeRuntimeVariables();

//VARIABLES
	ParticleSystem::ParticleManager manager;
	ParticleSystem::ParticleUI ui;
	ParticleSystem::PresetManager& prema = ParticleSystem::PresetManager::get();
	currentPresetName = prema.getCurrentPreset().name;
	ParticleSystem::ParticleEmitterManager emitterManager;
	const auto& emitters = emitterManager.getEmitters();
	
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
		if (IsKeyPressed(KEY_T)) trailEffect = !trailEffect;
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
		if (IsKeyPressed(KEY_K))
		{
			currentEmitterIndex = fmax(-1, currentEmitterIndex - 1);
			currentEmitter = (currentEmitterIndex == -1) ? "NONE" : emitters[currentEmitterIndex]->name;
		}
		if (IsKeyPressed(KEY_L))
		{
			if(emitters.size() > 0)
			{
				currentEmitterIndex = fmin(currentEmitterIndex + 1, emitters.size()-1);
				currentEmitter = emitters[currentEmitterIndex]->name;
			}
		}
		if (IsKeyPressed(KEY_E))
		{
			emitterManager.createEmitter(prema.getCurrentPreset(), GetMousePosition(), frequency);
		}
		if (IsKeyPressed(KEY_R))
		{
			emitterManager.createEmitter(
				{
					{
						ParticleSystem::ParticleShape(shapeIndex),
						particleSize,
						particleLifetime,
						colorPresets[colorIndex],
						GetMousePosition(),
						{0,0},
						ParticleSystem::CollissionAlgo(collissionIndex)
					},
					GetMousePosition(),
					{ -100,-100 },
					{ 100,100 },
					frequency
				}
			);
		}
		if (IsMouseButtonPressed(0))
		{
			if (currentEmitterIndex != -1) 
			{
				emitters[currentEmitterIndex]->setPos(GetMousePosition());
				emitters[currentEmitterIndex]->enabled = true;
			}
		}
		if (IsKeyDown(KEY_Z))
		{
			frequency = fmax(0.1f, frequency - 0.1f);
		}
		if (IsKeyDown(KEY_X))
		{
			frequency += 0.1f;
		}
		if (IsKeyPressed(KEY_LEFT_SHIFT))
		{
			if(currentEmitterIndex != -1) emitters[currentEmitterIndex]->enabled = !emitters[currentEmitterIndex]->enabled;
		}
		if (IsKeyPressed(KEY_DELETE)) 
		{
			if(currentEmitterIndex != -1)
			{
				emitterManager.removeEmitter(emitters[currentEmitterIndex]);
				currentEmitterIndex = Clamp(currentEmitterIndex, -1, emitters.size() - 1);
				currentEmitter = (currentEmitterIndex == -1) ? "NONE" : emitters[currentEmitterIndex]->name;
			}
		}
		if (IsKeyPressed(KEY_BACKSPACE)) 
		{
			emitterManager.clean();
			currentEmitterIndex = -1;
			currentEmitter = "NONE";
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
}