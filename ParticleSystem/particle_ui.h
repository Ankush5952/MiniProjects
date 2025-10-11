#pragma once

#include"particle_manager.h"
namespace ParticleSystem
{
	class ParticleUI
	{
	private:
		int particlesOnScreen = 0;
		ParticleSystem::ParticleShape drawShapeMode = CIRCLE;
		ParticleSystem::CollissionAlgo drawCollissionMode = BOUNCE;

	public:
	//getters
	
		ParticleShape getShapeMode();
		CollissionAlgo getCollissionResponse();
	//setters

		void updateParticlesOnScreen(int n);
		void updateShapeMode(ParticleSystem::ParticleShape s);
		void updateCollissionResponse(ParticleSystem::CollissionAlgo res);

		void drawUI(ParticleSystem::ParticleManager* manager);
	};
}

