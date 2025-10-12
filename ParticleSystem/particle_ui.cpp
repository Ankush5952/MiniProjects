#include "particle_ui.h"

void ParticleSystem::ParticleUI::drawUI(ParticleSystem::ParticleManager* manager)
{
	int particlesOnScreen = manager->getParticles().size();
	const char* gravityState = (gravityEnabled) ? "TRUE" : "FALSE";

	char text[512];
	sprintf_s(
		text,
		"Particles:     %i\n\n"
		"Shape:         %s (A)\n\n"
		"Collission     %s (D)\n\n"
		"Size:           %i (UP/DOWN)\n\n"
		"Color:         %s (<-/->)\n\n"
		"Life:           %.2f (+/-)\n\n"
		"Gravity:       %s (Q)\n\n"
		"GRID:          %s (G)\n\n"
		"FADE:          %s (F)\n\n"
		"BOUNDARY:      %s (B)\n\n"
		"PRESET :     %s (O/P)\n\n"
		"Apply Preset       ENTER\n\n"
		"VelX:          [%.2f, %.2f]\n\n"
		"VelY:          [%.2f, %.2f]\n\n"
		"SPAWN:        SPACE\n\n"
		"UI:             TAB\n\n",
		particlesOnScreen,
		shapes[shapeIndex],
		collissions[collissionIndex],
		particleSize,
		colors[colorIndex],
		particleLifetime,
		gravityState,
		(isGridEnabled)?"ON":"OFF",
		(fadeEffect)?"ON":"OFF",
		(boundary)?"ON":"OFF",
		currentPresetName.c_str(),
		velocityRangeX.x, velocityRangeX.y,
		velocityRangeY.x, velocityRangeY.y
	);

	char fps[20];
	sprintf_s(fps, "FPS: %i", GetFPS());

	DrawRectangle(0, 0, 215, HEIGHT, Fade(BLACK, 0.7));
	DrawText(text, WIDTH*0.009f, HEIGHT * 0.05, 15, GOLD);
	DrawText(fps, WIDTH - 100, 10, 20, GREEN);
}
