#pragma once

#include"particle_emitter.h"
namespace ParticleSystem
{
	class ParticleEmitterManager
	{
	private:
		std::vector<ParticleEmitter*> emitters;

		ParticleEmitterManager() = default;
		ParticleEmitterManager(const ParticleEmitterManager& m) = delete;
		ParticleEmitterManager& operator= (const ParticleEmitterManager&) = delete;
		ParticleEmitterManager(ParticleEmitterManager&&) = delete;
		ParticleEmitterManager& operator= (ParticleEmitterManager&&) = delete;

	public:
		//getters
		const std::vector<ParticleEmitter*>& getEmitters() const;

		static ParticleEmitterManager& get()
		{
			static ParticleEmitterManager instance;
			return instance;
		}
		std::string getNamesFormatted() const;

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

