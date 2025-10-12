#pragma once

#include"particle_manager.h"
#include"preset_manager.h"
namespace ParticleSystem
{
	class ParticleUI
	{
	private:
		int particlesOnScreen = 0;
		int drawParticleSize = 1;
		float drawParticleLifetime = 5.0f;
		int drawColorIndex = 0;
		ParticleSystem::ParticleShape drawShapeMode = CIRCLE;
		ParticleSystem::CollissionAlgo drawCollissionMode = DESTROY;
		std::string currentPreset = "NONE";

	public:
	//getters
	
		ParticleShape getShapeMode();
		CollissionAlgo getCollissionResponse();
	//setters

		void updateParticlesOnScreen(int n);
		void updateParticleSize(int n);
		void updateParticleLifetime(int n);
		void updateParticleColor(int c);
		void updateShapeMode(ParticleSystem::ParticleShape s);
		void updateCollissionResponse(ParticleSystem::CollissionAlgo res);
		void applyPreset(const ParticleSystem::Preset& preset);

		void drawUI(ParticleSystem::ParticleManager* manager);
	};
}

