#pragma once

#include"particle.h"
namespace ParticleSystem
{
	static Mesh circleMesh;
	static Mesh squareMesh;
	static Mesh triangleMesh;

	static unsigned int instanceVBO;

	class ParticleManager
	{
	private:
		std::vector<ParticleSystem::Particle*> particles;
		std::unordered_set<Particle*> particlesToDelete;
		float bounceFactor = 0.8f;

	public:
	//getters

		const std::vector<ParticleSystem::Particle*>& getParticles() const;
		bool getIsMeantForDeletion(Particle* p);

	//setters

		void addParticle(ParticleSystem::Particle* p);
		void removeParticle(ParticleSystem::Particle* p);
		void batchRemoveParticles();

	//misc

		void createParticle(ParticleSystem::Particle p);
		void createParticle(ParticleSystem::Particle* p);
		void createParticle(ParticleShape s, int side = 1, float lifetime = 1.0f, Color c = WHITE, Vector2 pos = { 0,0 }, Vector2 vel = { 0,0 }, CollissionAlgo response = BOUNCE);
		void update(float dt);
		void drawInstancedMesh(Mesh& mesh, std::vector<ParticleInstanceData>& instances);
		void draw();
		void clean();

	//collission responses

		void convertParticle(Particle* from, Particle* to);
		void absorbParticle(Particle* absorber, Particle* absorbed);
		void BounceParticles(Particle* a, Particle* b, Vector2 normal);
		void BounceCC(Particle* c1, Particle* c2, Vector2 normal);
		void BounceSS(Particle* s1, Particle* s2, Vector2 normal);
		void BounceTT(Particle* t1, Particle* t2, Vector2 normal);
		void BounceCT(Particle* c, Particle* t, Vector2 normal);
		void BounceCS(Particle* c, Particle* s, Vector2 normal);
		void BounceTS(Particle* t, Particle* s, Vector2 normal);
		void stickParticles(Particle* a, Particle* b);
		void StickCC(Particle* c1, Particle* c2);
		void StickSS(Particle* s1, Particle* s2);
		void StickTT(Particle* t1, Particle* t2);
		void StickCT(Particle* c, Particle* t);
		void StickCS(Particle* c, Particle* s);
		void StickTS(Particle* t, Particle* s);
		void repelParticles(Particle* a, Particle* b, Vector2 normal);

	//collission
		//circle-circle
		bool checkCollissionCC(ParticleSystem::Particle* c1, ParticleSystem::Particle* c2);
		void resolveCC(ParticleSystem::Particle* c1, ParticleSystem::Particle* c2);
		//circle-square
		bool checkCollissionCS(ParticleSystem::Particle* c, ParticleSystem::Particle* s);
		void resolveCS(ParticleSystem::Particle* c, ParticleSystem::Particle* s);
		//circle-triangle
		bool checkCollissionCT(ParticleSystem::Particle* c, ParticleSystem::Particle* t);
		void resolveCT(ParticleSystem::Particle* c, ParticleSystem::Particle* t);
		//triangle-triangle
		bool checkCollissionTT(ParticleSystem::Particle* t1, ParticleSystem::Particle* t2);
		void resolveTT(ParticleSystem::Particle* t1, ParticleSystem::Particle* t2);
		//triangle-square
		bool checkCollissionTS(ParticleSystem::Particle* t, ParticleSystem::Particle* s);
		void resolveTS(ParticleSystem::Particle* t, ParticleSystem::Particle* s);
		//square-square
		bool checkCollissionSS(ParticleSystem::Particle* s1, ParticleSystem::Particle* s2);
		void resolveSS(ParticleSystem::Particle* s1, ParticleSystem::Particle* s2);

		bool checkParticleCollission(ParticleSystem::Particle* a, ParticleSystem::Particle* b);
		void resolveParticlePositions(ParticleSystem::Particle* a, ParticleSystem::Particle* b);
		void resolveParticleCollission(ParticleSystem::Particle* a, ParticleSystem::Particle* b);

	//constructor
		ParticleManager();
	};
}



