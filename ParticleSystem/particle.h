#pragma once

#include"includes.h"
namespace ParticleSystem
{
	class Particle
	{
	private:
		Vector2 position = { 0,0 };
		Vector2 velocity = { 0,0 };
		int radius = 1.0f;
		Color color = WHITE;
		float lifetime = 1.0f;
		float timeSinceLifeBegan = 0.0f;
	
	public:
	//getters
		Vector2 getPos();
		Vector2 getVelocity();
		int getRadius();
		float getLifetime();
		float getTimeSinceLifeBegan();

	//setters
		void setPos(Vector2 p);
		void setVelocity(Vector2 v);
		void setRadius(int r);
		void setLifetime(int t);

	//misc
		void update(float dt);
		void draw();
		void resetParticle();

		Particle(int r, float t, Color c, Vector2 pos, Vector2 vel);
	};
}

