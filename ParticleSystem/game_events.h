#pragma once
#include"event.h"

class GameEvents
{
public: //Define Events here
	//Particle Events
	Event<int> onParticleCreated; //particle count
	Event<int> onParticleDestroyed; //particle count
	Event<int> onParticleCountChanged; //particle count

	//Collission Events


	//Preset Events
	Event<std::string> onPresetChanged; //preset name
	Event<std::string> onPresetApplied; //preset name

	//Window Events
	Event<> onWindowResized;
	Event<bool> onFullscreenToggled; //enabled

	//Physics Events
	Event<bool, float, float> onGravityChanged; //enabled, gx, gy
	Event<bool> onBoundaryToggled; //enabled

	//UI Events
	Event<bool> onUIToggled; //enabled
	Event<bool> onGridToggled; //enabled
	Event<bool> onFadeToggled; //enabled

	//System Events
	Event<> onParticlesCleaned; 

	//Performance Events


	//Singleton
	static GameEvents& get()
	{
		static GameEvents instance;
		return instance;
	}
private:

	GameEvents() = default;
	GameEvents(const GameEvents&) = delete; //delete copy constructor
	GameEvents& operator= (const GameEvents&) = delete; //delete copy assignment operator
	GameEvents(GameEvents&&) = delete; //delete move constructor
	GameEvents& operator= (GameEvents&&) = delete; //delete move assignment operator
};
