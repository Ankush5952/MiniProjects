#include "particle_emitter_manager.h"

const std::vector<ParticleSystem::ParticleEmitter*>& ParticleSystem::ParticleEmitterManager::getEmitters() const
{
	return emitters;
}

void ParticleSystem::ParticleEmitterManager::update(ParticleSystem::ParticleManager* manager)
{
	if (!manager) return;
	for (auto i : emitters)
	{
		i->update(manager);
	}
}

void ParticleSystem::ParticleEmitterManager::createEmitter(ParticleSystem::ParticleEmitter* emitter)
{
	emitters.push_back(emitter);
}

void ParticleSystem::ParticleEmitterManager::createEmitter(const ParticleSystem::ParticleEmitter emitter)
{
	ParticleSystem::ParticleEmitter* temp = new ParticleSystem::ParticleEmitter(emitter);
	temp->name = "Emitter" + std::to_string(emitters.size());
	emitters.push_back(temp);
}

void ParticleSystem::ParticleEmitterManager::createEmitter(const ParticleSystem::Preset& preset, Vector2 pos, float freq)
{
	ParticleSystem::ParticleEmitter* temp = new ParticleSystem::ParticleEmitter(preset, pos, freq);
	emitters.push_back(temp);
}

void ParticleSystem::ParticleEmitterManager::removeEmitter(ParticleSystem::ParticleEmitter* emitter)
{
	if (emitters.empty() || !emitter) return;
	delete(emitter);
	emitters.erase(remove(emitters.begin(), emitters.end(), emitter), emitters.end());

}

void ParticleSystem::ParticleEmitterManager::clean()
{
	for (auto& i : emitters) delete i;
	emitters.clear();
}

std::string ParticleSystem::ParticleEmitterManager::getNamesFormatted() const
{
	std::string names = "NONE;";
	for (auto& i : emitters)
	{
		names += i->name + ';';
	}
	names.pop_back();

	return names;
}

ParticleSystem::ParticleEmitterManager::~ParticleEmitterManager()
{
	clean();
}
