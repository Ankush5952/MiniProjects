#pragma once

#include"particle.h"
using json = nlohmann::json;

namespace ParticleSystem
{
	struct Preset
	{
		std::string name;
		ParticleShape shape;
		int size;
		float lifetime;
		Color color;
		CollissionAlgo algo;
		Vector2 velocity;

		Preset(std::string n = "default", ParticleShape s = CIRCLE, int a = 1, float t = 3.0f,
			Color c = WHITE, CollissionAlgo cr = BOUNCE, Vector2 v = { 0,0 }) :
			name(n), shape(s), size(a), lifetime(t), color(c), algo(cr), velocity(v){}

		json toJson() const;
		static Preset fromJson(const json& j);
	};

	class PresetManager
	{
	private:
		std::vector<Preset> presets;
		int currentPresetIndex;
		std::string filePath;

		//constructor
		PresetManager(const std::string& pathToFile = "resources/presets.json");
		PresetManager(const PresetManager& p) = delete;
		PresetManager& operator= (const PresetManager&) = delete;
		PresetManager(PresetManager&&) = delete;
		PresetManager& operator= (PresetManager&&) = delete;

	public:
		//getters
		static PresetManager& get()
		{
			static PresetManager instance;
			return instance;
		}
		const std::vector<Preset>& getPresets() const;
		const Preset& getCurrentPreset() const;
		const Preset& getPreset(int index) const;
		int getCurrentPresetIndex() const;
		int getPresetCount() const;
		std::vector<std::string> getPresetNames() const;
		std::string getNamesFormatted() const;

		//setters
		void addPreset(const Preset& p);
		void removePreset(int index);
		void nextPreset();
		void prevPreset();
		void setPresetIndex(int index);
		void applyPreset(const ParticleSystem::Preset& preset);

		//File
		bool loadFromFile();
		bool saveToFile();
		void createDefaultPresets();
	};
}

