#include "particle_ui.h"

ParticleSystem::ParticleShape ParticleSystem::ParticleUI::getShapeMode()
{
	return drawShapeMode;
}

ParticleSystem::CollissionAlgo ParticleSystem::ParticleUI::getCollissionResponse()
{
	return drawCollissionMode;
}

void ParticleSystem::ParticleUI::updateParticlesOnScreen(int n)
{
	particlesOnScreen = n;
}

void ParticleSystem::ParticleUI::updateParticleSize(int n)
{
	drawParticleSize = n;
}

void ParticleSystem::ParticleUI::updateParticleLifetime(int n)
{
	drawParticleLifetime = n;
}

void ParticleSystem::ParticleUI::updateParticleColor(int c)
{
	drawColorIndex = c;
}

void ParticleSystem::ParticleUI::updateShapeMode(ParticleSystem::ParticleShape s)
{
	drawShapeMode = s;
}

void ParticleSystem::ParticleUI::updateCollissionResponse(ParticleSystem::CollissionAlgo res)
{
	drawCollissionMode = res;
}

void ParticleSystem::ParticleUI::drawUI(ParticleSystem::ParticleManager* manager)
{
	particlesOnScreen = manager->getParticles().size();

	const char* gravityState = (gravityEnabled) ? "TRUE" : "FALSE";

	char text[512];
	sprintf_s(
		text,
		"Particles:     %i\n\n"
		"Shape:         %s (A)\n\n"
		"Collission     %s (D)\n\n"
		"Size:           %i (UP/DOWN)\n\n"
		"Color:         %s (L/R)\n\n"
		"Life:           %i (+/-)\n\n"
		"Gravity:       %s (Q)\n\n"
		"GRID:          %s (G)\n\n"
		"FADE:          %s (F)\n\n"
		"SPAWN:        SPACE\n\n"
		"UI:             TAB\n\n",
		particlesOnScreen,
		shapes[drawShapeMode],
		collissions[drawCollissionMode],
		drawParticleSize,
		colors[drawColorIndex],
		drawParticleLifetime,
		gravityState,
		(isGridEnabled)?"ON":"OFF",
		(fadeEffect)?"ON":"OFF"
	);

	char fps[20];
	sprintf_s(fps, "FPS: %i", GetFPS());

	DrawRectangle(0, 0, 215, HEIGHT, Fade(BLACK, 0.7));
	DrawText(text, WIDTH*0.009f, HEIGHT * 0.05, 15, GOLD);
	DrawText(fps, WIDTH - 100, 10, 20, GREEN);
}
