#pragma once

#include"particle_emitter.h"
namespace ParticleSystem
{
	class ParticleEmitterManager
	{
	private:
		std::vector<ParticleEmitter*> emitters;

	public:
		//getters
		const std::vector<ParticleEmitter*>& getEmitters() const;

		//update
		void update(ParticleManager* manager);

		//misc
		void createEmitter(ParticleEmitter* emitter);
		void createEmitter(const ParticleEmitter emitter);
		void createEmitter(const Preset& preset, Vector2 pos, float freq);

		void removeEmitter(ParticleEmitter* emitter);
		void clean();

		//deconstructor
		~ParticleEmitterManager();
	};
}

