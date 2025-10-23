#pragma once

#include"includes/includes.h"
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
	static const char* collissions[] = { "DESTROY", "CONVERT", "ABSORB", "BOUNCE", "REPEL", "STICK", "FLOW" };

	enum ParticleShape
	{
		CIRCLE,
		SQUARE,
		TRIANGLE
	};
	static const char* shapes[] = { "CIRCLE", "SQUARE", "TRIANGLE" };

	struct ParticleInstanceData
	{
		Vector2 position;
		float scale;
		Color color;
		float lifetime;
		float timeSinceLifeBegan;

		//ParticleInstanceData(Vector2 pos = { 0,0 }, float s = 1.0f, Color c = WHITE, float l = -1.0f, float t = 0) position(pos), scale(s), color(c), lifetime(l), timeSinceLifeBegan(t) {};
	};

	class Particle
	{
	private:
	//Vars

		Vector2 position = { 0,0 };
		Vector2 velocity = { 0,0 };
		ParticleShape shape = CIRCLE;
		float side = 1.0f;
		Color color = WHITE;
		float lifetime = 1.0f;
		float timeSinceLifeBegan = 0.0f;
		CollissionAlgo collissionResponse = BOUNCE;
		Vector2 v1 = { 0,0 }, v2 = { 0,0 }, v3 = { 0,0 };
		Vector2 n1 = { 0,0 }, n2 = { 0,0 }, n3 = { 0,0 };

		std::deque<Vector2> trail;
		
		static constexpr float MIN_DIST_TRAIL = 16.0f;
	
	public:
	//getters
		Vector2 getPos() const;
		Vector2 getVelocity() const;
		std::deque<Vector2>& getTrail();
		float getSide() const;
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
		void updateTrail();
		void update(float dt);
		void drawTrail();
		void drawParticle();
		void resetParticle();
		void updateTriangleGeometry();

	//Meshes
		//call once per program - creates default meshes that can be instantiated.
		static Mesh generateCircleMesh(int segments = 36);
		static Mesh generateSquareMesh();
		static Mesh generateTriangleMesh();

	//Shaders

		static void loadShader();
		static void unloadShader();

		//s = { CIRCLE, SQUARE, TRIANGLE }
		//a = { RADIUS, SIDE,   SIDE }
		// response = { DESTROY, CONVERT, ABSORB, BOUNCE, REPEL, STICK, FLOW }
		//t = lifetime
		Particle(ParticleShape s, int a, float t, Color c, Vector2 pos, Vector2 vel, CollissionAlgo response);
		Particle() = default;
	};
}

