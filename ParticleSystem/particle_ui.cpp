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

	char text[70];
	sprintf_s(text,
		"Particles: %i\nShape: %s\nCollission Response: %s",
		particlesOnScreen, shapes[drawShapeMode], collissions[drawCollissionMode]);
	DrawText(text, WIDTH * 0.05, HEIGHT * 0.05, 15, GOLD);
}
