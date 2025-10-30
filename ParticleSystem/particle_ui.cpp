#include "particle_ui.h"

void ParticleSystem::ParticleUI::drawUI(ParticleSystem::ParticleManager* manager)
{
	int particlesOnScreen = manager->getParticles().size();
	const char* gravityState = (gravityEnabled) ? "TRUE" : "FALSE";

	char labels[1024];
	char values[1024];
	sprintf_s(
		labels,
		"Particles\n\n"
		"Shape:\n\n"
		"Collission\n\n"
		"Size\n\n"
		"Color\n\n"
		"Life\n\n"
		"Gravity\n\n"
		"GRID\n\n"
		"FADE\n\n"
		"TRAIL\n\n"
		"GLOW\n\n"
		"BOUNDARY\n\n"
		"PRESET\n\n"
		"Apply Preset\n\n"
		"FREQUENCY\n\n"
		"EMITTER\n\n"
		"Create Emitter\n\n"
		"Create Emitter(Preset)\n\n"
		"Place Emitter\n\n"
		"Delete Emitter\n\n"
		"Delete all Emitters\n\n"
		"Toggle Emitter\n\n"
		"VelX\n\n"
		"VelY\n\n"
		"SPAWN\n\n"
		"CLEAR\n\n"
		"UI\n\n"
	);

	sprintf_s(
		values,
		"%i \n\n"
		"%s [ A ]\n\n"
		"%s [ D ]\n\n"
		"%i [ UP/DOWN ]\n\n"
		"%s [ <-/-> ]\n\n"
		"%.2f [ +/- ]\n\n"
		"%s [ Q ]\n\n"
		"%s [ G ]\n\n"
		"%s [ F ]\n\n"
		"%s [ T ]\n\n"
		"%s [ H ]\n\n"
		"%s [ B ]\n\n"
		"%s [ O/P ]\n\n"
		"ENTER\n\n"
		"%.2f [ Z/X ]\n\n"
		"%s [ K/L ]\n\n"
		"[ R ] \n\n"
		"[ E ] \n\n"
		"[ LMB ] \n\n"
		"[ DEL ] \n\n"
		"[ BKSPC ] \n\n"
		"[ L SHIFT ]\n\n"
		"[ %.2f, %.2f ]\n\n"
		"[ %.2f, %.2f ]\n\n"
		"[ SPACE ] \n\n"
		"[ C ] \n\n"
		"[ TAB ] \n\n",
		particlesOnScreen,
		shapes[shapeIndex],
		collissions[collissionIndex],
		particleSize,
		colors[colorIndex],
		particleLifetime,
		gravityState,
		(isGridEnabled) ? "ON" : "OFF",
		(fadeEffect) ? "ON" : "OFF",
		(trailEffect) ? "ON" : "OFF",
		(glowEffect) ? "ON" : "OFF",
		(boundary) ? "ON" : "OFF",
		currentPresetName.c_str(),
		frequency,
		currentEmitter.c_str(),
		velocityRangeX.x, velocityRangeX.y,
		velocityRangeY.x, velocityRangeY.y
	);

	/*const double fontsize = fmin(WIDTH, HEIGHT) * 0.015;
	const Vector2 label_vect = MeasureTextEx(default_font, labels, fontsize, 0.5);
	const Vector2 value_vect = MeasureTextEx(default_font, values, fontsize, 0.5);

	const float left_padding = WIDTH * 0.007f;
	const float middle_padding = 5.0f;
	const float right_padding = WIDTH * 0.007f;
	const float up_padding = HEIGHT * 0.05f;

	const float rectWidth = left_padding + label_vect.x + middle_padding + value_vect.x + right_padding;
	const float values_x = left_padding + label_vect.x + middle_padding;*/

	char fps[20];
	sprintf_s(fps, "FPS: %i", GetFPS());


	float scaleX = WIDTH / 900.0f;
	float scaleY = HEIGHT / 900.0f;
	float scale = fmin(scaleX, scaleY);
	windowRect.width = 200 * scaleX;
	windowRect.height = HEIGHT;
	if (showControls)
	{
		if (GuiWindowBox(windowRect, "CONTROLS")) showControls = false;

		if (showControls)
		{
			//CLEAR SCREEN
			Rectangle clsRect = { 30 * scale, 530 * scale, 170 * scaleX, 15 * scaleY };
			GuiLabel(clsRect, "CLEAR PARTICLES : C");
			//TOGGLE UI
			Rectangle uiToggleRect = { 30 * scale, 510 * scale, 170 * scaleX, 15 * scaleY };
			GuiLabel(uiToggleRect, "TOGGLE UI VIEW : TAB");

			//SPAWN
			Rectangle spawnRect = { 13 * scale, 490 * scale, 170 * scaleX, 15 * scaleY };
			GuiLabel(spawnRect, "SPAWN PARTICLES : HOLD SPACE");

			//PLACE EMITTER
			Rectangle placeRect = { 30 * scale, 470 * scale, 170 * scaleX, 15 * scaleY };
			GuiLabel(placeRect, "PLACE EMITTER : ENTER");

			//TOGGLE EMITTER
			Rectangle toggleRect = { 15 * scale, 450 * scale, 170 * scaleX, 15 * scaleY };
			if (GuiButton(toggleRect, "Toggle Emitter"))
			{
				if (currentEmitterIndex != 0)
				{
					auto& ref = emima.getEmitters()[currentEmitterIndex - 1];
					ref->enabled = !ref->enabled;
				}
			}

			//CLEAR EMITTERS
			Rectangle clearRect = { 15 * scale, 430 * scale, 170 * scaleX, 15 * scaleY };
			if (GuiButton(clearRect, "Clear Emitters"))
			{
				emima.clean();
				currentEmitterIndex = 0;
			}

			//DELETE EMITTER
			Rectangle delRect = { 15 * scale, 410 * scale, 170 * scaleX, 15 * scaleY };
			if (GuiButton(delRect, "Delete Emitter"))
			{
				if (currentEmitterIndex != 0)
				{
					emima.removeEmitter(emima.getEmitters()[currentEmitterIndex - 1]);
					currentEmitterIndex = Clamp(currentEmitterIndex, 0, emima.getEmitters().size());
				}
			}

			//CREATE PRESET EMITTER
			Rectangle cpeRect = { 15 * scale, 390 * scale, 170 * scaleX, 15 * scaleY };
			if (GuiButton(cpeRect, "Create Preset Emitter"))
			{
				emima.createEmitter(prema.getCurrentPreset(), GetMousePosition(), frequency);
			}

			//CREATE EMITTER
			Rectangle ceRect = { 15 * scale, 370 * scale, 170 * scaleX, 15 * scaleY };
			if (GuiButton(ceRect, "Create Emitter"))
			{
				emima.createEmitter(
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

			//CURRENT EMITTER
			Rectangle emiTextRect = { 15 * scale, 350 * scale, 70 * scaleX, 15 * scaleY };
			Rectangle emiRect = { 110 * scale, 350 * scale, 80 * scaleX, 15 * scaleY };
			GuiLabel(emiTextRect, "EMITTER: ");
			const char* emitterNames = emima.getNamesFormatted().c_str();
			if (GuiDropdownBox(emiRect, emitterNames, &currentEmitterIndex, emitter)) emitter = !emitter;

			//EMITTER FREQUENCY
			Rectangle freqTextRect = { 15 * scale, 330 * scale, 70 * scaleX, 15 * scaleY };
			Rectangle freqRect = { 110 * scale, 330 * scale, 70 * scaleX, 15 * scaleY };
			GuiLabel(freqTextRect, "EM FREQ: ");
			GuiSlider(freqRect, "1", "100", &frequency, 1, 100);
			DrawText(TextFormat("%.1f", frequency), 140 * scale, 332 * scale, 12, RED);

			//APPLY PRESET
			Rectangle applyTextRect = { 15 * scale, 310 * scale, 70 * scaleX, 15 * scaleY };
			Rectangle applyRect = { 110 * scale, 310 * scale, 70 * scaleX, 15 * scaleY };
			GuiLabel(applyTextRect, "APPLY PRESET: ");
			if (GuiButton(applyRect, "Press")) prema.applyPreset(prema.getCurrentPreset());

			//PRESET
			Rectangle presetTextRect = { 15 * scale, 290 * scale, 70 * scaleX, 15 * scaleY };
			Rectangle presetRect = { 110 * scale, 290 * scale, 80 * scaleX, 15 * scaleY };
			GuiLabel(presetTextRect, "PRESET: ");
			const char* presetNames = prema.getNamesFormatted().c_str();
			if (GuiDropdownBox(presetRect, presetNames, &pI, preset))
			{
				preset = !preset;
				if (!preset)
				{
					prema.setPresetIndex(pI);
					prema.applyPreset(prema.getCurrentPreset());
				}
			}

			//BOUNDARY
			Rectangle boundRect = { 15 * scale, 270 * scale, 170 * scaleX, 15 * scaleY };
			GuiToggle(boundRect, "TOGGLE BOUNDARY COLLISION", &boundary);

			//GLOW EFFECT
			Rectangle glowRect = { 15 * scale, 250 * scale, 170 * scaleX, 15 * scaleY };
			GuiToggle(glowRect, "TOGGLE GLOW EFFECT", &glowEffect);

			//PARTICLE TRAIL
			Rectangle trailRect = { 15 * scale, 230 * scale, 170 * scaleX, 15 * scaleY };
			GuiToggle(trailRect, "TOGGLE TRAIL EFFECT", &trailEffect);

			//PARTICLE FADE
			Rectangle fadeRect = { 15 * scale, 210 * scale, 170 * scaleX, 15 * scaleY };
			GuiToggle(fadeRect, "TOGGLE PARTICLE FADE", &fadeEffect);

			//GRID
			Rectangle gridRect = { 15 * scale, 190 * scale,170 * scaleX, 15 * scaleY };
			GuiToggle(gridRect, "TOGGLE GRID", &isGridEnabled);

			//GRAVITY
			Rectangle gravityRect = { 15 * scale, 170 * scale, 170 * scaleX, 15 * scaleY };
			GuiToggle(gravityRect, "TOGGLE GRAVITY", &gravityEnabled);
			gravity.y = (gravityEnabled) ? 200 : 0;

			//PARTICLE LIFETIME
			Rectangle lifeTextRect = { 15 * scale, 150 * scale,70 * scaleX,15 * scaleY };
			Rectangle lifeRect = { 130 * scale, 150 * scale, 50 * scaleX, 15 * scaleY };
			GuiLabel(lifeTextRect, "LIFETIME: ");
			int pL = particleLifetime;
			if (GuiValueBox(lifeRect, "", &pL, -1, 25, life))life = !life;
			particleLifetime = pL;

			//PARTICLE COLOR
			Rectangle colorTextRect = { 15 * scale,130 * scale,70 * scale,15 * scale };
			Rectangle colorRect = { 110 * scale,130 * scale,80 * scaleX,15 * scaleY };
			GuiLabel(colorTextRect, "COLOR: ");
			if (GuiDropdownBox(colorRect, "WHITE;RED;D.BLUE;GREEN;YELLOW;GRAY;BROWN;ORANGE;PINK;PURPLE", &colorIndex, color)) color = !color;

			//PARTICLE SIZE
			Rectangle sizeTextRect = { 15 * scale, 110 * scale, 70 * scaleX,15 * scaleY };
			Rectangle sizeRect = { 130 * scale, 110 * scale, 50 * scaleX, 15 * scaleY };
			GuiLabel(sizeTextRect, "SIZE: ");
			if (GuiValueBox(sizeRect, "", &particleSize, 1, 30, size)) size = !size;

			//COLLISSION ALGO
			Rectangle colTextRect = { 15 * scale, 90 * scale, 85 * scaleX, 15 * scaleY };
			Rectangle colRect = { 110 * scale, 90 * scale, 80 * scaleX, 15 * scaleY };
			GuiLabel(colTextRect, "COLLISION ALGO: ");
			if (GuiDropdownBox(colRect, "DESTROY;CONVERT;ABSORB;BOUNCE;REPEL;STICK;FLOW", &collissionIndex, collision)) collision = !collision;

			//SHAPES
			Rectangle shapeTextRect = { 15 * scale,70 * scale , 70 * scaleX, 15 * scaleY };
			Rectangle shapeRect = { 110 * scale,70 * scale,80 * scaleX,15 * scaleY };
			GuiLabel(shapeTextRect, "SHAPE: ");
			if(GuiDropdownBox(shapeRect, "CIRCLE;SQUARE;TRIANGLE", &shapeIndex, shape)) shape = !shape;

			//PARTICLE COUNT
			Rectangle particleTextRect = { 15 * scale, 50, 70 * scaleX, 15 * scaleY };
			Rectangle particleCountRect = { 130 * scale, 50 * scale, 50 * scaleX,15 * scaleY };
			GuiLabel(particleTextRect, "PARTICLES: ");
			GuiValueBox(particleCountRect, "", &particlesOnScreen, NULL, NULL, false);
		}
	}

	//DrawRectangle(0, 0, rectWidth, HEIGHT, Fade(BLACK, 0.7)); //BG RECT
	//DrawTextEx(default_font, labels, { left_padding, up_padding }, fontsize, 0.5, PINK);
	//DrawTextEx(default_font, values, { values_x, up_padding }, fontsize, 0.5, PINK);
	DrawText(fps, WIDTH - 100, 10, 20, GREEN); //FPS
}
