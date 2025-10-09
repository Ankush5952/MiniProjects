#pragma once

#include"includes.h"
namespace ParticleSystem
{
	enum CollissionAlgo
	{
		DESTROY,
		CONVERT,
		ABSORB,
		BOUNCE,
		REPEL,
		STICK,
		FLOW
	};
	class Particle
	{
	private:
		Vector2 position = { 0,0 };
		Vector2 velocity = { 0,0 };
		int radius = 1.0f;
		Color color = WHITE;
		float lifetime = 1.0f;
		float timeSinceLifeBegan = 0.0f;
		CollissionAlgo collissionResponse = BOUNCE;
	
	public:
	//getters
		Vector2 getPos() const;
		Vector2 getVelocity() const;
		int getRadius() const;
		Color getColor() const;
		float getLifetime() const;
		float getTimeSinceLifeBegan() const;
		CollissionAlgo getCollissionResponse() const;

	//setters
		void setPos(Vector2 p);
		void setVelocity(Vector2 v);
		void setRadius(int r);
		void setColor(Color c);
		void setLifetime(int t);
		void setCollissionResponse(CollissionAlgo c);

	//misc
		void update(float dt);
		void draw();
		void resetParticle();

		Particle(int r, float t, Color c, Vector2 pos, Vector2 vel, CollissionAlgo response);
	};
}

