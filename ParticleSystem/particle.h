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

	enum ParticleShape
	{
		CIRCLE,
		SQUARE,
		TRIANGLE
	};

	class Particle
	{
	private:
		Vector2 position = { 0,0 };
		Vector2 velocity = { 0,0 };
		ParticleShape shape = CIRCLE;
		int side = 1.0f;
		Color color = WHITE;
		float lifetime = 1.0f;
		float timeSinceLifeBegan = 0.0f;
		CollissionAlgo collissionResponse = BOUNCE;
		Vector2 v1 = { 0,0 }, v2 = { 0,0 }, v3 = { 0,0 };
		Vector2 n1 = { 0,0 }, n2 = { 0,0 }, n3 = { 0,0 };
	
	public:
	//getters
		Vector2 getPos() const;
		Vector2 getVelocity() const;
		int getSide() const;
		ParticleShape getShape() const;
		Color getColor() const;
		float getLifetime() const;
		float getTimeSinceLifeBegan() const;
		CollissionAlgo getCollissionResponse() const;
		Vector2 getTriangleVertex(int num) const;
		Vector2 getTriangleNormal(int num) const;

	//setters
		void setPos(Vector2 p);
		void setVelocity(Vector2 v);
		void setSide(int r);
		void setShape(ParticleShape s);
		void setColor(Color c);
		void setLifetime(int t);
		void setCollissionResponse(CollissionAlgo c);

	//misc
		void update(float dt);
		void draw();
		void resetParticle();
		void updateTriangleGeometry();

		//s = { CIRCLE, SQUARE, TRIANGLE }
		//a = { RADIUS, SIDE,   SIDE }
		// response = { DESTROY, CONVERT, ABSORB, BOUNCE, REPEL, STICK, FLOW }
		//t = lifetime
		Particle(ParticleShape s, int a, float t, Color c, Vector2 pos, Vector2 vel, CollissionAlgo response);
	};
}

