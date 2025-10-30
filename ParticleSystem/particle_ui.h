#pragma once

#include"particle_manager.h"
#include"particle_emitter_manager.h"

namespace ParticleSystem
{
	class ParticleUI
	{
		Rectangle windowRect = { 0, 0, 200, (float)HEIGHT };
		bool shape = false;
		bool collision = false;
		bool size = false;
		bool color = false;
		bool life = false;
		bool preset = false;
		bool emitter = false;

		PresetManager& prema = PresetManager::get();
		int pI = prema.getCurrentPresetIndex();

		ParticleEmitterManager& emima = ParticleEmitterManager::get();

	public:
		void drawUI(ParticleSystem::ParticleManager* manager);
	};
}

