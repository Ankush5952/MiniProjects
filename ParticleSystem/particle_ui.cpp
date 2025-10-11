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

	char text[140];
	sprintf_s(
		text,
		"Particles: %i\n"
		"Shape: %s (A)\n"
		"Collission Response: %s (D)\n"
		"Size: %i (UP/DOWN)\n"
		"Color: %s (L/R)\n"
		"Life: %i (+/-)\n"
		"SPAWN: SPACE\n"
		"GRID: G",
		particlesOnScreen,
		shapes[drawShapeMode], 
		collissions[drawCollissionMode], 
		drawParticleSize, 
		colors[drawColorIndex],
		drawParticleLifetime
	);

	DrawRectangle(WIDTH * 0.04, HEIGHT * 0.04, 320, 145, Fade(BLACK, 0.7));
	DrawText(text, WIDTH * 0.05, HEIGHT * 0.05, 15, GOLD);
}
